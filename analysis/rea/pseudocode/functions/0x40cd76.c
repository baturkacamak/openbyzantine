// rea decompile_all
// name: FUN_0040cd76
// address: 0x40cd76
// body: available
// range: 0x40cd76-0x40cec8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040cd76(int param_1,char param_2,int param_3)

{
  ushort uVar1;
  int unaff_EDI;
  byte abStack_608 [768];
  uint local_308;
  char local_304 [768];
  
  local_308 = local_308 & 0xffff0000;
  while ((local_308 & 0xffff) < 0x300) {
    abStack_608[local_308 & 0xffff] = 0;
    local_308 = CONCAT22(local_308._2_2_,(short)local_308 + 1);
  }
  local_308 = local_308 & 0xffff0000;
  do {
    if (param_2 != '\0') {
      FUN_0043b216((undefined4 *)0x0,0,unaff_EDI);
    }
    for (uVar1 = 0; uVar1 < 0x300; uVar1 = uVar1 + 1) {
      abStack_608[uVar1] = *(byte *)((uint)uVar1 + param_1);
      local_304[uVar1] =
           (char)((longlong)(ulonglong)abStack_608[uVar1] / (longlong)param_3) * (char)local_308;
    }
    FUN_004180f4((int)local_304);
    FUN_00426f95(0x32);
    uVar1 = (short)local_308 + 1;
    local_308 = CONCAT22(local_308._2_2_,uVar1);
  } while ((int)(uint)uVar1 < param_3);
  FUN_004180f4(param_1);
  return;
}

