// rea decompile_all
// name: FUN_0042875d
// address: 0x42875d
// body: available
// range: 0x42875d-0x4287fe inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0042875d(int param_1)

{
  MMRESULT MVar1;
  uint local_8;
  
  if (param_1 == 1) {
    if ((DAT_004601a0 == (int *)0x0) &&
       (MVar1 = FUN_004058fd(s_sounds_pxstart_wav_00460204,&local_8,&DAT_004601a0), MVar1 == 0)) {
      (**(code **)(*DAT_004601a0 + 0x30))(DAT_004601a0,0,0,1);
    }
  }
  else if (DAT_004601a0 != (int *)0x0) {
    (**(code **)(*DAT_004601a0 + 0x48))(DAT_004601a0);
    (**(code **)(*DAT_004601a0 + 8))(DAT_004601a0);
    DAT_004601a0 = (int *)0x0;
  }
  return;
}

