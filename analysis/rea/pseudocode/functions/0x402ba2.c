// rea decompile_all
// name: FUN_00402ba2
// address: 0x402ba2
// body: available
// range: 0x402ba2-0x402db7 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00402ba2(void)

{
  DWORD DVar1;
  undefined2 uVar3;
  undefined4 uVar2;
  int local_8c;
  undefined4 local_88 [9];
  int local_64;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  local_10 = 0;
  DVar1 = timeGetTime();
  if (DAT_0045c08c + _DAT_0045c088 < DVar1) {
    if (DAT_0045c090 == 0) {
      if (DAT_0045c084 == 0xff) {
        DAT_0045c090 = 1;
      }
      else {
        DAT_0045c084 = DAT_0045c084 * '\x02' | 1;
      }
    }
    else if (DAT_0045c084 == 0) {
      DAT_0045c090 = 0;
    }
    else {
      DAT_0045c084 = DAT_0045c084 >> 1;
    }
    _DAT_0045c088 = timeGetTime();
    _memset(local_88,0,0x6c);
    local_88[0] = 0x6c;
    (**(code **)(*DAT_0045c078 + 100))(DAT_0045c078,0,local_88,1,0);
    local_8c = local_64;
    (**(code **)(*DAT_0045c07c + 100))(DAT_0045c07c,0,local_88,1,0);
    local_1c = local_64;
    local_8 = (local_10 * DAT_00468ca0 + local_c) * 2 + DAT_0045c040;
    for (local_14 = 0; local_14 < DAT_00468b94; local_14 = local_14 + 1) {
      for (local_18 = 0; local_18 < DAT_00468c9c; local_18 = local_18 + 1) {
        if (*(short *)(local_8c + local_18 * 2) != DAT_00485c88) {
          uVar3 = (undefined2)((uint)local_18 >> 0x10);
          uVar2 = FUN_004066c6(CONCAT22(uVar3,*(undefined2 *)(local_8c + local_18 * 2)),
                               CONCAT22(uVar3,*(undefined2 *)(local_1c + local_18 * 2)),
                               (uint)DAT_0045c084);
          *(short *)(local_8 + local_18 * 2) = (short)uVar2;
        }
      }
      local_8 = local_8 + DAT_00468ca0 * 2;
      local_8c = local_8c + DAT_00468c9c * 2;
      local_1c = local_1c + DAT_00468c9c * 2;
    }
    (**(code **)(*DAT_0045c078 + 0x80))(DAT_0045c078,0);
    (**(code **)(*DAT_0045c07c + 0x80))(DAT_0045c07c,0);
  }
  FUN_00402808();
  return;
}

