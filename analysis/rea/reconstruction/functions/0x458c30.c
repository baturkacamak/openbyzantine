// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x458c30
// name: FUN_00458c30
// ranges: 0x458c30-0x458d46
// calls: 0x4508f0, 0x458c30

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 __cdecl FUN_00458c30(uint param_1,uint param_2)

{
  double dVar1;
  uint uVar2;
  int iVar3;
  float10 fVar4;
  int local_4;
  
  uVar2 = FUN_004508f0();
  if ((param_2._2_2_ & 0x7ff0) != 0x7ff0) {
    if ((param_2 & 0x7fffffff) == 0 && param_1 == 0) {
      fVar4 = __except1(4,0x25,param_1,param_2,SUB84(-_DAT_00463418,0),
                        (int)((ulonglong)-_DAT_00463418 >> 0x20),uVar2);
      return fVar4;
    }
    __decomp(param_1,param_2,&local_4);
    FUN_004508f0();
    return (float10)(local_4 + -1);
  }
  iVar3 = __sptype(param_1,param_2);
  if (0 < iVar3) {
    if (iVar3 < 3) {
      FUN_004508f0();
      return (float10)(double)CONCAT26(param_2._2_2_,CONCAT24((undefined2)param_2,param_1));
    }
    if (iVar3 == 3) {
      fVar4 = __handle_qnan1(0x25,param_1,param_2);
      return fVar4;
    }
  }
  dVar1 = (double)CONCAT26(param_2._2_2_,CONCAT24((undefined2)param_2,param_1)) + 1.0;
  fVar4 = __except1(8,0x25,param_1,param_2,SUB84(dVar1,0),(int)((ulonglong)dVar1 >> 0x20),uVar2);
  return fVar4;
}
