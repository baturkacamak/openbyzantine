// rea decompile_all
// name: FUN_00448170
// address: 0x448170
// body: available
// range: 0x448170-0x44824d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


int __cdecl FUN_00448170(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  int local_8;
  
  iVar1 = param_1[0x19];
  iVar2 = param_1[0x15];
  iVar7 = 1;
  do {
    iVar5 = iVar7;
    iVar7 = iVar5 + 1;
    iVar6 = iVar7;
    if (1 < iVar1) {
      iVar4 = iVar1 + -1;
      do {
        iVar6 = iVar6 * iVar7;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
  } while (iVar6 <= iVar2);
  if (iVar5 < 2) {
    *(undefined4 *)(*param_1 + 0x14) = 0x37;
    *(int *)(*param_1 + 0x18) = iVar6;
    (**(code **)*param_1)(param_1);
  }
  local_8 = 1;
  iVar7 = iVar1;
  piVar8 = param_2;
  if (0 < iVar1) {
    for (; iVar6 = iVar1, iVar7 != 0; iVar7 = iVar7 + -1) {
      *piVar8 = iVar5;
      piVar8 = piVar8 + 1;
    }
    do {
      local_8 = iVar5 * local_8;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
  }
  do {
    iVar7 = 0;
    bVar3 = false;
    if (0 < iVar1) {
      do {
        iVar6 = iVar7;
        if (param_1[10] == 2) {
          iVar6 = (&DAT_0045b7b8)[iVar7];
        }
        iVar5 = (local_8 / param_2[iVar6]) * (param_2[iVar6] + 1);
        if (iVar2 < iVar5) break;
        iVar7 = iVar7 + 1;
        param_2[iVar6] = param_2[iVar6] + 1;
        bVar3 = true;
        local_8 = iVar5;
      } while (iVar7 < iVar1);
    }
    if (!bVar3) {
      return local_8;
    }
  } while( true );
}

