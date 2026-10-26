// rea decompile_all
// name: FUN_0040c329
// address: 0x40c329
// body: available
// range: 0x40c329-0x40c38c inclusive
// range: 0x40c392-0x40c5ef inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_0040c329(void)

{
  bool bVar1;
  ushort uVar2;
  uint uVar3;
  undefined2 extraout_var_00;
  undefined2 extraout_var_01;
  undefined2 extraout_var_02;
  undefined3 extraout_var;
  undefined2 extraout_var_03;
  uint local_8;
  
  FUN_0041624a();
  uVar3 = FUN_0040c02c();
  local_8 = uVar3 & 0xffff;
  if ((uVar3 & 0xffff) == 0xfffc) {
    if (_DAT_00476330 == 0xfffc) {
      uVar3 = FUN_00408fe7();
    }
    else {
      FUN_00408f32(1);
      uVar3 = uVar3 & 0xffff;
      _DAT_00476330 = uVar3;
    }
    uVar3 = CONCAT22((short)(uVar3 >> 0x10),0xfffe);
  }
  else {
    if (_DAT_00476330 == 0xfffc) {
      FUN_00408f32(0);
      _DAT_00476330 = 0xfffe;
    }
    if ((uVar3 & 0xffff) == 0xfffb) {
      FUN_0040df05(0xb);
      uVar3 = CONCAT22(extraout_var_00,0xfffe);
    }
    else if ((uVar3 & 0xffff) == 0xfffd) {
      FUN_004083d4();
      uVar3 = CONCAT22(extraout_var_01,0xfffe);
    }
    else if ((uVar3 & 0xffff) == 0xffff) {
      FUN_0040df05(DAT_0045da80);
      uVar3 = CONCAT22(extraout_var_02,DAT_00476330);
    }
    else {
      if (DAT_004603d4 != -1) {
        FUN_004177c7((char *)0x0);
      }
      if ((uVar3 & 0xffff) == 0xfffe) {
        uVar3 = FUN_00407bd7(0);
        uVar2 = (ushort)uVar3;
      }
      else if (DAT_0047a8a2 == '\0') {
        uVar2 = (ushort)*(byte *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9);
        DAT_004603d4 = *(int *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 0x14);
        bVar1 = FUN_00407c5a(*(int *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 0x10),
                             *(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9));
        if (DAT_0045d1c8 != 0) {
          if (CONCAT31(extraout_var,bVar1) == 0) {
            uVar3 = FUN_00407bd7(0);
            uVar2 = (ushort)uVar3;
          }
          else if (((*(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9) == '\x12') ||
                   (*(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9) == '\x13')) ||
                  (*(char *)(*(int *)(&DAT_00464300 + (uVar3 & 0xffff) * 4) + 9) == '\x14')) {
            uVar3 = FUN_00407bd7(1);
            uVar2 = (ushort)uVar3;
          }
          else if (uVar2 == 0) {
            uVar3 = FUN_00407bd7(0);
            uVar2 = (ushort)uVar3;
          }
        }
        if ((CONCAT31(extraout_var,bVar1) == 0) && (uVar2 != 0x14)) {
          local_8 = 0xfffe;
        }
      }
      else {
        uVar2 = (short)uVar3 + 0xe;
      }
      FUN_0040df05(uVar2);
      _DAT_00476330 = local_8;
      uVar3 = CONCAT22(extraout_var_03,(undefined2)local_8);
      DAT_0045da80 = uVar2;
    }
  }
  return uVar3;
}

