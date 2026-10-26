// rea decompile_all
// name: _raise
// address: 0x4587c0
// body: available
// range: 0x4587c0-0x458913 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    _raise
   
   Library: Visual Studio 1998 Release */

int __cdecl _raise(int _SigNum)

{
  undefined4 uVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  code *pcVar7;
  undefined4 local_4;
  
  switch(_SigNum) {
  case 2:
    puVar6 = &DAT_0046400c;
    pcVar7 = DAT_0046400c;
    break;
  default:
    return -1;
  case 4:
  case 8:
  case 0xb:
    uVar3 = siglookup(_SigNum);
    puVar6 = (undefined4 *)(uVar3 + 8);
    pcVar7 = (code *)*puVar6;
    break;
  case 0xf:
    puVar6 = &DAT_00464018;
    pcVar7 = DAT_00464018;
    break;
  case 0x15:
    puVar6 = &DAT_00464010;
    pcVar7 = DAT_00464010;
    break;
  case 0x16:
    puVar6 = &DAT_00464014;
    pcVar7 = DAT_00464014;
  }
  if (pcVar7 == (code *)0x1) {
    return 0;
  }
  if (pcVar7 == (code *)0x0) {
    __exit(3);
  }
  uVar2 = DAT_00463908;
  uVar1 = DAT_00463904;
  if (((_SigNum == 8) || (_SigNum == 0xb)) || (uVar5 = local_4, _SigNum == 4)) {
    DAT_00463908 = 0;
    uVar5 = uVar2;
    if (_SigNum == 8) {
      DAT_00463904 = 0x8c;
      local_4 = uVar1;
      goto LAB_0045888c;
    }
  }
  else {
LAB_0045888c:
    if (_SigNum == 8) {
      if (DAT_004638f8 < DAT_004638fc + DAT_004638f8) {
        puVar6 = (undefined4 *)(DAT_004638f8 * 0xc + 0x463888);
        iVar4 = DAT_004638fc;
        do {
          *puVar6 = 0;
          puVar6 = puVar6 + 3;
          iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
      }
      goto LAB_004588cd;
    }
  }
  *puVar6 = 0;
LAB_004588cd:
  if (_SigNum == 8) {
    (*pcVar7)(8,DAT_00463904);
  }
  else {
    (*pcVar7)(_SigNum);
    if ((_SigNum != 0xb) && (_SigNum != 4)) {
      return 0;
    }
  }
  if (_SigNum == 8) {
    DAT_00463904 = local_4;
  }
  DAT_00463908 = uVar5;
  return 0;
}

