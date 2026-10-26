// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x455280
// name: __set_osfhnd
// ranges: 0x455280-0x455322
// calls: -

/* Library Function - Single Match
    __set_osfhnd
   
   Library: Visual Studio 1998 Release */

int __cdecl __set_osfhnd(int param_1,intptr_t param_2)

{
  int *piVar1;
  int iVar2;
  
  if ((uint)param_1 < DAT_004a7710) {
    piVar1 = (int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7U) >> 3));
    iVar2 = (param_1 & 0x1fU) * 8;
    if (*(int *)(*piVar1 + iVar2) == -1) {
      if (DAT_00463194 == 1) {
        if (param_1 == 0) {
          SetStdHandle(0xfffffff6,(HANDLE)param_2);
        }
        else if (param_1 == 1) {
          SetStdHandle(0xfffffff5,(HANDLE)param_2);
        }
        else if (param_1 == 2) {
          SetStdHandle(0xfffffff4,(HANDLE)param_2);
        }
      }
      *(intptr_t *)(*piVar1 + iVar2) = param_2;
      return 0;
    }
  }
  DAT_00462c74 = 9;
  DAT_00462c78 = 0;
  return -1;
}
