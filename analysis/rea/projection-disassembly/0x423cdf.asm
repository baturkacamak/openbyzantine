
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00423cdf <.text+0x22cdf>:
  423cdf:	55                   	push   ebp
  423ce0:	8b ec                	mov    ebp,esp
  423ce2:	83 ec 38             	sub    esp,0x38
  423ce5:	53                   	push   ebx
  423ce6:	56                   	push   esi
  423ce7:	57                   	push   edi
  423ce8:	89 4d d8             	mov    DWORD PTR [ebp-0x28],ecx
  423ceb:	68 00 00 f0 3f       	push   0x3ff00000
  423cf0:	6a 00                	push   0x0
  423cf2:	8b 4d d8             	mov    ecx,DWORD PTR [ebp-0x28]
  423cf5:	e8 48 ff ff ff       	call   0x423c42
  423cfa:	dd 5d e8             	fstp   QWORD PTR [ebp-0x18]
  423cfd:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
  423d00:	8b 48 2c             	mov    ecx,DWORD PTR [eax+0x2c]
  423d03:	89 4d f4             	mov    DWORD PTR [ebp-0xc],ecx
  423d06:	8b 40 28             	mov    eax,DWORD PTR [eax+0x28]
  423d09:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
  423d0c:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
  423d0f:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
  423d12:	c1 f8 01             	sar    eax,0x1
  423d15:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  423d18:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
  423d1b:	8b 40 08             	mov    eax,DWORD PTR [eax+0x8]
  423d1e:	c1 f8 01             	sar    eax,0x1
  423d21:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  423d24:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
  423d27:	50                   	push   eax
  423d28:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  423d2b:	50                   	push   eax
  423d2c:	e8 59 80 02 00       	call   0x44bd8a
  423d31:	83 c4 08             	add    esp,0x8
  423d34:	dc 4d f0             	fmul   QWORD PTR [ebp-0x10]
  423d37:	dd 5d d0             	fstp   QWORD PTR [ebp-0x30]
  423d3a:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
  423d3d:	50                   	push   eax
  423d3e:	8b 45 e8             	mov    eax,DWORD PTR [ebp-0x18]
  423d41:	50                   	push   eax
  423d42:	e8 39 80 02 00       	call   0x44bd80
  423d47:	83 c4 08             	add    esp,0x8
  423d4a:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  423d4d:	89 45 cc             	mov    DWORD PTR [ebp-0x34],eax
  423d50:	da 4d cc             	fimul  DWORD PTR [ebp-0x34]
  423d53:	dc 6d d0             	fsubr  QWORD PTR [ebp-0x30]
  423d56:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  423d59:	89 45 c8             	mov    DWORD PTR [ebp-0x38],eax
  423d5c:	83 3d e8 2a 46 00 00 	cmp    DWORD PTR ds:0x462ae8,0x0
  423d63:	75 05                	jne    0x423d6a
  423d65:	da 7d c8             	fidivr DWORD PTR [ebp-0x38]
  423d68:	eb 08                	jmp    0x423d72
  423d6a:	ff 75 c8             	push   DWORD PTR [ebp-0x38]
  423d6d:	e8 a6 76 02 00       	call   0x44b418
  423d72:	83 ec 08             	sub    esp,0x8
  423d75:	dd 1c 24             	fstp   QWORD PTR [esp]
  423d78:	e8 b4 96 02 00       	call   0x44d431
  423d7d:	83 c4 08             	add    esp,0x8
  423d80:	dd 55 dc             	fst    QWORD PTR [ebp-0x24]
  423d83:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
  423d86:	dc 48 28             	fmul   QWORD PTR [eax+0x28]
  423d89:	e8 aa 7e 02 00       	call   0x44bc38
  423d8e:	89 45 e4             	mov    DWORD PTR [ebp-0x1c],eax
  423d91:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  423d94:	e9 00 00 00 00       	jmp    0x423d99
  423d99:	5f                   	pop    edi
  423d9a:	5e                   	pop    esi
  423d9b:	5b                   	pop    ebx
  423d9c:	c9                   	leave
  423d9d:	c3                   	ret
