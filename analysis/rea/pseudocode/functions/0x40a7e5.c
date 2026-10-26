// rea decompile_all
// name: FUN_0040a7e5
// address: 0x40a7e5
// body: available
// range: 0x40a7e5-0x40a896 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040a7e5(undefined4 param_1,int *param_2,int param_3)

{
  ushort uVar1;
  char *pcVar2;
  byte bVar3;
  
  if (DAT_00485c6c == '\0') {
    FUN_00424780((int)param_2,param_3);
  }
  else {
    FUN_00409457(&DAT_0045d5b0,'\0');
    FUN_00436652(*param_2,2);
    FUN_004599c0((byte *)s___s__s__0045d5b4);
    bVar3 = 0;
    uVar1 = FUN_00436652(*param_2,2);
    pcVar2 = FUN_0044c580((uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar1 * 4));
    FUN_0040a897(param_2[param_3 + 0x1c8],(char)pcVar2 + 2,bVar3);
    FUN_0040aa02();
  }
  return;
}

