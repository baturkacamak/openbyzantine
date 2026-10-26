// rea decompile_all
// name: FUN_00440e20
// address: 0x440e20
// body: available
// range: 0x440e20-0x440f5d inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


void __cdecl FUN_00440e20(int *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  char *_Src;
  int iVar3;
  char cStack_5;
  int local_4;
  
  param_1[1] = 0;
  local_4 = FUN_00442350();
  puVar1 = (undefined4 *)FUN_004422e0(param_1,0x50);
  if (puVar1 == (undefined4 *)0x0) {
    FUN_00442360();
    *(undefined4 *)(*param_1 + 0x14) = 0x35;
    *(undefined4 *)(*param_1 + 0x18) = 0;
    (**(code **)*param_1)(param_1);
  }
  *puVar1 = FUN_00440f60;
  puVar1[1] = FUN_004410a0;
  puVar1[2] = FUN_00441140;
  puVar1[3] = FUN_004411f0;
  puVar1[4] = &LAB_004412b0;
  puVar1[5] = &LAB_00441320;
  puVar1[6] = FUN_00441390;
  puVar1[7] = &LAB_00441530;
  puVar1[8] = &LAB_00441700;
  iVar3 = 1;
  puVar1[9] = FUN_004418d0;
  puVar1[10] = &LAB_004419d0;
  puVar1[0xb] = local_4;
  puVar2 = puVar1 + 0xd;
  do {
    *puVar2 = 0;
    iVar3 = iVar3 + -1;
    puVar2[2] = 0;
    puVar2 = puVar2 + -1;
  } while (-1 < iVar3);
  puVar1[0x10] = 0;
  puVar1[0x11] = 0;
  puVar1[0x12] = 0x50;
  param_1[1] = (int)puVar1;
  _Src = _getenv(s_JPEGMEM_00462ad4);
  if (_Src != (char *)0x0) {
    cStack_5 = 'x';
    iVar3 = _sscanf(_Src,s__ld_c_00462acc,&local_4,&cStack_5);
    if (0 < iVar3) {
      if ((cStack_5 == 'm') || (cStack_5 == 'M')) {
        local_4 = local_4 * 1000;
      }
      puVar1[0xb] = local_4 * 1000;
    }
  }
  return;
}

