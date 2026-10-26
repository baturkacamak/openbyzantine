// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x412f77
// name: FUN_00412f77
// ranges: 0x412f77-0x41316e
// calls: 0x411dfc, 0x412206, 0x412f77

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

short FUN_00412f77(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  
  uVar1 = FUN_00411dfc(0x47dd70,(uint *)&DAT_004859a0,(ushort *)&DAT_004859d8);
  uVar2 = FUN_00411dfc(0x484e80,(uint *)&DAT_0046e800,(ushort *)&DAT_00477348);
  uVar3 = FUN_00411dfc(0x46e870,(uint *)&DAT_004859e0,(ushort *)&DAT_0046e24e);
  uVar4 = FUN_00411dfc(0x47a8e0,(uint *)&DAT_00483630,(ushort *)&DAT_00483048);
  uVar5 = FUN_00411dfc(0x483080,(uint *)&DAT_0047fa60,(ushort *)&DAT_0046e862);
  uVar6 = FUN_00411dfc(0x4848b0,(uint *)&DAT_00484310,&DAT_0048306e);
  uVar7 = FUN_00411dfc(0x46e280,(uint *)&DAT_0047a2a0,(ushort *)&DAT_004835fc);
  uVar8 = FUN_00411dfc(0x483d90,(uint *)&DAT_0046e240,(ushort *)&DAT_0046e814);
  uVar9 = FUN_00411dfc(0x485400,(uint *)&DAT_0047a270,&DAT_00485a00);
  uVar10 = FUN_00411dfc(0x47a2b0,(uint *)&DAT_0047ae60,(ushort *)&DAT_00483bb8);
  uVar11 = FUN_00411dfc(0x484330,(uint *)&DAT_00482440,(ushort *)&DAT_0047a25e);
  iVar12 = FUN_00412206((uint *)&DAT_00480320,0x483640);
  _DAT_0047a29e = (short)iVar12;
  return (short)uVar11 +
         (short)uVar10 +
         (short)uVar9 +
         (short)uVar8 +
         (short)uVar7 +
         (short)uVar6 + (short)uVar5 + (short)uVar4 + (short)uVar3 + (short)uVar2 + (short)uVar1;
}
