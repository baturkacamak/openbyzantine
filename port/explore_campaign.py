#!/usr/bin/env python3
"""One command for story, behavior, settings, mixed logic and real-media checks."""
import argparse
import html
import json
import os
from pathlib import Path
import secrets
import signal
import subprocess
import sys
import time

from explore_game import PORT,write_json


def run_process(command,folder,timeout):
    """Stream diagnostics, bounded wait, SIGINT lets inner runner clean players."""
    write_json(folder/'command.json',command)
    with (folder/'runner.log').open('w') as output:
        process=subprocess.Popen(command,cwd=PORT,stdout=output,stderr=subprocess.STDOUT,start_new_session=os.name=='posix')
        deadline=time.monotonic()+timeout
        with (folder/'runner.log').open() as reader:
            try:
                while True:
                    try:code=process.wait(timeout=.25)
                    except subprocess.TimeoutExpired:code=None
                    for line in reader:print(line.rstrip(),flush=True)
                    if code is not None:return code
                    if time.monotonic()>=deadline:raise TimeoutError('phase exceeded wall budget')
            except BaseException:
                if process.poll() is None:
                    try:
                        if os.name=='posix':os.killpg(process.pid,signal.SIGINT)
                        else:process.terminate()
                    except ProcessLookupError:pass
                    try:process.wait(timeout=10)
                    except subprocess.TimeoutExpired:
                        if os.name=='posix':os.killpg(process.pid,signal.SIGKILL)
                        else:process.kill()
                        process.wait()
                raise


