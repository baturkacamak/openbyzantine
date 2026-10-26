// rea decompile_all
// name: FUN_00439349
// address: 0x439349
// body: available
// range: 0x439349-0x43972f inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00439349(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_24;
  int local_8;
  
  DAT_0048253e = (undefined2)*(undefined4 *)((int)DAT_004603d0 + 4);
  DAT_00480330 = *(int *)((int)DAT_004603d0 + 8);
  iVar1 = *(int *)((int)DAT_004603d0 + 0xc);
  local_8 = 4;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    iVar2 = *(int *)((int)DAT_004603d0 + local_8 * 4);
    *(undefined4 *)(&DAT_004649e8 + iVar2 * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
    if (*(int *)(&DAT_004649e8 + iVar2 * 4) == 0) {
      *(undefined1 *)(*(int *)(&DAT_0047dd70 + iVar2 * 4) + 9) = 0;
    }
    else {
      *(undefined1 *)(*(int *)(&DAT_0047dd70 + iVar2 * 4) + 9) = 1;
    }
  }
  iVar1 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00464300 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  iVar1 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00464f60 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  iVar1 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar1; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00486a10 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  DAT_0046066c = *(undefined4 *)((int)DAT_004603d0 + local_8 * 4);
  DAT_0045fd2c = *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
  iVar1 = *(int *)((int)DAT_004603d0 + (local_8 + 2) * 4);
  iVar2 = *(int *)((int)DAT_004603d0 + (local_8 + 3) * 4);
  local_8 = local_8 + 4;
  for (local_24 = 0; local_24 < iVar2; local_24 = local_24 + 1) {
    *(undefined4 *)(&DAT_00486220 + *(int *)((int)DAT_004603d0 + local_8 * 4) * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
  }
  iVar2 = *(int *)((int)DAT_004603d0 + local_8 * 4);
  local_8 = local_8 + 1;
  for (local_24 = 0; local_24 < iVar2; local_24 = local_24 + 1) {
    iVar3 = *(int *)((int)DAT_004603d0 + local_8 * 4);
    *(undefined4 *)(&DAT_00485c90 + iVar3 * 4) =
         *(undefined4 *)((int)DAT_004603d0 + (local_8 + 1) * 4);
    local_8 = local_8 + 2;
    if (*(int *)(&DAT_00485c90 + iVar3 * 4) == 0) {
      *(undefined1 *)(*(int *)(&DAT_0047a8e0 + iVar3 * 4) + 9) = 0;
    }
    else {
      *(undefined1 *)(*(int *)(&DAT_0047a8e0 + iVar3 * 4) + 9) = 1;
    }
  }
  if (DAT_00464878 == 0) {
    if (*(int *)((int)DAT_004603d0 + local_8 * 4) == 0) {
      FUN_00417a17((uint *)0x0,2);
    }
    else {
      FUN_00417a17((uint *)((local_8 + 1) * 4 + (int)DAT_004603d0),0);
    }
  }
  FUN_0044ac40(DAT_004603d0);
  DAT_004603d0 = (LPVOID)0x0;
  DAT_0045c060 = (uint)(DAT_00480330 != -1);
  DAT_0045d1d0 = 1;
  FUN_004085e7();
  FUN_00426935();
  if ((iVar1 == -1) || (iVar1 == -2)) {
    DAT_0045fd2c = 0;
  }
  else {
    FUN_00427de1(iVar1,0);
  }
  return;
}

