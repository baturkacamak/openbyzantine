// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x457760
// name: copy_environ
// ranges: 0x457760-0x4577d3
// calls: 0x44c2f0

/* Library Function - Single Match
    _copy_environ
   
   Library: Visual Studio 1998 Release */

undefined4 * __cdecl copy_environ(int *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  char *pcVar4;
  int iVar5;
  int *piVar6;
  
  iVar5 = 0;
  if (param_1 != (int *)0x0) {
    iVar1 = *param_1;
    piVar6 = param_1;
    while (iVar1 != 0) {
      piVar6 = piVar6 + 1;
      iVar5 = iVar5 + 1;
      iVar1 = *piVar6;
    }
    puVar3 = (undefined4 *)FUN_0044c2f0(iVar5 * 4 + 4);
    if (puVar3 == (undefined4 *)0x0) {
      __amsg_exit(9);
    }
    iVar5 = *param_1;
    puVar2 = puVar3;
    while (iVar5 != 0) {
      pcVar4 = (char *)*param_1;
      param_1 = param_1 + 1;
      pcVar4 = __strdup(pcVar4);
      *puVar2 = pcVar4;
      puVar2 = puVar2 + 1;
      iVar5 = *param_1;
    }
    *puVar2 = 0;
    return puVar3;
  }
  return (undefined4 *)0x0;
}
