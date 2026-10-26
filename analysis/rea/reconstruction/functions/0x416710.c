// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x416710
// name: FUN_00416710
// ranges: 0x416710-0x416973
// calls: 0x4164f4, 0x416710, 0x42b83f, 0x44ac40, 0x44bc20

undefined4 __cdecl
FUN_00416710(char *param_1,undefined4 *param_2,int param_3,uint *param_4,uint *param_5)

{
  ushort uVar1;
  undefined4 uVar2;
  uint local_c0;
  uint local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  uint local_b0;
  uint local_ac;
  int local_a8;
  int local_94;
  undefined4 local_50;
  ushort *local_48;
  int local_44;
  FILE *local_40;
  int *local_3c;
  ushort local_38;
  undefined2 uStack_36;
  int local_34;
  char *local_2c [3];
  ushort *local_20;
  uint local_1c;
  uint local_18;
  
  local_40 = (FILE *)FUN_0044bc20(param_1,&DAT_0045f1ec);
  if (local_40 == (FILE *)0x0) {
    uVar2 = 1;
  }
  else {
    _fclose(local_40);
    _memset(local_2c,0,0x28);
    local_2c[0] = param_1;
    local_34 = FUN_004164f4(local_2c);
    if (local_34 == 0) {
      uVar2 = 1;
    }
    else {
      _memset(&local_b8,0,0x6c);
      local_b8 = 0x6c;
      local_b4 = 7;
      local_50 = 0x840;
      local_ac = local_1c;
      local_b0 = local_18;
      local_44 = (**(code **)(*DAT_00486a04 + 0x18))(DAT_00486a04,&local_b8,param_2,0);
      if (local_44 == 0) {
        local_3c = (int *)*param_2;
        local_44 = (**(code **)(*local_3c + 0x58))(local_3c,&local_b8);
        if ((local_ac == local_1c) && (local_18 == local_b0)) {
          if (param_3 != 0) {
            local_48 = local_20;
            uVar1 = *local_20;
            if (*local_20 == 0) {
              uVar1 = DAT_00485c88;
            }
            _local_38 = CONCAT22(uStack_36,uVar1);
            local_c0 = (uint)uVar1;
            local_bc = (uint)uVar1;
            local_44 = (**(code **)(*local_3c + 0x74))(local_3c,8,&local_c0);
            if (local_44 != 0) {
              return 1;
            }
          }
          local_44 = (**(code **)(*local_3c + 100))(local_3c,0,&local_b8,1,0);
          if (local_44 == 0) {
            FUN_0042b83f(local_94,local_20,local_a8 >> 1,local_18,local_1c,0,0);
            (**(code **)(*local_3c + 0x80))(local_3c,0);
            FUN_0044ac40(local_20);
            if (param_4 != (uint *)0x0) {
              *param_4 = local_ac;
            }
            if (param_5 != (uint *)0x0) {
              *param_5 = local_b0;
            }
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
      else {
        uVar2 = 1;
      }
    }
  }
  return uVar2;
}
