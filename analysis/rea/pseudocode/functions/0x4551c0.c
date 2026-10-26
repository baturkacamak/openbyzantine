// rea decompile_all
// name: FUN_004551c0
// address: 0x4551c0
// body: available
// range: 0x4551c0-0x455277 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int FUN_004551c0(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = -1;
  iVar6 = 0;
  iVar4 = 0;
  piVar3 = &DAT_004a7720;
  do {
    puVar1 = (undefined4 *)*piVar3;
    if (puVar1 == (undefined4 *)0x0) {
      puVar1 = (undefined4 *)FUN_0044c2f0(0x100);
      if (puVar1 != (undefined4 *)0x0) {
        DAT_004a7710 = DAT_004a7710 + 0x20;
        (&DAT_004a7720)[iVar6] = puVar1;
        if (puVar1 < puVar1 + 0x40) {
          do {
            *(undefined1 *)(puVar1 + 1) = 0;
            puVar2 = puVar1 + 2;
            *puVar1 = 0xffffffff;
            *(undefined1 *)((int)puVar1 + 5) = 10;
            puVar1 = puVar2;
          } while (puVar2 < (undefined4 *)((&DAT_004a7720)[iVar6] + 0x100));
        }
        iVar5 = iVar6 << 5;
      }
      return iVar5;
    }
    puVar2 = puVar1 + 0x40;
    for (; puVar1 < puVar2; puVar1 = puVar1 + 2) {
      if ((*(byte *)(puVar1 + 1) & 1) == 0) {
        *puVar1 = 0xffffffff;
        iVar5 = ((int)puVar1 - *piVar3 >> 3) + iVar4;
        break;
      }
    }
    if (iVar5 != -1) {
      return iVar5;
    }
    iVar4 = iVar4 + 0x20;
    piVar3 = piVar3 + 1;
    iVar6 = iVar6 + 1;
    if ((int *)0x4a781f < piVar3) {
      return -1;
    }
  } while( true );
}

