// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42382b
// name: FUN_0042382b
// ranges: 0x42382b-0x423976
// calls: 0x42382b, 0x44a820, 0x44ac60
// port: Seam padding [bounded]

void __fastcall FUN_0042382b(int param_1)

{
  void *_Dst;
  undefined4 local_c;
  
  _Dst = (void *)FUN_0044ac60(*(int *)(param_1 + 0x38) * 2);
  for (local_c = 0; local_c < *(int *)(param_1 + 0x20); local_c = local_c + 1) {
    FID_conflict__memcpy
              (_Dst,(void *)(*(int *)(param_1 + 0x24) * 2 + *(int *)(param_1 + 0x1c) * local_c * 2 +
                            *(int *)(param_1 + 0x34)),*(int *)(param_1 + 0x38) * 2);
    FID_conflict__memcpy
              ((void *)(*(int *)(param_1 + 0x1c) * local_c * 2 + *(int *)(param_1 + 0x34)),_Dst,
               *(int *)(param_1 + 0x38) * 2);
  }
  for (local_c = 0; local_c < *(int *)(param_1 + 0x20); local_c = local_c + 1) {
    FID_conflict__memcpy
              (_Dst,(void *)(*(int *)(param_1 + 0x38) * 2 + *(int *)(param_1 + 0x1c) * local_c * 2 +
                            *(int *)(param_1 + 0x34)),*(int *)(param_1 + 0x38) * 2);
    FID_conflict__memcpy
              ((void *)(*(int *)(param_1 + 0x24) * 2 + *(int *)(param_1 + 0x1c) * local_c * 2 +
                        *(int *)(param_1 + 0x38) * 2 + *(int *)(param_1 + 0x34)),_Dst,
               *(int *)(param_1 + 0x38) * 2);
  }
  FUN_0044a820(_Dst);
  return;
}
