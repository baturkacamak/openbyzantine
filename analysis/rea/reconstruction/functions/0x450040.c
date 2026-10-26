// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450040
// name: __except2
// ranges: 0x450040-0x450127
// calls: 0x450130, 0x4508f0

/* Library Function - Single Match
    __except2
   
   Library: Visual Studio 1998 Release */

float10 __cdecl
__except2(uint param_1,int param_2,uint param_3,undefined4 param_4,undefined4 param_5,
         undefined4 param_6,uint param_7,undefined4 param_8,uint param_9)

{
  bool bVar1;
  undefined3 extraout_var;
  int _Value;
  float10 fVar2;
  uint local_58 [10];
  undefined4 local_30;
  undefined4 local_2c;
  uint local_20;
  
  bVar1 = __handle_exc(param_1,(double *)&param_7,param_9);
  if (CONCAT31(extraout_var,bVar1) == 0) {
    local_20 = local_20 & 0xffffffe3 | 3;
    local_2c = param_6;
    local_30 = param_5;
    FUN_00450130(local_58,&param_9,param_1,param_2,&param_3,&param_7);
  }
  _Value = __errcode(param_1);
  if ((DAT_00463ebc == 0) && (_Value != 0)) {
    fVar2 = __umatherr(_Value,param_2);
    return fVar2;
  }
  __set_errno(_Value);
  FUN_004508f0();
  return (float10)(double)CONCAT44(param_8,param_7);
}
