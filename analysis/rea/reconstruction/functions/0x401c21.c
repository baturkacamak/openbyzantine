// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401c21
// name: FUN_00401c21
// ranges: 0x401c21-0x401c62
// calls: 0x401c21
// port: Stage panorama position [bounded]

void __cdecl FUN_00401c21(int param_1)

{
  if (param_1 < 0) {
    param_1 = 0;
  }
  else if (0x9bf < param_1) {
    param_1 = 0x9bf;
  }
  DAT_0045c064 = param_1;
  return;
}
