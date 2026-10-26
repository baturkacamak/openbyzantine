
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

0044bfd0 <.text+0x4afd0>:
  44bfd0:	a1 ec 2a 46 00       	mov    eax,ds:0x462aec
  44bfd5:	85 c0                	test   eax,eax
  44bfd7:	74 02                	je     0x44bfdb
  44bfd9:	ff d0                	call   eax
  44bfdb:	68 1c c0 45 00       	push   0x45c01c
  44bfe0:	68 0c c0 45 00       	push   0x45c00c
  44bfe5:	e8 f6 00 00 00       	call   0x44c0e0
  44bfea:	83 c4 08             	add    esp,0x8
  44bfed:	68 08 c0 45 00       	push   0x45c008
  44bff2:	68 00 c0 45 00       	push   0x45c000
  44bff7:	e8 e4 00 00 00       	call   0x44c0e0
  44bffc:	83 c4 08             	add    esp,0x8
  44bfff:	c3                   	ret
