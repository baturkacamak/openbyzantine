// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43264d
// name: FUN_0043264d
// ranges: 0x43264d-0x4326ea
// calls: 0x43264d, 0x44ac40

void __cdecl FUN_0043264d(short *param_1)

{
  short *local_c;
  short *local_8;
  
  local_8 = *(short **)(param_1 + 0x1e);
  local_c = (short *)0x0;
  while( true ) {
    if (local_8 == (short *)0x0) {
      return;
    }
    if ((*local_8 == *param_1) && (local_8[1] == param_1[1])) break;
    local_c = local_8;
    local_8 = *(short **)(local_8 + 2);
  }
  if (local_c == (short *)0x0) {
    *(undefined4 *)(param_1 + 0x1e) = *(undefined4 *)(local_8 + 2);
  }
  else {
    *(undefined4 *)(local_c + 2) = *(undefined4 *)(local_8 + 2);
  }
  FUN_0044ac40(local_8);
  return;
}
