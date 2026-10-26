// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x453660
// name: __setargv
// ranges: 0x453660-0x4536fa
// calls: 0x44c2f0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __setargv
   
   Library: Visual Studio 1998 Release */

int __cdecl __setargv(void)

{
  undefined4 *puVar1;
  byte *pbVar2;
  int local_8;
  int local_4;
  
  GetModuleFileNameA((HMODULE)0x0,&DAT_0046e100,0x104);
  _DAT_00462cac = &DAT_0046e100;
  pbVar2 = &DAT_0046e100;
  if (*DAT_004a7824 != 0) {
    pbVar2 = DAT_004a7824;
  }
  parse_cmdline(pbVar2,(undefined4 *)0x0,(byte *)0x0,&local_8,&local_4);
  puVar1 = (undefined4 *)FUN_0044c2f0(local_8 * 4 + local_4);
  if (puVar1 == (undefined4 *)0x0) {
    __amsg_exit(8);
  }
  parse_cmdline(pbVar2,puVar1,(byte *)(puVar1 + local_8),&local_8,&local_4);
  _DAT_00462c94 = puVar1;
  _DAT_00462c90 = local_8 + -1;
  return local_8 + -1;
}
