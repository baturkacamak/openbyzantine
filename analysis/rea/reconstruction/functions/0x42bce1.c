// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42bce1
// name: FUN_0042bce1
// ranges: 0x42bce1-0x42be06
// calls: 0x41842b, 0x42bc8b, 0x42bce1

void __cdecl FUN_0042bce1(int param_1,int param_2)

{
  int local_78 [28];
  undefined2 *local_8;
  
  local_8 = (undefined2 *)(param_1 + 0x6400);
  local_78[0] = 0;
  local_78[1] = 10;
  local_78[2] = 3;
  local_78[3] = 0x18;
  local_78[4] = 0xf;
  local_78[5] = 0xc;
  local_78[6] = 4;
  local_78[7] = 0x15;
  local_78[8] = 0x11;
  local_78[9] = 2;
  local_78[10] = 0x13;
  local_78[0xb] = 0x1a;
  local_78[0xc] = 6;
  local_78[0xd] = 0xd;
  local_78[0xe] = 8;
  local_78[0xf] = 0x17;
  local_78[0x10] = 0x12;
  local_78[0x11] = 1;
  local_78[0x12] = 0x14;
  local_78[0x13] = 5;
  local_78[0x14] = 0x16;
  local_78[0x15] = 0xb;
  local_78[0x16] = 7;
  local_78[0x17] = 0xe;
  local_78[0x18] = 0x19;
  local_78[0x19] = 9;
  local_78[0x1a] = 0x10;
  for (local_78[0x1b] = 0; local_78[0x1b] < 0x1b; local_78[0x1b] = local_78[0x1b] + 1) {
    FUN_0042bc8b(local_8,(undefined2 *)(param_2 + 0x6400),local_78[local_78[0x1b]],0x1b);
    FUN_0041842b(param_1,0x3e800);
  }
  return;
}
