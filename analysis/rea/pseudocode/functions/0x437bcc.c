// rea decompile_all
// name: FUN_00437bcc
// address: 0x437bcc
// body: available
// range: 0x437bcc-0x437c71 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00437bcc(int param_1)

{
  char *pcVar1;
  uint in_EAX;
  undefined2 uVar3;
  uint uVar2;
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = 0;
  local_c = 0;
  do {
    uVar3 = (undefined2)(in_EAX >> 0x10);
    if (0x2ed < local_c) {
LAB_00437c64:
      return CONCAT22(uVar3,(undefined2)local_10);
    }
    pcVar1 = *(char **)(param_1 + 0x720 + local_c * 4);
    if ((pcVar1 == (char *)0x0) || (*pcVar1 == -1)) {
      uVar3 = (undefined2)((uint)pcVar1 >> 0x10);
      goto LAB_00437c64;
    }
    uVar2 = FUN_004061e7(*(int *)(param_1 + 0x720 + local_c * 4),0);
    in_EAX = local_10;
    if (local_10 <= (uVar2 & 0xffff)) {
      in_EAX = uVar2 & 0xffff;
    }
    local_c = local_c + 1;
    local_10 = in_EAX;
  } while( true );
}

