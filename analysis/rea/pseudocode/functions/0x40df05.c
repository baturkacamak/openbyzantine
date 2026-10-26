// rea decompile_all
// name: FUN_0040df05
// address: 0x40df05
// body: available
// range: 0x40df05-0x40e1a7 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_0040df05(ushort param_1)

{
  ushort uVar1;
  int iVar2;
  undefined2 extraout_var;
  undefined4 local_80 [2];
  uint local_78;
  uint local_74;
  int local_14;
  int local_10;
  int local_c;
  int *local_8;
  
  if (param_1 < 0x20) {
    iVar2 = FUN_0040de7b((uint)param_1);
    param_1 = (ushort)iVar2;
  }
  if ((param_1 != DAT_00476338) && (DAT_0045dbd0 != 0)) {
    if ((DAT_00476338 < 0x20) && (1 < *(int *)(PTR_PTR_0045e534 + (uint)DAT_00476338 * 0x24 + 0x10))
       ) {
      FUN_0040ed73();
    }
    _memset(local_80,0,0x6c);
    local_80[0] = 0x6c;
    DAT_0045dbd0 = 0;
    FUN_0040e25f();
    if (param_1 < 0x20) {
      local_8 = *(int **)(PTR_PTR_0045e534 + (uint)param_1 * 0x24 + 0x14);
      *(undefined4 *)(PTR_PTR_0045e534 + (uint)param_1 * 0x24 + 0xc) = 0;
      (**(code **)(*local_8 + 0x58))(local_8,local_80);
      DAT_0046c448 = (int)DAT_00486a0a - *(int *)(PTR_PTR_0045e534 + (uint)param_1 * 0x24 + 4);
      DAT_0046c44c = (int)DAT_00486a0c - *(int *)(PTR_PTR_0045e534 + (uint)param_1 * 0x24 + 8);
    }
    else {
      uVar1 = FUN_004078f4(param_1,&local_14);
      local_10 = CONCAT22(extraout_var,uVar1);
      local_c = *(int *)(&DAT_00486f90 + local_14 * 4);
      if (*(int *)(local_c + 0x10) == 0) {
        FUN_00407254(local_c);
      }
      if (local_10 == 0) {
        local_8 = *(int **)(local_c + 0x10);
      }
      else {
        local_8 = *(int **)(local_c + 0x14);
      }
      (**(code **)(*local_8 + 0x58))(local_8,local_80);
      DAT_0046c448 = (int)DAT_00486a0a - (local_74 >> 1);
      DAT_0046c44c = (int)DAT_00486a0c - (local_78 >> 1);
      *(uint *)(PTR_PTR_0045e534 + 0x484) = local_74 >> 1;
      *(uint *)(PTR_PTR_0045e534 + 0x488) = local_78 >> 1;
    }
    DAT_0046c458 = local_74;
    DAT_0046c45c = local_78;
    (**(code **)(*DAT_0045dbd4 + 0x1c))(DAT_0045dbd4,0,0,local_8,&DAT_0046c450,0x10);
    FUN_0040e29f();
    FUN_0040e23e();
    DAT_0045dbd0 = 1;
    DAT_00476338 = param_1;
    if ((param_1 < 0x20) && (1 < *(int *)(PTR_PTR_0045e534 + (uint)param_1 * 0x24 + 0x10))) {
      FUN_0040ed39();
    }
  }
  return;
}

