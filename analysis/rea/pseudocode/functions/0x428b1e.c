// rea decompile_all
// name: FUN_00428b1e
// address: 0x428b1e
// body: available
// range: 0x428b1e-0x428cbc inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00428b1e(int param_1,int param_2)

{
  float fVar1;
  bool bVar2;
  int iVar3;
  float10 fVar4;
  float10 extraout_ST0;
  float10 extraout_ST0_00;
  longlong lVar5;
  int local_1c;
  int local_10;
  
  iVar3 = param_1;
  local_10 = 6000;
  if (param_1 == 0) {
    local_10 = 10000;
  }
  else if (param_2 == 0) {
    local_10 = -10000;
  }
  else {
    bVar2 = param_2 < param_1;
    if (bVar2) {
      param_1 = param_2;
      param_2 = iVar3;
    }
    if (DAT_00462ae8 == 0) {
      fVar4 = (float10)param_1 / (float10)param_2;
    }
    else {
      __adj_fdiv_m32i();
      fVar4 = extraout_ST0;
    }
    fVar1 = (float)(fVar4 * (float10)100.0);
    for (local_1c = 0; local_1c < 5; local_1c = local_1c + 1) {
      if ((fVar1 <= (float)*(int *)(&DAT_004601c0 + local_1c * 4)) &&
         ((float)*(int *)(&DAT_004601c4 + local_1c * 4) <= fVar1)) {
        if (DAT_00462ae8 == 0) {
          fVar4 = ((float10)*(int *)(&DAT_004601c0 + local_1c * 4) - (float10)fVar1) /
                  (float10)(*(int *)(&DAT_004601c0 + local_1c * 4) -
                           *(int *)(&DAT_004601c4 + local_1c * 4));
        }
        else {
          __adj_fdiv_m32i();
          fVar4 = extraout_ST0_00;
        }
        _floor((double)((float)(*(int *)(&DAT_004601ac + local_1c * 4) -
                               *(int *)(&DAT_004601a8 + local_1c * 4)) * (float)fVar4 +
                        (float)*(int *)(&DAT_004601a8 + local_1c * 4) + 0.5));
        lVar5 = __ftol();
        local_10 = (int)lVar5;
        break;
      }
    }
    if (bVar2) {
      local_10 = -local_10;
    }
  }
  return local_10;
}

