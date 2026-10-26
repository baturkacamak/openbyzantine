// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x427de1
// name: FUN_00427de1
// ranges: 0x427de1-0x427ff4
// calls: 0x41d440, 0x422b04, 0x4275ce, 0x42793f, 0x427d1a, 0x427de1, 0x4283a6, 0x428f8c

void __cdecl FUN_00427de1(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint local_114 [65];
  int *local_10;
  undefined4 *local_c;
  int *local_8;
  
  local_10 = (int *)0x0;
  local_8 = *(int **)(&DAT_00486220 + param_1 * 4);
  if ((local_8[6] != 0) || (iVar1 = FUN_0041d440(2), iVar1 != 0)) {
    if (*(char *)(*(int *)(&DAT_0047a8e0 + param_1 * 4) + 8) == '\x02') {
      FUN_00428f8c(local_114,(uint *)**(undefined4 **)(&DAT_0047a8e0 + param_1 * 4),
                   (uint *)&DAT_004601d8);
      local_c = FUN_00422b04((LPSTR)local_114,param_1,*(int **)(&DAT_00486220 + param_1 * 4));
      if (local_c == (undefined4 *)0x0) {
        FUN_0042793f(param_1);
      }
      else {
        *(undefined4 **)(&DAT_00485c90 + param_1 * 4) = local_c;
      }
    }
    else {
      if (*(int *)(&DAT_00485c90 + param_1 * 4) != 0) {
        local_10 = *(int **)(*(int *)(&DAT_00485c90 + param_1 * 4) + 8);
      }
      if (local_10 == (int *)0x0) {
        uVar2 = FUN_004283a6(*(int **)(&DAT_00486220 + param_1 * 4),
                             (uint *)**(undefined4 **)(&DAT_0047a8e0 + param_1 * 4));
        *(char *)(*(int *)(&DAT_0047a8e0 + param_1 * 4) + 9) = (char)uVar2;
        if (*(char *)(*(int *)(&DAT_0047a8e0 + param_1 * 4) + 9) != '\0') {
          local_10 = *(int **)(*(int *)(&DAT_00485c90 + param_1 * 4) + 8);
        }
      }
      if (local_10 != (int *)0x0) {
        if (param_2 != 0) {
          (**(code **)(*local_10 + 0x34))(local_10,0);
        }
        FUN_00427d1a(local_8);
        if (*(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x28) != 0) {
          (**(code **)(*local_10 + 0x3c))(local_10,0xffffd8f0);
        }
        if (*(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x14) == 0) {
          (**(code **)(*local_10 + 0x30))(local_10,0,0,0);
        }
        else {
          (**(code **)(*local_10 + 0x30))(local_10,0,0,1);
        }
        if (*(int *)(*(int *)(&DAT_00486220 + param_1 * 4) + 0x28) != 0) {
          FUN_004275ce(param_1,0,0);
        }
      }
    }
  }
  return;
}
