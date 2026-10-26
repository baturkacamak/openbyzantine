// rea decompile_all
// name: FUN_0040ae93
// address: 0x40ae93
// body: available
// range: 0x40ae93-0x40b083 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __thiscall FUN_0040ae93(void *this,undefined4 param_1,undefined4 param_2,int param_3)

{
  float10 fVar1;
  float10 extraout_ST0;
  longlong lVar2;
  undefined4 local_28;
  undefined1 local_24 [4];
  undefined4 local_20;
  undefined4 local_1c;
  float local_18;
  uint local_14;
  int local_10;
  undefined4 local_c;
  int local_8;
  
  *(undefined4 *)((int)this + 0x20) = 1;
  *(undefined4 *)((int)this + 8) = param_1;
  *(undefined4 *)((int)this + 0x18) = param_2;
  *(undefined4 *)((int)this + 0x2c) = 1;
  *(undefined4 *)((int)this + 0x38) = 0;
  *(undefined4 *)((int)this + 0x40) = 0;
  *(undefined4 *)((int)this + 0x3c) = *(undefined4 *)((int)this + 0x40);
  *(undefined4 *)((int)this + 0x24) = 0;
  if (DAT_00462ae8 == 0) {
    fVar1 = (float10)(uint)(*(int *)((int)this + 0x18) * param_3) / (float10)88200.0;
  }
  else {
    __adj_fdiv_m32(0x47ac4400);
    fVar1 = extraout_ST0;
  }
  local_18 = (float)fVar1;
  _ceil((double)local_18);
  lVar2 = __ftol();
  *(int *)((int)this + 0x1c) = (int)lVar2;
  *(undefined4 *)((int)this + 0x28) = 0;
  local_10 = (**(code **)(**(int **)((int)this + 4) + 0x2c))
                       (*(undefined4 *)((int)this + 4),0,*(undefined4 *)((int)this + 0x14),&local_8,
                        &local_1c,&local_c,local_24,0);
  if (local_10 == 0) {
    local_20 = AVIStreamRead(*(undefined4 *)((int)this + 8),0,
                             *(uint *)((int)this + 0x14) / *(uint *)((int)this + 0x18),local_8,
                             local_1c,&local_14,&local_28);
    if (local_14 < *(uint *)((int)this + 0x14)) {
      OutputDebugStringA(s_Size_is_less_than_a_full_segment_0045da08);
      _memset((void *)(local_14 + local_8),0,*(int *)((int)this + 0x14) - local_14);
      *(undefined4 *)((int)this + 0x28) = 1;
    }
    local_10 = (**(code **)(**(int **)((int)this + 4) + 0x4c))
                         (*(undefined4 *)((int)this + 4),local_8,local_14,local_c,0);
    *(undefined4 *)((int)this + 0x30) = local_28;
    *(undefined4 *)((int)this + 0x34) = 2;
    (**(code **)(**(int **)((int)this + 4) + 0x34))(*(undefined4 *)((int)this + 4),0);
    if (*(uint *)((int)this + 0x1c) <= *(uint *)((int)this + 0x34)) {
      OutputDebugStringA(s______DSound__Timer_Done_Writing_0045da2c);
      *(undefined4 *)((int)this + 0x28) = 1;
    }
  }
  else {
    local_20 = 0xffffffff;
  }
  return local_20;
}

