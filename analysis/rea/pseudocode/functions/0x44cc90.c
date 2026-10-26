// rea decompile_all
// name: FUN_0044cc90
// address: 0x44cc90
// body: available
// range: 0x44cc90-0x44cd16 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_0044cc90(FILETIME *param_1)

{
  BOOL BVar1;
  int iVar2;
  _SYSTEMTIME local_18;
  _FILETIME local_8;
  
  if ((param_1->dwLowDateTime == 0) && (param_1->dwHighDateTime == 0)) {
    return -1;
  }
  BVar1 = FileTimeToLocalFileTime(param_1,&local_8);
  if (BVar1 != 0) {
    BVar1 = FileTimeToSystemTime(&local_8,&local_18);
    if (BVar1 != 0) {
      iVar2 = FUN_00451cb0(local_18._0_4_ & 0xffff,(uint)local_18._0_4_ >> 0x10,(uint)local_18.wDay,
                           local_18._8_4_ & 0xffff,(uint)local_18._8_4_ >> 0x10,
                           local_18._12_4_ & 0xffff);
      return iVar2;
    }
  }
  return -1;
}

