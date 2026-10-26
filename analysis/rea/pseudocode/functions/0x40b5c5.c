// rea decompile_all
// name: FUN_0040b5c5
// address: 0x40b5c5
// body: available
// range: 0x40b5c5-0x40b717 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __fastcall FUN_0040b5c5(int param_1)

{
  int iVar1;
  double dVar2;
  LONG LVar3;
  float10 fVar4;
  float10 extraout_ST0;
  longlong lVar5;
  undefined1 local_34 [4];
  double local_30;
  double local_28;
  uint local_20;
  undefined8 local_1c;
  double local_14;
  double local_c;
  
  local_30 = (double)*(int *)(param_1 + 0x2c);
  local_c = (double)*(uint *)(param_1 + 0x14);
  local_1c = (double)*(uint *)(param_1 + 0x18);
  if (*(int *)(param_1 + 0x20) != 0) {
    while (LVar3 = InterlockedExchange((LONG *)(param_1 + 0x10),1), LVar3 != 0) {
      Sleep(2);
    }
    iVar1 = *(int *)(param_1 + 0xc);
    InterlockedExchange((LONG *)(param_1 + 0x10),0);
    if (iVar1 == 0) {
      return 0xffffffff;
    }
  }
  (**(code **)(**(int **)(param_1 + 4) + 0x10))(*(undefined4 *)(param_1 + 4),&local_20,local_34);
  dVar2 = local_30;
  if (*(uint *)(param_1 + 0x38) <= local_20) {
    dVar2 = local_30 - 1.0;
  }
  local_14 = dVar2 * local_c + (double)local_20;
  if (DAT_00462ae8 == 0) {
    fVar4 = (float10)local_14 / (float10)local_1c;
  }
  else {
    __adj_fdiv_m64((undefined4)local_1c,local_1c._4_4_);
    fVar4 = extraout_ST0;
  }
  local_28 = (double)fVar4;
  lVar5 = __ftol();
  return (int)lVar5;
}

