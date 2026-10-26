// rea decompile_all
// name: __except1
// address: 0x44ff80
// body: available
// range: 0x44ff80-0x45003a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __except1
   
   Library: Visual Studio 1998 Release */

float10 __cdecl
__except1(uint param_1,int param_2,uint param_3,undefined4 param_4,uint param_5,undefined4 param_6,
         uint param_7)

{
  bool bVar1;
  undefined3 extraout_var;
  int _Value;
  float10 fVar2;
  uint local_58 [14];
  uint local_20;
  
  bVar1 = __handle_exc(param_1,(double *)&param_5,param_7);
  if (CONCAT31(extraout_var,bVar1) == 0) {
    local_20 = local_20 & 0xfffffffe;
    FUN_00450130(local_58,&param_7,param_1,param_2,&param_3,&param_5);
  }
  _Value = __errcode(param_1);
  if ((DAT_00463ebc == 0) && (_Value != 0)) {
    fVar2 = __umatherr(_Value,param_2);
    return fVar2;
  }
  __set_errno(_Value);
  FUN_004508f0();
  return (float10)(double)CONCAT44(param_6,param_5);
}

