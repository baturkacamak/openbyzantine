// rea decompile_all
// name: FUN_0043ac17
// address: 0x43ac17
// body: available
// range: 0x43ac17-0x43ad1d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __fastcall FUN_0043ac17(undefined4 *param_1)

{
  int iVar1;
  undefined4 local_8;
  
  FUN_0040b368(param_1[1]);
  FUN_0043ad1e(param_1,param_1[0x20],&local_8);
  FUN_0043adce(param_1,0,local_8);
  iVar1 = FUN_0043af73(param_1,1);
  if (iVar1 == 0) {
    param_1[0x21] = param_1[0x21] + 1;
  }
  FUN_0043d856(param_1[3]);
  if (param_1[0x17] != 0) {
    FUN_0044a820((LPVOID)param_1[0x17]);
    param_1[0x17] = 0;
  }
  if (param_1[0x16] != 0) {
    FUN_0044ac40((LPVOID)param_1[0x16]);
    param_1[0x16] = 0;
  }
  FUN_0043a20d((int)param_1);
  FUN_0043a03b((int)param_1);
  FUN_00439dbb((int)param_1);
  PostMessageA((HWND)*param_1,0x111,0x9c44,param_1[0x21]);
  param_1[4] = 0;
  return 0;
}

