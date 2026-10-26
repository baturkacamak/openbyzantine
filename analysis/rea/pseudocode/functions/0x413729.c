// rea decompile_all
// name: FUN_00413729
// address: 0x413729
// body: available
// range: 0x413729-0x413801 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00413729(int param_1)

{
  ushort uVar1;
  LPVOID pvVar2;
  LPVOID pvVar3;
  undefined4 local_8;
  
  for (uVar1 = 0; uVar1 < 0x32; uVar1 = uVar1 + 1) {
    local_8 = *(LPVOID *)(param_1 + (uint)uVar1 * 4);
    while (pvVar3 = local_8, local_8 != (LPVOID)0x0) {
      FUN_00413802(*(LPVOID *)((int)local_8 + 4));
      *(undefined4 *)((int)local_8 + 4) = 0;
      local_8 = *(LPVOID *)((int)local_8 + 0xc);
      while (local_8 != (LPVOID)0x0) {
        FUN_00413802(*(LPVOID *)((int)local_8 + 4));
        *(undefined4 *)((int)local_8 + 4) = 0;
        pvVar2 = *(LPVOID *)((int)local_8 + 0xc);
        FUN_0044ac40(local_8);
        local_8 = pvVar2;
      }
      local_8 = *(LPVOID *)((int)pvVar3 + 8);
      FUN_0044ac40(pvVar3);
    }
  }
  return;
}

