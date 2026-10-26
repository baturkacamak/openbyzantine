// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44bc60
// name: _floor
// ranges: 0x44bc60-0x44bd74
// calls: 0x4508f0

/* Library Function - Single Match
    _floor
   
   Library: Visual Studio 1998 Release */

double __cdecl _floor(double _X)

{
  double dVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  float10 fVar5;
  undefined4 local_8;
  undefined4 uStack_4;
  
  uVar3 = FUN_004508f0();
  uVar2 = (uint)((ulonglong)_X >> 0x20);
  if ((_X._6_2_ & 0x7ff0) != 0x7ff0) {
    fVar5 = __frnd(_X);
    dVar1 = (double)fVar5;
    if (fVar5 == (float10)_X) {
      FUN_004508f0();
      return dVar1;
    }
    if ((uVar3 & 0x20) != 0) {
      FUN_004508f0();
      return dVar1;
    }
    uStack_4 = (undefined4)((ulonglong)dVar1 >> 0x20);
    local_8 = SUB84(dVar1,0);
    fVar5 = __except1(0x10,0xb,_X._0_4_,uVar2,local_8,uStack_4,uVar3);
    return (double)fVar5;
  }
  iVar4 = __sptype(_X._0_4_,uVar2);
  if (0 < iVar4) {
    if (iVar4 < 3) {
      FUN_004508f0();
      return _X;
    }
    if (iVar4 == 3) {
      fVar5 = __handle_qnan1(0xb,_X._0_4_,uVar2);
      return (double)fVar5;
    }
  }
  fVar5 = __except1(8,0xb,_X._0_4_,uVar2,SUB84(_X + 1.0,0),(int)((ulonglong)(_X + 1.0) >> 0x20),
                    uVar3);
  return (double)fVar5;
}
