// rea decompile_all
// name: FUN_004124d9
// address: 0x4124d9
// body: available
// range: 0x4124d9-0x412761 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_004124d9(void)

{
  ushort uVar1;
  ushort uVar2;
  FILE *pFVar3;
  uint uVar4;
  undefined4 uVar5;
  int iVar6;
  char *pcVar7;
  undefined2 extraout_var;
  int local_1c;
  uint local_18 [5];
  
  local_1c = -1;
  if (DAT_0045e7a8 == 0) {
    _sprintf((char *)local_18,s_engine__s_0045ea8c,&DAT_00483060);
    pFVar3 = (FILE *)FUN_0040f350((char *)local_18,&DAT_0045ea98);
  }
  else {
    pFVar3 = FUN_00411b4b((uint *)&DAT_00483060,&local_1c);
  }
  if (pFVar3 == (FILE *)0x0) {
    uVar5 = 1;
  }
  else {
    __fread_lk((char *)&DAT_0047a8a0,2,1,pFVar3);
    _fclose(pFVar3);
    if (DAT_0045e7a8 == 0) {
      _sprintf((char *)local_18,s_engine__s_0045ea9c,&DAT_00483610);
      pFVar3 = (FILE *)FUN_0040f350((char *)local_18,&DAT_0045eaa8);
    }
    else {
      pFVar3 = FUN_00411b4b((uint *)&DAT_00483610,&local_1c);
    }
    if (pFVar3 == (FILE *)0x0) {
      uVar5 = 1;
    }
    else {
      uVar2 = 0;
      while( true ) {
        uVar4 = FUN_00411db1(&pFVar3->_ptr,local_1c);
        if ((uVar4 != 0) || (DAT_0047a8a0 <= uVar2)) break;
        uVar5 = FUN_0044c2f0(8);
        *(undefined4 *)(&DAT_0047ae80 + (uint)uVar2 * 4) = uVar5;
        uVar1 = 0xffff;
        do {
          uVar1 = uVar1 + 1;
          iVar6 = _fgetc(pFVar3);
          *(char *)((int)local_18 + (uint)uVar1) = (char)iVar6;
        } while (*(char *)((int)local_18 + (uint)uVar1) != '\0');
        pcVar7 = FUN_0044c580(local_18);
        uVar5 = FUN_0044c2f0((SIZE_T)(pcVar7 + 1));
        **(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4) = uVar5;
        FUN_0044ba00((uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4),local_18);
        iVar6 = _fgetc(pFVar3);
        *(char *)(*(int *)(&DAT_0047ae80 + (uint)uVar2 * 4) + 4) = (char)iVar6;
        iVar6 = _fgetc(pFVar3);
        *(char *)(*(int *)(&DAT_0047ae80 + (uint)uVar2 * 4) + 5) = (char)iVar6;
        uVar2 = uVar2 + 1;
      }
      _fclose(pFVar3);
      if (DAT_0047a8a0 == uVar2) {
        uVar5 = 0;
      }
      else {
        FUN_0040abb4();
        uVar5 = CONCAT22(extraout_var,1);
      }
    }
  }
  return uVar5;
}

