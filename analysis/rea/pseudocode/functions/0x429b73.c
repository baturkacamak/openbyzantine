// rea decompile_all
// name: FUN_00429b73
// address: 0x429b73
// body: available
// range: 0x429b73-0x429d19 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00429b73(FILE *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = FUN_00429080(DAT_0046caa0);
  iVar2 = FUN_00429080(DAT_0046ca9c);
  iVar3 = FUN_00429080(DAT_0046ca98);
  FID_conflict__fwprintf(param_1,(wchar_t *)s__3s__3s__3s_0046027c,iVar3,iVar2,iVar1);
  iVar1 = FUN_00429080(DAT_0046caac);
  iVar2 = FUN_00429080(DAT_0046caa8);
  iVar3 = FUN_00429080(DAT_0046caa4);
  FID_conflict__fwprintf(param_1,(wchar_t *)s__3s__3s__3s_004602ac,iVar3,iVar2,iVar1);
  iVar1 = FUN_00429080(DAT_0046cab8);
  iVar2 = FUN_00429080(DAT_0046cab4);
  iVar3 = FUN_00429080(DAT_0046cab0);
  FID_conflict__fwprintf(param_1,(wchar_t *)s__3s__3s__3s_004602dc,iVar3,iVar2,iVar1);
  FUN_00429080(DAT_0046cad0);
  iVar1 = FUN_00429080(DAT_0046cacc);
  iVar2 = FUN_00429080(DAT_0046cac8);
  iVar3 = FUN_00429080(DAT_0046cac4);
  iVar4 = FUN_00429080(DAT_0046cac0);
  iVar5 = FUN_00429080(DAT_0046cabc);
  FID_conflict__fwprintf
            (param_1,(wchar_t *)s__3s__3s__3s__3s__3s__3s_0046030c,iVar5,iVar4,iVar3,iVar2,iVar1);
  iVar1 = FUN_00429080(DAT_0046cadc);
  iVar2 = FUN_00429080(DAT_0046cad8);
  iVar3 = FUN_00429080(DAT_0046cad4);
  FID_conflict__fwprintf(param_1,(wchar_t *)s__3s__3s__3s_0046033c,iVar3,iVar2,iVar1);
  iVar1 = FUN_00429080(DAT_0046cae8);
  iVar2 = FUN_00429080(DAT_0046cae4);
  iVar3 = FUN_00429080(DAT_0046cae0);
  FID_conflict__fwprintf(param_1,(wchar_t *)s__3s__3s__3s_0046036c,iVar3,iVar2,iVar1);
  iVar1 = FUN_00429080(DAT_0046caf4);
  iVar2 = FUN_00429080(DAT_0046caf0);
  iVar3 = FUN_00429080(DAT_0046caec);
  FID_conflict__fwprintf(param_1,(wchar_t *)s__3s__3s__3s_0046039c,iVar3,iVar2,iVar1);
  return;
}

