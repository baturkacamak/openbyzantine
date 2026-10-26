
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00402e70 <.text+0x1e70>:
  402e70:	55                   	push   ebp
  402e71:	8b ec                	mov    ebp,esp
  402e73:	83 ec 04             	sub    esp,0x4
  402e76:	53                   	push   ebx
  402e77:	56                   	push   esi
  402e78:	57                   	push   edi
  402e79:	89 4d fc             	mov    DWORD PTR [ebp-0x4],ecx
  402e7c:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  402e7f:	dd 40 50             	fld    QWORD PTR [eax+0x50]
  402e82:	dc 0d 80 b0 45 00    	fmul   QWORD PTR ds:0x45b080
  402e88:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  402e8f:	75 08                	jne    0x402e99
  402e91:	dc 35 88 b0 45 00    	fdiv   QWORD PTR ds:0x45b088
  402e97:	eb 11                	jmp    0x402eaa
  402e99:	ff 35 8c b0 45 00    	push   DWORD PTR ds:0x45b08c
  402e9f:	ff 35 88 b0 45 00    	push   DWORD PTR ds:0x45b088
  402ea5:	e8 ee 83 04 00       	call   0x44b298
  402eaa:	e9 00 00 00 00       	jmp    0x402eaf
  402eaf:	5f                   	pop    edi
  402eb0:	5e                   	pop    esi
  402eb1:	5b                   	pop    ebx
  402eb2:	c9                   	leave
  402eb3:	c3                   	ret
