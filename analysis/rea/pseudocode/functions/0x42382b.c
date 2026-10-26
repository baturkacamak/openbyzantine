// rea decompile_all
// name: FUN_0042382b
// address: 0x42382b
// body: available
// range: 0x42382b-0x423976 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __fastcall FUN_0042382b(int param_1)

{
  void *_Dst;
  undefined4 local_c;
  
  _Dst = (void *)FUN_0044ac60(*(int *)(param_1 + 0x38) * 2);
  for (local_c = 0; local_c < *(int *)(param_1 + 0x20); local_c = local_c + 1) {
    FID_conflict__memcpy
              (_Dst,(void *)(*(int *)(param_1 + 0x24) * 2 + *(int *)(param_1 + 0x1c) * local_c * 2 +
                            *(int *)(param_1 + 0x34)),*(int *)(param_1 + 0x38) * 2);
    FID_conflict__memcpy
              ((void *)(*(int *)(param_1 + 0x1c) * local_c * 2 + *(int *)(param_1 + 0x34)),_Dst,
               *(int *)(param_1 + 0x38) * 2);
  }
  for (local_c = 0; local_c < *(int *)(param_1 + 0x20); local_c = local_c + 1) {
    FID_conflict__memcpy
              (_Dst,(void *)(*(int *)(param_1 + 0x38) * 2 + *(int *)(param_1 + 0x1c) * local_c * 2 +
                            *(int *)(param_1 + 0x34)),*(int *)(param_1 + 0x38) * 2);
    FID_conflict__memcpy
              ((void *)(*(int *)(param_1 + 0x24) * 2 + *(int *)(param_1 + 0x1c) * local_c * 2 +
                        *(int *)(param_1 + 0x38) * 2 + *(int *)(param_1 + 0x34)),_Dst,
               *(int *)(param_1 + 0x38) * 2);
  }
  FUN_0044a820(_Dst);
  return;
}

