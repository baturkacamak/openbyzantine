// rea decompile_all
// name: FUN_0041b48b
// address: 0x41b48b
// body: available
// range: 0x41b48b-0x41b673 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


DWORD __thiscall FUN_0041b48b(void *this,undefined4 param_1,undefined4 param_2,int *param_3)

{
  DWORD DVar1;
  HDC local_14;
  undefined4 local_10;
  int local_c;
  DWORD local_8;
  
  if (*(int *)((int)this + 0x10) == 0) {
    local_8 = 0;
  }
  else {
    local_10 = *(undefined4 *)((int)this + 0x2c);
    local_c = *(int *)((int)this + 0x30);
    local_8 = FUN_0041b414(*(undefined4 *)this,param_2,*(undefined4 *)((int)this + 0x14),param_1,
                           *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                           *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                           *(undefined4 *)((int)this + 0x18),*(undefined4 *)((int)this + 0x1b8),
                           local_10,(*(int *)((int)this + 0x58) - local_c) -
                                    (*(int *)((int)this + 0x48) / *(int *)((int)this + 0x38)) *
                                    *(int *)((int)this + 0x38),*(undefined4 *)((int)this + 0x34),
                           *(undefined4 *)((int)this + 0x38));
    if (local_8 == 0) {
      (**(code **)(*param_3 + 0x44))(param_3,&local_14);
      if ((*(int *)((int)this + 0x44) == *(int *)((int)this + 0x34)) &&
         (*(int *)((int)this + 0x48) == *(int *)((int)this + 0x38))) {
        local_8 = BitBlt(local_14,*(int *)((int)this + 0x4c),*(int *)((int)this + 0x50),
                         *(int *)((int)this + 0x54),*(int *)((int)this + 0x58),
                         *(HDC *)((int)this + 0x1c8),0,0,0xcc0020);
        if (local_8 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      else {
        local_8 = StretchBlt(local_14,*(int *)((int)this + 0x3c),*(int *)((int)this + 0x40),
                             *(int *)((int)this + 0x44),*(int *)((int)this + 0x48),
                             *(HDC *)((int)this + 0x1c8),*(int *)((int)this + 0x2c),
                             *(int *)((int)this + 0x30),*(int *)((int)this + 0x34),
                             *(int *)((int)this + 0x38),0xcc0020);
        if (local_8 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      GdiFlush();
      (**(code **)(*param_3 + 0x68))(param_3,local_14);
      local_8 = 0;
    }
  }
  return local_8;
}

