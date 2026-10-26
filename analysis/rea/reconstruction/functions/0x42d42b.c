// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42d42b
// name: FUN_0042d42b
// ranges: 0x42d42b-0x42d52e
// calls: 0x42d42b

undefined4 __cdecl FUN_0042d42b(char *param_1,undefined4 *param_2)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  int local_14;
  int local_8;
  
  local_8 = 0;
  bVar1 = false;
  local_14 = 0;
  while ((local_14 < (int)(uint)DAT_0047a8a0 && (!bVar1))) {
    if (*(char *)(*(undefined4 **)(&DAT_0047ae80 + local_14 * 4) + 1) == -0x1f) {
      local_8 = local_8 + 1;
      iVar2 = __strcmpi((char *)**(undefined4 **)(&DAT_0047ae80 + local_14 * 4),param_1);
      if (iVar2 == 0) {
        bVar1 = true;
      }
    }
    local_14 = local_14 + 1;
  }
  if (bVar1) {
    local_14 = 0;
    while ((local_14 < 0xdac && (0 < local_8))) {
      if ((*(int *)(&DAT_004654e0 + local_14 * 4) != 0) && (local_8 = local_8 + -1, local_8 == 0)) {
        *param_2 = *(undefined4 *)(&DAT_004654e0 + local_14 * 4);
      }
      local_14 = local_14 + 1;
    }
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  return uVar3;
}
