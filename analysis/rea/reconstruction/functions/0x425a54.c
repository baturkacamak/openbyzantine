// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x425a54
// name: FUN_00425a54
// ranges: 0x425a54-0x425ab3
// calls: 0x425a54

bool __cdecl FUN_00425a54(int param_1)

{
  BOOL BVar1;
  DWORD local_18;
  DWORD local_14;
  DWORD local_10;
  DWORD local_c;
  int local_8;
  
  local_8 = 0;
  BVar1 = GetDiskFreeSpaceA((LPCSTR)0x0,&local_18,&local_10,&local_c,&local_14);
  if (BVar1 != 0) {
    local_8 = local_c * local_10 * local_18;
  }
  return param_1 < local_8;
}
