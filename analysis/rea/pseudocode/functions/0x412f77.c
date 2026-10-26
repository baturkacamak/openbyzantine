// rea decompile_all
// name: FUN_00412f77
// address: 0x412f77
// body: available
// range: 0x412f77-0x41316e inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

short FUN_00412f77(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  
  uVar1 = FUN_00411dfc(0x47dd70,(uint *)&DAT_004859a0,(ushort *)&DAT_004859d8);
  uVar2 = FUN_00411dfc(0x484e80,(uint *)&DAT_0046e800,(ushort *)&DAT_00477348);
  uVar3 = FUN_00411dfc(0x46e870,(uint *)&DAT_004859e0,(ushort *)&DAT_0046e24e);
  uVar4 = FUN_00411dfc(0x47a8e0,(uint *)&DAT_00483630,(ushort *)&DAT_00483048);
  uVar5 = FUN_00411dfc(0x483080,(uint *)&DAT_0047fa60,(ushort *)&DAT_0046e862);
  uVar6 = FUN_00411dfc(0x4848b0,(uint *)&DAT_00484310,&DAT_0048306e);
  uVar7 = FUN_00411dfc(0x46e280,(uint *)&DAT_0047a2a0,(ushort *)&DAT_004835fc);
  uVar8 = FUN_00411dfc(0x483d90,(uint *)&DAT_0046e240,(ushort *)&DAT_0046e814);
  uVar9 = FUN_00411dfc(0x485400,(uint *)&DAT_0047a270,&DAT_00485a00);
  uVar10 = FUN_00411dfc(0x47a2b0,(uint *)&DAT_0047ae60,(ushort *)&DAT_00483bb8);
  uVar11 = FUN_00411dfc(0x484330,(uint *)&DAT_00482440,(ushort *)&DAT_0047a25e);
  iVar12 = FUN_00412206((uint *)&DAT_00480320,0x483640);
  _DAT_0047a29e = (short)iVar12;
  return (short)uVar11 +
         (short)uVar10 +
         (short)uVar9 +
         (short)uVar8 +
         (short)uVar7 +
         (short)uVar6 + (short)uVar5 + (short)uVar4 + (short)uVar3 + (short)uVar2 + (short)uVar1;
}

