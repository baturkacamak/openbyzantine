// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401ee6
// name: FUN_00401ee6
// ranges: 0x401ee6-0x402073
// calls: 0x401ee6, 0x402074, 0x402e00, 0x402e70, 0x4176d8, 0x4181c5, 0x42382b, 0x42423d, 0x43be09

void __cdecl
FUN_00401ee6(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,int param_5
            )

{
  int iVar1;
  float10 fVar2;
  float fVar3;
  int *piVar4;
  uint local_14;
  int local_c;
  int *local_8;
  
  if (DAT_00464880 != (void *)0x0) {
    local_14 = (uint)(param_5 == 2);
    iVar1 = FUN_0043be09(DAT_0045c040,DAT_00468ca0,DAT_0046487c,DAT_004654d8,local_14,&local_c);
    if (((iVar1 != 0) || (param_5 != 1)) || (DAT_00464878 == 0)) {
      if (local_c != 0) {
        FUN_0042382b((int)DAT_00464880);
      }
      iVar1 = FUN_0042423d(DAT_00464880,param_1,param_2,param_3,param_4,0,0x3ff00000);
      if (iVar1 == 0) {
        if ((DAT_0048688c == (int *)0x0) || (DAT_0045f070 == '\0')) {
          local_8 = DAT_00486888;
        }
        else {
          local_8 = DAT_0048688c;
        }
        piVar4 = local_8;
        fVar2 = (float10)FUN_00402e00();
        fVar3 = (float)fVar2;
        fVar2 = (float10)FUN_00402e70();
        FUN_004176d8((float)(fVar2 + (float10)180.0),fVar3,piVar4);
        if ((DAT_0045c060 != 0) &&
           (FUN_004181c5(local_8,&DAT_0045c048,DAT_0045c048,DAT_0045c04c),
           (double)CONCAT44(param_2,param_1) != 0.0)) {
          FUN_00402074();
        }
      }
    }
  }
  return;
}
