import copy
import json
from pathlib import Path
import sys
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from test_stories import validate_pack,evaluate,milestones,suffix_assertions


class StoryContractTests(unittest.TestCase):
    def setUp(self):
        self.pack=json.loads((Path(__file__).parent/'exploration/story-contracts.json').read_text())

    def test_declared_story_goals_have_observable_intermediate_and_final_checks(self):
        validate_pack(self.pack)
        self.assertEqual(len(self.pack['cases']),9)
        self.assertFalse(self.pack['complete_playthrough_verified'])
        self.assertTrue(self.pack['uncovered'])
        self.assertEqual(sum(c['resume_after'] is not None for c in self.pack['cases']),8)

    def test_missing_final_assertion_and_out_of_range_resume_are_rejected(self):
        for mode in ['checks','resume']:
            pack=copy.deepcopy(self.pack);case=pack['cases'][0]
            if mode=='checks':case['checks']=[c for c in case['checks'] if c['at']!=len(case['actions'])]
            else:case['resume_after']=len(case['actions'])
            with self.assertRaises(ValueError):validate_pack(pack)

    def test_successful_exit_is_not_enough_to_pass_a_story(self):
        checks=evaluate([dict(scene='vc')],[dict(at=0,path=['scene'],op='eq',value='vh'),
            dict(at=1,path=['inventory','109',0],op='eq',value=1)])
        self.assertFalse(any(c['passed'] for c in checks))
        case=dict(milestones=[dict(name='diadem',at=1),dict(name='missing',at=2)])
        self.assertFalse(any(m['passed'] for m in milestones(case,checks)))

    def test_fresh_process_assertions_keep_the_saved_boundary_and_remaining_goals(self):
        case=copy.deepcopy(self.pack['cases'][0]);cut=case['resume_after']
        checks=suffix_assertions(case,cut)
        self.assertEqual(checks[0]['at'],0)
        self.assertEqual(checks[-1]['at'],len(case['actions'])-cut)
        case['checks']=[dict(at=2,path=['scene'],op='equal_checkpoint',other_at=0,other_path=['scene'])]
        with self.assertRaises(ValueError):suffix_assertions(case,cut)


if __name__=='__main__':unittest.main()
