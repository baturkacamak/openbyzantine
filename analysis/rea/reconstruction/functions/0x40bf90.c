// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40bf90
// name: FUN_0040bf90
// ranges: 0x40bf90-0x40c02b
// calls: 0x40bf90

undefined4 __cdecl FUN_0040bf90(char param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = _tolower((int)param_1);
  cVar1 = (char)iVar2;
  if (cVar1 == 'a') {
    DAT_0045da74 = 3;
  }
  else if (cVar1 == 'b') {
    DAT_0045da74 = 1;
  }
  else if (cVar1 == 'c') {
    DAT_0045da74 = 2;
  }
  else {
    if (cVar1 != 'f') {
      return 1;
    }
    DAT_0045da74 = 0;
  }
  return 0;
}
