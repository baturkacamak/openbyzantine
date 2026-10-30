import copy
import configparser
from pathlib import Path
import random
import sys
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import explore_game as e
from settings_fuzz import initial,ini,operation,expectation,OPERATIONS
from settings_checks import check_settings,validate_profile,check_flow


def snapshot():
    host,prefs=initial(random.Random(1),0)
    host['window_mode']='windowed';host['scale_mode']='fit'
    runtime={'window_mode':'windowed','scale_mode':'fit','logical':[640,480],
        'window':[1280,960],'output':[1280,960],'content_rect':[0,0,1280,960],
        'integer_scale':False,'overlay_enabled':host['hotspots'],'overlay_all':host['hotspots_all'],
        'audio_ready':True,'audio_gains':[host[k] for k in ['master_volume','speech_volume','ambient_volume','effects_volume']],
        'audio_muted':False,'controller_open':False,'text_resolved':'base','voice_resolved':'base',
        'vsync_supported':True,'renderer_flags':9,'last_content':'scene'}
    return {'host_settings':host,'gameprefs':prefs,'host_runtime':runtime,'settings':False,'menu':False}


class SettingsFuzzTests(unittest.TestCase):
    def test_valid_profiles_and_every_operation_is_real_bounded_input(self):
        host,prefs=initial(random.Random(3),0);rng=random.Random(4)
        profile={'schema':1,'ini':ini(host,rng),'gameprefs':prefs,'audio':True}
        self.assertIs(validate_profile(profile),profile)
        config=configparser.ConfigParser();config.read_string(profile['ini'])
        self.assertEqual(config['game']['autosave'],'0')
        for index,name in enumerate(OPERATIONS):
            action=operation(name,host,prefs,rng,index)
            self.assertEqual(action.category,'settings');self.assertLessEqual(len(action.commands),32)
    def test_seed_reproduces_settings_and_does_not_mutate_game_variables(self):
        a=initial(random.Random(77),3);b=initial(random.Random(77),3)
        self.assertEqual(a,b);self.assertNotEqual(a,initial(random.Random(78),3))
        self.assertEqual(len(a[1]),7);self.assertEqual(len(set(a[0][key] for key in a[0] if key.startswith('key_'))),8)
    def test_cannot_inject_vm_commands_or_use_settings_keys_as_scene_discovery(self):
        for command in ['run-label 9','key F5','resize 640 480']:
            with self.assertRaises(ValueError):e.Action.read(dict(id='x',category='hotspot',commands=[command]))
        for command in ['run-label 9','resize 99999 99999','focus somewhere']:
            with self.assertRaises(ValueError):e.Action.read(dict(id='x',category='settings',commands=[command]))
    def test_missing_probe_and_invalid_profile_fail_closed(self):
        with self.assertRaises(ValueError):check_settings({},dict(host={},gameprefs=[]))
        for profile in [dict(schema=1,ini='',gameprefs=[0]*6),dict(schema=1,ini='',gameprefs=[2]*7),dict(schema=1,ini='',gameprefs=[0]*7,save_variables=[1])]:
            with self.assertRaises(ValueError):validate_profile(profile)
    def test_requested_values_are_not_enough_without_effect(self):
        state=snapshot();expected=expectation(0,state['host_settings'],state['gameprefs'])
        self.assertTrue(all(c['passed'] for c in check_settings(state,expected)))
        broken=copy.deepcopy(state);broken['host_runtime']['audio_gains'][0]=1
        self.assertFalse(all(c['passed'] for c in check_settings(broken,expected)))
        broken=copy.deepcopy(state);broken['host_runtime']['overlay_enabled']=True
        self.assertFalse(all(c['passed'] for c in check_settings(broken,expected)))
    def test_old_fixed_image_resize_regression_is_detected(self):
        state=snapshot();expected=expectation(0,state['host_settings'],state['gameprefs'])
        state['host_runtime']['content_rect']=[320,240,640,480]
        failures=[c['id'] for c in check_settings(state,expected) if not c['passed']]
        self.assertIn('viewport.scales_with_window',failures)
    def test_focus_vsync_and_stale_menu_are_observed_independently(self):
        state=snapshot();expected=expectation(0,state['host_settings'],state['gameprefs'],muted=True)
        self.assertIn('focus.muted',[c['id'] for c in check_settings(state,expected) if not c['passed']])
        state['host_runtime']['renderer_flags']=13;expected['muted']=False
        self.assertIn('renderer.vsync',[c['id'] for c in check_settings(state,expected) if not c['passed']])
        state['host_runtime']['last_content']='menu'
        self.assertIn('menu_frame_removed',[c['id'] for c in check_settings(state,expected) if not c['passed']])
    def test_autosave_effect_is_checked_against_the_actual_travel(self):
        before=snapshot();after=copy.deepcopy(before)
        before.update(scene='map',inventory_ui={'return_armed':False},session_ended=False)
        after.update(scene='am',inventory_ui={'return_armed':False},session_ended=False)
        before['host_settings']['autosave']=True;after['host_settings']['autosave']=True
        log='automation_snapshot name=p0000 scene=MAP\nautosave file="private" bytes=42 scene=AM\nautomation_snapshot name=p0001 scene=AM\n'
        self.assertTrue(all(c['passed'] for c in check_flow([before,after],log)))
        self.assertFalse(all(c['passed'] for c in check_flow([before,after],log.replace('autosave file=','other file='))))
    def test_disabled_native_video_gate_detects_unwanted_playback(self):
        before=snapshot();before.update(scene='map',inventory_ui={'return_armed':False},session_ended=False)
        after=copy.deepcopy(before)
        log='automation_snapshot name=p0000 scene=MAP\nvideo name=test mode=2 placement=1x\nautomation_snapshot name=p0001 scene=MAP\n'
        self.assertIn('native_video_gate_5',[c['id'] for c in check_flow([before,after],log) if not c['passed']])
    def test_language_domains_follow_installed_pack_capabilities(self):
        host,prefs=initial(random.Random(1),0)
        domains={'text_language':['auto','tr'],'voice_language':['auto']}
        operation('text_language',host,prefs,random.Random(2),1,domains)
        self.assertEqual(host['text_language'],'tr')
        operation('voice_language',host,prefs,random.Random(2),2,domains)
        self.assertEqual(host['voice_language'],'auto')


if __name__=='__main__':unittest.main()
