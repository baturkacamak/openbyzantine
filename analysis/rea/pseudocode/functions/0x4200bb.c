// rea decompile_all
// name: FUN_004200bb
// address: 0x4200bb
// body: available
// range: 0x4200bb-0x4201cb inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_004200bb(int param_1,char *param_2)

{
  int *piVar1;
  uint uVar2;
  short sVar3;
  FILE *_File;
  undefined4 uVar4;
  int iVar5;
  int local_10;
  int local_c;
  
  local_c = 0;
  _File = (FILE *)FUN_0044bc20(param_2,&DAT_0045fbdc);
  if (_File != (FILE *)0x0) {
    sVar3 = FUN_0040cb26(_File,(char *)0x0);
    local_c = (int)sVar3;
  }
  for (local_10 = 0; local_10 < local_c; local_10 = local_10 + 1) {
    uVar4 = FUN_0044c2f0(0xc);
    *(undefined4 *)(param_1 + local_10 * 4) = uVar4;
    piVar1 = *(int **)(param_1 + local_10 * 4);
    iVar5 = FUN_0044c2f0(0x1a);
    piVar1[1] = iVar5;
    __fread_lk((char *)piVar1[1],0x1a,1,_File);
    uVar2 = *(uint *)(piVar1[1] + 0x16);
    iVar5 = FUN_0044c2f0(uVar2 * 2);
    *piVar1 = iVar5;
    __fread_lk((char *)*piVar1,uVar2,2,_File);
    FUN_0040be80(*piVar1,(int)*(short *)(piVar1[1] + 4));
  }
  _fclose(_File);
  return local_c;
}

