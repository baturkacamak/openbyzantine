import copy
from collections import deque
import random
from pathlib import Path
import tempfile
import time
from types import SimpleNamespace
import unittest

from test_explorer import state,report
import explore_game as e
from explore_policy import MixedPolicy,action_key
from behavior_checks import check_assertion,transition_checks,media_evidence


class MixedPolicyTests(unittest.TestCase):
    def action(self,name='a',category='hotspot'):
        return e.Action(name,category,['key Left'] if category=='camera' else ['click 20 30'])
    def test_root_coverage_and_fixed_lane_share(self):
        policy=MixedPolicy(random.Random(1))
        queue=deque([e.Plan('a',[self.action()]),e.Plan('b')])
        self.assertEqual(policy.pop(queue).seed,'b')
        for i in range(40):
            queue.append(e.Plan('a',[self.action()]*(i%8+1)))
        for _ in range(40):policy.pop(queue)
        self.assertEqual(policy.lanes,{'root':1,'deep':20,'balanced':10,'uniform':10})
    def test_depth_lane_selects_deepest_without_removing_other_routes(self):
        policy=MixedPolicy(random.Random(2));action=self.action()
        short=e.Plan('a',[action]);long=e.Plan('a',[action]*18)
        queue=deque([short,long])
        self.assertIs(policy.pop(queue),long);self.assertEqual(list(queue),[short])
    def test_deep_lane_rotates_all_available_starts(self):
        action=self.action();policy=MixedPolicy(random.Random(2891583293))
        queue=deque([e.Plan('busy',[action]*24) for _ in range(100)]+
                    [e.Plan('seed'+str(i),[action]) for i in range(48)])
        for _ in range(400):
            plan=policy.pop(queue);queue.append(plan)
        counts=policy.deep_seed_runs
        self.assertEqual(len(counts),49)
        self.assertLessEqual(max(counts.values())-min(counts.values()),1)
        self.assertLess(counts['busy'],10)
    def test_random_intent_is_not_overwhelmed_by_inventory_size(self):
        inventory=self.action('item','inventory');hotspot=self.action();camera=self.action(category='camera')
        counts={category:0 for category in ['inventory','hotspot','camera']}
        for seed in range(300):
            policy=MixedPolicy(random.Random(seed));policy.tick=3
            queue=deque([e.Plan('s',[inventory]) for _ in range(100)]+[e.Plan('s',[hotspot]),e.Plan('s',[camera])])
            counts[policy.pop(queue).actions[-1].category]+=1
        for count in counts.values():self.assertGreater(count,60);self.assertLess(count,140)
    def test_weighted_intent_is_not_multiplied_by_menu_size(self):
        counts={'inventory':0,'hotspot':0}
        for seed in range(400):
            policy=MixedPolicy(random.Random(seed))
            plans=[e.Plan('s',[self.action('item','inventory')]) for _ in range(100)]
            plans.append(e.Plan('s',[self.action()]))
            selected=policy.weighted(list(enumerate(plans)))
            counts[plans[selected].actions[-1].category]+=1
        self.assertGreater(counts['inventory'],40);self.assertLess(counts['inventory'],170)
    def test_travel_intent_uses_observed_object_kind_and_is_not_job_identity(self):
        a=self.action('hotspot_1');s=state();s['objects']={'1':[0,0,10,10,1,0x17,1,0,0]}
        self.assertEqual(e.candidate_goal(a,s),'travel')
        self.assertEqual(e.Plan('s',[a],goal='travel').key(),e.Plan('s',[a]).key())
    def test_repeated_selection_is_discounted_but_available(self):
        policy=MixedPolicy(random.Random(2));action=self.action(category='camera')
        plan=e.Plan('a',[action],origin_progress='p')
        initial=policy.quality(plan)
        for _ in range(8):policy.observe(plan,'p',False)
        self.assertGreater(policy.quality(plan),0);self.assertLess(policy.quality(plan),initial)
        novel=e.Plan('a',[action],origin_progress='new')
        self.assertEqual(policy.quality(novel),initial)
        loop=e.Plan('a',[action]*7,origin_progress='p')
        self.assertLess(policy.quality(loop),policy.quality(plan))
        self.assertEqual(plan.key(),novel.key())
    def test_view_tail_does_not_monopolize_deep_lane_or_merge_item_ids(self):
        policy=MixedPolicy(random.Random(3));camera=self.action(category='camera')
        views=e.Plan('a',[camera]*20)
        progress=e.Plan('a',[self.action()]*8)
        queue=deque([views,progress])
        self.assertIs(policy.pop(queue),progress);self.assertEqual(list(queue),[views])
        self.assertNotEqual(action_key(self.action('item_1','inventory')),
                            action_key(self.action('item_2','inventory')))
    def test_bounded_frontier_is_seed_reproducible(self):
        def order(seed):
            policy=MixedPolicy(random.Random(seed));queue=deque(e.Plan(str(i%3),[self.action(str(i))]*(i%5+1)) for i in range(30))
            return [policy.pop(queue).key() for _ in range(30)]
        self.assertEqual(order(6),order(6));self.assertNotEqual(order(6),order(7))
    def test_mixed_grows_long_chains_in_branched_same_progress_search(self):
        class Runner:
            def __init__(self,output,strategy):
                self.output=output;self.runs=0;self.deadline=time.monotonic()+30;self.seeds={'seed':{}}
                self.args=SimpleNamespace(strategy=strategy,random_seed=5,max_frontier=2000,
                    max_decisions=8,max_states=100,max_depth=24,max_actions=8,save_every=0,max_runs=60)
            def capacity(self):return self.runs<60
            def run(self,plan):
                self.runs+=1;states=[]
                for i in range(len(plan.actions)+1):
                    s=state();s['background']='root/'+'/'.join(a.id for a in plan.actions[:i])
                    s['exploration']['candidates']=[dict(id='h'+str(n),category='hotspot',commands=[f'click {n} 20']) for n in range(8)]
                    states.append(s)
                return e.Trial(self.output,plan,states,{})
        for strategy in ['bfs','mixed']:
            with tempfile.TemporaryDirectory() as directory:
                result=report();e.explore(Runner(Path(directory),strategy),result)
                depth=max(s['depth'] for s in result['states'])
                if strategy=='bfs':self.assertLessEqual(depth,2)
                else:
                    self.assertEqual(depth,24)
                    self.assertEqual(len(result['progress_states']),1)
                    self.assertGreater(result['policy_metrics']['lanes']['uniform'],0)


