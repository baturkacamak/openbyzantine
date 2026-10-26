// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408adc
// name: FUN_00408adc
// ranges: 0x408adc-0x408bb1
// calls: 0x407420, 0x4089b0, 0x408adc

void __cdecl FUN_00408adc(FILE *param_1)

{
  int local_8;
  
  DAT_0045d1c8 = 0;
  __fread_lk((char *)&DAT_0045d1c0,4,1,param_1);
  __fread_lk((char *)&DAT_0045d1c4,4,1,param_1);
  __fread_lk((char *)&DAT_0045d1bc,4,1,param_1);
  __fread_lk(&DAT_00468cc0,0x28,1,param_1);
  __fread_lk(&DAT_00468d78,14000,1,param_1);
  __fread_lk((char *)&local_8,4,1,param_1);
  if (local_8 == -1) {
    DAT_0045d1c8 = 0;
  }
  else {
    DAT_0045d1c8 = *(undefined4 *)(&DAT_00486f90 + local_8 * 4);
  }
  FUN_004089b0(param_1);
  FUN_00407420(0);
  return;
}
