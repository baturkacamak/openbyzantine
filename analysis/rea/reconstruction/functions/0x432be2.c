// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x432be2
// name: FUN_00432be2
// ranges: 0x432be2-0x432c64
// calls: 0x432be2, 0x44ac40

void __cdecl FUN_00432be2(int param_1,int param_2)

{
  LPVOID local_c;
  LPVOID local_8;
  
  local_8 = (LPVOID)0x0;
  local_c = *(LPVOID *)(param_1 + 0x34);
  while( true ) {
    if (local_c == (LPVOID)0x0) {
      return;
    }
    if (*(int *)((int)local_c + 4) == param_2) break;
    local_8 = local_c;
    local_c = *(LPVOID *)((int)local_c + 8);
  }
  if (local_8 == (LPVOID)0x0) {
    *(undefined4 *)(param_1 + 0x34) = *(undefined4 *)((int)local_c + 8);
  }
  else {
    *(undefined4 *)((int)local_8 + 8) = *(undefined4 *)((int)local_c + 8);
  }
  FUN_0044ac40(local_c);
  return;
}
