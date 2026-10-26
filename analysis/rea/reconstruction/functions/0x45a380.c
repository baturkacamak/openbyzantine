// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x45a380
// name: write_char
// ranges: 0x45a380-0x45a39f
// calls: 0x45a440

/* Library Function - Single Match
    _write_char
   
   Library: Visual Studio 1998 Release */

void __cdecl write_char(byte param_1,int *param_2)

{
  uint uVar1;
  
  uVar1 = FUN_0045a440(param_1);
  if (uVar1 == 0xffffffff) {
    *param_2 = -1;
    return;
  }
  *param_2 = *param_2 + 1;
  return;
}
