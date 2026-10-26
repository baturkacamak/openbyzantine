// rea decompile_all
// name: FUN_00415a4e
// address: 0x415a4e
// body: available
// range: 0x415a4e-0x415bcb inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00415a4e(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  
  iVar2 = FUN_00415bcc(param_1,param_2,param_3);
  if (iVar2 == 0) {
    uVar3 = 0;
  }
  else {
    iVar2 = FUN_00415c42();
    if (iVar2 == 0) {
      FUN_0040c5f0();
      iVar2 = FUN_0040dd7f();
      if (iVar2 == 0) {
        iVar2 = FUN_0043db30();
        if (iVar2 == 0) {
          uVar3 = 0;
        }
        else {
          if (DAT_0048688c == 0) {
            bVar1 = FUN_00413ac0(DAT_0046c4e8,DAT_00486a04,DAT_004869d4,DAT_00486888,DAT_00461934,0)
            ;
            if (CONCAT31(extraout_var_00,bVar1) != 0) {
              return 0;
            }
          }
          else {
            bVar1 = FUN_00413ac0(DAT_0046c4e8,DAT_00486a04,DAT_004869d4,DAT_0048688c,DAT_00461934,1)
            ;
            if (CONCAT31(extraout_var,bVar1) != 0) {
              return 0;
            }
          }
          FUN_004163f3();
          iVar2 = FUN_0042a9d0();
          if (iVar2 == 0) {
            uVar3 = 0;
          }
          else {
            iVar2 = FUN_00407160();
            if (iVar2 == 0) {
              iVar2 = FUN_00408e3f();
              if (iVar2 == 0) {
                FUN_004163ab();
                iVar2 = FUN_00401256();
                if (iVar2 == 0) {
                  FUN_00406cfe();
                  FUN_0044d060(0xbeef);
                  uVar3 = 1;
                }
                else {
                  uVar3 = 0;
                }
              }
              else {
                uVar3 = 0;
              }
            }
            else {
              uVar3 = 0;
            }
          }
        }
      }
      else {
        uVar3 = 0;
      }
    }
    else {
      FUN_00414be7(iVar2);
      uVar3 = 0;
    }
  }
  return uVar3;
}

