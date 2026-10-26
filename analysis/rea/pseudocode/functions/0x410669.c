// rea decompile_all
// name: FUN_00410669
// address: 0x410669
// body: available
// range: 0x410669-0x410f08 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Type propagation algorithm not settling */

undefined4 __cdecl FUN_00410669(uint param_1,uint *param_2,uint *param_3)

{
  ushort uVar1;
  undefined4 uVar2;
  char *pcVar3;
  int iVar4;
  uint *puVar5;
  char local_364 [260];
  FILE *local_260;
  DWORD local_25c;
  undefined4 local_258;
  uint local_254 [4];
  uint local_244;
  undefined4 local_230;
  uint local_22c [65];
  int local_128;
  int local_124;
  FILE *local_120;
  undefined2 local_11c;
  undefined2 uStack_11a;
  int local_118;
  undefined4 local_114;
  undefined1 local_110 [4];
  uint local_10c [65];
  DWORD local_8;
  
  local_114 = 0;
  local_25c = 0;
  local_118 = -1;
  local_124 = -1;
  local_128 = -1;
  *param_2 = 0xffff;
  local_254[0]._0_1_ = '\0';
  if ((char)*param_3 == '\0') {
    if (*(int *)(&DAT_0046e870 + param_1 * 4) != 0) {
      FUN_0044ba00(local_254,*(uint **)(*(int *)(&DAT_0046e870 + param_1 * 4) + 4));
      pcVar3 = FUN_0044c580(local_254);
      _local_11c = CONCAT22(uStack_11a,(short)pcVar3);
      if (((*(char *)((int)local_254 + (((uint)pcVar3 & 0xffff) - 3)) == 'c') &&
          (*(char *)((int)local_254 + (((uint)pcVar3 & 0xffff) - 2)) == 'c')) &&
         (*(char *)((int)local_254 + (((uint)pcVar3 & 0xffff) - 1)) == 'g')) {
        local_254[0]._0_1_ = '\0';
      }
      else {
        *(char *)((int)local_254 + (((uint)pcVar3 & 0xffff) - 1)) = 'b';
      }
    }
  }
  else {
    FUN_0044ba00(local_254,param_3);
    FUN_0044ba08(local_254,(uint *)&DAT_0045e8e0);
    *param_2 = param_1;
  }
  if ((char)local_254[0] != '\0') {
    if (DAT_0045e7a8 == 0) {
      _sprintf(local_364,s_engine__s_0045e8e8,local_254);
      local_110 = (undefined1  [4])FUN_0044bc20(local_364,&DAT_0045e8f4);
    }
    else {
      local_110 = (undefined1  [4])FUN_00411b4b(local_254,&local_118);
    }
    if (local_110 == (undefined1  [4])0x0) {
      FUN_0040ac43();
      FUN_0040ac38();
      local_114 = 1;
    }
    else {
      if (DAT_0045e7a8 == 0) {
        local_25c = FUN_0041199e((FILE *)local_110);
      }
      else {
        local_8 = FUN_0044c370((undefined4 *)local_110);
        local_25c = local_118 - local_8;
      }
      if (local_25c == 0) {
        FUN_0040ac43();
        FUN_0040ac38();
        _fclose((FILE *)local_110);
        local_114 = 1;
      }
      else {
        if ((char)*param_3 == '\0') {
          uVar1 = FUN_00410f09((ushort)param_1,
                               (char *)**(undefined4 **)(&DAT_0046e870 + param_1 * 4));
          *param_2 = (uint)uVar1;
        }
        if (*(int *)(&DAT_00482ad0 + *param_2 * 4) == 0) {
          uVar2 = FUN_0044c2f0(0x1ea4);
          *(undefined4 *)(&DAT_00482ad0 + *param_2 * 4) = uVar2;
          local_258 = *(undefined4 **)(&DAT_00482ad0 + *param_2 * 4);
          *local_258 = 0;
          local_258[1] = 0;
          *(undefined2 *)(local_258 + 2) = 0;
          *(undefined1 *)((int)local_258 + 10) = 0;
          *(undefined1 *)((int)local_258 + 0xb) = 0;
          *(undefined1 *)(local_258 + 3) = 0;
          *(undefined1 *)((int)local_258 + 0xd) = 0;
          *(undefined1 *)((int)local_258 + 0xe) = 0;
          local_244 = local_244 & 0xffff0000;
          while ((local_244 & 0xffff) < 0x2ee) {
            local_258[(local_244 & 0xffff) + 0x1c8] = 0;
            local_244 = CONCAT22(local_244._2_2_,(short)local_244 + 1);
          }
          *(undefined2 *)(local_258 + 0x1c7) = 0;
          _sprintf((char *)local_10c,s_engine__s_0045e948,local_254);
          puVar5 = (uint *)&DAT_0045e954;
          pcVar3 = FUN_0044c580(local_10c);
          FUN_0044ba00((uint *)(pcVar3 + (int)(local_110 + 1)),puVar5);
          local_244 = local_244 & 0xffff0000;
          while ((local_244 & 0xffff) < 0x2ee) {
            local_258[(local_244 & 0xffff) + 0x4b7] = 0;
            local_244 = CONCAT22(local_244._2_2_,(short)local_244 + 1);
          }
          *(undefined2 *)(local_258 + 0x4b6) = 0;
          _sprintf((char *)local_22c,s_engine__s_0045e958,local_254);
          pcVar3 = FUN_0044c580(local_22c);
          pcVar3[(int)&local_230 + 3] = 'f';
          if (DAT_0045e7a8 == 0) {
            local_260 = (FILE *)FUN_0040f350((char *)local_10c,&DAT_0045e964);
            local_120 = (FILE *)FUN_0040f350((char *)local_22c,&DAT_0045e968);
          }
          else {
            pcVar3 = _strrchr((char *)local_10c,0x5c);
            if (pcVar3 == (char *)0x0) {
              local_230 = local_10c;
            }
            else {
              local_230 = (uint *)(pcVar3 + 1);
            }
            local_260 = FUN_00411b4b(local_230,&local_128);
            pcVar3 = _strrchr((char *)local_22c,0x5c);
            if (pcVar3 == (char *)0x0) {
              local_230 = local_22c;
            }
            else {
              local_230 = (uint *)(pcVar3 + 1);
            }
            local_120 = FUN_00411b4b(local_230,&local_124);
          }
          if ((local_260 == (FILE *)0x0) || (local_120 == (FILE *)0x0)) {
            if (local_260 != (FILE *)0x0) {
              _fclose(local_260);
            }
            if (local_120 != (FILE *)0x0) {
              _fclose(local_120);
            }
            FUN_0040ac43();
            FUN_0040ac38();
            local_114 = 1;
          }
          else {
            iVar4 = FUN_004123f9(local_260,(int)(local_258 + 0x1c8),local_128);
            *(short *)(local_258 + 0x1c7) = (short)iVar4;
            _fclose(local_260);
            iVar4 = FUN_004123f9(local_120,(int)(local_258 + 0x4b7),local_124);
            *(short *)(local_258 + 0x4b6) = (short)iVar4;
            _fclose(local_120);
          }
          local_244 = local_244 & 0xffff0000;
          while ((local_244 & 0xffff) < 0x32) {
            local_258[(local_244 & 0xffff) + 0x131] = 0;
            local_258[(local_244 & 0xffff) + 0xf2] = 0;
            *(undefined1 *)((local_244 & 0xffff) + 0x490 + (int)local_258) = 0xff;
            local_258[(local_244 & 0xffff) + 0x163] = 0;
            local_258[(local_244 & 0xffff) + 0x195] = 0;
            *(undefined1 *)((local_244 & 0xffff) + 0xf + (int)local_258) = 0xff;
            *(undefined1 *)((local_244 & 0xffff) + 0x41 + (int)local_258) = 0xff;
            *(undefined1 *)((local_244 & 0xffff) + 0x73 + (int)local_258) = 0xff;
            local_258[(local_244 & 0xffff) + 0x2a] = 0;
            local_258[(local_244 & 0xffff) + 0x5c] = 0;
            local_258[(local_244 & 0xffff) + 0x8e] = 0;
            local_244 = CONCAT22(local_244._2_2_,(short)local_244 + 1);
          }
          FUN_004119e3(local_258,(FILE *)local_110);
          if ((char)*param_3 == '\0') {
            FUN_0044ba00(local_258 + 0x7a5,(uint *)**(undefined4 **)(&DAT_0046e870 + param_1 * 4));
          }
          else {
            FUN_0044ba00(local_258 + 0x7a5,param_3);
          }
        }
        _fclose((FILE *)local_110);
      }
    }
  }
  return local_114;
}

