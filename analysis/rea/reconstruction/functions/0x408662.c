// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x408662
// name: FUN_00408662
// ranges: 0x408662-0x4087c0
// calls: 0x408662, 0x40c624, 0x416710, 0x44c2f0

undefined4 __cdecl FUN_00408662(int param_1)

{
  undefined4 uVar1;
  undefined1 *puVar2;
  int iVar3;
  int local_110;
  uint local_108 [65];
  
  if (DAT_0045d1d8 == 0) {
    DAT_0045d1d8 = 1;
    for (local_110 = 0; local_110 < 0x15e; local_110 = local_110 + 1) {
      *(undefined4 *)(&DAT_00486f90 + local_110 * 4) = 0;
    }
    for (local_110 = 0; local_110 < param_1; local_110 = local_110 + 1) {
      puVar2 = (undefined1 *)FUN_0044c2f0(0x18);
      *(undefined1 **)(&DAT_00486f90 + local_110 * 4) = puVar2;
      *puVar2 = 0;
      *(int *)(puVar2 + 4) = local_110 * 2 + 0x20;
      *(undefined4 *)(puVar2 + 0xc) = 0;
      *(undefined4 *)(puVar2 + 0x14) = 0;
      *(undefined4 *)(puVar2 + 0x10) = *(undefined4 *)(puVar2 + 0x14);
      *(int *)(puVar2 + 8) = local_110;
    }
    FUN_0040c624(local_108,(uint *)s_inventor_0045d238,(uint *)s_instrip_0045d230,
                 (uint *)&DAT_0045d228);
    iVar3 = FUN_00416710((char *)local_108,&DAT_0045d1b4,0,(uint *)0x0,(uint *)0x0);
    if (iVar3 == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
