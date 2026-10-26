// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432b09
// name: FUN_00432b09
// ranges: 0x432b09-0x432be1
// calls: 0x4319b3, 0x432b09, 0x436652, 0x44c2f0

void __cdecl FUN_00432b09(uint param_1,int param_2)

{
  int iVar1;
  ushort uVar2;
  undefined4 uVar3;
  uint uVar4;
  uint *local_c;
  
  iVar1 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  if (*(int *)(iVar1 + 0x34) == 0) {
    uVar3 = FUN_0044c2f0(0xc);
    *(undefined4 *)(iVar1 + 0x34) = uVar3;
    local_c = *(uint **)(iVar1 + 0x34);
  }
  else {
    for (local_c = *(uint **)(iVar1 + 0x34); *(int *)((int)local_c + 8) != 0;
        local_c = *(uint **)((int)local_c + 8)) {
    }
    uVar3 = FUN_0044c2f0(0xc);
    *(undefined4 *)((int)local_c + 8) = uVar3;
    local_c = *(uint **)((int)local_c + 8);
  }
  local_c[2] = 0;
  uVar4 = FUN_004319b3(param_2,1);
  local_c[1] = uVar4 & 0xffff;
  uVar2 = FUN_00436652(param_2,2);
  *local_c = (uint)uVar2;
  return;
}
