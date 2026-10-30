"""Explicit observable contracts, separate from crash detection and state novelty."""
import re


def value_at(state,path):
    value=state
    for key in path:
        if isinstance(value,list) and type(key) is int:value=value[key]
        elif isinstance(value,dict) and key in value:value=value[key]
        else:raise ValueError('missing contract field: '+repr(path))
    return value


def check_assertion(states, assertion):
    if set(assertion)-{'at','path','op','value','other_at','other_path'}:
        raise ValueError('unknown assertion fields')
    index=assertion['at']
    if type(index) is not int or not 0<=index<len(states):raise ValueError('contract checkpoint missing')
    value=value_at(states[index],assertion['path']);op=assertion['op']
    expected=assertion.get('value')
    if op=='equal_checkpoint':expected=value_at(states[assertion['other_at']],assertion['other_path'])
    if op in ('eq','equal_checkpoint'):passed=value==expected
    elif op=='nonempty':passed=bool(value)
    elif op=='contains':passed=expected in value
    elif op=='contains_fields':passed=any(isinstance(item,dict) and all(item.get(k)==v for k,v in expected.items()) for item in value)
    elif op=='count_mask_eq':passed=sum(bool(node[1]&expected['mask']) for node in value)==expected['count']
    elif op=='gte':passed=type(value) in (int,float) and value>=expected
    else:raise ValueError('unsupported assertion operator: '+str(op))
    return {'checkpoint':index,'path':assertion['path'],'op':op,'expected':expected,
            'actual':value,'passed':passed}


def transition_checks(before,after):
    checks=[]
    parent=after['inventory_ui']
    if parent['return_armed']:
        checks.append({'id':'armed_examine_has_parent','passed':bool(parent['parent_scene'])})
    previous=before['inventory_ui']
    if previous['return_armed'] and not parent['return_armed']:
        checks.append({'id':'examine_returns_to_parent',
                       'expected':previous['parent_scene'],'actual':after['scene'],
                       'passed':after['scene']==previous['parent_scene']})
    return checks


def media_evidence(log):
    """Playback evidence, not merely decoder initialization or --with-media."""
    clips=[];errors=[];unavailable=[]
    for line in log.splitlines():
        if re.match(r'^(video|video_media|video_audio|conversation_media|conversation_audio)\b',line):
            if 'ERROR' in line or 'frame_error=' in line or 'reason=decode_failed' in line:
                errors.append(line)
            elif ('reason=missing' in line or ' missing (' in line or
                  ' unavailable (' in line or 'reason=not_shown' in line):unavailable.append(line)
        if re.match(r'^ elapsed_ms=\d+ written=[1-9]\d* .*\(script continues\)$',line):
            clips.append({'kind':'scene','completion':line})
        if re.match(r'^conversation record=.* media=[1-9]\d* audio_failures=0 resume_instruction=',line):
            clips.append({'kind':'conversation','completion':line})
    audio=re.findall(r'^(?:video_audio|conversation_audio) clip=.* queued_bytes=[1-9]\d* .*$',log,re.MULTILINE)
    return {'completed_playbacks':clips,'audio_queues':audio,'errors':errors,
            'unavailable':unavailable,'profile':'dummy SDL; actual decode/playback; no physical speaker check'}
