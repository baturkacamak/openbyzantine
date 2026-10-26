// rea decompile_all
// name: FUN_00417a17
// address: 0x417a17
// body: available
// range: 0x417a17-0x417c32 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00417a17(uint *param_1,int param_2)

{
  char *pcVar1;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  int local_80;
  int local_7c;
  undefined4 local_78 [9];
  int local_54;
  int local_8;
  
  local_7c = DAT_00487508 * 3;
  local_8 = local_7c + 0x2e;
  if (param_1 == (uint *)0x0) {
    param_2 = 2;
  }
  if (DAT_0045f218 != (int *)0x0) {
    _memset(local_78,0,0x6c);
    local_78[0] = 0x6c;
    if ((param_2 == 2) || (DAT_0045f240 != 2)) {
      (**(code **)(*DAT_0045f218 + 100))(DAT_0045f218,0,local_78,1,0);
      local_80 = local_54;
      FID_conflict__memcpy
                ((void *)(local_54 + 0xe100),(void *)(local_8 * 0x500 + local_54),local_7c * 0x500);
      (**(code **)(*DAT_0045f218 + 0x80))(DAT_0045f218,0);
    }
    if (param_2 == 2) {
      if (DAT_0045f214 != (uint *)0x0) {
        FUN_0044ac40(DAT_0045f214);
        DAT_0045f214 = (uint *)0x0;
      }
    }
    else {
      if ((param_2 != 1) && (param_1 != DAT_0045f214)) {
        if (DAT_0045f214 != (uint *)0x0) {
          FUN_0044ac40(DAT_0045f214);
        }
        pcVar1 = FUN_0044c580(param_1);
        DAT_0045f214 = (uint *)FUN_0044c2f0((SIZE_T)(pcVar1 + 1));
        FUN_0044ba00(DAT_0045f214,param_1);
      }
      if ((param_2 == 0) && (DAT_0045f240 == 1)) {
        param_2 = 1;
      }
      else if (DAT_0045f210 != 0) {
        FUN_004063e0((char *)param_1,0x26e,0x2d,3,DAT_0045f218,0,4,3);
      }
    }
    DAT_0045f240 = param_2;
    if (DAT_0045c060 == 0) {
      local_90 = 0;
      local_8c = 0;
      local_88 = 0x280;
      local_84 = 400;
      FUN_004179ac();
      (**(code **)(*DAT_00486888 + 0x1c))(DAT_00486888,0,0x14,DAT_0045f218,&local_90,1);
    }
  }
  return;
}

