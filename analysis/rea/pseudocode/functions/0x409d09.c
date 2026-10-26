// rea decompile_all
// name: FUN_00409d09
// address: 0x409d09
// body: available
// range: 0x409d09-0x409db8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00409d09(int param_1)

{
  uint local_8;
  
  FUN_0040a1e0(param_1,s_ANIMPOS_0045d468);
  local_8 = CONCAT22(local_8._2_2_,1);
  while ((local_8 & 0xffff) < 3) {
    if (*(char *)((local_8 & 0xffff) + 1 + param_1) == '@') {
      FUN_0040a40d(param_1,(byte)local_8,'\x01');
    }
    else if (*(char *)((local_8 & 0xffff) + 1 + param_1) == '\x02') {
      FUN_0040a4bc(param_1,local_8,'\x01','\x01');
    }
    local_8 = CONCAT22(local_8._2_2_,(short)local_8 + 1);
  }
  FUN_004094f2();
  return;
}

