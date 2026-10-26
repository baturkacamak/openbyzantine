// rea decompile_all
// name: FUN_0041045b
// address: 0x41045b
// body: available
// range: 0x41045b-0x410606 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


bool __cdecl FUN_0041045b(int param_1,uint param_2,int param_3)

{
  int iVar1;
  FILE *_File;
  ushort *puVar2;
  bool bVar3;
  uint local_88 [15];
  int local_4c [3];
  uint local_40 [15];
  
  if (param_3 == 4) {
    iVar1 = FUN_0040860e((char *)local_88);
    if (iVar1 == -1) {
      return true;
    }
    FUN_0040c624(local_40,(uint *)s_inventor_examine_0045e874,local_88,(uint *)&DAT_0045e86c);
    FUN_0044bc20((char *)local_40,&DAT_0045e888);
  }
  else {
    _sprintf((char *)local_88,s__c_c__s_0045e88c,
             (uint)*(byte *)**(undefined4 **)(&DAT_00484e80 + (param_2 & 0xffff) * 4),
             (uint)*(byte *)(**(int **)(&DAT_00484e80 + (param_2 & 0xffff) * 4) + 1),
             **(undefined4 **)(&DAT_00484e80 + (param_2 & 0xffff) * 4));
    FUN_0040c624(local_40,(uint *)s_backs_0045e89c,local_88,(uint *)&DAT_0045e894);
  }
  _File = (FILE *)FUN_0044bc20((char *)local_40,&DAT_0045e8a4);
  if (_File == (FILE *)0x0) {
    FUN_0040ac43();
    FUN_0040ac38();
    bVar3 = true;
  }
  else {
    _fclose(_File);
    local_4c[0] = 0x280;
    local_4c[1] = 400;
    puVar2 = FUN_0040f135((char *)local_40,local_4c,(uint *)(local_4c + 1),
                          (ushort *)(param_1 + 0x6400),0x280);
    bVar3 = puVar2 == (ushort *)0x0;
  }
  return bVar3;
}

