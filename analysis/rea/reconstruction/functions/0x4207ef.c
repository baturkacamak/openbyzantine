// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4207ef
// name: FUN_004207ef
// ranges: 0x4207ef-0x420887
// calls: 0x418040, 0x4207a6, 0x4207ef, 0x44a460

void __cdecl FUN_004207ef(int param_1,FILE *param_2)

{
  undefined4 uVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  char *local_8;
  
  local_1c = 0;
  local_18 = 0;
  local_14 = 0xa8;
  local_10 = 0xa8;
  puVar3 = &local_1c;
  uVar1 = FUN_00418040(param_1);
  piVar2 = (int *)FUN_00418040((int)DAT_00477344);
  FUN_004207a6(piVar2,uVar1,puVar3);
  local_8 = DAT_00477344;
  for (local_c = 0; local_c < 0xa8; local_c = local_c + 1) {
    FUN_0044a460(local_8,2,0xa8,param_2);
    local_8 = local_8 + 0x500;
  }
  return;
}
