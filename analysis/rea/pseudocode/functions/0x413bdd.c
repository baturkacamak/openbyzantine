// rea decompile_all
// name: FUN_00413bdd
// address: 0x413bdd
// body: available
// range: 0x413bdd-0x413d00 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_00413bdd(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int iVar1;
  int *local_8;
  
  local_8 = DAT_00486888;
  if (DAT_0046c4dc != (void *)0x0) {
    if (DAT_0048688c != (int *)0x0) {
      (**(code **)(*DAT_0048688c + 0x1c))(DAT_0048688c,0,0,DAT_004869d4,0,0x10);
      local_8 = DAT_0048688c;
    }
    while (DAT_00460198 != 0) {
      FUN_0041624a();
    }
    iVar1 = FUN_00439a28(DAT_0046c4dc,param_1,param_4,param_2,param_3,param_5);
    if (iVar1 == 0) {
      DAT_0045f060 = 0;
      DAT_0045f050 = 0;
      while (DAT_0045f060 == 0) {
        FUN_0041624a();
        if ((DAT_0045fdac != 0) && (DAT_0045f054 != 0)) {
          FUN_00424981(local_8);
        }
        if (DAT_0045f050 != 0) {
          FUN_00439cce((int)DAT_0046c4dc);
          DAT_0045f050 = 0;
        }
      }
      FUN_00414c3d((byte)DAT_0045f064);
    }
  }
  return;
}

