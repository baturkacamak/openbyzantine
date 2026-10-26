// rea decompile_all
// name: FUN_00429d47
// address: 0x429d47
// body: available
// range: 0x429d47-0x42a004 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00429d47(int param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  
  if (param_2 < param_1) {
    FUN_00429d1a(&param_1,&param_2);
  }
  if (param_3 < param_1) {
    FUN_00429d1a(&param_1,&param_3);
  }
  if (param_3 < param_2) {
    FUN_00429d1a(&param_2,&param_3);
  }
  if (((param_1 == 0) && (param_2 == 1)) && (param_3 == 2)) {
    uVar1 = 0;
  }
  else if (((param_1 == 2) && (param_2 == 0xe)) && (param_3 == 0x17)) {
    uVar1 = 1;
  }
  else if (((param_1 == 0x15) && (param_2 == 0x16)) && (param_3 == 0x17)) {
    uVar1 = 2;
  }
  else if (((param_1 == 0) && (param_2 == 9)) && (param_3 == 0x15)) {
    uVar1 = 3;
  }
  else if (((param_1 == 3) && (param_2 == 4)) && (param_3 == 5)) {
    uVar1 = 4;
  }
  else if (((param_1 == 5) && (param_2 == 0xd)) && (param_3 == 0x14)) {
    uVar1 = 5;
  }
  else if (((param_1 == 0x12) && (param_2 == 0x13)) && (param_3 == 0x14)) {
    uVar1 = 6;
  }
  else if (((param_1 == 3) && (param_2 == 10)) && (param_3 == 0x12)) {
    uVar1 = 7;
  }
  else if (((param_1 == 6) && (param_2 == 7)) && (param_3 == 8)) {
    uVar1 = 8;
  }
  else if (((param_1 == 8) && (param_2 == 0xc)) && (param_3 == 0x11)) {
    uVar1 = 9;
  }
  else if (((param_1 == 0xf) && (param_2 == 0x10)) && (param_3 == 0x11)) {
    uVar1 = 10;
  }
  else if (((param_1 == 6) && (param_2 == 0xb)) && (param_3 == 0xf)) {
    uVar1 = 0xb;
  }
  else if (((param_1 == 1) && (param_2 == 4)) && (param_3 == 7)) {
    uVar1 = 0xc;
  }
  else if (((param_1 == 0xc) && (param_2 == 0xd)) && (param_3 == 0xe)) {
    uVar1 = 0xd;
  }
  else if (((param_1 == 0x10) && (param_2 == 0x13)) && (param_3 == 0x16)) {
    uVar1 = 0xe;
  }
  else {
    uVar1 = 0xf;
  }
  return uVar1;
}

