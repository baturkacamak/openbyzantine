// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422c2d
// name: FUN_00422c2d
// ranges: 0x422c2d-0x422d3e; 0x422d44-0x422e6d; 0x422e73-0x422efe; 0x422f09-0x422f36
// calls: 0x40513f, 0x422770, 0x422c2d, 0x43cda0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00422c2d(HWND param_1,int param_2,short param_3)

{
  MMRESULT MVar1;
  LONG LVar2;
  
  if (param_2 == 2) {
    SendMessageA(param_1,0x111,0x39c47,0);
    if (DAT_0045fd20 != 0) {
      timeKillEvent(DAT_0045fd24);
      timeEndPeriod(0x32);
      while (LVar2 = InterlockedExchange((LONG *)&DAT_004618b0,1), LVar2 != 0) {
        Sleep(100);
      }
      DAT_0045fd20 = 0;
    }
  }
  else if (param_2 == 0x111) {
    if (param_3 == -0x63ba) {
      if (DAT_0045fd1c != 0) {
        SendMessageA(param_1,0x111,0x9c47,0);
      }
      if ((DAT_0045fd18 == 0) || (DAT_0045fd14 == (int *)0x0)) {
        DAT_0045fd20 = 0;
        DAT_0045fd1c = 0;
      }
      else {
        (**(code **)(*DAT_0045fd14 + 0x34))(DAT_0045fd14,0);
        (**(code **)(*DAT_0045fd14 + 0x30))(DAT_0045fd14,0,0,1);
        MVar1 = timeBeginPeriod(0x32);
        if (MVar1 == 0) {
          _DAT_004618b0 = 0;
          DAT_0045fd24 = timeSetEvent(0x32,10,FUN_0043cda0,0,1);
          if (DAT_0045fd24 != 0) {
            DAT_0045fd20 = 1;
          }
          DAT_0045fd1c = 1;
        }
        else {
          (**(code **)(*DAT_0045fd14 + 0x48))(DAT_0045fd14);
          DAT_0045fd20 = 0;
          DAT_0045fd1c = 0;
        }
      }
    }
    else if (param_3 == -0x63b9) {
      DAT_004867e4 = 1;
      if (DAT_0045fd20 != 0) {
        timeKillEvent(DAT_0045fd24);
        timeEndPeriod(0x32);
        while (LVar2 = InterlockedExchange((LONG *)&DAT_004618b0,1), LVar2 != 0) {
          Sleep(100);
        }
        DAT_0045fd20 = 0;
      }
      if (DAT_0045fd1c != 0) {
        DAT_0045fd1c = 0;
        (**(code **)(*DAT_0045fd14 + 0x48))(DAT_0045fd14);
      }
      if (DAT_0045fd18 != 0) {
        FUN_0040513f(&DAT_004867a4,&DAT_004867a0);
        if (DAT_0045fd14 != (int *)0x0) {
          (**(code **)(*DAT_0045fd14 + 8))(DAT_0045fd14);
        }
        DAT_0045fd14 = (int *)0x0;
        DAT_0045fd18 = 0;
        FUN_00422770();
      }
    }
  }
  else if (param_2 == 0x500) {
    SendMessageA(param_1,0x111,0x9c47,0);
  }
  return;
}
