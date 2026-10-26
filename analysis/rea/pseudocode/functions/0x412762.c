// rea decompile_all
// name: FUN_00412762
// address: 0x412762
// body: available
// range: 0x412762-0x412a3b inclusive
// range: 0x412a41-0x412a5c inclusive
// range: 0x412ac6-0x412ae9 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined2 __cdecl FUN_00412762(undefined4 *param_1,FILE *param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  uint local_20;
  byte *local_1c;
  byte *local_14;
  byte local_10;
  byte local_f;
  uint local_c;
  undefined2 local_8;
  
  local_8 = 0;
  local_20 = 0;
  local_14 = (byte *)0x0;
  local_1c = (byte *)0x0;
  __fread_lk((char *)&local_c,2,1,param_2);
  do {
    if ((local_c & 0xffff) <= local_20) {
LAB_00412ad4:
      *param_1 = local_14;
      return local_8;
    }
    if (local_20 == 0) {
      local_1c = (byte *)FUN_0044c2f0(0x30);
      local_14 = local_1c;
    }
    else {
      uVar2 = FUN_0044c2f0(0x30);
      *(undefined4 *)(local_1c + 0x2c) = uVar2;
      local_1c = *(byte **)(local_1c + 0x2c);
    }
    *local_1c = 0;
    for (uVar1 = 0; uVar1 < 8; uVar1 = uVar1 + 1) {
      local_1c[uVar1 + 1] = 0;
      (local_1c + (uint)uVar1 * 4 + 10)[0] = 0xff;
      (local_1c + (uint)uVar1 * 4 + 10)[1] = 0;
      (local_1c + (uint)uVar1 * 4 + 0xc)[0] = 0xff;
      (local_1c + (uint)uVar1 * 4 + 0xc)[1] = 0;
    }
    local_1c[0x2c] = 0;
    local_1c[0x2d] = 0;
    local_1c[0x2e] = 0;
    local_1c[0x2f] = 0;
    __fread_lk((char *)local_1c,1,1,param_2);
    if ((*local_1c < 0x32) || (0xe1 < *local_1c)) {
      FUN_0040abb4();
      local_8 = 1;
      goto LAB_00412ad4;
    }
    for (uVar1 = 0; uVar1 < (byte)(&DAT_00460466)[*local_1c]; uVar1 = uVar1 + 1) {
      __fread_lk((char *)(local_1c + uVar1 + 1),1,1,param_2);
      switch(local_1c[uVar1 + 1]) {
      case 0:
        break;
      case 1:
      case 4:
        __fread_lk((char *)(local_1c + (uint)uVar1 * 4 + 10),1,1,param_2);
        break;
      case 2:
        __fread_lk((char *)&local_10,2,1,param_2);
        *(ushort *)(local_1c + (uint)uVar1 * 4 + 10) = (ushort)local_10;
        *(ushort *)(local_1c + (uint)uVar1 * 4 + 0xc) = (ushort)local_f;
        break;
      case 8:
      case 0x10:
      case 0x18:
      case 0x40:
        __fread_lk((char *)(local_1c + (uint)uVar1 * 4 + 10),1,1,param_2);
        __fread_lk((char *)(local_1c + (uint)uVar1 * 4 + 0xc),1,1,param_2);
        break;
      case 0x20:
        __fread_lk((char *)&local_10,2,1,param_2);
        *(ushort *)(local_1c + (uint)uVar1 * 4 + 10) = (ushort)local_10;
        *(ushort *)(local_1c + (uint)uVar1 * 4 + 0xc) = (ushort)local_f;
        __fread_lk((char *)&local_10,2,1,param_2);
      }
    }
    local_20 = (uint)(ushort)((short)local_20 + 1);
  } while( true );
}

