// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x417c33
// name: FUN_00417c33
// ranges: 0x417c33-0x417d61
// calls: 0x4063e0, 0x417c33, 0x44d0b4

undefined4 __cdecl FUN_00417c33(float param_1,float param_2,int *param_3)

{
  float10 fVar1;
  char local_18 [20];
  
  _sscanf((char *)(**(int **)(&DAT_00483d90 + DAT_00480330 * 4) + 2),&DAT_0045f2b0);
  fVar1 = (float10)FUN_0044d0b4(0,0x40000000);
  _sprintf(local_18,s_X___04_3f_0045f2b4,(double)(param_1 * 1.012));
  FUN_004063e0(local_18,0x26e,0xe0,1,param_3,0,2,1);
  _sprintf(local_18,s_Y___04_3f_0045f2c0,(double)param_2);
  FUN_004063e0(local_18,0x26e,0xf6,1,param_3,0,2,1);
  _sprintf(local_18,s_Z___04_3f_0045f2cc,(double)(float)(fVar1 + (float10)2.0));
  FUN_004063e0(local_18,0x26e,0x10c,1,param_3,0,2,1);
  return 0;
}
