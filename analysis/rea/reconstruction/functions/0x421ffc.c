// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421ffc
// name: FUN_00421ffc
// ranges: 0x421ffc-0x422100
// calls: 0x4063e0, 0x418040, 0x4181c5, 0x41d45a, 0x420683, 0x421ffc

void __cdecl FUN_00421ffc(int param_1,int param_2)

{
  int local_78;
  int local_74;
  uint local_6c [20];
  int *local_1c;
  int local_18;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  uint local_8;
  
  local_74 = 0x55;
  local_18 = 0x172;
  local_14 = 0x55;
  local_10 = 0x22e;
  local_c = 0x16d;
  local_1c = (int *)FUN_00418040(DAT_00477344);
  if (param_2 != 0) {
    FUN_00420683(&local_18);
  }
  for (local_78 = 0x34; (local_78 < 0x3b && (local_78 <= DAT_0045f7dc)); local_78 = local_78 + 1) {
    local_8 = (uint)(local_78 == param_1);
    FUN_0041d45a(local_6c,local_78);
    FUN_004063e0((char *)local_6c,0x1d0,local_74,2,local_1c,local_8,1,1);
    local_74 = local_74 + 0x28;
  }
  if (param_2 != 0) {
    FUN_004181c5(local_1c,&local_18,local_18,local_14);
  }
  return;
}
