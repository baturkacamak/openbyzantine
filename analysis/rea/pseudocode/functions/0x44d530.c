// rea decompile_all
// name: entry
// address: 0x44d530
// body: available
// range: 0x44d530-0x44d674 inclusive
// range: 0x44d69c-0x44d6b5 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void entry(void)

{
  byte bVar1;
  DWORD DVar2;
  int iVar3;
  HMODULE pHVar4;
  uint uVar5;
  byte *pbVar6;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uVar7;
  _STARTUPINFOA local_74;
  undefined1 *local_1c;
  undefined4 local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_14 = *unaff_FS_OFFSET;
  local_8 = 0xffffffff;
  puStack_c = &DAT_0045b7e8;
  puStack_10 = &LAB_00453f64;
  *unaff_FS_OFFSET = &local_14;
  local_1c = &stack0xffffff80;
  DVar2 = GetVersion();
  _DAT_00462c8c = DVar2 >> 8 & 0xff;
  DAT_00462c80 = DVar2 >> 0x10;
  _DAT_00462c88 = DVar2 & 0xff;
  _DAT_00462c84 = _DAT_00462c88 * 0x100 + _DAT_00462c8c;
  FUN_0044faf0();
  local_8 = 0;
  FUN_00451100();
  FUN_00453f50();
  DAT_004a7824 = (byte *)GetCommandLineA();
  DAT_00463184 = FUN_00453ae0();
  if ((DAT_00463184 == (LPSTR)0x0) || (DAT_004a7824 == (byte *)0x0)) {
    FUN_0044c000(0xffffffff);
  }
  __setargv();
  FUN_00453580();
  FUN_0044bfd0();
  bVar1 = *DAT_004a7824;
  pbVar6 = DAT_004a7824;
  if (bVar1 == 0x22) {
    pbVar6 = DAT_004a7824 + 1;
    if (*pbVar6 != 0x22) {
      do {
        if (*pbVar6 == 0) break;
        iVar3 = __ismbblead((uint)*pbVar6);
        if (iVar3 != 0) {
          pbVar6 = pbVar6 + 1;
        }
        pbVar6 = pbVar6 + 1;
      } while (*pbVar6 != 0x22);
      if (*pbVar6 != 0x22) goto LAB_0044d627;
    }
    pbVar6 = pbVar6 + 1;
  }
  else {
    while (0x20 < bVar1) {
      bVar1 = pbVar6[1];
      pbVar6 = pbVar6 + 1;
    }
  }
LAB_0044d627:
  bVar1 = *pbVar6;
  while ((bVar1 != 0 && (*pbVar6 < 0x21))) {
    pbVar6 = pbVar6 + 1;
    bVar1 = *pbVar6;
  }
  local_74.dwFlags = 0;
  GetStartupInfoA(&local_74);
  uVar7 = 0;
  pHVar4 = GetModuleHandleA((LPCSTR)0x0);
  uVar5 = FUN_0041535a(pHVar4,uVar7,(char *)pbVar6);
  FUN_0044c000(uVar5);
  *unaff_FS_OFFSET = local_14;
  return;
}

