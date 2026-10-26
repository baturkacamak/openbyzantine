// rea decompile_all
// name: FUN_0043bb77
// address: 0x43bb77
// body: available
// range: 0x43bb77-0x43be08 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0043bb77(int param_1,undefined4 *param_2)

{
  if ((*(byte *)(param_1 + 9) & 4) == 0) {
    if (*(short *)(param_1 + 6) == 0) {
      *(undefined2 *)(param_1 + 6) = *(undefined2 *)(param_1 + 4);
      *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x1c);
      if ((*(byte *)(param_1 + 9) & 2) == 0) {
        if (*(int *)(*(int *)(param_1 + 0x1c) + 4) == *(int *)(param_1 + 0x18)) {
          if (*(char *)(param_1 + 0x29) != '\0') {
            DAT_00482acd = 0;
            *(undefined1 *)(param_1 + 0x29) = 0;
          }
          if ((*(int *)(param_1 + 0x2c) != -1) && (param_2 != (undefined4 *)0x0)) {
            *param_2 = *(undefined4 *)(&DAT_004654e0 + *(int *)(param_1 + 0x2c) * 4);
            *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
          }
          if ((*(byte *)(param_1 + 9) & 8) != 0) {
            *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) | 4;
          }
          if ((*(byte *)(param_1 + 9) & 1) == 0) {
            if ((*(byte *)(param_1 + 9) & 4) == 0) {
              *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4);
            }
          }
          else {
            *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 8);
            *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) | 2;
          }
        }
        else {
          *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4);
        }
      }
      else if (*(int *)(*(int *)(param_1 + 0x1c) + 0x14) == 0) {
        if (*(char *)(param_1 + 0x29) != '\0') {
          DAT_00482acd = 0;
          *(undefined1 *)(param_1 + 0x29) = 0;
        }
        if ((*(int *)(param_1 + 0x2c) != -1) && (param_2 != (undefined4 *)0x0)) {
          *param_2 = *(undefined4 *)(&DAT_004654e0 + *(int *)(param_1 + 0x2c) * 4);
          *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
        }
        if ((*(byte *)(param_1 + 9) & 8) != 0) {
          *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) | 4;
        }
        if ((*(byte *)(param_1 + 9) & 1) == 0) {
          if ((*(byte *)(param_1 + 9) & 4) == 0) {
            *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 8);
          }
        }
        else {
          *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 4);
          *(byte *)(param_1 + 9) = *(byte *)(param_1 + 9) & 0xfd;
        }
      }
      else {
        *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)(param_1 + 0x1c) + 8);
      }
    }
    else {
      *(short *)(param_1 + 6) = *(short *)(param_1 + 6) - (short)_DAT_004859fc;
      if (0xfff0 < *(ushort *)(param_1 + 6)) {
        *(undefined2 *)(param_1 + 6) = 0;
      }
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x1c);
  }
  return;
}

