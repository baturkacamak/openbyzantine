// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40701f
// name: FUN_0040701f
// ranges: 0x40701f-0x40711a
// calls: 0x4061e7, 0x40701f, 0x44ac40, 0x44ba00, 0x44c2f0, 0x44c580

int __cdecl FUN_0040701f(uint *param_1,int param_2,int param_3)

{
  char *pcVar1;
  uint *puVar2;
  int local_14;
  char *local_c;
  int local_8;
  
  local_c = (char *)0x0;
  local_8 = 1;
  pcVar1 = FUN_0044c580(param_1);
  puVar2 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar1 + 1));
  FUN_0044ba00(puVar2,param_1);
  local_14 = FUN_004061e7((int)param_1,param_3);
  while (param_2 < local_14) {
    local_c = _strrchr((char *)param_1,0x20);
    *local_c = '\0';
    local_14 = FUN_004061e7((int)param_1,param_3);
  }
  if (local_c != (char *)0x0) {
    pcVar1 = FUN_0044c580(param_1);
    FUN_0044ba00((uint *)(local_c + 1),(uint *)(pcVar1 + 1 + (int)puVar2));
    *local_c = '\n';
    local_8 = FUN_0040701f((uint *)(local_c + 1),param_2,param_3);
    local_8 = local_8 + 1;
  }
  FUN_0044ac40(puVar2);
  return local_8;
}
