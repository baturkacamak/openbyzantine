// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40c9ce
// name: FUN_0040c9ce
// ranges: 0x40c9ce-0x40cb25
// calls: 0x40c70e, 0x40c9ce

char __cdecl FUN_0040c9ce(int param_1)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int local_10;
  char local_c;
  
  cVar1 = DAT_00486a0e;
  local_c = '\0';
  if (((DAT_0045da74 == 1) || (DAT_0045da74 == 2)) && (param_1 != 0)) {
    DAT_0045da78 = param_1;
  }
  if ((DAT_00486a0e == '\0') || (iVar3 = FUN_0040c70e(param_1), cVar2 = DAT_00486a0e, iVar3 == 0)) {
    for (local_10 = 0; cVar2 = local_c, DAT_00486a0e = cVar1, local_10 < DAT_0045da7c;
        local_10 = local_10 + 1) {
      DAT_00486a0e = DAT_0046c440 + (char)local_10;
      if (cVar1 != DAT_00486a0e) {
        if ((DAT_0045da74 != 0) && (0 < local_10)) {
          DAT_00486a0e = DAT_0045e768;
        }
        if (DAT_0045da74 == 2) {
          if (param_1 == 3) {
            DAT_00486a0e = 'C';
          }
          else if ((param_1 == 4) || (param_1 == 5)) {
            DAT_00486a0e = 'D';
          }
          else {
            DAT_00486a0e = 'E';
          }
        }
        iVar3 = FUN_0040c70e(param_1);
        if (iVar3 != 0) {
          cVar2 = DAT_00486a0e;
          DAT_00486a0e = cVar1;
          return cVar2;
        }
      }
    }
  }
  return cVar2;
}
