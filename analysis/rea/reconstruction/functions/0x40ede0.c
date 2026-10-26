// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40ede0
// name: FUN_0040ede0
// ranges: 0x40ede0-0x40eec2; 0x40eec8-0x40eee5; 0x40eeeb-0x40ef17
// calls: 0x40ede0

undefined4 FUN_0040ede0(HWND param_1,int param_2,short param_3)

{
  HWND pHVar1;
  char *lpString;
  LPCSTR pCVar2;
  int iVar3;
  char local_8 [4];
  
  if (param_2 == 0x110) {
    pCVar2 = &DAT_0046c470;
    pHVar1 = GetDlgItem(param_1,0x3ea);
    SetWindowTextA(pHVar1,pCVar2);
    pCVar2 = &DAT_00486a08;
    pHVar1 = GetDlgItem(param_1,0x3eb);
    SetWindowTextA(pHVar1,pCVar2);
    SetFocus(param_1);
  }
  else if (param_2 == 0x111) {
    if (param_3 == 1) {
      iVar3 = 2;
      lpString = local_8;
      pHVar1 = GetDlgItem(param_1,0x3eb);
      GetWindowTextA(pHVar1,lpString,iVar3);
      iVar3 = FID_conflict___toupper_lk((int)local_8[0]);
      local_8[0] = (char)iVar3;
      if (('@' < local_8[0]) && (local_8[0] < '[')) {
        DAT_00486a08 = local_8[0];
        EndDialog(param_1,1);
      }
      return 1;
    }
    if (param_3 == 2) {
      EndDialog(param_1,0);
      return 1;
    }
  }
  return 0;
}
