// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4223c1
// name: FUN_004223c1
// ranges: 0x4223c1-0x4224ff
// calls: 0x4063e0, 0x422375, 0x4223c1

undefined4 __cdecl
FUN_004223c1(int param_1,int param_2,int param_3,int param_4,int param_5,int *param_6,int param_7,
            int param_8)

{
  int iVar1;
  int local_20;
  int local_1c;
  int local_18;
  int local_10;
  char *local_c;
  int local_8;
  
  local_18 = param_5;
  local_1c = param_1;
  while( true ) {
    if (param_2 <= local_1c) {
      return 0;
    }
    local_20 = 6;
    local_8 = param_4;
    local_10 = 1;
    local_c = *(char **)(param_3 + local_1c * 4);
    if (**(char **)(param_3 + local_1c * 4) == '$') {
      local_20 = 5;
      local_c = (char *)(*(int *)(param_3 + local_1c * 4) + 1);
    }
    if (*local_c == '>') {
      local_10 = 0;
      local_c = local_c + 1;
      local_8 = param_4 + 10;
    }
    else if (*local_c == '<') {
      local_10 = 2;
      local_c = local_c + 1;
      local_8 = param_4 + -10;
    }
    FUN_004063e0(local_c,local_8,local_18,local_20,param_6,0,local_10,1);
    iVar1 = FUN_00422375(*(char **)(param_3 + local_1c * 4),*(char **)(param_3 + 4 + local_1c * 4));
    if ((iVar1 != 0) && (local_18 = local_18 + param_8, param_7 <= local_18)) break;
    local_1c = local_1c + 1;
  }
  return 0;
}
