// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41c704
// name: FUN_0041c704
// ranges: 0x41c704-0x41cae7; 0x41cb02-0x41cb12
// calls: 0x41a8e0, 0x41a962, 0x41ae57, 0x41c704, 0x44ac60, 0x44c2f0

DWORD __thiscall FUN_0041c704(void *this,undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *unaff_FS_OFFSET;
  undefined4 *local_1b8;
  undefined4 local_1b0;
  undefined4 local_1ac;
  undefined4 local_1a8;
  undefined4 local_1a4;
  undefined4 local_1a0;
  undefined4 local_19c;
  undefined4 local_140;
  int local_13c;
  undefined1 local_138 [4];
  undefined4 local_134;
  undefined4 local_130;
  undefined1 local_110 [4];
  int local_10c;
  DWORD local_84;
  undefined4 local_80;
  undefined1 local_7c [16];
  undefined4 local_6c;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0041caf8;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  local_80 = 0;
  local_1ac = 0;
  local_84 = AVIFileOpenA(&local_1ac,param_1,0x20,0);
  if (local_84 == 0) {
    AVIFileInfoA(local_1ac,local_7c,0x6c);
    *(undefined4 *)((int)this + 0x134) = local_6c;
    AVIFileRelease(local_1ac);
    local_84 = AVIStreamOpenFromFileA((int)this + 0x270,param_1,0x73646976,0,0x10,0);
    if (local_84 == 0) {
      AVIStreamInfoA(*(undefined4 *)((int)this + 0x270),local_110,0x8c);
      local_1b0 = 0x28;
      local_84 = AVIStreamReadFormat(*(undefined4 *)((int)this + 0x270),0,local_138,&local_1b0);
      if (local_84 == 0) {
        uVar1 = AVIStreamLength(*(undefined4 *)((int)this + 0x270));
        *(undefined4 *)((int)this + 0x274) = uVar1;
        if (*(int *)((int)this + 0x274) < 0x4d) {
          *(undefined4 *)((int)this + 0x278) = local_134;
          *(undefined4 *)((int)this + 0x27c) = local_130;
          _memset(&local_1a8,0,0x6c);
          local_1a8 = 0x6c;
          local_1a4 = 7;
          local_140 = 0x840;
          local_19c = *(undefined4 *)((int)this + 0x278);
          local_1a0 = *(undefined4 *)((int)this + 0x27c);
          local_84 = (**(code **)(*DAT_00486a04 + 0x18))
                               (DAT_00486a04,&local_1a8,(int)this + 0x268,0);
          if (local_84 == 0) {
            puVar2 = (undefined4 *)FUN_0044ac60(0x1d8);
            local_8 = 0;
            if (puVar2 == (undefined4 *)0x0) {
              local_1b8 = (undefined4 *)0x0;
            }
            else {
              local_1b8 = FUN_0041a8e0(puVar2);
            }
            local_8 = 0xffffffff;
            *(undefined4 **)((int)this + 0x26c) = local_1b8;
            local_84 = FUN_0041a962(*(void **)((int)this + 0x26c),local_10c,local_138,0);
            if ((local_84 == 0) &&
               (local_84 = FUN_0041ae57(*(void **)((int)this + 0x26c),*(int **)((int)this + 0x268)),
               local_84 == 0)) {
              for (local_13c = 0; local_84 = 0, local_13c < *(int *)((int)this + 0x274);
                  local_13c = local_13c + 1) {
                local_84 = AVIStreamRead(*(undefined4 *)((int)this + 0x270),local_13c,1,0,0,
                                         (int)this + local_13c * 4 + 0x138,0);
                if (local_84 != 0) goto LAB_0041cb02;
                uVar1 = FUN_0044c2f0(*(SIZE_T *)((int)this + local_13c * 4 + 0x138));
                *(undefined4 *)((int)this + local_13c * 4 + 4) = uVar1;
                local_84 = AVIStreamRead(*(undefined4 *)((int)this + 0x270),local_13c,1,
                                         *(undefined4 *)((int)this + local_13c * 4 + 4),
                                         *(undefined4 *)((int)this + local_13c * 4 + 0x138),0,0);
                if (local_84 != 0) goto LAB_0041cb02;
              }
              local_84 = 0;
            }
          }
        }
        else {
          local_84 = 1;
        }
      }
    }
  }
LAB_0041cb02:
  *unaff_FS_OFFSET = local_10;
  return local_84;
}
