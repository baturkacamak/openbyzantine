// rea decompile_all
// name: FUN_0042b5b2
// address: 0x42b5b2
// body: available
// range: 0x42b5b2-0x42b678 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint __cdecl FUN_0042b5b2(uint param_1)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_8 = CONCAT22(local_8._2_2_,(short)((param_1 & 0xffff) >> ((byte)DAT_00485c8c & 0x1f))) &
            0xffff001f;
  local_10 = CONCAT22(local_10._2_2_,(short)((param_1 & 0xffff) >> (DAT_00485c80 & 0x1f))) &
             0xffff001f;
  local_c = CONCAT22(local_c._2_2_,(short)((param_1 & 0xffff) >> (DAT_00485c78 & 0x1f))) &
            0xffff001f;
  uVar3 = FUN_0042b395(local_c);
  bVar1 = DAT_00485c78 & 0x1f;
  uVar4 = FUN_0042b395(local_10);
  bVar2 = DAT_00485c80 & 0x1f;
  uVar5 = FUN_0042b395(local_8);
  local_8 = (uVar5 & 0xffff) << ((byte)DAT_00485c8c & 0x1f) & 0xffff;
  return local_8 | (uVar4 & 0xffff) << bVar2 & 0xffff | (uVar3 & 0xffff) << bVar1 & 0xffff;
}

