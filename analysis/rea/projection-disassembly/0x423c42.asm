
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00423c42 <.text+0x22c42>:
  423c42:	55                   	push   ebp
  423c43:	8b ec                	mov    ebp,esp
  423c45:	83 ec 2c             	sub    esp,0x2c
  423c48:	53                   	push   ebx
  423c49:	56                   	push   esi
  423c4a:	57                   	push   edi
  423c4b:	89 4d e4             	mov    DWORD PTR [ebp-0x1c],ecx
  423c4e:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  423c51:	8b 40 08             	mov    eax,DWORD PTR [eax+0x8]
  423c54:	c1 f8 01             	sar    eax,0x1
  423c57:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  423c5a:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  423c5d:	8b 40 20             	mov    eax,DWORD PTR [eax+0x20]
  423c60:	c1 f8 01             	sar    eax,0x1
  423c63:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  423c66:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  423c69:	dd 40 28             	fld    QWORD PTR [eax+0x28]
  423c6c:	dc 4d 08             	fmul   QWORD PTR [ebp+0x8]
  423c6f:	dd 5d e8             	fstp   QWORD PTR [ebp-0x18]
  423c72:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  423c75:	89 45 e0             	mov    DWORD PTR [ebp-0x20],eax
  423c78:	db 45 e0             	fild   DWORD PTR [ebp-0x20]
  423c7b:	dc 4d e8             	fmul   QWORD PTR [ebp-0x18]
  423c7e:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  423c81:	dd 40 28             	fld    QWORD PTR [eax+0x28]
  423c84:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  423c87:	89 45 dc             	mov    DWORD PTR [ebp-0x24],eax
  423c8a:	da 4d dc             	fimul  DWORD PTR [ebp-0x24]
  423c8d:	de e9                	fsubp  st(1),st
  423c8f:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  423c92:	89 45 d8             	mov    DWORD PTR [ebp-0x28],eax
  423c95:	db 45 d8             	fild   DWORD PTR [ebp-0x28]
  423c98:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  423c9b:	89 45 d4             	mov    DWORD PTR [ebp-0x2c],eax
  423c9e:	da 4d d4             	fimul  DWORD PTR [ebp-0x2c]
  423ca1:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  423ca4:	dd 40 28             	fld    QWORD PTR [eax+0x28]
  423ca7:	dc 4d e8             	fmul   QWORD PTR [ebp-0x18]
  423caa:	de c1                	faddp  st(1),st
  423cac:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  423cb3:	75 04                	jne    0x423cb9
  423cb5:	de f9                	fdivp  st(1),st
  423cb7:	eb 09                	jmp    0x423cc2
  423cb9:	50                   	push   eax
  423cba:	b0 0d                	mov    al,0xd
  423cbc:	e8 c6 70 02 00       	call   0x44ad87
  423cc1:	58                   	pop    eax
  423cc2:	83 ec 08             	sub    esp,0x8
  423cc5:	dd 1c 24             	fstp   QWORD PTR [esp]
  423cc8:	e8 64 97 02 00       	call   0x44d431
  423ccd:	83 c4 08             	add    esp,0x8
  423cd0:	dd 55 f0             	fst    QWORD PTR [ebp-0x10]
  423cd3:	e9 00 00 00 00       	jmp    0x423cd8
  423cd8:	5f                   	pop    edi
  423cd9:	5e                   	pop    esi
  423cda:	5b                   	pop    ebx
  423cdb:	c9                   	leave
  423cdc:	c2                   	.byte 0xc2
