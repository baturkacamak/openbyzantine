// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x425525
// name: FUN_00425525
// ranges: 0x425525-0x4255d0
// calls: 0x425525

void __cdecl FUN_00425525(int param_1,FILE *param_2)

{
  undefined4 local_c;
  undefined1 local_8;
  
  local_8 = '\0';
  if (param_1 == 0) {
    _fputc(0xff,param_2);
  }
  else {
    for (local_c = param_1; local_c != 0; local_c = *(int *)(local_c + 8)) {
      local_8 = local_8 + '\x01';
    }
    _fputc((int)local_8,param_2);
    for (local_c = param_1; local_c != 0; local_c = *(int *)(local_c + 8)) {
      _fputc((uint)*(byte *)(local_c + 0x10),param_2);
      _fputc((uint)*(byte *)(local_c + 0x11),param_2);
    }
  }
  return;
}
