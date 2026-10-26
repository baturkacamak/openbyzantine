// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x426892
// name: FUN_00426892
// ranges: 0x426892-0x426934
// calls: 0x426892, 0x436721

void __cdecl FUN_00426892(int param_1,int *param_2,FILE *param_3)

{
  int iVar1;
  uint uVar2;
  undefined1 *local_10;
  byte local_c;
  byte local_8;
  
  *param_2 = 0;
  local_10 = (undefined1 *)0x0;
  iVar1 = _fgetc(param_3);
  local_8 = (byte)iVar1;
  for (local_c = 0; local_c < local_8; local_c = local_c + 1) {
    FUN_00436721(param_2,(int *)&local_10);
    iVar1 = _fgetc(param_3);
    *local_10 = (char)iVar1;
    uVar2 = _fgetc(param_3);
    *(undefined4 *)(local_10 + 8) = *(undefined4 *)(param_1 + 0x58c + (uVar2 & 0xff) * 4);
    local_10[0xc] = (char)uVar2;
    *(undefined4 *)(local_10 + 4) = 0;
  }
  return;
}
