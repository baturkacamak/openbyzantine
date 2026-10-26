// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c250
// name: FUN_0044c250
// ranges: 0x44c250-0x44c2e8
// calls: 0x44c250, 0x44c370, 0x450fc0

int __cdecl FUN_0044c250(FILE *param_1,LONG param_2,DWORD param_3)

{
  uint uVar1;
  DWORD DVar2;
  
  if (((param_1->_flag & 0x83U) != 0) && (((param_3 == 0 || (param_3 == 1)) || (param_3 == 2)))) {
    param_1->_flag = param_1->_flag & 0xffffffef;
    if (param_3 == 1) {
      param_3 = 0;
      DVar2 = FUN_0044c370(&param_1->_ptr);
      param_2 = param_2 + DVar2;
    }
    __flush(param_1);
    uVar1 = param_1->_flag;
    if ((uVar1 & 0x80) == 0) {
      if ((((uVar1 & 1) != 0) && ((uVar1 & 8) != 0)) && ((uVar1 & 0x400) == 0)) {
        param_1->_bufsiz = 0x200;
      }
    }
    else {
      param_1->_flag = uVar1 & 0xfffffffc;
    }
    DVar2 = FUN_00450fc0(param_1->_file,param_2,param_3);
    return -(uint)(DVar2 == 0xffffffff);
  }
  DAT_00462c74 = 0x16;
  return -1;
}
