// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42875d
// name: FUN_0042875d
// ranges: 0x42875d-0x4287fe
// calls: 0x4058fd, 0x42875d

void __cdecl FUN_0042875d(int param_1)

{
  MMRESULT MVar1;
  uint local_8;
  
  if (param_1 == 1) {
    if ((DAT_004601a0 == (int *)0x0) &&
       (MVar1 = FUN_004058fd(s_sounds_pxstart_wav_00460204,&local_8,&DAT_004601a0), MVar1 == 0)) {
      (**(code **)(*DAT_004601a0 + 0x30))(DAT_004601a0,0,0,1);
    }
  }
  else if (DAT_004601a0 != (int *)0x0) {
    (**(code **)(*DAT_004601a0 + 0x48))(DAT_004601a0);
    (**(code **)(*DAT_004601a0 + 8))(DAT_004601a0);
    DAT_004601a0 = (int *)0x0;
  }
  return;
}
