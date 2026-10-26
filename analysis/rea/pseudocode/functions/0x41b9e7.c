// rea decompile_all
// name: FUN_0041b9e7
// address: 0x41b9e7
// body: available
// range: 0x41b9e7-0x41bb09 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __thiscall FUN_0041b9e7(void *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  int iVar1;
  undefined4 local_c;
  
  if (param_2 == 0) {
    param_2 = *(int *)((int)this + 0x2c);
  }
  if (param_3 == 0) {
    param_3 = *(int *)((int)this + 0x30);
  }
  if (param_4 == 0) {
    param_4 = *(int *)((int)this + 0x34);
  }
  if (param_5 == 0) {
    param_5 = *(int *)((int)this + 0x38);
  }
  if (param_1 == 0) {
    local_c = *(int *)((int)this + 0x18);
  }
  else {
    local_c = param_1;
  }
  if ((((param_2 < 0) || (param_3 < 0)) || (*(int *)(local_c + 4) < param_4 + param_2)) ||
     (*(int *)(local_c + 8) < param_3 + param_5)) {
    iVar1 = -1;
  }
  else {
    iVar1 = FUN_0041b0db(*(undefined4 *)this,0,*(undefined4 *)((int)this + 0x14),0,
                         *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                         *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),local_c
                         ,0,param_2,param_3,param_4,param_5);
    if (iVar1 == 0) {
      iVar1 = 0;
    }
  }
  return iVar1;
}

