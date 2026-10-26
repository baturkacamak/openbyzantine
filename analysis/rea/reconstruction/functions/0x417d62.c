// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x417d62
// name: FUN_00417d62
// ranges: 0x417d62-0x417dc6
// calls: 0x417d62, 0x418040

void __cdecl FUN_00417d62(LPRECT param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00418040(DAT_0047a8a4);
  SetRect(param_1,5,0x14a,0x280,0x1a4);
  (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,5,0x14a,uVar1,param_1,0);
  DAT_0045f204 = 0;
  return;
}
