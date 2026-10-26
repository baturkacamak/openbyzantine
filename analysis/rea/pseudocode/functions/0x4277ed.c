// rea decompile_all
// name: FUN_004277ed
// address: 0x4277ed
// body: available
// range: 0x4277ed-0x4278aa inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004277ed(int param_1)

{
  int iVar1;
  LPVOID lpParameter;
  HANDLE pvVar2;
  DWORD local_8;
  
  iVar1 = FUN_00428143(param_1);
  if (iVar1 == 0) {
    lpParameter = (LPVOID)FUN_0044c2f0(0x18);
    *(undefined4 *)((int)lpParameter + 4) =
         *(undefined4 *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x2c);
    *(undefined4 *)((int)lpParameter + 0x10) = *(undefined4 *)(&DAT_00485c90 + param_1 * 4);
    *(undefined4 *)(&DAT_00485c90 + param_1 * 4) = 0;
    iVar1 = FUN_00422a0d();
    if (iVar1 == param_1) {
      *(undefined4 *)((int)lpParameter + 0xc) = 1;
    }
    else {
      *(undefined4 *)((int)lpParameter + 0xc) = 0;
    }
    pvVar2 = CreateThread((LPSECURITY_ATTRIBUTES)0x0,0,FUN_00427460,lpParameter,0,&local_8);
    *(HANDLE *)((int)lpParameter + 0x14) = pvVar2;
    if (*(int *)((int)lpParameter + 0x14) != 0) {
      DAT_00460198 = DAT_00460198 + 1;
    }
  }
  return;
}

