// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x421373
// name: FUN_00421373
// ranges: 0x421373-0x42151d
// calls: 0x41db33, 0x42130c, 0x421373, 0x44ce20, 0x44d340

undefined4 * __cdecl FUN_00421373(FILE *param_1,int param_2,undefined4 param_3)

{
  bool bVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 local_170;
  int local_164;
  int local_15c;
  char local_158 [80];
  int aiStack_108 [32];
  int local_88;
  int aiStack_84 [32];
  
  bVar1 = false;
  iVar2 = FUN_0044d340(param_1,(byte *)s_Scene___d_Dialog___d_0045fcc0);
  if (iVar2 == 2) {
    for (local_15c = 0; iVar2 = local_15c, local_15c < 0x20; local_15c = local_15c + 1) {
      local_88 = FUN_0044d340(param_1,(byte *)s__d___d_0045fcd8);
      if (local_88 != 2) break;
      if (aiStack_108[local_15c] == param_2) {
        bVar1 = true;
      }
    }
    FUN_0044ce20(local_158,0x50,param_1);
    if (bVar1) {
      puVar3 = FUN_0042130c(local_170,local_15c,param_3);
      for (local_15c = 0; local_15c < iVar2; local_15c = local_15c + 1) {
        *(int *)(puVar3[3] + local_15c * 4) = aiStack_108[local_15c];
        FUN_0041db33(local_164,aiStack_84[local_15c],(undefined4 *)(puVar3[4] + local_15c * 4));
      }
    }
    else {
      puVar3 = (undefined4 *)0x0;
    }
  }
  else {
    puVar3 = (undefined4 *)0x0;
  }
  return puVar3;
}
