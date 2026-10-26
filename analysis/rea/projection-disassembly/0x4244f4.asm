
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

004244f4 <.text+0x234f4>:
  4244f4:	dd 45 08             	fld    QWORD PTR [ebp+0x8]
  4244f7:	dc 0d 60 b1 45 00    	fmul   QWORD PTR ds:0x45b160
  4244fd:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  424504:	75 08                	jne    0x42450e
  424506:	dc 35 80 b1 45 00    	fdiv   QWORD PTR ds:0x45b180
  42450c:	eb 11                	jmp    0x42451f
  42450e:	ff 35 84 b1 45 00    	push   DWORD PTR ds:0x45b184
  424514:	ff 35 80 b1 45 00    	push   DWORD PTR ds:0x45b180
  42451a:	e8 79 6d 02 00       	call   0x44b298
  42451f:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424525:	dc 40 50             	fadd   QWORD PTR [eax+0x50]
  424528:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  42452e:	dd 58 50             	fstp   QWORD PTR [eax+0x50]
  424531:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424537:	dd 40 50             	fld    QWORD PTR [eax+0x50]
  42453a:	dc 1d 60 b1 45 00    	fcomp  QWORD PTR ds:0x45b160
  424540:	df e0                	fnstsw ax
  424542:	f6 c4 41             	test   ah,0x41
  424545:	0f 85 1d 00 00 00    	jne    0x424568
  42454b:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424551:	dd 40 50             	fld    QWORD PTR [eax+0x50]
  424554:	dc 25 40 b1 45 00    	fsub   QWORD PTR ds:0x45b140
  42455a:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424560:	dd 58 50             	fstp   QWORD PTR [eax+0x50]
  424563:	e9 c9 ff ff ff       	jmp    0x424531
  424568:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  42456e:	dd 40 50             	fld    QWORD PTR [eax+0x50]
  424571:	dc 1d 58 b1 45 00    	fcomp  QWORD PTR ds:0x45b158
  424577:	df e0                	fnstsw ax
  424579:	f6 c4 01             	test   ah,0x1
  42457c:	0f 84 1d 00 00 00    	je     0x42459f
  424582:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424588:	dd 40 50             	fld    QWORD PTR [eax+0x50]
  42458b:	dc 05 40 b1 45 00    	fadd   QWORD PTR ds:0x45b140
  424591:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424597:	dd 58 50             	fstp   QWORD PTR [eax+0x50]
  42459a:	e9 c9 ff ff ff       	jmp    0x424568
  42459f:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4245a5:	dd 40 28             	fld    QWORD PTR [eax+0x28]
  4245a8:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4245ae:	dc 48 50             	fmul   QWORD PTR [eax+0x50]
  4245b1:	e8 82 76 02 00       	call   0x44bc38
