// rea decompile_all
// name: FUN_0041159f
// address: 0x41159f
// body: available
// range: 0x41159f-0x4117a5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


DWORD __cdecl FUN_0041159f(uint *param_1,undefined4 *param_2,char param_3)

{
  char *pcVar1;
  void *_Dst;
  FILE *_File;
  int iVar2;
  undefined4 uVar3;
  DWORD local_11c;
  undefined1 local_10c;
  uint local_108 [65];
  
  FUN_0040c624(local_108,(uint *)s_sounds_0045ea20,param_1,(uint *)&DAT_0045ea18);
  _File = (FILE *)FUN_0044bc20((char *)local_108,&DAT_0045ea28);
  if ((_File == (FILE *)0x0) && (DAT_00485c6c == '\0')) {
    FUN_0040ac43();
    FUN_0040ac38();
    local_11c = 0;
  }
  else {
    local_11c = FUN_0041199e(_File);
    iVar2 = local_11c + 0xc;
    local_10c = (undefined1)iVar2;
    DAT_0045e7cb = local_10c;
    local_10c = (undefined1)((uint)iVar2 >> 8);
    DAT_0045e7cc = local_10c;
    local_10c = (undefined1)((uint)iVar2 >> 0x10);
    DAT_0045e7cd = local_10c;
    if (param_3 == '\b') {
      DAT_0045e7d2 = 8;
      DAT_0045e7d4 = 0;
    }
    else {
      DAT_0045e7d2 = 0x10;
      DAT_0045e7d4 = 4;
    }
    DAT_0045e7ce = 0x22;
    DAT_0045e7cf = 0x56;
    uVar3 = FUN_0044c2f0(local_11c + 0x2b);
    *param_2 = uVar3;
    _Dst = (void *)*param_2;
    FID_conflict__memcpy(_Dst,&DAT_0045e7b0,0x2a);
    pcVar1 = (char *)((int)_Dst + 0x2a);
    __fread_lk(pcVar1,local_11c,1,_File);
    pcVar1[local_11c] = '\0';
    __fread_lk((char *)*param_2,local_11c,1,_File);
    _fclose(_File);
  }
  return local_11c;
}

