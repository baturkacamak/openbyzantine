// rea decompile_all
// name: FUN_0041b251
// address: 0x41b251
// body: available
// range: 0x41b251-0x41b413 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


DWORD __thiscall FUN_0041b251(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  DWORD DVar1;
  BOOL BVar2;
  
  if (*(int *)((int)this + 0x10) == 0) {
    DVar1 = 0;
  }
  else {
    DVar1 = FUN_0041b414(*(undefined4 *)this,param_3,*(undefined4 *)((int)this + 0x14),param_1,
                         *(undefined4 *)((int)this + 0x1c),*(undefined4 *)((int)this + 0x20),
                         *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                         *(undefined4 *)((int)this + 0x18),param_2,*(undefined4 *)((int)this + 0x2c)
                         ,(*(int *)((int)this + 0x58) - *(int *)((int)this + 0x30)) -
                          (*(int *)((int)this + 0x48) / *(int *)((int)this + 0x38)) *
                          *(int *)((int)this + 0x38),*(undefined4 *)((int)this + 0x34),
                         *(undefined4 *)((int)this + 0x38));
    if (DVar1 == 0) {
      if ((*(int *)((int)this + 0x44) == *(int *)((int)this + 0x34)) &&
         (*(int *)((int)this + 0x48) == *(int *)((int)this + 0x38))) {
        BVar2 = BitBlt(*(HDC *)((int)this + 4),*(int *)((int)this + 0x4c),*(int *)((int)this + 0x50)
                       ,*(int *)((int)this + 0x54),*(int *)((int)this + 0x58),
                       *(HDC *)((int)this + 0x1c8),0,0,0xcc0020);
        if (BVar2 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      else {
        BVar2 = StretchBlt(*(HDC *)((int)this + 4),*(int *)((int)this + 0x3c),
                           *(int *)((int)this + 0x40),*(int *)((int)this + 0x44),
                           *(int *)((int)this + 0x48),*(HDC *)((int)this + 0x1c8),
                           *(int *)((int)this + 0x2c),*(int *)((int)this + 0x30),
                           *(int *)((int)this + 0x34),*(int *)((int)this + 0x38),0xcc0020);
        if (BVar2 == 0) {
          DVar1 = GetLastError();
          return DVar1;
        }
      }
      GdiFlush();
      DVar1 = 0;
    }
  }
  return DVar1;
}

