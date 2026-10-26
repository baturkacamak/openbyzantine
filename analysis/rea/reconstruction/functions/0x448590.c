// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x448590
// name: FUN_00448590
// ranges: 0x448590-0x4485ee
// calls: 0x448590

int __cdecl FUN_00448590(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = (*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x400);
  iVar4 = 0;
  do {
    iVar3 = 0;
    do {
      iVar1 = iVar3 + iVar4;
      iVar3 = iVar3 + 1;
      *(int *)(iVar2 + iVar1 * 4) =
           (int)((uint)(byte)(&DAT_0045b6b8)[iVar1] * -0x1fe + 0xfe01) / ((param_2 + -1) * 0x200);
    } while (iVar3 < 0x10);
    iVar4 = iVar4 + 0x10;
  } while (iVar4 < 0x100);
  return iVar2;
}
