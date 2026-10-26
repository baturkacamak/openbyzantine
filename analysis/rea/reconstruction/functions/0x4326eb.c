// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4326eb
// name: FUN_004326eb
// ranges: 0x4326eb-0x432903
// calls: 0x4326eb, 0x436652, 0x44c2f0

void __cdecl FUN_004326eb(uint param_1,int param_2)

{
  short *psVar1;
  bool bVar2;
  short sVar3;
  ushort uVar4;
  short *psVar5;
  short *local_c;
  
  bVar2 = false;
  psVar1 = *(short **)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  if ((*(byte *)((int)psVar1 + 9) & 0x20) == 0) {
    local_c = *(short **)(psVar1 + 0x1e);
    while ((local_c != (short *)0x0 && (!bVar2))) {
      if ((*local_c == *psVar1) && (local_c[1] == psVar1[1])) {
        bVar2 = true;
      }
      local_c = *(short **)(local_c + 2);
    }
    if (!bVar2) {
      psVar5 = (short *)FUN_0044c2f0(8);
      *psVar5 = *psVar1;
      psVar5[1] = psVar1[1];
      psVar5[2] = 0;
      psVar5[3] = 0;
      local_c = *(short **)(psVar1 + 0x1e);
      if (local_c == (short *)0x0) {
        *(short **)(psVar1 + 0x1e) = psVar5;
      }
      else {
        for (; *(int *)((int)local_c + 4) != 0; local_c = *(short **)((int)local_c + 4)) {
        }
        *(short **)((int)local_c + 4) = psVar5;
      }
    }
  }
  if (*(char *)(param_2 + 2) == '@') {
    sVar3 = FUN_00436652(param_2,1);
    *psVar1 = sVar3;
  }
  else if (*(char *)(param_2 + 2) == '\x02') {
    uVar4 = FUN_00436652(param_2,1);
    *psVar1 = *(short *)(&DAT_0047e2f0 +
                        (uint)*(ushort *)
                               (&DAT_0046ee00 + (uint)uVar4 * 2 + (uint)DAT_00482428 * 600) * 2);
  }
  if (*(char *)(param_2 + 3) == '@') {
    sVar3 = FUN_00436652(param_2,2);
    psVar1[1] = sVar3;
  }
  else if (*(char *)(param_2 + 3) == '\x02') {
    uVar4 = FUN_00436652(param_2,2);
    psVar1[1] = *(short *)(&DAT_0047e2f0 +
                          (uint)*(ushort *)
                                 (&DAT_0046ee00 + (uint)uVar4 * 2 + (uint)DAT_00482428 * 600) * 2);
  }
  psVar1[1] = psVar1[1] + 0x14;
  return;
}
