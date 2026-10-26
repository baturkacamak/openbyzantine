// rea decompile_all
// name: FUN_00417c33
// address: 0x417c33
// body: available
// range: 0x417c33-0x417d61 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00417c33(float param_1,float param_2,int *param_3)

{
  float10 fVar1;
  char local_18 [20];
  
  _sscanf((char *)(**(int **)(&DAT_00483d90 + DAT_00480330 * 4) + 2),&DAT_0045f2b0);
  fVar1 = (float10)FUN_0044d0b4(0,0x40000000);
  _sprintf(local_18,s_X___04_3f_0045f2b4,(double)(param_1 * 1.012));
  FUN_004063e0(local_18,0x26e,0xe0,1,param_3,0,2,1);
  _sprintf(local_18,s_Y___04_3f_0045f2c0,(double)param_2);
  FUN_004063e0(local_18,0x26e,0xf6,1,param_3,0,2,1);
  _sprintf(local_18,s_Z___04_3f_0045f2cc,(double)(float)(fVar1 + (float10)2.0));
  FUN_004063e0(local_18,0x26e,0x10c,1,param_3,0,2,1);
  return 0;
}

