// rea decompile_all
// name: FUN_00456360
// address: 0x456360
// body: available
// range: 0x456360-0x456591 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00456360(void)

{
  byte bVar1;
  byte bVar2;
  byte *pbVar3;
  DWORD DVar4;
  int iVar5;
  long lVar6;
  uint uVar7;
  uint uVar8;
  byte *pbVar9;
  byte *pbVar10;
  bool bVar11;
  _TIME_ZONE_INFORMATION local_ac;
  
  pbVar3 = (byte *)_getenv(&DAT_00463efc);
  if (pbVar3 == (byte *)0x0) {
    DVar4 = GetTimeZoneInformation(&local_ac);
    if (DVar4 != 0xffffffff) {
      DAT_00463f00 = local_ac.Bias * 0x3c;
      if (local_ac.StandardDate.wMonth != 0) {
        DAT_00463f00 = DAT_00463f00 + local_ac.StandardBias * 0x3c;
      }
      if ((local_ac.DaylightDate.wMonth == 0) || (local_ac.DaylightBias == 0)) {
        DAT_00463f04 = 0;
      }
      else {
        DAT_00463f04 = 1;
      }
      *PTR_DAT_00463f28 = 0;
      *PTR_DAT_00463f2c = 0;
      return;
    }
  }
  else if (*pbVar3 != 0) {
    pbVar10 = pbVar3;
    pbVar9 = DAT_00463ef4;
    if (DAT_00463ef4 != (byte *)0x0) {
      do {
        bVar1 = *pbVar10;
        bVar11 = bVar1 < *pbVar9;
        if (bVar1 != *pbVar9) {
LAB_00456445:
          iVar5 = (1 - (uint)bVar11) - (uint)(bVar11 != 0);
          goto LAB_0045644a;
        }
        if (bVar1 == 0) break;
        bVar1 = pbVar10[1];
        bVar11 = bVar1 < pbVar9[1];
        if (bVar1 != pbVar9[1]) goto LAB_00456445;
        pbVar10 = pbVar10 + 2;
        pbVar9 = pbVar9 + 2;
      } while (bVar1 != 0);
      iVar5 = 0;
LAB_0045644a:
      if (iVar5 == 0) {
        return;
      }
    }
    FUN_0044ac40(DAT_00463ef4);
    uVar7 = 0xffffffff;
    pbVar10 = pbVar3;
    do {
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      bVar1 = *pbVar10;
      pbVar10 = pbVar10 + 1;
    } while (bVar1 != 0);
    DAT_00463ef4 = (byte *)FUN_0044c2f0(~uVar7);
    if (DAT_00463ef4 != (byte *)0x0) {
      uVar7 = 0xffffffff;
      pbVar10 = pbVar3;
      do {
        pbVar9 = pbVar10;
        if (uVar7 == 0) break;
        uVar7 = uVar7 - 1;
        pbVar9 = pbVar10 + 1;
        bVar1 = *pbVar10;
        pbVar10 = pbVar9;
      } while (bVar1 != 0);
      uVar7 = ~uVar7;
      pbVar10 = pbVar9 + -uVar7;
      pbVar9 = DAT_00463ef4;
      for (uVar8 = uVar7 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
        *(undefined4 *)pbVar9 = *(undefined4 *)pbVar10;
        pbVar10 = pbVar10 + 4;
        pbVar9 = pbVar9 + 4;
      }
      for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
        *pbVar9 = *pbVar10;
        pbVar10 = pbVar10 + 1;
        pbVar9 = pbVar9 + 1;
      }
      pbVar10 = pbVar3 + 3;
      FUN_0044c630(PTR_DAT_00463f28,(char *)pbVar3,3);
      bVar1 = *pbVar10;
      if (bVar1 == 0x2d) {
        pbVar10 = pbVar3 + 4;
      }
      lVar6 = _atol((char *)pbVar10);
      DAT_00463f00 = lVar6 * 0xe10;
      for (; (bVar2 = *pbVar10, bVar2 == 0x2b || (('/' < (char)bVar2 && ((char)bVar2 < ':'))));
          pbVar10 = pbVar10 + 1) {
      }
      if (*pbVar10 == 0x3a) {
        pbVar10 = pbVar10 + 1;
        lVar6 = _atol((char *)pbVar10);
        DAT_00463f00 = DAT_00463f00 + lVar6 * 0x3c;
        bVar2 = *pbVar10;
        while (('/' < (char)bVar2 && ((char)*pbVar10 < ':'))) {
          pbVar10 = pbVar10 + 1;
          bVar2 = *pbVar10;
        }
        if (*pbVar10 == 0x3a) {
          pbVar10 = pbVar10 + 1;
          lVar6 = _atol((char *)pbVar10);
          DAT_00463f00 = DAT_00463f00 + lVar6;
          bVar2 = *pbVar10;
          while (('/' < (char)bVar2 && ((char)*pbVar10 < ':'))) {
            pbVar10 = pbVar10 + 1;
            bVar2 = *pbVar10;
          }
        }
      }
      if (bVar1 == 0x2d) {
        DAT_00463f00 = -DAT_00463f00;
      }
      DAT_00463f04 = (int)(char)*pbVar10;
      if (DAT_00463f04 != 0) {
        FUN_0044c630(PTR_DAT_00463f2c,(char *)pbVar10,3);
        return;
      }
      *PTR_DAT_00463f2c = 0;
    }
  }
  return;
}