def render(folder,report):
    esc=lambda value:html.escape(str(value),quote=True)
    rows=''.join('<tr>'+''.join('<td>'+esc(p.get(key,''))+'</td>' for key in ('name','status','runs','depth','outcome_checks','findings','media_playbacks'))+
        '<td><a href="'+esc(p['name'])+'/result/report.json">Rapor</a> · <a href="'+esc(p['name'])+'/runner.log">Günlük</a></td></tr>' for p in report['phases'])
    (folder/'index.html').write_text('<!doctype html><html lang="tr"><meta charset="utf-8"><title>Oyun test kampanyası</title>'
        '<style>body{font:16px system-ui;max-width:1100px;margin:40px auto;background:#181b20;color:#eee}a{color:#e7bf68}td,th{padding:12px;text-align:left;border-bottom:1px solid #555}</style>'
        '<h1>Oyun test kampanyası: '+esc(report['status'])+'</h1>'
        '<p>Mantık taraması sınırı: '+str(report['logic_run_limit'])+'; hikâye, ayar, davranış ve küçük medya profilleri ayrıca çalışır. Toplam çalıştırma: '+str(report['runs'])+'</p>'
        '<p>Klipler gerçek dosyalardan çözülür; SDL sanal cihazları kullanılır. Özgün oyunla eşdeğerlik veya bütün oyunun tamamlandığı iddia edilmez.</p>'
        '<table><tr><th>Profil</th><th>Durum</th><th>Çalıştırma</th><th>En uzun yol</th><th>Sonuç kontrolü</th><th>Bulgu</th><th>Medya tamamlanması</th><th>Kanıt</th></tr>'+rows+'</table></html>')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('logic_runs',type=int,nargs='?',default=2000)
    parser.add_argument('--data',type=Path,default=Path(os.environ.get('BYZ_DATA','/tmp/byz-scene')))
    parser.add_argument('--output',type=Path)
    parser.add_argument('--random-seed',type=int,default=None)
    parser.add_argument('--media-runs',type=int,default=12)
    parser.add_argument('--settings-cases',type=int,default=32)
    parser.add_argument('--settings-seconds',type=float,default=1200)
    parser.add_argument('--max-depth',type=int,default=24)
    parser.add_argument('--logic-seconds',type=float,default=14400)
    parser.add_argument('--media-seconds',type=float,default=900)
    parser.add_argument('--publish-latest',action='store_true')
    args=parser.parse_args()
    if min(args.logic_runs,args.media_runs,args.max_depth,args.logic_seconds,args.media_seconds,args.settings_cases,args.settings_seconds)<=0:parser.error('positive budgets required')
    seed=args.random_seed if args.random_seed is not None else secrets.randbits(32)
    if not 0<=seed<2**64:parser.error('invalid random seed')
    folder=(args.output or PORT/'output'/('campaign-'+time.strftime('%Y%m%dT%H%M%S')+'-'+str(os.getpid()))).resolve()
    folder.mkdir(parents=True,exist_ok=False)
    if args.publish_latest:
        latest=PORT/'output/EXPLORATION_LATEST';temporary=latest.with_name('EXPLORATION_LATEST.tmp.'+str(os.getpid()))
        temporary.write_text(str(folder)+'\n');temporary.replace(latest)
    data=args.data.resolve();python=sys.executable
    report={'schema':1,'kind':'multi-profile-campaign','status':'RUNNING','logic_run_limit':args.logic_runs,
            'random_seed':seed,'phases':[],'runs':0,'findings':[],
            'scope':'dummy SDL; genuine media decode/playback; original-runtime equivalence unverified'}
    logic=[python,'-u',str(PORT/'explore_game.py'),'--data',str(data),'--strategy','mixed',
        '--random-seed',str(seed),'--max-depth',str(args.max_depth),'--max-frontier','20000',
        '--max-actions','32','--screenshots','findings']
    phases=[('behavior', [python,'-u',str(PORT/'test_behaviors.py'),'--data',str(data)],1000),
            ('story',[python,'-u',str(PORT/'test_stories.py'),'--data',str(data)],1000),
            ('settings',[python,'-u',str(PORT/'settings_fuzz.py'),'--data',str(data),'--cases',str(args.settings_cases),
                '--random-seed',str(seed),'--max-seconds',str(args.settings_seconds)],args.settings_seconds+90),
            ('logic',logic+['--all-saves','--max-runs',str(args.logic_runs),'--max-states',str(args.logic_runs),
                '--save-every','20','--timeout','90','--max-seconds',str(args.logic_seconds)],args.logic_seconds+90),
            ('media-behavior',[python,'-u',str(PORT/'test_behaviors.py'),'--data',str(data),'--with-media',
                '--max-seconds',str(args.media_seconds)],args.media_seconds+90),
            ('media-settings',[python,'-u',str(PORT/'settings_fuzz.py'),'--data',str(data),'--cases','2','--with-media',
                '--random-seed',str(seed),'--max-seconds',str(args.media_seconds),'--timeout','240'],args.media_seconds+90),
            ('media-exploration',logic+['--seeds',str(PORT/'tests/exploration/media-seeds.json'),
                '--with-media','--max-runs',str(args.media_runs),'--max-states',str(args.media_runs),
                '--save-every','0','--timeout','240','--max-seconds',str(args.media_seconds)],args.media_seconds+90)]
    write_json(folder/'report.json',report);render(folder,report)
    interrupted=False
    for name,command,timeout in phases:
        phase=folder/name;phase.mkdir();result_folder=phase/'result'
        item={'name':name,'status':'RUNNING','runs':0,'findings':0,'media_playbacks':0};report['phases'].append(item)
        write_json(folder/'report.json',report);render(folder,report)
        print('Campaign phase:',name,flush=True)
        try:
            code=run_process(command+['--output',str(result_folder)],phase,timeout)
            child=json.loads((result_folder/'report.json').read_text())
            item.update(status=child['status'],runs=child.get('runs',0),findings=len(child.get('findings',[])),exit_code=code)
            item['depth']=max((s['depth'] for s in child.get('states',[])),default=0)
            item['outcome_checks']=child.get('assertions_checked',0)+child.get('behavior_checks',0)+child.get('settings_checked',0)
            item['media_playbacks']=child.get('media_playbacks',sum(len(c.get('media',{}).get('completed_playbacks',[])) for c in child.get('cases',[])))
            for finding in child.get('findings',[]):report['findings'].append(dict(finding,phase=name))
            if code and not child.get('findings'):
                report['findings'].append({'kind':'profile_failure','severity':'error','phase':name,'reason':child.get('error',child['status'])})
        except KeyboardInterrupt:
            item['status']='INTERRUPTED';interrupted=True
        except Exception as error:
            item['status']='ERROR';item['error']=str(error)
            report['findings'].append({'kind':'profile_failure','severity':'error','phase':name,'reason':str(error)})
        report['runs']=sum(p['runs'] for p in report['phases'])
        write_json(folder/'report.json',report);render(folder,report)
        if interrupted:break
    report['status']='INTERRUPTED' if interrupted else ('FINDINGS' if report['findings'] else 'PASS')
    write_json(folder/'report.json',report);render(folder,report)
    print('Campaign report:',folder/'index.html',report['status'],flush=True)
    return int(interrupted or any(f['severity']=='error' for f in report['findings']))


if __name__=='__main__':sys.exit(main())
