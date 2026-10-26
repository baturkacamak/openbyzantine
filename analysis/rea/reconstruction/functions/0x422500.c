// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422500
// name: FUN_00422500
// ranges: 0x422500-0x422767
// calls: 0x40c624, 0x414c3d, 0x41624a, 0x4181c5, 0x4221b1, 0x422318, 0x422375, 0x4223c1, 0x422500

void __cdecl FUN_00422500(undefined4 param_1)

{
  int iVar1;
  DWORD DVar2;
  DWORD local_a94;
  uint local_a90 [65];
  int local_98c;
  int local_988;
  int *local_980;
  int local_97c;
  undefined4 local_978 [600];
  int local_18 [5];
  
  local_97c = 0;
  local_980 = DAT_00486888;
  local_18[0] = 0;
  local_18[1] = 0;
  local_18[2] = 0x280;
  local_18[3] = 0x1e0;
  local_18[4] = DAT_00487708;
  FUN_0040c624(local_a90,(uint *)&DAT_0045fd0c,(uint *)s_credits_0045fd04,(uint *)&DAT_0045fcfc);
  local_988 = FUN_004221b1((char *)local_a90,(int)local_978,600);
  local_a94 = timeGetTime();
  local_98c = 0x1de;
  DAT_0045f050 = 0;
  DAT_0045f058 = 0;
  if (DAT_0048688c != (int *)0x0) {
    DAT_0045f2d8 = 0;
    local_980 = DAT_0048688c;
    DAT_0045f070 = 1;
  }
  do {
    do {
      (**(code **)(*local_980 + 0x1c))(local_980,0,0,param_1,0,0x10);
      iVar1 = FUN_004223c1(local_97c,local_988,(int)local_978,0x140,local_98c,local_980,0x1e0,
                           local_18[4]);
    } while (iVar1 != 0);
    do {
      DVar2 = timeGetTime();
    } while (DVar2 - local_a94 < 0x19);
    FUN_004181c5(local_980,local_18,0,0);
    local_a94 = timeGetTime();
    local_98c = local_98c + -2;
    FUN_0041624a();
    if (-local_98c != local_18[4] && local_98c <= -local_18[4]) {
      local_98c = local_98c + local_18[4];
      iVar1 = FUN_00422375((char *)local_978[local_97c],(char *)local_978[local_97c + 1]);
      if (iVar1 == 0) {
        local_97c = local_97c + 2;
      }
      else {
        local_97c = local_97c + 1;
      }
      if (local_988 <= local_97c) goto LAB_0042272b;
    }
  } while ((DAT_0045f050 == 0) && (DAT_0045f058 == 0));
  DAT_0045f050 = 0;
  DAT_0045f058 = 0;
LAB_0042272b:
  if (DAT_0048688c != (int *)0x0) {
    FUN_00414c3d((byte)DAT_0045f2d8);
    DAT_0045f070 = 0;
  }
  FUN_00422318((int)local_978,local_988);
  return;
}
