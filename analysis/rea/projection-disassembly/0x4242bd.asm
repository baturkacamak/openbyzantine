
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

004242bd <.text+0x232bd>:
  4242bd:	55                   	push   ebp
  4242be:	8b ec                	mov    ebp,esp
  4242c0:	81 ec ac 00 00 00    	sub    esp,0xac
  4242c6:	53                   	push   ebx
  4242c7:	56                   	push   esi
  4242c8:	57                   	push   edi
  4242c9:	89 8d 54 ff ff ff    	mov    DWORD PTR [ebp-0xac],ecx
  4242cf:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4242d5:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
  4242d8:	89 45 d8             	mov    DWORD PTR [ebp-0x28],eax
  4242db:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4242e1:	8b 40 08             	mov    eax,DWORD PTR [eax+0x8]
  4242e4:	89 85 60 ff ff ff    	mov    DWORD PTR [ebp-0xa0],eax
  4242ea:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4242f0:	66 8b 40 30          	mov    ax,WORD PTR [eax+0x30]
  4242f4:	66 89 45 dc          	mov    WORD PTR [ebp-0x24],ax
  4242f8:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4242fe:	8b 40 34             	mov    eax,DWORD PTR [eax+0x34]
  424301:	89 45 ec             	mov    DWORD PTR [ebp-0x14],eax
  424304:	83 7d ec 00          	cmp    DWORD PTR [ebp-0x14],0x0
  424308:	0f 85 0a 00 00 00    	jne    0x424318
  42430e:	b8 01 00 00 00       	mov    eax,0x1
  424313:	e9 40 04 00 00       	jmp    0x424758
  424318:	6a 6c                	push   0x6c
  42431a:	6a 00                	push   0x0
  42431c:	8d 85 64 ff ff ff    	lea    eax,[ebp-0x9c]
  424322:	50                   	push   eax
  424323:	e8 98 7a 02 00       	call   0x44bdc0
  424328:	83 c4 0c             	add    esp,0xc
  42432b:	c7 85 64 ff ff ff 6c 	mov    DWORD PTR [ebp-0x9c],0x6c
  424332:	00 00 00 
  424335:	6a 00                	push   0x0
  424337:	6a 01                	push   0x1
  424339:	8d 85 64 ff ff ff    	lea    eax,[ebp-0x9c]
  42433f:	50                   	push   eax
  424340:	6a 00                	push   0x0
  424342:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424348:	8b 00                	mov    eax,DWORD PTR [eax]
  42434a:	50                   	push   eax
  42434b:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424351:	8b 00                	mov    eax,DWORD PTR [eax]
  424353:	8b 00                	mov    eax,DWORD PTR [eax]
  424355:	ff 50 64             	call   DWORD PTR [eax+0x64]
  424358:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  42435b:	83 7d f8 00          	cmp    DWORD PTR [ebp-0x8],0x0
  42435f:	0f 84 0a 00 00 00    	je     0x42436f
  424365:	b8 01 00 00 00       	mov    eax,0x1
  42436a:	e9 e9 03 00 00       	jmp    0x424758
  42436f:	8b 45 88             	mov    eax,DWORD PTR [ebp-0x78]
  424372:	89 85 5c ff ff ff    	mov    DWORD PTR [ebp-0xa4],eax
  424378:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  42437e:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
  424381:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424387:	8b 40 10             	mov    eax,DWORD PTR [eax+0x10]
  42438a:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  42438d:	c1 f9 01             	sar    ecx,0x1
  424390:	0f af c1             	imul   eax,ecx
  424393:	8b 8d 54 ff ff ff    	mov    ecx,DWORD PTR [ebp-0xac]
  424399:	03 41 0c             	add    eax,DWORD PTR [ecx+0xc]
  42439c:	03 c0                	add    eax,eax
  42439e:	01 85 5c ff ff ff    	add    DWORD PTR [ebp-0xa4],eax
  4243a4:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
  4243a7:	89 45 e8             	mov    DWORD PTR [ebp-0x18],eax
  4243aa:	dd 45 10             	fld    QWORD PTR [ebp+0x10]
  4243ad:	dc 1d 78 b1 45 00    	fcomp  QWORD PTR ds:0x45b178
  4243b3:	df e0                	fnstsw ax
  4243b5:	f6 c4 40             	test   ah,0x40
  4243b8:	0f 84 14 00 00 00    	je     0x4243d2
  4243be:	dd 45 18             	fld    QWORD PTR [ebp+0x18]
  4243c1:	dc 1d 50 b1 45 00    	fcomp  QWORD PTR ds:0x45b150
  4243c7:	df e0                	fnstsw ax
  4243c9:	f6 c4 40             	test   ah,0x40
  4243cc:	0f 85 22 01 00 00    	jne    0x4244f4
  4243d2:	dd 45 10             	fld    QWORD PTR [ebp+0x10]
  4243d5:	dc 0d 60 b1 45 00    	fmul   QWORD PTR ds:0x45b160
  4243db:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  4243e2:	75 08                	jne    0x4243ec
  4243e4:	dc 35 80 b1 45 00    	fdiv   QWORD PTR ds:0x45b180
  4243ea:	eb 11                	jmp    0x4243fd
  4243ec:	ff 35 84 b1 45 00    	push   DWORD PTR ds:0x45b184
  4243f2:	ff 35 80 b1 45 00    	push   DWORD PTR ds:0x45b180
  4243f8:	e8 9b 6e 02 00       	call   0x44b298
  4243fd:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424403:	dc 68 58             	fsubr  QWORD PTR [eax+0x58]
  424406:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  42440c:	dd 58 58             	fstp   QWORD PTR [eax+0x58]
  42440f:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424415:	dd 40 60             	fld    QWORD PTR [eax+0x60]
  424418:	dc 4d 18             	fmul   QWORD PTR [ebp+0x18]
  42441b:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424421:	dd 58 60             	fstp   QWORD PTR [eax+0x60]
  424424:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  42442a:	dd 40 60             	fld    QWORD PTR [eax+0x60]
  42442d:	dd 05 50 b1 45 00    	fld    QWORD PTR ds:0x45b150
  424433:	d8 d1                	fcom   st(1)
  424435:	df e0                	fnstsw ax
  424437:	f6 c4 01             	test   ah,0x1
  42443a:	0f 85 02 00 00 00    	jne    0x424442
  424440:	d9 c9                	fxch   st(1)
  424442:	dd d8                	fstp   st(0)
  424444:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  42444a:	dd 58 60             	fstp   QWORD PTR [eax+0x60]
  42444d:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424453:	8b 48 64             	mov    ecx,DWORD PTR [eax+0x64]
  424456:	51                   	push   ecx
  424457:	8b 40 60             	mov    eax,DWORD PTR [eax+0x60]
  42445a:	50                   	push   eax
  42445b:	8b 8d 54 ff ff ff    	mov    ecx,DWORD PTR [ebp-0xac]
  424461:	e8 dc f7 ff ff       	call   0x423c42
  424466:	dd 55 d0             	fst    QWORD PTR [ebp-0x30]
  424469:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  42446f:	dc 58 58             	fcomp  QWORD PTR [eax+0x58]
  424472:	df e0                	fnstsw ax
  424474:	f6 c4 41             	test   ah,0x41
  424477:	0f 84 24 00 00 00    	je     0x4244a1
  42447d:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424483:	c7 40 68 01 00 00 00 	mov    DWORD PTR [eax+0x68],0x1
  42448a:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  424490:	8b 4d d4             	mov    ecx,DWORD PTR [ebp-0x2c]
  424493:	89 48 5c             	mov    DWORD PTR [eax+0x5c],ecx
  424496:	8b 4d d0             	mov    ecx,DWORD PTR [ebp-0x30]
  424499:	89 48 58             	mov    DWORD PTR [eax+0x58],ecx
  42449c:	e9 48 00 00 00       	jmp    0x4244e9
  4244a1:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4244a7:	dd 40 58             	fld    QWORD PTR [eax+0x58]
  4244aa:	dd 45 d0             	fld    QWORD PTR [ebp-0x30]
  4244ad:	d9 e0                	fchs
  4244af:	de d9                	fcompp
  4244b1:	df e0                	fnstsw ax
  4244b3:	f6 c4 01             	test   ah,0x1
  4244b6:	0f 85 20 00 00 00    	jne    0x4244dc
  4244bc:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4244c2:	c7 40 68 01 00 00 00 	mov    DWORD PTR [eax+0x68],0x1
  4244c9:	dd 45 d0             	fld    QWORD PTR [ebp-0x30]
  4244cc:	d9 e0                	fchs
  4244ce:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4244d4:	dd 58 58             	fstp   QWORD PTR [eax+0x58]
  4244d7:	e9 0d 00 00 00       	jmp    0x4244e9
  4244dc:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4244e2:	c7 40 68 00 00 00 00 	mov    DWORD PTR [eax+0x68],0x0
  4244e9:	8b 8d 54 ff ff ff    	mov    ecx,DWORD PTR [ebp-0xac]
  4244ef:	e8 43 f5 ff ff       	call   0x423a37
