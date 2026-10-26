// rea decompile_all
// name: FUN_0043e4b0
// address: 0x43e4b0
// body: available
// range: 0x43e4b0-0x43e55d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0043e4b0(int param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  if (*(int *)(param_1 + 0x10) != 0xcc) {
    (*(code *)**(undefined4 **)(param_1 + 0x17c))(param_1);
    *(undefined4 *)(param_1 + 0x78) = 0;
    *(undefined4 *)(param_1 + 0x10) = 0xcc;
  }
  do {
    if (*(char *)(*(int *)(param_1 + 0x17c) + 8) == '\0') {
      iVar4 = 0xce - (uint)(*(char *)(param_1 + 0x41) == '\0');
      *(int *)(param_1 + 0x10) = iVar4;
      return CONCAT31((int3)((uint)iVar4 >> 8),1);
    }
    puVar1 = (uint *)(param_1 + 0x78);
    while (*puVar1 < *(uint *)(param_1 + 0x60)) {
      if (*(int *)(param_1 + 8) != 0) {
        *(uint *)(*(int *)(param_1 + 8) + 4) = *puVar1;
        *(undefined4 *)(*(int *)(param_1 + 8) + 8) = *(undefined4 *)(param_1 + 0x60);
        (*(code *)**(undefined4 **)(param_1 + 8))(param_1);
      }
      uVar2 = *puVar1;
      uVar3 = (**(code **)(*(int *)(param_1 + 0x180) + 4))(param_1,0,puVar1,0);
      if (*puVar1 == uVar2) {
        return uVar3 & 0xffffff00;
      }
    }
    (**(code **)(*(int *)(param_1 + 0x17c) + 4))(param_1);
    (*(code *)**(undefined4 **)(param_1 + 0x17c))(param_1);
    *puVar1 = 0;
  } while( true );
}

