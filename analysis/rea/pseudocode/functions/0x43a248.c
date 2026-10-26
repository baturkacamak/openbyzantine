// rea decompile_all
// name: FUN_0043a248
// address: 0x43a248
// body: available
// range: 0x43a248-0x43a3ac inclusive
// range: 0x43a3b2-0x43a4cb inclusive
// range: 0x43a4d1-0x43a6c5 inclusive
// range: 0x43a6cb-0x43a80b inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __thiscall FUN_0043a248(void *this,int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  int local_c;
  undefined4 local_8;
  
  uVar1 = FUN_0044ac60(*(SIZE_T *)((int)this + 0x3c));
  *(undefined4 *)((int)this + 0x5c) = uVar1;
  uVar1 = FUN_0044c2f0(0x34);
  *(undefined4 *)((int)this + 0x58) = uVar1;
  _memset(*(void **)((int)this + 0x58),0,0x28);
  iVar2 = FUN_0043d5f7(*(undefined4 **)((int)this + 0xc));
  if ((iVar2 == *(int *)((int)this + 0x40)) ||
     (iVar2 = FUN_0043d52b(*(void **)((int)this + 0xc),*(undefined4 *)((int)this + 0x40)),
     iVar2 == 0)) {
    FUN_0043d612(*(void **)((int)this + 0xc),*(undefined4 *)((int)this + 0x54),
                 *(undefined4 *)((int)this + 0x58));
    *(undefined4 *)((int)this + 0x44) = *(undefined4 *)(*(int *)((int)this + 0x54) + 4);
    *(undefined4 *)((int)this + 0x48) = *(undefined4 *)(*(int *)((int)this + 0x54) + 8);
    if (*(int *)((int)this + 0x40) == 0x31345649) {
      if (param_1 == 0) {
        FUN_0043d9c7(*(void **)((int)this + 0xc),0);
        *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)((int)this + 0x44);
        *(undefined4 *)((int)this + 0x50) = *(undefined4 *)((int)this + 0x48);
      }
      else if (param_1 == 1) {
        FUN_0043d9c7(*(void **)((int)this + 0xc),1);
        *(int *)((int)this + 0x4c) = *(int *)((int)this + 0x44) * 2;
        *(int *)((int)this + 0x50) = *(int *)((int)this + 0x48) * 2;
      }
      else if (param_1 == 2) {
        FUN_0043d9c7(*(void **)((int)this + 0xc),2);
        *(int *)((int)this + 0x4c) = *(int *)((int)this + 0x44) * 2;
        *(int *)((int)this + 0x50) = *(int *)((int)this + 0x48) * 2;
      }
      FUN_00413e27(*(void **)((int)this + 8),(undefined4 *)(*(int *)((int)this + 0x58) + 0x28),
                   (undefined4 *)(*(int *)((int)this + 0x58) + 0x2c),
                   (undefined4 *)(*(int *)((int)this + 0x58) + 0x30));
      *(undefined4 *)(*(int *)((int)this + 0x58) + 0x10) = 3;
      *(undefined2 *)(*(int *)((int)this + 0x58) + 0xe) = 0x10;
      *(undefined4 *)(*(int *)((int)this + 0x58) + 0x14) = 0;
      *(undefined2 *)(*(int *)((int)this + 0x58) + 0xc) = 1;
      *(undefined4 *)(*(int *)((int)this + 0x58) + 4) = *(undefined4 *)((int)this + 0x4c);
      *(int *)(*(int *)((int)this + 0x58) + 8) = -*(int *)((int)this + 0x50);
      iVar2 = FUN_0043d643(*(void **)((int)this + 0xc),0,*(undefined4 *)((int)this + 0x54),
                           *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44),
                           *(undefined4 *)((int)this + 0x48),*(undefined4 *)((int)this + 0x58),0,
                           *(undefined4 *)((int)this + 0x2c),*(undefined4 *)((int)this + 0x30),
                           *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
      if (iVar2 != 0) {
        return 0xffffffff;
      }
      FUN_0043d6c0(*(void **)((int)this + 0xc),0,*(undefined4 *)((int)this + 0x54),
                   *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44),
                   *(undefined4 *)((int)this + 0x48),*(undefined4 *)((int)this + 0x58),0,0,0,
                   *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
    }
    else {
      *(undefined4 *)(*(int *)((int)this + 0x58) + 0x10) = 3;
      puVar3 = (undefined4 *)(*(int *)((int)this + 0x58) + 0x28);
      puVar4 = (undefined4 *)(*(int *)((int)this + 0x58) + 0x2c);
      puVar5 = (undefined4 *)(*(int *)((int)this + 0x58) + 0x30);
      FUN_00413e27(*(void **)((int)this + 8),puVar3,puVar4,puVar5);
      *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)((int)this + 0x44);
      *(undefined4 *)((int)this + 0x50) = *(undefined4 *)((int)this + 0x48);
      *(undefined4 *)(*(int *)((int)this + 0x58) + 0x10) = 3;
      *(undefined2 *)(*(int *)((int)this + 0x58) + 0xe) = 0x10;
      *(undefined4 *)(*(int *)((int)this + 0x58) + 0x14) = 0;
      *(undefined2 *)(*(int *)((int)this + 0x58) + 0xc) = 1;
      *(undefined4 *)(*(int *)((int)this + 0x58) + 4) = *(undefined4 *)((int)this + 0x4c);
      *(int *)(*(int *)((int)this + 0x58) + 8) = -*(int *)((int)this + 0x50);
      local_c = FUN_0043d643(*(void **)((int)this + 0xc),0,*(undefined4 *)((int)this + 0x54),
                             *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44)
                             ,*(undefined4 *)((int)this + 0x48),*(undefined4 *)((int)this + 0x58),0,
                             *(undefined4 *)((int)this + 0x2c),*(undefined4 *)((int)this + 0x30),
                             *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
      if (local_c != 0) {
        *(undefined4 *)(*(int *)((int)this + 0x58) + 0x10) = 0;
        *puVar3 = 0;
        *puVar4 = 0;
        *puVar5 = 0;
        local_c = FUN_0043d643(*(void **)((int)this + 0xc),0,*(undefined4 *)((int)this + 0x54),
                               *(undefined4 *)((int)this + 0x5c),0,0,
                               *(undefined4 *)((int)this + 0x44),*(undefined4 *)((int)this + 0x48),
                               *(undefined4 *)((int)this + 0x58),0,*(undefined4 *)((int)this + 0x2c)
                               ,*(undefined4 *)((int)this + 0x30),*(undefined4 *)((int)this + 0x4c),
                               *(undefined4 *)((int)this + 0x50));
      }
      if (local_c != 0) {
        return 0xffffffff;
      }
      FUN_0043d6c0(*(void **)((int)this + 0xc),0,*(undefined4 *)((int)this + 0x54),
                   *(undefined4 *)((int)this + 0x5c),0,0,*(undefined4 *)((int)this + 0x44),
                   *(undefined4 *)((int)this + 0x48),*(undefined4 *)((int)this + 0x58),0,0,0,
                   *(undefined4 *)((int)this + 0x4c),*(undefined4 *)((int)this + 0x50));
    }
    *(undefined4 *)((int)this + 0x84) = 0;
    *(undefined4 *)((int)this + 0x88) = 0;
    *(undefined4 *)((int)this + 0x8c) = 0;
    FUN_0043ad1e(this,0,&local_8);
    if (param_1 == 2) {
      FUN_00414a8b(*(void **)((int)this + 8),*(undefined4 *)((int)this + 0x2c),
                   *(int *)((int)this + 0x30),*(undefined4 *)((int)this + 0x4c),
                   *(int *)((int)this + 0x50));
    }
    FUN_0043adce(this,0,local_8);
    if (param_2 == 0) {
      iVar2 = FUN_0043af73(this,1);
      if (iVar2 == 0) {
        *(int *)((int)this + 0x84) = *(int *)((int)this + 0x84) + 1;
      }
    }
    else {
      FUN_00414b2b(*(void **)((int)this + 8),param_1);
    }
    if (param_1 == 2) {
      FUN_00414a8b(*(void **)((int)this + 8),*(undefined4 *)((int)this + 0x2c),
                   *(int *)((int)this + 0x30),*(undefined4 *)((int)this + 0x4c),
                   *(int *)((int)this + 0x50));
    }
    *(undefined4 *)((int)this + 0x74) = 0;
    uVar1 = 0;
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}

