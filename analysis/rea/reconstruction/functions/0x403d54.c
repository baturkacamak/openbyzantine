// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x403d54
// name: FUN_00403d54
// ranges: 0x403d54-0x404187; 0x40418d-0x404201; 0x404286-0x4042bc; 0x4042d2-0x40434c
// calls: 0x403d54, 0x404a15, 0x416220, 0x44ba08, 0x44be20

void __cdecl
FUN_00403d54(uint *param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int iVar1;
  undefined1 local_1004 [2048];
  undefined1 local_804 [2016];
  undefined4 uStackY_24;
  
  FUN_0044be20();
  local_804[0] = '\0';
  local_1004[0] = '\0';
  FUN_0044ba08((uint *)local_804,(uint *)s_ASSERT_D3DERR__0045c948);
  FUN_0044ba08((uint *)local_804,param_1);
  _sprintf(local_1004,s___Line____d__0045c958);
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  _sprintf(local_1004,s_File____s_0045c968);
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  if (param_4 < -0x7789fd39) {
    if (param_4 == -0x7789fd3a) {
      _sprintf(local_1004,s_D3DERR_EXECUTE_CREATE_FAILED_0045c9d8);
      goto LAB_004042d2;
    }
    if (param_4 == -0x7789fd44) {
      _sprintf(local_1004,s_D3DERR_BADMAJORVERSION_0045c980);
      goto LAB_004042d2;
    }
    if (param_4 == -0x7789fd43) {
      _sprintf(local_1004,s_D3DERR_BADMINORVERSION_0045c99c);
      goto LAB_004042d2;
    }
  }
  else {
    if (param_4 < -0x7789fd11) {
      if (param_4 == -0x7789fd12) {
        _sprintf(local_1004,s_D3DERR_LIGHT_SET_FAILED_0045caa4);
        goto LAB_004042d2;
      }
      switch(param_4) {
      case -0x7789fd39:
        _sprintf(local_1004,s_D3DERR_EXECUTE_DESTROY_FAILED_0045c9f8);
        break;
      case -0x7789fd38:
        _sprintf(local_1004,s_D3DERR_EXECUTE_LOCK_FAILED_0045ca30);
        break;
      case -0x7789fd37:
        _sprintf(local_1004,s_D3DERR_EXECUTE_UNLOCK_FAILED_0045ca84);
        break;
      case -0x7789fd36:
        _sprintf(local_1004,s_D3DERR_EXECUTE_LOCKED_0045ca50);
        break;
      case -0x7789fd35:
        _sprintf(local_1004,s_D3DERR_EXECUTE_NOT_LOCKED_0045ca68);
        break;
      case -0x7789fd34:
        _sprintf(local_1004,s_D3DERR_EXECUTE_FAILED_0045ca18);
        break;
      case -0x7789fd33:
        _sprintf(local_1004,s_D3DERR_EXECUTE_CLIPPED_FAILED_0045c9b8);
        break;
      default:
        goto switchD_004041fb_caseD_887602ce;
      case -0x7789fd30:
        _sprintf(local_1004,s_D3DERR_TEXTURE_NO_SUPPORT_0045cd10);
        break;
      case -0x7789fd2f:
        _sprintf(local_1004,s_D3DERR_TEXTURE_CREATE_FAILED_0045cc58);
        break;
      case -0x7789fd2e:
        _sprintf(local_1004,s_D3DERR_TEXTURE_DESTROY_FAILED_0045cc78);
        break;
      case -0x7789fd2d:
        _sprintf(local_1004,s_D3DERR_TEXTURE_LOCK_FAILED_0045ccd8);
        break;
      case -0x7789fd2c:
        _sprintf(local_1004,s_D3DERR_TEXTURE_UNLOCK_FAILED_D3D_0045cd68);
        break;
      case -0x7789fd2b:
        _sprintf(local_1004,s_D3DERR_TEXTURE_LOAD_FAILED_0045ccb8);
        break;
      case -0x7789fd2a:
        _sprintf(local_1004,s_D3DERR_TEXTURE_SWAP_FAILED_0045cd48);
        break;
      case -0x7789fd29:
        _sprintf(local_1004,s_D3DERR_TEXTURE_LOCKED_0045ccf8);
        break;
      case -0x7789fd28:
        _sprintf(local_1004,s_D3DERR_TEXTURE_NOT_LOCKED_0045cd2c);
        break;
      case -0x7789fd27:
        _sprintf(local_1004,s_D3DERR_TEXTURE_GETSURF_FAILED_0045cc98);
        break;
      case -0x7789fd26:
        _sprintf(local_1004,s_D3DERR_MATRIX_CREATE_FAILED_0045cb4c);
        break;
      case -0x7789fd25:
        _sprintf(local_1004,s_D3DERR_MATRIX_DESTROY_FAILED_0045cb6c);
        break;
      case -0x7789fd24:
        _sprintf(local_1004,s_D3DERR_MATRIX_SETDATA_FAILED_0045cbac);
        break;
      case -0x7789fd23:
        _sprintf(local_1004,s_D3DERR_MATRIX_GETDATA_FAILED_0045cb8c);
        break;
      case -0x7789fd22:
        _sprintf(local_1004,s_D3DERR_SETVIEWPORTDATA_FAILED_0045cc38);
        break;
      case -0x7789fd1c:
        _sprintf(local_1004,s_D3DERR_MATERIAL_CREATE_FAILED_0045cac0);
        break;
      case -0x7789fd1b:
        _sprintf(local_1004,s_D3DERR_MATERIAL_DESTROY_FAILED_0045cae0);
        break;
      case -0x7789fd1a:
        _sprintf(local_1004,s_D3DERR_MATERIAL_SETDATA_FAILED_0045cb28);
        break;
      case -0x7789fd19:
        _sprintf(local_1004,s_D3DERR_MATERIAL_GETDATA_FAILED_0045cb04);
      }
      goto LAB_004042d2;
    }
    if (param_4 < 1) {
      if (param_4 == 0) {
        _sprintf(local_1004,s_D3D_OK_0045c974);
        goto LAB_004042d2;
      }
      switch(param_4) {
      case -0x7789fd08:
        _sprintf(local_1004,s_D3DERR_SCENE_IN_SCENE_0045cc04);
        break;
      case -0x7789fd07:
        _sprintf(local_1004,s_D3DERR_SCENE_NOT_IN_SCENE_0045cc1c);
        break;
      case -0x7789fd06:
        _sprintf(local_1004,s_D3DERR_SCENE_BEGIN_FAILED_0045cbcc);
        break;
      case -0x7789fd05:
        _sprintf(local_1004,s_D3DERR_SCENE_END_FAILED_0045cbe8);
        break;
      default:
        goto switchD_004041fb_caseD_887602ce;
      }
      goto LAB_004042d2;
    }
  }
switchD_004041fb_caseD_887602ce:
  _sprintf(local_1004,s_no_error_associated_0045cd90);
LAB_004042d2:
  FUN_0044ba08((uint *)local_804,(uint *)local_1004);
  uStackY_24 = 0x404300;
  FUN_00404a15((uint *)local_804,-0x29a,(uint *)0x0,-0x29a);
  iVar1 = FUN_00416220();
  if (iVar1 != 0) {
    (**(code **)(*DAT_00486a04 + 0x28))();
  }
  uStackY_24 = 0x404336;
  MessageBoxA((HWND)0x0,local_804,s_Assert_Failed_0045cda8,0);
  if (param_5 != 0) {
                    /* WARNING: Subroutine does not return */
    ExitProcess(0);
  }
  return;
}
