"""Settings oracle: requested values, applied host state and private persistence."""
import math


def validate_profile(value):
    if value is None:return None
    if set(value)-{'schema','ini','gameprefs','audio'} or value.get('schema')!=1:
        raise ValueError('invalid settings profile schema')
    if not isinstance(value['ini'],str) or len(value['ini'])>32768 or '\0' in value['ini']:
        raise ValueError('invalid initial settings INI')
    if len(value['gameprefs'])!=7 or any(type(x) is not int or x not in (0,1) for x in value['gameprefs']):
        raise ValueError('settings profile needs seven boolean native preferences')
    if type(value.get('audio',True)) is not bool:raise ValueError('invalid audio profile')
    return value


def check_settings(snapshot,expected):
    checks=[]
    def check(name,actual,wanted):
        passed=(math.isclose(actual,wanted,rel_tol=0,abs_tol=2e-5)
                if type(wanted) is float and type(actual) in (int,float) else actual==wanted)
        checks.append({'id':name,'actual':actual,'expected':wanted,'passed':passed})
    host=snapshot.get('host_settings');runtime=snapshot.get('host_runtime')
    if host is None or runtime is None:raise ValueError('player did not expose settings diagnostics')
    for key,value in expected['host'].items():check('host.'+key,host.get(key),value)
    check('native_preferences',snapshot.get('gameprefs'),expected['gameprefs'])
    check('applied.window_mode',runtime['window_mode'],host['window_mode'])
    check('applied.scale_mode',runtime['scale_mode'],host['scale_mode'])
    if runtime.get('vsync_supported'):
        check('renderer.vsync',bool(runtime['renderer_flags']&4),host['vsync'])
    check('logical_screen',runtime['logical'],[640,480])
    check('applied.hotspots',runtime['overlay_enabled'],host['hotspots'])
    check('applied.hotspots_all',runtime['overlay_all'],host['hotspots_all'])
    if expected.get('require_audio'):check('audio_device_available',runtime['audio_ready'],True)
    if runtime['audio_ready']:
        for i,key in enumerate(('master_volume','speech_volume','ambient_volume','effects_volume')):
            check('mixer.'+key,runtime['audio_gains'][i],host[key])
    if 'muted' in expected:check('focus.muted',runtime['audio_muted'],expected['muted'])
    if host['text_language']!='auto':check('text_language_applied',runtime['text_resolved'],host['text_language'])
    if host['voice_language']!='auto':check('voice_language_applied',runtime['voice_resolved'],host['voice_language'])
    if not host['gamepad']:check('disabled_controller_closed',runtime['controller_open'],False)
    if host['window_mode']=='windowed':
        w,h=runtime['window'];check('window_aspect_4_3',abs(3*w-4*h)<=4,True)
        check('windowed_uses_fit',runtime['integer_scale'],False)
    if 'content_rect' in runtime:
        w,h=runtime['window'];x,y,cw,ch=runtime['content_rect'];ow,oh=runtime['output']
        factor=min(w/640,h/480)
        expected_w,expected_h=640*factor,480*factor
        if runtime['integer_scale'] and ow and oh:
            integer=max(1,min(ow//640,oh//480));expected_w=640*integer*w/ow;expected_h=480*integer*h/oh
        check('viewport.fits_window',x>=-2 and y>=-2 and x+cw<=w+2 and y+ch<=h+2,True)
        check('viewport.scales_with_window',abs(cw-expected_w)<=2 and abs(ch-expected_h)<=2,True)
    if expected.get('closed',True):
        check('settings_closed',snapshot['settings'],False);check('native_menu_closed',snapshot['menu'],False)
        if 'last_content' in runtime:check('menu_frame_removed',runtime['last_content'],'scene')
    return checks


def verify_restart(folder,expected,timeout,execute):
    """Fresh process, final private INI/preferences; original scene save only."""
    import json
    fresh=folder/'fresh-settings';fresh.mkdir()
    command=json.loads((folder/'command.json').read_text())
    script=fresh/'actions.txt';script.write_text('snapshot reopened\nquit\n')
    command[command.index('--automation')+1]=str(script)
    command[command.index('--automation-output')+1]=str(fresh)
    prefs=folder/'saves/gamepref_000'
    if prefs.is_file():command[command.index('--gameprefs')+1]=str(prefs)
    (fresh/'command.json').write_text(json.dumps(command,indent=2)+'\n')
    code,expired=execute(command,fresh,timeout)
    if code or expired:return [{'id':'fresh_process','passed':False,'exit_code':code,'timed_out':expired}]
    snapshot=json.loads((fresh/'reopened.json').read_text())
    return check_settings(snapshot,expected)


def check_flow(states,log):
    """Effects across scene inputs: autosave and native clip preference gates."""
    import re
    checks=[]
    for i,(before,after) in enumerate(zip(states,states[1:]),1):
        start=log.find('automation_snapshot name=p'+str(i-1).zfill(4)+' ')
        end=log.find('automation_snapshot name=p'+str(i).zfill(4)+' ')
        if start<0 or end<start:raise ValueError('settings flow checkpoint markers missing')
        segment=log[start:end]
        saved=re.findall(r'^autosave file=.* scene=(\w+)',segment,re.MULTILINE)
        a=before['host_settings']['autosave'];b=after['host_settings']['autosave']
        if not a and not b:
            checks.append({'id':'autosave_off_does_not_write','passed':not saved,'actual':saved})
        if (a and b and before['scene']!=after['scene'] and not before['inventory_ui']['return_armed']
                and not after['session_ended']):
            checks.append({'id':'autosave_on_travel_writes','passed':after['scene'].lower() in [name.lower() for name in saved],
                           'actual':saved,'expected':after['scene']})
        modes=[int(n) for n in re.findall(r'^video name=\S+ mode=(\d+) placement=',segment,re.MULTILINE)]
        for mode,index in [(2,5),(3,4)]:
            if before['gameprefs'][index]==0 and after['gameprefs'][index]==0:
                checks.append({'id':'native_video_gate_'+str(index),'passed':mode not in modes,'actual_modes':modes})
    return checks
