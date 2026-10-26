// rea decompile_all
// name: FUN_00418780
// address: 0x418780
// body: available
// range: 0x418780-0x418939 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00418780(int param_1,int param_2,int param_3)

{
  undefined2 *puVar1;
  int iVar2;
  bool bVar3;
  tagRECT local_4c;
  int *local_3c;
  undefined2 *local_38;
  undefined4 local_34;
  int local_30;
  uint local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  int local_18 [4];
  int *local_8;
  
  local_4c.left = 0;
  local_4c.top = 0x14;
  local_4c.right = 0x280;
  local_4c.bottom = 0x1a4;
  local_28 = 0;
  local_24 = 0x14;
  local_20 = 0x280;
  local_1c = 0x1a4;
  local_18[0] = 0;
  local_18[1] = 0x14;
  local_18[2] = 0x280;
  local_18[3] = 0x1a4;
  local_8 = local_18;
  local_34 = FUN_00418040(param_1);
  local_3c = (int *)FUN_00418040(param_3);
  local_38 = (undefined2 *)(param_3 + 0x6400);
  puVar1 = (undefined2 *)(param_2 + 0x6400);
  bVar3 = DAT_0045dbd0 != 0;
  if (bVar3) {
    DAT_0045dbd0 = 0;
  }
  local_2c = (uint)bVar3;
  iVar2 = FUN_00416220();
  if (iVar2 == 0) {
    FUN_0041809a(&local_4c);
  }
  for (local_30 = 0; local_30 < 0x1b; local_30 = local_30 + 1) {
    if (local_30 < 0xe) {
      local_8[1] = local_8[1] + 3;
      local_8[3] = local_8[3] + -3;
      *local_8 = *local_8 + 3;
      local_8[2] = local_8[2] + -3;
      (**(code **)(*local_3c + 0x14))(local_3c,&local_28,local_34,local_8,0x1000000,0);
      local_30 = local_30 + 1;
      FUN_00418598(local_38,puVar1,local_30);
    }
    else {
      FUN_00418570(local_38,puVar1,local_30);
      local_30 = local_30 + 1;
      FUN_00418570(local_38,puVar1,local_30);
    }
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_4c,local_3c,&local_28,0x1000000,0);
  }
  if (local_2c != 0) {
    DAT_0045dbd0 = 1;
  }
  return;
}

