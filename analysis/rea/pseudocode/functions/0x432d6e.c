// rea decompile_all
// name: FUN_00432d6e
// address: 0x432d6e
// body: available
// range: 0x432d6e-0x43302a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined2 __cdecl FUN_00432d6e(char param_1,int *param_2,uint param_3)

{
  bool bVar1;
  ushort uVar2;
  int iVar3;
  uint uVar4;
  bool bVar5;
  byte local_14;
  undefined2 local_8;
  
  local_8 = 0xffff;
  iVar3 = FUN_0041d440(3);
  if (iVar3 == 1) {
    FUN_0042120c((uint)DAT_00482428,(int)param_1);
  }
  DAT_0045c060 = 0;
  DAT_0046068c = 1;
  if (DAT_0045f494 == 0) {
    DAT_00460684 = 1;
  }
  DAT_0045dad8 = 0;
  if (DAT_0045d1c8 != (undefined1 *)0x0) {
    *DAT_0045d1c8 = 1;
    DAT_0045d1c8 = (undefined1 *)0x0;
    FUN_00407420(0);
  }
  bVar5 = DAT_0045d1c0 != 0;
  if (bVar5) {
    DAT_0045d1c0 = 0;
  }
  DAT_0047a898 = 0xff;
  bVar1 = false;
  while (!bVar1) {
    FUN_00437ca2();
    for (local_14 = 0; local_14 < 0x32; local_14 = local_14 + 1) {
      param_2[local_14 + 0xc0] = 0;
    }
    DAT_0048430c = 0;
    for (local_14 = 0; local_14 < 0x19; local_14 = local_14 + 1) {
      *(undefined2 *)(&DAT_0046e830 + (uint)local_14 * 2) = 0;
    }
    *(undefined2 *)(param_2 + 2) = 0;
    *(undefined1 *)((int)param_2 + 10) = 0;
    if (*(char *)((int)param_2 + 0xe) != '\0') {
      uVar2 = FUN_0043302b((int)param_2,*(byte *)((int)param_2 + 0xd));
      *(ushort *)(param_2 + 2) = uVar2;
    }
    if ((char)param_2[3] != '\0') {
      uVar2 = FUN_0043302b((int)param_2,*(byte *)((int)param_2 + 0xb));
      *(ushort *)(param_2 + 2) = uVar2;
      *(undefined1 *)(param_2 + 3) = 0;
    }
    DAT_00483d82 = 1;
    FUN_0043340a(param_2,(byte *)*param_2);
    FUN_0043340a(param_2,(byte *)param_2[1]);
    FUN_00437c72();
    *(undefined1 *)((int)param_2 + 0xe) = 0;
    do {
      uVar2 = FUN_004330bf((int)param_2,(int)param_2 + 0xf);
      *(ushort *)(param_2 + 2) = uVar2;
      uVar2 = FUN_004331d2((int)param_2,(int)(param_2 + 0x2a));
      *(ushort *)(param_2 + 2) = uVar2;
      uVar4 = FUN_0043688b(param_2,param_3);
      local_8 = (undefined2)uVar4;
    } while (*(char *)((int)param_2 + 0xe) == '\0');
    bVar1 = true;
  }
  FUN_00424c4e(1);
  iVar3 = FUN_0041d440(3);
  if (iVar3 == 1) {
    FUN_00421264();
  }
  if (DAT_0045f494 == 0) {
    FUN_0040df05(0);
  }
  if (bVar5) {
    DAT_0045d1c0 = 1;
  }
  DAT_00460684 = 0;
  FUN_004365fa();
  return local_8;
}

