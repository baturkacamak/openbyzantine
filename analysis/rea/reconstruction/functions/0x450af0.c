// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450af0
// name: __decomp
// ranges: 0x450af0-0x450bdd
// calls: -

/* Library Function - Single Match
    __decomp
   
   Library: Visual Studio 1998 Release */

float10 __cdecl __decomp(uint param_1,uint param_2,int *param_3)

{
  ushort uVar1;
  int iVar2;
  double dVar3;
  byte bVar4;
  int iVar5;
  float10 fVar6;
  double local_8;
  
  if ((param_2 & 0x7fffffff) == 0 && param_1 == 0) {
    iVar5 = 0;
    local_8 = 0.0;
  }
  else if (((param_2 & 0x7ff00000) == 0) && (((param_2 & 0xfffff) != 0 || (param_1 != 0)))) {
    dVar3 = (double)CONCAT17(param_2._3_1_,CONCAT16(param_2._2_1_,CONCAT24((ushort)param_2,param_1))
                            );
    iVar5 = -0x3fd;
    if ((param_2 & 0x100000) == 0) {
      do {
        bVar4 = param_2._2_1_;
        iVar2 = CONCAT13(param_2._3_1_,CONCAT12(param_2._2_1_,(ushort)param_2)) << 1;
        param_2._0_2_ = (ushort)iVar2;
        param_2._2_1_ = (byte)((uint)iVar2 >> 0x10);
        param_2._3_1_ = (byte)((uint)iVar2 >> 0x18);
        if ((param_1 & 0x80000000) != 0) {
          param_2._0_2_ = (ushort)param_2 | 1;
        }
        iVar5 = iVar5 + -1;
        param_1 = param_1 << 1;
      } while ((bVar4 & 8) == 0);
    }
    uVar1 = CONCAT11(param_2._3_1_,param_2._2_1_) & 0xffef;
    param_2._2_1_ = (byte)uVar1;
    param_2._3_1_ = (byte)(uVar1 >> 8);
    if (dVar3 < 0.0) {
      param_2._3_1_ = param_2._3_1_ | 0x80;
    }
    fVar6 = __set_exp(param_1,CONCAT13(param_2._3_1_,CONCAT12(param_2._2_1_,(ushort)param_2)),0);
    local_8 = (double)fVar6;
  }
  else {
    iVar5 = (short)(((ushort)(param_2 >> 0x10) & 0x7ff0) >> 4) + -0x3fe;
    fVar6 = __set_exp(param_1,param_2,0);
    local_8 = (double)fVar6;
  }
  *param_3 = iVar5;
  return (float10)local_8;
}
