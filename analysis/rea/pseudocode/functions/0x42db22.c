// rea decompile_all
// name: FUN_0042db22
// address: 0x42db22
// body: available
// range: 0x42db22-0x42db97 inclusive
// range: 0x42db9d-0x42e837 inclusive
// range: 0x42e83d-0x42e865 inclusive
// range: 0x42e8a3-0x431731 inclusive
// range: 0x431737-0x431757 inclusive
// range: 0x431920-0x43194e inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0042db22(int *param_1,char param_2)

{
  undefined2 uVar1;
  short sVar2;
  ushort uVar3;
  ushort uVar4;
  char *pcVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  undefined2 extraout_var;
  undefined4 uVar9;
  uint uVar10;
  undefined2 extraout_var_00;
  DWORD DVar11;
  undefined2 extraout_var_01;
  uint local_64;
  uint local_5c;
  uint local_58;
  uint local_50;
  int local_4c;
  uint local_48;
  undefined2 local_44 [2];
  uint local_40;
  int local_3c;
  uint local_38;
  uint local_34;
  uint local_30;
  undefined4 local_2c;
  uint local_28;
  undefined4 local_24;
  uint local_20;
  uint local_14;
  int *local_10;
  undefined2 local_8;
  
  local_10 = (int *)0x0;
  local_8 = 0xffff;
  DAT_0048598f = param_2;
  switch(*(undefined1 *)*param_1) {
  case 100:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_004324b0(local_28,*param_1);
      }
    }
    else {
      FUN_00409c5e(*param_1);
    }
    break;
  case 0x65:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar9 = FUN_0044c2f0(0x3c);
    *(undefined4 *)(&DAT_00486220 + (local_28 & 0xffff) * 4) = uVar9;
    **(uint **)(&DAT_00486220 + (local_28 & 0xffff) * 4) = local_28 & 0xffff;
    uVar9 = FUN_004319b3(*param_1,1);
    local_14 = CONCAT22(local_14._2_2_,(short)uVar9);
    if ((short)uVar9 == 0) {
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x14) = 1;
    }
    else {
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x14) = 0;
    }
    uVar10 = FUN_004319b3(*param_1,2);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x10) = uVar10 & 0xffff;
    uVar3 = FUN_00436652(*param_1,3);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0xc) = (uint)uVar3;
    uVar3 = FUN_00436652(*param_1,4);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 8) = (uint)uVar3;
    uVar3 = FUN_00436652(*param_1,5);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x18) = (uint)uVar3;
    if (*(char *)(*param_1 + 7) == '\x02') {
      *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 4) =
           (uint)*(ushort *)(*param_1 + 0x22);
    }
    else {
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 4) = 1;
    }
    if (((*(int *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x18) == 0) &&
        (iVar8 = FUN_0041d440(2), iVar8 == 0)) &&
       (*(int *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 4) == 0)) {
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 4) = 1;
    }
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x1c) = 10000;
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x20) = 0x32;
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x24) = 0x32;
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x28) = 0;
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x2c) = 0;
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x30) = 0xffffffff;
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x38) = 0;
    *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x34) = 0;
    break;
  case 0x66:
    break;
  case 0x67:
    if (param_2 == '\0') {
      uVar9 = FUN_004319b3(*param_1,0);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
      uVar3 = FUN_00436652(*param_1,1);
      local_14 = CONCAT22(local_14._2_2_,
                          *(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600));
      *(undefined2 *)
       (&DAT_0047e2f0 +
       (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) =
           (undefined2)local_28;
    }
    FUN_00409901(*param_1);
    break;
  case 0x68:
    local_4c = -1;
    if (DAT_00460660 == 0) {
      if (DAT_004603d0 == (undefined4 *)0x0) {
        if ((*(char *)(*param_1 + 2) == '\x02') && (sVar2 = FUN_00436652(*param_1,1), sVar2 == 0)) {
          local_4c = 0;
        }
        if ((local_4c != DAT_00464878) && (FUN_0042dad9(local_4c), DAT_0046019c != 0)) {
          FUN_004278ab();
        }
      }
    }
    else {
      FUN_00439349();
      DAT_00460660 = 0;
      pcVar5 = FUN_00432464(DAT_0047a238);
      *param_1 = (int)pcVar5;
      local_8 = 2;
    }
    break;
  case 0x69:
    FUN_00417a17((uint *)0x0,2);
    if (DAT_00485c6c == '\0') {
      if (*(char *)(*param_1 + 2) == '\x02') {
        iVar8 = 0;
        uVar3 = FUN_00436652(*param_1,1);
        uVar9 = FUN_00432133((uint)uVar3,iVar8);
        local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
      }
      else if (DAT_00460664 == -1) {
        local_28 = CONCAT22(local_28._2_2_,100);
      }
      else {
        local_28 = CONCAT22(local_28._2_2_,(short)DAT_00460664);
        DAT_00460664 = -1;
      }
      iVar8 = 0;
      uVar10 = local_28 & 0xffff;
      uVar3 = FUN_00436652(*param_1,0);
      FUN_00431ec2(uVar3,uVar10,iVar8);
    }
    else {
      FUN_0040a2c3(*param_1,&DAT_00484e80,s_BACKGRND_00460cbc);
    }
    DAT_0045c060 = 0;
    DAT_00480330 = 0xffffffff;
    FUN_0040d862();
    FUN_004177c7((char *)0x0);
    FUN_0041dcc9();
    break;
  case 0x6a:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) == 0) {
        uVar9 = FUN_0044c2f0(0x14);
        *(undefined4 *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) = uVar9;
      }
      uVar10 = FUN_004319b3(*param_1,1);
      local_14 = CONCAT22(local_14._2_2_,(short)uVar10);
      **(uint **)(&DAT_00464f60 + (local_28 & 0xffff) * 4) = uVar10 & 0xffff;
      uVar3 = FUN_00436652(*param_1,2);
      *(uint *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 0xc) = (uint)uVar3;
      *(undefined1 *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 0x10) = 1;
      DVar11 = timeGetTime();
      *(DWORD *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 8) = DVar11;
      *(undefined4 *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 4) = 0;
      if (*(char *)(*param_1 + 4) == '\x02') {
        *(undefined1 *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 0x11) =
             *(undefined1 *)(*param_1 + 0x16);
      }
      else {
        *(undefined1 *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 0x11) = 0;
      }
    }
    break;
  case 0x6b:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        *(undefined1 *)(*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) + 0x10) = 1;
        DVar11 = timeGetTime();
        *(DWORD *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 8) = DVar11;
      }
    }
    break;
  case 0x6c:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        *(undefined1 *)(*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) + 0x10) = 0;
      }
    }
    break;
  case 0x6d:
    if (param_2 == '\0') {
      local_2c = (uint)local_2c._2_2_ << 0x10;
      while ((local_2c & 0xffff) < 8) {
        if (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02') {
          uVar3 = FUN_00436652(*param_1,(byte)local_2c);
          local_28 = CONCAT22(local_28._2_2_,uVar3);
          if (*(int *)(&DAT_00464300 + (uint)uVar3 * 4) == 0) {
            FUN_0040ac43();
            FUN_0040ac38();
          }
          else {
            *(undefined1 *)(*(int *)(&DAT_00464300 + (uint)uVar3 * 4) + 8) = 1;
            FUN_00438cd0();
          }
        }
        local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
      }
    }
    break;
  case 0x6e:
    if (param_2 == '\0') {
      local_2c = (uint)local_2c._2_2_ << 0x10;
      while ((local_2c & 0xffff) < 8) {
        if (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02') {
          uVar3 = FUN_00436652(*param_1,(byte)local_2c);
          local_28 = CONCAT22(local_28._2_2_,uVar3);
          if (*(int *)(&DAT_00464300 + (uint)uVar3 * 4) == 0) {
            FUN_0040ac43();
            FUN_0040ac38();
          }
          else {
            *(undefined1 *)(*(int *)(&DAT_00464300 + (uint)uVar3 * 4) + 8) = 0;
            FUN_00438cd0();
          }
        }
        local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
      }
    }
    break;
  case 0x6f:
    break;
  case 0x70:
    if (param_2 == '\0') {
      if (DAT_0047a280 == 0) {
        if (DAT_0048306d != 'p') {
          FUN_00437c72();
        }
        uVar3 = *(ushort *)(*param_1 + 10);
        local_24 = CONCAT22(local_24._2_2_,uVar3);
        uVar3 = FUN_00410f09(uVar3,(char *)**(undefined4 **)(&DAT_0046e870 + (uint)uVar3 * 4));
        local_14 = CONCAT22(local_14._2_2_,uVar3);
        uVar9 = FUN_00437bcc(*(int *)(&DAT_00482ad0 + (uint)uVar3 * 4));
        _DAT_0046e824 = (undefined2)uVar9;
        DAT_0045d1d0 = 0;
        uVar1 = FUN_00432d6e((char)local_14,*(int **)(&DAT_00482ad0 + (local_14 & 0xffff) * 4),
                             CONCAT22((short)((uint)*param_1 >> 0x10),
                                      *(undefined2 *)(*param_1 + 0xe)));
        local_28 = CONCAT22(local_28._2_2_,uVar1);
        DAT_0045d1d0 = 1;
        FUN_00438cd0();
        if ((local_28 & 0xffff) == 0xfffe) {
          local_8 = 1;
        }
        else if ((local_28 & 0xffff) != 0xffff) {
          *param_1 = *(int *)(&DAT_004654e0 +
                             *(int *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0xc) * 4);
          local_8 = 2;
        }
      }
    }
    else {
      FUN_00409457(s_CONVERSE_00460cd0,'\x01');
      FUN_004599c0(&DAT_00460cdc);
      FUN_004599c0(&DAT_00460ce0);
      FUN_004094f2();
    }
    break;
  case 0x71:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) == (local_28 & 0xffff))) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x72:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    sVar2 = FUN_00436652(*param_1,6);
    local_14 = CONCAT22(local_14._2_2_,sVar2);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) == 0) {
        uVar9 = FUN_0044c2f0(0x18);
        *(undefined4 *)(&DAT_00464300 + (local_28 & 0xffff) * 4) = uVar9;
        sVar2 = FUN_00436652(*param_1,1);
        **(short **)(&DAT_00464300 + (local_28 & 0xffff) * 4) = sVar2;
        sVar2 = FUN_00436652(*param_1,2);
        *(short *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 2) = sVar2 + 0x14;
        sVar2 = FUN_00436652(*param_1,3);
        *(short *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 4) = sVar2;
        sVar2 = FUN_00436652(*param_1,4);
        *(short *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 6) = sVar2;
        *(undefined1 *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 8) = 0;
        uVar3 = FUN_00436652(*param_1,5);
        *(uint *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0xc) = (uint)uVar3;
        *(undefined1 *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 9) =
             (undefined1)local_14;
        *(undefined4 *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0x14) = 0xffffffff;
        if (*(char *)(*param_1 + 8) == '\x02') {
          uVar3 = FUN_00436652(*param_1,7);
          local_20 = CONCAT22(local_20._2_2_,uVar3);
          if ((*(char *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 9) == '\x14') ||
             (*(char *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 9) == '\x13')) {
            *(uint *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0x10) = (uint)uVar3;
          }
          else {
            uVar3 = FUN_00408bb2(uVar3);
            *(uint *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0x10) =
                 CONCAT22(extraout_var_00,uVar3);
          }
        }
        else {
          *(undefined4 *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0x10) = 0xffffffff;
        }
      }
      else {
        FUN_0040abb4();
      }
    }
    break;
  case 0x73:
    uVar3 = FUN_00436652(*param_1,0);
    uVar4 = (ushort)*(uint *)(&DAT_00482460 + (uint)uVar3 * 4);
    local_28 = CONCAT22(local_28._2_2_,uVar4);
    if (((param_2 == '\0') && ((*(uint *)(&DAT_00482460 + (uint)uVar3 * 4) & 0xffff) != 0xffff)) &&
       (local_8 = 0, DAT_00482428 = uVar4, DAT_004603d0 != (undefined4 *)0x0)) {
      DAT_00482428 = (ushort)*DAT_004603d0;
      DAT_00460660 = 1;
    }
    break;
  case 0x74:
    if (param_2 == '\0') {
      uVar9 = FUN_004319b3(*param_1,0);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
      uVar3 = FUN_00436652(*param_1,1);
      *(short *)(&DAT_0047e2f0 +
                (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) =
           *(short *)(&DAT_0047e2f0 +
                     (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) *
                     2) + (short)local_28;
    }
    else {
      FUN_00409a12(*param_1);
    }
    break;
  case 0x75:
    if (param_2 == '\0') {
      uVar9 = FUN_004319b3(*param_1,0);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
      uVar3 = FUN_00436652(*param_1,1);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2)
          < (local_28 & 0xffff)) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        uVar3 = FUN_00436652(*param_1,1);
        *(short *)(&DAT_0047e2f0 +
                  (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2)
             = *(short *)(&DAT_0047e2f0 +
                         (uint)*(ushort *)
                                (&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) -
               (short)local_28;
      }
    }
    else {
      FUN_00409ab8(*param_1);
    }
    break;
  case 0x76:
    if (param_2 == '\0') {
      uVar3 = FUN_00436652(*param_1,0);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x77:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) != (local_28 & 0xffff))) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x78:
    if (param_2 == '\0') {
      uVar3 = FUN_00436652(*param_1,0);
      uVar3 = *(ushort *)
               (&DAT_0047e2f0 +
               (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2);
      local_28 = CONCAT22(local_28._2_2_,uVar3);
      if ((uVar3 < 7) && (*(char *)(uVar3 + 2 + *param_1) == '\x02')) {
        uVar3 = FUN_00436652(*param_1,(char)uVar3 + 1);
        *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
        local_8 = 2;
      }
    }
    break;
  case 0x79:
    if ((param_2 == '\0') && (local_8 = 1, DAT_0045f490 == 0)) {
      DAT_0045f490 = 1;
    }
    break;
  case 0x7a:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    break;
  case 0x7b:
    if ((param_2 == '\0') && (DAT_00485c6c == '\0')) {
      uVar9 = FUN_004319b3(*param_1,0);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
    }
    FUN_00409881(*param_1);
    break;
  case 0x7d:
    if (param_2 == '\0') {
      if (*(char *)(*param_1 + 2) == '\x02') {
        uVar3 = FUN_00436652(*param_1,1);
        local_28 = CONCAT22(local_28._2_2_,
                            *(undefined2 *)
                             (&DAT_0047e2f0 +
                             (uint)*(ushort *)
                                    (&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2
                             ));
      }
      else if (*(char *)(*param_1 + 2) == '@') {
        sVar2 = FUN_00436652(*param_1,1);
        local_28 = CONCAT22(local_28._2_2_,sVar2);
      }
      else {
        local_28 = CONCAT22(local_28._2_2_,0xffff);
      }
      uVar3 = FUN_00436652(*param_1,0);
      uVar10 = FUN_00431e89((ushort)local_28);
      *(short *)(&DAT_0047e2f0 +
                (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) =
           (short)uVar10;
    }
    FUN_00409981(*param_1);
    break;
  case 0x7e:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar10 = FUN_004319b3(*param_1,1);
    local_14 = CONCAT22(local_14._2_2_,(short)uVar10);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        **(uint **)(&DAT_00464f60 + (local_28 & 0xffff) * 4) = uVar10 & 0xffff;
        *(undefined4 *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 4) = 0;
        DVar11 = timeGetTime();
        *(DWORD *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 8) = DVar11;
      }
    }
    break;
  case 0x7f:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    local_14 = CONCAT22(local_14._2_2_,
                        (short)**(undefined4 **)(&DAT_00464f60 + (uint)uVar3 * 4) -
                        (short)*(undefined4 *)(*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) + 4));
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        uVar3 = FUN_00436652(*param_1,1);
        *(undefined2 *)
         (&DAT_0047e2f0 +
         (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) =
             (undefined2)local_14;
      }
    }
    break;
  case 0x80:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        uVar3 = FUN_00436652(*param_1,1);
        *(uint *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 0xc) = (uint)uVar3;
      }
    }
    break;
  case 0x81:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        *(uint *)(*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) + 0xc) =
             (uint)*(ushort *)(*param_1 + 0xe);
      }
    }
    break;
  case 0x82:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        *(undefined4 *)(*(int *)(&DAT_00464f60 + (uint)uVar3 * 4) + 4) = 0;
        DVar11 = timeGetTime();
        *(DWORD *)(*(int *)(&DAT_00464f60 + (local_28 & 0xffff) * 4) + 8) = DVar11;
      }
    }
    break;
  case 0x83:
    break;
  case 0x84:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_004326eb(local_28,*param_1);
      }
    }
    else {
      FUN_00409d09(*param_1);
    }
    break;
  case 0x85:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432904(local_28,*param_1,'\x01');
      }
    }
    else {
      FUN_00409db9(*param_1);
    }
    break;
  case 0x86:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_0043299d(local_28,*param_1);
      }
    }
    else {
      FUN_00409efc(*param_1);
    }
    break;
  case 0x87:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_004329e4(local_28,*param_1);
      }
    }
    else {
      FUN_00409f8d(*param_1);
    }
    break;
  case 0x88:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432a63(local_28,*param_1);
      }
    }
    else {
      FUN_0040a00a(*param_1);
    }
    break;
  case 0x89:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432ad2(local_28,*param_1);
      }
    }
    else {
      FUN_0040a087(*param_1);
    }
    break;
  case 0x8a:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432b09(local_28,*param_1);
      }
    }
    else {
      FUN_0040a0c0(*param_1);
    }
    break;
  case 0x8b:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432c65(local_28,*param_1);
      }
    }
    else {
      FUN_0040a109(*param_1);
    }
    break;
  case 0x8c:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432c9c(local_28);
      }
    }
    else {
      FUN_0040a142(*param_1);
    }
    break;
  case 0x8d:
    if (param_2 == '\0') {
      uVar10 = FUN_004319b3(*param_1,0);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar10);
      uVar9 = FUN_00431a69(uVar10 & 0xffff);
      local_8 = (undefined2)uVar9;
    }
    FUN_00409513(*param_1);
    break;
  case 0x8e:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432d14(local_28);
      }
    }
    else {
      FUN_0040a16b(*param_1);
    }
    break;
  case 0x8f:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    sVar2 = FUN_00436652(*param_1,1);
    local_14 = CONCAT22(local_14._2_2_,sVar2);
    uVar10 = local_14;
    if (param_2 == '\0') {
      if (*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        local_14._0_1_ = (undefined1)sVar2;
        *(undefined1 *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 9) =
             (undefined1)local_14;
        local_14 = uVar10;
        FUN_00438cd0();
      }
    }
    break;
  case 0x90:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (sVar2 == 0) {
      FUN_00432303();
    }
    FUN_00432401();
    DAT_004859cd = 1;
    break;
  case 0x91:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        uVar10 = FUN_004319b3(*param_1,1);
        local_14 = CONCAT22(local_14._2_2_,(short)uVar10);
        FUN_004325d3(local_28 & 0xffff,uVar10 & 0xffff);
      }
    }
    else {
      FUN_00409e3a(*param_1);
    }
    break;
  case 0x95:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        uVar9 = FUN_004319b3(*param_1,1);
        local_14 = CONCAT22(local_14._2_2_,(short)uVar9);
        uVar10 = local_14;
        local_14._0_1_ = (undefined1)uVar9;
        *(undefined1 *)(*(int *)(&DAT_004649e8 + (local_28 & 0xffff) * 4) + 8) =
             (undefined1)local_14;
        local_14 = uVar10;
      }
    }
    else {
      FUN_00409e9b(*param_1);
    }
    break;
  case 0x96:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) < (local_28 & 0xffff))) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x97:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) <= (local_28 & 0xffff))) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x98:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_28 & 0xffff) < (local_14 & 0xffff))) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x99:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_28 & 0xffff) <= (local_14 & 0xffff))) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x9a:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    break;
  case 0x9b:
    if (param_2 == '\0') {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,0);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x9c:
    local_2c = (uint)local_2c._2_2_ << 0x10;
    while ((local_2c & 0xffff) < 8) {
      if (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02') {
        uVar3 = FUN_00436652(*param_1,(byte)local_2c);
        local_28 = CONCAT22(local_28._2_2_,uVar3);
        FUN_004102f7((uint)uVar3);
      }
      local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
    }
    break;
  case 0x9d:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) == (local_28 & 0xffff))) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x9e:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) != (local_28 & 0xffff))) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0x9f:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) < (local_28 & 0xffff))) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xa0:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_14 & 0xffff) <= (local_28 & 0xffff))) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xa1:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_28 & 0xffff) < (local_14 & 0xffff))) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xa2:
    if ((param_2 == '\0') &&
       (FUN_00431a36(*param_1,(undefined2 *)&local_28,(undefined2 *)&local_14),
       (local_28 & 0xffff) <= (local_14 & 0xffff))) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xa3:
    if (param_2 == '\0') {
      uVar9 = FUN_004319b3(*param_1,1);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
      uVar3 = FUN_00436652(*param_1,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2)
          < (uint)(0xffff / (ulonglong)(longlong)(int)(local_28 & 0xffff))) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        uVar3 = FUN_00436652(*param_1,0);
        *(short *)(&DAT_0047e2f0 +
                  (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2)
             = *(short *)(&DAT_0047e2f0 +
                         (uint)*(ushort *)
                                (&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) *
               (short)local_28;
      }
    }
    else {
      FUN_00409b5e(*param_1);
    }
    break;
  case 0xa4:
    if (param_2 == '\0') {
      uVar9 = FUN_004319b3(*param_1,1);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
      if ((short)uVar9 == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        uVar3 = FUN_00436652(*param_1,0);
        *(short *)(&DAT_0047e2f0 +
                  (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2)
             = (short)((ulonglong)
                       *(ushort *)
                        (&DAT_0047e2f0 +
                        (uint)*(ushort *)
                               (&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) /
                      (ulonglong)(longlong)(int)(local_28 & 0xffff));
      }
    }
    else {
      FUN_00409bde(*param_1);
    }
    break;
  case 0xa5:
    uVar9 = FUN_004319b3(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
    break;
  case 0xa6:
    uVar9 = FUN_004319b3(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
    break;
  case 0xa7:
    if (param_2 == '\0') {
      _DAT_00485c70 = 1;
    }
    break;
  case 0xa8:
    if (param_2 == '\0') {
      _DAT_00485c70 = 0;
    }
    break;
  case 0xa9:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432d3b();
      }
    }
    else {
      FUN_0040a194(*param_1,0xa9);
    }
    break;
  case 0xaa:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    if (DAT_00485c6c == '\0') {
      iVar8 = FUN_00431b0f(local_28,*(undefined1 *)*param_1);
      if (iVar8 == 0) {
        FUN_00432d46();
      }
    }
    else {
      FUN_0040a194(*param_1,0xaa);
    }
    break;
  case 0xab:
    if (param_2 == '\0') {
      uVar3 = FUN_00436652(*param_1,0);
      local_20._0_2_ = uVar3;
      uVar3 = FUN_00408bb2(uVar3);
      local_20 = CONCAT22(local_20._2_2_,uVar3);
      sVar2 = FUN_00436652(*param_1,1);
      local_38 = CONCAT22(local_38._2_2_,sVar2);
      uVar3 = FUN_00436652(*param_1,2);
      uVar10 = FUN_00408c81((uint)uVar3);
      local_40 = CONCAT22(local_40._2_2_,(short)uVar10);
      if ((local_38 & 0xffff) == 1) {
        FUN_004077b2(local_40,local_20);
      }
      else if (((local_38 & 0xffff) == 0) || ((local_38 & 0xffff) == 2)) {
        FUN_00407933(local_40,local_20);
      }
      **(undefined1 **)(&DAT_00486f90 + (local_20 & 0xffff) * 4) = (undefined1)local_38;
    }
    break;
  case 0xac:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    if ((*(uint *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) == (uint)DAT_00476338) ||
       (*(int *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) + 1U == (uint)DAT_00476338)) {
      uVar3 = FUN_00436652(*param_1,1);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xad:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    if ((*(uint *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) != (uint)DAT_00476338) &&
       (*(int *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) + 1U != (uint)DAT_00476338)) {
      uVar3 = FUN_00436652(*param_1,1);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xae:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    if ((*(uint *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) == (uint)DAT_00476338) ||
       (*(int *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) + 1U == (uint)DAT_00476338)) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,1);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xaf:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    if ((*(uint *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) != (uint)DAT_00476338) &&
       (*(int *)(*(int *)(&DAT_00486f90 + (uint)uVar3 * 4) + 4) + 1U != (uint)DAT_00476338)) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,1);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xb0:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    uVar3 = FUN_00436652(*param_1,1);
    local_38 = CONCAT22(local_38._2_2_,uVar3);
    if (**(byte **)(&DAT_00486f90 + (local_20 & 0xffff) * 4) == uVar3) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xb1:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    uVar3 = FUN_00436652(*param_1,1);
    local_38 = CONCAT22(local_38._2_2_,uVar3);
    if (**(byte **)(&DAT_00486f90 + (local_20 & 0xffff) * 4) != uVar3) {
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xb2:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    uVar3 = FUN_00436652(*param_1,1);
    local_38 = CONCAT22(local_38._2_2_,uVar3);
    if (**(byte **)(&DAT_00486f90 + (local_20 & 0xffff) * 4) == uVar3) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xb3:
    uVar3 = FUN_00436652(*param_1,0);
    local_20._0_2_ = uVar3;
    uVar3 = FUN_00408bb2(uVar3);
    local_20 = CONCAT22(local_20._2_2_,uVar3);
    uVar3 = FUN_00436652(*param_1,1);
    local_38 = CONCAT22(local_38._2_2_,uVar3);
    if (**(byte **)(&DAT_00486f90 + (local_20 & 0xffff) * 4) != uVar3) {
      FUN_0043194f(*param_1);
      uVar3 = FUN_00436652(*param_1,2);
      *param_1 = *(int *)(&DAT_004654e0 + (uint)uVar3 * 4);
      local_8 = 2;
    }
    break;
  case 0xb4:
    DAT_0045d1c0 = 0;
    FUN_004075a1(1);
    break;
  case 0xb5:
    DAT_0045d1c0 = 1;
    FUN_00407420(1);
    break;
  case 0xb6:
    if (param_2 == '\0') {
      uVar3 = FUN_00436652(*param_1,0);
      local_20._0_2_ = uVar3;
      uVar3 = FUN_00408bb2(uVar3);
      local_20 = CONCAT22(local_20._2_2_,uVar3);
      uVar3 = FUN_00436652(*param_1,1);
      uVar10 = FUN_00408c81((uint)uVar3);
      local_40 = CONCAT22(local_40._2_2_,(short)uVar10);
      FUN_004077b2(local_40,local_20);
      FUN_00407ad5(*(undefined1 **)(&DAT_00486f90 + (local_20 & 0xffff) * 4));
    }
    break;
  case 0xb7:
    FUN_00438d78();
    break;
  case 0xb8:
    FUN_00438cf9();
    break;
  case 0xb9:
    uVar10 = local_24 >> 0x10;
    local_24 = local_24 & 0xffff0000;
    if (*(char *)(*param_1 + 4) == '\x02') {
      iVar8 = 1;
      uVar3 = FUN_00436652(*param_1,3);
      uVar9 = FUN_00432133((uint)uVar3,iVar8);
      local_24 = CONCAT22(local_24._2_2_,(short)uVar9);
    }
    else if (DAT_00460664 != -1) {
      local_24 = CONCAT22((short)uVar10,(short)DAT_00460664);
      DAT_00460664 = -1;
    }
    uVar9 = FUN_004319b3(*param_1,1);
    local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
    if ((short)uVar9 != 0) {
      uVar3 = FUN_00436652(*param_1,1);
      *(undefined2 *)
       (&DAT_0047e2f0 +
       (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) = 0;
    }
    uVar10 = FUN_004319b3(*param_1,2);
    local_14 = CONCAT22(local_14._2_2_,(short)uVar10);
    if (DAT_00485c6c == '\0') {
      uVar7 = local_24 & 0xffff;
      uVar10 = uVar10 & 0xffff;
      uVar6 = local_28 & 0xffff;
      uVar3 = FUN_00436652(*param_1,0);
      FUN_00432295((uint)uVar3,uVar6,uVar10,uVar7);
    }
    else {
      FUN_0040a2c3(*param_1,&DAT_00483d90,&DAT_00460cc8);
    }
    DAT_0045c060 = 1;
    DAT_0048253e = 0xffff;
    FUN_0040d862();
    FUN_004177c7((char *)0x0);
    FUN_0041dcc9();
    break;
  case 0xba:
    uVar3 = FUN_00436652(*param_1,0);
    FUN_00401c21((uint)uVar3);
    break;
  case 0xbb:
    uVar3 = FUN_00436652(*param_1,0);
    FUN_004073f0((uint)uVar3);
    break;
  case 0xbc:
    local_2c = (uint)local_2c._2_2_ << 0x10;
    while ((local_2c & 0xffff) < 8) {
      if (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02') {
        uVar3 = *(ushort *)(*param_1 + 10 + (local_2c & 0xffff) * 4);
        local_28 = CONCAT22(local_28._2_2_,uVar3);
        if (*(int *)(*(int *)(&DAT_00486220 + (uint)uVar3 * 4) + 0x2c) == 0) {
          FUN_004281b1((uint)uVar3);
        }
        else {
          FUN_004277ed((uint)uVar3);
        }
        *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x34) = 0;
      }
      local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
    }
    break;
  case 0xbd:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    local_24 = local_24 & 0xffff0000;
    local_14 = local_14 & 0xffff0000;
    if (*(char *)(*param_1 + 3) == '@') {
      sVar2 = FUN_00436652(*param_1,2);
      local_14 = CONCAT22(local_14._2_2_,sVar2);
      if (*(char *)(*param_1 + 4) == '@') {
        sVar2 = FUN_00436652(*param_1,3);
        local_24 = CONCAT22(local_24._2_2_,sVar2);
      }
    }
    uVar10 = local_24 & 0xffff;
    uVar7 = local_14 & 0xffff;
    uVar3 = FUN_00436652(*param_1,1);
    FUN_0040d031((uint *)**(undefined4 **)(&DAT_00484330 + (local_28 & 0xffff) * 4),(uint)uVar3,
                 uVar7,uVar10);
    break;
  case 0xbe:
  case 0xd2:
    if ((DAT_00460670 != -1) && (iVar8 = FUN_0041d440(0), iVar8 == 1)) {
      FUN_0042515f((uint)*(byte *)*param_1);
    }
    break;
  case 0xbf:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar10 = FUN_004319b3(*param_1,1);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x28) = uVar10 & 0xffff;
    uVar10 = FUN_004319b3(*param_1,2);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x2c) = uVar10 & 0xffff;
    if (*(char *)(*param_1 + 4) == '\x02') {
      uVar10 = FUN_004319b3(*param_1,3);
      *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x1c) = uVar10 & 0xffff;
    }
    break;
  case 0xc0:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar10 = FUN_004319b3(*param_1,1);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x1c) = uVar10 & 0xffff;
    if (*(char *)(*param_1 + 3) == '\x02') {
      uVar10 = FUN_004319b3(*param_1,2);
      *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x20) = uVar10 & 0xffff;
      if (*(char *)(*param_1 + 4) == '\x02') {
        uVar10 = FUN_004319b3(*param_1,3);
        *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x24) = uVar10 & 0xffff;
      }
    }
    FUN_00427d1a(*(int **)(&DAT_00486220 + (local_28 & 0xffff) * 4));
    break;
  case 0xc1:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar10 = FUN_004319b3(*param_1,1);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x30) = uVar10 & 0xffff;
    if (*(int *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x30) == 3000) {
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x30) = 0xffffffff;
    }
    break;
  case 0xc2:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar10 = FUN_004319b3(*param_1,1);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x20) = uVar10 & 0xffff;
    uVar10 = FUN_004319b3(*param_1,2);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x24) = uVar10 & 0xffff;
    if (*(char *)(*param_1 + 4) == '\x02') {
      uVar10 = FUN_004319b3(*param_1,3);
      *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x1c) = uVar10 & 0xffff;
    }
    FUN_00427d1a(*(int **)(&DAT_00486220 + (local_28 & 0xffff) * 4));
    break;
  case 0xc3:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar3 = FUN_00436652(*param_1,1);
    local_14 = CONCAT22(local_14._2_2_,uVar3);
    local_10 = FUN_004111c7((uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar3 * 4));
    *(int **)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x38) = local_10;
    break;
  case 0xc4:
    local_2c = (uint)local_2c._2_2_ << 0x10;
    while ((local_2c & 0xffff) < 8) {
      if (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02') {
        uVar3 = *(ushort *)(*param_1 + 10 + (local_2c & 0xffff) * 4);
        local_28 = CONCAT22(local_28._2_2_,uVar3);
        FUN_00427de1((uint)uVar3,1);
        if (*(int *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x14) != 0) {
          *(undefined4 *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x34) = 1;
        }
        iVar8 = __strcmpi((char *)**(undefined4 **)(&DAT_0047a8e0 + (local_28 & 0xffff) * 4),
                          s_endgame_00460ce8);
        if (iVar8 == 0) {
          DAT_0046066c = 1;
        }
      }
      local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
    }
    break;
  case 0xc5:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar10 = FUN_004319b3(*param_1,1);
    *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x10) = uVar10 & 0xffff;
    FUN_00427d1a(*(int **)(&DAT_00486220 + (local_28 & 0xffff) * 4));
    break;
  case 0xc6:
    uVar3 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,uVar3);
    if (*(int *)(&DAT_00486a10 + (uint)uVar3 * 4) == 0) {
      uVar9 = FUN_0044c2f0(0x68);
      *(undefined4 *)(&DAT_00486a10 + (local_28 & 0xffff) * 4) = uVar9;
      local_3c = *(int *)(&DAT_00486a10 + (local_28 & 0xffff) * 4);
      *(undefined4 *)(local_3c + 100) = 0;
      local_2c = (uint)local_2c._2_2_ << 0x10;
      while ((local_2c & 0xffff) < 0x19) {
        *(undefined4 *)(local_3c + (local_2c & 0xffff) * 4) = 0xffffffff;
        local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
      }
    }
    else {
      local_3c = *(int *)(&DAT_00486a10 + (uint)uVar3 * 4);
    }
    local_2c = CONCAT22(local_2c._2_2_,1);
    while ((local_2c & 0xffff) < 8) {
      if (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02') {
        uVar3 = FUN_00436652(*param_1,(byte)local_2c);
        local_20 = CONCAT22(local_20._2_2_,uVar3);
        uVar3 = FUN_00408bb2(uVar3);
        *(uint *)(local_3c + *(int *)(local_3c + 100) * 4) = CONCAT22(extraout_var,uVar3);
        *(int *)(local_3c + 100) = *(int *)(local_3c + 100) + 1;
      }
      local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
    }
    break;
  case 199:
    uVar3 = FUN_00436652(*param_1,0);
    local_3c = *(int *)(&DAT_00486a10 + (uint)uVar3 * 4);
    local_2c = CONCAT22(local_2c._2_2_,1);
    while (((local_2c & 0xffff) < 8 && (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02'))) {
      uVar3 = FUN_00436652(*param_1,(byte)local_2c);
      local_20._0_2_ = uVar3;
      uVar3 = FUN_00408bb2(uVar3);
      local_20 = CONCAT22(local_20._2_2_,uVar3);
      local_30 = local_30 & 0xffff0000;
      while ((int)(local_30 & 0xffff) < *(int *)(local_3c + 100)) {
        if (*(uint *)(local_3c + (local_30 & 0xffff) * 4) == (uint)uVar3) {
          *(undefined4 *)(local_3c + (local_30 & 0xffff) * 4) = 0xffffffff;
          *(int *)(local_3c + 100) = *(int *)(local_3c + 100) + -1;
          local_34 = CONCAT22(local_34._2_2_,(short)local_30);
          while ((int)(local_34 & 0xffff) < *(int *)(local_3c + 100)) {
            *(undefined4 *)(local_3c + (local_34 & 0xffff) * 4) =
                 *(undefined4 *)(local_3c + 4 + (local_34 & 0xffff) * 4);
            local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
          }
        }
        local_30 = CONCAT22(local_30._2_2_,(short)local_30 + 1);
      }
      local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
    }
    break;
  case 200:
    uVar10 = FUN_004319b3(*param_1,0);
    sVar2 = (short)uVar10;
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    switch(uVar10 & 0xffff) {
    case 0x1e:
      DAT_0045d1d0 = 0;
      FUN_0042a6aa(sVar2,(undefined2 *)0x0,(undefined2 *)0x0);
      break;
    case 0x1f:
      local_44[0] = 0;
      uVar9 = FUN_004319b3(*param_1,1);
      local_14 = CONCAT22(local_14._2_2_,(short)uVar9);
      if (((*(char *)(*param_1 + 3) == '\x02') && (*(char *)(*param_1 + 4) == '\x02')) &&
         (*(char *)(*param_1 + 5) == '\x02')) {
        uVar10 = FUN_0042a6aa((short)uVar9,(undefined2 *)&local_24,local_44);
        local_14 = CONCAT22(local_14._2_2_,(short)uVar10);
        uVar3 = FUN_00436652(*param_1,2);
        *(undefined2 *)
         (&DAT_0047e2f0 +
         (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) =
             (undefined2)local_24;
        uVar3 = FUN_00436652(*param_1,3);
        *(undefined2 *)
         (&DAT_0047e2f0 +
         (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) =
             (undefined2)local_14;
        uVar3 = FUN_00436652(*param_1,4);
        *(undefined2 *)
         (&DAT_0047e2f0 +
         (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar3 * 2 + (uint)DAT_00482428 * 600) * 2) =
             local_44[0];
      }
      break;
    case 0x20:
      FUN_0042a6aa(sVar2,(undefined2 *)0x0,(undefined2 *)0x0);
      DAT_0045d1d0 = 1;
      break;
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x34:
    case 0x35:
      break;
    case 0x32:
      DAT_00460668 = 1;
      break;
    case 0x33:
      DAT_00460668 = 2;
      break;
    case 0x3a:
      DAT_0045f490 = 2;
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
      FUN_00427389(uVar10 & 0xffff);
    }
    break;
  case 0xcb:
    local_28 = CONCAT22(local_28._2_2_,10);
    if (*(char *)(*param_1 + 2) == '@') {
      uVar9 = FUN_004319b3(*param_1,1);
      local_28 = CONCAT22(local_28._2_2_,(short)uVar9);
    }
    uVar10 = local_28 & 0xffff;
    uVar7 = FUN_004319b3(*param_1,0);
    FUN_00402285(uVar7 & 0xffff,uVar10);
    break;
  case 0xcc:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar3 = FUN_00436652(*param_1,1);
    *(uint *)(*(int *)(&DAT_00464300 + (local_28 & 0xffff) * 4) + 0x14) = (uint)uVar3;
    break;
  case 0xcd:
    if (DAT_00464878 == 0) {
      sVar2 = FUN_00436652(*param_1,0);
      local_28 = CONCAT22(local_28._2_2_,sVar2);
      uVar3 = FUN_00436652(*param_1,1);
      local_14 = CONCAT22(local_14._2_2_,uVar3);
      FUN_00417a17(*(uint **)(&DAT_00483640 + (local_28 & 0xffff) * 4),(uint)uVar3);
    }
    break;
  case 0xce:
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar9 = FUN_004319b3(*param_1,1);
    local_14 = CONCAT22(local_14._2_2_,(short)uVar9);
    if ((short)uVar9 == 0) {
      local_24 = CONCAT22(local_24._2_2_,1);
    }
    else {
      local_24 = (uint)local_24._2_2_ << 0x10;
    }
    if (*(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x14) != (local_24 & 0xffff))
    {
      *(uint *)(*(int *)(&DAT_00486220 + (local_28 & 0xffff) * 4) + 0x14) = local_24 & 0xffff;
      FUN_00427d49(*(int **)(&DAT_00486220 + (local_28 & 0xffff) * 4));
    }
    break;
  case 0xcf:
    local_2c = (uint)local_2c._2_2_ << 0x10;
    while ((local_2c & 0xffff) < 8) {
      if (*(char *)((local_2c & 0xffff) + 1 + *param_1) == '\x02') {
        uVar3 = *(ushort *)(*param_1 + 10 + (local_2c & 0xffff) * 4);
        local_28 = CONCAT22(local_28._2_2_,uVar3);
        FUN_00427a01((uint)uVar3);
      }
      local_2c = CONCAT22(local_2c._2_2_,(short)local_2c + 1);
    }
    break;
  case 0xd0:
    FUN_004278ab();
    break;
  case 0xd1:
    local_48 = 0xffffffff;
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    sVar2 = FUN_00436652(*param_1,1);
    local_14 = CONCAT22(local_14._2_2_,sVar2);
    if (*(char *)(*param_1 + 3) == '\x02') {
      uVar3 = FUN_00436652(*param_1,2);
      local_48 = (uint)uVar3;
    }
    uVar10 = FUN_00428cbd(local_28 & 0xffff,local_14 & 0xffff,local_48,0,-1,0,0xffffffff,0,
                          0xffffffff);
    local_8 = (undefined2)uVar10;
    break;
  case 0xd3:
    local_5c = 0;
    local_58 = 0;
    local_50 = 0xffffffff;
    local_64 = 0xffffffff;
    sVar2 = FUN_00436652(*param_1,0);
    local_28 = CONCAT22(local_28._2_2_,sVar2);
    uVar10 = FUN_004319b3(*param_1,1);
    uVar3 = FUN_00436652(*param_1,2);
    if (*(char *)(*param_1 + 4) == '@') {
      local_58 = FUN_004319b3(*param_1,3);
      local_58 = local_58 & 0xffff;
      uVar4 = FUN_00436652(*param_1,4);
      local_64 = (uint)uVar4;
    }
    if (*(char *)(*param_1 + 6) == '@') {
      local_5c = FUN_004319b3(*param_1,5);
      local_5c = local_5c & 0xffff;
      uVar4 = FUN_00436652(*param_1,6);
      local_50 = (uint)uVar4;
    }
    uVar10 = FUN_00428cbd(local_28 & 0xffff,0,-1,uVar10 & 0xffff,(uint)uVar3,local_58,local_64,
                          local_5c,local_50);
    local_8 = (undefined2)uVar10;
    break;
  case 0xd4:
    DAT_004859cd = 1;
    break;
  case 0xd5:
    DAT_004859cd = 0;
  }
  uVar1 = 0;
  if (param_2 != '\0') {
    FUN_00437c8c();
    uVar1 = extraout_var_01;
  }
  return CONCAT22(uVar1,local_8);
}

