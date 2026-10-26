// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x422b04
// name: FUN_00422b04
// ranges: 0x422b04-0x422c2c
// calls: 0x422b04, 0x422f37, 0x4275ce, 0x428229

undefined4 * __cdecl FUN_00422b04(LPSTR param_1,int param_2,int *param_3)

{
  bool bVar1;
  undefined4 *puVar2;
  int iVar3;
  HWND pHVar4;
  char *lpText;
  LPCSTR lpCaption;
  WPARAM wParam;
  UINT UVar5;
  LPARAM lParam;
  
  if (DAT_0045fd28 == -1) {
    iVar3 = FUN_00422f37(param_1,param_3);
    if (iVar3 == 0) {
      if (((param_3 == (int *)0x0) || (param_2 == -2)) || (param_3[10] == 0)) {
        bVar1 = false;
      }
      else {
        bVar1 = true;
      }
      if (bVar1) {
        (**(code **)(*DAT_0045fd14 + 0x3c))(DAT_0045fd14,0xfffff448);
      }
      DAT_0045fd18 = 1;
      DAT_0045fd28 = param_2;
      puVar2 = FUN_00428229(DAT_0045fd14,0);
      lParam = 0;
      wParam = 0x9c46;
      UVar5 = 0x111;
      pHVar4 = GetActiveWindow();
      SendMessageA(pHVar4,UVar5,wParam,lParam);
      if (bVar1) {
        FUN_004275ce(param_2,(int)puVar2,1);
        puVar2 = (undefined4 *)0x0;
      }
    }
    else {
      UVar5 = 0x30;
      lpCaption = (LPCSTR)0x0;
      lpText = s_Couldn_t_setup_spooling_0045fd30;
      pHVar4 = GetActiveWindow();
      MessageBoxA(pHVar4,lpText,lpCaption,UVar5);
      puVar2 = (undefined4 *)0x0;
    }
  }
  else {
    (**(code **)(*DAT_0045fd14 + 0x30))(DAT_0045fd14,0,0,1);
    puVar2 = (undefined4 *)0x0;
  }
  return puVar2;
}
