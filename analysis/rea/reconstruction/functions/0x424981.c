// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x424981
// name: FUN_00424981
// ranges: 0x424981-0x424b14
// calls: 0x4063e0, 0x418040, 0x4181c5, 0x424981, 0x44c520

void __cdecl FUN_00424981(int *param_1)

{
  int *piVar1;
  LONG LVar2;
  char *pcVar3;
  int local_18;
  int local_14 [4];
  
  piVar1 = (int *)FUN_00418040(DAT_00477344);
  local_14[0] = 0x37;
  local_14[1] = 0x1a4;
  local_14[2] = 0x280;
  local_14[3] = 0x1e0;
  _memset((void *)(DAT_00477344 + 0x83400),0,0x12c00);
  FUN_004063e0(DAT_0045fdb4,0x37,0x1a4,4,piVar1,0,3,DAT_0045fdb8);
  DAT_0045fdac = DAT_0045fdac + -1;
  if (DAT_0045fdac != 0) {
    FUN_004063e0(s__more__0045fdcc,0x27f,0x1cc,4,piVar1,1,2,1);
  }
  if (DAT_0048688c == param_1) {
    while( true ) {
      LVar2 = InterlockedExchange((LONG *)&DAT_00486798,1);
      if (LVar2 == 0) break;
      Sleep(10);
    }
    (**(code **)(*DAT_004869d4 + 0x1c))(DAT_004869d4,0x37,0x1a4,piVar1,local_14,0x10);
    (**(code **)(*DAT_0048688c + 0x1c))(DAT_0048688c,0x37,0x1a4,piVar1,local_14,0x10);
    InterlockedExchange((LONG *)&DAT_00486798,0);
  }
  else {
    FUN_004181c5(piVar1,local_14,0x37,0x1a4);
  }
  DAT_0045f054 = 0;
  if (DAT_0045fdac != 0) {
    for (local_18 = 0; local_18 < DAT_0045fdb8; local_18 = local_18 + 1) {
      pcVar3 = FUN_0044c520(DAT_0045fdb4,'\n');
      DAT_0045fdb4 = pcVar3 + 1;
    }
  }
  return;
}
