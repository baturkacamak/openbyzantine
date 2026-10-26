// rea decompile_all
// name: FUN_00428cbd
// address: 0x428cbd
// body: available
// range: 0x428cbd-0x428f8b inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl
FUN_00428cbd(int param_1,int param_2,int param_3,uint param_4,int param_5,undefined4 param_6,
            undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  short extraout_AX;
  uint uVar1;
  int iVar2;
  char *pcVar3;
  DWORD DVar4;
  DWORD local_3c;
  uint *local_38;
  uint local_34 [4];
  uint local_24;
  int local_20;
  int local_1c;
  int local_18;
  undefined4 local_14;
  int local_10 [3];
  
  local_34[3] = 0;
  local_1c = *(int *)(&DAT_00486220 + param_1 * 4);
  local_24 = CONCAT22(local_24._2_2_,0xffff);
  local_18 = 0;
  local_38 = (uint *)0x0;
  local_34[0] = param_4;
  local_34[1] = param_6;
  local_34[2] = param_8;
  local_10[0] = param_5;
  local_10[1] = param_7;
  local_10[2] = param_9;
  local_20 = 0;
  if (local_1c == 0) {
    uVar1 = 0xffff;
  }
  else if ((*(int *)(local_1c + 0x18) == 0) && (iVar2 = FUN_0041d440(2), iVar2 == 0)) {
    uVar1 = 0xffff;
  }
  else {
    FUN_00427de1(param_1,0);
    local_3c = timeGetTime();
    if (param_3 != -1) {
      if (DAT_0045f214 != (uint *)0x0) {
        pcVar3 = FUN_0044c580(DAT_0045f214);
        local_38 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar3 + 1));
        FUN_0044ba00(local_38,DAT_0045f214);
      }
      FUN_00417a17(*(uint **)(&DAT_00483640 + param_3 * 4),1);
    }
    if (DAT_0045d1d0 != 0) {
      DAT_0045d1d0 = 0;
      local_18 = 1;
    }
    DAT_0045f050 = 0;
    while (local_34[3] == 0) {
      if ((local_34[local_20] != 0) &&
         (DVar4 = timeGetTime(), iVar2 = local_20, local_34[local_20] < DVar4 - local_3c)) {
        if (local_10[local_20] != -1) {
          local_20 = local_20 + 1;
          FUN_00431ec2((ushort)local_10[iVar2],0x66,0);
        }
        local_3c = timeGetTime();
      }
      if (DAT_0045c060 == 0) {
        FUN_0043b216(&local_14,0,0);
      }
      else {
        FUN_004013ae();
        if (DAT_004859cd == '\0') {
          FUN_00401184(0);
        }
      }
      if (param_2 == 0) {
        FUN_0043c8d2(&DAT_0047a230,1);
        local_24 = CONCAT22(local_24._2_2_,extraout_AX);
        if ((extraout_AX == 2) || (extraout_AX == 0)) {
          DAT_004848a8 = 0;
          FUN_004281b1(param_1);
        }
        else if (DAT_0045f050 != 0) {
          FUN_004281b1(param_1);
        }
      }
      FUN_0041624a();
      local_34[3] = FUN_00428143(param_1);
    }
    if ((param_3 != -1) && (FUN_00417a17((uint *)0x0,2), local_38 != (uint *)0x0)) {
      FUN_00417a17(local_38,0);
      FUN_0044ac40(local_38);
    }
    if ((local_18 != 0) && ((local_24 & 0xffff) != 0)) {
      DAT_0045d1d0 = 1;
    }
    uVar1 = local_24 & 0xffff;
  }
  return uVar1;
}

