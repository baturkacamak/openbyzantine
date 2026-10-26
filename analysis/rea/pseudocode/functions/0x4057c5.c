// rea decompile_all
// name: FUN_004057c5
// address: 0x4057c5
// body: available
// range: 0x4057c5-0x4058fc inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_004057c5(HMMIO param_1,HMMIO param_2,FOURCC *param_3)

{
  HGLOBAL pvVar1;
  undefined4 uVar2;
  MMRESULT MVar3;
  FOURCC FVar4;
  _MMCKINFO local_1c;
  HPSTR local_8;
  
  pvVar1 = GlobalAlloc(0x42,param_3[1]);
  local_8 = GlobalLock(pvVar1);
  if (local_8 == (HPSTR)0x0) {
    uVar2 = 0;
  }
  else {
    local_1c.ckid = *param_3;
    local_1c.cksize = param_3[1];
    MVar3 = mmioCreateChunk(param_2,&local_1c,0);
    if ((((MVar3 == 0) && (FVar4 = mmioRead(param_1,local_8,param_3[1]), FVar4 == param_3[1])) &&
        (FVar4 = mmioWrite(param_2,local_8,param_3[1]), FVar4 == param_3[1])) &&
       (MVar3 = mmioAscend(param_2,&local_1c,0), MVar3 == 0)) {
      if (local_8 != (HPSTR)0x0) {
        pvVar1 = GlobalHandle(local_8);
        GlobalUnlock(pvVar1);
        pvVar1 = GlobalHandle(local_8);
        GlobalFree(pvVar1);
      }
      uVar2 = 1;
    }
    else {
      if (local_8 != (HPSTR)0x0) {
        pvVar1 = GlobalHandle(local_8);
        GlobalUnlock(pvVar1);
        pvVar1 = GlobalHandle(local_8);
        GlobalFree(pvVar1);
      }
      uVar2 = 0;
    }
  }
  return uVar2;
}

