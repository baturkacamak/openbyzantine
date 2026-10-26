// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40c70e
// name: FUN_0040c70e
// ranges: 0x40c70e-0x40c7b3
// calls: 0x40c624, 0x40c70e, 0x44bc20

undefined4 __cdecl FUN_0040c70e(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint local_110 [65];
  FILE *local_c;
  char local_8;
  
  FUN_0040c624(local_110,(uint *)0x0,(uint *)&DAT_0045daa4,(uint *)&DAT_0045da9c);
  local_c = (FILE *)FUN_0044bc20((char *)local_110,&DAT_0045daa8);
  if (local_c == (FILE *)0x0) {
    uVar1 = 0;
  }
  else {
    iVar2 = _fgetc(local_c);
    local_8 = (char)iVar2;
    _fclose(local_c);
    if (param_1 == 0) {
      uVar1 = 1;
    }
    else if ((int)local_8 == param_1 + 0x30) {
      uVar1 = 1;
    }
    else {
      uVar1 = 0;
    }
  }
  return uVar1;
}
