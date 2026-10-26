// rea decompile_all
// name: FUN_004326eb
// address: 0x4326eb
// body: available
// range: 0x4326eb-0x432903 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_004326eb(uint param_1,int param_2)

{
  short *psVar1;
  bool bVar2;
  short sVar3;
  ushort uVar4;
  short *psVar5;
  short *local_c;
  
  bVar2 = false;
  psVar1 = *(short **)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  if ((*(byte *)((int)psVar1 + 9) & 0x20) == 0) {
    local_c = *(short **)(psVar1 + 0x1e);
    while ((local_c != (short *)0x0 && (!bVar2))) {
      if ((*local_c == *psVar1) && (local_c[1] == psVar1[1])) {
        bVar2 = true;
      }
      local_c = *(short **)(local_c + 2);
    }
    if (!bVar2) {
      psVar5 = (short *)FUN_0044c2f0(8);
      *psVar5 = *psVar1;
      psVar5[1] = psVar1[1];
      psVar5[2] = 0;
      psVar5[3] = 0;
      local_c = *(short **)(psVar1 + 0x1e);
      if (local_c == (short *)0x0) {
        *(short **)(psVar1 + 0x1e) = psVar5;
      }
      else {
        for (; *(int *)((int)local_c + 4) != 0; local_c = *(short **)((int)local_c + 4)) {
        }
        *(short **)((int)local_c + 4) = psVar5;
      }
    }
  }
  if (*(char *)(param_2 + 2) == '@') {
    sVar3 = FUN_00436652(param_2,1);
    *psVar1 = sVar3;
  }
  else if (*(char *)(param_2 + 2) == '\x02') {
    uVar4 = FUN_00436652(param_2,1);
    *psVar1 = *(short *)(&DAT_0047e2f0 +
                        (uint)*(ushort *)
                               (&DAT_0046ee00 + (uint)uVar4 * 2 + (uint)DAT_00482428 * 600) * 2);
  }
  if (*(char *)(param_2 + 3) == '@') {
    sVar3 = FUN_00436652(param_2,2);
    psVar1[1] = sVar3;
  }
  else if (*(char *)(param_2 + 3) == '\x02') {
    uVar4 = FUN_00436652(param_2,2);
    psVar1[1] = *(short *)(&DAT_0047e2f0 +
                          (uint)*(ushort *)
                                 (&DAT_0046ee00 + (uint)uVar4 * 2 + (uint)DAT_00482428 * 600) * 2);
  }
  psVar1[1] = psVar1[1] + 0x14;
  return;
}

