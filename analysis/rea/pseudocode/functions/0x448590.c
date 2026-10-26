// rea decompile_all
// name: FUN_00448590
// address: 0x448590
// body: available
// range: 0x448590-0x4485ee inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00448590(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x400);
  iVar4 = 0;
  do {
    iVar3 = 0;
    do {
      iVar1 = iVar3 + iVar4;
      iVar3 = iVar3 + 1;
      *(int *)(iVar2 + iVar1 * 4) =
           (int)((uint)(byte)(&DAT_0045b6b8)[iVar1] * -0x1fe + 0xfe01) / ((param_2 + -1) * 0x200);
    } while (iVar3 < 0x10);
    iVar4 = iVar4 + 0x10;
  } while (iVar4 < 0x100);
  return iVar2;
}

