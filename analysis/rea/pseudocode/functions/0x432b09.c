// rea decompile_all
// name: FUN_00432b09
// address: 0x432b09
// body: available
// range: 0x432b09-0x432be1 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00432b09(uint param_1,int param_2)

{
  int iVar1;
  ushort uVar2;
  undefined4 uVar3;
  uint uVar4;
  uint *local_c;
  
  iVar1 = *(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4);
  if (*(int *)(iVar1 + 0x34) == 0) {
    uVar3 = FUN_0044c2f0(0xc);
    *(undefined4 *)(iVar1 + 0x34) = uVar3;
    local_c = *(uint **)(iVar1 + 0x34);
  }
  else {
    for (local_c = *(uint **)(iVar1 + 0x34); *(int *)((int)local_c + 8) != 0;
        local_c = *(uint **)((int)local_c + 8)) {
    }
    uVar3 = FUN_0044c2f0(0xc);
    *(undefined4 *)((int)local_c + 8) = uVar3;
    local_c = *(uint **)((int)local_c + 8);
  }
  local_c[2] = 0;
  uVar4 = FUN_004319b3(param_2,1);
  local_c[1] = uVar4 & 0xffff;
  uVar2 = FUN_00436652(param_2,2);
  *local_c = (uint)uVar2;
  return;
}

