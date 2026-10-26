// rea decompile_all
// name: FUN_0041fe46
// address: 0x41fe46
// body: available
// range: 0x41fe46-0x42007a inclusive
// range: 0x420080-0x420093 inclusive
// range: 0x4200ac-0x4200ba inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0041fe46(undefined4 param_1)

{
  int local_8;
  
  for (local_8 = 0; local_8 < 0x28; local_8 = local_8 + 1) {
    (&DAT_0046c5d0)[local_8] = 0;
  }
  switch(param_1) {
  case 0:
    DAT_0046c5ee = 1;
    DAT_0046c5f3 = 1;
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
    }
    DAT_0046c5d6 = 1;
    DAT_0046c5d7 = 1;
    DAT_0046c5d9 = 1;
    DAT_0046c5db = 1;
    DAT_0046c5da = 1;
    DAT_0046c5d8 = 1;
    DAT_0046c5dc = 0;
    for (local_8 = 0; local_8 < 7; local_8 = local_8 + 1) {
      if (*(int *)(&DAT_0045f528 + local_8 * 4) == 0) {
        FUN_0041f94e(local_8 + 6);
      }
    }
    break;
  case 1:
    DAT_0046c5d3 = 1;
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
      DAT_0046c5d0 = 1;
    }
    DAT_0046c5e0 = 1;
    DAT_0046c5de = 1;
    DAT_0046c5e1 = 1;
    DAT_0046c5f7 = 1;
    FUN_0042044d(0);
    break;
  case 2:
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
    }
    DAT_0046c5f4 = 1;
    DAT_0046c5df = 1;
    DAT_0046c5ee = 1;
    DAT_0046c5de = 1;
    DAT_0046c5e1 = 1;
    DAT_0046c5f7 = 1;
    break;
  case 3:
    DAT_0046c5ee = 1;
    DAT_0046c5ef = 1;
    DAT_0046c5d4 = 1;
    if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
      DAT_0046c5d5 = 1;
    }
    DAT_0046c5de = 1;
    break;
  case 4:
    DAT_0046c5ee = 1;
    DAT_0046c5f0 = 1;
    DAT_0046c5de = 1;
    DAT_0046c5f1 = 1;
    DAT_0046c5f2 = 1;
    break;
  case 5:
    DAT_0046c5ee = 1;
    DAT_0046c5dd = 1;
    DAT_0046c5de = 1;
    DAT_0046c5f1 = 1;
    FUN_00421949(-1,0);
  }
  FUN_0040df05(0x1d);
  return;
}

