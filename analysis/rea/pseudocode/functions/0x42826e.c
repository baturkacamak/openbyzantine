// rea decompile_all
// name: FUN_0042826e
// address: 0x42826e
// body: available
// range: 0x42826e-0x4283a5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0042826e(uint *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  MMRESULT MVar3;
  int *local_118 [2];
  FILE *local_110;
  uint local_10c [65];
  uint local_8;
  
  if ((param_1[0xc] != 0) || (iVar1 = FUN_0041d440(2), iVar1 != 0)) {
    FUN_00428f8c(local_10c,param_1,(uint *)&DAT_004601e0);
    if (param_1[5] == 2) {
      puVar2 = FUN_00422b04((LPSTR)local_10c,-2,(int *)(param_1 + 6));
      if (puVar2 != (undefined4 *)0x0) {
        param_1[0x15] = (uint)puVar2;
      }
    }
    else {
      local_110 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_004601e8);
      if (local_110 != (FILE *)0x0) {
        _fclose(local_110);
        MVar3 = FUN_004058fd((LPSTR)local_10c,&local_8,local_118);
        if (MVar3 == 0) {
          puVar2 = FUN_00428229(local_118[0],local_8);
          param_1[0x15] = (uint)puVar2;
          FUN_00427d1a((int *)(param_1 + 6));
          (**(code **)(*local_118[0] + 0x30))(local_118[0],0,0,1);
        }
      }
    }
  }
  return;
}

