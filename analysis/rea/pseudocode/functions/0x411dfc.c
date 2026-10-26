// rea decompile_all
// name: FUN_00411dfc
// address: 0x411dfc
// body: available
// range: 0x411dfc-0x412205 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00411dfc(int param_1,uint *param_2,ushort *param_3)

{
  ushort uVar1;
  ushort uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  char *pcVar6;
  int local_110;
  FILE *local_10c;
  uint local_108 [65];
  
  local_110 = -1;
  for (uVar1 = 0; uVar1 < 0x15e; uVar1 = uVar1 + 1) {
    if (*(int *)(param_1 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(param_1 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(param_1 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(param_1 + (uint)uVar1 * 4));
      *(undefined4 *)(param_1 + (uint)uVar1 * 4) = 0;
    }
  }
  *param_3 = 0;
  if (DAT_0045e7a8 == 0) {
    _sprintf((char *)local_108,s_engine__s_0045ea6c,param_2);
    local_10c = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045ea78);
    if (local_10c == (FILE *)0x0) {
      return 0;
    }
  }
  else {
    local_10c = FUN_00411b4b(param_2,&local_110);
    if (local_10c == (FILE *)0x0) {
      return 0;
    }
  }
  uVar1 = *param_3;
  while (uVar3 = FUN_00411db1(&local_10c->_ptr,local_110), uVar3 == 0) {
    uVar2 = 0xffff;
    do {
      uVar2 = uVar2 + 1;
      iVar4 = _fgetc(local_10c);
      *(char *)((int)local_108 + (uint)uVar2) = (char)iVar4;
      if (*(char *)((int)local_108 + (uint)uVar2) == '\0') break;
      uVar3 = FUN_00411db1(&local_10c->_ptr,local_110);
    } while (uVar3 == 0);
    uVar3 = FUN_00411db1(&local_10c->_ptr,local_110);
    if (uVar3 == 0) {
      if (uVar1 == 0x15e) break;
      uVar5 = FUN_0044c2f0(0xc);
      *(undefined4 *)(param_1 + (uint)uVar1 * 4) = uVar5;
      pcVar6 = FUN_0044c580(local_108);
      uVar5 = FUN_0044c2f0((SIZE_T)(pcVar6 + 1));
      **(undefined4 **)(param_1 + (uint)uVar1 * 4) = uVar5;
      FUN_0044ba00((uint *)**(undefined4 **)(param_1 + (uint)uVar1 * 4),local_108);
      uVar2 = 0xffff;
      do {
        uVar2 = uVar2 + 1;
        iVar4 = _fgetc(local_10c);
        *(char *)((int)local_108 + (uint)uVar2) = (char)iVar4;
        if (*(char *)((int)local_108 + (uint)uVar2) == '\0') break;
        uVar3 = FUN_00411db1(&local_10c->_ptr,local_110);
      } while (uVar3 == 0);
      pcVar6 = FUN_0044c580(local_108);
      uVar5 = FUN_0044c2f0((SIZE_T)(pcVar6 + 1));
      *(undefined4 *)(*(int *)(param_1 + (uint)uVar1 * 4) + 4) = uVar5;
      FUN_0044ba00(*(uint **)(*(int *)(param_1 + (uint)uVar1 * 4) + 4),local_108);
      iVar4 = _fgetc(local_10c);
      *(char *)(*(int *)(param_1 + (uint)uVar1 * 4) + 8) = (char)iVar4;
      *(undefined1 *)(*(int *)(param_1 + (uint)uVar1 * 4) + 9) = 0;
      uVar1 = uVar1 + 1;
    }
  }
  uVar3 = _fclose(local_10c);
  *param_3 = uVar1;
  return uVar3 & 0xffff0000;
}

