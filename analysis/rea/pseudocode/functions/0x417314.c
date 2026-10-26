// rea decompile_all
// name: FUN_00417314
// address: 0x417314
// body: available
// range: 0x417314-0x417490 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00417314(int param_1)

{
  int *piVar1;
  undefined4 local_88 [9];
  int local_64;
  int local_1c;
  int local_18;
  tagRECT local_14;
  
  piVar1 = DAT_0046c5b8;
  if (DAT_0046c5b8 != (int *)0x0) {
    _memset(local_88,0,0x6c);
    local_88[0] = 0x6c;
    local_18 = (int)*(short *)(piVar1[1] + 6);
    local_1c = (int)*(short *)(piVar1[1] + 8);
    SetRect(&local_14,local_18,local_1c,*(short *)(piVar1[1] + 2) + local_18,
            *(short *)(piVar1[1] + 4) + local_1c);
    (**(code **)(*DAT_0045f218 + 100))(DAT_0045f218,0,local_88,1,0);
    if (param_1 == 0) {
      FID_conflict__memcpy
                ((void *)(local_1c * 0x500 + local_64),
                 (void *)(((local_1c - *(short *)(piVar1[1] + 4)) * 5 + -10) * 0x100 + local_64),
                 *(short *)(piVar1[1] + 4) * 0x500);
    }
    else {
      FUN_0040bbb0((short)local_18,(short)local_1c,(int)*(short *)(piVar1[1] + 4),0x280);
      FUN_0040bbfd(*piVar1,local_64,(short)local_18,(short)local_1c,&local_14.left,0);
    }
    (**(code **)(*DAT_0045f218 + 0x80))(DAT_0045f218,0);
  }
  return;
}

