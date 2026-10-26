// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4587c0
// name: _raise
// ranges: 0x4587c0-0x458913
// calls: -

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
