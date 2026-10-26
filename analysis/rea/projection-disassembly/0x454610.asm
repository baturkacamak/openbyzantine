
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00454610 <.text+0x53610>:
  454610:	66 8b 54 24 04       	mov    dx,WORD PTR [esp+0x4]
  454615:	33 c0                	xor    eax,eax
  454617:	f6 c2 01             	test   dl,0x1
  45461a:	74 05                	je     0x454621
  45461c:	b8 10 00 00 00       	mov    eax,0x10
  454621:	f6 c2 04             	test   dl,0x4
  454624:	74 03                	je     0x454629
  454626:	83 c8 08             	or     eax,0x8
  454629:	f6 c2 08             	test   dl,0x8
  45462c:	74 03                	je     0x454631
  45462e:	83 c8 04             	or     eax,0x4
  454631:	f6 c2 10             	test   dl,0x10
  454634:	74 03                	je     0x454639
  454636:	83 c8 02             	or     eax,0x2
  454639:	f6 c2 20             	test   dl,0x20
  45463c:	74 03                	je     0x454641
  45463e:	83 c8 01             	or     eax,0x1
  454641:	f6 c2 02             	test   dl,0x2
  454644:	74 05                	je     0x45464b
  454646:	0d 00 00 08 00       	or     eax,0x80000
  45464b:	33 c9                	xor    ecx,ecx
  45464d:	66 8b ca             	mov    cx,dx
  454650:	81 e1 00 0c 00 00    	and    ecx,0xc00
  454656:	81 f9 00 04 00 00    	cmp    ecx,0x400
  45465c:	74 12                	je     0x454670
  45465e:	81 f9 00 08 00 00    	cmp    ecx,0x800
  454664:	74 11                	je     0x454677
  454666:	81 f9 00 0c 00 00    	cmp    ecx,0xc00
  45466c:	74 10                	je     0x45467e
  45466e:	eb 13                	jmp    0x454683
  454670:	0d 00 01 00 00       	or     eax,0x100
  454675:	eb 0c                	jmp    0x454683
  454677:	0d 00 02 00 00       	or     eax,0x200
  45467c:	eb 05                	jmp    0x454683
  45467e:	0d 00 03 00 00       	or     eax,0x300
  454683:	33 c9                	xor    ecx,ecx
  454685:	66 8b ca             	mov    cx,dx
  454688:	81 e1 00 03 00 00    	and    ecx,0x300
  45468e:	74 0a                	je     0x45469a
  454690:	81 f9 00 02 00 00    	cmp    ecx,0x200
  454696:	74 09                	je     0x4546a1
  454698:	eb 0c                	jmp    0x4546a6
  45469a:	0d 00 00 02 00       	or     eax,0x20000
  45469f:	eb 05                	jmp    0x4546a6
  4546a1:	0d 00 00 01 00       	or     eax,0x10000
  4546a6:	f6 c6 10             	test   dh,0x10
  4546a9:	74 05                	je     0x4546b0
  4546ab:	0d 00 00 04 00       	or     eax,0x40000
  4546b0:	c3                   	ret
  4546b1:	cc                   	int3
  4546b2:	cc                   	int3
  4546b3:	cc                   	int3
  4546b4:	cc                   	int3
  4546b5:	cc                   	int3
  4546b6:	cc                   	int3
  4546b7:	cc                   	int3
  4546b8:	cc                   	int3
  4546b9:	cc                   	int3
  4546ba:	cc                   	int3
  4546bb:	cc                   	int3
  4546bc:	cc                   	int3
  4546bd:	cc                   	int3
  4546be:	cc                   	int3
  4546bf:	cc                   	int3
  4546c0:	66 33 c0             	xor    ax,ax
  4546c3:	8b 54 24 04          	mov    edx,DWORD PTR [esp+0x4]
  4546c7:	f6 c2 10             	test   dl,0x10
  4546ca:	74 04                	je     0x4546d0
  4546cc:	66 b8 01 00          	mov    ax,0x1
  4546d0:	f6 c2 08             	test   dl,0x8
  4546d3:	74 02                	je     0x4546d7
  4546d5:	0c 04                	or     al,0x4
  4546d7:	f6 c2 04             	test   dl,0x4
  4546da:	74 02                	je     0x4546de
  4546dc:	0c 08                	or     al,0x8
  4546de:	f6 c2 02             	test   dl,0x2
  4546e1:	74 02                	je     0x4546e5
  4546e3:	0c 10                	or     al,0x10
  4546e5:	f6 c2 01             	test   dl,0x1
  4546e8:	74 02                	je     0x4546ec
  4546ea:	0c 20                	or     al,0x20
  4546ec:	f7 c2 00 00 08 00    	test   edx,0x80000
  4546f2:	74 02                	je     0x4546f6
  4546f4:	0c 02                	or     al,0x2
  4546f6:	8b ca                	mov    ecx,edx
  4546f8:	81 e1 00 03 00 00    	and    ecx,0x300
  4546fe:	81 f9 00 01 00 00    	cmp    ecx,0x100
  454704:	74 12                	je     0x454718
  454706:	81 f9 00 02 00 00    	cmp    ecx,0x200
  45470c:	74 0f                	je     0x45471d
  45470e:	81 f9 00 03 00 00    	cmp    ecx,0x300
  454714:	74 0c                	je     0x454722
  454716:	eb 0d                	jmp    0x454725
  454718:	80 cc 04             	or     ah,0x4
  45471b:	eb 08                	jmp    0x454725
  45471d:	80 cc 08             	or     ah,0x8
  454720:	eb 03                	jmp    0x454725
  454722:	80 cc 0c             	or     ah,0xc
  454725:	8b ca                	mov    ecx,edx
  454727:	81 e1 00 00 03 00    	and    ecx,0x30000
  45472d:	74 0a                	je     0x454739
  45472f:	81 f9 00 00 01 00    	cmp    ecx,0x10000
  454735:	74 07                	je     0x45473e
  454737:	eb 08                	jmp    0x454741
  454739:	80 cc 03             	or     ah,0x3
  45473c:	eb 03                	jmp    0x454741
  45473e:	80 cc 02             	or     ah,0x2
  454741:	f7 c2 00 00 04 00    	test   edx,0x40000
  454747:	74 03                	je     0x45474c
  454749:	80 cc 10             	or     ah,0x10
  45474c:	c3                   	ret
