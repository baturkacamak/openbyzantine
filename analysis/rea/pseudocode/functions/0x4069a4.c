// rea decompile_all
// name: FUN_004069a4
// address: 0x4069a4
// body: available
// range: 0x4069a4-0x406cbe inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl
FUN_004069a4(byte param_1,int param_2,int param_3,int param_4,int param_5,int param_6,int param_7)

{
  short sVar1;
  byte bVar2;
  uint uVar3;
  int iVar4;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  uint local_48;
  uint local_3c;
  uint local_34;
  uint local_30;
  int local_2c;
  int local_28;
  int local_24;
  uint local_20;
  int local_1c;
  int local_18;
  uint local_14;
  int local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  if (0x7f < param_1) {
    param_1 = FUN_00405bf4();
  }
  if (param_1 < 0x20) {
    uVar3 = 0;
  }
  else {
    bVar2 = param_1 - 0x20;
    local_34 = (uint)*(ushort *)((&DAT_00487518)[param_4 * 0x12] + (uint)bVar2 * 8);
    local_48 = (uint)*(ushort *)((&DAT_00487518)[param_4 * 0x12] + 2 + (uint)bVar2 * 8);
    uVar3 = (uint)*(ushort *)((&DAT_00487518)[param_4 * 0x12] + 4 + (uint)bVar2 * 8);
    local_3c = (uint)*(ushort *)((&DAT_00487518)[param_4 * 0x12] + 6 + (uint)bVar2 * 8);
    if (param_3 < 0) {
      local_48 = local_48 - param_3;
      local_3c = local_3c + param_3;
      param_3 = 0;
    }
    else {
      iVar4 = local_3c + param_3 + -0x1e0;
      if (0 < iVar4) {
        local_3c = local_3c - iVar4;
      }
    }
    if ((int)local_3c < 1) {
      uVar3 = 0;
    }
    else {
      local_20 = (uint)*(ushort *)(*(int *)(&DAT_00487510 + param_4 * 0x48) + 10);
      if (*(int *)(&DAT_00487538 + param_4 * 0x48) == 0) {
        sVar1 = *(short *)(&DAT_00487554 + param_4 * 0x48);
        local_60 = (param_7 * param_3 + param_2) * 2 + param_5;
        local_64 = (local_20 * local_48 + local_34) * 2 + *(int *)(&DAT_00487520 + param_4 * 0x48);
        for (local_58 = 0; local_58 < (int)local_3c; local_58 = local_58 + 1) {
          for (local_5c = 0; local_5c < (int)uVar3; local_5c = local_5c + 1) {
            if (*(short *)(local_64 + local_5c * 2) != sVar1) {
              *(undefined2 *)(local_60 + local_5c * 2) = *(undefined2 *)(local_64 + local_5c * 2);
            }
          }
          local_60 = local_60 + param_7 * 2;
          local_64 = local_64 + local_20 * 2;
        }
      }
      else {
        local_24 = param_7;
        local_30 = local_48;
        local_2c = uVar3 + local_34;
        local_28 = local_3c + local_48;
        local_1c = param_2;
        local_18 = param_3;
        local_14 = (uint)(byte)(&DAT_00487534)[param_4 * 0x48];
        local_10 = param_5;
        local_c = *(undefined4 *)(&DAT_00487538 + param_4 * 0x48);
        if (*(int *)(&DAT_00487550 + param_4 * 0x48) == 0) {
          local_8 = 0;
        }
        else {
          local_8 = *(undefined4 *)(&DAT_00487550 + param_4 * 0x48);
        }
        if (param_6 == 0) {
          FUN_004067e2((int *)&local_34,
                       CONCAT22((short)((uint)(param_4 * 8) >> 0x10),
                                *(undefined2 *)(&DAT_00487554 + param_4 * 0x48)));
        }
        else {
          FUN_004067e2((int *)&local_34,
                       CONCAT22((short)((uint)(param_4 * 8) >> 0x10),
                                *(undefined2 *)(&DAT_00487556 + param_4 * 0x48)));
        }
      }
    }
  }
  return uVar3;
}

