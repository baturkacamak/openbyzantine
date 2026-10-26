// rea decompile_all
// name: FUN_00438de4
// address: 0x438de4
// body: available
// range: 0x438de4-0x439348 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00438de4(int param_1)

{
  int iVar1;
  uint uVar2;
  uint local_2c;
  uint local_28;
  uint local_24;
  uint local_20;
  uint local_18;
  char *local_14;
  uint local_10;
  uint local_c;
  int local_8;
  
  local_c = 0;
  local_24 = 0;
  local_10 = 0;
  local_28 = 0;
  local_2c = 0;
  local_18 = 0;
  DAT_0045d1d0 = 0;
  DAT_0045c060 = 0;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_004649e8 + local_20 * 4) != 0) {
      local_c = local_c + 1;
    }
    if (*(int *)(&DAT_00464300 + local_20 * 4) != 0) {
      local_24 = local_24 + 1;
    }
    if (*(int *)(&DAT_00464f60 + local_20 * 4) != 0) {
      local_10 = local_10 + 1;
    }
    if (*(int *)(&DAT_00486a10 + local_20 * 4) != 0) {
      local_28 = local_28 + 1;
    }
    if (*(int *)(&DAT_00486220 + local_20 * 4) != 0) {
      local_18 = local_18 + 1;
    }
    if (*(int *)(&DAT_00485c90 + local_20 * 4) != 0) {
      local_2c = local_2c + 1;
    }
  }
  iVar1 = (local_2c + local_28 + local_10 + local_24 + local_c + local_18) * 2;
  local_14 = (char *)(iVar1 + 0xd);
  if ((DAT_00464878 == 0) && (DAT_0045f214 != (uint *)0x0)) {
    local_14 = FUN_0044c580(DAT_0045f214);
    local_14 = local_14 + iVar1 + 0xe;
  }
  DAT_004603d0 = (uint *)FUN_0044c2f0((int)local_14 << 2);
  *DAT_004603d0 = (uint)DAT_00482428;
  DAT_004603d0[1] = (uint)DAT_0048253e;
  DAT_004603d0[2] = DAT_00480330;
  DAT_004603d0[3] = local_c;
  local_8 = 4;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_004649e8 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_004649e8 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_004649e8 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_24;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00464300 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00464300 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00464300 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_10;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00464f60 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00464f60 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00464f60 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_28;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00486a10 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00486a10 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00486a10 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = DAT_0046066c;
  DAT_0046066c = 0;
  uVar2 = FUN_0042295d();
  DAT_004603d0[local_8 + 1] = uVar2;
  uVar2 = FUN_00422927();
  DAT_004603d0[local_8 + 2] = uVar2;
  if (-1 < (int)DAT_004603d0[local_8 + 2]) {
    local_2c = local_2c - 1;
  }
  if (param_1 != 0) {
    DAT_004603d0[local_8 + 2] = 0xffffffff;
  }
  DAT_004603d0[local_8 + 3] = local_18;
  local_8 = local_8 + 4;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00486220 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00486220 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00486220 + local_20 * 4) = 0;
    }
  }
  DAT_004603d0[local_8] = local_2c;
  local_8 = local_8 + 1;
  for (local_20 = 0; (int)local_20 < 0x15e; local_20 = local_20 + 1) {
    if (*(int *)(&DAT_00485c90 + local_20 * 4) != 0) {
      DAT_004603d0[local_8] = local_20;
      DAT_004603d0[local_8 + 1] = *(uint *)(&DAT_00485c90 + local_20 * 4);
      local_8 = local_8 + 2;
      *(undefined4 *)(&DAT_00485c90 + local_20 * 4) = 0;
    }
  }
  if (DAT_00464878 == 0) {
    if (DAT_0045f214 == (uint *)0x0) {
      DAT_004603d0[local_8] = 0;
    }
    else {
      DAT_004603d0[local_8] = 1;
      FUN_0044ba00(DAT_004603d0 + local_8 + 1,DAT_0045f214);
    }
    FUN_00417a17((uint *)0x0,2);
  }
  return;
}

