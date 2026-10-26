// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40cb26
// name: FUN_0040cb26
// ranges: 0x40cb26-0x40cbcb
// calls: 0x40cb26
// port: RLE frame count [bounded]

undefined2 __cdecl FUN_0040cb26(FILE *param_1,char *param_2)

{
  undefined2 local_8 [2];
  
  if (param_2 == (char *)0x0) {
    __fread_lk((char *)local_8,2,1,param_1);
  }
  else {
    __fread_lk(param_2,2,1,param_1);
  }
  _fgetc(param_1);
  _fgetc(param_1);
  _fgetc(param_1);
  _fgetc(param_1);
  __fread_lk((char *)local_8,2,1,param_1);
  __fread_lk((char *)local_8,2,1,param_1);
  return local_8[0];
}
