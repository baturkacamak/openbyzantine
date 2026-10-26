// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x424cb0
// name: FUN_00424cb0
// ranges: 0x424cb0-0x424dbc
// calls: 0x4063e0, 0x418040, 0x41842b, 0x424cb0, 0x42b2db

void __cdecl FUN_00424cb0(char *param_1,int param_2)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_00418040((int)DAT_0046e82c);
  if (param_2 == -1) {
    FUN_0042b2db(DAT_0046e82c,DAT_0047a8a4,0x4b000);
    FUN_0042b2db(DAT_00477344,DAT_0047a8a4,0x4b000);
    FUN_004063e0(param_1,0x4b,0x32,0,piVar1,0,3,4);
    DAT_0046edf4 = 0;
  }
  else if (param_2 == -3) {
    FUN_0042b2db(DAT_0046e82c,DAT_0047a8a4,0x4b000);
    FUN_0042b2db(DAT_00477344,DAT_0047a8a4,0x4b000);
    FUN_0041842b((int)DAT_0046e82c,0x4b000);
  }
  else {
    FUN_004063e0(param_1,100,0x8f,0,piVar1,1,3,DAT_0045fdb8);
    FUN_0041842b((int)DAT_0046e82c,0x4b000);
  }
  return;
}
