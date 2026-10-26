// rea decompile_all
// name: FUN_00419a51
// address: 0x419a51
// body: available
// range: 0x419a51-0x419cf5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00419a51(int param_1,void *param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  undefined4 uVar5;
  int iVar6;
  LONG *pLVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  tagRECT local_50;
  LONG local_40;
  LONG local_3c;
  LONG local_38;
  LONG local_34;
  int local_30;
  int local_2c;
  int local_28;
  undefined4 local_24;
  ushort *local_20;
  undefined4 local_1c;
  undefined4 local_18;
  byte local_14;
  ushort local_10;
  undefined2 uStack_e;
  ushort local_c;
  undefined2 uStack_a;
  int local_8;
  
  local_14 = 0x7f;
  local_40 = 0;
  local_3c = 0x14;
  local_38 = 0x280;
  local_34 = 0x1a4;
  uVar5 = FUN_00418040((int)param_2);
  FUN_0042b280(param_2,0x4b000);
  iVar6 = FUN_00416220();
  if (iVar6 == 0) {
    FUN_0041809a(&local_50);
  }
  else {
    local_50.left = local_40;
    local_50.top = local_3c;
    local_50.right = local_38;
    local_50.bottom = local_34;
  }
  for (local_30 = 0; local_30 < 7; local_30 = local_30 + 1) {
    uVar4 = 0x100 - local_14;
    _local_c = CONCAT22(uStack_a,uVar4);
    local_8 = 0x3200;
    if ((local_30 != 0) && (local_30 != 4)) {
      if ((local_30 == 1) || (local_30 == 5)) {
        local_8 = 0x3201;
      }
      else if ((local_30 == 2) || (local_30 == 6)) {
        local_8 = 0x3480;
      }
      else if ((local_30 == 3) || (local_30 == 7)) {
        local_8 = 0x3481;
      }
    }
    local_20 = (ushort *)(local_8 * 2 + (int)param_2);
    for (local_28 = 0; local_28 < 200; local_28 = local_28 + 1) {
      for (local_2c = 0; local_2c < 0x140; local_2c = local_2c + 1) {
        uVar1 = *(ushort *)(param_1 + local_8 * 2);
        _local_10 = CONCAT22(uStack_e,uVar1);
        uVar2 = uVar1 >> (DAT_00485c78 & 0x1f);
        local_1c = CONCAT22(local_1c._2_2_,uVar2) & 0xffff001f;
        uVar3 = uVar1 >> (DAT_00485c80 & 0x1f);
        local_24 = CONCAT22(local_24._2_2_,uVar3) & 0xffff001f;
        local_18 = CONCAT22(local_18._2_2_,uVar1) & 0xffff001f;
        *local_20 = (ushort)((*(uint *)(&DAT_00487710 + (uint)uVar4 * 4 + (uVar3 & 0x1f) * 0x800) >>
                              8 & 0x1f) << (DAT_00485c80 & 0x1f)) |
                    (ushort)((*(uint *)(&DAT_00487710 + (uint)uVar4 * 4 + (uVar2 & 0x1f) * 0x800) >>
                             8) << (DAT_00485c78 & 0x1f)) |
                    (ushort)((uint)*(undefined4 *)
                                    (&DAT_00487710 + (uint)uVar4 * 4 + (uVar1 & 0x1f) * 0x800) >> 8)
                    & 0x1f;
        local_20 = local_20 + 2;
        local_8 = local_8 + 2;
      }
      local_20 = local_20 + 0x280;
      local_8 = local_8 + 0x280;
    }
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_50,uVar5,&local_40,0x1000000,0);
    local_14 = local_14 >> 1;
  }
  uVar9 = 0;
  uVar8 = 0x1000000;
  pLVar7 = &local_40;
  uVar5 = FUN_00418040(param_1);
  (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_50,uVar5,pLVar7,uVar8,uVar9);
  return;
}

