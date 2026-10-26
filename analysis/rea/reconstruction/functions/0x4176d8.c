// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4176d8
// name: FUN_004176d8
// ranges: 0x4176d8-0x4177c6
// calls: 0x417054, 0x417491, 0x4176d8, 0x4177c7

void __cdecl FUN_004176d8(float param_1,float param_2,int *param_3)

{
  int iVar1;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_14 = 0;
  local_10 = 0;
  local_c = 0x280;
  local_8 = 400;
  if ((DAT_0045f218 != 0) && (FUN_00417491(param_1,param_2,param_3), DAT_00486888 == param_3)) {
    if ((DAT_004603d4 != -1) &&
       (iVar1 = FUN_004177c7(*(char **)(&DAT_00483640 + DAT_004603d4 * 4)), iVar1 != 0)) {
      FUN_004177c7((char *)0x0);
    }
    if ((DAT_0045f214 != 0) && (DAT_0045c060 == 0)) {
      local_24 = 0;
      local_20 = 0;
      local_1c = 0x280;
      local_18 = 400;
      (**(code **)(*param_3 + 0x1c))(param_3,0,0x14,DAT_0045f218,&local_24,1);
    }
    FUN_00417054();
  }
  return;
}
