// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42658f
// name: FUN_0042658f
// ranges: 0x42658f-0x426661
// calls: 0x40ac38, 0x40ac43, 0x42658f

undefined4 __cdecl FUN_0042658f(int param_1,FILE *param_2)

{
  int iVar1;
  int iVar2;
  undefined1 local_14;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = 0;
  iVar1 = _fgetc(param_2);
  if ((char)iVar1 != 0xff) {
    local_c = param_1;
    for (local_14 = '\0'; local_14 < (char)iVar1; local_14 = local_14 + '\x01') {
      if (local_c == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
        local_8 = 1;
      }
      else {
        iVar2 = _fgetc(param_2);
        *(char *)(local_c + 0x10) = (char)iVar2;
        iVar2 = _fgetc(param_2);
        *(char *)(local_c + 0x11) = (char)iVar2;
        local_c = *(int *)(local_c + 8);
      }
    }
  }
  return local_8;
}
