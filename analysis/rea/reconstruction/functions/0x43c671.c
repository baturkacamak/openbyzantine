// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c671
// name: FUN_0043c671
// ranges: 0x43c671-0x43c703
// calls: 0x43c671

void FUN_0043c671(void)

{
  int iVar1;
  DWORD DVar2;
  int local_c;
  
  for (local_c = 0; (local_c < 0x15e && (iVar1 = *(int *)(&DAT_00464f60 + local_c * 4), iVar1 != 0))
      ; local_c = local_c + 1) {
    if ((*(char *)(iVar1 + 0x10) != '\0') &&
       ((*(char *)(iVar1 + 0x11) == '\x02' || (*(char *)(iVar1 + 0x11) == '\x03')))) {
      *(undefined4 *)(iVar1 + 4) = 0;
      DVar2 = timeGetTime();
      *(DWORD *)(iVar1 + 8) = DVar2;
    }
  }
  return;
}
