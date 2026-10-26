// rea decompile_all
// name: FUN_00419d28
// address: 0x419d28
// body: available
// range: 0x419d28-0x41a06a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00419d28(ushort *param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  HWND hWnd;
  ushort *local_68;
  tagRECT local_60;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  ushort *local_40;
  ushort *local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  ushort *local_18;
  undefined4 local_14;
  ushort local_10;
  undefined2 uStack_e;
  ushort local_c;
  undefined2 uStack_a;
  ushort *local_8;
  
  local_60.left = 0;
  local_60.top = 0;
  local_60.right = 0x280;
  local_60.bottom = 0x1e0;
  local_50 = 0;
  local_4c = 0;
  local_48 = 0x280;
  local_44 = 0x1e0;
  local_28 = 0x140;
  local_24 = 0xf0;
  local_20 = 0x280;
  local_1c = 0x1e0;
  local_14 = CONCAT22(local_14._2_2_,0x1f);
  uVar2 = FUN_00418040((int)param_1);
  FUN_0040d9f7();
  iVar3 = FUN_00416220();
  if (iVar3 == 0) {
    hWnd = (HWND)FUN_004161f6();
    GetClientRect(hWnd,&local_60);
    ClientToScreen(hWnd,(LPPOINT)&local_60);
    local_60.right = local_60.left + 0x280;
    local_60.bottom = local_60.top + 0x1e0;
  }
  FUN_00418c92((int)param_1,&local_60,&local_28);
  local_40 = param_1;
  local_8 = param_1 + 0x140;
  local_3c = param_1 + 0x25800;
  local_68 = param_1 + 0x25940;
  for (local_30 = 0; local_30 < 0xf0; local_30 = local_30 + 1) {
    for (local_34 = 0; local_34 < 0x140; local_34 = local_34 + 1) {
      uVar1 = local_68[local_34];
      _local_c = CONCAT22(uStack_a,uVar1);
      local_40[local_34] = uVar1 >> (DAT_00485c78 & 0x1f) & 0x1f;
      local_8[local_34] = uVar1 >> (DAT_00485c80 & 0x1f) & 0x1f;
      local_3c[local_34] = uVar1 & 0x1f;
    }
    local_68 = local_68 + 0x280;
    local_40 = local_40 + 0x280;
    local_8 = local_8 + 0x280;
    local_3c = local_3c + 0x280;
  }
  for (local_38 = 0; local_38 < 4; local_38 = local_38 + 1) {
    local_40 = param_1;
    local_3c = param_1 + 0x25800;
    local_18 = param_1 + 0x25940;
    uVar1 = 0x100 - (short)local_14;
    _local_10 = CONCAT22(uStack_e,uVar1);
    local_8 = param_1;
    for (local_30 = 0; local_8 = local_8 + 0x140, local_30 < 0xf0; local_30 = local_30 + 1) {
      for (local_34 = 0; local_34 < 0x140; local_34 = local_34 + 1) {
        local_2c = *(int *)(&DAT_00487710 + (local_14 & 0xffff) * 4);
        local_18[local_34] =
             (ushort)(((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_8 * 0x800) +
                             local_2c) >> 8 & 0x1f) << (DAT_00485c80 & 0x1f)) |
             (ushort)((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_3c * 0x800) +
                            local_2c) >> 8) & 0x1f |
             (ushort)(((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_40 * 0x800) +
                             local_2c) >> 8) << (DAT_00485c78 & 0x1f));
        local_40 = local_40 + 1;
        local_8 = local_8 + 1;
        local_3c = local_3c + 1;
      }
      local_18 = local_18 + 0x280;
      local_40 = local_40 + 0x140;
      local_3c = local_3c + 0x140;
    }
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_60,uVar2,&local_28,0x1000000,0);
    local_14 = CONCAT22(local_14._2_2_,(short)local_14 * 2) | 1;
  }
  return;
}

