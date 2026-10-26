// rea decompile_all
// name: FUN_0040562e
// address: 0x40562e
// body: available
// range: 0x40562e-0x4056c5 inclusive
// range: 0x4056e4-0x40571d inclusive
// range: 0x405728-0x4057c4 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl
FUN_0040562e(undefined4 *param_1,LPMMCKINFO param_2,MMCKINFO *param_3,undefined4 *param_4)

{
  uint uVar1;
  LONG LVar2;
  MMRESULT MVar3;
  undefined4 local_8;
  
  LVar2 = mmioSeek((HMMIO)*param_1,param_3->dwDataOffset + 4,0);
  if (LVar2 == -1) {
    local_8 = 0xe102;
  }
  else {
    local_8 = 0;
    while ((MVar3 = mmioDescend((HMMIO)*param_1,param_2,param_3,0), MVar3 == 0 &&
           (param_2->dwDataOffset + param_2->cksize <= param_3->cksize + param_3->dwDataOffset))) {
      uVar1 = param_2->ckid;
      if ((0x20657563 < uVar1) && (0x4b4e554a < uVar1)) {
        if (uVar1 < 0x61746165) {
          if ((uVar1 != 0x61746164) && (uVar1 == 0x50534944)) {
            FUN_004057c5((HMMIO)*param_1,(HMMIO)*param_4,&param_2->ckid);
          }
        }
        else if ((uVar1 != 0x74636166) && (uVar1 == 0x74736c70)) {
          FUN_004057c5((HMMIO)*param_1,(HMMIO)*param_4,&param_2->ckid);
        }
      }
      mmioAscend((HMMIO)*param_1,param_2,0);
    }
  }
  mmioSeek((HMMIO)*param_1,param_3->dwDataOffset + 4,0);
  return local_8;
}

