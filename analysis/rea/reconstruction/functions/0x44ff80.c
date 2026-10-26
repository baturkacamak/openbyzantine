// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44ff80
// name: __except1
// ranges: 0x44ff80-0x45003a
// calls: 0x450130, 0x4508f0

/* Library Function - Single Match
    __except1
   
   Library: Visual Studio 1998 Release */

float10 __cdecl
__except1(uint param_1,int param_2,uint param_3,undefined4 param_4,uint param_5,undefined4 param_6,
         uint param_7)

{
  bool bVar1;
  undefined3 extraout_var;
  int _Value;
  float10 fVar2;
  uint local_58 [14];
  uint local_20;
  
  bVar1 = __handle_exc(param_1,(double *)&param_5,param_7);
  if (CONCAT31(extraout_var,bVar1) == 0) {
    local_20 = local_20 & 0xfffffffe;
    FUN_00450130(local_58,&param_7,param_1,param_2,&param_3,&param_5);
  }
  _Value = __errcode(param_1);
  if ((DAT_00463ebc == 0) && (_Value != 0)) {
    fVar2 = __umatherr(_Value,param_2);
    return fVar2;
  }
  __set_errno(_Value);
  FUN_004508f0();
  return (float10)(double)CONCAT44(param_6,param_5);
}
