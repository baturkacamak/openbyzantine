// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432904
// name: FUN_00432904
// ranges: 0x432904-0x43299c
// calls: 0x432904

void __cdecl FUN_00432904(uint param_1,int param_2,char param_3)

{
  int iVar1;
  int local_c;
  
  iVar1 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  for (local_c = (int)param_3; local_c < 8; local_c = local_c + 1) {
    if (*(char *)(local_c + 1 + param_2) == '\x02') {
      *(byte *)(iVar1 + 9) =
           (&DAT_00460418)[*(ushort *)(param_2 + 10 + local_c * 4)] & *(byte *)(iVar1 + 9);
      *(byte *)(iVar1 + 9) =
           (&DAT_00460428)[*(ushort *)(param_2 + 10 + local_c * 4)] | *(byte *)(iVar1 + 9);
    }
  }
  return;
}
