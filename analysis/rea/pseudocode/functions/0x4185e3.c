// rea decompile_all
// name: FUN_004185e3
// address: 0x4185e3
// body: available
// range: 0x4185e3-0x41877f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_004185e3(void)

{
  undefined4 uVar1;
  undefined2 *puVar2;
  int iVar3;
  DWORD DVar4;
  bool bVar5;
  tagRECT local_38;
  undefined2 *local_28;
  DWORD local_24;
  int local_20;
  uint local_1c;
  int local_18 [4];
  int *local_8;
  
  local_38.left = 0;
  local_38.top = 0x14;
  local_38.right = 0x280;
  local_38.bottom = 0x1a4;
  local_18[0] = 0;
  local_18[1] = 0x14;
  local_18[2] = 0x280;
  local_18[3] = 0x1a4;
  local_8 = local_18;
  local_24 = timeGetTime();
  uVar1 = FUN_00418040(DAT_0047a8a4);
  local_28 = (undefined2 *)(DAT_0047a8a4 + 0x6400);
  puVar2 = (undefined2 *)(DAT_0046e82c + 0x6400);
  bVar5 = DAT_0045dbd0 != 0;
  if (bVar5) {
    DAT_0045dbd0 = 0;
  }
  local_1c = (uint)bVar5;
  iVar3 = FUN_00416220();
  if (iVar3 == 0) {
    FUN_0041809a(&local_38);
  }
  for (local_20 = 0; local_20 < 0x1b; local_20 = local_20 + 1) {
    while( true ) {
      DVar4 = timeGetTime();
      if (local_24 + 0xd <= DVar4) break;
      FUN_0041624a();
    }
    local_24 = timeGetTime();
    if (local_20 < 0xc) {
      local_8[1] = local_8[1] + 3;
      local_8[3] = local_8[3] + -3;
      *local_8 = *local_8 + 3;
      local_8[2] = local_8[2] + -3;
    }
    else if (local_20 == 0xc) {
      local_8[1] = local_8[1] + -0x24;
      local_8[3] = local_8[3] + 0x24;
      *local_8 = *local_8 + -0x24;
      local_8[2] = local_8[2] + 0x24;
    }
    FUN_00418570(local_28,puVar2,local_20);
    (**(code **)(*DAT_004869d4 + 0x14))(DAT_004869d4,&local_38,uVar1,local_8,0x1000000,0);
  }
  if (local_1c != 0) {
    DAT_0045dbd0 = 1;
  }
  return;
}

