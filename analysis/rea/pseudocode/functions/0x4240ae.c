// rea decompile_all
// name: FUN_004240ae
// address: 0x4240ae
// body: available
// range: 0x4240ae-0x4241e1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __thiscall FUN_004240ae(void *this,int *param_1,int *param_2)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  longlong lVar4;
  
  if (*(int *)((int)this + 0x34) == 0) {
    uVar2 = 1;
  }
  else if ((param_2 == (int *)0x0) || (param_1 == (int *)0x0)) {
    uVar2 = 1;
  }
  else {
    iVar3 = *param_1;
    iVar1 = param_1[1];
    if ((iVar3 < *(int *)((int)this + 4)) &&
       (((-1 < iVar3 && (iVar1 < *(int *)((int)this + 8))) && (-1 < iVar1)))) {
      lVar4 = __ftol();
      iVar3 = *(int *)(*(int *)((int)this + 0x3c) + (*(int *)((int)this + 4) * iVar1 + iVar3) * 4) +
              (int)lVar4;
      *param_2 = iVar3 % *(int *)((int)this + 0x1c) - *(int *)((int)this + 0x38);
      while (*(int *)((int)this + 0x24) <= *param_2) {
        *param_2 = *param_2 - *(int *)((int)this + 0x24);
      }
      while (*param_2 < 0) {
        *param_2 = *param_2 + *(int *)((int)this + 0x24);
      }
      param_2[1] = iVar3 / *(int *)((int)this + 0x1c);
      uVar2 = 0;
    }
    else {
      uVar2 = 1;
    }
  }
  return uVar2;
}

