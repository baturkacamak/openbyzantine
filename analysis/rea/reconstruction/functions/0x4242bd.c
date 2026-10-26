// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4242bd
// name: FUN_004242bd
// ranges: 0x4242bd-0x42475e
// calls: 0x423a37, 0x423c42, 0x4242bd
// port: Camera advance and normal RGB16 sample [partial]

/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined4 __thiscall
FUN_004242bd(void *this,double param_1,double param_2,double param_3,undefined4 param_4,
            undefined4 param_5,int param_6)

{
  double dVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined4 extraout_EDX;
  int extraout_EDX_00;
  int *piVar6;
  ushort *puVar7;
  float10 fVar8;
  float10 extraout_ST0;
  float10 extraout_ST0_00;
  longlong lVar9;
  int local_a4;
  undefined4 local_a0 [4];
  int local_90;
  int local_7c;
  double local_34;
  uint local_2c;
  ushort local_28;
  int *local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_2c = *(uint *)((int)this + 4);
  local_a4 = *(int *)((int)this + 8);
  local_28 = *(ushort *)((int)this + 0x30);
  local_18 = *(int *)((int)this + 0x34);
  if (local_18 == 0) {
    uVar2 = 1;
  }
  else {
    _memset(local_a0,0,0x6c);
    local_a0[0] = 0x6c;
    local_c = (**(code **)(**(int **)this + 100))();
    if (local_c == 0) {
      local_10 = local_90;
      puVar7 = (ushort *)
               (local_7c +
               (*(int *)((int)this + 0x10) * (local_90 >> 1) + *(int *)((int)this + 0xc)) * 2);
      local_1c = local_18;
      if ((param_2 != 0.0) || (param_3 != 1.0)) {
        if (DAT_00462ae8 == 0) {
          fVar8 = ((float10)param_2 * (float10)3.14159265359) / (float10)180.0;
        }
        else {
          __adj_fdiv_m64(0,0x40668000);
          fVar8 = extraout_ST0;
        }
        *(double *)((int)this + 0x58) = (double)((float10)*(double *)((int)this + 0x58) - fVar8);
        *(double *)((int)this + 0x60) = *(double *)((int)this + 0x60) * param_3;
        dVar1 = *(double *)((int)this + 0x60);
        if (dVar1 <= 1.0) {
          dVar1 = 1.0;
        }
        *(double *)((int)this + 0x60) = dVar1;
        fVar8 = (float10)FUN_00423c42(this,*(double *)((int)this + 0x60));
        local_34 = (double)fVar8;
        if ((float10)*(double *)((int)this + 0x58) < fVar8) {
          if (-local_34 < *(double *)((int)this + 0x58)) {
            *(undefined4 *)((int)this + 0x68) = 0;
          }
          else {
            *(undefined4 *)((int)this + 0x68) = 1;
            *(double *)((int)this + 0x58) = -local_34;
          }
        }
        else {
          *(undefined4 *)((int)this + 0x68) = 1;
          *(double *)((int)this + 0x58) = local_34;
        }
        FUN_00423a37((int)this,extraout_EDX);
      }
      if (DAT_00462ae8 == 0) {
        fVar8 = ((float10)param_1 * (float10)3.14159265359) / (float10)180.0;
      }
      else {
        __adj_fdiv_m64(0,0x40668000);
        fVar8 = extraout_ST0_00;
      }
      *(double *)((int)this + 0x50) = (double)(fVar8 + (float10)*(double *)((int)this + 0x50));
      while (3.14159265359 < *(double *)((int)this + 0x50)) {
        *(double *)((int)this + 0x50) = *(double *)((int)this + 0x50) - 6.28318530718;
      }
      while (*(double *)((int)this + 0x50) < -3.14159265359) {
        *(double *)((int)this + 0x50) = *(double *)((int)this + 0x50) + 6.28318530718;
      }
      lVar9 = __ftol();
      local_24 = *(int **)((int)this + 0x3c);
      local_14 = local_10 + local_2c * -2;
      if (param_6 == 0) {
        uVar5 = local_2c;
        piVar6 = local_24;
        do {
          do {
            *puVar7 = *(ushort *)(local_18 + *piVar6 * 2 + (int)lVar9 * 2);
            puVar7 = puVar7 + 1;
            piVar6 = piVar6 + 1;
            uVar5 = uVar5 - 1;
          } while (uVar5 != 0);
          puVar7 = (ushort *)((int)puVar7 + local_14);
          local_a4 = local_a4 + -1;
          uVar5 = local_2c;
        } while (local_a4 != 0);
      }
      else {
        lVar9 = __ftol();
        iVar4 = local_18;
        local_8 = (int)lVar9;
        local_20 = local_8 >> 1;
        piVar6 = local_24;
        do {
          uVar3 = _rand();
          uVar5 = local_2c;
          if ((int)(uVar3 & 0xff) < local_8) {
            if (local_20 < (int)(uVar3 & 0xff)) {
              for (uVar5 = local_2c >> 1; uVar5 != 0; uVar5 = uVar5 - 1) {
                puVar7[0] = 0;
                puVar7[1] = 0;
                puVar7 = puVar7 + 2;
              }
              for (uVar5 = local_2c & 1; uVar5 != 0; uVar5 = uVar5 - 1) {
                *puVar7 = 0;
                puVar7 = puVar7 + 1;
              }
              piVar6 = piVar6 + local_2c;
            }
            else {
              for (uVar5 = local_2c >> 1; uVar5 != 0; uVar5 = uVar5 - 1) {
                *(uint *)puVar7 = CONCAT22(local_28,local_28);
                puVar7 = puVar7 + 2;
              }
              for (uVar5 = local_2c & 1; uVar5 != 0; uVar5 = uVar5 - 1) {
                *puVar7 = local_28;
                puVar7 = puVar7 + 1;
              }
              piVar6 = piVar6 + local_2c;
            }
          }
          else {
            do {
              *puVar7 = *(ushort *)(iVar4 + *piVar6 * 2 + extraout_EDX_00) & local_28;
              puVar7 = puVar7 + 1;
              piVar6 = piVar6 + 1;
              uVar5 = uVar5 - 1;
            } while (uVar5 != 0);
          }
          puVar7 = (ushort *)((int)puVar7 + local_14);
          local_a4 = local_a4 + -1;
        } while (local_a4 != 0);
      }
      iVar4 = (**(code **)(**(int **)this + 0x80))(*(undefined4 *)this,local_7c);
      if (iVar4 == 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = 1;
      }
    }
    else {
      uVar2 = 1;
    }
  }
  return uVar2;
}
