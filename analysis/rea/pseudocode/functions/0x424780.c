// rea decompile_all
// name: FUN_00424780
// address: 0x424780
// body: available
// range: 0x424780-0x424980 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00424780(int param_1,int param_2)

{
  int iVar1;
  float10 fVar2;
  float10 extraout_ST0;
  longlong lVar3;
  char local_20 [3];
  undefined1 local_1d;
  int local_18;
  uint *local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  local_8 = 0;
  FUN_0044c630(local_20,(char *)(*(int *)(param_1 + 0x12dc + param_2 * 4) + 3),3);
  local_1d = 0;
  local_10 = FUN_0040ced0(local_20);
  iVar1 = DAT_0045fdbc;
  if ((local_10 != -1) && (iVar1 = local_10, DAT_0045fdbc == local_10)) {
    FUN_00424c4e(0);
    iVar1 = DAT_0045fdbc;
  }
  DAT_0045fdbc = iVar1;
  local_14 = *(uint **)(param_1 + 0x720 + param_2 * 4);
  iVar1 = __strcmpi((char *)local_14,&DAT_0045fdc0);
  if (iVar1 != 0) {
    local_c = 1;
    local_8 = 1;
  }
  iVar1 = _strcmp((char *)local_14,&DAT_0045fdc8);
  if (iVar1 == 0) {
    local_c = 0;
  }
  iVar1 = FUN_0041d440(6);
  if ((iVar1 == 1) && (local_c != 0)) {
    DAT_0045f054 = 1;
    if (DAT_0045f494 == 0) {
      local_18 = FUN_0040701f(local_14,0x212,4);
      FUN_00424b15(local_14);
    }
    else {
      local_18 = FUN_0040701f(local_14,0x1b8,0);
      FUN_00424cb0((char *)local_14,local_10);
    }
    if (DAT_00462ae8 == 0) {
      fVar2 = (float10)local_18 / (float10)DAT_0045fdb8;
    }
    else {
      __adj_fdiv_m32i();
      fVar2 = extraout_ST0;
    }
    _ceil((double)fVar2);
    lVar3 = __ftol();
    DAT_0045fdac = DAT_0045fdac + (int)lVar3;
    FUN_0040711b((char *)local_14);
  }
  if ((local_10 != -1) && (local_8 != 0)) {
    FUN_0040d2a1(*(uint **)(param_1 + 0x12dc + param_2 * 4),param_1 + 0x1e94);
  }
  iVar1 = FUN_0041d440(3);
  if ((iVar1 == 1) && (local_c != 0)) {
    FUN_004212b4(local_10,param_2);
  }
  return;
}

