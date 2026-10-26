// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41fe46
// name: FUN_0041fe46
// ranges: 0x41fe46-0x42007a; 0x420080-0x420093; 0x4200ac-0x4200ba
// calls: 0x40df05, 0x41f94e, 0x41fe46, 0x42044d, 0x421949

void __cdecl FUN_0041fe46(undefined4 param_1)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x28; local_8 = local_8 + 1) {
    (&DAT_0046c5d0)[local_8] = 0;
  }
  switch(param_1) {
  case 0:
    DAT_0046c5ee = 1;
    DAT_0046c5f3 = 1;
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
    }
    DAT_0046c5d6 = 1;
    DAT_0046c5d7 = 1;
    DAT_0046c5d9 = 1;
    DAT_0046c5db = 1;
    DAT_0046c5da = 1;
    DAT_0046c5d8 = 1;
    DAT_0046c5dc = 0;
    for (local_8 = 0; local_8 < 7; local_8 = local_8 + 1) {
      if (*(int *)(&DAT_0045f528 + local_8 * 4) == 0) {
        FUN_0041f94e(local_8 + 6);
      }
    }
    break;
  case 1:
    DAT_0046c5d3 = 1;
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
      DAT_0046c5d0 = 1;
    }
    DAT_0046c5e0 = 1;
    DAT_0046c5de = 1;
    DAT_0046c5e1 = 1;
    DAT_0046c5f7 = 1;
    FUN_0042044d(0);
    break;
  case 2:
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
    }
    DAT_0046c5f4 = 1;
    DAT_0046c5df = 1;
    DAT_0046c5ee = 1;
    DAT_0046c5de = 1;
    DAT_0046c5e1 = 1;
    DAT_0046c5f7 = 1;
    break;
  case 3:
    DAT_0046c5ee = 1;
    DAT_0046c5ef = 1;
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
    }
    DAT_0046c5de = 1;
    break;
  case 4:
    DAT_0046c5ee = 1;
    DAT_0046c5f0 = 1;
    DAT_0046c5de = 1;
    DAT_0046c5f1 = 1;
    DAT_0046c5f2 = 1;
    break;
  case 5:
    DAT_0046c5ee = 1;
    DAT_0046c5dd = 1;
    DAT_0046c5de = 1;
    DAT_0046c5f1 = 1;
    FUN_00421949(-1,0);
  }
  FUN_0040df05(0x1d);
  return;
}
