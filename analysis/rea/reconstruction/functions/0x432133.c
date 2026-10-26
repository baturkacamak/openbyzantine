// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432133
// name: FUN_00432133
// ranges: 0x432133-0x432178; 0x43217e-0x432191; 0x4321a2-0x4321e3; 0x4321e9-0x432204; 0x432286-0x432294
// calls: 0x432133

undefined4 __cdecl FUN_00432133(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  if (param_2 == 0) {
    switch(param_1) {
    case 0:
    case 100:
      uVar1 = 100;
      break;
    case 1:
      uVar1 = 0x67;
      break;
    case 2:
      uVar1 = 0x65;
      break;
    case 3:
      uVar1 = 0x66;
      break;
    case 4:
      uVar1 = 4;
      break;
    default:
switchD_0043218b_default:
      uVar1 = 100;
    }
  }
  else {
    switch(param_1) {
    case 0:
      uVar1 = 100;
      break;
    case 1:
      uVar1 = 0x67;
      break;
    case 2:
      uVar1 = 0x65;
      break;
    case 3:
      uVar1 = 0x66;
      break;
    default:
      goto switchD_0043218b_default;
    }
  }
  return uVar1;
}
