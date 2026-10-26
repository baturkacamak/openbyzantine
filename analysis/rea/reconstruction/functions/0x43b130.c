// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43b130
// name: FUN_0043b130
// ranges: 0x43b130-0x43b215
// calls: 0x40bbb0, 0x40bbfd, 0x41624a, 0x41cd96, 0x43b130

void __cdecl
FUN_0043b130(int param_1,int param_2,short param_3,short param_4,int *param_5,uint param_6,
            int param_7)

{
  int *local_8;
  
  FUN_0041624a();
  if (*(int *)(param_1 + 0x38) == 0) {
    if ((param_7 == 1) && (*(char *)(param_1 + 0x28) != '\0')) {
      local_8 = *(int **)(param_1 + 0x18);
    }
    else {
      local_8 = *(int **)(param_1 + 0x1c);
    }
    while( true ) {
      FUN_0040bbb0(param_3,param_4,local_8[4],param_6);
      FUN_0040bbfd(*local_8,param_2,param_3,param_4,param_5,*(byte *)(param_1 + 9) & 0x80);
      if (*(int **)(param_1 + 0x1c) == local_8) break;
      local_8 = (int *)local_8[1];
    }
  }
  else {
    FUN_0041cd96(param_1,param_2,(int)param_3,(int)param_4,param_6,param_5);
  }
  FUN_0041624a();
  return;
}
