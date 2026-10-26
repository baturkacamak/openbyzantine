// rea decompile_all
// name: FUN_00405c19
// address: 0x405c19
// body: available
// range: 0x405c19-0x405db1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00405c19(int param_1,ushort *param_2)

{
  ushort *puVar1;
  int iVar2;
  size_t _Size;
  int local_2c;
  void *local_28;
  int local_24;
  void *local_18;
  
  puVar1 = param_2;
  if (*(char *)(param_1 + 0x10) == '\x10') {
    for (local_24 = 0; local_24 < (int)(uint)*(ushort *)(param_1 + 0xe); local_24 = local_24 + 1) {
      for (local_2c = 0; local_2c < (int)(uint)*(ushort *)(param_1 + 0xc); local_2c = local_2c + 1)
      {
        *param_2 = ((*param_2 & 0x7c00) >> 10) << (DAT_00485c78 & 0x1f) |
                   ((*param_2 & 0x3e0) >> 5) << (DAT_00485c80 & 0x1f) | *param_2 & 0x1f;
        param_2 = param_2 + 1;
      }
    }
  }
  local_28 = *(void **)(param_1 + 0x14);
  iVar2 = (*(ushort *)(param_1 + 0xe) - 1) * (uint)*(byte *)(param_1 + 0x10) *
          (uint)*(ushort *)(param_1 + 0xc);
  local_18 = (void *)((int)puVar1 + ((int)(iVar2 + (iVar2 >> 0x1f & 7U)) >> 3));
  _Size = ((int)(uint)*(byte *)(param_1 + 0x10) >> 3) * (uint)*(ushort *)(param_1 + 0xc);
  for (local_24 = 0; local_24 < (int)(uint)*(ushort *)(param_1 + 0xe); local_24 = local_24 + 1) {
    FID_conflict__memcpy(local_28,local_18,_Size);
    local_28 = (void *)((int)local_28 + _Size);
    local_18 = (void *)((int)local_18 - _Size);
  }
  return;
}

