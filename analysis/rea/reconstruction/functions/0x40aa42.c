// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40aa42
// name: FUN_0040aa42
// ranges: 0x40aa42-0x40ab26
// calls: 0x40aa42, 0x44ba00, 0x44ba08

void __cdecl FUN_0040aa42(uint *param_1,int param_2)

{
  bool bVar1;
  int local_28;
  int local_24;
  undefined1 local_18;
  undefined1 local_17;
  
  local_24 = 0;
  bVar1 = false;
  if (param_2 < 0) {
    bVar1 = true;
    param_2 = -param_2;
  }
  do {
    for (local_28 = local_24; 0 < local_28; local_28 = local_28 + -1) {
      *(undefined1 *)(local_28 + (int)param_1) = *(undefined1 *)(local_28 + -1 + (int)param_1);
    }
    *(char *)param_1 = (char)param_2 + (char)(param_2 / 10) * -10 + '0';
    param_2 = param_2 / 10;
    local_24 = local_24 + 1;
  } while (param_2 != 0);
  *(undefined1 *)(local_24 + (int)param_1) = 0;
  if (bVar1) {
    local_18 = 0x2d;
    local_17 = 0;
    FUN_0044ba08((uint *)&local_18,param_1);
    FUN_0044ba00(param_1,(uint *)&local_18);
  }
  return;
}
