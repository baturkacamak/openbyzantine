// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42b5b2
// name: FUN_0042b5b2
// ranges: 0x42b5b2-0x42b678
// calls: 0x42b395, 0x42b5b2

uint __cdecl FUN_0042b5b2(uint param_1)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = CONCAT22(local_8._2_2_,(short)((param_1 & 0xffff) >> ((byte)DAT_00485c8c & 0x1f))) &
            0xffff001f;
  local_10 = CONCAT22(local_10._2_2_,(short)((param_1 & 0xffff) >> (DAT_00485c80 & 0x1f))) &
             0xffff001f;
  local_c = CONCAT22(local_c._2_2_,(short)((param_1 & 0xffff) >> (DAT_00485c78 & 0x1f))) &
            0xffff001f;
  uVar3 = FUN_0042b395(local_c);
  bVar1 = DAT_00485c78 & 0x1f;
  uVar4 = FUN_0042b395(local_10);
  bVar2 = DAT_00485c80 & 0x1f;
  uVar5 = FUN_0042b395(local_8);
  local_8 = (uVar5 & 0xffff) << ((byte)DAT_00485c8c & 0x1f) & 0xffff;
  return local_8 | (uVar4 & 0xffff) << bVar2 & 0xffff | (uVar3 & 0xffff) << bVar1 & 0xffff;
}
