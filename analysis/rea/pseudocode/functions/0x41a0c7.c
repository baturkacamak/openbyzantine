// rea decompile_all
// name: FUN_0041a0c7
// address: 0x41a0c7
// body: available
// range: 0x41a0c7-0x41a877 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __cdecl FUN_0041a0c7(int *param_1,int param_2,int param_3)

{
  int *piVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  int iVar5;
  HWND hWnd;
  int iVar6;
  tagPOINT *lpPoint;
  tagPOINT local_14c;
  int local_144;
  int local_140;
  void *local_13c;
  ushort local_138;
  undefined2 uStack_136;
  int local_134;
  ushort local_130;
  undefined2 uStack_12e;
  uint local_128;
  int local_124;
  int local_120;
  int local_11c;
  uint local_118;
  int local_114;
  int local_110;
  int local_10c;
  int local_108;
  undefined4 local_104 [4];
  uint local_f4;
  int local_e0;
  int local_98;
  ushort local_94;
  undefined2 uStack_92;
  int local_90;
  uint local_8c;
  int local_88;
  int local_84;
  ushort local_80;
  undefined2 uStack_7e;
  int local_7c;
  undefined4 local_78 [4];
  uint local_68;
  int local_54;
  int local_c;
  int local_8;
  
  local_144 = param_2;
  local_110 = param_2 + 0x4b000;
  local_10c = 0x1a - _DAT_00485c78;
  local_140 = 0xf - _DAT_00485c80;
  local_108 = 4 - DAT_00485c8c;
  local_114 = 0;
  local_8 = 0;
  local_84 = 0x280;
  _memset(local_78,0,0x6c);
  _memset(local_104,0,0x6c);
  local_78[0] = 0x6c;
  local_104[0] = 0x6c;
  if ((param_3 == 2) || (param_3 == 4)) {
    local_88 = 0x1e0;
  }
  else {
    local_88 = 400;
  }
  local_124 = (**(code **)(*param_1 + 100))(param_1,0,local_104,1,0);
  if (local_124 == 0) {
    local_128 = local_f4 >> 1;
    local_13c = (void *)local_e0;
    local_124 = (**(code **)(*DAT_004869d4 + 100))(DAT_004869d4,0,local_78,1,0);
    if (local_124 == 0) {
      local_8c = local_68 >> 1;
      local_98 = local_54;
      if (param_3 == 0) {
        local_7c = 0xa0;
        local_90 = 0x78;
        local_84 = 0x140;
      }
      else if ((param_3 == 1) || (param_3 == 3)) {
        local_7c = 0;
        local_90 = 0x14;
      }
      local_8 = local_128 * local_90 + local_7c;
      local_114 = local_8c * local_90 + local_7c;
      iVar5 = FUN_00416220();
      if (iVar5 == 0) {
        local_14c.x = local_7c;
        local_14c.y = local_90;
        lpPoint = &local_14c;
        hWnd = (HWND)FUN_004161f6();
        ClientToScreen(hWnd,lpPoint);
        local_114 = local_14c.y * local_8c + local_14c.x;
      }
      local_98 = local_98 + local_114 * 2;
      local_13c = (void *)((int)local_13c + local_8 * 2);
      local_c = 0;
      for (local_11c = 0; local_11c < local_88 >> 1; local_11c = local_11c + 1) {
        local_134 = 0;
        for (local_120 = 0; local_120 < 0x140; local_120 = local_120 + 1) {
          if (param_3 == 0) {
            uVar3 = *(ushort *)((int)local_13c + local_120 * 2);
          }
          else {
            uVar3 = *(ushort *)((int)local_13c + local_134 * 2);
          }
          local_118 = (uint)(DAT_00485c7c & uVar3) << ((byte)local_10c & 0x1f) |
                      (uint)(uVar3 & DAT_00485c84) << ((byte)local_108 & 0x1f) |
                      (uint)(DAT_00485c86 & uVar3) << ((byte)local_140 & 0x1f) | 0x2004000;
          if (param_3 == 0) {
            uVar3 = *(ushort *)(local_98 + local_120 * 2);
          }
          else {
            uVar3 = *(ushort *)(local_98 + local_134 * 2);
          }
          _local_80 = CONCAT22(uStack_7e,uVar3);
          _local_138 = CONCAT22(uStack_136,DAT_00485c7c & uVar3);
          _local_130 = CONCAT22(uStack_12e,DAT_00485c86 & uVar3);
          _local_94 = CONCAT22(uStack_92,uVar3 & DAT_00485c84);
          *(uint *)(local_144 + local_c * 4) =
               (uint)(DAT_00485c7c & uVar3) << ((byte)local_10c & 0x1f) |
               (uint)(uVar3 & DAT_00485c84) << ((byte)local_108 & 0x1f) |
               (uint)(DAT_00485c86 & uVar3) << ((byte)local_140 & 0x1f);
          *(int *)(local_110 + local_c * 4) =
               (int)(local_118 - *(int *)(local_144 + local_c * 4)) >> 3;
          local_c = local_c + 1;
          local_134 = local_134 + 2;
        }
        if (param_3 == 0) {
          iVar5 = local_8c * 2;
          iVar6 = local_128 * 2;
        }
        else {
          iVar5 = local_8c * 4;
          iVar6 = local_128 * 4;
        }
        local_98 = local_98 + iVar5;
        local_13c = (void *)((int)local_13c + iVar6);
      }
      (**(code **)(*DAT_004869d4 + 0x80))(DAT_004869d4,0);
      (**(code **)(*param_1 + 0x80))(param_1,0);
      local_134 = 0;
      while ((local_134 < 8 &&
             (local_124 = (**(code **)(*param_1 + 100))(param_1,0,local_104,1,0), local_124 == 0)))
      {
        local_128 = (int)local_f4 >> 1;
        local_8 = local_128 * local_90 + local_7c;
        local_13c = (void *)(local_e0 + local_8 * 2);
        local_c = 0;
        for (local_120 = 0; local_120 < local_88 >> 1; local_120 = local_120 + 1) {
          for (local_11c = 0; local_11c < local_84; local_11c = local_11c + 1) {
            piVar1 = (int *)(local_144 + local_c * 4);
            *piVar1 = *piVar1 + *(int *)(local_110 + local_c * 4);
            local_118 = *(uint *)(local_144 + local_c * 4);
            uVar2 = (ushort)((int)local_118 >> ((byte)local_10c & 0x1f)) & DAT_00485c7c;
            _local_138 = CONCAT22(uStack_136,uVar2);
            uVar3 = (ushort)((int)local_118 >> ((byte)local_140 & 0x1f)) & DAT_00485c86;
            _local_130 = CONCAT22(uStack_12e,uVar3);
            uVar4 = (ushort)((int)local_118 >> ((byte)local_108 & 0x1f)) & DAT_00485c84;
            _local_94 = CONCAT22(uStack_92,uVar4);
            uVar2 = uVar4 | uVar3 | uVar2;
            _local_80 = CONCAT22(uStack_7e,uVar2);
            *(ushort *)((int)local_13c + local_11c * 2) = uVar2;
            if (param_3 != 0) {
              local_11c = local_11c + 1;
              *(ushort *)((int)local_13c + local_11c * 2) = uVar2;
            }
            local_c = local_c + 1;
          }
          if (param_3 == 0) {
            local_13c = (void *)((int)local_13c + local_128 * 2);
          }
          else {
            if ((param_3 == 1) || (param_3 == 2)) {
              _memset((void *)(local_128 * 2 + (int)local_13c),0,0x500);
            }
            else {
              FID_conflict__memcpy((void *)(local_128 * 2 + (int)local_13c),local_13c,0x500);
            }
            local_13c = (void *)((int)local_13c + local_128 * 4);
          }
        }
        (**(code **)(*param_1 + 0x80))(param_1,0);
        if (param_1 == DAT_0048688c) {
          (**(code **)(*DAT_004869d4 + 0x2c))(DAT_004869d4,0,1);
        }
        else {
          FUN_0041842b(DAT_0046e82c,0x4b000);
        }
        local_134 = local_134 + 1;
      }
    }
    else {
      (**(code **)(*param_1 + 0x80))(param_1,0);
    }
  }
  return local_124;
}

