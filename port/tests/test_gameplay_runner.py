import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

spec = importlib.util.spec_from_file_location('gameplay', Path(__file__).resolve().parents[1]/'gameplay_tests.py')
g = importlib.util.module_from_spec(spec)
spec.loader.exec_module(g)

class RunnerTests(unittest.TestCase):
    def test_missed_action_is_failure(self):
        with self.assertRaises(AssertionError):
            g.check({'scene':'ea','active_slots':[1,2]}, {'active_slots':[18,19]})
    def test_ended_session_is_failure(self):
        with self.assertRaises(AssertionError):
            g.check({'session_ended':True}, {})
    def test_state_change_detected(self):
        with self.assertRaises(AssertionError):
            g.compare({'variable_pool':[1]}, {'variable_pool':[2]}, ['variable_pool'])
    def test_missing_original_is_not_pass(self):
        self.assertEqual(g.compare_original(None,'ea',None,'e','a','s')['status'], 'UNVERIFIED')
    def test_forged_original_rejected(self):
        with tempfile.TemporaryDirectory() as d:
            p=Path(d);(p/'ea.json').write_text(json.dumps({'producer':'port'}))
            with self.assertRaises(AssertionError):
                g.compare_original(p,'ea',p,'e','a','s')
    def test_incomplete_run_and_nonzero_exit(self):
        import sys, os
        with tempfile.TemporaryDirectory() as d:
            for code in ['print("hello")', 'raise SystemExit(2)']:
                with self.assertRaises(AssertionError):
                    g.execute([sys.executable,'-c',code],Path(d)/'log',5,os.environ.copy())
    def test_original_pixels_and_coverage(self):
        with tempfile.TemporaryDirectory() as d:
            p=Path(d);refs=p/'refs';refs.mkdir();shots=p/'shots';shots.mkdir()
            raw=b'P6\n640 480\n255\n'+bytes(640*480*3)
            (refs/'first.ppm').write_bytes(raw);(shots/'first.ppm').write_bytes(raw)
            (shots/'first.json').write_text('{}')
            manifest={'producer':'original-runtime','exe_sha256':'e','script_sha256':'a',
                      'save_sha256':'s','checkpoints':{'first':{'image':'first.ppm',
                      'sha256':g.digest(refs/'first.ppm')}}}
            (refs/'ea.json').write_text(json.dumps(manifest))
            self.assertEqual(g.compare_original(refs,'ea',shots,'e','a','s')['status'],'PASS')
            (shots/'first.ppm').write_bytes(raw[:15]+bytes([255])*(640*480*3))
            with self.assertRaises(AssertionError):
                g.compare_original(refs,'ea',shots,'e','a','s')
            (shots/'second.json').write_text('{}')
            with self.assertRaisesRegex(AssertionError,'every script checkpoint'):
                g.compare_original(refs,'ea',shots,'e','a','s')
    def test_missing_state_is_failure(self):
        with self.assertRaises(AssertionError):
            g.compare({}, {}, ['variable_pool'])
    def test_timeout(self):
        import sys, os
        with tempfile.TemporaryDirectory() as d:
            with self.assertRaisesRegex(AssertionError,'timed out'):
                g.execute([sys.executable,'-c','import time;time.sleep(3)'],Path(d)/'log',0.05,os.environ.copy())

if __name__ == '__main__': unittest.main()
