// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40c02c
// name: FUN_0040c02c
// ranges: 0x40c02c-0x40c328
// calls: 0x401ca6, 0x40c02c, 0x40d84d, 0x41624a, 0x417dc7

uint FUN_0040c02c(void)

{
  uint uVar1;
  undefined2 extraout_var;
  int iVar2;
  undefined2 extraout_var_00;
  uint local_18;
  uint local_14;
  undefined4 local_10;
  int local_c;
  int local_8;
  
  if (DAT_00482acd == '\0') {
    FUN_0041624a();
    if ((DAT_00486a0c < 0x14) && (DAT_0045f498 == 0)) {
      DAT_004859b4 = 0xffff;
      DAT_00483050 = 0xffff;
      uVar1 = CONCAT22(extraout_var,0xfffc);
    }
    else if (DAT_00486a0c < 0x1a4) {
      iVar2 = FUN_00417dc7((int)DAT_00486a0a,(int)DAT_00486a0c);
      if (iVar2 == 0) {
        local_c = (int)DAT_00486a0a;
        local_8 = (int)DAT_00486a0c;
        FUN_00401ca6(&local_c,(int *)&local_18);
        if ((DAT_004859b0 == local_18) && (DAT_004859b4 == local_14)) {
          uVar1 = CONCAT22(extraout_var_00,0xffff);
        }
        else {
          local_10 = (uint)local_10._2_2_ << 0x10;
          while ((local_10 & 0xffff) < (uint)DAT_0048306e) {
            if ((*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) != 0) &&
               (*(char *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 8) != '\0')) {
              if (*(char *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 9) == '\x02') {
                iVar2 = FUN_0040d84d();
                if (iVar2 == 5) {
                  DAT_004859b0 = (uint)**(ushort **)(&DAT_00464300 + (local_10 & 0xffff) * 4);
                  DAT_004859b4 = (uint)*(ushort *)
                                        (*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 2);
                  break;
                }
              }
              if (((((int)(uint)**(ushort **)(&DAT_00464300 + (local_10 & 0xffff) * 4) <=
                     (int)local_18) &&
                   ((int)local_18 <=
                    (int)((uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 4) +
                         (uint)**(ushort **)(&DAT_00464300 + (local_10 & 0xffff) * 4)))) &&
                  ((int)(uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 2) <=
                   (int)local_14)) &&
                 ((int)local_14 <=
                  (int)((uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 2) +
                       (uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 6)))) {
                DAT_004859b0 = local_18;
                DAT_004859b4 = local_14;
                break;
              }
            }
            local_10 = CONCAT22(local_10._2_2_,(short)local_10 + 1);
          }
          if ((local_10 & 0xffff) == (uint)DAT_0048306e) {
            local_10 = 0xfffe;
          }
          if ((local_10 & 0xffff) == DAT_00483050) {
            local_10 = 0xffff;
          }
          else {
            DAT_00483050 = local_10 & 0xffff;
          }
          uVar1 = local_10 & 0xffff;
        }
      }
      else {
        DAT_004859b4 = 0xffff;
        DAT_00483050 = 0xffff;
        uVar1 = CONCAT22((short)((uint)iVar2 >> 0x10),0xfffb);
      }
    }
    else {
      DAT_004859b4 = 0xffff;
      DAT_00483050 = 0xffff;
      uVar1 = 0xfffd;
    }
  }
  else {
    uVar1 = 0xfffe;
  }
  return uVar1;
}
