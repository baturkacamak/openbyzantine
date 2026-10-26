// rea decompile_all
// name: FUN_004594e0
// address: 0x4594e0
// body: available
// range: 0x4594e0-0x45955f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_004594e0(uint param_1,int param_2)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  
  if (param_1 < DAT_004a7710) {
    pbVar1 = (byte *)(*(int *)((int)&DAT_004a7720 + ((int)(param_1 & 0xffffffe7) >> 3)) + 4 +
                     (param_1 & 0x1f) * 8);
    bVar2 = *pbVar1;
    if ((bVar2 & 1) != 0) {
      if (param_2 == 0x8000) {
        bVar3 = bVar2 & 0x7f;
      }
      else {
        if (param_2 != 0x4000) {
          DAT_00462c74 = 0x16;
          return -1;
        }
        bVar3 = bVar2 | 0x80;
      }
      *pbVar1 = bVar3;
      return (-(uint)((bVar2 & 0x80) == 0) & 0x4000) + 0x4000;
    }
  }
  DAT_00462c74 = 9;
  return -1;
}

