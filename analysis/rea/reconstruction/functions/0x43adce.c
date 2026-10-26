// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43adce
// name: FUN_0043adce
// ranges: 0x43adce-0x43af72
// calls: 0x4146a7, 0x414a53, 0x43adce, 0x43d73d

undefined4 __thiscall FUN_0043adce(void *this,undefined4 param_1,undefined4 param_2)

{
  int local_94;
  int local_90 [33];
  undefined4 local_c;
  undefined4 local_8;
  
  *(undefined4 *)(*(int *)((int)this + 0x54) + 0x14) = param_2;
  FUN_004146a7(*(void **)((int)this + 8),&local_8,&local_94,local_90);
  *(int *)(*(int *)((int)this + 0x58) + 4) = local_94 / 2;
  *(int *)(*(int *)((int)this + 0x58) + 8) = -local_90[0];
  if (*(int *)((int)this + 0x40) == 0x31345649) {
    local_c = FUN_0043d73d(*(void **)((int)this + 0xc),param_1,*(undefined4 *)((int)this + 0x54),
                           *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44),
                           *(undefined4 *)((int)this + 0x48),*(int *)((int)this + 0x58),local_8,
                           *(undefined4 *)((int)this + 0x2c),
                           (local_90[0] - *(int *)((int)this + 0x30)) - *(int *)((int)this + 0x50),
                           *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
  }
  else {
    local_c = FUN_0043d73d(*(void **)((int)this + 0xc),param_1,*(undefined4 *)((int)this + 0x54),
                           *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44),
                           *(undefined4 *)((int)this + 0x48),*(int *)((int)this + 0x58),local_8,
                           *(undefined4 *)((int)this + 0x2c),*(undefined4 *)((int)this + 0x30),
                           *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
  }
  FUN_00414a53(*(int *)((int)this + 8));
  return local_c;
}
