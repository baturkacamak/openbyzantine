// rea decompile_all
// name: FUN_0042aa1a
// address: 0x42aa1a
// body: available
// range: 0x42aa1a-0x42ad6e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl
FUN_0042aa1a(int *param_1,int param_2,int param_3,int param_4,uint param_5,int param_6)

{
  byte *pbVar1;
  int iVar2;
  ushort uVar3;
  undefined4 uVar4;
  int iVar5;
  int local_e0;
  int local_dc;
  void *local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  int local_b4;
  int local_a0;
  undefined1 local_58 [4];
  byte local_54;
  byte local_53;
  byte local_52;
  byte local_50;
  byte local_4f;
  byte local_4e;
  int local_4c;
  int local_48;
  void *local_40;
  size_t local_3c;
  int local_38;
  uint local_34;
  undefined4 local_30 [3];
  uint local_24;
  undefined1 local_8;
  
  _memset(&local_c4,0,0x6c);
  _memset(local_30,0,0x20);
  local_30[0] = 0x20;
  local_c4 = 0x6c;
  local_c0 = 0xe;
  local_4c = (**(code **)(*param_1 + 0x54))(param_1,local_30);
  local_34 = local_24;
  if ((((local_24 == 0x18) || (local_24 == 0x10)) && (param_5 == 8)) ||
     (((local_24 == 8 && ((param_5 == 0x18 || (param_5 == 0x10)))) || (local_24 == 0x20)))) {
    uVar4 = 0;
  }
  else if (((local_24 == 0x10) && (param_5 == 0x18)) &&
          (iVar5 = FUN_0042ad6f(param_1,local_58), iVar5 == 0)) {
    uVar4 = 0;
  }
  else {
    local_c0 = 0xe;
    local_4c = (**(code **)(*param_1 + 100))(param_1,0,&local_c4,1,0);
    if (local_4c == 0) {
      (**(code **)(*param_1 + 0x80))(param_1,0);
      local_c8 = (void *)(local_a0 + local_b4 * param_6);
      local_40 = (void *)((param_4 + -1) * param_3 * (param_5 >> 3) + param_2);
      local_3c = param_5 * param_3 >> 3;
      if (param_5 == local_34) {
        for (local_48 = 0; local_48 < param_4; local_48 = local_48 + 1) {
          FID_conflict__memcpy(local_c8,local_40,local_3c);
          local_c8 = (void *)((int)local_c8 + local_b4);
          local_40 = (void *)((int)local_40 - local_3c);
        }
      }
      else if ((local_34 == 0x10) && (param_5 == 0x18)) {
        for (local_48 = 0; local_48 < param_4; local_48 = local_48 + 1) {
          local_e0 = 0;
          local_dc = 0;
          for (local_38 = 0; local_38 < param_3; local_38 = local_38 + 1) {
            pbVar1 = (byte *)(local_e0 + (int)local_40);
            iVar5 = local_e0 + 1;
            iVar2 = local_e0 + 2;
            local_e0 = local_e0 + 3;
            uVar3 = (ushort)(((int)(uint)*(byte *)(iVar2 + (int)local_40) >> (local_52 & 0x1f)) <<
                            (local_4e & 0x1f)) |
                    (ushort)(((int)(uint)*(byte *)(iVar5 + (int)local_40) >> (local_53 & 0x1f)) <<
                            (local_4f & 0x1f)) |
                    (ushort)(((int)(uint)*pbVar1 >> (local_54 & 0x1f)) << (local_50 & 0x1f));
            local_8 = (undefined1)uVar3;
            *(undefined1 *)(local_dc + (int)local_c8) = local_8;
            *(char *)(local_dc + 1 + (int)local_c8) = (char)(uVar3 >> 8);
            local_dc = local_dc + 2;
          }
          local_c8 = (void *)((int)local_c8 + local_b4);
          local_40 = (void *)((int)local_40 - local_3c);
        }
      }
      uVar4 = 1;
    }
    else {
      uVar4 = 0;
    }
  }
  return uVar4;
}

