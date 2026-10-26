// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x4123f9
// name: FUN_004123f9
// ranges: 0x4123f9-0x4124d8
// calls: 0x411db1, 0x4123f9, 0x44ba00, 0x44c2f0, 0x44c580, 0x44ce20

int __cdecl FUN_004123f9(FILE *param_1,int param_2,int param_3)

{
  uint uVar1;
  char *pcVar2;
  undefined4 uVar3;
  int local_c;
  char *local_8;
  
  local_c = 0;
  while (uVar1 = FUN_00411db1(&param_1->_ptr,param_3), uVar1 == 0) {
    FUN_0044ce20(&DAT_0047fa70,0x800,param_1);
    pcVar2 = FUN_0044c580((uint *)&DAT_0047fa70);
    local_8 = pcVar2;
    if (pcVar2[0x47fa6f] == '\n') {
      local_8 = pcVar2 + -1;
      pcVar2[0x47fa6f] = '\0';
    }
    if (local_8[0x47fa6f] == '\r') {
      local_8[0x47fa6f] = '\0';
      local_8 = local_8 + -1;
    }
    uVar3 = FUN_0044c2f0((SIZE_T)(local_8 + 1));
    *(undefined4 *)(param_2 + local_c * 4) = uVar3;
    FUN_0044ba00(*(uint **)(param_2 + local_c * 4),(uint *)&DAT_0047fa70);
    local_c = local_c + 1;
  }
  return local_c;
}
