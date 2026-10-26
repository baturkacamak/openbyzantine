// rea decompile_all
// name: FUN_0040be80
// address: 0x40be80
// body: available
// range: 0x40be80-0x40bf4a inclusive
// range: 0x40bf50-0x40bf63 inclusive
// range: 0x40bf74-0x40bf84 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040be80(int param_1,int param_2)

{
  int iVar1;
  ushort uVar2;
  ushort uVar3;
  ushort *puVar4;
  int local_18;
  int local_10;
  int local_8;
  
  local_10 = 0;
  local_8 = -1;
  do {
    puVar4 = (ushort *)(local_10 * 2 + param_1);
    iVar1 = local_10 + 1;
    uVar3 = *puVar4;
    switch((byte)(*puVar4 >> 0xe)) {
    case 0:
      local_10 = iVar1;
      for (local_18 = 0; iVar1 = local_10, local_18 < (short)(uVar3 & 0x3fff);
          local_18 = local_18 + 1) {
        uVar2 = FUN_004164a0(*(ushort *)(param_1 + local_10 * 2));
        *(ushort *)(param_1 + local_10 * 2) = uVar2;
        local_10 = local_10 + 1;
      }
      break;
    case 1:
      local_8 = local_8 + 1;
      break;
    case 2:
      uVar3 = FUN_004164a0(*(ushort *)(param_1 + iVar1 * 2));
      *(ushort *)(param_1 + iVar1 * 2) = uVar3;
      iVar1 = local_10 + 2;
      break;
    case 3:
    }
    local_10 = iVar1;
  } while (local_8 < param_2);
  return;
}

