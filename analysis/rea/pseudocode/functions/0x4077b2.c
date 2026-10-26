// rea decompile_all
// name: FUN_004077b2
// address: 0x4077b2
// body: available
// range: 0x4077b2-0x4078f3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004077b2(uint param_1,uint param_2)

{
  char *pcVar1;
  int iVar2;
  int local_10;
  
  pcVar1 = *(char **)(&DAT_00486f90 + (param_2 & 0xffff) * 4);
  if ((pcVar1 != (char *)0x0) && (*pcVar1 != '\x01')) {
    *pcVar1 = '\x01';
    if (pcVar1 == DAT_0045d1c8) {
      FUN_00407b9c(0);
    }
    else {
      iVar2 = FUN_00407254((int)pcVar1);
      if (iVar2 == 0) {
        local_10 = *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4);
        *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) =
             *(int *)(&DAT_00468cc0 + (param_1 & 0xffff) * 4) + 1;
        for (; 0 < local_10; local_10 = local_10 + -1) {
          *(undefined4 *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578 + local_10 * 4) =
               *(undefined4 *)(&DAT_00468d74 + local_10 * 4 + (param_1 & 0xffff) * 0x578);
        }
        *(uint *)(&DAT_00468d78 + (param_1 & 0xffff) * 0x578) = param_2 & 0xffff;
      }
    }
    if ((param_1 & 0xffff) == DAT_0045d1c4) {
      FUN_00407420(1);
    }
  }
  return;
}

