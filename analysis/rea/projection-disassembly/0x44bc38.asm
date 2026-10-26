
analysis/rea/BETRAYAL.EXE:     file format pei-i386


Disassembly of section .text:

0044bc38 <.text+0x4ac38>:
  44bc38:	55                   	push   ebp
  44bc39:	8b ec                	mov    ebp,esp
  44bc3b:	83 c4 f4             	add    esp,0xfffffff4
  44bc3e:	9b d9 7d fe          	fstcw  WORD PTR [ebp-0x2]
  44bc42:	9b                   	fwait
  44bc43:	66 8b 45 fe          	mov    ax,WORD PTR [ebp-0x2]
  44bc47:	80 cc 0c             	or     ah,0xc
  44bc4a:	66 89 45 fc          	mov    WORD PTR [ebp-0x4],ax
  44bc4e:	d9 6d fc             	fldcw  WORD PTR [ebp-0x4]
  44bc51:	df 7d f4             	fistp  QWORD PTR [ebp-0xc]
  44bc54:	d9 6d fe             	fldcw  WORD PTR [ebp-0x2]
  44bc57:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  44bc5a:	8b 55 f8             	mov    edx,DWORD PTR [ebp-0x8]
  44bc5d:	c9                   	leave
  44bc5e:	c3                   	ret
