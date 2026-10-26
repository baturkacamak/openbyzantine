// rea decompile_all
// name: FUN_00431b0f
// address: 0x431b0f
// body: available
// range: 0x431b0f-0x431d56 inclusive
// range: 0x431d5c-0x431d7b inclusive
// range: 0x431e31-0x431e88 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __cdecl FUN_00431b0f(uint param_1,undefined1 param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint local_48 [17];
  
  if (((*(char *)(*(int *)(&DAT_0047dd70 + (param_1 & 0xffff) * 4) + 8) == '\0') ||
      (*(char *)(*(int *)(&DAT_0047dd70 + (param_1 & 0xffff) * 4) + 8) == '\x01')) &&
     (*(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4) == 0)) {
    if (((DAT_0045f068 != 0) && (iVar1 = FUN_00438afa(), iVar1 == 4)) &&
       (iVar1 = _strncmp((char *)**(undefined4 **)(&DAT_0047dd70 + (param_1 & 0xffff) * 4),
                         &DAT_00460f80,2), iVar1 == 0)) {
      return 1;
    }
    FUN_0040fe70(param_1,(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4));
    FUN_00438cd0();
  }
  if (*(int *)(&DAT_004649e8 + (param_1 & 0xffff) * 4) == 0) {
    FUN_0044ba00(local_48,(uint *)&DAT_00460f84);
    switch(param_2) {
    case 0x3a:
      FUN_0044ba00(local_48,(uint *)s_DIALOGUE_00460fd4);
      break;
    case 0x50:
      FUN_0044ba00(local_48,(uint *)s_PHONEME_00460fe0);
      break;
    case 100:
      break;
    case 0x84:
      FUN_0044ba08(local_48,(uint *)&DAT_00460f8c);
      break;
    case 0x85:
      FUN_0044ba08(local_48,(uint *)&DAT_00460f90);
      break;
    case 0x86:
      FUN_0044ba08(local_48,(uint *)&DAT_00460f94);
      break;
    case 0x87:
      FUN_0044ba08(local_48,(uint *)&DAT_00460f9c);
      break;
    case 0x88:
      FUN_0044ba08(local_48,(uint *)&DAT_00460fa0);
      break;
    case 0x89:
      FUN_0044ba08(local_48,(uint *)&DAT_00460fa8);
      break;
    case 0x8a:
      FUN_0044ba08(local_48,(uint *)&DAT_00460fb0);
      break;
    case 0x8b:
      FUN_0044ba08(local_48,(uint *)&DAT_00460fc0);
      break;
    case 0x8c:
      FUN_0044ba00(local_48,(uint *)s_SUSPEND_00460fc4);
      break;
    case 0x8e:
      FUN_0044ba00(local_48,(uint *)s_UNSUS_00460fcc);
      break;
    case 0x95:
      FUN_0044ba08(local_48,(uint *)&DAT_00460fb8);
      break;
    case 0xa9:
      FUN_0044ba00(local_48,(uint *)s_COMPOSIT_00460fe8);
      break;
    case 0xaa:
      FUN_0044ba00(local_48,(uint *)s_PRISTINE_00460ff4);
    }
    FUN_0044ba08(local_48,(uint *)&DAT_00461000);
    FUN_0040ac43();
    FUN_0040ac38();
    uVar2 = 1;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}

