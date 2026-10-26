
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

00450df5 <.text+0x4fdf5>:
  450df5:	55                   	push   ebp
  450df6:	8b ec                	mov    ebp,esp
  450df8:	81 c4 60 fd ff ff    	add    esp,0xfffffd60
  450dfe:	53                   	push   ebx
  450dff:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  450e02:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  450e05:	e8 17 00 00 00       	call   0x450e21
  450e0a:	83 c4 08             	add    esp,0x8
  450e0d:	9b d9 bd 5c ff ff ff 	fstcw  WORD PTR [ebp-0xa4]
  450e14:	e8 f7 4a 00 00       	call   0x455910
  450e19:	e8 a0 fe ff ff       	call   0x450cbe
  450e1e:	5b                   	pop    ebx
  450e1f:	c9                   	leave
  450e20:	c3                   	ret
