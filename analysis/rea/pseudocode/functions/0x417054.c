// rea decompile_all
// name: FUN_00417054
// address: 0x417054
// body: available
// range: 0x417054-0x417313 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00417054(void)

{
  DWORD DVar1;
  undefined4 local_4c;
  undefined4 local_48;
  int local_44;
  undefined4 local_40;
  int *local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  undefined4 local_24;
  undefined4 local_20;
  int local_1c;
  LONG local_18;
  tagRECT local_14;
  
  local_14.left = 0;
  local_14.top = 0;
  local_14.right = DAT_0046c530;
  local_14.bottom = DAT_0046c518;
  local_24 = 0x110;
  local_20 = 0;
  local_1c = DAT_0046c530 + 0x110;
  local_18 = DAT_0046c518;
  if (DAT_0045f21c != 0) {
    if ((DAT_00476338 == 0xb) && (DAT_0045f208 != 0)) {
      local_2c = 1;
    }
    else {
      local_2c = 0;
    }
    if ((DAT_00476338 == 0xb) && (DAT_0045f20c != 0)) {
      local_28 = 1;
    }
    else {
      local_28 = 0;
    }
    if (((local_2c != 0) || (local_28 != 0)) || (DAT_0045c060 == 0)) {
      DVar1 = timeGetTime();
      if (200 < DVar1 - _DAT_0045f228) {
        DAT_0045f224 = (uint)(DAT_0045f224 == 0);
        _DAT_0045f228 = timeGetTime();
      }
      if ((local_2c == 0) || (DAT_0045f224 == 0)) {
        (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0x110,0x14,DAT_0045f218,&local_24,1);
      }
      else {
        (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0x110,0x14,DAT_0045f21c,&local_14,1);
      }
      if ((local_28 == 0) || (DAT_0045f224 == 0)) {
        local_4c = 0x110;
        local_48 = 0x17c;
        local_44 = DAT_0046c530 + 0x110;
        local_40 = 400;
        (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0x110,400,DAT_0045f218,&local_4c,1);
      }
      else {
        if (DAT_0045f210 == 0) {
          local_30 = 2;
        }
        else {
          local_30 = 0;
        }
        local_3c = (int *)(&DAT_0046c520)[local_30];
        if (local_3c != (int *)0x0) {
          local_34 = (int)*(short *)(local_3c[1] + 6);
          local_38 = *(short *)(local_3c[1] + 8) + 0x14;
          SetRect(&local_14,local_34,local_38,*(short *)(local_3c[1] + 2) + local_34,
                  *(short *)(local_3c[1] + 4) + local_38);
          FUN_0040bbb0((short)local_34,(short)local_38,(int)*(short *)(local_3c[1] + 4),0x280);
          FUN_0040bbfd(*local_3c,DAT_0046e82c,(short)local_34,(short)local_38,&local_14.left,0);
        }
      }
    }
  }
  return;
}

