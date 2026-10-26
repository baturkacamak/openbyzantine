// rea decompile_all
// name: FUN_004196a4
// address: 0x4196a4
// body: available
// range: 0x4196a4-0x419a50 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004196a4(int param_1,ushort *param_2,int param_3)

{
  ushort uVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  ushort *local_6c;
  tagRECT local_64;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  ushort *local_44;
  ushort *local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  ushort *local_1c;
  undefined4 local_18;
  undefined4 local_14;
  ushort local_10;
  undefined2 uStack_e;
  ushort local_c;
  undefined2 uStack_a;
  ushort *local_8;
  
  local_64.left = 0;
  local_64.top = 0x14;
  local_64.right = 0x280;
  local_64.bottom = 0x1a4;
  local_54 = 0;
  local_50 = 0x15;
  local_4c = 0x280;
  local_48 = 0x1a5;
  local_2c = 0x140;
  local_28 = 200;
  local_24 = 0x280;
  local_20 = 400;
  local_14 = CONCAT22(local_14._2_2_,0x1f);
  local_18 = 0;
  piVar2 = (int *)FUN_00418040((int)param_2);
  iVar3 = FUN_00416220();
  if (iVar3 == 0) {
    FUN_0041809a(&local_64);
  }
  FUN_0040d9f7();
  if (param_3 == 1) {
    local_14 = CONCAT22(local_14._2_2_,0x7f);
  }
  if (param_1 == 0) {
    FUN_00418c92((int)param_2,&local_64,&local_2c);
  }
  else {
    uVar7 = 0;
    uVar6 = 0x1000000;
    puVar5 = &local_54;
    uVar4 = FUN_00418040(param_1);
    (**(code **)(*piVar2 + 0x14))(piVar2,&local_2c,uVar4,puVar5,uVar6,uVar7);
  }
  local_44 = param_2;
  local_8 = param_2 + 0x140;
  local_40 = param_2 + 0x1f400;
  local_6c = param_2 + 0x1f540;
  for (local_34 = 0; local_34 < 200; local_34 = local_34 + 1) {
    for (local_38 = 0; local_38 < 0x140; local_38 = local_38 + 1) {
      uVar1 = local_6c[local_38];
      _local_c = CONCAT22(uStack_a,uVar1);
      local_44[local_38] = uVar1 >> (DAT_00485c78 & 0x1f) & 0x1f;
      local_8[local_38] = uVar1 >> (DAT_00485c80 & 0x1f) & 0x1f;
      local_40[local_38] = uVar1 & 0x1f;
    }
    local_6c = local_6c + 0x280;
    local_44 = local_44 + 0x280;
    local_8 = local_8 + 0x280;
    local_40 = local_40 + 0x280;
  }
  for (local_3c = 0; local_3c < 4; local_3c = local_3c + 1) {
    local_44 = param_2;
    local_40 = param_2 + 0x1f400;
    local_1c = param_2 + 0x1f540;
    uVar1 = 0x100 - (short)local_14;
    _local_10 = CONCAT22(uStack_e,uVar1);
    local_8 = param_2;
    for (local_34 = 0; local_8 = local_8 + 0x140, local_34 < 200; local_34 = local_34 + 1) {
      for (local_38 = 0; local_38 < 0x140; local_38 = local_38 + 1) {
        local_30 = *(int *)(&DAT_00487710 + (local_14 & 0xffff) * 4);
        local_1c[local_38] =
             (ushort)(((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_8 * 0x800) +
                             local_30) >> 8 & 0x1f) << (DAT_00485c80 & 0x1f)) |
             (ushort)((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_40 * 0x800) +
                            local_30) >> 8) & 0x1f |
             (ushort)(((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_44 * 0x800) +
                             local_30) >> 8) << (DAT_00485c78 & 0x1f));
        local_44 = local_44 + 1;
        local_8 = local_8 + 1;
        local_40 = local_40 + 1;
      }
      local_1c = local_1c + 0x280;
      local_44 = local_44 + 0x140;
      local_40 = local_40 + 0x140;
    }
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_64,piVar2,&local_2c,0x1000000,0);
    if (param_3 == 0) {
      local_14 = CONCAT22(local_14._2_2_,(ushort)local_14 * 2) | 1;
    }
    else {
      local_14 = CONCAT22(local_14._2_2_,(ushort)local_14 >> 1);
    }
  }
  if (param_3 == 1) {
    uVar7 = 0;
    uVar6 = 0x1000000;
    puVar5 = &local_54;
    uVar4 = FUN_00418040(param_1);
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_64,uVar4,puVar5,uVar6,uVar7);
  }
  return;
}

