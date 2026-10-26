// rea decompile_all
// name: FUN_0041bdbd
// address: 0x41bdbd
// body: available
// range: 0x41bdbd-0x41beda inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __thiscall FUN_0041bdbd(void *this,int param_1,int param_2,int param_3,int param_4)

{
  if ((((*(int *)((int)this + 0xc) == 0x31347669) && (-0x100 < param_1)) && (param_1 < 0x100)) &&
     (((-0x100 < param_2 && (param_2 < 0x100)) && ((-0x100 < param_3 && (param_3 < 0x100)))))) {
    *(undefined4 *)((int)this + 200) = 0x800000e0;
    if (param_4 == 0) {
      *(int *)((int)this + 0xfc) = param_1;
      *(int *)((int)this + 0x100) = param_2;
      *(int *)((int)this + 0x104) = param_3;
      *(undefined4 *)((int)this + 0xc0) = 2;
      ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0xb4,0x70);
    }
    else {
      *(undefined4 *)((int)this + 0xc0) = 1;
      ICSendMessage(*(undefined4 *)this,0x5001,(int)this + 0xb4,*(undefined4 *)((int)this + 0xb4));
    }
  }
  return;
}

