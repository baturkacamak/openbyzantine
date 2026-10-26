// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x431e89
// name: FUN_00431e89
// ranges: 0x431e89-0x431ec1
// calls: 0x431e89

uint __cdecl FUN_00431e89(ushort param_1)

{
  uint in_EAX;
  uint uVar1;
  int iVar2;
  
  if (param_1 == 0) {
    uVar1 = in_EAX & 0xffff0000;
  }
  else {
    iVar2 = _rand();
    uVar1 = CONCAT22((short)((uint)(iVar2 / (int)(uint)param_1) >> 0x10),
                     (short)(iVar2 % (int)(uint)param_1));
  }
  return uVar1;
}
