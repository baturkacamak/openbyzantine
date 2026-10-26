// rea decompile_all
// name: FUN_0041ae57
// address: 0x41ae57
// body: available
// range: 0x41ae57-0x41b063 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


DWORD __thiscall FUN_0041ae57(void *this,int *param_1)

{
  HDC pHVar1;
  HBITMAP pHVar2;
  HGDIOBJ pvVar3;
  undefined4 local_c;
  DWORD local_8;
  
  (**(code **)(*param_1 + 0x44))(param_1,&local_c);
  *(undefined4 *)((int)this + 4) = local_c;
  pHVar1 = CreateCompatibleDC(*(HDC *)((int)this + 4));
  *(HDC *)((int)this + 0x1c8) = pHVar1;
  local_8 = FUN_0041b0db(*(undefined4 *)this,0,*(undefined4 *)((int)this + 0x14),0,
                         *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                         *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                         *(undefined4 *)((int)this + 0x18),0,*(undefined4 *)((int)this + 0x2c),
                         *(undefined4 *)((int)this + 0x30),*(undefined4 *)((int)this + 0x34),
                         *(undefined4 *)((int)this + 0x38));
  if (local_8 == 0) {
    (**(code **)(*param_1 + 0x68))(param_1,local_c);
    local_8 = FUN_0041b064(*(undefined4 *)this,0,*(undefined4 *)((int)this + 0x14),0,
                           *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                           *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                           *(undefined4 *)((int)this + 0x18),0,*(undefined4 *)((int)this + 0x2c),
                           *(undefined4 *)((int)this + 0x30),*(undefined4 *)((int)this + 0x34),
                           *(undefined4 *)((int)this + 0x38));
    if (local_8 == 0) {
      *(undefined4 *)((int)this + 0x1d4) = 0;
      *(undefined4 *)((int)this + 0x1d0) = *(undefined4 *)((int)this + 0x1d4);
      *(undefined4 *)((int)this + 0x1bc) = *(undefined4 *)((int)this + 0x1d0);
      pHVar2 = CreateDIBSection(*(HDC *)((int)this + 4),*(BITMAPINFO **)((int)this + 0x18),0,
                                (void **)((int)this + 0x1b8),(HANDLE)0x0,0);
      *(HBITMAP *)((int)this + 0x1b4) = pHVar2;
      if (*(int *)((int)this + 0x1b4) == 0) {
        local_8 = GetLastError();
      }
      if ((*(int *)((int)this + 0x1b4) == 0) || (*(int *)((int)this + 0x1b8) == 0)) {
        local_8 = 1;
      }
      else {
        pvVar3 = SelectObject(*(HDC *)((int)this + 0x1c8),*(HGDIOBJ *)((int)this + 0x1b4));
        *(HGDIOBJ *)((int)this + 0x1cc) = pvVar3;
        SetStretchBltMode(*(HDC *)((int)this + 4),3);
        *(undefined4 *)((int)this + 0x10) = 1;
        local_8 = 0;
      }
    }
  }
  return local_8;
}

