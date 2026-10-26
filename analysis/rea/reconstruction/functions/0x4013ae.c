// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4013ae
// name: FUN_004013ae
// ranges: 0x4013ae-0x40186d
// calls: 0x401184, 0x4013ae, 0x401ee6, 0x402e00, 0x402e50, 0x408f32, 0x40d9f7, 0x40da1f, 0x40df05, 0x414c3d, 0x41624a, 0x417dc7, 0x42475f, 0x438cd0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_004013ae(void)

{
  int iVar1;
  DWORD DVar2;
  float10 fVar3;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  undefined4 local_24;
  DWORD local_20;
  float local_1c;
  int local_18;
  undefined4 local_14;
  float local_10;
  int local_c;
  uint local_8;
  
  local_28 = 0;
  local_20 = timeGetTime();
  local_14 = 0;
  if (((DAT_0045c060 != 0) && (DAT_00464880 != (void *)0x0)) &&
     (iVar1 = FUN_00417dc7((int)DAT_00486a0a,(int)DAT_00486a0c), iVar1 == 0)) {
    local_8 = (uint)DAT_00476338;
    while (((DAT_00486a0a < 0x32 || (0x24e < DAT_00486a0a)) ||
           ((DAT_00486a0c < 0x50 || (0x168 < DAT_00486a0c))))) {
      local_24 = 0;
      iVar1 = FUN_00417dc7((int)DAT_00486a0a,(int)DAT_00486a0c);
      if (((iVar1 != 0) || (DAT_00486a0c < 0x14)) || ((0x1a3 < DAT_00486a0c || (DAT_0046066c == 2)))
         ) break;
      FUN_0041624a();
      DVar2 = timeGetTime();
      if (DVar2 < local_20 + 500) {
        FUN_00401184(0);
        FUN_0041624a();
      }
      else {
        local_18 = 0;
        fVar3 = (float10)FUN_00402e00();
        local_1c = (float)fVar3;
        if (DAT_00486a0c < 0x50) {
          iVar1 = FUN_00402e50((int)DAT_00464880);
          if ((iVar1 == 0) || (0.0 <= local_1c)) {
            local_10 = 7.0;
            local_24 = 0x18;
          }
          else {
            local_18 = 1;
            local_10 = 0.0;
          }
        }
        else if (DAT_00486a0c < 0x169) {
          local_10 = 0.0;
        }
        else {
          local_10 = -7.0;
          iVar1 = FUN_00402e50((int)DAT_00464880);
          if ((iVar1 == 0) || (local_1c <= 0.0)) {
            if ((DAT_0045c05c == 0) || (local_1c + 7.0 < 13.9)) {
              local_24 = 0x19;
            }
            else {
              local_10 = local_1c - 13.9;
              if (local_10 <= -0.001) {
                local_24 = 0x19;
              }
              else {
                local_10 = 0.0;
              }
            }
          }
          else {
            local_18 = 1;
            local_10 = 0.0;
          }
        }
        if (DAT_00486a0a < 0x32) {
          local_c = DAT_00486a0a + -0x32;
          local_24 = 0x1a;
        }
        else if (DAT_00486a0a < 0x24f) {
          local_c = 0;
          if (local_18 != 0) break;
        }
        else {
          local_c = DAT_00486a0a + -0x24e;
          local_24 = 0x1b;
        }
        if (local_c < 1) {
          if (local_c < 0) {
            local_c = (local_c >> 3) + -1;
          }
        }
        else {
          local_c = (local_c >> 3) + 1;
        }
        FUN_0041624a();
        if (DAT_0045d1d4 != -1) {
          FUN_00408f32(0);
          _DAT_00476330 = 0xfffe;
        }
        if ((DAT_0045f070 == '\0') && (DAT_0048688c != (int *)0x0)) {
          local_38 = 0;
          local_34 = 0x1a4;
          local_30 = 0x280;
          local_2c = 0x1e0;
          DAT_0045f070 = '\x01';
          DAT_0045f2d8 = 0;
          FUN_0042475f(DAT_00464880,DAT_0048688c);
          (**(code **)(*DAT_0048688c + 0x1c))(DAT_0048688c,0,0x1a4,DAT_00486888,&local_38,0x10);
          local_34 = 0;
          local_2c = 0x14;
          (**(code **)(*DAT_0048688c + 0x1c))(DAT_0048688c,0,0,DAT_00486888,&local_38,0x10);
        }
        FUN_00401ee6(SUB84((double)local_c,0),(int)((ulonglong)(double)local_c >> 0x20),
                     SUB84((double)local_10,0),(int)((ulonglong)(double)local_10 >> 0x20),0);
        local_28 = 1;
        FUN_0040da1f();
        FUN_0040df05((ushort)local_24);
      }
    }
    if (local_28 != 0) {
      if (local_8 < 0x20) {
        FUN_0040df05(0);
      }
      else {
        FUN_0040df05((ushort)local_8);
      }
      FUN_00438cd0();
      if (DAT_0045f070 != '\0') {
        FUN_0040d9f7();
        FUN_00414c3d((byte)DAT_0045f2d8);
        (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0,DAT_004869d4,0,0x10);
        DAT_0045f070 = '\0';
        FUN_0042475f(DAT_00464880,DAT_00486888);
        FUN_0040da1f();
      }
    }
  }
  return;
}
