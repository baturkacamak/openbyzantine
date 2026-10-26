// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c8d2
// name: FUN_0043c8d2
// ranges: 0x43c8d2-0x43cb80; 0x43cb86-0x43cb9f; 0x43cbb8-0x43ccad
// calls: 0x4078f4, 0x407c1d, 0x408076, 0x4091e5, 0x40c329, 0x40d862, 0x40df05, 0x4161f6, 0x417fd8, 0x42d42b, 0x438c80, 0x438cd0, 0x438de4, 0x4397ac, 0x43c8d2

int __cdecl FUN_0043c8d2(undefined4 *param_1,int param_2)

{
  bool bVar1;
  ushort uVar2;
  uint uVar3;
  undefined4 uVar4;
  undefined2 extraout_var;
  uint uVar5;
  HWND hWnd;
  int iVar6;
  char *lpText;
  LPCSTR lpCaption;
  UINT uType;
  short local_bc;
  undefined4 in_stack_ffffff4c;
  uint uVar7;
  ushort uVar8;
  
  uVar8 = (ushort)((uint)in_stack_ffffff4c >> 0x10);
  uVar7 = CONCAT22(uVar8,0xffff);
  bVar1 = false;
  uVar3 = FUN_0040c329();
  uVar3 = uVar3 & 0xffff;
  uVar4 = FUN_0040d862();
  local_bc = (short)uVar4;
  if (DAT_0045f058 != 0) {
    local_bc = 3;
  }
  switch(local_bc) {
  case 0:
    if (((uVar3 != 0xfffe) && (*(char *)(*(int *)(&DAT_00464300 + uVar3 * 4) + 9) == '\x17')) &&
       (*(char *)(*(int *)(&DAT_00464300 + uVar3 * 4) + 8) != '\0')) {
      uVar7 = CONCAT22(uVar8,2);
      bVar1 = true;
    }
    break;
  case 1:
  case 5:
    if (((uVar3 != 0xfffe) && (*(char *)(*(int *)(&DAT_00464300 + uVar3 * 4) + 8) != '\0')) &&
       ((local_bc == 1 ||
        ((local_bc == 5 && (*(char *)(*(int *)(&DAT_00464300 + uVar3 * 4) + 9) == '\x02')))))) {
      if (DAT_00476338 < 0x20) {
        uVar7 = CONCAT22(uVar8,2);
      }
      else {
        uVar2 = FUN_004078f4(DAT_00476338,(int *)0x0);
        if (CONCAT22(extraout_var,uVar2) != 0) {
          uVar7 = CONCAT22(uVar8,2);
        }
      }
    }
    break;
  case 2:
    FUN_004397ac();
    iVar6 = FUN_00408076();
    if (iVar6 != 0) {
      uVar5 = FUN_00438c80();
      if (uVar5 == 0xffffffff) {
        uType = 0;
        lpCaption = (LPCSTR)0x0;
        lpText = s_Not_linked_to_examine_code_0046188c;
        hWnd = (HWND)FUN_004161f6();
        MessageBoxA(hWnd,lpText,lpCaption,uType);
      }
      else {
        if (DAT_00482428 != uVar5) {
          FUN_00438de4(param_2);
          DAT_00482428 = (ushort)uVar5;
          DAT_004859cd = 1;
        }
        uVar7 = (uint)uVar8 << 0x10;
        *param_1 = 0;
      }
    }
    break;
  case 3:
    FUN_004091e5();
    DAT_00484e28 = *param_1;
    *param_1 = 0;
    return uVar7;
  case 4:
    if (DAT_0045f208 == 0) {
      if (DAT_0045f20c == 0) {
        return uVar7;
      }
      FUN_00417fd8();
      return uVar7;
    }
    if ((DAT_0045f498 != 0) && (DAT_004603d0 == 0)) {
      return CONCAT22(uVar8,1);
    }
    iVar6 = FUN_0042d42b(s_exitSim_004618a8,&DAT_0047a230);
    if (iVar6 == 0) {
      return uVar7;
    }
    FUN_004397ac();
    FUN_0040df05(0x11);
    return CONCAT22(uVar8,2);
  }
  if ((uVar7 & 0xffff) == 2) {
    FUN_00438cd0();
    if ((DAT_00476338 == 8) || (DAT_00476338 == 1)) {
      DAT_00460664 = 0x68;
    }
    else if (DAT_00476338 == 0xb) {
      DAT_00460664 = 0x67;
    }
    else if (DAT_00476338 == 0xc) {
      DAT_00460664 = 0x66;
    }
    else if (DAT_00476338 == 2) {
      DAT_00460664 = 100;
      FUN_00407c1d();
    }
    if (!bVar1) {
      FUN_0040df05(0x11);
    }
    FUN_004397ac();
    *param_1 = *(undefined4 *)
                (&DAT_004654e0 + *(int *)(*(int *)(&DAT_00464300 + uVar3 * 4) + 0xc) * 4);
  }
  return uVar7;
}
