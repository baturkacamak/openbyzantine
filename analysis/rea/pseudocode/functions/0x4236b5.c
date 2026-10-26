// rea decompile_all
// name: FUN_004236b5
// address: 0x4236b5
// body: available
// range: 0x4236b5-0x42382a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __thiscall FUN_004236b5(void *this,int param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  undefined4 extraout_EDX;
  float10 fVar2;
  float10 extraout_ST0;
  ulonglong uVar3;
  int local_8;
  
  if (param_1 == 0) {
    uVar1 = 1;
  }
  else {
    if ((*(int *)((int)this + 0x24) != param_2) || (*(int *)((int)this + 0x20) != param_3)) {
      if (*(int *)((int)this + 0x34) != 0) {
        FUN_0044a820(*(LPVOID *)((int)this + 0x34));
      }
      *(int *)((int)this + 0x24) = param_2;
      *(int *)((int)this + 0x20) = param_3;
      if (DAT_00462ae8 == 0) {
        fVar2 = (float10)param_2 / (float10)6.28318530718;
      }
      else {
        __adj_fdiv_m64(0x54442eea,0x401921fb);
        fVar2 = extraout_ST0;
      }
      *(double *)((int)this + 0x28) = (double)fVar2;
      uVar3 = FUN_00423cdf(this);
      *(int *)((int)this + 0x38) = (int)uVar3 + 1;
      *(int *)((int)this + 0x1c) = *(int *)((int)this + 0x38) * 2 + param_2;
      uVar1 = FUN_0044ac60(*(int *)((int)this + 0x1c) * *(int *)((int)this + 0x20) * 2);
      *(undefined4 *)((int)this + 0x34) = uVar1;
      if (*(int *)((int)this + 0x34) == 0) {
        return 1;
      }
    }
    for (local_8 = 0; local_8 < *(int *)((int)this + 0x20); local_8 = local_8 + 1) {
      FID_conflict__memcpy
                ((void *)(*(int *)((int)this + 0x38) * 2 + *(int *)((int)this + 0x1c) * local_8 * 2
                         + *(int *)((int)this + 0x34)),(void *)(local_8 * param_2 * 2 + param_1),
                 param_2 * 2);
    }
    FUN_0042382b((int)this);
    FUN_004241e2((int)this,extraout_EDX);
    uVar1 = 0;
  }
  return uVar1;
}

