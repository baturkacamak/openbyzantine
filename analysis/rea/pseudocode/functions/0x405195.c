// rea decompile_all
// name: FUN_00405195
// address: 0x405195
// body: available
// range: 0x405195-0x40537c inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


MMRESULT __cdecl
FUN_00405195(LPSTR param_1,int *param_2,short *param_3,LPMMCKINFO param_4,LPMMCKINFO param_5)

{
  HMMIO pHVar1;
  LONG LVar2;
  MMRESULT MVar3;
  char local_20 [4];
  _MMCKINFO local_1c;
  MMRESULT local_8;
  
  local_20[0] = -1;
  local_20[1] = -1;
  local_20[2] = -1;
  local_20[3] = -1;
  local_8 = 0;
  pHVar1 = mmioOpenA(param_1,(LPMMIOINFO)0x0,0x11002);
  *param_2 = (int)pHVar1;
  if (*param_2 == 0) {
    local_8 = 0xe104;
  }
  else {
    param_5->fccType = 0x45564157;
    param_5->cksize = 0;
    local_8 = mmioCreateChunk((HMMIO)*param_2,param_5,0x20);
    if (local_8 == 0) {
      param_4->ckid = 0x20746d66;
      param_4->cksize = 0x10;
      local_8 = mmioCreateChunk((HMMIO)*param_2,param_4,0);
      if (local_8 == 0) {
        if (*param_3 == 1) {
          LVar2 = mmioWrite((HMMIO)*param_2,(char *)param_3,0x10);
          if (LVar2 != 0x10) {
            return 0xe104;
          }
        }
        else {
          LVar2 = mmioWrite((HMMIO)*param_2,(char *)param_3,(ushort)param_3[8] + 0x12);
          if (LVar2 != (ushort)param_3[8] + 0x12) {
            return 0xe104;
          }
        }
        local_8 = mmioAscend((HMMIO)*param_2,param_4,0);
        if (local_8 == 0) {
          local_1c.ckid = 0x74636166;
          local_1c.cksize = 0;
          local_8 = mmioCreateChunk((HMMIO)*param_2,&local_1c,0);
          if (local_8 == 0) {
            LVar2 = mmioWrite((HMMIO)*param_2,local_20,4);
            if (LVar2 == 4) {
              MVar3 = mmioAscend((HMMIO)*param_2,&local_1c,0);
              if (MVar3 == 0) {
                local_8 = 0;
              }
              else {
                local_8 = 0xe104;
              }
            }
            else {
              local_8 = 0xe104;
            }
          }
        }
      }
    }
  }
  return local_8;
}

