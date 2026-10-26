// rea decompile_all
// name: FUN_0042b039
// address: 0x42b039
// body: available
// range: 0x42b039-0x42b271 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0042b039(undefined4 *param_1)

{
  DWORD DVar1;
  undefined4 uVar2;
  HGLOBAL pvVar3;
  char local_278 [256];
  BOOL local_178;
  undefined4 local_174;
  undefined1 local_160 [16];
  undefined4 local_150;
  undefined1 local_14c [4];
  undefined4 local_148;
  undefined4 local_144;
  ushort local_13e;
  HANDLE local_124;
  undefined4 local_120;
  SIZE_T local_118;
  DWORD local_c [2];
  
  local_174 = 0;
  local_120 = 0;
  local_150 = 0;
  _memset(local_278,0,0x100);
  local_124 = FUN_0042a9e5((LPCSTR)*param_1);
  if ((local_124 == (HANDLE)0xffffffff) && (DVar1 = GetLastError(), DVar1 == 0)) {
    CloseHandle(local_124);
    uVar2 = 0;
  }
  else {
    local_178 = ReadFile(local_124,local_160,0xe,local_c,(LPOVERLAPPED)0x0);
    if (local_178 == 0) {
      CloseHandle(local_124);
      uVar2 = 0;
    }
    else {
      local_178 = ReadFile(local_124,local_14c,0x28,local_c,(LPOVERLAPPED)0x0);
      if (local_178 == 0) {
        CloseHandle(local_124);
        uVar2 = 0;
      }
      else {
        param_1[6] = (uint)local_13e;
        param_1[5] = local_144;
        param_1[4] = local_148;
        if (param_1[4] == 0) {
          _sprintf(local_278,(char *)*param_1);
        }
        local_118 = ((int)(uint)local_13e >> 3) * param_1[5] * param_1[4];
        if (param_1[3] == 0) {
          pvVar3 = GlobalAlloc(0x40,local_118);
          param_1[3] = pvVar3;
        }
        local_178 = ReadFile(local_124,(LPVOID)param_1[3],local_118,local_c,(LPOVERLAPPED)0x0);
        if (local_178 == 0) {
          GlobalFree((HGLOBAL)param_1[3]);
          param_1[3] = 0;
          CloseHandle(local_124);
          uVar2 = 0;
        }
        else {
          CloseHandle(local_124);
          uVar2 = 1;
        }
      }
    }
  }
  return uVar2;
}

