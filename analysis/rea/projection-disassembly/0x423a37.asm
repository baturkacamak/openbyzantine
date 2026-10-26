
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00423a37 <.text+0x22a37>:
  423a37:	55                   	push   ebp
  423a38:	8b ec                	mov    ebp,esp
  423a3a:	81 ec 94 00 00 00    	sub    esp,0x94
  423a40:	53                   	push   ebx
  423a41:	56                   	push   esi
  423a42:	57                   	push   edi
  423a43:	89 8d 74 ff ff ff    	mov    DWORD PTR [ebp-0x8c],ecx
  423a49:	c7 45 a0 00 01 00 00 	mov    DWORD PTR [ebp-0x60],0x100
  423a50:	c7 45 a4 00 00 38 43 	mov    DWORD PTR [ebp-0x5c],0x43380000
  423a57:	c7 45 ec 00 00 00 00 	mov    DWORD PTR [ebp-0x14],0x0
  423a5e:	c7 45 f0 00 00 f0 3f 	mov    DWORD PTR [ebp-0x10],0x3ff00000
  423a65:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423a6b:	8b 48 5c             	mov    ecx,DWORD PTR [eax+0x5c]
  423a6e:	89 4d cc             	mov    DWORD PTR [ebp-0x34],ecx
  423a71:	8b 40 58             	mov    eax,DWORD PTR [eax+0x58]
  423a74:	89 45 c8             	mov    DWORD PTR [ebp-0x38],eax
  423a77:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423a7d:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
  423a80:	89 45 98             	mov    DWORD PTR [ebp-0x68],eax
  423a83:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423a89:	8b 40 08             	mov    eax,DWORD PTR [eax+0x8]
  423a8c:	89 45 88             	mov    DWORD PTR [ebp-0x78],eax
  423a8f:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423a95:	8b 40 1c             	mov    eax,DWORD PTR [eax+0x1c]
  423a98:	89 45 b0             	mov    DWORD PTR [ebp-0x50],eax
  423a9b:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423aa1:	8b 48 2c             	mov    ecx,DWORD PTR [eax+0x2c]
  423aa4:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  423aa7:	8b 40 28             	mov    eax,DWORD PTR [eax+0x28]
  423aaa:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
  423aad:	c7 45 a8 00 00 00 00 	mov    DWORD PTR [ebp-0x58],0x0
  423ab4:	c7 45 ac 00 40 8f 40 	mov    DWORD PTR [ebp-0x54],0x408f4000
  423abb:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423ac1:	8b 40 44             	mov    eax,DWORD PTR [eax+0x44]
  423ac4:	89 45 b4             	mov    DWORD PTR [ebp-0x4c],eax
  423ac7:	c7 85 7c ff ff ff 00 	mov    DWORD PTR [ebp-0x84],0x0
  423ace:	00 00 00 
  423ad1:	c7 45 80 00 40 8f 40 	mov    DWORD PTR [ebp-0x80],0x408f4000
  423ad8:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423ade:	8b 40 4c             	mov    eax,DWORD PTR [eax+0x4c]
  423ae1:	89 45 8c             	mov    DWORD PTR [ebp-0x74],eax
  423ae4:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423aea:	dd 40 60             	fld    QWORD PTR [eax+0x60]
  423aed:	dc 4d f4             	fmul   QWORD PTR [ebp-0xc]
  423af0:	dd 5d 90             	fstp   QWORD PTR [ebp-0x70]
  423af3:	8b 45 98             	mov    eax,DWORD PTR [ebp-0x68]
  423af6:	c1 f8 01             	sar    eax,0x1
  423af9:	89 45 84             	mov    DWORD PTR [ebp-0x7c],eax
  423afc:	8b 45 88             	mov    eax,DWORD PTR [ebp-0x78]
  423aff:	c1 f8 01             	sar    eax,0x1
  423b02:	89 85 78 ff ff ff    	mov    DWORD PTR [ebp-0x88],eax
  423b08:	8b 45 b0             	mov    eax,DWORD PTR [ebp-0x50]
  423b0b:	c1 f8 01             	sar    eax,0x1
  423b0e:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  423b11:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423b17:	8b 40 20             	mov    eax,DWORD PTR [eax+0x20]
  423b1a:	c1 f8 01             	sar    eax,0x1
  423b1d:	89 45 e0             	mov    DWORD PTR [ebp-0x20],eax
  423b20:	8b 85 74 ff ff ff    	mov    eax,DWORD PTR [ebp-0x8c]
  423b26:	8b 40 3c             	mov    eax,DWORD PTR [eax+0x3c]
  423b29:	89 45 9c             	mov    DWORD PTR [ebp-0x64],eax
  423b2c:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  423b2f:	89 45 e8             	mov    DWORD PTR [ebp-0x18],eax
  423b32:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  423b35:	89 45 e4             	mov    DWORD PTR [ebp-0x1c],eax
  423b38:	8b 85 78 ff ff ff    	mov    eax,DWORD PTR [ebp-0x88]
  423b3e:	f7 d8                	neg    eax
  423b40:	89 85 70 ff ff ff    	mov    DWORD PTR [ebp-0x90],eax
  423b46:	db 85 70 ff ff ff    	fild   DWORD PTR [ebp-0x90]
  423b4c:	dd 5d b8             	fstp   QWORD PTR [ebp-0x48]
  423b4f:	8b 45 84             	mov    eax,DWORD PTR [ebp-0x7c]
  423b52:	f7 d8                	neg    eax
  423b54:	89 85 6c ff ff ff    	mov    DWORD PTR [ebp-0x94],eax
  423b5a:	db 85 6c ff ff ff    	fild   DWORD PTR [ebp-0x94]
  423b60:	dd 5d d8             	fstp   QWORD PTR [ebp-0x28]
  423b63:	60                   	pusha
  423b64:	9c                   	pushf
  423b65:	8b 7d 9c             	mov    edi,DWORD PTR [ebp-0x64]
  423b68:	dd 45 a0             	fld    QWORD PTR [ebp-0x60]
  423b6b:	dd 45 c8             	fld    QWORD PTR [ebp-0x38]
  423b6e:	d9 ff                	fcos
  423b70:	dd 45 c8             	fld    QWORD PTR [ebp-0x38]
  423b73:	d9 fe                	fsin
  423b75:	8b 5d fc             	mov    ebx,DWORD PTR [ebp-0x4]
  423b78:	8b 55 e0             	mov    edx,DWORD PTR [ebp-0x20]
  423b7b:	8b 75 88             	mov    esi,DWORD PTR [ebp-0x78]
  423b7e:	d9 c0                	fld    st(0)
  423b80:	dc 4d 90             	fmul   QWORD PTR [ebp-0x70]
  423b83:	d9 c2                	fld    st(2)
  423b85:	dc 4d b8             	fmul   QWORD PTR [ebp-0x48]
  423b88:	de c1                	faddp  st(1),st
  423b8a:	dc 4d f4             	fmul   QWORD PTR [ebp-0xc]
  423b8d:	d9 c1                	fld    st(1)
  423b8f:	dc 4d b8             	fmul   QWORD PTR [ebp-0x48]
  423b92:	d9 c3                	fld    st(3)
  423b94:	dc 4d 90             	fmul   QWORD PTR [ebp-0x70]
  423b97:	de e1                	fsubrp st(1),st
  423b99:	d9 e8                	fld1
  423b9b:	de f1                	fdivrp st(1),st
  423b9d:	8b 4d 98             	mov    ecx,DWORD PTR [ebp-0x68]
  423ba0:	dd 45 d8             	fld    QWORD PTR [ebp-0x28]
  423ba3:	d9 c1                	fld    st(1)
  423ba5:	d8 c9                	fmul   st,st(1)
  423ba7:	dc 8d 7c ff ff ff    	fmul   QWORD PTR [ebp-0x84]
  423bad:	d8 c6                	fadd   st,st(6)
  423baf:	dd 5d d0             	fstp   QWORD PTR [ebp-0x30]
  423bb2:	8b 45 d0             	mov    eax,DWORD PTR [ebp-0x30]
  423bb5:	2b 45 a0             	sub    eax,DWORD PTR [ebp-0x60]
  423bb8:	c1 e0 02             	shl    eax,0x2
  423bbb:	03 45 8c             	add    eax,DWORD PTR [ebp-0x74]
  423bbe:	d9 00                	fld    DWORD PTR [eax]
  423bc0:	d9 c0                	fld    st(0)
  423bc2:	dc 4d e4             	fmul   QWORD PTR [ebp-0x1c]
  423bc5:	d8 c7                	fadd   st,st(7)
  423bc7:	dd 5d d0             	fstp   QWORD PTR [ebp-0x30]
  423bca:	8b 45 a0             	mov    eax,DWORD PTR [ebp-0x60]
  423bcd:	29 45 d0             	sub    DWORD PTR [ebp-0x30],eax
  423bd0:	dc 4d a8             	fmul   QWORD PTR [ebp-0x58]
  423bd3:	d8 c6                	fadd   st,st(6)
  423bd5:	dd 5d c0             	fstp   QWORD PTR [ebp-0x40]
  423bd8:	8b 45 c0             	mov    eax,DWORD PTR [ebp-0x40]
  423bdb:	2b 45 a0             	sub    eax,DWORD PTR [ebp-0x60]
  423bde:	c1 e0 02             	shl    eax,0x2
  423be1:	03 45 b4             	add    eax,DWORD PTR [ebp-0x4c]
  423be4:	d9 00                	fld    DWORD PTR [eax]
  423be6:	d8 cb                	fmul   st,st(3)
  423be8:	d8 ca                	fmul   st,st(2)
  423bea:	d8 c6                	fadd   st,st(6)
  423bec:	dd 5d c0             	fstp   QWORD PTR [ebp-0x40]
  423bef:	8b 45 c0             	mov    eax,DWORD PTR [ebp-0x40]
  423bf2:	2b 45 a0             	sub    eax,DWORD PTR [ebp-0x60]
  423bf5:	03 c2                	add    eax,edx
  423bf7:	0f af 45 b0          	imul   eax,DWORD PTR [ebp-0x50]
  423bfb:	03 45 d0             	add    eax,DWORD PTR [ebp-0x30]
  423bfe:	03 c3                	add    eax,ebx
  423c00:	89 07                	mov    DWORD PTR [edi],eax
  423c02:	83 c7 04             	add    edi,0x4
  423c05:	dc 45 ec             	fadd   QWORD PTR [ebp-0x14]
  423c08:	49                   	dec    ecx
  423c09:	0f 85 94 ff ff ff    	jne    0x423ba3
  423c0f:	dd c0                	ffree  st(0)
  423c11:	d9 f7                	fincstp
  423c13:	dd c0                	ffree  st(0)
  423c15:	d9 f7                	fincstp
  423c17:	dd c0                	ffree  st(0)
  423c19:	d9 f7                	fincstp
  423c1b:	d9 e8                	fld1
  423c1d:	dc 45 b8             	fadd   QWORD PTR [ebp-0x48]
  423c20:	dd 5d b8             	fstp   QWORD PTR [ebp-0x48]
  423c23:	4e                   	dec    esi
  423c24:	0f 85 54 ff ff ff    	jne    0x423b7e
  423c2a:	dd c0                	ffree  st(0)
  423c2c:	d9 f7                	fincstp
  423c2e:	dd c0                	ffree  st(0)
  423c30:	d9 f7                	fincstp
  423c32:	dd c0                	ffree  st(0)
  423c34:	d9 f7                	fincstp
  423c36:	9d                   	popf
  423c37:	61                   	popa
  423c38:	e9 00 00 00 00       	jmp    0x423c3d
  423c3d:	5f                   	pop    edi
  423c3e:	5e                   	pop    esi
  423c3f:	5b                   	pop    ebx
  423c40:	c9                   	leave
  423c41:	c3                   	ret
