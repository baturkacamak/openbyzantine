
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

004245b6 <.text+0x235b6>:
  4245b6:	89 85 58 ff ff ff    	mov    DWORD PTR [ebp-0xa8],eax
  4245bc:	8b 85 54 ff ff ff    	mov    eax,DWORD PTR [ebp-0xac]
  4245c2:	8b 40 3c             	mov    eax,DWORD PTR [eax+0x3c]
  4245c5:	89 45 e0             	mov    DWORD PTR [ebp-0x20],eax
  4245c8:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  4245cb:	8b 4d d8             	mov    ecx,DWORD PTR [ebp-0x28]
  4245ce:	03 c9                	add    ecx,ecx
  4245d0:	2b c1                	sub    eax,ecx
  4245d2:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
  4245d5:	83 7d 28 00          	cmp    DWORD PTR [ebp+0x28],0x0
  4245d9:	0f 85 4b 00 00 00    	jne    0x42462a
  4245df:	60                   	pusha
  4245e0:	9c                   	pushf
  4245e1:	8b bd 5c ff ff ff    	mov    edi,DWORD PTR [ebp-0xa4]
  4245e7:	8b 75 ec             	mov    esi,DWORD PTR [ebp-0x14]
  4245ea:	8b 5d e0             	mov    ebx,DWORD PTR [ebp-0x20]
  4245ed:	8b 95 58 ff ff ff    	mov    edx,DWORD PTR [ebp-0xa8]
  4245f3:	c1 e2 01             	shl    edx,0x1
  4245f6:	8b 4d d8             	mov    ecx,DWORD PTR [ebp-0x28]
  4245f9:	8b 03                	mov    eax,DWORD PTR [ebx]
  4245fb:	c1 e0 01             	shl    eax,0x1
  4245fe:	03 c2                	add    eax,edx
  424600:	66 8b 04 06          	mov    ax,WORD PTR [esi+eax*1]
  424604:	66 89 07             	mov    WORD PTR [edi],ax
  424607:	83 c7 02             	add    edi,0x2
  42460a:	83 c3 04             	add    ebx,0x4
  42460d:	49                   	dec    ecx
  42460e:	0f 85 e5 ff ff ff    	jne    0x4245f9
  424614:	03 7d f0             	add    edi,DWORD PTR [ebp-0x10]
  424617:	ff 8d 60 ff ff ff    	dec    DWORD PTR [ebp-0xa0]
  42461d:	0f 85 d3 ff ff ff    	jne    0x4245f6
  424623:	9d                   	popf
  424624:	61                   	popa
  424625:	e9 f3 00 00 00       	jmp    0x42471d
