// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x428f8c
// name: FUN_00428f8c
// ranges: 0x428f8c-0x429048; 0x42905d-0x429079
// calls: 0x40c624, 0x428f8c, 0x438afa, 0x44bc20

void __cdecl FUN_00428f8c(uint *param_1,uint *param_2,uint *param_3)

{
  FILE *_File;
  undefined4 uVar1;
  uint *local_c;
  
  _sprintf((char *)param_1,s_sounds__s_s_00460218,param_2,param_3);
  _File = (FILE *)FUN_0044bc20((char *)param_1,&DAT_00460224);
  if (_File == (FILE *)0x0) {
    uVar1 = FUN_00438afa();
    switch(uVar1) {
    case 1:
      local_c = (uint *)s_sounds_vt_00460258;
      break;
    case 2:
      local_c = (uint *)s_sounds_vb_00460228;
      break;
    case 3:
      local_c = (uint *)s_sounds_vc_00460240;
      break;
    case 4:
      local_c = (uint *)s_sounds_vh_0046024c;
      break;
    case 5:
      local_c = (uint *)s_sounds_va_00460234;
      break;
    default:
      local_c = (uint *)s_sounds_00460264;
    }
    FUN_0040c624(param_1,local_c,param_2,param_3);
  }
  else {
    _fclose(_File);
  }
  return;
}
