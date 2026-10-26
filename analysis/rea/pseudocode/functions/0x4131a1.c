// rea decompile_all
// name: FUN_004131a1
// address: 0x4131a1
// body: available
// range: 0x4131a1-0x413728 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_004131a1(void)

{
  ushort uVar1;
  
  for (uVar1 = 0; uVar1 < 0x15e; uVar1 = uVar1 + 1) {
    if (*(int *)(&DAT_0047dd70 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0047dd70 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_0047dd70 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_0047dd70 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_0047dd70 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00484e80 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_00484e80 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00484e80 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_00484e80 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00484e80 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_0046e870 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0046e870 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_0046e870 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_0046e870 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_0046e870 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_0047a8e0 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0047a8e0 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_0047a8e0 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_0047a8e0 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_0047a8e0 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00483080 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_00483080 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00483080 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_00483080 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00483080 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_004848b0 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_004848b0 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_004848b0 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_004848b0 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_004848b0 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_0046e280 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0046e280 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_0046e280 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_0046e280 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_0046e280 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00483d90 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_00483d90 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00483d90 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_00483d90 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00483d90 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00485400 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_00485400 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00485400 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_00485400 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00485400 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_0047a2b0 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_0047a2b0 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_0047a2b0 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_0047a2b0 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_0047a2b0 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00484330 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40((LPVOID)**(undefined4 **)(&DAT_00484330 + (uint)uVar1 * 4));
      FUN_0044ac40(*(LPVOID *)(*(int *)(&DAT_00484330 + (uint)uVar1 * 4) + 4));
      FUN_0044ac40(*(LPVOID *)(&DAT_00484330 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00484330 + (uint)uVar1 * 4) = 0;
    }
    if (*(int *)(&DAT_00483640 + (uint)uVar1 * 4) != 0) {
      FUN_0044ac40(*(LPVOID *)(&DAT_00483640 + (uint)uVar1 * 4));
      *(undefined4 *)(&DAT_00483640 + (uint)uVar1 * 4) = 0;
    }
  }
  return;
}

