// rea decompile_all
// name: FUN_004275ce
// address: 0x4275ce
// body: available
// range: 0x4275ce-0x4276b0 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004275ce(int param_1,int param_2,int param_3)

{
  int *lpParameter;
  HANDLE pvVar1;
  DWORD local_8;
  
  if (param_2 == 0) {
    if (*(int *)(&DAT_00485c90 + param_1 * 4) == 0) {
      return;
    }
    param_2 = *(int *)(&DAT_00485c90 + param_1 * 4);
  }
  if (*(int *)(&DAT_00486220 + param_1 * 4) != 0) {
    lpParameter = (int *)FUN_0044c2f0(0x18);
    lpParameter[4] = param_2;
    *(undefined4 *)(&DAT_00485c90 + param_1 * 4) = 0;
    *lpParameter = param_1;
    lpParameter[1] = *(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x28);
    lpParameter[2] = *(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x1c) + -10000;
    lpParameter[3] = param_3;
    pvVar1 = CreateThread((LPSECURITY_ATTRIBUTES)0x0,0,FUN_004276b1,lpParameter,0,&local_8);
    lpParameter[5] = (int)pvVar1;
    if (lpParameter[5] != 0) {
      DAT_00460198 = DAT_00460198 + 1;
    }
  }
  return;
}

