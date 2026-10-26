// rea decompile_all
// name: FUN_00406cfe
// address: 0x406cfe
// body: available
// range: 0x406cfe-0x406ef4 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void FUN_00406cfe(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  
  bVar1 = DAT_00485c78 & 0x1f;
  bVar2 = DAT_00485c80 & 0x1f;
  bVar3 = DAT_00485c80 & 0x1f;
  bVar4 = DAT_00485c78 & 0x1f;
  bVar5 = DAT_00485c80 & 0x1f;
  bVar6 = DAT_00485c80 & 0x1f;
  bVar7 = DAT_00485c78 & 0x1f;
  bVar8 = DAT_00485c78 & 0x1f;
  FUN_00406cbf();
  FUN_00405b90();
  DAT_00468ca4 = FUN_00405db2(s_fonts_dialog_sfn_0045d0bc);
  FUN_00405e7e((int)DAT_00468ca4,0,(ushort)(0x16 << bVar6) | (ushort)(0x1c << bVar7),0xffff);
  FUN_0040612a(s_fonts_gr_dlg_tga_0045d0d0,0);
  FUN_00406153(s_fonts_bk_dlg_tga_0045d0e4,0);
  DAT_00487708 = (uint)*(ushort *)(DAT_00487518 + 6);
  DAT_00468ca8 = FUN_00405db2(s_fonts_klio_sfn_0045d0f8);
  FUN_00405e7e((int)DAT_00468ca8,1,(short)(0x1b << bVar5),0xffff);
  FUN_0040612a(s_fonts_gr_klio_tga_0045d108,1);
  FUN_00406153(s_fonts_bk_klio_tga_0045d11c,1);
  DAT_00487508 = (uint)*(ushort *)(DAT_00487560 + 6);
  DAT_00468cac = FUN_00405db2(s_fonts_setup_sfn_0045d130);
  FUN_00405e7e((int)DAT_00468cac,2,0xf,(short)(0x1f << bVar8));
  FUN_0040612a(s_fonts_setup_tga_0045d140,2);
  DAT_00468cb0 = FUN_00405db2(s_fonts_klio_sfn_0045d150);
  FUN_00405e7e((int)DAT_00468cb0,3,(ushort)(0xb << bVar1) | (ushort)(0xe << bVar2) | 2,0xffff);
  DAT_00468cb4 = FUN_00405db2(s_fonts_captions_sfn_0045d160);
  FUN_00405e7e((int)DAT_00468cb4,4,(ushort)(0x1f << bVar3) | (ushort)(0 << bVar4) | 0xb,0xffff);
  FUN_0040612a(s_fonts_captions_tga_0045d174,4);
  return;
}

