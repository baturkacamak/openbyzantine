// rea decompile_all
// name: FUN_00408076
// address: 0x408076
// body: available
// range: 0x408076-0x4083d3 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_00408076(void)

{
  char *pcVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_1c;
  int local_18;
  undefined4 local_c;
  
  local_c = 0;
  iVar3 = (int)DAT_00486a0a;
  iVar4 = (int)DAT_00486a0c;
  iVar5 = FUN_00407f6a();
  if (iVar5 == 0) {
    iVar5 = FUN_00407eef();
    if (iVar5 == 0) {
      iVar5 = FUN_00407df9();
      if (iVar5 == 0) {
        iVar5 = FUN_00407e74();
        if (iVar5 == 0) {
          local_18 = -1;
          for (local_1c = 0; local_1c < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4);
              local_1c = local_1c + 1) {
            if ((((*(int *)(&DAT_00468d00 + local_1c * 0x10) < iVar3) &&
                 (iVar3 < *(int *)(&DAT_00468d08 + local_1c * 0x10))) &&
                (*(int *)(&DAT_00468d04 + local_1c * 0x10) < iVar4)) &&
               (iVar4 < *(int *)(&DAT_00468d0c + local_1c * 0x10))) {
              local_18 = local_1c;
              break;
            }
          }
          if (local_18 != -1) {
            iVar3 = *(int *)(&DAT_00468d78 + (DAT_0045d1bc + local_18) * 4 + DAT_0045d1c4 * 0x578);
            pcVar1 = *(char **)(&DAT_00486f90 + iVar3 * 4);
            if (*pcVar1 == '\x03') {
              *pcVar1 = '\x01';
              FUN_00407b9c(0);
              FUN_00407420(1);
            }
            else if (pcVar1 != DAT_0045d1cc) {
              if (DAT_0045d1c8 == (char *)0x0) {
                FUN_00407ad5(pcVar1);
              }
              else {
                bVar2 = false;
                local_1c = 0;
                while ((local_1c < *(int *)(&DAT_00468cc0 + DAT_0045d1c4 * 4) && (!bVar2))) {
                  if (**(char **)(&DAT_00486f90 +
                                 *(int *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578) * 4)
                      == '\x03') {
                    **(undefined1 **)
                      (&DAT_00486f90 +
                      *(int *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578) * 4) = 1;
                    *(undefined4 *)
                     (&DAT_00468d78 + (DAT_0045d1bc + local_18) * 4 + DAT_0045d1c4 * 0x578) =
                         *(undefined4 *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578);
                    *(int *)(&DAT_00468d78 + local_1c * 4 + DAT_0045d1c4 * 0x578) = iVar3;
                    bVar2 = true;
                  }
                  local_1c = local_1c + 1;
                }
                if (bVar2) {
                  FUN_00407ad5(pcVar1);
                }
              }
            }
          }
        }
        else {
          FUN_00407527(1);
        }
      }
      else {
        FUN_00407527(0);
      }
    }
    else {
      iVar3 = FUN_00407fe5((int)DAT_0045d1c8);
      if (iVar3 != 0) {
        *DAT_0045d1c8 = '\x01';
        FUN_00407b9c(0);
        FUN_00407420(1);
      }
    }
  }
  else {
    iVar3 = FUN_00407fe5((int)DAT_0045d1c8);
    if (iVar3 != 0) {
      if (DAT_0045d1cc != (char *)0x0) {
        *DAT_0045d1cc = '\x01';
      }
      DAT_0045d1cc = DAT_0045d1c8;
      *DAT_0045d1c8 = '\x04';
      FUN_00407b9c(0);
      FUN_00407420(0);
      local_c = 1;
    }
  }
  return local_c;
}

