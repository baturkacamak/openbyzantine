// rea decompile_all
// name: FUN_00439df6
// address: 0x439df6
// body: available
// range: 0x439df6-0x43a03a inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __fastcall FUN_00439df6(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int local_94;
  undefined1 local_90 [4];
  undefined4 local_8c;
  undefined4 local_68;
  
  iVar1 = AVIFileGetStream(*(undefined4 *)(param_1 + 0x14),param_1 + 0x28,0x73646976,0);
  if (iVar1 == 0) {
    uVar2 = AVIStreamLength(*(undefined4 *)(param_1 + 0x28));
    *(undefined4 *)(param_1 + 0x34) = uVar2;
    if (*(int *)(param_1 + 0x34) == -1) {
      FUN_0043a03b(param_1);
      uVar2 = 0xffffffff;
    }
    else {
      *(int *)(param_1 + 0x80) = *(int *)(param_1 + 0x34) + -1;
      iVar1 = AVIStreamInfoA(*(undefined4 *)(param_1 + 0x28),local_90,0x8c);
      if (iVar1 == 0) {
        *(undefined4 *)(param_1 + 0x38) = 0x42;
        *(undefined4 *)(param_1 + 0x3c) = local_68;
        *(undefined4 *)(param_1 + 0x40) = local_8c;
        if (*(int *)(param_1 + 0x40) != 0x31345649) {
          OutputDebugStringA(s_Init_Video_for_Windows_004617fc);
        }
        iVar1 = AVIStreamReadFormat(*(undefined4 *)(param_1 + 0x28),0,0,&local_94);
        if (iVar1 == 0) {
          if (local_94 == 0x28) {
            if (*(int *)(param_1 + 0x54) != 0) {
              FUN_0044a820(*(LPVOID *)(param_1 + 0x54));
            }
            uVar2 = FUN_0044ac60(0x28);
            *(undefined4 *)(param_1 + 0x54) = uVar2;
            if (*(int *)(param_1 + 0x54) == 0) {
              FUN_0043a03b(param_1);
              uVar2 = 0xffffffff;
            }
            else {
              AVIStreamReadFormat(*(undefined4 *)(param_1 + 0x28),0,0,&local_94);
              iVar1 = AVIStreamReadFormat(*(undefined4 *)(param_1 + 0x28),0,
                                          *(undefined4 *)(param_1 + 0x54),&local_94);
              if (iVar1 == 0) {
                uVar2 = 0;
              }
              else {
                FUN_0043a03b(param_1);
                uVar2 = 0xffffffff;
              }
            }
          }
          else {
            FUN_0043a03b(param_1);
            uVar2 = 0xffffffff;
          }
        }
        else {
          FUN_0043a03b(param_1);
          uVar2 = 0xffffffff;
        }
      }
      else {
        FUN_0043a03b(param_1);
        uVar2 = 0xffffffff;
      }
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}

