// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42ad6f
// name: FUN_0042ad6f
// ranges: 0x42ad6f-0x42aebe
// calls: 0x42ad6f

undefined4 __cdecl FUN_0042ad6f(int *param_1,undefined1 *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_78;
  undefined4 local_74;
  uint local_20;
  uint local_1c;
  uint local_18;
  char local_8;
  
  local_78 = 0x6c;
  local_74 = 0x1000;
  iVar1 = (**(code **)(*param_1 + 0x58))(param_1,&local_78);
  if (iVar1 == 0) {
    if (local_20 == 0) {
      uVar2 = 1;
    }
    else {
      local_8 = '\0';
      for (; (local_20 & 1) == 0; local_20 = local_20 >> 1) {
        local_8 = local_8 + '\x01';
      }
      param_2[2] = (undefined1)local_20;
      param_2[10] = local_8;
      if (local_20 == 0x1f) {
        param_2[6] = 3;
      }
      else {
        param_2[6] = 2;
      }
      local_8 = '\0';
      for (; (local_1c & 1) == 0; local_1c = local_1c >> 1) {
        local_8 = local_8 + '\x01';
      }
      param_2[1] = (undefined1)local_1c;
      param_2[9] = local_8;
      if (local_1c == 0x1f) {
        param_2[5] = 3;
      }
      else {
        param_2[5] = 2;
      }
      local_8 = '\0';
      for (; (local_18 & 1) == 0; local_18 = local_18 >> 1) {
        local_8 = local_8 + '\x01';
      }
      *param_2 = (undefined1)local_18;
      param_2[8] = local_8;
      if (local_18 == 0x1f) {
        param_2[4] = 3;
      }
      else {
        param_2[4] = 2;
      }
      uVar2 = 1;
    }
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}
