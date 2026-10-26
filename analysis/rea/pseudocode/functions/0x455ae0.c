// rea decompile_all
// name: __87except
// address: 0x455ae0
// body: available
// range: 0x455ae0-0x455bde inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __87except
   
   Library: Visual Studio 1998 Release */

void __cdecl __87except(int param_1,int *param_2,ushort *param_3)

{
  bool bVar1;
  undefined3 extraout_var;
  int iVar2;
  uint uVar3;
  uint local_5c;
  uint local_58 [10];
  int local_30;
  int local_2c;
  uint local_20;
  
  local_5c = (uint)*param_3;
  switch(*param_2) {
  case 1:
  case 5:
    uVar3 = 8;
    break;
  case 2:
    uVar3 = 4;
    break;
  case 3:
    uVar3 = 0x11;
    break;
  case 4:
    uVar3 = 0x12;
    break;
  case 7:
    *param_2 = 1;
  default:
    uVar3 = 0;
    break;
  case 8:
    uVar3 = 0x10;
  }
  if (uVar3 != 0) {
    bVar1 = __handle_exc(uVar3,(double *)(param_2 + 6),local_5c);
    if (CONCAT31(extraout_var,bVar1) == 0) {
      if (((param_1 == 0x10) || (param_1 == 0x16)) || (param_1 == 0x1d)) {
        local_2c = param_2[5];
        local_20 = local_20 & 0xffffffe3 | 3;
        local_30 = param_2[4];
      }
      else {
        local_20 = local_20 & 0xfffffffe;
      }
      FUN_00450130(local_58,&local_5c,uVar3,param_1,(uint *)(param_2 + 2),(uint *)(param_2 + 6));
    }
  }
  FUN_004508f0();
  iVar2 = 0;
  if ((*param_2 != 8) && (DAT_00463ebc == 0)) {
    iVar2 = FUN_00455900();
  }
  if (iVar2 == 0) {
    __set_errno(*param_2);
  }
  return;
}

