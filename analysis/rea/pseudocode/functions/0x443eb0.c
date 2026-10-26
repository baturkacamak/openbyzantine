// rea decompile_all
// name: FUN_00443eb0
// address: 0x443eb0
// body: available
// range: 0x443eb0-0x443ff0 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00443eb0(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  undefined4 *local_c;
  int *local_8;
  int local_4;
  
  iVar1 = param_1[0x65];
  if ((((param_1[0x5a] != 0) || (param_1[0x5b] != 0x3f)) || (param_1[0x5c] != 0)) ||
     (param_1[0x5d] != 0)) {
    *(undefined4 *)(*param_1 + 0x14) = 0x76;
    (**(code **)(*param_1 + 4))(param_1,0xffffffff);
  }
  local_4 = 0;
  if (0 < param_1[0x48]) {
    local_c = (undefined4 *)(iVar1 + 0x14);
    local_8 = param_1 + 0x49;
    do {
      iVar2 = *(int *)(*local_8 + 0x14);
      iVar3 = *(int *)(*local_8 + 0x18);
      if (((iVar2 < 0) || (3 < iVar2)) || (param_1[iVar2 + 0x28] == 0)) {
        *(undefined4 *)(*param_1 + 0x14) = 0x31;
        *(int *)(*param_1 + 0x18) = iVar2;
        (**(code **)*param_1)(param_1);
      }
      if (((iVar3 < 0) || (3 < iVar3)) || (param_1[iVar3 + 0x2c] == 0)) {
        *(undefined4 *)(*param_1 + 0x14) = 0x31;
        *(int *)(*param_1 + 0x18) = iVar3;
        (**(code **)*param_1)(param_1);
      }
      pbVar4 = FUN_00443a70((int)param_1,param_1[iVar2 + 0x28],(int *)(iVar1 + iVar2 * 4 + 0x28));
      FUN_00443a70((int)param_1,param_1[(int)(pbVar4 + 0x2c)],
                   (int *)(iVar1 + (int)pbVar4 * 4 + 0x38));
      local_4 = local_4 + 1;
      *local_c = 0;
      local_c = local_c + 1;
      local_8 = local_8 + 1;
    } while (local_4 < param_1[0x48]);
  }
  *(undefined4 *)(iVar1 + 0xc) = 0;
  *(undefined4 *)(iVar1 + 8) = 0;
  *(undefined1 *)(iVar1 + 0x10) = 0;
  *(int *)(iVar1 + 0x24) = param_1[0x3f];
  return;
}

