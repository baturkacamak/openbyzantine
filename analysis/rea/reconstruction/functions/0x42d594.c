// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42d594
// name: FUN_0042d594
// ranges: 0x42d594-0x42d82e; 0x42d834-0x42d869; 0x42d86f-0x42d8b9
// calls: 0x4013ae, 0x41624a, 0x426935, 0x42d42b, 0x42d594, 0x42d8ba, 0x42d915, 0x42db22, 0x43c466, 0x43c566, 0x43c8d2

ushort __cdecl FUN_0042d594(char *param_1,char param_2)

{
  ushort uVar1;
  ushort extraout_AX;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  
  uVar1 = 0xffff;
  DAT_0048598e = DAT_0047a23d;
  DAT_0048598d = DAT_0047a23c;
  if (param_2 == '\x02') {
    if (DAT_0045f058 == 0) {
      FUN_00426935();
    }
    else {
      DAT_0045f058 = 0;
    }
  }
  else {
    DAT_0048306d = '\0';
    DAT_0048253e = 0xffff;
    DAT_0047a230 = param_1;
  }
  if ((param_2 == '\x01') && (*DAT_0047a230 == -0x7d)) {
    uVar1 = FUN_0042d915();
    return uVar1;
  }
LAB_0042d62e:
  if (DAT_0047a230 == (char *)0x0) {
    return uVar1;
  }
  if (uVar1 != 0xffff) {
    return uVar1;
  }
  FUN_0041624a();
  _rand();
  uVar1 = FUN_0043c466(&DAT_0047a230,'\x01');
  FUN_0042d8ba(&DAT_0047a230);
  if ((*DAT_0047a230 == 'o') || (*DAT_0047a230 == -0x7d)) goto LAB_0042d69f;
  goto LAB_0042d766;
LAB_0042d69f:
  FUN_0041624a();
  FUN_004013ae();
  FUN_0041624a();
  FUN_0043c8d2(&DAT_0047a230,0);
  uVar1 = extraout_AX;
  if (DAT_0047a230 != (char *)0x0) {
    if (extraout_AX == 0xffff) {
      uVar2 = FUN_0043c566(&DAT_0047a230,'\0');
      uVar1 = (ushort)uVar2;
    }
    if ((*DAT_0047a230 == 'o') && (uVar1 == 2)) {
      uVar1 = 0xffff;
    }
    FUN_0042d8ba(&DAT_0047a230);
    if ((DAT_0046066c == 2) &&
       (iVar3 = FUN_0042d42b(s_vemazetme_00460c84,&DAT_0047a230), iVar3 != 0)) {
      DAT_0046066c = 3;
      uVar1 = 2;
    }
LAB_0042d766:
    if (((DAT_0047a230 != (char *)0x0) && (*DAT_0047a230 != 'o')) && (*DAT_0047a230 != -0x7d)) {
      DAT_00485978 = 1;
      DAT_0048306d = *DAT_0047a230;
      uVar4 = FUN_0042db22((int *)&DAT_0047a230,'\0');
      uVar1 = (ushort)uVar4;
      uVar4 = uVar4 & 0xffff;
      if (uVar4 < 0x10000) {
        if (uVar4 == 0xffff) {
          DAT_0047a230 = *(char **)(DAT_0047a230 + 0x2c);
          FUN_0042d8ba(&DAT_0047a230);
        }
        else if (uVar4 == 0) {
          DAT_0047a230 = (char *)0x0;
        }
        else if (uVar4 == 1) {
          if (DAT_0047a230 != (char *)0x0) {
            DAT_00484e28 = DAT_0047a230;
          }
          DAT_0047a230 = (char *)0x0;
        }
        else if (uVar4 == 2) {
          uVar1 = 0xffff;
        }
      }
      if (((DAT_0047a230 != (char *)0x0) && (*DAT_0047a230 == -0x7d)) && (param_2 == '\x01')) {
        uVar1 = FUN_0042d915();
      }
    }
  }
  goto LAB_0042d62e;
}
