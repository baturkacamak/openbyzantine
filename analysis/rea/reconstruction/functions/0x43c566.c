// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c566
// name: FUN_0043c566
// ranges: 0x43c566-0x43c670
// calls: 0x43c566

undefined4 __cdecl FUN_0043c566(undefined4 *param_1,char param_2)

{
  char cVar1;
  uint *puVar2;
  DWORD DVar3;
  int local_c;
  
  DVar3 = timeGetTime();
  local_c = 0;
  do {
    if (0x15d < local_c) {
      return 0xffff;
    }
    puVar2 = *(uint **)(&DAT_00464f60 + local_c * 4);
    if (((puVar2 != (uint *)0x0) && ((char)puVar2[4] != '\0')) &&
       (((cVar1 = *(char *)((int)puVar2 + 0x11), cVar1 == '\x01' || (cVar1 == '\x03')) ||
        ((param_2 == '\0' && ((cVar1 == '\0' || (cVar1 == '\x02')))))))) {
      puVar2[1] = puVar2[1] + (DVar3 - puVar2[2]);
      puVar2[2] = DVar3;
      if (*puVar2 < puVar2[1]) {
        *(undefined1 *)(puVar2 + 4) = 0;
        *param_1 = *(undefined4 *)(&DAT_004654e0 + puVar2[3] * 4);
        return 2;
      }
    }
    local_c = local_c + 1;
  } while( true );
}
