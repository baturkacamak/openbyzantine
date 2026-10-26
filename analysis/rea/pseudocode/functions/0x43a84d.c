// rea decompile_all
// name: FUN_0043a84d
// address: 0x43a84d
// body: available
// range: 0x43a84d-0x43aa16 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __fastcall FUN_0043a84d(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  LONG LVar4;
  undefined4 local_8;
  
  iVar1 = FUN_0040b263(param_1[1]);
  if (iVar1 == 0) {
    do {
      if ((int)param_1[0x20] <= param_1[0x1e] + 1) goto LAB_0043aa00;
      iVar1 = FUN_0040b445(param_1[1]);
      if (iVar1 + -0x10 < -0x10) {
        InterlockedExchange(param_1 + 8,1);
      }
      else {
        iVar1 = AVIStreamTimeToSample(param_1[10],iVar1 + -0x10);
        if ((int)param_1[0x1e] < iVar1) {
          iVar3 = FUN_0043ad1e(param_1,iVar1,&local_8);
          if (((iVar3 == 0) && (iVar3 = FUN_0043adce(param_1,0,local_8), iVar3 == 0)) &&
             (iVar3 = FUN_0043af73(param_1,1), iVar3 == 0)) {
            param_1[0x21] = param_1[0x21] + 1;
          }
          param_1[0x1d] = iVar1;
        }
        param_1[0x1e] = param_1[0x1d];
      }
      LVar4 = InterlockedExchange(param_1 + 8,0);
    } while (LVar4 == 0);
    InterlockedExchange(param_1 + 8,1);
LAB_0043aa00:
    FUN_0043ac17(param_1);
    uVar2 = 0;
  }
  else {
    FUN_0043ac17(param_1);
    uVar2 = 0xfffffff9;
  }
  return uVar2;
}

