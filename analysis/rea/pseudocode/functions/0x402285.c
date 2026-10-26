// rea decompile_all
// name: FUN_00402285
// address: 0x402285
// body: available
// range: 0x402285-0x40241b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00402285(int param_1,int param_2)

{
  float10 fVar1;
  float10 extraout_ST0;
  float10 extraout_ST0_00;
  bool local_2c;
  int local_28;
  int local_24 [2];
  double local_1c;
  int local_14;
  int local_10 [3];
  
  local_10[0] = 0x140;
  local_10[1] = 0xf0;
  if ((DAT_0045c060 != 0) && (DAT_00464880 != (void *)0x0)) {
    if (param_2 < 1) {
      param_2 = 1;
    }
    FUN_004240ae(DAT_00464880,local_10,local_24);
    local_28 = FUN_0044bdb0(param_1 - local_24[0]);
    local_2c = local_24[0] < param_1;
    if (DAT_0046487c >> 1 < local_28) {
      local_2c = !local_2c;
      local_28 = DAT_0046487c - local_28;
    }
    if (DAT_00462ae8 == 0) {
      fVar1 = (float10)param_2 / (float10)DAT_0046487c;
    }
    else {
      __adj_fdiv_m32i();
      fVar1 = extraout_ST0;
    }
    local_1c = (double)(fVar1 * (float10)360.0);
    for (local_14 = 0; local_14 < local_28; local_14 = local_14 + param_2) {
      if (local_28 < param_2 + local_14) {
        if (DAT_00462ae8 == 0) {
          fVar1 = (float10)(local_28 - local_14) / (float10)DAT_0046487c;
        }
        else {
          __adj_fdiv_m32i();
          fVar1 = extraout_ST0_00;
        }
        local_1c = (double)(fVar1 * (float10)360.0);
      }
      if (local_2c) {
        FUN_00401ee6(local_1c._0_4_,(int)((ulonglong)local_1c >> 0x20),0,0,0);
      }
      else {
        FUN_00401ee6(SUB84(-local_1c,0),(int)((ulonglong)-local_1c >> 0x20),0,0,0);
      }
    }
  }
  return;
}

