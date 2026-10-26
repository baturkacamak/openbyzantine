// rea decompile_all
// name: FUN_0040c02c
// address: 0x40c02c
// body: available
// range: 0x40c02c-0x40c328 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


uint FUN_0040c02c(void)

{
  uint uVar1;
  undefined2 extraout_var;
  int iVar2;
  undefined2 extraout_var_00;
  uint local_18;
  uint local_14;
  undefined4 local_10;
  int local_c;
  int local_8;
  
  if (DAT_00482acd == '\0') {
    FUN_0041624a();
    if ((DAT_00486a0c < 0x14) && (DAT_0045f498 == 0)) {
      DAT_004859b4 = 0xffff;
      DAT_00483050 = 0xffff;
      uVar1 = CONCAT22(extraout_var,0xfffc);
    }
    else if (DAT_00486a0c < 0x1a4) {
      iVar2 = FUN_00417dc7((int)DAT_00486a0a,(int)DAT_00486a0c);
      if (iVar2 == 0) {
        local_c = (int)DAT_00486a0a;
        local_8 = (int)DAT_00486a0c;
        FUN_00401ca6(&local_c,(int *)&local_18);
        if ((DAT_004859b0 == local_18) && (DAT_004859b4 == local_14)) {
          uVar1 = CONCAT22(extraout_var_00,0xffff);
        }
        else {
          local_10 = (uint)local_10._2_2_ << 0x10;
          while ((local_10 & 0xffff) < (uint)DAT_0048306e) {
            if ((*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) != 0) &&
               (*(char *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 8) != '\0')) {
              if (*(char *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 9) == '\x02') {
                iVar2 = FUN_0040d84d();
                if (iVar2 == 5) {
                  DAT_004859b0 = (uint)**(ushort **)(&DAT_00464300 + (local_10 & 0xffff) * 4);
                  DAT_004859b4 = (uint)*(ushort *)
                                        (*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 2);
                  break;
                }
              }
              if (((((int)(uint)**(ushort **)(&DAT_00464300 + (local_10 & 0xffff) * 4) <=
                     (int)local_18) &&
                   ((int)local_18 <=
                    (int)((uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 4) +
                         (uint)**(ushort **)(&DAT_00464300 + (local_10 & 0xffff) * 4)))) &&
                  ((int)(uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 2) <=
                   (int)local_14)) &&
                 ((int)local_14 <=
                  (int)((uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 2) +
                       (uint)*(ushort *)(*(int *)(&DAT_00464300 + (local_10 & 0xffff) * 4) + 6)))) {
                DAT_004859b0 = local_18;
                DAT_004859b4 = local_14;
                break;
              }
            }
            local_10 = CONCAT22(local_10._2_2_,(short)local_10 + 1);
          }
          if ((local_10 & 0xffff) == (uint)DAT_0048306e) {
            local_10 = 0xfffe;
          }
          if ((local_10 & 0xffff) == DAT_00483050) {
            local_10 = 0xffff;
          }
          else {
            DAT_00483050 = local_10 & 0xffff;
          }
          uVar1 = local_10 & 0xffff;
        }
      }
      else {
        DAT_004859b4 = 0xffff;
        DAT_00483050 = 0xffff;
        uVar1 = CONCAT22((short)((uint)iVar2 >> 0x10),0xfffb);
      }
    }
    else {
      DAT_004859b4 = 0xffff;
      DAT_00483050 = 0xffff;
      uVar1 = 0xfffd;
    }
  }
  else {
    uVar1 = 0xfffe;
  }
  return uVar1;
}

