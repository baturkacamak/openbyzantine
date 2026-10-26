// rea decompile_all
// name: FUN_0044e8e0
// address: 0x44e8e0
// body: available
// range: 0x44e8e0-0x44e9f5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 FUN_0044e8e0(void)

{
  undefined1 uVar1;
  undefined3 extraout_var;
  HMODULE hModule;
  FARPROC pFVar2;
  FARPROC pFVar3;
  FARPROC pFVar4;
  FARPROC pFVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  undefined3 extraout_var_00;
  uint unaff_EDI;
  undefined1 auStack_8 [4];
  undefined1 auStack_4 [4];
  
  uVar1 = __ms_p5_test_fdiv();
  if (CONCAT31(extraout_var,uVar1) != 0) {
    return 1;
  }
  hModule = LoadLibraryA(s_KERNEL32_00463210);
  if (hModule == (HMODULE)0x0) {
    return 0;
  }
  pFVar2 = GetProcAddress(hModule,s_GetProcessAffinityMask_004631f8);
  if (pFVar2 == (FARPROC)0x0) {
    return 0;
  }
  pFVar3 = GetProcAddress(hModule,s_GetCurrentProcess_004631e4);
  if (pFVar3 == (FARPROC)0x0) {
    return 0;
  }
  pFVar4 = GetProcAddress(hModule,s_SetThreadAffinityMask_004631cc);
  if (pFVar4 == (FARPROC)0x0) {
    return 0;
  }
  pFVar5 = GetProcAddress(hModule,s_GetCurrentThread_004631b8);
  if (pFVar5 == (FARPROC)0x0) {
    return 0;
  }
  iVar6 = (*pFVar3)();
  iVar6 = (*pFVar2)(iVar6,auStack_4,auStack_8);
  if (iVar6 == 0) {
    return 0;
  }
  iVar6 = 0;
  do {
    uVar7 = 1 << ((byte)iVar6 & 0x1f);
    if ((unaff_EDI & uVar7) != 0) {
      iVar8 = (*pFVar5)(uVar7);
      (*pFVar4)(iVar8);
      uVar1 = __ms_p5_test_fdiv();
      if (CONCAT31(extraout_var_00,uVar1) != 0) {
        iVar6 = (*pFVar5)(unaff_EDI);
        (*pFVar4)(iVar6);
        return 1;
      }
    }
    iVar6 = iVar6 + 1;
  } while (iVar6 < 0x20);
  iVar6 = (*pFVar5)(unaff_EDI);
  (*pFVar4)(iVar6);
  return 0;
}

