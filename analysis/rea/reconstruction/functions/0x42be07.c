// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42be07
// name: FUN_0042be07
// ranges: 0x42be07-0x42beba
// calls: 0x42be07, 0x42c1b8, 0x42c57e

void __cdecl FUN_0042be07(ushort *param_1,ushort *param_2,int param_3,int param_4)

{
  if (param_3 == 0) {
    param_3 = DAT_00477344;
  }
  if (param_4 == 1) {
    FUN_0042c1b8(param_1 + 0x3200,param_2 + 0x3200,param_3,1);
  }
  else if (param_4 == 0) {
    FUN_0042c57e((int)param_1,(int)param_2,param_3);
  }
  else if (param_4 == 2) {
    FUN_0042c1b8(param_1 + 0x3200,param_2 + 0x3200,param_3,0);
  }
  else {
    FUN_0042c1b8(param_1,param_2,param_3,0);
  }
  return;
}
