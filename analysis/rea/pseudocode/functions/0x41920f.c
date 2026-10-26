// rea decompile_all
// name: FUN_0041920f
// address: 0x41920f
// body: available
// range: 0x41920f-0x4196a3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0041920f(ushort *param_1,int param_2,ushort *param_3)

{
  ushort uVar1;
  int *piVar2;
  int iVar3;
  DWORD DVar4;
  ushort *local_90;
  ushort *local_8c;
  tagRECT local_84;
  int local_74 [8];
  ushort *local_54;
  ushort *local_50;
  ushort *local_4c;
  int local_48;
  ushort *local_44;
  ushort *local_40;
  DWORD local_3c;
  int local_38;
  int local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  ushort *local_1c;
  undefined4 local_18;
  uint local_14;
  ushort local_10;
  undefined2 uStack_e;
  ushort local_c;
  undefined2 uStack_a;
  ushort *local_8;
  
  local_84.left = 0;
  local_84.top = 0x14;
  local_84.right = 0x280;
  local_84.bottom = 0x1a4;
  local_74[0] = 0;
  local_74[1] = 200;
  local_74[2] = 0x140;
  local_74[3] = 400;
  local_74[4] = 0;
  local_74[5] = 0x14;
  local_74[6] = 0x280;
  local_74[7] = 0x1a4;
  local_30 = 0x140;
  local_2c = 200;
  local_28 = 0x280;
  local_24 = 400;
  local_14 = CONCAT22(local_14._2_2_,2);
  local_18 = FUN_00418040((int)param_1);
  local_20 = FUN_00418040(param_2);
  piVar2 = (int *)FUN_00418040((int)param_3);
  local_3c = timeGetTime();
  iVar3 = FUN_00416220();
  if (iVar3 == 0) {
    FUN_0041809a(&local_84);
  }
  (**(code **)(*piVar2 + 0x14))(piVar2,&local_30,local_18,local_74 + 4,0x1000000,0);
  local_44 = param_1;
  local_90 = param_1 + 64000;
  local_40 = param_1 + 0x1f400;
  local_54 = param_1 + 0x2ee00;
  local_8 = param_3;
  local_50 = param_3 + 64000;
  local_4c = (ushort *)(param_2 + 0x6400);
  local_8c = param_3 + 0x1f540;
  for (local_34 = 0; local_34 < 200; local_34 = local_34 + 1) {
    for (local_38 = 0; local_38 < 0x140; local_38 = local_38 + 1) {
      uVar1 = local_8c[local_38];
      local_54[local_38] = uVar1 >> (DAT_00485c78 & 0x1f) & 0x1f;
      local_8[local_38] = uVar1 >> (DAT_00485c80 & 0x1f) & 0x1f;
      local_50[local_38] = uVar1 & 0x1f;
      uVar1 = *local_4c;
      _local_c = CONCAT22(uStack_a,uVar1);
      local_44[local_38] = uVar1 >> (DAT_00485c78 & 0x1f) & 0x1f;
      local_90[local_38] = uVar1 >> (DAT_00485c80 & 0x1f) & 0x1f;
      local_40[local_38] = uVar1 & 0x1f;
      local_4c = local_4c + 2;
    }
    local_8c = local_8c + 0x280;
    local_4c = local_4c + 0x280;
    local_44 = local_44 + 0x140;
    local_90 = local_90 + 0x140;
    local_40 = local_40 + 0x140;
    local_54 = local_54 + 0x140;
    local_8 = local_8 + 0x140;
    local_50 = local_50 + 0x140;
  }
  for (local_48 = 0; local_48 < 4; local_48 = local_48 + 1) {
    local_44 = param_1;
    local_90 = param_1 + 64000;
    local_40 = param_1 + 0x1f400;
    local_54 = param_1 + 0x2ee00;
    local_8 = param_3;
    local_50 = param_3 + 64000;
    local_1c = param_3 + 0x1f400;
    uVar1 = 0x100 - (short)local_14;
    _local_10 = CONCAT22(uStack_e,uVar1);
    for (local_34 = 0; local_34 < 200; local_34 = local_34 + 1) {
      for (local_38 = 0; local_38 < 0x140; local_38 = local_38 + 1) {
        local_1c[local_38] =
             (ushort)(((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_8 * 0x800) +
                             *(int *)(&DAT_00487710 +
                                     (local_14 & 0xffff) * 4 + (uint)*local_90 * 0x800)) >> 8 & 0x1f
                      ) << (DAT_00485c80 & 0x1f)) |
             (ushort)(((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_54 * 0x800) +
                             *(int *)(&DAT_00487710 +
                                     (local_14 & 0xffff) * 4 + (uint)*local_44 * 0x800)) >> 8) <<
                     (DAT_00485c78 & 0x1f)) |
             (ushort)((uint)(*(int *)(&DAT_00487710 + (uint)uVar1 * 4 + (uint)*local_50 * 0x800) +
                            *(int *)(&DAT_00487710 +
                                    (local_14 & 0xffff) * 4 + (uint)*local_40 * 0x800)) >> 8) & 0x1f
        ;
        local_44 = local_44 + 1;
        local_90 = local_90 + 1;
        local_40 = local_40 + 1;
        local_54 = local_54 + 1;
        local_8 = local_8 + 1;
        local_50 = local_50 + 1;
      }
      local_1c = local_1c + 0x280;
    }
    FUN_00418e2c(local_74,1);
    do {
      DVar4 = timeGetTime();
    } while (DVar4 < local_3c + 0x3c);
    local_3c = timeGetTime();
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_84,piVar2,local_74,0x1000000,0);
    local_14 = CONCAT22(local_14._2_2_,(short)local_14 << 2);
  }
  return;
}

