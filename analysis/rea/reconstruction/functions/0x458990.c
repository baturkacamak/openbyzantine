// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x458990
// name: __chsize
// ranges: 0x458990-0x458af6
// calls: 0x44be20, 0x44d8e0, 0x450fc0, 0x4553c0, 0x4594e0

/* Library Function - Single Match
    __chsize
   
   Library: Visual Studio 1998 Release */

int __cdecl __chsize(int _FileHandle,long _Size)

{
  DWORD DVar1;
  DWORD DVar2;
  uint uVar3;
  int iVar4;
  HANDLE hFile;
  BOOL BVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 *puVar9;
  uint in_stack_00001008;
  int in_stack_0000100c;
  
  FUN_0044be20();
  iVar7 = 0;
  if ((DAT_004a7710 <= in_stack_00001008) ||
     ((*(byte *)(*(int *)((int)&DAT_004a7720 + ((int)(in_stack_00001008 & 0xffffffe7) >> 3)) + 4 +
                (in_stack_00001008 & 0x1f) * 8) & 1) == 0)) {
    DAT_00462c74 = 9;
    return -1;
  }
  DVar1 = FUN_00450fc0(in_stack_00001008,0,1);
  if ((DVar1 == 0xffffffff) || (DVar2 = FUN_00450fc0(in_stack_00001008,0,2), DVar2 == 0xffffffff)) {
    return -1;
  }
  uVar8 = in_stack_0000100c - DVar2;
  if ((int)uVar8 < 1) {
    if ((int)uVar8 < 0) {
      FUN_00450fc0(in_stack_00001008,in_stack_0000100c,0);
      hFile = (HANDLE)FUN_004553c0(in_stack_00001008);
      BVar5 = SetEndOfFile(hFile);
      iVar7 = -(uint)(BVar5 == 0);
      if (iVar7 == -1) {
        DAT_00462c74 = 0xd;
        DAT_00462c78 = GetLastError();
      }
    }
  }
  else {
    puVar9 = (undefined4 *)register0x00000010;
    for (iVar6 = 0x400; puVar9 = puVar9 + 1, iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar9 = 0;
    }
    iVar6 = FUN_004594e0(in_stack_00001008,0x8000);
    do {
      uVar3 = 0x1000;
      if ((int)uVar8 < 0x1000) {
        uVar3 = uVar8;
      }
      iVar4 = FUN_0044d8e0(in_stack_00001008,(char *)&_FileHandle,uVar3);
      if (iVar4 == -1) {
        if (DAT_00462c78 == 5) {
          DAT_00462c74 = 0xd;
        }
        iVar7 = -1;
        break;
      }
      uVar8 = uVar8 - iVar4;
    } while (0 < (int)uVar8);
    FUN_004594e0(in_stack_00001008,iVar6);
  }
  FUN_00450fc0(in_stack_00001008,DVar1,0);
  return iVar7;
}
