// rea decompile_all
// name: FUN_0041c225
// address: 0x41c225
// body: available
// range: 0x41c225-0x41c2ba inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __thiscall FUN_0041c225(void *this,undefined4 *param_1)

{
  uint uVar1;
  int iVar2;
  
  if (*(int *)((int)this + 0xc) == 0x31347669) {
    *(undefined4 *)((int)this + 200) = 0x80000800;
    *(undefined4 *)((int)this + 0xc0) = 2;
    iVar2 = ICSendMessage(*(undefined4 *)this,0x5000,(int)this + 0xb4,
                          *(undefined4 *)((int)this + 0xb4));
    *param_1 = *(undefined4 *)((int)this + 0x118);
    uVar1 = (uint)(iVar2 == 0);
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}

