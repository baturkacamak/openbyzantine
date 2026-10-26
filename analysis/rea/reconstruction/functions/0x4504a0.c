// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4504a0
// name: __handle_exc
// ranges: 0x4504a0-0x45075f
// calls: 0x450930

/* Library Function - Single Match
    __handle_exc
   
   Library: Visual Studio 1998 Release */

bool __cdecl __handle_exc(uint param_1,double *param_2,uint param_3)

{
  ulonglong uVar1;
  uint uVar2;
  bool bVar3;
  float10 fVar4;
  undefined8 local_c;
  int local_4;
  
  uVar2 = param_1 & 0x1f;
  if (((param_1 & 8) == 0) || ((param_3 & 1) == 0)) {
    if (((param_1 & 4) == 0) || ((param_3 & 4) == 0)) {
      if (((param_1 & 1) == 0) || ((param_3 & 8) == 0)) {
        if (((param_1 & 2) != 0) && ((param_3 & 0x10) != 0)) {
          bVar3 = (param_1 & 0x10) != 0;
          if (((ulonglong)*param_2 & 0x7fffffff00000000) == 0 && *(int *)param_2 == 0) {
            bVar3 = true;
          }
          else {
            fVar4 = __decomp(*(uint *)param_2,*(uint *)((int)param_2 + 4),&local_4);
            local_4 = local_4 + -0x600;
            if (local_4 < -0x432) {
              bVar3 = true;
              local_c = 0.0;
            }
            else {
              local_c = (double)(ulonglong)
                                (SUB87((double)fVar4,0) & 0xfffffffffffff | 0x10000000000000);
              if (local_4 < -0x3fd) {
                local_4 = -0x3fd - local_4;
                do {
                  if ((((ulonglong)local_c & 1) != 0) && (!bVar3)) {
                    bVar3 = true;
                  }
                  uVar2 = (uint)local_c >> 1;
                  uVar1 = (ulonglong)local_c & 0x100000000;
                  local_c._0_4_ = uVar2;
                  if (uVar1 != 0) {
                    local_c._0_4_ = uVar2 | 0x80000000;
                  }
                  local_c = (double)CONCAT44(local_c._4_4_ >> 1,(uint)local_c);
                  local_4 = local_4 + -1;
                } while (local_4 != 0);
              }
              if ((double)fVar4 < 0.0) {
                local_c = -local_c;
              }
            }
            *(uint *)((int)param_2 + 4) = local_c._4_4_;
            *(uint *)param_2 = (uint)local_c;
          }
          if (bVar3) {
            FUN_00450930();
          }
          uVar2 = param_1 & 0x1d;
        }
      }
      else {
        FUN_00450930();
        uVar2 = param_3 & 0xc00;
        if (uVar2 < 0x401) {
          if (uVar2 == 0x400) {
            if (*param_2 <= 0.0) {
              uVar2 = param_1 & 0x1e;
              *param_2 = -(double)CONCAT44(DAT_0046341c,DAT_00463418);
            }
            else {
              uVar2 = param_1 & 0x1e;
              *(undefined4 *)((int)param_2 + 4) = DAT_0046342c;
              *(undefined4 *)param_2 = DAT_00463428;
            }
          }
          else if (uVar2 == 0) {
            if (*param_2 <= 0.0) {
              uVar2 = param_1 & 0x1e;
              *param_2 = -(double)CONCAT44(DAT_0046341c,DAT_00463418);
            }
            else {
              uVar2 = param_1 & 0x1e;
              *(undefined4 *)((int)param_2 + 4) = DAT_0046341c;
              *(undefined4 *)param_2 = DAT_00463418;
            }
          }
          else {
            uVar2 = param_1 & 0x1e;
          }
        }
        else if (uVar2 == 0x800) {
          if (*param_2 <= 0.0) {
            uVar2 = param_1 & 0x1e;
            *param_2 = -(double)CONCAT44(DAT_0046342c,DAT_00463428);
          }
          else {
            uVar2 = param_1 & 0x1e;
            *(undefined4 *)((int)param_2 + 4) = DAT_0046341c;
            *(undefined4 *)param_2 = DAT_00463418;
          }
        }
        else if (uVar2 == 0xc00) {
          if (*param_2 <= 0.0) {
            uVar2 = param_1 & 0x1e;
            *param_2 = -(double)CONCAT44(DAT_0046342c,DAT_00463428);
          }
          else {
            uVar2 = param_1 & 0x1e;
            *(undefined4 *)((int)param_2 + 4) = DAT_0046342c;
            *(undefined4 *)param_2 = DAT_00463428;
          }
        }
        else {
          uVar2 = param_1 & 0x1e;
        }
      }
    }
    else {
      uVar2 = param_1 & 0x1b;
      FUN_00450930();
    }
  }
  else {
    uVar2 = param_1 & 0x17;
    FUN_00450930();
  }
  if (((param_1 & 0x10) != 0) && ((param_3 & 0x20) != 0)) {
    uVar2 = uVar2 & 0xffffffef;
    FUN_00450930();
  }
  return uVar2 == 0;
}
