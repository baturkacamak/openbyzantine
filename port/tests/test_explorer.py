import copy
import importlib.util
import json
import os
from pathlib import Path
from types import SimpleNamespace
import sys
import tempfile
import time
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import explore_game as e


def state():
    result={key:{} for key in e.STATE_FIELDS}
    result.update(scene='ea',background='room',layer=2,media_disc=2,session_ended=False,
                  menu=False,settings=False,panorama=[180,0],selected=None,active_slots=[1],
                  variable_pool=[0,1],effective_pool=[0,1],variables={'0':1},inventory={'17':[1,1]},
                  channels=[[17]],channel=0,random_state=0xbeef,morris={'initialized':False,'turns':0,'nodes':[]},
                  inventory_ui={'visible':True,'scroll':0,'return_armed':False,'parent_scene':'','using':None},
                  exploration={'schema':1,'record_identity':{},'record_callbacks':{},'hover_preview':False,'blocker':'',
                               'script_end':False,'scene_switch_pending':False,'decisions':[],
                               'candidates':[{'id':'turn_left','category':'camera','commands':['key Left']}]})
    return result


def report():
    return {'status':'RUNNING','states':[],'edges':[],'findings':[],'trials':[],'save_checks':[],
            'duplicate_states':0,'deferred_frontier':0,'deferred_actions':0,'depth_frontier':0,'dialogue_observations':[]}


