// rea decompile_all
// name: FUN_004119e3
// address: 0x4119e3
// body: available
// range: 0x4119e3-0x411b4a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_004119e3(undefined4 *param_1,FILE *param_2)

{
  uint uVar1;
  short sVar2;
  ushort uVar3;
  uint uVar4;
  byte local_10 [8];
  uint local_8;
  
  local_8 = local_8 & 0xffff0000;
  sVar2 = FUN_00412762(param_1,param_2);
  local_8._0_2_ = sVar2 + (short)local_8;
  sVar2 = FUN_00412762(param_1 + 1,param_2);
  local_8._0_2_ = sVar2 + (short)local_8;
  __fread_lk((char *)local_10,1,1,param_2);
  sVar2 = FUN_00412aea((int)(param_1 + 0x131),local_10[0],param_2);
  local_8 = CONCAT22(local_8._2_2_,sVar2 + (short)local_8);
  __fread_lk((char *)local_10,1,1,param_2);
  uVar3 = FUN_00412aea((int)(param_1 + 0x163),local_10[0],param_2);
  uVar4 = (uint)uVar3 + (local_8 & 0xffff);
  local_8 = uVar4 & 0xffff;
  uVar1 = local_8;
  local_8._0_2_ = (short)uVar4;
  if (uVar1 == 0) {
    *(undefined1 *)((int)param_1 + 0xe) = 0;
    *(undefined1 *)(param_1 + 3) = 0;
    *(undefined1 *)((int)param_1 + 0xd) = 0xff;
    *(undefined1 *)((int)param_1 + 0xb) = 0xff;
    for (uVar3 = 0; uVar4 = (uint)uVar3, uVar4 < 0x32; uVar3 = uVar3 + 1) {
      param_1[uVar3 + 0xf2] = param_1[uVar3 + 0x131];
      *(undefined1 *)(uVar3 + 0x490 + (int)param_1) = 0;
    }
  }
  return CONCAT22((short)(uVar4 >> 0x10),(short)local_8);
}

