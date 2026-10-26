// rea decompile_all
// name: FUN_00406041
// address: 0x406041
// body: available
// range: 0x406041-0x406129 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


bool __cdecl FUN_00406041(LPCSTR param_1,LPVOID param_2)

{
  HANDLE hFile;
  ushort *lpBuffer;
  undefined4 uVar1;
  SIZE_T nNumberOfBytesToRead;
  DWORD local_8;
  
  hFile = CreateFileA(param_1,0x80000000,0,(LPSECURITY_ATTRIBUTES)0x0,3,0x80,(HANDLE)0x0);
  if (hFile != (HANDLE)0xffffffff) {
    ReadFile(hFile,param_2,0x12,&local_8,(LPOVERLAPPED)0x0);
    nNumberOfBytesToRead =
         ((int)(uint)*(byte *)((int)param_2 + 0x10) >> 3) * (uint)*(ushort *)((int)param_2 + 0xc) *
         (uint)*(ushort *)((int)param_2 + 0xe);
    lpBuffer = (ushort *)FUN_0044c2f0(nNumberOfBytesToRead);
    uVar1 = FUN_0044c2f0(nNumberOfBytesToRead);
    *(undefined4 *)((int)param_2 + 0x14) = uVar1;
    ReadFile(hFile,lpBuffer,nNumberOfBytesToRead,&local_8,(LPOVERLAPPED)0x0);
    CloseHandle(hFile);
    FUN_00405c19((int)param_2,lpBuffer);
    FUN_0044ac40(lpBuffer);
  }
  return hFile != (HANDLE)0xffffffff;
}

