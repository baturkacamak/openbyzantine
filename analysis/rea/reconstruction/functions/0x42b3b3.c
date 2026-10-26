// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42b3b3
// name: FUN_0042b3b3
// ranges: 0x42b3b3-0x42b49e
// calls: 0x42b395, 0x42b3b3

void __cdecl FUN_0042b3b3(ushort *param_1,int param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  uint uVar4;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  for (local_14 = 0; local_14 < param_2; local_14 = local_14 + 1) {
    local_8 = CONCAT22(local_8._2_2_,(short)((int)(uint)*param_1 >> ((byte)DAT_00485c8c & 0x1f))) &
              0xffff001f;
    local_10 = CONCAT22(local_10._2_2_,(short)((int)(uint)*param_1 >> (DAT_00485c80 & 0x1f))) &
               0xffff001f;
    local_c = CONCAT22(local_c._2_2_,(short)((int)(uint)*param_1 >> (DAT_00485c78 & 0x1f))) &
              0xffff001f;
    uVar4 = FUN_0042b395(local_c);
    uVar1 = (ushort)((uVar4 & 0xffff) << (DAT_00485c78 & 0x1f));
    local_c = CONCAT22(local_c._2_2_,uVar1);
    uVar4 = FUN_0042b395(local_10);
    uVar2 = (ushort)((uVar4 & 0xffff) << (DAT_00485c80 & 0x1f));
    local_10 = CONCAT22(local_10._2_2_,uVar2);
    uVar4 = FUN_0042b395(local_8);
    uVar3 = (ushort)((uVar4 & 0xffff) << ((byte)DAT_00485c8c & 0x1f));
    local_8 = CONCAT22(local_8._2_2_,uVar3);
    *param_1 = uVar3 | uVar2 | uVar1;
    param_1 = param_1 + 1;
  }
  return;
}
