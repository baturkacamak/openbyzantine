// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40513f
// name: FUN_0040513f
// ranges: 0x40513f-0x405194
// calls: 0x40513f

undefined4 __cdecl FUN_0040513f(int *param_1,int *param_2)

{
  if (*param_2 != 0) {
    GlobalFree((HGLOBAL)*param_2);
    *param_2 = 0;
  }
  if (*param_1 != 0) {
    mmioClose((HMMIO)*param_1,0);
    *param_1 = 0;
  }
  return 0;
}
