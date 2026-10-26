// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x456e50
// name: FUN_00456e50
// ranges: 0x456e50-0x456e67
// calls: 0x456e50

int __cdecl FUN_00456e50(short *param_1)

{
  short sVar1;
  short *psVar2;
  short *psVar3;
  
  psVar2 = param_1;
  do {
    psVar3 = psVar2 + 1;
    sVar1 = *psVar2;
    psVar2 = psVar3;
  } while (sVar1 != 0);
  return ((int)psVar3 - (int)param_1 >> 1) + -1;
}
