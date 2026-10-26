// rea decompile_all
// name: FUN_0042bebb
// address: 0x42bebb
// body: available
// range: 0x42bebb-0x42c1b7 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0042bebb(ushort *param_1,ushort *param_2)

{
  int *piVar1;
  ushort uVar2;
  int iVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  LPVOID pvVar7;
  LPVOID pvVar8;
  int local_34;
  int local_2c;
  int local_28;
  ushort *local_20;
  ushort *local_18;
  int local_10;
  int local_8;
  
  pvVar7 = (LPVOID)FUN_0044c2f0(0xfa000);
  pvVar8 = (LPVOID)FUN_0044c2f0(0xfa000);
  local_8 = 0;
  local_18 = param_1;
  local_20 = param_2;
  for (local_28 = 0; local_28 < 400; local_28 = local_28 + 1) {
    for (local_2c = 0; local_2c < 0x280; local_2c = local_2c + 1) {
      uVar6 = *local_20;
      uVar4 = DAT_00485c7c & uVar6;
      uVar5 = DAT_00485c86 & uVar6;
      uVar6 = DAT_00485c84 & uVar6;
      uVar2 = *local_18;
      *(uint *)((int)pvVar7 + local_8 * 4) =
           (uint)(DAT_00485c84 & uVar2) << 5 | (uint)(DAT_00485c7c & uVar2) << 0xf |
           (uint)(DAT_00485c86 & uVar2) << 10;
      *(uint *)((int)pvVar8 + local_8 * 4) =
           (int)(((uint)uVar6 << 5 | (uint)uVar4 << 0xf | (uint)uVar5 << 10 | 0x2004000) -
                *(int *)((int)pvVar7 + local_8 * 4)) >> 3 & 0xff9ff3ff;
      local_8 = local_8 + 1;
      local_18 = local_18 + 1;
      local_20 = local_20 + 1;
    }
  }
  for (local_34 = 0; local_34 < 8; local_34 = local_34 + 1) {
    local_8 = 0;
    local_10 = 0;
    for (local_2c = 0; local_2c < 400; local_2c = local_2c + 1) {
      for (local_28 = 0; local_28 < 0x280; local_28 = local_28 + 1) {
        piVar1 = (int *)((int)pvVar7 + local_10 * 4);
        *piVar1 = *piVar1 + *(int *)((int)pvVar8 + local_10 * 4);
        iVar3 = *(int *)((int)pvVar7 + local_10 * 4);
        uVar6 = (ushort)(iVar3 >> 5) & DAT_00485c84 | (ushort)(iVar3 >> 10) & DAT_00485c86 |
                (ushort)(iVar3 >> 0xf) & DAT_00485c7c;
        param_1[local_8] = uVar6;
        param_1[local_8 + 1] = uVar6;
        local_8 = local_8 + 2;
        local_10 = local_10 + 1;
      }
    }
    FUN_0041842b((int)param_1,0x3e800);
  }
  FUN_0042b2db(param_1,param_2,0x3e800);
  FUN_0041842b((int)param_1,0x3e800);
  FUN_0044ac40(pvVar7);
  FUN_0044ac40(pvVar8);
  return;
}

