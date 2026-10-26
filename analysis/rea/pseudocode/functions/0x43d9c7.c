// rea decompile_all
// name: FUN_0043d9c7
// address: 0x43d9c7
// body: available
// range: 0x43d9c7-0x43dabd inclusive
// range: 0x43dac3-0x43db2d inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __thiscall FUN_0043d9c7(void *this,int param_1)

{
  undefined4 uVar1;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_84;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_8;
  
  if (*(int *)this == 0x31345649) {
    local_a4 = 0x2c;
    local_a0 = 0x31345649;
    local_9c = 0x10002;
    local_98 = 4;
    local_94 = 2;
    local_78 = 0x70;
    local_74 = 0x31345649;
    local_70 = 0x10002;
    local_6c = 2;
    local_68 = 2;
    local_84 = 2;
    local_64 = 0x80000001;
    local_60 = 0x32;
    if (param_1 == 1) {
      local_7c = 0;
    }
    else if (param_1 == 2) {
      local_7c = 1;
    }
    else {
      local_7c = 0;
    }
    local_90 = 0x8000000a;
    local_8 = ICSendMessage(*(undefined4 *)((int)this + 4),0x5001,&local_78,0x70);
    uVar1 = ICSendMessage(*(undefined4 *)((int)this + 4),0x5001,&local_a4,0x2c);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

