// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x456dd0
// name: FUN_00456dd0
// ranges: 0x456dd0-0x456e4e
// calls: 0x456dd0, 0x4590b0

undefined4 __cdecl FUN_00456dd0(int param_1,uint param_2)

{
  double dVar1;
  uint uVar2;
  float10 fVar3;
  
  uVar2 = FUN_004590b0(param_1,param_2);
  if ((uVar2 & 0x90) != 0) {
    return 0;
  }
  fVar3 = __frnd((double)CONCAT44(param_2,param_1));
  if (fVar3 == (float10)(double)CONCAT44(param_2,param_1)) {
    dVar1 = (double)CONCAT44(param_2,param_1) * 0.5;
    fVar3 = __frnd(dVar1);
    if (fVar3 == (float10)dVar1) {
      return 2;
    }
    return 1;
  }
  return 0;
}
