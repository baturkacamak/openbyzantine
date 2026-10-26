// rea decompile_all
// name: FUN_0043340a
// address: 0x43340a
// body: available
// range: 0x43340a-0x43626c inclusive
// range: 0x436272-0x436294 inclusive
// range: 0x4364bc-0x4364ee inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0043340a(int *param_1,byte *param_2)

{
  bool bVar1;
  ushort uVar2;
  short sVar3;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined3 extraout_var_01;
  undefined3 extraout_var_02;
  undefined3 extraout_var_03;
  undefined3 extraout_var_04;
  undefined3 extraout_var_05;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  undefined4 uVar7;
  DWORD DVar8;
  int iVar9;
  int local_4c [2];
  uint local_44;
  char *local_40 [2];
  uint local_38;
  undefined4 local_34;
  uint local_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined4 local_c;
  int local_8;
  
  local_40[0] = (char *)0x0;
  local_4c[0] = 0;
  local_8 = 0;
  while ((param_2 != (byte *)0x0 && (DAT_0047a8d8 == '\0'))) {
    uVar2 = local_34._2_2_;
    switch(*param_2) {
    case 0x32:
      *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 10);
      *(undefined1 *)((int)param_1 + 10) = 0;
      DAT_0047a8d8 = '\x01';
      FUN_0040a719(&DAT_004610d8,(int)param_2);
      break;
    case 0x33:
      *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 10);
      *(undefined1 *)((int)param_1 + 10) = 1;
      DAT_0047a8d8 = '\x01';
      FUN_0040a719(s_REPLY_004610e0,(int)param_2);
      break;
    case 0x34:
      uVar6 = FUN_0043669a((int)param_1,(int)param_2,local_40);
      if ((uVar6 & 0xffff) == 0) {
        if (local_40[0] == (char *)0x0) {
          FUN_00436721(param_1 + *(ushort *)(param_2 + 0xe) + 0x195,(int *)local_40);
        }
        *local_40[0] = '\0';
        *(int *)(local_40[0] + 8) = param_1[*(ushort *)(param_2 + 10) + 0x163];
        local_40[0][0xc] = param_2[10];
        local_40[0] = (char *)0x0;
      }
      FUN_0040a6e2(&DAT_004610e8,(int)param_2);
      break;
    case 0x35:
      uVar6 = FUN_0043669a((int)param_1,(int)param_2,local_40);
      if ((uVar6 & 0xffff) == 0) {
        if (local_40[0] == (char *)0x0) {
          FUN_00436721(param_1 + *(ushort *)(param_2 + 0xe) + 0x195,(int *)local_40);
        }
        *local_40[0] = '\x01';
        *(int *)(local_40[0] + 8) = param_1[*(ushort *)(param_2 + 10) + 0x163];
        local_40[0][0xc] = param_2[10];
        local_40[0] = (char *)0x0;
      }
      FUN_0040a6e2(s_THOOK_004610f0,(int)param_2);
      break;
    case 0x36:
      FUN_004367eb((int)param_1,(int)param_2,local_4c,0);
      *(undefined1 *)(local_4c[0] + 0x10) = 1;
      FUN_004365b5(local_4c[0],1);
      FUN_0040a760(&DAT_004610f8,(int)param_2,'\0');
      break;
    case 0x37:
      FUN_004367eb((int)param_1,(int)param_2,local_4c,0);
      *(undefined1 *)(local_4c[0] + 0x10) = 0;
      FUN_004365b5(local_4c[0],0);
      FUN_0040a760(&DAT_004610fc,(int)param_2,'\0');
      break;
    case 0x38:
      FUN_004367eb((int)param_1,(int)param_2,local_4c,0);
      param_1[*(ushort *)(param_2 + 10) + 0xf2] = local_4c[0];
      *(byte *)(*(ushort *)(param_2 + 10) + 0x490 + (int)param_1) = param_2[0xc];
      FUN_0040a760(&DAT_0046110c,(int)param_2,'\x01');
      break;
    case 0x39:
      *(undefined1 *)((int)param_1 + 0xe) = 1;
      break;
    case 0x3b:
      *(byte *)((int)param_1 + 0xb) = param_2[10];
      FUN_0040a719(&DAT_00461110,(int)param_2);
      break;
    case 0x3c:
      *(byte *)(*(ushort *)(param_2 + 10) + 0xf + (int)param_1) = param_2[0xe];
      FUN_0040a6e2(&DAT_00461138,(int)param_2);
      break;
    case 0x3d:
      *(undefined2 *)(&DAT_0046e830 + (uint)DAT_0048430c * 2) = *(undefined2 *)(param_2 + 10);
      DAT_0048430c = DAT_0048430c + 1;
      break;
    case 0x3e:
      if (DAT_00485c6c != '\0') {
        uVar2 = FUN_00436652((int)param_2,0);
        FUN_0040a7e5(&DAT_00461120,param_1,(uint)uVar2);
      }
      break;
    case 0x3f:
      FUN_004367eb((int)param_1,(int)param_2,local_4c,0);
      *(undefined1 *)(local_4c[0] + 0x11) = 0;
      FUN_0040a760(s_UNSPEAK_00461128,(int)param_2,'\0');
      break;
    case 0x40:
      *(byte *)((int)param_1 + 0xd) = param_2[10];
      FUN_0040a719(&DAT_00461118,(int)param_2);
      break;
    case 0x41:
      if (*(short *)(param_2 + 0x12) == 2) {
        *(byte *)(*(ushort *)(param_2 + 10) + 0x41 + (int)param_1) = param_2[0xe];
      }
      else {
        *(byte *)(*(ushort *)(param_2 + 10) + 0x73 + (int)param_1) = param_2[0xe];
      }
      break;
    case 0x42:
      sVar3 = FUN_00436652((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          == (local_30 & 0xffff)) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        *(undefined1 *)(local_4c[0] + 0x10) = 1;
        FUN_004365b5(local_4c[0],1);
      }
      FUN_0040a615(&DAT_00461158,(int)param_2,'\0','\0');
      break;
    case 0x43:
      sVar3 = FUN_00436652((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          == (local_30 & 0xffff)) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        *(undefined1 *)(local_4c[0] + 0x10) = 0;
        FUN_004365b5(local_4c[0],0);
      }
      FUN_0040a615(s_IFOFF_00461150,(int)param_2,'\0','\0');
      break;
    case 0x44:
      sVar3 = FUN_00436652((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          == (local_30 & 0xffff)) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        param_1[*(ushort *)(param_2 + 0x12) + 0xf2] = local_4c[0];
        *(byte *)(*(ushort *)(param_2 + 0x12) + 0x490 + (int)param_1) = param_2[0x14];
      }
      FUN_0040a615(s_IFSET_00461170,(int)param_2,'\x01','\0');
      break;
    case 0x45:
      if (param_1[*(ushort *)(param_2 + 10) + 0x2a] == 0) {
        iVar9 = FUN_0044c2f0(8);
        param_1[*(ushort *)(param_2 + 10) + 0x2a] = iVar9;
      }
      *(byte *)param_1[*(ushort *)(param_2 + 10) + 0x2a] = param_2[0xe];
      FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
      *(int *)(param_1[*(ushort *)(param_2 + 10) + 0x2a] + 4) = local_4c[0];
      break;
    case 0x46:
      if (*(short *)(param_2 + 0x16) == 2) {
        if (param_1[*(ushort *)(param_2 + 10) + 0x5c] == 0) {
          iVar9 = FUN_0044c2f0(8);
          param_1[*(ushort *)(param_2 + 10) + 0x5c] = iVar9;
        }
        *(byte *)param_1[*(ushort *)(param_2 + 10) + 0x5c] = param_2[0xe];
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        *(int *)(param_1[*(ushort *)(param_2 + 10) + 0x5c] + 4) = local_4c[0];
      }
      else {
        if (param_1[*(ushort *)(param_2 + 10) + 0x8e] == 0) {
          iVar9 = FUN_0044c2f0(8);
          param_1[*(ushort *)(param_2 + 10) + 0x8e] = iVar9;
        }
        *(byte *)param_1[*(ushort *)(param_2 + 10) + 0x8e] = param_2[0xe];
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        *(int *)(param_1[*(ushort *)(param_2 + 10) + 0x8e] + 4) = local_4c[0];
      }
      break;
    case 0x47:
      if (param_2[1] == 2) {
        uVar2 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,
                            *(undefined2 *)
                             (&DAT_0047e2f0 +
                             (uint)*(ushort *)
                                    (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2
                             ));
      }
      else {
        sVar3 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,sVar3);
      }
      if ((local_30 & 0xffff) < (uint)DAT_0048430c) {
        FUN_00424780((int)param_1,(uint)*(ushort *)(&DAT_0046e830 + (local_30 & 0xffff) * 2));
      }
      break;
    case 0x48:
      sVar3 = FUN_00436652((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      FUN_0040a615(s_IFREPLY_00461148,(int)param_2,'\0','\x01');
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          == (local_30 & 0xffff)) {
        *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 0x12);
        *(undefined1 *)((int)param_1 + 10) = 1;
        DAT_0047a8d8 = '\x01';
      }
      break;
    case 0x49:
      sVar3 = FUN_00436652((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      FUN_0040a615(s_IFGOTO_00461140,(int)param_2,'\x01','\x01');
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          == (local_30 & 0xffff)) {
        *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 0x12);
        *(undefined1 *)((int)param_1 + 10) = 0;
        DAT_0047a8d8 = '\x01';
      }
      break;
    case 0x4a:
      FUN_004367eb((int)param_1,(int)param_2,local_4c,0);
      FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 10),
                   (char)*(undefined2 *)(param_2 + 0xc));
      *(undefined1 *)(local_4c[0] + 0x10) = 0;
      FUN_0040a760(&DAT_00461104,(int)param_2,'\0');
      break;
    case 0x4b:
      FUN_004367eb((int)param_1,(int)param_2,local_4c,0);
      FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 10),
                   (char)*(undefined2 *)(param_2 + 0xc));
      *(undefined1 *)(local_4c[0] + 0x10) = 1;
      FUN_0040a760(&DAT_00461100,(int)param_2,'\0');
      break;
    case 0x4c:
      sVar3 = FUN_00436652((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          == (local_30 & 0xffff)) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 0x12),
                     (char)*(undefined2 *)(param_2 + 0x14));
        *(undefined1 *)(local_4c[0] + 0x10) = 0;
      }
      FUN_0040a615(s_IFTOFF_00461160,(int)param_2,'\0','\0');
      break;
    case 0x4d:
      sVar3 = FUN_00436652((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          == (local_30 & 0xffff)) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 0x12),
                     (char)*(undefined2 *)(param_2 + 0x14));
        *(undefined1 *)(local_4c[0] + 0x10) = 1;
      }
      FUN_0040a615(s_IFTON_00461168,(int)param_2,'\0','\0');
      break;
    case 0x4e:
      uVar2 = *(ushort *)(param_2 + 10);
      local_30 = CONCAT22(local_30._2_2_,uVar2);
      local_4c[0] = param_1[uVar2 + 0x163];
      local_24 = local_24 & 0xffff0000;
      for (; local_4c[0] != 0; local_4c[0] = *(int *)(local_4c[0] + 8)) {
        if ((*(char *)(local_4c[0] + 0x10) != '\0') && (*(char *)(local_4c[0] + 0x11) == '\0')) {
          local_24 = CONCAT22(local_24._2_2_,(short)local_24 + 1);
        }
        for (local_40[0] = (char *)param_1[uVar2 + 0x195]; local_40[0] != (char *)0x0;
            local_40[0] = *(char **)(local_40[0] + 4)) {
          if (*local_40[0] != -1) {
            for (local_8 = *(int *)(local_40[0] + 8); local_8 != 0; local_8 = *(int *)(local_8 + 8))
            {
              if ((*(char *)(local_8 + 0x10) != '\0') && (*(char *)(local_8 + 0x11) == '\0')) {
                local_24 = CONCAT22(local_24._2_2_,(short)local_24 + 1);
              }
            }
          }
        }
      }
      uVar2 = FUN_00436652((int)param_2,1);
      *(short *)(&DAT_0047e2f0 +
                (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) =
           (short)local_24;
      break;
    case 0x4f:
      FUN_004367eb((int)param_1,(int)param_2,local_4c,0);
      *(undefined1 *)(local_4c[0] + 0x11) = 1;
      FUN_0040a760(s_SPEAK_00461130,(int)param_2,'\0');
      break;
    case 0x52:
      bVar1 = FUN_00433309((int)param_2);
      if (CONCAT31(extraout_var,bVar1) != 0) {
        *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 0x12);
        *(undefined1 *)((int)param_1 + 10) = 0;
        DAT_0047a8d8 = '\x01';
        FUN_0040a719(s_INVGOTO_00461178,(int)param_2);
      }
      break;
    case 0x53:
      bVar1 = FUN_00433309((int)param_2);
      if (CONCAT31(extraout_var_00,bVar1) != 0) {
        *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 0x12);
        *(undefined1 *)((int)param_1 + 10) = 1;
        DAT_0047a8d8 = '\x01';
        FUN_0040a719(s_INVREPLY_00461180,(int)param_2);
      }
      break;
    case 0x54:
      bVar1 = FUN_00433309((int)param_2);
      if (CONCAT31(extraout_var_01,bVar1) != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        param_1[*(ushort *)(param_2 + 0x12) + 0xf2] = local_4c[0];
        *(byte *)(*(ushort *)(param_2 + 0x12) + 0x490 + (int)param_1) = param_2[0x14];
        FUN_0040a760(s_INVSET_0046118c,(int)param_2,'\x01');
      }
      break;
    case 0x55:
      bVar1 = FUN_00433309((int)param_2);
      if (CONCAT31(extraout_var_02,bVar1) != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        *(undefined1 *)(local_4c[0] + 0x10) = 0;
        FUN_004365b5(local_4c[0],0);
        FUN_0040a760(s_INVOFF_00461194,(int)param_2,'\0');
      }
      break;
    case 0x56:
      bVar1 = FUN_00433309((int)param_2);
      if (CONCAT31(extraout_var_03,bVar1) != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        *(undefined1 *)(local_4c[0] + 0x10) = 1;
        FUN_004365b5(local_4c[0],1);
        FUN_0040a760(s_INVON_0046119c,(int)param_2,'\0');
      }
      break;
    case 0x57:
      bVar1 = FUN_00433309((int)param_2);
      if (CONCAT31(extraout_var_04,bVar1) != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 0x12),
                     (char)*(undefined2 *)(param_2 + 0x14));
        *(undefined1 *)(local_4c[0] + 0x10) = 0;
        FUN_0040a760(s_INVTOFF_004611a4,(int)param_2,'\0');
      }
      break;
    case 0x58:
      bVar1 = FUN_00433309((int)param_2);
      if (CONCAT31(extraout_var_05,bVar1) != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,2);
        FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 0x12),
                     (char)*(undefined2 *)(param_2 + 0x14));
        *(undefined1 *)(local_4c[0] + 0x10) = 1;
        FUN_0040a760(s_INVTON_004611ac,(int)param_2,'\0');
      }
      break;
    case 0x59:
      iVar9 = FUN_00433384((int)param_2);
      if (iVar9 != 0) {
        *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 0xe);
        *(undefined1 *)((int)param_1 + 10) = 0;
        DAT_0047a8d8 = '\x01';
        FUN_0040a719(s_CURSGOTO_004611b4,(int)param_2);
      }
      break;
    case 0x5a:
      iVar9 = FUN_00433384((int)param_2);
      if (iVar9 != 0) {
        *(undefined2 *)(param_1 + 2) = *(undefined2 *)(param_2 + 0xe);
        *(undefined1 *)((int)param_1 + 10) = 1;
        DAT_0047a8d8 = '\x01';
        FUN_0040a719(s_CURSREPL_004611c0,(int)param_2);
      }
      break;
    case 0x5b:
      iVar9 = FUN_00433384((int)param_2);
      if (iVar9 != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,1);
        param_1[*(ushort *)(param_2 + 0xe) + 0xf2] = local_4c[0];
        *(byte *)(*(ushort *)(param_2 + 0xe) + 0x490 + (int)param_1) = param_2[0x10];
        FUN_0040a760(s_CURSSET_004611cc,(int)param_2,'\x01');
      }
      break;
    case 0x5c:
      iVar9 = FUN_00433384((int)param_2);
      if (iVar9 != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,1);
        *(undefined1 *)(local_4c[0] + 0x10) = 0;
        FUN_004365b5(local_4c[0],0);
        FUN_0040a760(s_CURSOFF_004611d4,(int)param_2,'\0');
      }
      break;
    case 0x5d:
      iVar9 = FUN_00433384((int)param_2);
      if (iVar9 != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,1);
        *(undefined1 *)(local_4c[0] + 0x10) = 1;
        FUN_004365b5(local_4c[0],1);
        FUN_0040a760(s_CURSON_004611dc,(int)param_2,'\0');
      }
      break;
    case 0x5e:
      iVar9 = FUN_00433384((int)param_2);
      if (iVar9 != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,1);
        FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 0xe),
                     (char)*(undefined2 *)(param_2 + 0x10));
        *(undefined1 *)(local_4c[0] + 0x10) = 0;
        FUN_0040a760(s_CURSTOFF_004611e4,(int)param_2,'\0');
      }
      break;
    case 0x5f:
      iVar9 = FUN_00433384((int)param_2);
      if (iVar9 != 0) {
        FUN_004367eb((int)param_1,(int)param_2,local_4c,1);
        FUN_004364ef(local_4c[0],*param_2,(char)*(undefined2 *)(param_2 + 0xe),
                     (char)*(undefined2 *)(param_2 + 0x10));
        *(undefined1 *)(local_4c[0] + 0x10) = 1;
        FUN_0040a760(s_CURSTON_004611f0,(int)param_2,'\0');
      }
      break;
    case 100:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_004324b0(local_30,(int)param_2);
      }
      break;
    case 0x65:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar7 = FUN_0044c2f0(0x3c);
      *(undefined4 *)(&DAT_00486220 + (local_30 & 0xffff) * 4) = uVar7;
      **(uint **)(&DAT_00486220 + (local_30 & 0xffff) * 4) = local_30 & 0xffff;
      uVar7 = FUN_004319b3((int)param_2,1);
      local_24 = CONCAT22(local_24._2_2_,(short)uVar7);
      if ((short)uVar7 == 0) {
        *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x14) = 1;
      }
      else {
        *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x14) = 0;
      }
      uVar6 = FUN_004319b3((int)param_2,2);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x10) = uVar6 & 0xffff;
      uVar2 = FUN_00436652((int)param_2,3);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0xc) = (uint)uVar2;
      uVar2 = FUN_00436652((int)param_2,4);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 8) = (uint)uVar2;
      uVar2 = FUN_00436652((int)param_2,5);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x18) = (uint)uVar2;
      if (param_2[7] == 2) {
        *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 4) =
             (uint)*(ushort *)(param_2 + 0x22);
      }
      else {
        *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 4) = 1;
      }
      if (((*(int *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x18) == 0) &&
          (iVar9 = FUN_0041d440(2), iVar9 == 0)) &&
         (*(int *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 4) == 0)) {
        *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 4) = 1;
      }
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x1c) = 10000;
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x20) = 0x32;
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x24) = 0x32;
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x28) = 0;
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x2c) = 0;
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x30) = 0xffffffff;
      *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x38) = 0;
      break;
    case 0x66:
      break;
    case 0x67:
      if (param_2[1] == 2) {
        uVar2 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,
                            *(undefined2 *)
                             (&DAT_0047e2f0 +
                             (uint)*(ushort *)
                                    (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2
                             ));
      }
      else {
        sVar3 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,sVar3);
      }
      uVar2 = FUN_00436652((int)param_2,1);
      *(undefined2 *)
       (&DAT_0047e2f0 +
       (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) =
           (undefined2)local_30;
      FUN_00409901((int)param_2);
      break;
    case 0x69:
      if (DAT_00485c6c == '\0') {
        if (param_2[2] == 2) {
          iVar9 = 0;
          uVar2 = FUN_00436652((int)param_2,1);
          uVar7 = FUN_00432133((uint)uVar2,iVar9);
          local_30 = CONCAT22(local_30._2_2_,(short)uVar7);
        }
        else if (DAT_00460664 == -1) {
          local_30 = CONCAT22(local_30._2_2_,100);
        }
        else {
          local_30 = CONCAT22(local_30._2_2_,(short)DAT_00460664);
          DAT_00460664 = -1;
        }
        iVar9 = 0;
        uVar6 = local_30 & 0xffff;
        uVar2 = FUN_00436652((int)param_2,0);
        FUN_00431ec2(uVar2,uVar6,iVar9);
      }
      else {
        FUN_0040a2c3(param_2,&DAT_00484e80,s_BACKGRND_004611f8);
      }
      DAT_0045c060 = 0;
      DAT_00480330 = 0xffffffff;
      FUN_0040d862();
      FUN_004177c7((char *)0x0);
      FUN_0041dcc9();
      break;
    case 0x6b:
      uVar2 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,uVar2);
      if (*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        *(undefined1 *)(*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) + 0x10) = 1;
        DVar8 = timeGetTime();
        *(DWORD *)(*(int *)(&DAT_00464f60 + (local_30 & 0xffff) * 4) + 8) = DVar8;
      }
      break;
    case 0x6c:
      uVar2 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,uVar2);
      if (*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        uVar2 = FUN_00436652((int)param_2,0);
        *(undefined1 *)(*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) + 0x10) = 0;
      }
      break;
    case 0x6d:
      FUN_00416295();
      local_34 = local_34 & 0xffff0000;
      while ((local_34 & 0xffff) < 8) {
        if (param_2[(local_34 & 0xffff) + 1] == 2) {
          uVar2 = FUN_00436652((int)param_2,(byte)local_34);
          local_30 = CONCAT22(local_30._2_2_,uVar2);
          if (*(int *)(&DAT_00464300 + (uint)uVar2 * 4) == 0) {
            FUN_0040ac43();
            FUN_0040ac38();
          }
          else {
            *(undefined1 *)(*(int *)(&DAT_00464300 + (uint)uVar2 * 4) + 8) = 1;
            FUN_00438cd0();
          }
        }
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
      }
      break;
    case 0x6e:
      FUN_00416295();
      local_34 = local_34 & 0xffff0000;
      while ((local_34 & 0xffff) < 8) {
        if (param_2[(local_34 & 0xffff) + 1] == 2) {
          uVar2 = FUN_00436652((int)param_2,(byte)local_34);
          local_30 = CONCAT22(local_30._2_2_,uVar2);
          if (*(int *)(&DAT_00464300 + (uint)uVar2 * 4) == 0) {
            FUN_0040ac43();
            FUN_0040ac38();
          }
          else {
            *(undefined1 *)(*(int *)(&DAT_00464300 + (uint)uVar2 * 4) + 8) = 0;
            FUN_00438cd0();
          }
        }
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
      }
      break;
    case 0x74:
      if (param_2[1] == 2) {
        uVar2 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,
                            *(undefined2 *)
                             (&DAT_0047e2f0 +
                             (uint)*(ushort *)
                                    (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2
                             ));
      }
      else {
        sVar3 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,sVar3);
      }
      uVar2 = FUN_00436652((int)param_2,1);
      *(short *)(&DAT_0047e2f0 +
                (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) =
           *(short *)(&DAT_0047e2f0 +
                     (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) *
                     2) + (short)local_30;
      FUN_00409a12((int)param_2);
      break;
    case 0x75:
      if (param_2[1] == 2) {
        uVar2 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,
                            *(undefined2 *)
                             (&DAT_0047e2f0 +
                             (uint)*(ushort *)
                                    (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2
                             ));
      }
      else {
        sVar3 = FUN_00436652((int)param_2,0);
        local_30 = CONCAT22(local_30._2_2_,sVar3);
      }
      uVar2 = FUN_00436652((int)param_2,1);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          < (local_30 & 0xffff)) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        uVar2 = FUN_00436652((int)param_2,1);
        *(short *)(&DAT_0047e2f0 +
                  (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
             = *(short *)(&DAT_0047e2f0 +
                         (uint)*(ushort *)
                                (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) -
               (short)local_30;
        FUN_00409ab8((int)param_2);
      }
      break;
    case 0x7c:
    case 0x92:
    case 0x93:
    case 0x94:
      local_34 = (uint)local_34._2_2_ << 0x10;
      if (*param_2 != 0x7c) {
        local_34 = CONCAT22(uVar2,*param_2 - 0x7b);
      }
      FUN_004096f9((int)param_1,(int)param_2,local_34 & 0xffff);
      break;
    case 0x7d:
      if (param_2[2] == 2) {
        uVar2 = FUN_00436652((int)param_2,1);
        local_30 = CONCAT22(local_30._2_2_,
                            *(undefined2 *)
                             (&DAT_0047e2f0 +
                             (uint)*(ushort *)
                                    (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2
                             ));
      }
      else if (param_2[2] == 0x40) {
        sVar3 = FUN_00436652((int)param_2,1);
        local_30 = CONCAT22(local_30._2_2_,sVar3);
      }
      else {
        local_30 = CONCAT22(local_30._2_2_,0xffff);
      }
      uVar2 = FUN_00436652((int)param_2,0);
      uVar6 = FUN_00431e89((ushort)local_30);
      *(short *)(&DAT_0047e2f0 +
                (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) =
           (short)uVar6;
      FUN_00409981((int)param_2);
      break;
    case 0x7e:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      if (param_2[2] == 2) {
        uVar2 = FUN_00436652((int)param_2,1);
        local_24 = CONCAT22(local_24._2_2_,
                            *(undefined2 *)
                             (&DAT_0047e2f0 +
                             (uint)*(ushort *)
                                    (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2
                             ));
      }
      else {
        sVar3 = FUN_00436652((int)param_2,1);
        local_24 = CONCAT22(local_24._2_2_,sVar3);
      }
      if (*(int *)(&DAT_00464f60 + (local_30 & 0xffff) * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        **(uint **)(&DAT_00464f60 + (local_30 & 0xffff) * 4) = local_24 & 0xffff;
        *(undefined4 *)(*(int *)(&DAT_00464f60 + (local_30 & 0xffff) * 4) + 4) = 0;
        DVar8 = timeGetTime();
        *(DWORD *)(*(int *)(&DAT_00464f60 + (local_30 & 0xffff) * 4) + 8) = DVar8;
      }
      break;
    case 0x7f:
      uVar2 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,uVar2);
      local_24 = CONCAT22(local_24._2_2_,
                          (short)**(undefined4 **)(&DAT_00464f60 + (uint)uVar2 * 4) -
                          (short)*(undefined4 *)(*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) + 4));
      if (*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        uVar2 = FUN_00436652((int)param_2,1);
        *(undefined2 *)
         (&DAT_0047e2f0 +
         (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) =
             (undefined2)local_30;
      }
      break;
    case 0x82:
      uVar2 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,uVar2);
      if (*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) == 0) {
        FUN_0040abb4();
      }
      else {
        *(undefined4 *)(*(int *)(&DAT_00464f60 + (uint)uVar2 * 4) + 4) = 0;
        DVar8 = timeGetTime();
        *(DWORD *)(*(int *)(&DAT_00464f60 + (local_30 & 0xffff) * 4) + 8) = DVar8;
      }
      break;
    case 0x84:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_004326eb(local_30,(int)param_2);
      }
      break;
    case 0x85:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_00432904(local_30,(int)param_2,'\x01');
      }
      break;
    case 0x86:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_0043299d(local_30,(int)param_2);
      }
      break;
    case 0x87:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_004329e4(local_30,(int)param_2);
      }
      break;
    case 0x88:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_00432a63(local_30,(int)param_2);
      }
      break;
    case 0x8c:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_00432c9c(local_30);
      }
      break;
    case 0x8d:
      FUN_00431a69(local_30 & 0xffff);
      break;
    case 0x8e:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_00432d14(local_30);
      }
      break;
    case 0x90:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      if (sVar3 == 0) {
        FUN_00432303();
      }
      FUN_00432401();
      DAT_004859cd = 1;
      break;
    case 0x91:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        if (param_2[2] == 2) {
          uVar2 = FUN_00436652((int)param_2,1);
          local_24 = CONCAT22(local_24._2_2_,
                              *(undefined2 *)
                               (&DAT_0047e2f0 +
                               (uint)*(ushort *)
                                      (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) *
                               2));
        }
        else {
          sVar3 = FUN_00436652((int)param_2,1);
          local_24 = CONCAT22(local_24._2_2_,sVar3);
        }
        FUN_004325d3(local_30 & 0xffff,local_24 & 0xffff);
      }
      break;
    case 0x95:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        if (param_2[2] == 2) {
          uVar2 = FUN_00436652((int)param_2,1);
          local_24 = CONCAT22(local_24._2_2_,
                              *(undefined2 *)
                               (&DAT_0047e2f0 +
                               (uint)*(ushort *)
                                      (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) *
                               2));
        }
        else {
          sVar3 = FUN_00436652((int)param_2,1);
          local_24 = CONCAT22(local_24._2_2_,sVar3);
        }
        *(undefined1 *)(*(int *)(&DAT_004649e8 + (local_30 & 0xffff) * 4) + 8) =
             (undefined1)local_24;
      }
      break;
    case 0x9c:
      local_34 = (uint)local_34._2_2_ << 0x10;
      while ((local_34 & 0xffff) < 8) {
        if (param_2[(local_34 & 0xffff) + 1] == 2) {
          uVar2 = FUN_00436652((int)param_2,(byte)local_34);
          local_30 = CONCAT22(local_30._2_2_,uVar2);
          FUN_004102f7((uint)uVar2);
        }
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
      }
      break;
    case 0xa3:
      uVar7 = FUN_004319b3((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,(short)uVar7);
      uVar2 = FUN_00436652((int)param_2,0);
      if ((uint)*(ushort *)
                 (&DAT_0047e2f0 +
                 (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
          < (uint)(0xffff / (ulonglong)(longlong)(int)(local_30 & 0xffff))) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        uVar2 = FUN_00436652((int)param_2,0);
        *(short *)(&DAT_0047e2f0 +
                  (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
             = *(short *)(&DAT_0047e2f0 +
                         (uint)*(ushort *)
                                (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) *
               (short)local_30;
        FUN_00409b5e((int)param_2);
      }
      break;
    case 0xa4:
      uVar7 = FUN_004319b3((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,(short)uVar7);
      if ((short)uVar7 == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
      }
      else {
        uVar2 = FUN_00436652((int)param_2,0);
        *(short *)(&DAT_0047e2f0 +
                  (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2)
             = (short)((ulonglong)
                       *(ushort *)
                        (&DAT_0047e2f0 +
                        (uint)*(ushort *)
                               (&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) /
                      (ulonglong)(longlong)(int)(local_30 & 0xffff));
        FUN_00409bde((int)param_2);
      }
      break;
    case 0xa5:
      uVar7 = FUN_004319b3((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,(short)uVar7);
      break;
    case 0xa6:
      uVar7 = FUN_004319b3((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,(short)uVar7);
      break;
    case 0xa7:
      _DAT_00485c70 = 1;
      break;
    case 0xa8:
      _DAT_00485c70 = 0;
      break;
    case 0xa9:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_00432d3b();
      }
      break;
    case 0xaa:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      iVar9 = FUN_00431b0f(local_30,*param_2);
      if (iVar9 == 0) {
        FUN_00432d46();
      }
      break;
    case 0xab:
      uVar2 = FUN_00436652((int)param_2,0);
      local_28._0_2_ = uVar2;
      uVar2 = FUN_00408bb2(uVar2);
      local_28 = CONCAT22(local_28._2_2_,uVar2);
      sVar3 = FUN_00436652((int)param_2,1);
      local_38 = CONCAT22(local_38._2_2_,sVar3);
      uVar2 = FUN_00436652((int)param_2,2);
      uVar6 = FUN_00408c81((uint)uVar2);
      local_44 = CONCAT22(local_44._2_2_,(short)uVar6);
      if ((local_38 & 0xffff) == 1) {
        FUN_004077b2(local_44,local_28);
      }
      else if (((local_38 & 0xffff) == 0) || ((local_38 & 0xffff) == 2)) {
        FUN_00407933(local_44,local_28);
      }
      **(undefined1 **)(&DAT_00486f90 + (local_28 & 0xffff) * 4) = (undefined1)local_38;
      break;
    case 0xb4:
      DAT_0045d1c0 = 0;
      break;
    case 0xb5:
      DAT_0045d1c0 = 1;
      FUN_00407420(1);
      break;
    case 0xb9:
      uVar6 = local_2c >> 0x10;
      local_2c = local_2c & 0xffff0000;
      if (param_2[4] == 2) {
        iVar9 = 1;
        uVar2 = FUN_00436652((int)param_2,3);
        uVar7 = FUN_00432133((uint)uVar2,iVar9);
        local_2c = CONCAT22(local_2c._2_2_,(short)uVar7);
      }
      else if (DAT_00460664 != -1) {
        local_2c = CONCAT22((short)uVar6,(short)DAT_00460664);
        DAT_00460664 = -1;
      }
      uVar7 = FUN_004319b3((int)param_2,1);
      local_30 = CONCAT22(local_30._2_2_,(short)uVar7);
      if ((short)uVar7 != 0) {
        uVar2 = FUN_00436652((int)param_2,1);
        *(undefined2 *)
         (&DAT_0047e2f0 +
         (uint)*(ushort *)(&DAT_0046ee00 + (uint)uVar2 * 2 + (uint)DAT_00482428 * 600) * 2) = 0;
      }
      uVar6 = FUN_004319b3((int)param_2,2);
      local_24 = CONCAT22(local_24._2_2_,(short)uVar6);
      if (DAT_00485c6c == '\0') {
        uVar5 = local_2c & 0xffff;
        uVar6 = uVar6 & 0xffff;
        uVar4 = local_30 & 0xffff;
        uVar2 = FUN_00436652((int)param_2,0);
        FUN_00432295((uint)uVar2,uVar4,uVar6,uVar5);
      }
      else {
        FUN_0040a2c3(param_2,&DAT_00483d90,&DAT_00461204);
      }
      DAT_0045c060 = 1;
      DAT_0048253e = 0xffff;
      FUN_0040d862();
      FUN_004177c7((char *)0x0);
      FUN_0041dcc9();
      break;
    case 0xba:
      uVar2 = FUN_00436652((int)param_2,0);
      FUN_00401c21((uint)uVar2);
      break;
    case 0xbb:
      uVar2 = FUN_00436652((int)param_2,0);
      DAT_0045d1c4 = FUN_00408c81((uint)uVar2);
      FUN_00407420(0);
      break;
    case 0xbc:
      local_34 = (uint)local_34._2_2_ << 0x10;
      while ((local_34 & 0xffff) < 8) {
        if (param_2[(local_34 & 0xffff) + 1] == 2) {
          uVar2 = *(ushort *)(param_2 + (local_34 & 0xffff) * 4 + 10);
          local_30 = CONCAT22(local_30._2_2_,uVar2);
          if (*(int *)(*(int *)(&DAT_00486220 + (uint)uVar2 * 4) + 0x2c) == 0) {
            FUN_004281b1((uint)uVar2);
          }
          else {
            FUN_004277ed((uint)uVar2);
          }
          *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x34) = 0;
        }
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
      }
      break;
    case 0xbd:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      local_2c = local_2c & 0xffff0000;
      local_24 = local_24 & 0xffff0000;
      if (param_2[3] == 0x40) {
        sVar3 = FUN_00436652((int)param_2,2);
        local_24 = CONCAT22(local_24._2_2_,sVar3);
        if (param_2[4] == 0x40) {
          sVar3 = FUN_00436652((int)param_2,3);
          local_2c = CONCAT22(local_2c._2_2_,sVar3);
        }
      }
      uVar6 = local_2c & 0xffff;
      uVar5 = local_24 & 0xffff;
      uVar2 = FUN_00436652((int)param_2,1);
      FUN_0040d031((uint *)**(undefined4 **)(&DAT_00484330 + (local_30 & 0xffff) * 4),(uint)uVar2,
                   uVar5,uVar6);
      break;
    case 0xbf:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar6 = FUN_004319b3((int)param_2,1);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x28) = uVar6 & 0xffff;
      uVar6 = FUN_004319b3((int)param_2,2);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x2c) = uVar6 & 0xffff;
      if (param_2[4] == 2) {
        uVar6 = FUN_004319b3((int)param_2,3);
        *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x1c) = uVar6 & 0xffff;
      }
      break;
    case 0xc0:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar6 = FUN_004319b3((int)param_2,1);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x1c) = uVar6 & 0xffff;
      if (param_2[3] == 2) {
        uVar6 = FUN_004319b3((int)param_2,2);
        *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x20) = uVar6 & 0xffff;
        if (param_2[4] == 2) {
          uVar6 = FUN_004319b3((int)param_2,3);
          *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x24) = uVar6 & 0xffff;
        }
      }
      FUN_00427d1a(*(int **)(&DAT_00486220 + (local_30 & 0xffff) * 4));
      break;
    case 0xc1:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar6 = FUN_004319b3((int)param_2,1);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x30) = uVar6 & 0xffff;
      if (*(int *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x30) == 3000) {
        *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x30) = 0xffffffff;
      }
      break;
    case 0xc2:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar6 = FUN_004319b3((int)param_2,1);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x20) = uVar6 & 0xffff;
      uVar6 = FUN_004319b3((int)param_2,2);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x24) = uVar6 & 0xffff;
      if (param_2[4] == 2) {
        uVar6 = FUN_004319b3((int)param_2,3);
        *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x1c) = uVar6 & 0xffff;
      }
      FUN_00427d1a(*(int **)(&DAT_00486220 + (local_30 & 0xffff) * 4));
      break;
    case 0xc4:
      local_34 = (uint)local_34._2_2_ << 0x10;
      while ((local_34 & 0xffff) < 8) {
        if (param_2[(local_34 & 0xffff) + 1] == 2) {
          local_30 = CONCAT22(local_30._2_2_,*(ushort *)(param_2 + (local_34 & 0xffff) * 4 + 10));
          FUN_00427de1((uint)*(ushort *)(param_2 + (local_34 & 0xffff) * 4 + 10),1);
          if (*(int *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x14) != 0) {
            *(undefined4 *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x34) = 1;
          }
        }
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
      }
      break;
    case 0xc5:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar6 = FUN_004319b3((int)param_2,1);
      *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x10) = uVar6 & 0xffff;
      FUN_00427d1a(*(int **)(&DAT_00486220 + (local_30 & 0xffff) * 4));
      break;
    case 0xce:
      sVar3 = FUN_00436652((int)param_2,0);
      local_30 = CONCAT22(local_30._2_2_,sVar3);
      uVar7 = FUN_004319b3((int)param_2,1);
      local_24 = CONCAT22(local_24._2_2_,(short)uVar7);
      if ((short)uVar7 == 0) {
        local_2c = CONCAT22(local_2c._2_2_,1);
      }
      else {
        local_2c = (uint)local_2c._2_2_ << 0x10;
      }
      if (*(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x14) != (local_2c & 0xffff)
         ) {
        *(uint *)(*(int *)(&DAT_00486220 + (local_30 & 0xffff) * 4) + 0x14) = local_2c & 0xffff;
        FUN_00427d49(*(int **)(&DAT_00486220 + (local_30 & 0xffff) * 4));
      }
      break;
    case 0xcf:
      local_34 = (uint)local_34._2_2_ << 0x10;
      while ((local_34 & 0xffff) < 8) {
        if (param_2[(local_34 & 0xffff) + 1] == 2) {
          local_30 = CONCAT22(local_30._2_2_,*(ushort *)(param_2 + (local_34 & 0xffff) * 4 + 10));
          FUN_00427a01((uint)*(ushort *)(param_2 + (local_34 & 0xffff) * 4 + 10));
        }
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
      }
      break;
    case 0xd0:
      FUN_004278ab();
    }
    if (DAT_0045c060 == 0) {
      FUN_0043c466(&local_c,'\0');
    }
    param_2 = *(byte **)(param_2 + 0x2c);
  }
  return;
}

