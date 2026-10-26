// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42826e
// name: FUN_0042826e
// ranges: 0x42826e-0x4283a5
// calls: 0x4058fd, 0x41d440, 0x422b04, 0x427d1a, 0x428229, 0x42826e, 0x428f8c, 0x44bc20

void __cdecl FUN_0042826e(uint *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  MMRESULT MVar3;
  int *local_118 [2];
  FILE *local_110;
  uint local_10c [65];
  uint local_8;
  
  if ((param_1[0xc] != 0) || (iVar1 = FUN_0041d440(2), iVar1 != 0)) {
    FUN_00428f8c(local_10c,param_1,(uint *)&DAT_004601e0);
    if (param_1[5] == 2) {
      puVar2 = FUN_00422b04((LPSTR)local_10c,-2,(int *)(param_1 + 6));
      if (puVar2 != (undefined4 *)0x0) {
        param_1[0x15] = (uint)puVar2;
      }
    }
    else {
      local_110 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_004601e8);
      if (local_110 != (FILE *)0x0) {
        _fclose(local_110);
        MVar3 = FUN_004058fd((LPSTR)local_10c,&local_8,local_118);
        if (MVar3 == 0) {
          puVar2 = FUN_00428229(local_118[0],local_8);
          param_1[0x15] = (uint)puVar2;
          FUN_00427d1a((int *)(param_1 + 6));
          (**(code **)(*local_118[0] + 0x30))(local_118[0],0,0,1);
        }
      }
    }
  }
  return;
}
