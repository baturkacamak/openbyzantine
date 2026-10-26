// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43669a
// name: FUN_0043669a
// ranges: 0x43669a-0x436720
// calls: 0x43669a

undefined4 __cdecl FUN_0043669a(int param_1,int param_2,undefined4 *param_3)

{
  char *pcVar1;
  
  pcVar1 = *(char **)(param_1 + 0x654 + (uint)*(ushort *)(param_2 + 0xe) * 4);
  while( true ) {
    if (pcVar1 == (char *)0x0) {
      return 0;
    }
    if (*(int *)(param_1 + 0x58c + (uint)*(ushort *)(param_2 + 10) * 4) == *(int *)(pcVar1 + 8))
    break;
    pcVar1 = *(char **)(pcVar1 + 4);
  }
  if (*pcVar1 == -1) {
    *param_3 = pcVar1;
  }
  return CONCAT22((short)((uint)pcVar1 >> 0x10),1);
}
