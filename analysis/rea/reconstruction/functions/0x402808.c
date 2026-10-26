// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x402808
// name: FUN_00402808
// ranges: 0x402808-0x402a43
// calls: 0x401ee6, 0x40278c, 0x402808, 0x402e00, 0x402e70, 0x4176d8, 0x418040, 0x4181c5, 0x42427b

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00402808(void)

{
  DWORD DVar1;
  int *piVar2;
  float10 fVar3;
  float fVar4;
  tagRECT local_2c;
  tagRECT local_1c;
  undefined4 local_c;
  undefined4 local_8;
  
  local_c = 0;
  local_8 = 0x3ff00000;
  DVar1 = timeGetTime();
  if (DAT_0045c070 + _DAT_0045c068 < DVar1) {
    if (_DAT_0045c06c == 0.0) {
      _DAT_0045c06c = 0.0001;
      FUN_0040278c();
      DAT_0045c070 = 100;
    }
    else {
      _DAT_0045c06c = _DAT_0045c06c + 0.02;
      if (0.1 < _DAT_0045c06c) {
        _DAT_0045c06c = 0.0;
        DAT_0045c074 = 0x14;
        DAT_0045c070 = 5000;
      }
    }
    _DAT_0045c068 = timeGetTime();
  }
  if (_DAT_0045c06c == 0.0) {
    FUN_00401ee6(0,0,0,0,0);
  }
  else {
    DAT_0045c074 = DAT_0045c074 + 100;
    if (400 < DAT_0045c074) {
      DAT_0045c074 = 0x14;
    }
    FUN_0042427b(DAT_00464880,0,0,0,0,local_c,local_8,SUB84((double)_DAT_0045c06c,0),
                 (int)((ulonglong)(double)_DAT_0045c06c >> 0x20));
    if (DAT_0045c074 != 0x14) {
      piVar2 = (int *)FUN_00418040(DAT_0047a8a4);
      SetRect(&local_1c,0,DAT_0045c074,0x280,0x1a4);
      SetRect(&local_2c,0,0x14,0x280,DAT_0045c074);
      (**(code **)(*piVar2 + 0x1c))(piVar2,0,0x14,DAT_00486888,&local_1c);
      (**(code **)(*piVar2 + 0x1c))
                (piVar2,0,(local_1c.bottom + 0x14) - DAT_0045c074,DAT_00486888,&local_2c,0x10);
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0x14,piVar2,&DAT_0045c048,0x10);
    }
    piVar2 = DAT_00486888;
    fVar3 = (float10)FUN_00402e00();
    fVar4 = (float)fVar3;
    fVar3 = (float10)FUN_00402e70();
    FUN_004176d8((float)(fVar3 + (float10)180.0),fVar4,piVar2);
    FUN_004181c5(DAT_00486888,&DAT_0045c048,DAT_0045c048,DAT_0045c04c);
  }
  return;
}
