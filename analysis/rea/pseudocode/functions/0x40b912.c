// rea decompile_all
// name: FUN_0040b912
// address: 0x40b912
// body: available
// range: 0x40b912-0x40bb7f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __fastcall FUN_0040b912(int param_1)

{
  undefined4 uVar1;
  uint local_68;
  undefined1 local_64 [4];
  uint local_60;
  int local_5c;
  undefined1 local_58 [4];
  uint local_54;
  int local_50;
  undefined4 local_c;
  void *local_8;
  
  if ((DAT_0045da04 == 0) && (*(int *)(param_1 + 0x24) == 0)) {
    DAT_0045da04 = 1;
    (**(code **)(**(int **)(param_1 + 4) + 0x10))
              (*(undefined4 *)(param_1 + 4),param_1 + 0x40,local_58);
    if ((*(uint *)(param_1 + 0x40) < *(uint *)(param_1 + 0x38)) ||
       (*(int *)(param_1 + 0x38) + 0x16f8U <= *(uint *)(param_1 + 0x40))) {
      *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(param_1 + 0x40);
      local_50 = (**(code **)(**(int **)(param_1 + 4) + 0x2c))
                           (*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 0x38),0x16f8,
                            &local_8,&local_60,&local_c,local_64,0);
      if ((local_50 == 0) && (local_60 == 0x16f8)) {
        if (*(int *)(param_1 + 0x28) == 0) {
          local_5c = AVIStreamRead(*(undefined4 *)(param_1 + 8),*(undefined4 *)(param_1 + 0x30),
                                   (int)(0x16f8 / (ulonglong)*(uint *)(param_1 + 0x18)),local_8,
                                   0x16f8,&local_54,&local_68);
          if (local_5c != 0) {
            (**(code **)(**(int **)(param_1 + 4) + 0x4c))
                      (*(undefined4 *)(param_1 + 4),local_8,0,local_c,0);
            DAT_0045da04 = 0;
            DAT_0045f050 = 1;
            return 0xffffffff;
          }
        }
        else {
          _memset(local_8,0,0x16f8);
          local_54 = local_60;
          local_68 = local_60 / *(uint *)(param_1 + 0x18);
        }
        *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0x30) + local_68;
        *(int *)(param_1 + 0x34) = *(int *)(param_1 + 0x34) + 1;
        *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) + 0x16f8;
        if (*(uint *)(param_1 + 0x14) <= *(uint *)(param_1 + 0x38)) {
          *(int *)(param_1 + 0x2c) = *(int *)(param_1 + 0x2c) + 1;
          *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) - *(int *)(param_1 + 0x14);
        }
        (**(code **)(**(int **)(param_1 + 4) + 0x4c))
                  (*(undefined4 *)(param_1 + 4),local_8,local_54,local_c,0);
        if (*(uint *)(param_1 + 0x1c) <= *(uint *)(param_1 + 0x34)) {
          *(undefined4 *)(param_1 + 0x28) = 1;
        }
        DAT_0045da04 = 0;
        uVar1 = 0;
      }
      else {
        if (local_50 == 0) {
          (**(code **)(**(int **)(param_1 + 4) + 0x4c))
                    (*(undefined4 *)(param_1 + 4),local_8,0,local_c,0);
        }
        DAT_0045da04 = 0;
        uVar1 = 0xffffffff;
      }
    }
    else {
      DAT_0045da04 = 0;
      uVar1 = 0xffffffff;
    }
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}

