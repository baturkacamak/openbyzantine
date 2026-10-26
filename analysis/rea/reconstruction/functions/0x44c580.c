// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c580
// name: FUN_0044c580
// ranges: 0x44c580-0x44c5f6
// calls: 0x44c580

char * __cdecl FUN_0044c580(uint *param_1)

{
  uint uVar1;
  uint *puVar2;
  uint *puVar3;
  
  puVar2 = param_1;
  do {
    if (((uint)puVar2 & 3) == 0) goto LAB_0044c59c;
    uVar1 = *puVar2;
    puVar2 = (uint *)((int)puVar2 + 1);
  } while ((char)uVar1 != '\0');
LAB_0044c5cf:
  return (char *)((int)puVar2 + (-1 - (int)param_1));
LAB_0044c59c:
  do {
    do {
      puVar3 = puVar2;
      puVar2 = puVar3 + 1;
    } while (((*puVar3 ^ 0xffffffff ^ *puVar3 + 0x7efefeff) & 0x81010100) == 0);
    uVar1 = *puVar3;
    if ((char)uVar1 == '\0') {
      return (char *)((int)puVar3 - (int)param_1);
    }
    if ((char)(uVar1 >> 8) == '\0') {
      return (char *)((int)puVar3 + (1 - (int)param_1));
    }
    if ((uVar1 & 0xff0000) == 0) {
      return (char *)((int)puVar3 + (2 - (int)param_1));
    }
  } while ((uVar1 & 0xff000000) != 0);
  goto LAB_0044c5cf;
}
