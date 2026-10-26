// rea decompile_all
// name: FUN_00401ddf
// address: 0x401ddf
// body: available
// range: 0x401ddf-0x401ee5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __fastcall FUN_00401ddf(undefined4 param_1,undefined4 param_2)

{
  float10 fVar1;
  float10 extraout_ST0;
  longlong lVar2;
  
  DAT_004654dc = 0;
  DAT_00468b90 = 0;
  if (DAT_0045c064 != -1) {
    if (DAT_00464878 == 0) {
      DAT_00468b90 = 0;
    }
    else {
      DAT_00468b90 = 0xfffffffa;
    }
    if (DAT_00464880 != 0) {
      FUN_004241e2(DAT_00464880,param_2);
    }
    if (DAT_00462ae8 == 0) {
      fVar1 = (float10)DAT_0045c064 / (float10)DAT_0046487c;
    }
    else {
      __adj_fdiv_m32i();
      fVar1 = extraout_ST0;
    }
    _floor((double)(fVar1 * (float10)360.0 + (float10)0.5));
    lVar2 = __ftol();
    DAT_004654dc = (int)lVar2;
    if (DAT_004654dc < 0) {
      DAT_004654dc = 0;
    }
    else if (0x168 < DAT_004654dc) {
      DAT_004654dc = 0x168;
    }
    DAT_0045c064 = -1;
  }
  return;
}

