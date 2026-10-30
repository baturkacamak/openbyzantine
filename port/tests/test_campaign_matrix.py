import contextlib
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest

test_dir=Path(__file__).resolve().parent
original_path=sys.path[:]
sys.path[:]=[value for value in sys.path if Path(value or '.').resolve()!=test_dir]
sys.path.insert(0,str(test_dir.parent))
from unittest.mock import patch
sys.path[:]=original_path
sys.path.insert(0,str(test_dir.parent))
import explore_campaign as campaign


class CampaignTests(unittest.TestCase):
    def test_all_profiles_run_and_error_is_not_hidden(self):
        with tempfile.TemporaryDirectory() as directory:
            output=Path(directory)/'campaign';commands=[]
            def fake_run(command,folder,timeout):
                commands.append(command)
                target=Path(command[command.index('--output')+1]);target.mkdir()
                broken=folder.name=='logic'
                report={'status':'FINDINGS' if broken else 'PASS','runs':2,
                        'findings':[{'kind':'synthetic','severity':'error','reason':'fixture'}] if broken else [],
                        'media_playbacks':1 if '--with-media' in command else 0}
                (target/'report.json').write_text(json.dumps(report));return 1 if broken else 0
            with patch.object(sys,'argv',['campaign','10','--output',str(output),'--random-seed','4']),patch.object(campaign,'run_process',fake_run),contextlib.redirect_stdout(io.StringIO()):
                code=campaign.main()
            r=json.loads((output/'report.json').read_text())
            self.assertEqual(code,1);self.assertEqual(len(commands),7)
            self.assertEqual(r['runs'],14);self.assertEqual(r['findings'][0]['phase'],'logic')
            self.assertEqual(sum('--with-media' in command for command in commands),3)
            self.assertTrue((output/'index.html').is_file())
            self.assertEqual(r['logic_run_limit'],10)
    def test_review_is_reported_and_does_not_become_an_error(self):
        with tempfile.TemporaryDirectory() as directory:
            output=Path(directory)/'campaign'
            def fake_run(command,folder,timeout):
                target=Path(command[command.index('--output')+1]);target.mkdir()
                (target/'report.json').write_text(json.dumps({'status':'FINDINGS','runs':1,'findings':[{'severity':'review','reason':'asset fixture','kind':'media_unavailable'}]}));return 0
            with patch.object(sys,'argv',['campaign','2','--output',str(output)]),patch.object(campaign,'run_process',fake_run),contextlib.redirect_stdout(io.StringIO()):code=campaign.main()
            self.assertEqual(code,0);self.assertEqual(json.loads((output/'report.json').read_text())['status'],'FINDINGS')
    @unittest.skipUnless(os.name=='posix','process group cleanup')
    def test_wall_timeout_stops_the_owned_phase(self):
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory)
            with contextlib.redirect_stdout(io.StringIO()):
                with self.assertRaises(TimeoutError):
                    campaign.run_process([sys.executable,'-u','-c','import time;print("started");time.sleep(30)'],root,.1)
            self.assertIn('started',(root/'runner.log').read_text())


if __name__=='__main__':unittest.main()
