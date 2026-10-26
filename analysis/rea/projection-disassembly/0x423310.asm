
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00423310 <.text+0x22310>:
  423310:	55                   	push   ebp
  423311:	8b ec                	mov    ebp,esp
  423313:	81 ec 90 00 00 00    	sub    esp,0x90
  423319:	53                   	push   ebx
  42331a:	56                   	push   esi
  42331b:	57                   	push   edi
  42331c:	89 8d 78 ff ff ff    	mov    DWORD PTR [ebp-0x88],ecx
  423322:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  423328:	c7 40 1c 00 00 00 00 	mov    DWORD PTR [eax+0x1c],0x0
  42332f:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  423335:	c7 40 20 00 00 00 00 	mov    DWORD PTR [eax+0x20],0x0
  42333c:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  42333f:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423345:	89 01                	mov    DWORD PTR [ecx],eax
  423347:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  42334d:	c7 40 34 00 00 00 00 	mov    DWORD PTR [eax+0x34],0x0
  423354:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  42335a:	c7 40 3c 00 00 00 00 	mov    DWORD PTR [eax+0x3c],0x0
  423361:	6a 6c                	push   0x6c
  423363:	6a 00                	push   0x0
  423365:	8d 85 7c ff ff ff    	lea    eax,[ebp-0x84]
  42336b:	50                   	push   eax
  42336c:	e8 4f 8a 02 00       	call   0x44bdc0
  423371:	83 c4 0c             	add    esp,0xc
  423374:	c7 85 7c ff ff ff 6c 	mov    DWORD PTR [ebp-0x84],0x6c
  42337b:	00 00 00 
  42337e:	8d 85 7c ff ff ff    	lea    eax,[ebp-0x84]
  423384:	50                   	push   eax
  423385:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  423388:	50                   	push   eax
  423389:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  42338c:	8b 00                	mov    eax,DWORD PTR [eax]
  42338e:	ff 50 58             	call   DWORD PTR [eax+0x58]
  423391:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  423394:	83 7d 0c 00          	cmp    DWORD PTR [ebp+0xc],0x0
  423398:	0f 85 37 00 00 00    	jne    0x4233d5
  42339e:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  4233a4:	c7 40 0c 00 00 00 00 	mov    DWORD PTR [eax+0xc],0x0
  4233ab:	8b 45 88             	mov    eax,DWORD PTR [ebp-0x78]
  4233ae:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  4233b4:	89 41 14             	mov    DWORD PTR [ecx+0x14],eax
  4233b7:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  4233bd:	c7 40 10 00 00 00 00 	mov    DWORD PTR [eax+0x10],0x0
  4233c4:	8b 45 84             	mov    eax,DWORD PTR [ebp-0x7c]
  4233c7:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  4233cd:	89 41 18             	mov    DWORD PTR [ecx+0x18],eax
  4233d0:	e9 22 00 00 00       	jmp    0x4233f7
  4233d5:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4233d8:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  4233de:	83 c1 0c             	add    ecx,0xc
  4233e1:	8b 10                	mov    edx,DWORD PTR [eax]
  4233e3:	89 11                	mov    DWORD PTR [ecx],edx
  4233e5:	8b 50 04             	mov    edx,DWORD PTR [eax+0x4]
  4233e8:	89 51 04             	mov    DWORD PTR [ecx+0x4],edx
  4233eb:	8b 50 08             	mov    edx,DWORD PTR [eax+0x8]
  4233ee:	89 51 08             	mov    DWORD PTR [ecx+0x8],edx
  4233f1:	8b 40 0c             	mov    eax,DWORD PTR [eax+0xc]
  4233f4:	89 41 0c             	mov    DWORD PTR [ecx+0xc],eax
  4233f7:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  4233fd:	8b 40 14             	mov    eax,DWORD PTR [eax+0x14]
  423400:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423406:	2b 41 0c             	sub    eax,DWORD PTR [ecx+0xc]
  423409:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  42340f:	89 41 04             	mov    DWORD PTR [ecx+0x4],eax
  423412:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  423418:	8b 40 18             	mov    eax,DWORD PTR [eax+0x18]
  42341b:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423421:	2b 41 10             	sub    eax,DWORD PTR [ecx+0x10]
  423424:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  42342a:	89 41 08             	mov    DWORD PTR [ecx+0x8],eax
  42342d:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
  423430:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423436:	66 89 41 30          	mov    WORD PTR [ecx+0x30],ax
  42343a:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  423440:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
  423443:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423449:	0f af 41 08          	imul   eax,DWORD PTR [ecx+0x8]
  42344d:	c1 e0 02             	shl    eax,0x2
  423450:	50                   	push   eax
  423451:	e8 0a 78 02 00       	call   0x44ac60
  423456:	83 c4 04             	add    esp,0x4
  423459:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  42345f:	89 41 3c             	mov    DWORD PTR [ecx+0x3c],eax
  423462:	c7 45 fc 45 0c 00 00 	mov    DWORD PTR [ebp-0x4],0xc45
  423469:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  42346c:	c1 e0 02             	shl    eax,0x2
  42346f:	50                   	push   eax
  423470:	e8 eb 77 02 00       	call   0x44ac60
  423475:	83 c4 04             	add    esp,0x4
  423478:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  42347e:	89 41 40             	mov    DWORD PTR [ecx+0x40],eax
  423481:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  423484:	83 e0 fe             	and    eax,0xfffffffe
  423487:	c1 f8 00             	sar    eax,0x0
  42348a:	03 c0                	add    eax,eax
  42348c:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423492:	03 41 40             	add    eax,DWORD PTR [ecx+0x40]
  423495:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  42349b:	89 41 44             	mov    DWORD PTR [ecx+0x44],eax
  42349e:	c7 45 e8 00 00 00 00 	mov    DWORD PTR [ebp-0x18],0x0
  4234a5:	e9 03 00 00 00       	jmp    0x4234ad
  4234aa:	ff 45 e8             	inc    DWORD PTR [ebp-0x18]
  4234ad:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  4234b0:	c1 f8 01             	sar    eax,0x1
  4234b3:	3b 45 e8             	cmp    eax,DWORD PTR [ebp-0x18]
  4234b6:	0f 8e 7f 00 00 00    	jle    0x42353b
  4234bc:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  4234bf:	89 85 74 ff ff ff    	mov    DWORD PTR [ebp-0x8c],eax
  4234c5:	db 85 74 ff ff ff    	fild   DWORD PTR [ebp-0x8c]
  4234cb:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  4234d2:	75 08                	jne    0x4234dc
  4234d4:	dc 35 38 b1 45 00    	fdiv   QWORD PTR ds:0x45b138
  4234da:	eb 11                	jmp    0x4234ed
  4234dc:	ff 35 3c b1 45 00    	push   DWORD PTR ds:0x45b13c
  4234e2:	ff 35 38 b1 45 00    	push   DWORD PTR ds:0x45b138
  4234e8:	e8 ab 7d 02 00       	call   0x44b298
  4234ed:	dd 5d ec             	fstp   QWORD PTR [ebp-0x14]
  4234f0:	8b 45 f0             	mov    eax,DWORD PTR [ebp-0x10]
  4234f3:	50                   	push   eax
  4234f4:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
  4234f7:	50                   	push   eax
  4234f8:	e8 8d 88 02 00       	call   0x44bd8a
  4234fd:	83 c4 08             	add    esp,0x8
  423500:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  423503:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423509:	8b 49 44             	mov    ecx,DWORD PTR [ecx+0x44]
  42350c:	d9 1c 81             	fstp   DWORD PTR [ecx+eax*4]
  42350f:	dd 45 ec             	fld    QWORD PTR [ebp-0x14]
  423512:	d9 e0                	fchs
  423514:	83 ec 08             	sub    esp,0x8
  423517:	dd 1c 24             	fstp   QWORD PTR [esp]
  42351a:	e8 6b 88 02 00       	call   0x44bd8a
  42351f:	83 c4 08             	add    esp,0x8
  423522:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  423525:	c1 e0 02             	shl    eax,0x2
  423528:	f7 d8                	neg    eax
  42352a:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423530:	8b 49 44             	mov    ecx,DWORD PTR [ecx+0x44]
  423533:	d9 1c 08             	fstp   DWORD PTR [eax+ecx*1]
  423536:	e9 6f ff ff ff       	jmp    0x4234aa
  42353b:	c7 45 f4 a0 0f 00 00 	mov    DWORD PTR [ebp-0xc],0xfa0
  423542:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  423545:	c1 e0 02             	shl    eax,0x2
  423548:	50                   	push   eax
  423549:	e8 12 77 02 00       	call   0x44ac60
  42354e:	83 c4 04             	add    esp,0x4
  423551:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423557:	89 41 48             	mov    DWORD PTR [ecx+0x48],eax
  42355a:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  42355d:	83 e0 fe             	and    eax,0xfffffffe
  423560:	c1 f8 00             	sar    eax,0x0
  423563:	03 c0                	add    eax,eax
  423565:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  42356b:	03 41 48             	add    eax,DWORD PTR [ecx+0x48]
  42356e:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423574:	89 41 4c             	mov    DWORD PTR [ecx+0x4c],eax
  423577:	c7 45 e8 00 00 00 00 	mov    DWORD PTR [ebp-0x18],0x0
  42357e:	e9 03 00 00 00       	jmp    0x423586
  423583:	ff 45 e8             	inc    DWORD PTR [ebp-0x18]
  423586:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  423589:	c1 f8 01             	sar    eax,0x1
  42358c:	3b 45 e8             	cmp    eax,DWORD PTR [ebp-0x18]
  42358f:	0f 8e 7f 00 00 00    	jle    0x423614
  423595:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  423598:	89 85 70 ff ff ff    	mov    DWORD PTR [ebp-0x90],eax
  42359e:	db 85 70 ff ff ff    	fild   DWORD PTR [ebp-0x90]
  4235a4:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  4235ab:	75 08                	jne    0x4235b5
  4235ad:	dc 35 38 b1 45 00    	fdiv   QWORD PTR ds:0x45b138
  4235b3:	eb 11                	jmp    0x4235c6
  4235b5:	ff 35 3c b1 45 00    	push   DWORD PTR ds:0x45b13c
  4235bb:	ff 35 38 b1 45 00    	push   DWORD PTR ds:0x45b138
  4235c1:	e8 d2 7c 02 00       	call   0x44b298
  4235c6:	dd 5d ec             	fstp   QWORD PTR [ebp-0x14]
  4235c9:	8b 45 f0             	mov    eax,DWORD PTR [ebp-0x10]
  4235cc:	50                   	push   eax
  4235cd:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
  4235d0:	50                   	push   eax
  4235d1:	e8 5b 9e 02 00       	call   0x44d431
  4235d6:	83 c4 08             	add    esp,0x8
  4235d9:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  4235dc:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  4235e2:	8b 49 4c             	mov    ecx,DWORD PTR [ecx+0x4c]
  4235e5:	d9 1c 81             	fstp   DWORD PTR [ecx+eax*4]
  4235e8:	dd 45 ec             	fld    QWORD PTR [ebp-0x14]
  4235eb:	d9 e0                	fchs
  4235ed:	83 ec 08             	sub    esp,0x8
  4235f0:	dd 1c 24             	fstp   QWORD PTR [esp]
  4235f3:	e8 39 9e 02 00       	call   0x44d431
  4235f8:	83 c4 08             	add    esp,0x8
  4235fb:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  4235fe:	c1 e0 02             	shl    eax,0x2
  423601:	f7 d8                	neg    eax
  423603:	8b 8d 78 ff ff ff    	mov    ecx,DWORD PTR [ebp-0x88]
  423609:	8b 49 4c             	mov    ecx,DWORD PTR [ecx+0x4c]
  42360c:	d9 1c 08             	fstp   DWORD PTR [eax+ecx*1]
  42360f:	e9 6f ff ff ff       	jmp    0x423583
  423614:	e9 00 00 00 00       	jmp    0x423619
  423619:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  42361f:	5f                   	pop    edi
  423620:	5e                   	pop    esi
  423621:	5b                   	pop    ebx
  423622:	c9                   	leave
  423623:	c2 08 00             	ret    0x8
