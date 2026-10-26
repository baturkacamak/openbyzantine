// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x443e70
// name: FUN_00443e70
// ranges: 0x443e70-0x443eab
// calls: 0x443e70, 0x443eb0, 0x444000

void __cdecl FUN_00443e70(int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = (undefined4 *)(*(code *)**(undefined4 **)(param_1 + 4))(param_1,1,0x48);
  *(undefined4 **)(param_1 + 0x194) = puVar1;
  *puVar1 = FUN_00443eb0;
  iVar2 = 4;
  puVar1[1] = FUN_00444000;
  puVar1 = puVar1 + 0xe;
  do {
    *puVar1 = 0;
    iVar2 = iVar2 + -1;
    puVar1[-4] = 0;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  return;
}
