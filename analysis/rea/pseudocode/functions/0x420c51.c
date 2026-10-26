// rea decompile_all
// name: FUN_00420c51
// address: 0x420c51
// body: available
// range: 0x420c51-0x420dc8 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int FUN_00420c51(void)

{
  int iVar1;
  int local_70;
  int local_6c;
  int local_68;
  int local_64;
  char local_5c [80];
  int local_c;
  FILE *local_8;
  
  local_70 = 0;
  local_8 = (FILE *)FUN_0044bc20(PTR_s_savegame_notebook_000_0045f4e0,&DAT_0045fc34);
  for (local_68 = 0; local_68 < 0x10; local_68 = local_68 + 1) {
    *(undefined4 *)(&DAT_0046c800 + local_68 * 4) = 0xffffffff;
  }
  if (local_8 == (FILE *)0x0) {
    local_70 = 0;
  }
  else {
    while ((local_8->_flag & 0x10) == 0) {
      iVar1 = FUN_0044d340(local_8,(byte *)s_Scene___d_Dialog___d_0045fc38);
      if (iVar1 == 2) {
        local_68 = 0;
        while ((local_68 < 0x20 &&
               (local_c = FUN_0044d340(local_8,(byte *)s__d___d_0045fc50), local_c == 2))) {
          if (local_64 != -1) {
            for (local_6c = 0;
                (local_6c < local_70 && (*(int *)(&DAT_0046c800 + local_6c * 4) != local_64));
                local_6c = local_6c + 1) {
            }
            if ((local_70 == local_6c) && (local_64 != -3)) {
              *(int *)(&DAT_0046c800 + local_70 * 4) = local_64;
              local_70 = local_70 + 1;
            }
          }
          local_68 = local_68 + 1;
        }
      }
      FUN_0044ce20(local_5c,0x50,local_8);
    }
    _fclose(local_8);
  }
  return local_70;
}

