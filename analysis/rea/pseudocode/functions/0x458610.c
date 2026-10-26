// rea decompile_all
// name: _signal
// address: 0x458610
// body: available
// range: 0x458610-0x45873f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    _signal
   
   Library: Visual Studio 1998 Release */

void __cdecl _signal(int param_1)

{
  int *piVar1;
  uint uVar2;
  BOOL BVar3;
  int in_stack_00000008;
  
  if (in_stack_00000008 == 4) {
    DAT_00462c74 = 0x16;
    return;
  }
  if (in_stack_00000008 == 3) {
    DAT_00462c74 = 0x16;
    return;
  }
  if (param_1 != 2) {
    if (((param_1 != 0x15) && (param_1 != 0x16)) && (param_1 != 0xf)) {
      if (((param_1 != 8) && (param_1 != 4)) && (param_1 != 0xb)) {
        DAT_00462c74 = 0x16;
        return;
      }
      uVar2 = siglookup(param_1);
      if (uVar2 == 0) {
        DAT_00462c74 = 0x16;
        return;
      }
      if (*(int *)(uVar2 + 4) != param_1) {
        return;
      }
      do {
        *(int *)(uVar2 + 8) = in_stack_00000008;
        piVar1 = (int *)(uVar2 + 0x10);
        uVar2 = uVar2 + 0xc;
      } while (*piVar1 == param_1);
      return;
    }
    if ((param_1 != 2) && (param_1 != 0x15)) goto LAB_004586ad;
  }
  if (DAT_0046401c == 0) {
    BVar3 = SetConsoleCtrlHandler((PHANDLER_ROUTINE)&LAB_00458770,1);
    if (BVar3 != 1) {
      DAT_00462c78 = GetLastError();
      DAT_00462c74 = 0x16;
      return;
    }
    DAT_0046401c = 1;
  }
LAB_004586ad:
  switch(param_1) {
  case 2:
    DAT_0046400c = in_stack_00000008;
    return;
  default:
    return;
  case 0xf:
    DAT_00464018 = in_stack_00000008;
    return;
  case 0x15:
    DAT_00464010 = in_stack_00000008;
    return;
  case 0x16:
    DAT_00464014 = in_stack_00000008;
    return;
  }
}

