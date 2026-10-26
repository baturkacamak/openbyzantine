// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x425952
// name: FUN_00425952
// ranges: 0x425952-0x4259dd
// calls: 0x425952

void __cdecl FUN_00425952(byte *param_1,FILE *param_2)

{
  byte *local_c;
  byte local_8;
  
  local_8 = 0;
  for (local_c = param_1; local_c != (byte *)0x0; local_c = *(byte **)(local_c + 4)) {
    local_8 = local_8 + 1;
  }
  _fputc((uint)local_8,param_2);
  for (local_c = param_1; local_c != (byte *)0x0; local_c = *(byte **)(local_c + 4)) {
    _fputc((uint)*local_c,param_2);
    _fputc((uint)local_c[0xc],param_2);
  }
  return;
}
