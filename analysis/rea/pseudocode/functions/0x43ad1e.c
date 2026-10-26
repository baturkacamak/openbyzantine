// rea decompile_all
// name: FUN_0043ad1e
// address: 0x43ad1e
// body: available
// range: 0x43ad1e-0x43ad8d inclusive
// range: 0x43ad93-0x43adcd inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __thiscall FUN_0043ad1e(void *this,undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = AVIStreamRead(*(undefined4 *)((int)this + 0x28),param_1,1,
                        *(undefined4 *)((int)this + 0x5c),*(undefined4 *)((int)this + 0x3c),param_2,
                        0);
  if (iVar1 == -0x7ffbbf99) {
    OutputDebugStringA(s_AVIERR_MEMORY_00461858);
  }
  else if (iVar1 == -0x7ffbbf93) {
    OutputDebugStringA(s_AVIERR_FILEREAD_00461868);
  }
  else if (iVar1 == -0x7ffbbf8c) {
    OutputDebugStringA(s_AVIERR_BUFFERTOOSMALL_00461840);
  }
  return iVar1;
}

