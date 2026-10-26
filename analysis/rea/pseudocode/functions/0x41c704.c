// rea decompile_all
// name: FUN_0041c704
// address: 0x41c704
// body: available
// range: 0x41c704-0x41cae7 inclusive
// range: 0x41cb02-0x41cb12 inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


DWORD __thiscall FUN_0041c704(void *this,undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *unaff_FS_OFFSET;
  undefined4 *local_1b8;
  undefined4 local_1b0;
  undefined4 local_1ac;
  undefined4 local_1a8;
  undefined4 local_1a4;
  undefined4 local_1a0;
  undefined4 local_19c;
  undefined4 local_140;
  int local_13c;
  undefined1 local_138 [4];
  undefined4 local_134;
  undefined4 local_130;
  undefined1 local_110 [4];
  int local_10c;
  DWORD local_84;
  undefined4 local_80;
  undefined1 local_7c [16];
  undefined4 local_6c;
  undefined4 local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0041caf8;
  local_10 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_10;
  local_80 = 0;
  local_1ac = 0;
  local_84 = AVIFileOpenA(&local_1ac,param_1,0x20,0);
  if (local_84 == 0) {
    AVIFileInfoA(local_1ac,local_7c,0x6c);
    *(undefined4 *)((int)this + 0x134) = local_6c;
    AVIFileRelease(local_1ac);
    local_84 = AVIStreamOpenFromFileA((int)this + 0x270,param_1,0x73646976,0,0x10,0);
    if (local_84 == 0) {
      AVIStreamInfoA(*(undefined4 *)((int)this + 0x270),local_110,0x8c);
      local_1b0 = 0x28;
      local_84 = AVIStreamReadFormat(*(undefined4 *)((int)this + 0x270),0,local_138,&local_1b0);
      if (local_84 == 0) {
        uVar1 = AVIStreamLength(*(undefined4 *)((int)this + 0x270));
        *(undefined4 *)((int)this + 0x274) = uVar1;
        if (*(int *)((int)this + 0x274) < 0x4d) {
          *(undefined4 *)((int)this + 0x278) = local_134;
          *(undefined4 *)((int)this + 0x27c) = local_130;
          _memset(&local_1a8,0,0x6c);
          local_1a8 = 0x6c;
          local_1a4 = 7;
          local_140 = 0x840;
          local_19c = *(undefined4 *)((int)this + 0x278);
          local_1a0 = *(undefined4 *)((int)this + 0x27c);
          local_84 = (**(code **)(*DAT_00486a04 + 0x18))
                               (DAT_00486a04,&local_1a8,(int)this + 0x268,0);
          if (local_84 == 0) {
            puVar2 = (undefined4 *)FUN_0044ac60(0x1d8);
            local_8 = 0;
            if (puVar2 == (undefined4 *)0x0) {
              local_1b8 = (undefined4 *)0x0;
            }
            else {
              local_1b8 = FUN_0041a8e0(puVar2);
            }
            local_8 = 0xffffffff;
            *(undefined4 **)((int)this + 0x26c) = local_1b8;
            local_84 = FUN_0041a962(*(void **)((int)this + 0x26c),local_10c,local_138,0);
            if ((local_84 == 0) &&
               (local_84 = FUN_0041ae57(*(void **)((int)this + 0x26c),*(int **)((int)this + 0x268)),
               local_84 == 0)) {
              for (local_13c = 0; local_84 = 0, local_13c < *(int *)((int)this + 0x274);
                  local_13c = local_13c + 1) {
                local_84 = AVIStreamRead(*(undefined4 *)((int)this + 0x270),local_13c,1,0,0,
                                         (int)this + local_13c * 4 + 0x138,0);
                if (local_84 != 0) goto LAB_0041cb02;
                uVar1 = FUN_0044c2f0(*(SIZE_T *)((int)this + local_13c * 4 + 0x138));
                *(undefined4 *)((int)this + local_13c * 4 + 4) = uVar1;
                local_84 = AVIStreamRead(*(undefined4 *)((int)this + 0x270),local_13c,1,
                                         *(undefined4 *)((int)this + local_13c * 4 + 4),
                                         *(undefined4 *)((int)this + local_13c * 4 + 0x138),0,0);
                if (local_84 != 0) goto LAB_0041cb02;
              }
              local_84 = 0;
            }
          }
        }
        else {
          local_84 = 1;
        }
      }
    }
  }
LAB_0041cb02:
  *unaff_FS_OFFSET = local_10;
  return local_84;
}

