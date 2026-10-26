// rea decompile_all
// name: FUN_0043be09
// address: 0x43be09
// body: available
// range: 0x43be09-0x43c113 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl
FUN_0043be09(int param_1,uint param_2,int param_3,int param_4,int param_5,undefined4 *param_6)

{
  short sVar1;
  DWORD DVar2;
  int iVar3;
  uint local_3c;
  short local_38 [2];
  uint local_34;
  uint local_30;
  short local_2c [2];
  short local_28 [2];
  short local_24 [2];
  uint local_20;
  tagRECT local_1c;
  int local_c;
  undefined4 local_8;
  
  local_1c.left = 0;
  local_1c.top = 0;
  local_1c.right = param_3 + -1;
  local_1c.bottom = param_4 + -1;
  local_3c = 0x15e;
  local_20 = 0;
  local_8 = 0;
  *param_6 = 0;
  for (local_30 = 0; (int)local_30 < 0x15e; local_30 = local_30 + 1) {
    if (*(int *)(&DAT_004649e8 + local_30 * 4) != 0) {
      if ((int)local_30 < (int)local_3c) {
        local_3c = local_30;
      }
      if ((int)local_20 < (int)local_30) {
        local_20 = local_30;
      }
    }
  }
  if ((int)local_20 < (int)local_3c) {
    local_3c = 0;
    local_20 = 1;
  }
  DVar2 = GetTickCount();
  DAT_0047ae58 = DVar2 - _DAT_0046187c;
  if ((0x10 < DAT_0047ae58) || (param_5 != 0)) {
    _DAT_0046187c = DAT_0047ae58;
    for (local_30 = 0; (int)local_30 < 0x15e; local_30 = local_30 + 1) {
      (&DAT_0046caf8)[local_30 * 4] = 0xffff;
    }
    for (local_34 = 0; (int)local_34 < 5; local_34 = local_34 + 1) {
      for (local_30 = local_20; (int)local_3c <= (int)local_30; local_30 = local_30 - 1) {
        local_c = *(int *)(&DAT_004649e8 + local_30 * 4);
        if (((local_c != 0) && (*(byte *)(local_c + 8) == local_34)) &&
           ((*(byte *)(local_c + 9) & 0x20) == 0)) {
          if ((*(byte *)(local_c + 9) & 0x40) == 0) {
            if (*(char *)(local_c + 0x29) != '\0') {
              DAT_00482acd = 1;
            }
            FUN_0043c114(local_30,local_24,local_2c,local_38,local_28,'\0');
            if ((((*(byte *)(local_c + 9) & 0x10) != 0) ||
                ((*(short *)(local_c + 10) == 0 && (*(short *)(local_c + 0xc) == 0)))) &&
               ((iVar3 = FUN_0040241c(local_24,local_2c,local_38[0],local_28[0],&local_1c),
                iVar3 != 0 || (param_5 != 0)))) {
              local_8 = 1;
              sVar1 = local_2c[0] + -0x14;
              local_2c[0] = sVar1;
              FUN_0043b130(local_c,param_1,local_24[0],sVar1,&local_1c.left,param_2,param_5);
              iVar3 = FUN_0040271b(local_24[0],local_38[0]);
              if (iVar3 != 0) {
                *param_6 = 1;
              }
              FUN_0043bb77(local_c,(undefined4 *)0x0);
            }
          }
          else {
            *(byte *)(local_c + 9) = *(byte *)(local_c + 9) | 0x20;
            *(byte *)(local_c + 9) = *(byte *)(local_c + 9) & 0xbf;
            if (*(char *)(local_c + 0x29) != '\0') {
              *(undefined1 *)(local_c + 0x29) = 0;
              DAT_00482acd = 0;
            }
            *(undefined4 *)(local_c + 0x20) = 0;
          }
        }
      }
    }
  }
  return local_8;
}

