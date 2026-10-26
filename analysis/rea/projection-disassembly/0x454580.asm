
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00454580 <.text+0x53580>:
  454580:	83 ec 04             	sub    esp,0x4
  454583:	56                   	push   esi
  454584:	9b d9 7c 24 06       	fstcw  WORD PTR [esp+0x6]
  454589:	8b 44 24 06          	mov    eax,DWORD PTR [esp+0x6]
  45458d:	50                   	push   eax
  45458e:	e8 7d 00 00 00       	call   0x454610
  454593:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
  454597:	83 c4 04             	add    esp,0x4
  45459a:	8b f1                	mov    esi,ecx
  45459c:	23 4c 24 0c          	and    ecx,DWORD PTR [esp+0xc]
  4545a0:	f7 d6                	not    esi
  4545a2:	23 f0                	and    esi,eax
  4545a4:	0b f1                	or     esi,ecx
  4545a6:	56                   	push   esi
  4545a7:	e8 14 01 00 00       	call   0x4546c0
  4545ac:	66 89 44 24 08       	mov    WORD PTR [esp+0x8],ax
  4545b1:	83 c4 04             	add    esp,0x4
  4545b4:	d9 6c 24 04          	fldcw  WORD PTR [esp+0x4]
  4545b8:	8b c6                	mov    eax,esi
  4545ba:	5e                   	pop    esi
  4545bb:	83 c4 04             	add    esp,0x4
  4545be:	c3                   	ret
  4545bf:	cc                   	int3
  4545c0:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  4545c4:	8b 4c 24 04          	mov    ecx,DWORD PTR [esp+0x4]
  4545c8:	25 ff ff f7 ff       	and    eax,0xfff7ffff
  4545cd:	50                   	push   eax
  4545ce:	51                   	push   ecx
  4545cf:	e8 ac ff ff ff       	call   0x454580
  4545d4:	83 c4 08             	add    esp,0x8
  4545d7:	c3                   	ret
