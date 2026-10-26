// rea decompile_all
// name: FUN_0043688b
// address: 0x43688b
// body: available
// range: 0x43688b-0x436b00 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0043688b(int *param_1,uint param_2)

{
  ushort *puVar1;
  uint uVar2;
  uint local_1ac [52];
  ushort *local_dc;
  undefined2 local_d8;
  uint local_d4 [52];
  
  local_d8 = 0xffff;
  DAT_00483d82 = 0;
  if (*(char *)((int)param_1 + 10) == '\x01') {
    uVar2 = FUN_00436b01(param_1,param_2 & 0xffff);
  }
  else {
    FUN_00437ca2();
    FUN_004365fa();
    do {
      local_dc = (ushort *)param_1[*(ushort *)(param_1 + 2) + 0x131];
      *(short *)(param_1 + 2) = (short)param_1[2] + 1;
      if (local_dc != (ushort *)0x0) break;
    } while ((short)param_1[2] != 0x32);
    if ((local_dc == (ushort *)0x0) || ((short)param_1[2] == 0x32)) {
      *(undefined1 *)((int)param_1 + 0xe) = 1;
      uVar2 = CONCAT22((short)((uint)param_1 >> 0x10),local_d8);
    }
    else {
      *(undefined1 *)((int)param_1 + 10) = 1;
      *(short *)(param_1 + 2) = (short)param_1[2] + -1;
      if (param_1[*(ushort *)(param_1 + 2) + 0xf2] != 0) {
        local_dc = (ushort *)param_1[*(ushort *)(param_1 + 2) + 0xf2];
      }
      FUN_0040aa42(local_d4,(uint)*(ushort *)(param_1 + 2));
      FUN_0044ba08(local_d4,(uint *)&DAT_0046142c);
      FUN_0040aa42(local_1ac,(uint)*(byte *)(*(ushort *)(param_1 + 2) + 0x490 + (int)param_1));
      FUN_0044ba08(local_d4,local_1ac);
      FUN_0040a7e5(local_d4,param_1,(uint)*local_dc);
      puVar1 = local_dc;
      for (local_dc = *(ushort **)(local_dc + 6); local_dc != (ushort *)0x0;
          local_dc = *(ushort **)(local_dc + 6)) {
        FUN_0043340a(param_1,*(byte **)(local_dc + 2));
        if (DAT_0047a8d8 != '\0') {
          DAT_0047a8d8 = '\0';
        }
        DAT_00483d82 = 1;
        FUN_0040a7e5(local_d4,param_1,(uint)*local_dc);
      }
      FUN_0043340a(param_1,*(byte **)(puVar1 + 2));
      if (DAT_0047a8d8 != '\0') {
        DAT_0047a8d8 = '\0';
      }
      DAT_00483d82 = 1;
      if (*(char *)((int)param_1 + 10) == '\0') {
        param_1 = (int *)FUN_00437c72();
      }
      uVar2 = CONCAT22((short)((uint)param_1 >> 0x10),local_d8);
    }
  }
  return uVar2;
}

