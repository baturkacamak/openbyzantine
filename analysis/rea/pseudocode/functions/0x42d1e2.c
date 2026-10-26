// rea decompile_all
// name: FUN_0042d1e2
// address: 0x42d1e2
// body: available
// range: 0x42d1e2-0x42d2d8 inclusive
// range: 0x42d2de-0x42d2fd inclusive
// range: 0x42d347-0x42d382 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort __cdecl FUN_0042d1e2(int param_1)

{
  ushort uVar1;
  
  for (uVar1 = 0; *(char *)((uint)uVar1 + param_1) == '-'; uVar1 = uVar1 + 2) {
    switch(*(undefined1 *)((uint)(ushort)(uVar1 + 1) + param_1)) {
    case 0x62:
      _DAT_0046e270 = 1;
      break;
    default:
      FUN_004599c0((byte *)s__s_is_an_illegal_option_00460c50);
      break;
    case 100:
      _DAT_0046e828 = 1;
      break;
    case 0x65:
      _DAT_00484308 = 1;
      break;
    case 0x6d:
      DAT_0047a8a2 = 1;
      break;
    case 0x6e:
      DAT_0047734a = 1;
      break;
    case 0x6f:
      DAT_00483070 = 1;
      break;
    case 0x73:
      DAT_00485c74 = 1;
      break;
    case 0x74:
      DAT_00485c6c = '\x01';
      break;
    case 0x76:
      DAT_00485990 = 1;
      break;
    case 0x77:
      DAT_0047a280 = 1;
      break;
    case 0x7a:
      _DAT_0046edf0 = 1;
    }
  }
  if (DAT_00485c6c == '\0') {
    DAT_00485c74 = 1;
  }
  else {
    _DAT_0046e828 = 0;
  }
  return uVar1;
}

