// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42722b
// name: FUN_0042722b
// ranges: 0x42722b-0x427388
// calls: 0x41d440, 0x42722b, 0x44ac40, 0x44c2f0

void __cdecl FUN_0042722b(FILE *param_1)

{
  char *pcVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  int local_10;
  char local_c;
  char *local_8;
  
  local_8 = (char *)0x0;
  __fread_lk((char *)&DAT_0045fd2c,4,1,param_1);
  for (local_10 = 0; local_10 < 0x15e; local_10 = local_10 + 1) {
    iVar2 = _fgetc(param_1);
    if ((char)iVar2 == '\0') {
      uVar3 = FUN_0044c2f0(0x3c);
      *(undefined4 *)(&DAT_00486220 + local_10 * 4) = uVar3;
      __fread_lk(*(char **)(&DAT_00486220 + local_10 * 4),0x3c,1,param_1);
    }
  }
  iVar2 = _fgetc(param_1);
  local_c = (char)iVar2;
  while (local_c != -1) {
    pcVar4 = (char *)FUN_0044c2f0(0x5c);
    __fread_lk(pcVar4,0x5c,1,param_1);
    if ((*(int *)(pcVar4 + 0x30) == 0) && (iVar2 = FUN_0041d440(2), iVar2 == 0)) {
      FUN_0044ac40(pcVar4);
      pcVar1 = DAT_0046019c;
      pcVar4 = local_8;
    }
    else {
      pcVar4[0x54] = '\0';
      pcVar4[0x55] = '\0';
      pcVar4[0x56] = '\0';
      pcVar4[0x57] = '\0';
      pcVar4[0x58] = '\0';
      pcVar4[0x59] = '\0';
      pcVar4[0x5a] = '\0';
      pcVar4[0x5b] = '\0';
      pcVar1 = pcVar4;
      if (local_8 != (char *)0x0) {
        *(char **)(local_8 + 0x58) = pcVar4;
        pcVar1 = DAT_0046019c;
      }
    }
    local_8 = pcVar4;
    DAT_0046019c = pcVar1;
    iVar2 = _fgetc(param_1);
    local_c = (char)iVar2;
  }
  return;
}
