// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40d031
// name: FUN_0040d031
// ranges: 0x40d031-0x40d171; 0x40d177-0x40d190; 0x40d1a9-0x40d2a0
// calls: 0x40c624, 0x40d031, 0x40d3af, 0x40da1f, 0x41d440, 0x44bc20

void __cdecl FUN_0040d031(uint *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  FILE *local_114;
  uint *local_110;
  uint local_10c [65];
  int local_8;
  
  local_8 = 1;
  if (((param_2 != 2) || (iVar1 = FUN_0041d440(5), iVar1 != 0)) &&
     ((param_2 != 3 || (iVar1 = FUN_0041d440(4), iVar1 != 0)))) {
    switch(param_2) {
    case 0:
      local_110 = (uint *)s_video_slide_0045db4c;
      DAT_00460664 = 0x65;
      break;
    case 1:
      local_110 = (uint *)s_video_intro_0045db28;
      local_8 = 0;
      break;
    case 2:
      DAT_00460664 = 0x6a;
      local_110 = (uint *)s_video_site_0045db34;
      break;
    case 3:
      local_110 = (uint *)s_video_transit_0045db58;
      if (DAT_00464878 == 0) {
        if (DAT_00460668 == 2) {
          DAT_00460664 = 100;
          local_8 = 0;
        }
        else {
          DAT_00460664 = 0x65;
        }
      }
      else {
        DAT_00460664 = 100;
      }
      break;
    case 4:
      DAT_00460664 = 0x67;
      local_110 = (uint *)s_video_char_0045db40;
      break;
    case 5:
      local_110 = (uint *)s_video_anim_0045db68;
      local_8 = 0;
      break;
    default:
      local_110 = (uint *)s_video_0045db74;
    }
    _sprintf((char *)local_10c,s_avi__s_avi_0045db7c,param_1);
    local_114 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_0045db88);
    if (local_114 == (FILE *)0x0) {
      FUN_0040c624(local_10c,local_110,param_1,(uint *)&DAT_0045db8c);
      local_114 = (FILE *)FUN_0044bc20((char *)local_10c,&DAT_0045db94);
      if (local_114 == (FILE *)0x0) {
        return;
      }
    }
    _fclose(local_114);
    FUN_0040d3af(local_10c,param_2,param_3,param_4,local_8,1);
    iVar1 = __strcmpi((char *)param_1,s_mcaft1a1_0045db98);
    if (iVar1 == 0) {
      FUN_0040da1f();
    }
    if (param_2 == 5) {
      FUN_0040da1f();
    }
    else {
      DAT_004859cd = 1;
    }
  }
  return;
}
