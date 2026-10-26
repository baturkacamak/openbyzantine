// rea decompile_all
// name: FUN_00405e7e
// address: 0x405e7e
// body: available
// range: 0x405e7e-0x406040 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00405e7e(int param_1,int param_2,undefined2 param_3,undefined2 param_4)

{
  undefined1 local_1c [12];
  undefined4 local_10;
  undefined1 local_c;
  void *local_8;
  
  *(int *)(&DAT_00487510 + param_2 * 0x48) = param_1;
  (&DAT_00487518)[param_2 * 0x12] =
       *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0x10) + param_1;
  if (**(short **)(&DAT_00487510 + param_2 * 0x48) == 8) {
    *(int *)(&DAT_00487514 + param_2 * 0x48) =
         *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 4) + param_1;
    *(int *)(&DAT_0048751c + param_2 * 0x48) =
         *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0x14) + param_1;
  }
  else {
    *(int *)(&DAT_00487520 + param_2 * 0x48) =
         *(int *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0x14) + param_1;
    local_10 = CONCAT22(*(ushort *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 0xc),
                        *(ushort *)(*(int *)(&DAT_00487510 + param_2 * 0x48) + 10));
    local_c = 0x10;
    if (((param_2 == 3) || (param_2 == 5)) || (param_2 == 6)) {
      local_8 = (void *)FUN_0044c2f0((uint)*(ushort *)
                                            (*(int *)(&DAT_00487510 + param_2 * 0x48) + 10) *
                                     (uint)*(ushort *)
                                            (*(int *)(&DAT_00487510 + param_2 * 0x48) + 0xc) * 2);
      FUN_00405c19((int)local_1c,*(ushort **)(&DAT_00487520 + param_2 * 0x48));
      FID_conflict__memcpy
                (*(void **)(&DAT_00487520 + param_2 * 0x48),local_8,
                 (local_10 & 0xffff) * (local_10 >> 0x10) * 2);
      FUN_0044ac40(local_8);
    }
  }
  *(undefined2 *)(&DAT_00487554 + param_2 * 0x48) = param_3;
  *(undefined2 *)(&DAT_00487556 + param_2 * 0x48) = param_4;
  *(undefined4 *)(&DAT_00487538 + param_2 * 0x48) = 0;
  *(undefined4 *)(&DAT_00487550 + param_2 * 0x48) = 0;
  return;
}