class ExplorerTests(unittest.TestCase):
    def test_action_contract(self):
        self.assertEqual(e.Action.read({'id':'slot1','category':'hotspot','commands':['click 0 479']}).id,'slot1')
        for commands in (['run-label 3'],['click 640 3'],['key F5'],['snapshot ../file'],['wait 6001'],[]):
            with self.assertRaises(ValueError):e.Action.read({'id':'a','category':'hotspot','commands':commands})
    def test_state_abstraction(self):
        first=state();second=copy.deepcopy(first)
        second['exploration']['decisions']=[{'choice':1}]
        self.assertEqual(e.fingerprint(first),e.fingerprint(second))
        second['selected']=17
        self.assertNotEqual(e.fingerprint(first),e.fingerprint(second))
        second=copy.deepcopy(first);second['panorama'][0]=-180
        self.assertEqual(e.fingerprint(first),e.fingerprint(second))
        second['random_state']=1
        self.assertNotEqual(e.fingerprint(first),e.fingerprint(second))
    def test_progress_ignores_view_and_inventory_selection(self):
        a=state();b=copy.deepcopy(a)
        b.update(panorama=[90,20],background='closeup',layer=5,selected=17,
                 channel=3,random_state=123)
        b['inventory_ui'].update(scroll=4,using=17,visible=False)
        b['inventory']['17']=[4,0]
        self.assertEqual(e.progress_fingerprint(a),e.progress_fingerprint(b))
        self.assertNotEqual(e.fingerprint(a),e.fingerprint(b))
        b['inventory']['17']=[3,1]
        self.assertEqual(e.progress_fingerprint(a),e.progress_fingerprint(b))
    def test_pending_script_callbacks_are_not_merged_with_idle_or_other_goals(self):
        a=state();b=copy.deepcopy(a)
        a['exploration']['record_callbacks']={'4':[1,2,4294967295,[[170,5]]]}
        b['exploration']['record_callbacks']={'4':[2,1,4294967295,[[170,5]]]}
        self.assertNotEqual(e.fingerprint(a),e.fingerprint(b))
        self.assertEqual(e.progress_fingerprint(a),e.progress_fingerprint(b))
        b['exploration']['record_callbacks']['4'][3]=[[173,5]]
        self.assertNotEqual(e.progress_fingerprint(a),e.progress_fingerprint(b))
        b['exploration']['record_callbacks']={}
        self.assertNotEqual(e.fingerprint(a),e.fingerprint(b))
        del b['exploration']['record_callbacks']
        with self.assertRaises(ValueError):e.fingerprint(b)
    def test_progress_retains_unknown_variables_ownership_dialogue_and_gates(self):
        a=state()
        for field,value in [('effective_pool',[0,2]),('channels',[[]]),
                            ('conversation',{'4':{'done':True}}),
                            ('enabled',{'1':False}),('scene','mr')]:
            b=copy.deepcopy(a);b[field]=value
            self.assertNotEqual(e.progress_fingerprint(a),e.progress_fingerprint(b),field)
        a['objects']={'1':[10,20,30,40,True,23,7,0,0]}
        b=copy.deepcopy(a);b['objects']['1'][0]=90
        self.assertEqual(e.progress_fingerprint(a),e.progress_fingerprint(b))
        b['objects']['1'][4]=False
        self.assertNotEqual(e.progress_fingerprint(a),e.progress_fingerprint(b))
    def test_priority_is_seeded_soft_preference_not_pruning(self):
        from collections import deque
        import random
        a=e.Action('a','camera',['key Left'])
        preferred=e.Plan('seed',[a],progress_priority=True)
        other=e.Plan('seed',[a,e.Action('b','camera',['key Right'])])
        count=0
        for seed in range(400):
            queue=deque([preferred,other])
            first=e.pop_frontier(queue,'random',random.Random(seed))
            count+=first is preferred
            self.assertEqual(len(queue),1)
            self.assertIsNot(e.pop_frontier(queue,'random',random.Random(seed)),first)
        self.assertGreater(count,300);self.assertLess(count,390)
        self.assertEqual(preferred.key(),e.Plan('seed',[a]).key())
    def test_equal_progress_detailed_states_still_expand(self):
        class Runner:
            def __init__(self,output):
                self.output=output;self.runs=0;self.deadline=time.monotonic()+30
                self.seeds={'seed':{}}
                self.args=SimpleNamespace(max_frontier=8,max_decisions=8,max_states=10,
                    max_depth=3,max_actions=8,save_every=0,max_runs=4)
            def capacity(self):return self.runs<4
            def run(self,plan):
                self.runs+=1;states=[]
                for depth in range(len(plan.actions)+1):
                    current=state();current['panorama']=[180+depth*10,0];states.append(current)
                return e.Trial(self.output,plan,states,{})
        with tempfile.TemporaryDirectory() as directory:
            r=report();e.explore(Runner(Path(directory)),r)
            self.assertEqual(len(r['states']),4)
            self.assertEqual(len(r['progress_states']),1)
            self.assertEqual(r['interaction_only_changes'],3)
            self.assertEqual(r['progress_changes'],0)

    def test_persistence_ignores_inventory_cache_but_keeps_ownership(self):
        a=state();b=copy.deepcopy(a);b['inventory']['17'][1]=0
        self.assertEqual(e.persistence_projection(a),e.persistence_projection(b))
        b['inventory']['17'][0]=0
        self.assertNotEqual(e.persistence_projection(a),e.persistence_projection(b))
    def test_morris_board_is_part_of_both_state_keys(self):
        a=state();b=copy.deepcopy(a);b['morris']['turns']=1
        self.assertNotEqual(e.fingerprint(a),e.fingerprint(b))
        self.assertNotEqual(e.progress_fingerprint(a),e.progress_fingerprint(b))
    def test_clickability_is_part_of_state(self):
        a=state();b=copy.deepcopy(a);b['active_slots']=[]
        self.assertNotEqual(e.fingerprint(a),e.fingerprint(b))
    def test_closeup_parent_is_part_of_state(self):
        a=state();b=copy.deepcopy(a)
        a['exploration']['return_context']={'scene':'ea','background':'desk'}
        b['exploration']['return_context']={'scene':'ea','background':'door'}
        self.assertNotEqual(e.fingerprint(a),e.fingerprint(b))
    def test_random_frontier_is_seeded_and_not_bfs(self):
        from collections import deque
        import random
        actions=[e.Action(str(i),'hotspot',[f'click {i} 20']) for i in range(10)]
        def order(seed):
            queue=deque(e.Plan('seed',[a]) for a in actions);rng=random.Random(seed)
            result=[]
            while queue:result.append(e.pop_frontier(queue,'random',rng).actions[0].id)
            return result
        self.assertEqual(order(1234),order(1234))
        self.assertNotEqual(order(1234),order(4321))
        self.assertEqual(set(order(1234)),{str(i) for i in range(10)})
    def test_random_search_balances_starting_contexts(self):
        from collections import deque
        import random
        action=e.Action('a','camera',['key Left'])
        count=0
        for seed in range(300):
            queue=deque([e.Plan('busy',[action]) for _ in range(100)]+[e.Plan('quiet',[action])])
            if e.pop_frontier(queue,'random',random.Random(seed)).seed=='quiet':count+=1
        self.assertGreater(count,90)
        self.assertLess(count,210)
    def test_random_search_initializes_all_seeds_first(self):
        from collections import deque
        import random
        child=e.Plan('a',[e.Action('child','camera',['key Left'])])
        queue=deque([child,e.Plan('b'),e.Plan('c')])
        rng=random.Random(99)
        self.assertEqual(e.pop_frontier(queue,'random',rng).seed,'b')
        self.assertEqual(e.pop_frontier(queue,'random',rng).seed,'c')
        self.assertEqual(e.pop_frontier(queue,'random',rng).actions[0].id,'child')
    def test_random_candidate_selection_is_reproducible_without_mutation(self):
        import random
        candidates=[e.Action(str(i),'hotspot',[f'click {i} 20']) for i in range(12)]
        first=e.select_candidates(candidates,4,'random',random.Random(5))
        again=e.select_candidates(candidates,4,'random',random.Random(5))
        self.assertEqual([a.id for a in first],[a.id for a in again])
        self.assertEqual([a.id for a in candidates],[str(i) for i in range(12)])
        self.assertEqual(e.select_candidates(candidates,4,'bfs',random.Random(5)),candidates[:4])
    def test_save_catalogue_balances_scenes_and_records_skips(self):
        from explore_seeds import collect_save_seeds
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);folder=root/'copy2hd/savegame';folder.mkdir(parents=True)
            for name,value in [('25.btb',b'a'),('26.btb',b'b'),('27.btb',b'c'),
                               ('28.btb',b'a'),('29.btb',b'bad'),('quicksave.btb',b'user')]:
                (folder/name).write_bytes(value)
            def read(path):
                if path.name=='29.btb':raise ValueError('invalid fixture')
                return {'scene':'MAP' if path.name!='27.btb' else 'EA','disc':2}
            seeds,catalogue=collect_save_seeds(root,read)
            self.assertEqual(catalogue['candidate_count'],5)
            self.assertEqual(catalogue['seed_count'],3)
            self.assertEqual([s['scene'] for s in seeds],['EA','MAP','MAP'])
            self.assertTrue(all(s['restore'] and not s.get('entry') for s in seeds))
            self.assertEqual({s['reason'] for s in catalogue['skipped']},{'duplicate bytes','invalid fixture'})
    def test_save_metadata_does_not_need_a_guessed_label(self):
        from explore_seeds import parse_save_summary
        text='cd=4 chapter=show description=loaded\nscene_index=7 chapter_scenes=44 scene=ea current_scene=4294967295\n'
        self.assertEqual(parse_save_summary(text),{'scene':'EA','disc':4})
        with self.assertRaises(ValueError):parse_save_summary(text.replace('scene_index=7','scene_index=99'))
    def test_missing_state_not_deduplicated(self):
        with self.assertRaises(ValueError):e.fingerprint({'scene':'ea'})
    def test_answer_branches_stop_at_trigger(self):
        s=state();s['exploration']['decisions']=[
            {'action':1,'choice':0,'lines':[10,11]}, {'action':4,'choice':1,'lines':[20,21,22]}]
        actions=[e.Action('a','hotspot',['click 1 2']) for _ in range(3)]
        trial=e.Trial(Path('.'),e.Plan('seed',actions,[],['root','one','two']),[s],{0:0,1:1,4:2})
        branches=e.decision_branches(trial,8)
        self.assertEqual([(len(p.actions),p.choices) for p in branches],[(1,[1]),(2,[0,0]),(2,[0,2])])
        self.assertEqual(branches[0].expected,['root'])
        self.assertEqual(len(e.decision_branches(trial,1)),1)
    def test_startup_answer_branch(self):
        s=state();s['exploration']['decisions']=[{'action':0,'choice':0,'lines':[1,2]}]
        trial=e.Trial(Path('.'),e.Plan('seed'),[s],{0:0})
        branch=e.decision_branches(trial,10)[0]
        self.assertEqual(branch.actions,[]);self.assertEqual(branch.choices,[1])
    def test_probe_is_not_a_failed_action(self):
        log='probe stopped: unsupported_opcode=0xff\nautomation_complete actions=2\n'
        self.assertIsNone(e.classify(log,0,False,[state()]))
        self.assertEqual(e.classify('click at 1,2 slot 1 stopped: unsupported_opcode=0xff\n'+log,0,False,[state()]).kind,'blocked_action')
    def test_incomplete_crash_and_timeout(self):
        self.assertEqual(e.classify('',0,False,[]).kind,'incomplete_script')
        self.assertEqual(e.classify('error: test',2,False,[]).severity,'error')
        self.assertEqual(e.classify('',-9,True,[]).severity,'review')
        self.assertEqual(e.classify('exploration decision budget exhausted',1,False,[]).kind,'decision_budget')
    def test_no_hotspots_is_not_immediately_a_softlock(self):
        s=state();s['active_slots']=[]
        s['exploration']['candidates']=[{'id':'wait','category':'wait','commands':['wait 1000']}]
        wait=e.Action('wait','wait',['wait 1000'])
        self.assertIsNone(e.wait_issue(s,None))
        self.assertEqual(e.wait_issue(s,wait).severity,'review')
        s['session_ended']=True;self.assertIsNone(e.wait_issue(s,wait))
        s['session_ended']=False;s['exploration']['script_end']=True
        self.assertIsNone(e.wait_issue(s,wait))
        s['exploration']['script_end']=False;s['exploration']['scene_switch_pending']=True
        self.assertIsNone(e.wait_issue(s,wait))
    def test_saved_contract_detects_difference(self):
        a=state();b=copy.deepcopy(a);b['effective_pool']=[1,2]
        self.assertEqual(e.differences(a,b,e.SAVE_FIELDS),['effective_pool'])
    def test_timeout_kills_owned_process_group(self):
        with tempfile.TemporaryDirectory() as directory:
            code,expired=e.execute([sys.executable,'-c','import time;time.sleep(10)'],Path(directory),.05)
            self.assertTrue(expired);self.assertNotEqual(code,0)
    def test_report_escapes_text_and_tolerates_partial_images(self):
        with tempfile.TemporaryDirectory() as directory:
            output=Path(directory);folder=output/'F0001';(folder/'checkpoints').mkdir(parents=True)
            (folder/'checkpoints/p0000.ppm').write_bytes(b'P6\n640 480\n255\n')
            r=report();r['findings']=[dict(kind='test',reason='<script>bad</script>',scene='ea',severity='review',bundle=str(folder))]
            e.render_report(output,r)
            text=(output/'index.html').read_text()
            self.assertNotIn('<script>',text);self.assertIn('&lt;script&gt;',text)
            self.assertIn('tamamlandığını kanıtlamaz',text)
    def test_deduplication_and_budgets(self):
        class FakeRunner:
            def __init__(self,output,max_runs=5,max_states=10):
                self.output=output;self.runs=0;self.deadline=time.monotonic()+30
                self.seeds={'seed':{}}
                self.args=SimpleNamespace(max_frontier=4,max_decisions=8,max_states=max_states,
                    max_depth=3,max_actions=8,save_every=0,max_runs=max_runs)
            def capacity(self):return self.runs<self.args.max_runs
            def run(self,plan):
                self.runs+=1
                return e.Trial(self.output,plan,[state() for _ in range(len(plan.actions)+1)],{})
        with tempfile.TemporaryDirectory() as directory:
            output=Path(directory);r=report();runner=FakeRunner(output)
            e.explore(runner,r)
            self.assertEqual(len(r['states']),1);self.assertEqual(r['duplicate_states'],1)
            self.assertEqual(r['stop_reason'],'frontier_exhausted_within_abstraction')
        with tempfile.TemporaryDirectory() as directory:
            output=Path(directory);r=report();e.explore(FakeRunner(output,max_runs=1),r)
            self.assertEqual(r['stop_reason'],'max_runs');self.assertEqual(r['frontier'],1)
        with tempfile.TemporaryDirectory() as directory:
            output=Path(directory);r=report();e.explore(FakeRunner(output,max_states=1),r)
            self.assertEqual(r['stop_reason'],'max_states')

if __name__=='__main__':unittest.main()
