// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41917b
// name: FUN_0041917b
// ranges: 0x41917b-0x41920e
// calls: 0x418040, 0x41917b, 0x42b280

void FUN_0041917b(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  piVar1 = (int *)FUN_00418040((int)DAT_00477344);
  local_24 = 0;
  local_20 = 0x14;
  local_1c = 0x280;
  local_18 = 0x1a4;
  local_14 = 0x80;
  local_10 = 0x94;
  local_c = 0x200;
  local_8 = 0x124;
  FUN_0042b280(DAT_00477344,0x4b000);
  uVar5 = 0;
  uVar4 = 0x1000000;
  puVar3 = &local_24;
  uVar2 = FUN_00418040(DAT_0046e82c);
  (**(code **)(*piVar1 + 0x14))(piVar1,&local_14,uVar2,puVar3,uVar4,uVar5);
  return;
}
