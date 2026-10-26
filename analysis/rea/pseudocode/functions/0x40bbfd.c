// rea decompile_all
// name: FUN_0040bbfd
// address: 0x40bbfd
// body: available
// range: 0x40bbfd-0x40be2a inclusive
// range: 0x40be30-0x40be43 inclusive
// range: 0x40be54-0x40be7f inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl
FUN_0040bbfd(int param_1,int param_2,short param_3,short param_4,int *param_5,int param_6)

{
  ushort uVar1;
  ushort *puVar2;
  uint uVar3;
  int iVar4;
  undefined2 local_18;
  int local_10;
  
  do {
    puVar2 = (ushort *)(DAT_0046c438 * 2 + param_1);
    DAT_0046c438 = DAT_0046c438 + 1;
    uVar1 = *puVar2 & 0x3fff;
    switch((byte)(*puVar2 >> 0xe)) {
    case 0:
      if (param_4 + DAT_0046c428 < param_5[1]) {
        DAT_0046c430 = DAT_0046c430 + (short)uVar1;
        DAT_0046c438 = DAT_0046c438 + (short)uVar1;
      }
      else {
        for (local_10 = 0; local_10 < (short)uVar1; local_10 = local_10 + 1) {
          iVar4 = (param_3 + DAT_0046c430) - DAT_0046c43c;
          if (param_6 == 0) {
            local_18 = *(undefined2 *)(param_1 + DAT_0046c438 * 2);
          }
          else {
            uVar3 = FUN_0042b5b2((uint)*(ushort *)(DAT_0047a8a4 + DAT_0046c430 * 2));
            local_18 = (undefined2)uVar3;
          }
          if ((*param_5 <= iVar4) && (iVar4 <= param_5[2])) {
            *(undefined2 *)(param_2 + DAT_0046c430 * 2) = local_18;
          }
          DAT_0046c430 = DAT_0046c430 + 1;
          DAT_0046c438 = DAT_0046c438 + 1;
        }
      }
      break;
    case 1:
      DAT_0046c43c = DAT_0046c43c + DAT_0046c42c;
      DAT_0046c430 = (short)uVar1 + DAT_0046c43c;
      DAT_0046c428 = DAT_0046c428 + 1;
      break;
    case 2:
      if (param_6 == 0) {
        local_18 = *(undefined2 *)(param_1 + DAT_0046c438 * 2);
      }
      else {
        uVar3 = FUN_0042b5b2((uint)*(ushort *)(DAT_0047a8a4 + DAT_0046c430 * 2));
        local_18 = (undefined2)uVar3;
      }
      DAT_0046c438 = DAT_0046c438 + 1;
      if (param_4 + DAT_0046c428 < param_5[1]) {
        DAT_0046c430 = DAT_0046c430 + (short)uVar1;
      }
      else {
        for (local_10 = 0; local_10 < (short)uVar1; local_10 = local_10 + 1) {
          iVar4 = (param_3 + DAT_0046c430) - DAT_0046c43c;
          if ((*param_5 <= iVar4) && (iVar4 <= param_5[2])) {
            *(undefined2 *)(param_2 + DAT_0046c430 * 2) = local_18;
          }
          DAT_0046c430 = DAT_0046c430 + 1;
        }
      }
      break;
    case 3:
      DAT_0046c430 = DAT_0046c430 + (short)uVar1;
    }
  } while ((DAT_0046c428 < DAT_0046c434) && (param_4 + DAT_0046c428 <= param_5[3]));
  return;
}

