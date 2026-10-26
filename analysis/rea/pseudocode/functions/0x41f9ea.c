// rea decompile_all
// name: FUN_0041f9ea
// address: 0x41f9ea
// body: available
// range: 0x41f9ea-0x41fa47 inclusive
// range: 0x41fa4d-0x41fa60 inclusive
// range: 0x41fa79-0x41fc31 inclusive
// range: 0x41fc37-0x41fc4d inclusive
// range: 0x41fc62-0x41fc86 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_0041f9ea(undefined4 param_1)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  int *local_18;
  int local_14 [2];
  ushort *local_c;
  char *local_8;
  
  local_14[0] = 0x280;
  local_14[1] = 0x1e0;
  switch(param_1) {
  case 0:
    local_8 = s_setupart_preffs_jpg_0045fb3c;
    break;
  case 1:
  case 2:
    local_8 = s_setupart_setup00_jpg_0045fb24;
    break;
  case 3:
    local_8 = s_setupart_s_note00_jpg_0045fb50;
    break;
  case 4:
    local_8 = s_setupart_s_note01_jpg_0045fb68;
    break;
  case 5:
    local_8 = s_setupart_s_note02_jpg_0045fb80;
  }
  local_c = FUN_0040f135(local_8,local_14,(uint *)(local_14 + 1),DAT_00477344,0x280);
  if (local_c == (ushort *)0x0) {
    param_1 = 0xffffffff;
  }
  else {
    FUN_0042b2db(DAT_0047a8a4,DAT_00477344,0x4b000);
    FUN_0041fe46(param_1);
    switch(param_1) {
    case 1:
      if ((DAT_0045f490 == 0) || (DAT_0045f490 == 3)) {
        local_28 = 0;
        local_24 = 0;
        local_20 = 0xa8;
        local_1c = 0xa8;
        local_18 = (int *)FUN_00418040((int)DAT_00477344);
        puVar4 = &local_28;
        uVar1 = FUN_00418040(DAT_0046e82c);
        FUN_004207a6(DAT_0045f7c8,uVar1,puVar4);
        (**(code **)(*local_18 + 0x1c))(local_18,0x45,0x2c,DAT_0045f7c8,0,0x10);
        local_18 = (int *)FUN_00418040((int)DAT_0047a8a4);
        (**(code **)(*local_18 + 0x1c))(local_18,0x45,0x2c,DAT_0045f7c8,0,0x10);
        iVar5 = 1;
        uVar1 = 0x27;
        iVar3 = 0xe2;
        piVar2 = (int *)FUN_00418040((int)DAT_0047a8a4);
        FUN_00416974(s_setupart_pprclip_tga_0045fb98,piVar2,iVar3,uVar1,iVar5);
        iVar5 = 1;
        uVar1 = 0x27;
        iVar3 = 0xe2;
        piVar2 = (int *)FUN_00418040((int)DAT_00477344);
        FUN_00416974(s_setupart_pprclip_tga_0045fbb0,piVar2,iVar3,uVar1,iVar5);
      }
      FUN_004202ff(0x29,0);
      FUN_00420587(0xf,0);
      break;
    case 2:
      FUN_00420587(0x10,0);
      FUN_00421ffc(0x29,0);
      break;
    case 4:
      FUN_00420dc9(0x29,0);
      break;
    case 5:
      FUN_00421949(0,0);
    }
    FUN_0041842b((int)DAT_00477344,0x4b000);
    FUN_0041f98e();
  }
  return param_1;
}

