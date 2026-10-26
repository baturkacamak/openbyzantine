// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x404d10
// name: FUN_00404d10
// ranges: 0x404d10-0x404f63
// calls: 0x404d10

MMRESULT __cdecl FUN_00404d10(LPSTR param_1,undefined4 *param_2,int *param_3,LPMMCKINFO param_4)

{
  LONG LVar1;
  HGLOBAL pvVar2;
  uint uVar3;
  _MMCKINFO local_34;
  uint local_20 [4];
  uint local_10;
  HMMIO local_c;
  MMRESULT local_8;
  
  *param_3 = 0;
  local_8 = 0;
  local_c = (HMMIO)0x0;
  local_c = mmioOpenA(param_1,(LPMMIOINFO)0x0,0x10000);
  if (local_c == (HMMIO)0x0) {
    local_8 = 0xe100;
  }
  else {
    local_8 = mmioDescend(local_c,param_4,(MMCKINFO *)0x0,0);
    if (local_8 == 0) {
      if ((param_4->ckid == 0x46464952) && (param_4->fccType == 0x45564157)) {
        local_34.ckid = 0x20746d66;
        local_8 = mmioDescend(local_c,&local_34,param_4,0x10);
        if (local_8 == 0) {
          if (local_34.cksize < 0x10) {
            local_8 = 0xe101;
          }
          else {
            LVar1 = mmioRead(local_c,(HPSTR)local_20,0x10);
            if (LVar1 == 0x10) {
              if ((local_20[0] & 0xffff) == 1) {
                local_10 = local_10 & 0xffff0000;
              }
              else {
                LVar1 = mmioRead(local_c,(HPSTR)&local_10,2);
                if (LVar1 != 2) {
                  local_8 = 0xe102;
                  goto LAB_00404f11;
                }
              }
              pvVar2 = GlobalAlloc(0,(local_10 & 0xffff) + 0x12);
              *param_3 = (int)pvVar2;
              if (*param_3 == 0) {
                local_8 = 0xe000;
              }
              else {
                FID_conflict__memcpy((void *)*param_3,local_20,0x10);
                *(undefined2 *)(*param_3 + 0x10) = (undefined2)local_10;
                if (((local_10 & 0xffff) == 0) ||
                   (uVar3 = mmioRead(local_c,(HPSTR)(*param_3 + 0x12),local_10 & 0xffff),
                   uVar3 == (local_10 & 0xffff))) {
                  local_8 = mmioAscend(local_c,&local_34,0);
                  if (local_8 == 0) goto LAB_00404f4f;
                }
                else {
                  local_8 = 0xe101;
                }
              }
            }
            else {
              local_8 = 0xe102;
            }
          }
        }
      }
      else {
        local_8 = 0xe101;
      }
    }
  }
LAB_00404f11:
  if (*param_3 != 0) {
    GlobalFree((HGLOBAL)*param_3);
    *param_3 = 0;
  }
  if (local_c != (HMMIO)0x0) {
    mmioClose(local_c,0);
    local_c = (HMMIO)0x0;
  }
LAB_00404f4f:
  *param_2 = local_c;
  return local_8;
}
