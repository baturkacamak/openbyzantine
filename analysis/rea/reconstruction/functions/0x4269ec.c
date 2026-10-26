// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4269ec
// name: FUN_004269ec
// ranges: 0x4269ec-0x426b9c
// calls: 0x4259de, 0x4269a0, 0x4269ec, 0x44a460

int __cdecl FUN_004269ec(FILE *param_1)

{
  char *pcVar1;
  undefined4 *puVar2;
  int iVar3;
  int local_18;
  int local_14;
  byte local_10;
  int local_c;
  
  local_14 = 0;
  local_18 = 0;
  while ((local_18 < 0x15e && (local_14 == 0))) {
    pcVar1 = *(char **)(&DAT_004649e8 + local_18 * 4);
    if (pcVar1 == (char *)0x0) {
      _fputc(0xff,param_1);
    }
    else {
      if ((*(char *)(*(int *)(&DAT_0047dd70 + local_18 * 4) + 8) == '\0') ||
         (*(char *)(*(int *)(&DAT_0047dd70 + local_18 * 4) + 8) == '\x01')) {
        _fputc(1,param_1);
      }
      else {
        _fputc(0,param_1);
      }
      FUN_0044a460(pcVar1,0x40,1,param_1);
      FUN_004259de((int)pcVar1,param_1);
      local_14 = FUN_004269a0(*(undefined4 *)(pcVar1 + 0x2c),param_1);
      iVar3 = FUN_004269a0(*(undefined4 *)(pcVar1 + 0x30),param_1);
      local_14 = local_14 + iVar3;
      if (*(int *)(pcVar1 + 0x34) == 0) {
        _fputc(0,param_1);
      }
      else {
        local_10 = 0;
        for (local_c = *(int *)(pcVar1 + 0x34); local_c != 0; local_c = *(int *)(local_c + 8)) {
          local_10 = local_10 + 1;
        }
        _fputc((uint)local_10,param_1);
        puVar2 = *(undefined4 **)(pcVar1 + 0x34);
        while (puVar2 != (undefined4 *)0x0) {
          iVar3 = FUN_004269a0(*puVar2,param_1);
          local_14 = local_14 + iVar3;
          FUN_0044a460((char *)(puVar2 + 1),4,1,param_1);
        }
      }
    }
    local_18 = local_18 + 1;
  }
  return local_14;
}
