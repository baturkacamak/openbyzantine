// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x437cad
// name: FUN_00437cad
// ranges: 0x437cad-0x437f75
// calls: 0x40ac38, 0x40ac43, 0x437cad, 0x437f76, 0x438173, 0x44ba00, 0x44ba08, 0x44c580

int FUN_00437cad(void)

{
  uint uVar1;
  char *pcVar2;
  uint local_c;
  int local_8;
  
  local_8 = 0;
  for (local_c = 0; ((&DAT_0046e260)[local_c] != '\0' && ((&DAT_0046e260)[local_c] != '.'));
      local_c = local_c + 1) {
  }
  if ((&DAT_0046e260)[local_c] == '\0') {
    FUN_0044ba08((uint *)&DAT_0046e260,(uint *)&DAT_00461524);
    local_8 = FUN_00437f76(&local_c);
  }
  else {
    uVar1 = local_c + 1;
    if ((&DAT_0046e261)[local_c] == '\0') {
      local_c = uVar1;
      local_8 = FUN_00437f76(&local_c);
    }
    else if (((((&DAT_0046e261)[local_c] & 0xdf) == 0x4c) &&
             (((&DAT_0046e262)[local_c] & 0xdf) == 0x43)) &&
            (((&DAT_0046e263)[local_c] & 0xdf) == 0x43)) {
      local_c = uVar1;
      FUN_0044ba00((uint *)&DAT_0046e250,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00485980,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00484320,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_0047a290,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00482430,(uint *)&DAT_0046e260);
      FUN_0044ba00((uint *)&DAT_00482ac0,(uint *)&DAT_0046e260);
      pcVar2 = FUN_0044c580((uint *)&DAT_00485980);
      pcVar2[0x48597f] = 'v';
      pcVar2 = FUN_0044c580((uint *)&DAT_00484320);
      pcVar2[0x48431e] = 'v';
      pcVar2 = FUN_0044c580((uint *)&DAT_00484320);
      pcVar2[0x48431f] = 'm';
      pcVar2 = FUN_0044c580((uint *)&DAT_0047a290);
      pcVar2[0x47a28f] = 'x';
      pcVar2 = FUN_0044c580((uint *)&DAT_00482430);
      pcVar2[0x48242f] = 'i';
      pcVar2 = FUN_0044c580((uint *)&DAT_00482ac0);
      pcVar2[0x482abf] = 'y';
      local_8 = FUN_00438173();
      if (local_8 == 0xff) {
        FUN_0040ac43();
        FUN_0040ac38();
        local_8 = 1;
      }
    }
    else if (((((&DAT_0046e261)[local_c] & 0xdf) != 0x43) ||
             (((&DAT_0046e262)[local_c] & 0xdf) != 0x43)) ||
            (((&DAT_0046e263)[local_c] & 0xdf) != 0)) {
      local_c = uVar1;
      FUN_0040ac43();
      FUN_0040ac38();
      local_8 = 1;
    }
  }
  return local_8;
}
