// rea decompile_all
// name: FUN_00423cdf
// address: 0x423cdf
// body: available
// range: 0x423cdf-0x423d9d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


ulonglong __fastcall FUN_00423cdf(void *param_1)

{
  int iVar1;
  int iVar2;
  double dVar3;
  float10 fVar4;
  float10 fVar5;
  float10 extraout_ST0;
  ulonglong uVar6;
  undefined4 local_1c;
  undefined4 uStack_18;
  
  fVar4 = (float10)FUN_00423c42(param_1,1.0);
  dVar3 = *(double *)((int)param_1 + 0x28);
  iVar1 = *(int *)((int)param_1 + 4);
  iVar2 = *(int *)((int)param_1 + 8);
  uStack_18 = (int)((ulonglong)(double)fVar4 >> 0x20);
  local_1c = SUB84((double)fVar4,0);
  fVar4 = (float10)FUN_0044bd8a();
  fVar5 = (float10)FUN_0044bd80(local_1c,uStack_18);
  if (DAT_00462ae8 == 0) {
    fVar4 = (float10)(iVar1 >> 1) /
            ((float10)(double)(fVar4 * (float10)dVar3) - fVar5 * (float10)(iVar2 >> 1));
  }
  else {
    __adj_fdivr_m32i();
    fVar4 = extraout_ST0;
  }
  FUN_0044d431(SUB84((double)fVar4,0),(int)((ulonglong)(double)fVar4 >> 0x20));
  uVar6 = __ftol();
  return uVar6;
}

