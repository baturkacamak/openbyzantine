// rea decompile_all
// name: FUN_004096f9
// address: 0x4096f9
// body: available
// range: 0x4096f9-0x40976b inclusive
// range: 0x409771-0x409784 inclusive
// range: 0x409795-0x409880 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004096f9(int param_1,int param_2,undefined4 param_3)

{
  byte bVar1;
  ushort uVar2;
  char *pcVar3;
  byte bVar4;
  uint local_34 [12];
  
  if (DAT_00485c6c == '\0') {
    uVar2 = FUN_00436652(param_2,2);
    FUN_00424780(param_1,(uint)uVar2);
  }
  else {
    switch(param_3) {
    case 0:
      FUN_00409457(s_VOICE_0045d3dc,'\x01');
      break;
    case 1:
      FUN_00409457(s_XVOICE_0045d3e4,'\x01');
      break;
    case 2:
      FUN_00409457(s_YVOICE_0045d3ec,'\x01');
      break;
    case 3:
      FUN_00409457(s_ZVOICE_0045d3f4,'\x01');
    }
    uVar2 = FUN_00436652(param_2,0);
    FUN_0044ba00(local_34,(uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4));
    FUN_0044ba08(local_34,(uint *)&DAT_0045d3fc);
    uVar2 = FUN_00436652(param_2,1);
    FUN_0044ba08(local_34,(uint *)**(undefined4 **)(&DAT_0047ae80 + (uint)uVar2 * 4));
    FUN_0044ba08(local_34,(uint *)&DAT_0045d400);
    FUN_004599c0(&DAT_0045d404);
    bVar4 = 0xc;
    pcVar3 = FUN_0044c580(local_34);
    bVar1 = (char)pcVar3 + 0xc;
    uVar2 = FUN_00436652(param_2,2);
    FUN_0040a897(*(int *)(param_1 + 0x720 + (uint)uVar2 * 4),bVar1,bVar4);
    FUN_004094f2();
  }
  return;
}

