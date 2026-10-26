// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x409d09
// name: FUN_00409d09
// ranges: 0x409d09-0x409db8
// calls: 0x4094f2, 0x409d09, 0x40a1e0, 0x40a40d, 0x40a4bc

void __cdecl FUN_00409d09(int param_1)

{
  uint local_8;
  
  FUN_0040a1e0(param_1,s_ANIMPOS_0045d468);
  local_8 = CONCAT22(local_8._2_2_,1);
  while ((local_8 & 0xffff) < 3) {
    if (*(char *)((local_8 & 0xffff) + 1 + param_1) == '@') {
      FUN_0040a40d(param_1,(byte)local_8,'\x01');
    }
    else if (*(char *)((local_8 & 0xffff) + 1 + param_1) == '\x02') {
      FUN_0040a4bc(param_1,local_8,'\x01','\x01');
    }
    local_8 = CONCAT22(local_8._2_2_,(short)local_8 + 1);
  }
  FUN_004094f2();
  return;
}
