// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4183aa
// name: FUN_004183aa
// ranges: 0x4183aa-0x41842a
// calls: 0x418040, 0x4181c5, 0x4183aa

void __cdecl
FUN_004183aa(uint param_1,uint param_2,int param_3,uint param_4,uint param_5,uint param_6,
            uint param_7)

{
  int *piVar1;
  uint local_14;
  uint local_10;
  int local_c;
  int local_8;
  
  piVar1 = (int *)FUN_00418040(param_3);
  local_10 = param_2 & 0xffff;
  local_14 = param_1 & 0xffff;
  local_c = (param_6 & 0xffff) + (param_1 & 0xffff);
  local_8 = (param_7 & 0xffff) + (param_2 & 0xffff);
  FUN_004181c5(piVar1,(int *)&local_14,param_4 & 0xffff,param_5 & 0xffff);
  return;
}
