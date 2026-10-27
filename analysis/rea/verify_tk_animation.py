#!/usr/bin/env python3
"""Compare real TK actor pixels with an independent cumulative RLE reference.
Usage: verify_tk_animation.py <data-root> [preferred-disc-root]
Dummy SDL, no claim of physical window/speaker validation.
"""
from pathlib import Path
import struct,subprocess,os,re,time,sys
r=Path(__file__).resolve().parents[2];d=Path(sys.argv[1]).resolve();overlay=Path(sys.argv[2]).resolve() if len(sys.argv)>2 else d;b=(overlay/'anims/tk/TKNUXB.RLE').read_bytes();at=10;frames=[]
for i in range(struct.unpack_from('<H',b,8)[0]):
 w,h=struct.unpack_from('<hh',b,at+2);n=struct.unpack_from('<I',b,at+22)[0];at+=26;words=struct.unpack_from('<'+'H'*n,b,at);at+=n*2;frames.append((w,h,words))
def reference(index):
 pix=[0]*37500
 for w,h,words in frames[:index+1]:
  row=-1;col=0;k=0
  while k<len(words):
   op=words[k]>>14;n=words[k]&16383;k+=1
   if op==1: row+=1;col=n;continue
   if op==3: col+=n;continue
   values=words[k:k+n] if op==0 else [words[k]]*n;k+=n if op==0 else 1
   for v in values:
    if 0<=row<h and 0<=col<w:pix[row*150+col]=(v&31)|((v&65504)<<1)&65535
    col+=1
 return bytes(c for v in pix for c in [((v>>11)<<3)|((v>>11)>>2),(((v>>5)&63)<<2)|(((v>>5)&63)>>4),((v&31)<<3)|((v&31)>>2)])
a=[str(r/'port/byzantine-play'),str(r/'port/output/cc_all/TK'),str(r/'port/output/cb_all/TK'),'--entry','--engine',str(d/'copy2hd/engine'),'--backgrounds',str(d),'--anims',str(d),'--no-audio','--no-hotspots','--click-at','390,269','--print-records']
for k in ['ca','ci','cy','cs','cf']:a+=['--'+k,str(d/k/'TK')]
for k in ['lci','lcy','lcx']:a+=['--'+k,str(d/'copy2hd/engine'/('SHOW.'+k.upper()))]
a+=['--anims-overlay',str(overlay)]
out=r/'port/output/tk-animation-fix';out.mkdir(exist_ok=True)
for count in [10,50,180,350]:
 file=out/f'frame-{count}.ppm';t=time.monotonic();p=subprocess.run(a+['--compose-frames',str(count),'--shot',str(file)],env=dict(os.environ,SDL_VIDEODRIVER='dummy',SDL_AUDIODRIVER='dummy'),text=True,capture_output=True,timeout=60);assert p.returncode==0,p.stdout+p.stderr
 m=re.search(r'record id=1 .*?frame=(\d+).*? delay=(\d+) count=(-?\d+)',p.stdout);idx,delay,counter=map(int,m.groups());assert counter!=delay,'avoid boundary case'
 raw=file.read_bytes().split(b'\n',3);cw,ch=map(int,raw[1].split());img=raw[3];roi=b''.join(img[((y+170)*cw+244)*3:((y+170)*cw+394)*3] for y in range(250));expected=reference(idx);bad=sum(roi[i:i+3]!=expected[i:i+3] for i in range(0,len(roi),3));print(count,'frame',idx,'bad_pixels',bad,'seconds',round(time.monotonic()-t,3));assert bad==0
