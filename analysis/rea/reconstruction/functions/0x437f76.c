// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x437f76
// name: FUN_00437f76
// ranges: 0x437f76-0x4380fe
// calls: 0x40ac38, 0x40ac43, 0x437f76, 0x438173, 0x44ba00, 0x44ba08

int __cdecl FUN_00437f76(uint *param_1)

{
  int local_8;
  
  *param_1 = *param_1 + 1;
  if (*param_1 < 10) {
    FUN_0044ba00((uint *)&DAT_0046e250,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00485980,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00484320,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_0047a290,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00482530,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00482430,(uint *)&DAT_0046e260);
    FUN_0044ba00((uint *)&DAT_00482ac0,(uint *)&DAT_0046e260);
    FUN_0044ba08((uint *)&DAT_0046e250,(uint *)&DAT_004615ac);
    FUN_0044ba08((uint *)&DAT_00485980,(uint *)&DAT_004615b0);
    FUN_0044ba08((uint *)&DAT_00484320,(uint *)&DAT_004615b4);
    FUN_0044ba08((uint *)&DAT_0047a290,(uint *)&DAT_004615b8);
    FUN_0044ba08((uint *)&DAT_00482530,(uint *)&DAT_004615bc);
    FUN_0044ba08((uint *)&DAT_00482430,(uint *)&DAT_004615c0);
    FUN_0044ba08((uint *)&DAT_00482ac0,(uint *)&DAT_004615c4);
    local_8 = FUN_00438173();
    if (local_8 == 0xff) {
      FUN_0044ba08((uint *)&DAT_0046e260,(uint *)&DAT_004615c8);
      local_8 = 0;
    }
  }
  else {
    FUN_0040ac43();
    FUN_0040ac38();
    local_8 = 1;
  }
  return local_8;
}
