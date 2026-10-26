
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00402e00 <.text+0x1e00>:
  402e00:	55                   	push   ebp
  402e01:	8b ec                	mov    ebp,esp
  402e03:	83 ec 04             	sub    esp,0x4
  402e06:	53                   	push   ebx
  402e07:	56                   	push   esi
  402e08:	57                   	push   edi
  402e09:	89 4d fc             	mov    DWORD PTR [ebp-0x4],ecx
  402e0c:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  402e0f:	dd 40 58             	fld    QWORD PTR [eax+0x58]
  402e12:	dc 0d 80 b0 45 00    	fmul   QWORD PTR ds:0x45b080
  402e18:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  402e1f:	75 08                	jne    0x402e29
  402e21:	dc 35 88 b0 45 00    	fdiv   QWORD PTR ds:0x45b088
  402e27:	eb 11                	jmp    0x402e3a
  402e29:	ff 35 8c b0 45 00    	push   DWORD PTR ds:0x45b08c
  402e2f:	ff 35 88 b0 45 00    	push   DWORD PTR ds:0x45b088
  402e35:	e8 5e 84 04 00       	call   0x44b298
  402e3a:	e9 00 00 00 00       	jmp    0x402e3f
  402e3f:	5f                   	pop    edi
  402e40:	5e                   	pop    esi
  402e41:	5b                   	pop    ebx
  402e42:	c9                   	leave
  402e43:	c3                   	ret
