// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c114
// name: FUN_0043c114
// ranges: 0x43c114-0x43c1a7
// calls: 0x43c114

void __cdecl
FUN_0043c114(uint param_1,undefined2 *param_2,undefined2 *param_3,undefined2 *param_4,
            undefined2 *param_5,char param_6)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 *puVar3;
  int local_14;
  
  puVar3 = *(undefined2 **)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  if (param_6 == '\0') {
    local_14 = *(int *)(puVar3 + 0xe);
    uVar1 = *puVar3;
    uVar2 = puVar3[1];
  }
  else {
    local_14 = *(int *)(puVar3 + 0x10);
    uVar1 = puVar3[0x12];
    uVar2 = puVar3[0x13];
  }
  *param_2 = uVar1;
  *param_3 = uVar2;
  *param_4 = (short)*(undefined4 *)(local_14 + 0xc);
  *param_5 = (short)*(undefined4 *)(local_14 + 0x10);
  return;
}
