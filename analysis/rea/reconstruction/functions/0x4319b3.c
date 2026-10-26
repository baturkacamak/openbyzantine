// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4319b3
// name: FUN_004319b3
// ranges: 0x4319b3-0x431a35
// calls: 0x4319b3, 0x436652
// port: Operand resolution [partial]

undefined4 __cdecl FUN_004319b3(int param_1,int param_2)

{
  ushort uVar1;
  undefined2 extraout_var;
  undefined2 uVar2;
  short local_8;
  
  if (*(char *)(param_2 + 1 + param_1) == '\x02') {
    uVar1 = FUN_00436652(param_1,(byte)param_2);
    local_8 = *(short *)(&DAT_0047e2f0 +
                        (uint)*(ushort *)
                               (&DAT_0046ee00 + (uint)uVar1 * 2 + (uint)DAT_00482428 * 600) * 2);
    uVar2 = 0;
  }
  else {
    local_8 = FUN_00436652(param_1,(byte)param_2);
    uVar2 = extraout_var;
  }
  return CONCAT22(uVar2,local_8);
}
