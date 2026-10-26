// rea decompile_all
// name: __chsize
// address: 0x458990
// body: available
// range: 0x458990-0x458af6 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __chsize
   
   Library: Visual Studio 1998 Release */

int __cdecl __chsize(int _FileHandle,long _Size)

{
  DWORD DVar1;
  DWORD DVar2;
  uint uVar3;
  int iVar4;
  HANDLE hFile;
  BOOL BVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 *puVar9;
  uint in_stack_00001008;
  int in_stack_0000100c;
  
  FUN_0044be20();
  iVar7 = 0;
  if ((DAT_004a7710 <= in_stack_00001008) ||
     ((*(byte *)(*(int *)((int)&DAT_004a7720 + ((int)(in_stack_00001008 & 0xffffffe7) >> 3)) + 4 +
                (in_stack_00001008 & 0x1f) * 8) & 1) == 0)) {
    DAT_00462c74 = 9;
    return -1;
  }
  DVar1 = FUN_00450fc0(in_stack_00001008,0,1);
  if ((DVar1 == 0xffffffff) || (DVar2 = FUN_00450fc0(in_stack_00001008,0,2), DVar2 == 0xffffffff)) {
    return -1;
  }
  uVar8 = in_stack_0000100c - DVar2;
  if ((int)uVar8 < 1) {
    if ((int)uVar8 < 0) {
      FUN_00450fc0(in_stack_00001008,in_stack_0000100c,0);
      hFile = (HANDLE)FUN_004553c0(in_stack_00001008);
      BVar5 = SetEndOfFile(hFile);
      iVar7 = -(uint)(BVar5 == 0);
      if (iVar7 == -1) {
        DAT_00462c74 = 0xd;
        DAT_00462c78 = GetLastError();
      }
    }
  }
  else {
    puVar9 = (undefined4 *)register0x00000010;
    for (iVar6 = 0x400; puVar9 = puVar9 + 1, iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar9 = 0;
    }
    iVar6 = FUN_004594e0(in_stack_00001008,0x8000);
    do {
      uVar3 = 0x1000;
      if ((int)uVar8 < 0x1000) {
        uVar3 = uVar8;
      }
      iVar4 = FUN_0044d8e0(in_stack_00001008,(char *)&_FileHandle,uVar3);
      if (iVar4 == -1) {
        if (DAT_00462c78 == 5) {
          DAT_00462c74 = 0xd;
        }
        iVar7 = -1;
        break;
      }
      uVar8 = uVar8 - iVar4;
    } while (0 < (int)uVar8);
    FUN_004594e0(in_stack_00001008,iVar6);
  }
  FUN_00450fc0(in_stack_00001008,DVar1,0);
  return iVar7;
}

