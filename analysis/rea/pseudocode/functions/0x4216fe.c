// rea decompile_all
// name: FUN_004216fe
// address: 0x4216fe
// body: available
// range: 0x4216fe-0x421948 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004216fe(uint param_1)

{
  int *piVar1;
  int iVar2;
  int local_80;
  int local_78;
  int local_70;
  char local_6c [8];
  uint local_64 [20];
  uint *local_14;
  int *local_10;
  int local_c;
  int local_8;
  
  local_10 = (int *)FUN_00418040(DAT_00477344);
  piVar1 = *(int **)(&DAT_0046c8d0 + param_1 * 4);
  if (piVar1 != (int *)0x0) {
    if ((param_1 & 1) == 0) {
      local_70 = 0x7f;
      local_78 = 0x61;
      local_c = 0x46;
    }
    else {
      local_70 = 0x16c;
      local_78 = 0x69;
      local_c = 0x4d;
    }
    _sprintf(local_6c,&DAT_0045fce4,piVar1[2] + 1);
    if (*piVar1 == -1) {
      FUN_0041d45a(local_64,0x3b);
      local_14 = local_64;
    }
    else {
      local_14 = (uint *)FUN_00421690(*piVar1);
    }
    FUN_004063e0(local_6c,local_70 + 0x14,local_c,2,local_10,0,2,1);
    FUN_004063e0((char *)local_14,local_70 + 0x14,local_c,2,local_10,0,0,1);
    for (local_80 = 0; local_80 < piVar1[1]; local_80 = local_80 + 1) {
      iVar2 = *(int *)(piVar1[3] + local_80 * 4);
      if (iVar2 == -1) {
        local_14 = DAT_0045f4e4;
        local_8 = 1;
      }
      else if (iVar2 == -3) {
        FUN_0041d45a(local_64,0x33);
        local_14 = local_64;
        local_8 = 0;
      }
      else {
        local_14 = *(uint **)(&DAT_0046c840 + iVar2 * 4);
        local_8 = 0;
      }
      FUN_004063e0((char *)local_14,local_70,local_78,2,local_10,local_8,0,99);
      local_78 = local_78 + 0x14;
      local_14 = *(uint **)(piVar1[4] + local_80 * 4);
      iVar2 = FUN_0040701f(local_14,0xb9,2);
      if (iVar2 * 0x14 + local_78 < 0x1e0) {
        FUN_004063e0((char *)local_14,local_70 + 10,local_78,2,local_10,local_8,3,99);
      }
      FUN_0040711b((char *)local_14);
      local_78 = local_78 + iVar2 * 0x14;
      if (0x1df < local_78) {
        return;
      }
    }
  }
  return;
}

