// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408dbd
// name: FUN_00408dbd
// ranges: 0x408dbd-0x408e3e
// calls: 0x408dbd, 0x44c2f0

void __cdecl FUN_00408dbd(FILE *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int local_c;
  
  for (local_c = 0; local_c < 0x15e; local_c = local_c + 1) {
    iVar1 = _fgetc(param_1);
    if ((char)iVar1 != -1) {
      uVar2 = FUN_0044c2f0(0x68);
      *(undefined4 *)(&DAT_00486a10 + local_c * 4) = uVar2;
      __fread_lk(*(char **)(&DAT_00486a10 + local_c * 4),0x68,1,param_1);
    }
  }
  return;
}
