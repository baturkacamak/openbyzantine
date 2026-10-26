// rea decompile_all
// name: FUN_00431ec2
// address: 0x431ec2
// body: available
// range: 0x431ec2-0x4320c9 inclusive
// range: 0x4320cf-0x4320e6 inclusive
// range: 0x432103-0x432132 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00431ec2(ushort param_1,int param_2,int param_3)

{
  bool bVar1;
  undefined3 extraout_var;
  
  FUN_00418040((int)DAT_0046e82c);
  FUN_0042b29f((int)DAT_00477344,0x20,0x4b000);
  DAT_0048253e = param_1;
  bVar1 = FUN_0041045b((int)DAT_0046e82c,(uint)param_1,param_2);
  if (CONCAT31(extraout_var,bVar1) == 0) {
    if (((param_2 != 0x65) && (param_2 != 0x68)) &&
       (FUN_0042b2db(DAT_0047a8a4,DAT_0046e82c,0x4b000), param_2 == 4)) {
      param_2 = 100;
    }
    DAT_0045c060 = 0;
    FUN_0043b216((undefined4 *)0x0,1,param_3);
    FUN_0040d9f7();
    switch(param_2) {
    case 100:
    case 0x69:
      FUN_0041842b((int)DAT_0046e82c,0x3e800);
      break;
    case 0x65:
      FUN_00418bcf((int)DAT_0047a8a4);
      FUN_0042be07(DAT_0047a8a4,DAT_0046e82c,(int)DAT_00477344,2);
      FUN_0041045b((int)DAT_0047a8a4,(uint)DAT_0048253e,param_2);
      break;
    case 0x66:
      FUN_00418bcf((int)DAT_00477344);
      FUN_0042bce1((int)DAT_00477344,(int)DAT_0046e82c);
      break;
    case 0x67:
      FUN_00419cf6(0,(int)DAT_0046e82c,DAT_00477344);
      break;
    case 0x68:
      FUN_00418bcf((int)DAT_0047a8a4);
      FUN_00418eb2((int)DAT_0047a8a4,(int)DAT_0046e82c,DAT_00477344);
      FUN_0041045b((int)DAT_0047a8a4,(uint)DAT_0048253e,param_2);
      break;
    case 0x6a:
      FUN_00419a51((int)DAT_0046e82c,DAT_00477344);
    }
    FUN_004075fe(1);
    FUN_0042b29f((int)DAT_0046e82c,0,0x3200);
    FUN_0040da1f();
    DAT_004859cd = 0;
  }
  else {
    FUN_0040ac43();
    FUN_0040ac38();
  }
  return;
}

