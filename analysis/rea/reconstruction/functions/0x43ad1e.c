// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43ad1e
// name: FUN_0043ad1e
// ranges: 0x43ad1e-0x43ad8d; 0x43ad93-0x43adcd
// calls: 0x43ad1e

int __thiscall FUN_0043ad1e(void *this,undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = AVIStreamRead(*(undefined4 *)((int)this + 0x28),param_1,1,
                        *(undefined4 *)((int)this + 0x5c),*(undefined4 *)((int)this + 0x3c),param_2,
                        0);
  if (iVar1 == -0x7ffbbf99) {
    OutputDebugStringA(s_AVIERR_MEMORY_00461858);
  }
  else if (iVar1 == -0x7ffbbf93) {
    OutputDebugStringA(s_AVIERR_FILEREAD_00461868);
  }
  else if (iVar1 == -0x7ffbbf8c) {
    OutputDebugStringA(s_AVIERR_BUFFERTOOSMALL_00461840);
  }
  return iVar1;
}
