// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42793f
// name: FUN_0042793f
// ranges: 0x42793f-0x427a00
// calls: 0x422823, 0x42793f, 0x44ac40

void __cdecl FUN_0042793f(int param_1)

{
  LPVOID local_c;
  LPVOID local_8;
  
  local_8 = DAT_0046019c;
  local_c = (LPVOID)0x0;
  while( true ) {
    if (local_8 == (LPVOID)0x0) {
      return;
    }
    if ((*(uint *)((int)local_8 + 0x10) == (uint)DAT_00482428) &&
       (*(int *)((int)local_8 + 0xc) == param_1)) break;
    local_c = local_8;
    local_8 = *(LPVOID *)((int)local_8 + 0x58);
  }
  if (*(int *)((int)local_8 + 0x14) == 2) {
    FUN_00422823(*(int *)((int)local_8 + 0xc));
  }
  *(undefined4 *)(&DAT_00485c90 + param_1 * 4) = *(undefined4 *)((int)local_8 + 0x54);
  if (local_c == (LPVOID)0x0) {
    DAT_0046019c = *(LPVOID *)((int)local_8 + 0x58);
  }
  else {
    *(undefined4 *)((int)local_c + 0x58) = *(undefined4 *)((int)local_8 + 0x58);
  }
  FUN_0044ac40(local_8);
  return;
}
