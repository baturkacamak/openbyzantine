// rea decompile_all
// name: FUN_00436b01
// address: 0x436b01
// body: available
// range: 0x436b01-0x437bcb inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_00436b01(int *param_1,uint param_2)

{
  short sVar1;
  ushort uVar2;
  int iVar3;
  undefined4 uVar4;
  INT_PTR IVar5;
  int aiStackY_424 [83];
  undefined4 uStackY_2d8;
  byte local_2a8;
  uint local_240;
  uint local_23c;
  byte local_234;
  byte local_228;
  LPARAM local_220;
  uint auStack_20e [48];
  undefined1 auStack_14c [44];
  uint auStack_120 [10];
  int local_f8;
  int *local_f4;
  ushort *local_f0;
  byte local_e8;
  char *local_e4;
  uint local_e0;
  char *local_dc;
  undefined1 local_d8;
  undefined1 local_d7;
  undefined1 local_d6;
  uint local_8;
  
  local_8 = 0xffff;
  local_f4 = (int *)FUN_00418040(DAT_00477344);
  iVar3 = param_1[2];
  uVar2 = FUN_004330bf((int)param_1,(int)param_1 + 0x41);
  *(ushort *)(param_1 + 2) = uVar2;
  uVar2 = FUN_004331d2((int)param_1,(int)(param_1 + 0x5c));
  *(ushort *)(param_1 + 2) = uVar2;
  if ((short)param_1[2] == (short)iVar3) {
    local_e8 = 0;
    do {
      iVar3 = param_1[2];
      uVar2 = FUN_004330bf((int)param_1,(int)param_1 + 0x73);
      *(ushort *)(param_1 + 2) = uVar2;
      uVar2 = FUN_004331d2((int)param_1,(int)(param_1 + 0x8e));
      *(ushort *)(param_1 + 2) = uVar2;
      local_e8 = local_e8 + 1;
      if ((short)param_1[2] == (short)iVar3) break;
    } while (local_e8 < 0x32);
    if (local_e8 == 0x32) {
      FUN_0040ac43();
      FUN_0040ac38();
    }
    uVar2 = *(ushort *)(param_1 + 2);
    do {
      local_f0 = (ushort *)param_1[*(ushort *)(param_1 + 2) + 0x163];
      *(short *)(param_1 + 2) = (short)param_1[2] + 1;
      if (local_f0 != (ushort *)0x0) break;
    } while ((short)param_1[2] != 0x32);
    *(short *)(param_1 + 2) = (short)param_1[2] + -1;
    DAT_0047a8d8 = '\0';
    if (((local_f0 == (ushort *)0x0) || ((short)param_1[2] == 0x31)) &&
       (param_1[uVar2 + 0x195] == 0)) {
      *(undefined1 *)((int)param_1 + 0xe) = 1;
    }
    else {
      *(ushort *)(param_1 + 2) = uVar2;
      local_f0 = (ushort *)param_1[uVar2 + 0x163];
      local_e4 = (char *)param_1[*(ushort *)(param_1 + 2) + 0x195];
      *(undefined1 *)((int)param_1 + 10) = 0;
      local_e8 = 0;
      for (; local_f0 != (ushort *)0x0; local_f0 = *(ushort **)((int)local_f0 + 8)) {
        if ((*(char *)((int)local_f0 + 0x10) == '\x01') &&
           ((*(char *)((int)local_f0 + 0x11) == '\0' || (DAT_00483070 != '\0')))) {
          local_e8 = local_e8 + 1;
        }
      }
      for (; local_e4 != (char *)0x0; local_e4 = *(char **)(local_e4 + 4)) {
        if (*local_e4 != -1) {
          if (*local_e4 != '\0') {
            *local_e4 = -1;
          }
          for (local_f0 = *(ushort **)(local_e4 + 8); local_f0 != (ushort *)0x0;
              local_f0 = *(ushort **)((int)local_f0 + 8)) {
            if ((*(char *)((int)local_f0 + 0x10) == '\x01') &&
               ((*(char *)((int)local_f0 + 0x11) == '\0' || (DAT_00483070 != '\0')))) {
              local_e8 = local_e8 + 1;
            }
          }
        }
      }
      local_f0 = (ushort *)param_1[*(ushort *)(param_1 + 2) + 0x163];
      if (local_e8 == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
        *(undefined1 *)((int)param_1 + 0xe) = 1;
      }
      else {
        DAT_0048304c = 0;
        for (local_234 = 0; local_234 < 0x14; local_234 = local_234 + 1) {
          *(undefined2 *)((int)auStack_120 + (uint)local_234 * 2) = 0;
          *(undefined2 *)(auStack_14c + (uint)local_234 * 2) = 0;
        }
        local_e4 = (char *)param_1[*(ushort *)(param_1 + 2) + 0x195];
        if (local_f0 == (ushort *)0x0) {
          local_f0 = *(ushort **)(local_e4 + 8);
          local_e4 = *(char **)(local_e4 + 4);
        }
        local_234 = 0x61;
        while (local_f0 != (ushort *)0x0) {
          if (((char)local_f0[8] != '\0') &&
             ((DAT_00483070 != '\0' || (*(char *)((int)local_f0 + 0x11) == '\0')))) {
            local_d8 = 0x7f;
            local_d7 = 0x20;
            local_d6 = 0;
            FUN_0044ba08((uint *)&local_d8,(uint *)param_1[*local_f0 + 0x1c8]);
            FUN_0044ba00((uint *)(&DAT_00476340 + (local_234 - 0x61) * 0xcd),(uint *)&local_d8);
            aiStackY_424[local_234] = (int)local_f0;
            local_234 = local_234 + 1;
          }
          local_f0 = *(ushort **)(local_f0 + 4);
          if ((local_e4 != (char *)0x0) && (local_f0 == (ushort *)0x0)) {
            local_f0 = *(ushort **)(local_e4 + 8);
            local_e4 = *(char **)(local_e4 + 4);
          }
        }
        local_240 = 0x8000;
        DAT_0046edf4 = 1;
        DAT_00485c68 = 100;
        local_f8 = 0;
        local_234 = 0;
        while( true ) {
          sVar1 = (short)DAT_00487708;
          if (local_e8 <= local_234) break;
          *(short *)(auStack_14c + (uint)local_234 * 2) = (short)local_f8 * sVar1;
          iVar3 = FUN_0040701f((uint *)(&DAT_00476340 + (uint)local_234 * 0xcd),
                               (uint)DAT_00485c68 * -2 + 0x280,0);
          local_f8 = local_f8 + iVar3;
          local_234 = local_234 + 1;
        }
        *(short *)(auStack_14c + (uint)local_234 * 2) = (short)local_f8 * sVar1;
        DAT_0047a284 = (short)local_f8 * sVar1;
        DAT_00485c6a = 0x19f - DAT_0047a284;
        if (DAT_0045f494 == 0) {
          FUN_0040d6e5();
          DAT_00485c6a = DAT_00485c6a - 0x2d;
          if (DAT_00460684 != 0) {
            FUN_004075a1(1);
            DAT_00460684 = 0;
          }
        }
        FUN_0040da1f();
        FUN_0040df05(0x1d);
        for (local_234 = 0; local_234 < local_e8; local_234 = local_234 + 1) {
          uStackY_2d8 = 0x437345;
          iVar3 = FUN_004063e0(&DAT_00476340 + (uint)local_234 * 0xcd,(uint)DAT_00485c68,
                               (*(uint *)(auStack_14c + (uint)local_234 * 2) & 0xffff) +
                               (uint)DAT_00485c6a,0,local_f4,0,0,99);
          *(short *)((int)auStack_120 + (uint)local_234 * 2) = (short)iVar3;
          if ((uint)DAT_0048304c < (*(uint *)((int)auStack_120 + (uint)local_234 * 2) & 0xffff)) {
            DAT_0048304c = *(ushort *)((int)auStack_120 + (uint)local_234 * 2);
          }
        }
        local_23c = 0;
        local_228 = 0;
        while (local_23c == 0) {
          local_e0 = CONCAT22(local_e0._2_2_,0x1d);
          uVar4 = DAT_0047a230;
          while (((DAT_0047a230 = uVar4, iVar3 = FUN_0040d862(), iVar3 == 0 && (local_240 == 0x8000)
                  ) && (DAT_0045f058 == 0))) {
            local_2a8 = 0;
            FUN_0041624a();
            for (local_234 = 0; local_234 < local_e8; local_234 = local_234 + 1) {
              if ((((int)(uint)DAT_00485c68 <= (int)DAT_00486a0a) &&
                  ((int)DAT_00486a0a <=
                   (int)((*(uint *)((int)auStack_120 + (uint)local_234 * 2) & 0xffff) +
                        (uint)DAT_00485c68))) &&
                 (((int)((*(uint *)(auStack_14c + (uint)local_234 * 2) & 0xffff) +
                        (uint)DAT_00485c6a) <= (int)DAT_00486a0c &&
                  ((int)DAT_00486a0c <
                   (int)((*(uint *)(auStack_14c + (uint)local_234 * 2 + 2) & 0xffff) +
                        (uint)DAT_00485c6a))))) {
                DAT_00483050 = 0xf;
                local_23c = local_234 + 0x61;
                local_2a8 = (byte)local_23c;
                if ((uint)local_228 != (local_23c & 0xff)) {
                  if (local_228 != 0) {
                    uStackY_2d8 = 0x437579;
                    FUN_004063e0(&DAT_00476340 + (local_228 - 0x61) * 0xcd,(uint)DAT_00485c68,
                                 (*(uint *)((int)auStack_20e + (uint)local_228 * 2) & 0xffff) +
                                 (uint)DAT_00485c6a,0,local_f4,0,0,99);
                  }
                  uStackY_2d8 = 0x4375d2;
                  FUN_004063e0(&DAT_00476340 + (uint)local_234 * 0xcd,(uint)DAT_00485c68,
                               (*(uint *)(auStack_14c + (uint)local_234 * 2) & 0xffff) +
                               (uint)DAT_00485c6a,0,local_f4,1,0,99);
                  local_228 = (byte)local_23c;
                  FUN_0043b216((undefined4 *)0x0,1,0);
                  break;
                }
              }
            }
            if ((local_2a8 == 0) && (local_228 != 0)) {
              for (local_234 = 0; local_234 < local_e8; local_234 = local_234 + 1) {
                uStackY_2d8 = 0x437699;
                FUN_004063e0(&DAT_00476340 + (uint)local_234 * 0xcd,(uint)DAT_00485c68,
                             (*(uint *)(auStack_14c + (uint)local_234 * 2) & 0xffff) +
                             (uint)DAT_00485c6a,0,local_f4,0,0,99);
              }
              local_228 = 0;
              local_23c = 0;
            }
            if (DAT_0045f05c == 0) {
              iVar3 = FUN_0041620b();
              if (iVar3 != 0) {
                FUN_0043b216((undefined4 *)0x0,0,0);
              }
            }
            else {
              uVar4 = FUN_00418040(DAT_0046e82c);
              FUN_0041a06b(uVar4);
            }
            uVar4 = DAT_0047a230;
            if ((DAT_0045f494 == 0) && (iVar3 = FUN_0043c566(&local_dc,'\x01'), iVar3 == 2)) {
              uVar2 = FUN_0042d594(local_dc,'\x01');
              local_240 = (uint)uVar2;
            }
          }
          FUN_0041624a();
          uVar4 = FUN_0040d862();
          if (local_240 == 0x8000) {
            if (DAT_0045f058 == 0) {
              if ((((((short)uVar4 != 0) && (FUN_0043c671(), local_23c == 0)) &&
                   ((param_2 & 0xffff) != (local_e0 & 0xffff))) &&
                  (((local_e0 & 0xffff) != 0xffff && ((local_e0 & 0xffff) != 0xfffe)))) &&
                 (*(char *)(*(int *)(&DAT_00464300 + (local_e0 & 0xffff) * 4) + 8) != '\0')) {
                FUN_004397ac();
                DAT_004835f8 = DAT_00485c68;
                DAT_004835fa = DAT_00485c6a;
                DAT_0047ae5c = DAT_0048304c;
                DAT_0047a828 = DAT_0047a284;
                DAT_0046edf4 = 0;
                local_23c = 0x1b;
                param_2 = local_e0 & 0xffff;
                local_8 = local_e0 & 0xffff;
              }
            }
            else {
              if (DAT_0045f494 == 0) {
                local_220 = 9;
              }
              else {
                local_220 = 0x50;
              }
              local_23c = 0;
              IVar5 = FUN_0041d6df(local_220);
              if (IVar5 == 6) {
                local_8 = 0xfffe;
                DAT_0045f48c = 1;
                local_23c = 0x1b;
              }
              else {
                DAT_0045f058 = 0;
              }
            }
          }
          else {
            local_23c = 0x1b;
          }
        }
        if (local_23c == 0x1b) {
          if ((local_240 & 0xc0) == 0) {
            *(undefined1 *)((int)param_1 + 0xe) = 1;
            if ((local_240 & 1) == 0) {
              *(undefined1 *)(param_1 + 3) = 1;
            }
            param_1[*(ushort *)(param_1 + 2) + 0xc0] = 0;
            for (local_234 = 0; local_234 < 0x32; local_234 = local_234 + 1) {
              if (param_1[local_234 + 0xc0] != 0) {
                *(undefined1 *)(param_1[local_234 + 0xc0] + 0x11) = 0;
              }
            }
          }
          else {
            *(ushort *)(param_1 + 2) = (ushort)(local_240 >> 8) & 0x7f;
            if ((local_240 & 0x80) == 0) {
              *(undefined1 *)((int)param_1 + 10) = 0;
              if (param_1[*(ushort *)(param_1 + 2) + 0x131] == 0) {
                FUN_0040ac43();
                FUN_0040ac38();
              }
            }
            else {
              *(undefined1 *)((int)param_1 + 10) = 1;
              if (param_1[*(ushort *)(param_1 + 2) + 0x163] == 0) {
                FUN_0040ac43();
                FUN_0040ac38();
              }
            }
          }
        }
        else {
          FUN_00424780((int)param_1,(uint)*(ushort *)aiStackY_424[local_23c]);
          FUN_004397ac();
          *(undefined1 *)(aiStackY_424[local_23c] + 0x11) = 1;
          param_1[*(ushort *)(param_1 + 2) + 0xc0] = aiStackY_424[local_23c];
          FUN_0043340a(param_1,*(byte **)(aiStackY_424[local_23c] + 4));
          if (DAT_0047a8d8 == '\0') {
            *(short *)(param_1 + 2) = (short)param_1[2] + 1;
          }
          else {
            DAT_0047a8d8 = '\0';
          }
        }
      }
    }
    if (local_23c != 0x1b) {
      FUN_00437c72();
    }
    if (((DAT_00485c6c == '\0') && (DAT_0048304c != 0)) && (DAT_0047a284 != 0)) {
      FUN_0042b29f(DAT_00477344,0x20,0x4b000);
      DAT_004835f8 = DAT_00485c68;
      DAT_004835fa = DAT_00485c6a;
      DAT_0047ae5c = DAT_0048304c;
      DAT_0047a828 = DAT_0047a284;
      DAT_0046edf4 = 0;
    }
  }
  else {
    *(undefined1 *)((int)param_1 + 10) = 0;
    FUN_00437c72();
  }
  return local_8;
}