class BehaviorTests(unittest.TestCase):
    def test_wrong_outcome_is_detected_without_a_crash(self):
        s=state()
        self.assertFalse(check_assertion([s],dict(at=0,path=['scene'],op='eq',value='mr'))['passed'])
        self.assertTrue(check_assertion([s],dict(at=0,path=['active_slots'],op='contains',value=1))['passed'])
    def test_missing_checkpoint_field_or_operator_is_not_a_pass(self):
        for assertion in [dict(at=1,path=['scene'],op='eq',value='ea'),
                          dict(at=0,path=['unknown'],op='nonempty'),
                          dict(at=0,path=['scene'],op='guess')]:
            with self.assertRaises(ValueError):check_assertion([state()],assertion)
    def test_return_contract_catches_wrong_parent(self):
        before=state();before['inventory_ui'].update(return_armed=True,parent_scene='vt')
        after=state();checks=transition_checks(before,after)
        self.assertFalse(checks[0]['passed'])
        after['scene']='vt';self.assertTrue(transition_checks(before,after)[0]['passed'])
    def test_positive_and_negative_board_and_decision_checks(self):
        s=state();s['morris']['nodes']=[[20,6],[21,5]]
        for mask in [1,2]:self.assertTrue(check_assertion([s],dict(at=0,path=['morris','nodes'],op='count_mask_eq',value={'mask':mask,'count':1}))['passed'])
        s['exploration']['decisions']=[dict(scene='am',record=0,group=1,choice=2,lines=[8,9,10])]
        self.assertTrue(check_assertion([s],dict(at=0,path=['exploration','decisions'],op='contains_fields',value={'choice':2,'group':1}))['passed'])
    def test_decode_or_flag_alone_is_not_playback(self):
        evidence=media_evidence('video name=test frames=42 fps=15\n')
        self.assertEqual(evidence['completed_playbacks'],[]);self.assertEqual(evidence['audio_queues'],[])
    def test_completed_media_audio_errors_and_missing_assets_are_separate(self):
        log=('video_audio clip=test wav=audio.wav duration_ms=100 queued_bytes=123 device=dummy\n'
             ' elapsed_ms=100 written=3 events=0 (script continues)\n'
             'conversation record=0 name=test result=65535 lines=2 answer_lists=0 media=2 audio_failures=0 resume_instruction=3\n'
             'conversation_media clip=absent missing (text stays)\n'
             'video_audio clip=broken ERROR queue: device failed\n')
        evidence=media_evidence(log)
        self.assertEqual(len(evidence['completed_playbacks']),2)
        self.assertEqual(len(evidence['audio_queues']),1)
        self.assertEqual(len(evidence['errors']),1);self.assertEqual(len(evidence['unavailable']),1)


if __name__=='__main__':unittest.main()
