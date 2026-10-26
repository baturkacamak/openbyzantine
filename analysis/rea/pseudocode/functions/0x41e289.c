// rea decompile_all
// name: FUN_0041e289
// address: 0x41e289
// body: available
// range: 0x41e289-0x41e9fe inclusive
// range: 0x41ea04-0x41ea24 inclusive
// range: 0x41ea41-0x41ed46 inclusive
// range: 0x41ed4c-0x41ed6d inclusive
// range: 0x41edea-0x41efc0 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0041e289(int param_1)

{
  bool bVar1;
  undefined4 in_EAX;
  INT_PTR IVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined2 extraout_var_01;
  undefined2 extraout_var_02;
  undefined2 uVar6;
  uint local_258 [65];
  uint local_154;
  int local_150;
  int local_14c;
  int local_148;
  uint local_144;
  uint local_140;
  INT_PTR local_13c;
  undefined2 local_134;
  undefined2 uStack_132;
  char local_130 [260];
  uint local_2c;
  uint local_28;
  int local_24;
  int local_20;
  char *local_1c [4];
  int local_c;
  uint local_8;
  
  local_8 = CONCAT22(local_8._2_2_,0xfffd);
  local_c = 0;
  local_1c[0] = s_vtdisc_0045f994;
  local_1c[1] = s_vbdisc_0045f9a4;
  local_1c[2] = s_vcdisc_0045f9b4;
  local_1c[3] = s_vhdisc_0045f9c4;
  _local_134 = CONCAT22(uStack_132,DAT_00476338);
  if (((param_1 != 6) && (DAT_0045d1d0 == 0)) && (DAT_0045f490 == 0)) {
    param_1 = 7;
  }
  if (param_1 == 7) {
    iVar3 = CONCAT22((short)((uint)in_EAX >> 0x10),DAT_00476338);
    if (DAT_0047a230 == 0) {
      DAT_0047a230 = DAT_00484e28;
      iVar3 = DAT_00484e28;
    }
    DAT_0045f058 = 1;
    uVar4 = CONCAT22((short)((uint)iVar3 >> 0x10),0xffff);
  }
  else {
    DAT_004867f0 = 1;
    FUN_004287ff();
    FUN_004177c7((char *)0x0);
    if (param_1 == 6) {
      DAT_0047a230 = DAT_00484e28;
      if ((DAT_0045f48c != 0) || (IVar2 = FUN_0041d6df(9), IVar2 == 6)) {
        local_8 = CONCAT22(local_8._2_2_,1);
      }
    }
    else {
      DAT_0045d1d0 = 0;
      if (DAT_0045f490 != 2) {
        FUN_0040da1f();
        FUN_0040df05(0x1d);
      }
      DAT_0045f7d0 = -1;
      DAT_0045f7d4 = -1;
      DAT_0045f7d8 = -1;
      if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
        local_c = 1;
      }
      FUN_0041f753(0);
      iVar3 = FUN_00420b99();
      if ((iVar3 == 0) &&
         ((FUN_0041f065(1), DAT_0045f490 == 2 || (iVar3 = FUN_0041f9ea(param_1), iVar3 == param_1)))
         ) {
        FUN_0042875d(DAT_0045f530);
        FUN_00420c51();
        do {
          local_28 = 0;
          local_2c = 0x29;
          while ((local_8 & 0xffff) == 0xfffd) {
            FUN_0041624a();
            local_144 = 0x29;
            if (DAT_0045f490 == 2) {
              local_144 = 0x24;
              DAT_0045f7d8 = 0x34;
              DAT_0045f490 = 1;
            }
            while (local_144 == 0x29) {
              FUN_0041624a();
              if (DAT_0045f05c != 0) {
                uVar4 = FUN_00418040(DAT_00477344);
                FUN_0041a06b(uVar4);
              }
              local_154 = 0x29;
              local_14c = (int)DAT_00486a0a;
              local_150 = (int)DAT_00486a0c;
              for (local_140 = 0; (int)local_140 < 0x28; local_140 = local_140 + 1) {
                if (((((&DAT_0046c5d0)[local_140] != '\0') &&
                     (*(int *)(&DAT_0045f548 + local_140 * 0x10) < local_14c)) &&
                    (local_14c < *(int *)(&DAT_0045f550 + local_140 * 0x10))) &&
                   ((*(int *)(&DAT_0045f54c + local_140 * 0x10) < local_150 &&
                    (local_150 < *(int *)(&DAT_0045f554 + local_140 * 0x10))))) {
                  local_154 = local_140;
                  break;
                }
              }
              if (((local_154 == 0x20) || (local_2c == 0x20)) && (param_1 == 4)) {
                FUN_00420fe5(local_14c,local_150);
              }
              if ((local_154 == 0x24) || (local_2c == 0x24)) {
                FUN_00422101(local_154,local_150);
              }
              if (local_2c != local_154) {
                if ((int)local_2c < 0xf) {
                  FUN_0041f92e(local_2c);
                }
                else if ((0x11 < (int)local_2c) && ((int)local_2c < 0x1c)) {
                  FUN_004202ff(0x29,1);
                  FUN_0041f279(-1);
                }
                if (local_154 == 0x29) {
                  FUN_0040df05(0x1d);
                }
                else {
                  FUN_0040df05(0x1e);
                  if ((int)local_154 < 0xf) {
                    FUN_0041f90e(local_154);
                  }
                  else if ((0x11 < (int)local_154) && ((int)local_154 < 0x1c)) {
                    FUN_004202ff(local_154 - 0x12,1);
                    FUN_0041f279(local_154 - 0x12);
                  }
                }
              }
              local_2c = local_154;
              iVar3 = FUN_0040d862();
              if (iVar3 == 0) {
                local_28 = 0x29;
              }
              else {
                local_144 = local_154;
              }
            }
            if (local_28 != local_144) {
              local_28 = local_144;
              switch(local_144) {
              case 0:
                if (DAT_0045f7d0 == -1) {
                  FUN_0041effe(local_130);
                }
                else {
                  IVar2 = FUN_0041d6df(0xc);
                  if (IVar2 == 7) {
                    FUN_0041f92e(0);
                    break;
                  }
                  _sprintf(local_130,s_savegame__s_0045f9f0,(&DAT_0046c660)[DAT_0045f7d0]);
                }
                local_13c = FUN_00421fa2();
                if (local_13c != 0) {
                  local_24 = FUN_00424dc0(local_130);
                  if (local_24 == 0) {
                    local_c = 0;
                  }
                  if (DAT_0045f7d0 == -1) {
                    FUN_0041f1ac(local_130);
                  }
                  FUN_004206d6();
                }
                FUN_0041f92e(0);
                break;
              case 1:
                if (DAT_0045f7d0 != -1) {
                  IVar2 = FUN_0041d6df(8);
                  if (IVar2 == 6) {
                    FUN_0044ba00(local_258,(uint *)(&DAT_0046c660)[DAT_0045f7d0]);
                    uVar5 = FUN_00425ab4(local_258);
                    if (uVar5 == 0) {
                      local_8 = CONCAT22(local_8._2_2_,2);
                    }
                    else {
                      FUN_0041d6df(0x3f);
                    }
                  }
                  FUN_0041f92e(1);
                }
                break;
              case 2:
                if (DAT_0045f7d0 != -1) {
                  IVar2 = FUN_0041d6df(0xb);
                  if (IVar2 == 6) {
                    FUN_0041f595(DAT_0045f7d0);
                    FUN_004206d6();
                  }
                  FUN_0041f92e(2);
                }
                break;
              case 3:
                IVar2 = FUN_0041d6df(10);
                if ((IVar2 == 6) &&
                   ((bVar1 = FUN_0040c6d6(1), CONCAT31(extraout_var_00,bVar1) != 0 ||
                    (iVar3 = FUN_0040c7b4(6), iVar3 == 0)))) {
                  uVar5 = FUN_00425ab4((uint *)s_new_can_0045f9e8);
                  if (uVar5 == 0) {
                    local_8 = CONCAT22(local_8._2_2_,0xffff);
                    FUN_00408f32(0);
                  }
                  else {
                    FUN_0041d6df(0x3f);
                  }
                }
                FUN_0041f92e(3);
                break;
              case 4:
                if ((local_c == 0) || (IVar2 = FUN_0041d6df(9), IVar2 == 6)) {
                  local_8 = CONCAT22(local_8._2_2_,1);
                }
                else {
                  FUN_0041f92e(4);
                }
                break;
              case 5:
                if (DAT_0045f7e0 == 0) {
                  DAT_0047a230 = DAT_00484e28;
                  local_8 = CONCAT22(local_8._2_2_,0xffff);
                }
                else {
                  uVar5 = FUN_00425ab4((uint *)s_qsave_000_0045f9cc);
                  if (uVar5 == 0) {
                    local_8 = CONCAT22(local_8._2_2_,2);
                  }
                  else {
                    FUN_0041d6df(0x3f);
                  }
                }
                break;
              case 6:
              case 7:
              case 9:
              case 10:
              case 0xb:
              case 0xc:
                local_20 = local_144 - 6;
                if (*(int *)(&DAT_0045f528 + local_20 * 4) == 0) {
                  *(undefined4 *)(&DAT_0045f528 + local_20 * 4) = 1;
                }
                else {
                  *(undefined4 *)(&DAT_0045f528 + local_20 * 4) = 0;
                }
                if (*(int *)(&DAT_0045f528 + local_20 * 4) == 1) {
                  FUN_0041f96e(local_144);
                }
                else {
                  FUN_0041f94e(local_144);
                }
                break;
              case 8:
                DAT_0045f530 = (uint)(DAT_0045f530 == 0);
                FUN_0042875d(DAT_0045f530);
                if (DAT_0045f530 == 0) {
                  FUN_00428aa4();
                  FUN_0041f94e(local_144);
                }
                else {
                  FUN_0041f96e(local_144);
                }
                break;
              case 0xd:
              case 0x1f:
                param_1 = FUN_0041f9ea(4);
                break;
              case 0xe:
              case 0xf:
              case 0x10:
              case 0x11:
                param_1 = FUN_0041f9ea(local_144 - 0xe);
                DAT_0045f7d0 = -1;
                break;
              case 0x1c:
              case 0x1d:
                FUN_0042062a(local_144);
                break;
              case 0x1e:
              case 0x21:
                param_1 = FUN_0041f9ea(1);
                break;
              case 0x20:
                if (DAT_0045f7d4 != -1) {
                  param_1 = FUN_0041f9ea(5);
                }
                break;
              case 0x22:
              case 0x23:
                param_1 = FUN_0041f9ea(3);
                break;
              case 0x24:
                switch(DAT_0045f7d8) {
                case 0x34:
                  FUN_0041fcf3();
                  break;
                case 0x35:
                  local_148 = DAT_0045da78;
                  bVar1 = FUN_0040c6d6(1);
                  if ((CONCAT31(extraout_var,bVar1) != 0) || (iVar3 = FUN_0040c7b4(6), iVar3 == 0))
                  {
                    FUN_0041fc87((uint *)s_mintro_0045f9d8);
                    DAT_0045da78 = local_148;
                  }
                  break;
                case 0x36:
                case 0x37:
                case 0x38:
                case 0x39:
                  local_20 = DAT_0045f7d8 + -0x36;
                  iVar3 = FUN_0041dd22(local_20,local_c);
                  if (iVar3 == 0) {
                    for (local_140 = 0; local_140 < DAT_00482528; local_140 = local_140 + 1) {
                      iVar3 = __strcmpi(&DAT_00483bc0 + local_140 * 9,local_1c[local_20]);
                      if (iVar3 == 0) {
                        DAT_00482428 = (undefined2)local_140;
                        DAT_0047a230 = 0;
                        local_8 = local_8 & 0xffff0000;
                        FUN_0041f753(1);
                        FUN_00419d28(DAT_0047a8a4);
                        DAT_004859cd = 1;
                        break;
                      }
                    }
                  }
                  break;
                case 0x3a:
                  local_148 = DAT_0045da78;
                  iVar3 = FUN_0040c7b4(6);
                  if (iVar3 == 0) {
                    FUN_0041fc87((uint *)s_mepilog_0045f9e0);
                    DAT_0045da78 = local_148;
                  }
                }
                if ((local_8 & 0xffff) != 0) {
                  param_1 = FUN_0041f9ea(2);
                }
                break;
              case 0x25:
              case 0x26:
                FUN_00421949(local_144,1);
                break;
              case 0x27:
                FUN_0041e002();
                FUN_0041f9ea(param_1);
                DAT_0045f7d0 = -1;
              }
              if ((0x11 < (int)local_144) && ((int)local_144 < 0x1c)) {
                local_20 = (local_144 - 0x12) + DAT_0045f7cc * 10;
                if (local_20 == DAT_0045f7d0) {
                  DAT_0045f7d0 = -1;
                }
                else {
                  if (DAT_0045f7d0 != -1) {
                    DAT_0045f7d0 = 0xffffffff;
                    FUN_0041f279(local_144 - 0x12);
                  }
                  DAT_0045f7d0 = local_20;
                }
                FUN_0042044d(1);
                FUN_004202ff(0x29,1);
              }
            }
          }
          if ((local_8 & 0xffff) == 0) {
            DAT_0045f498 = 1;
            FUN_0040105d();
            break;
          }
          DAT_0045f7e0 = 0;
          DAT_0045f498 = 0;
          FUN_0041f7be();
          if (((local_8 & 0xffff) == 1) || (iVar3 = FUN_0040c7b4(DAT_0045da78), iVar3 == 0)) break;
          local_8 = CONCAT22(local_8._2_2_,0xfffd);
          FUN_0041f92e(5);
        } while( true );
      }
    }
    if ((local_8 & 0xffff) == 0xfffd) {
      local_8 = CONCAT22(local_8._2_2_,0xffff);
    }
    FUN_00420a95();
    FUN_0041fd8a();
    FUN_0041fde8();
    FUN_0042151e();
    DAT_0045d1d0 = 1;
    FUN_0042875d(0);
    if ((local_8 & 0xffff) == 1) {
      FUN_00419d28(DAT_0047a8a4);
      uVar6 = extraout_var_02;
    }
    else {
      FUN_0040df05((ushort)_local_134);
      FUN_004288e5();
      FUN_0041dcc9();
      FUN_0042b29f(DAT_00477344,0x20,0x4b000);
      uVar6 = extraout_var_01;
      if (param_1 != 6) {
        DAT_0045f058 = 0;
      }
    }
    DAT_0045f490 = 0;
    DAT_004867f0 = 0;
    uVar4 = CONCAT22(uVar6,(undefined2)local_8);
  }
  return uVar4;
}

