// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x401ddf
// name: FUN_00401ddf
// ranges: 0x401ddf-0x401ee5
// calls: 0x401ddf, 0x4241e2
// port: Consume panorama camera position [bounded]

void __fastcall FUN_00401ddf(undefined4 param_1,undefined4 param_2)

{
  float10 fVar1;
  float10 extraout_ST0;
  longlong lVar2;
  
  DAT_004654dc = 0;
  DAT_00468b90 = 0;
  if (DAT_0045c064 != -1) {
    if (DAT_00464878 == 0) {
      DAT_00468b90 = 0;
    }
    else {
      DAT_00468b90 = 0xfffffffa;
    }
    if (DAT_00464880 != 0) {
      FUN_004241e2(DAT_00464880,param_2);
    }
    if (DAT_00462ae8 == 0) {
      fVar1 = (float10)DAT_0045c064 / (float10)DAT_0046487c;
    }
    else {
      __adj_fdiv_m32i();
      fVar1 = extraout_ST0;
    }
    _floor((double)(fVar1 * (float10)360.0 + (float10)0.5));
    lVar2 = __ftol();
    DAT_004654dc = (int)lVar2;
    if (DAT_004654dc < 0) {
      DAT_004654dc = 0;
    }
    else if (0x168 < DAT_004654dc) {
      DAT_004654dc = 0x168;
    }
    DAT_0045c064 = -1;
  }
  return;
}
