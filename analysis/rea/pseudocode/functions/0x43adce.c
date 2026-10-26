// rea decompile_all
// name: FUN_0043adce
// address: 0x43adce
// body: available
// range: 0x43adce-0x43af72 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __thiscall FUN_0043adce(void *this,undefined4 param_1,undefined4 param_2)

{
  int local_94;
  int local_90 [33];
  undefined4 local_c;
  undefined4 local_8;
  
  *(undefined4 *)(*(int *)((int)this + 0x54) + 0x14) = param_2;
  FUN_004146a7(*(void **)((int)this + 8),&local_8,&local_94,local_90);
  *(int *)(*(int *)((int)this + 0x58) + 4) = local_94 / 2;
  *(int *)(*(int *)((int)this + 0x58) + 8) = -local_90[0];
  if (*(int *)((int)this + 0x40) == 0x31345649) {
    local_c = FUN_0043d73d(*(void **)((int)this + 0xc),param_1,*(undefined4 *)((int)this + 0x54),
                           *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44),
                           *(undefined4 *)((int)this + 0x48),*(int *)((int)this + 0x58),local_8,
                           *(undefined4 *)((int)this + 0x2c),
                           (local_90[0] - *(int *)((int)this + 0x30)) - *(int *)((int)this + 0x50),
                           *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
  }
  else {
    local_c = FUN_0043d73d(*(void **)((int)this + 0xc),param_1,*(undefined4 *)((int)this + 0x54),
                           *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44),
                           *(undefined4 *)((int)this + 0x48),*(int *)((int)this + 0x58),local_8,
                           *(undefined4 *)((int)this + 0x2c),*(undefined4 *)((int)this + 0x30),
                           *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
  }
  FUN_00414a53(*(int *)((int)this + 8));
  return local_c;
}

