// rea decompile_all
// name: FUN_00439a28
// address: 0x439a28
// body: available
// range: 0x439a28-0x439ccd inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


undefined4 __thiscall
FUN_00439a28(void *this,undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,
            int param_5)

{
  undefined4 uVar1;
  int iVar2;
  LONG LVar3;
  HANDLE pvVar4;
  
  if (*(int *)((int)this + 0x10) == 0) {
    *(undefined4 *)((int)this + 0x10) = 1;
    iVar2 = FUN_00439d71(this,param_1);
    if (iVar2 == 0) {
      iVar2 = FUN_00439df6((int)this);
      if (iVar2 == 0) {
        iVar2 = FUN_0043a0a8(this);
        if (iVar2 == 0) {
          if (*(int *)((int)this + 0x24) == 0) {
            iVar2 = FUN_0040b084(*(void **)((int)this + 4),*(undefined4 *)((int)this + 0x60),
                                 *(undefined4 *)((int)this + 0x68),*(int *)((int)this + 0x6c),
                                 *(int *)((int)this + 0x70));
            if (iVar2 != 0) {
              *(undefined4 *)((int)this + 0x10) = 0;
              return 0xffffffff;
            }
          }
          else {
            iVar2 = FUN_0040ae93(*(void **)((int)this + 4),*(undefined4 *)((int)this + 0x60),
                                 *(undefined4 *)((int)this + 0x68),*(int *)((int)this + 0x6c));
            if (iVar2 != 0) {
              *(undefined4 *)((int)this + 0x10) = 0;
              return 0xffffffff;
            }
          }
          *(undefined4 *)((int)this + 0x2c) = param_3;
          *(undefined4 *)((int)this + 0x30) = param_4;
          iVar2 = FUN_0043a248(this,param_2,param_5);
          if (iVar2 == 0) {
            *(undefined4 *)((int)this + 0x74) = 0;
            *(undefined4 *)((int)this + 0x78) = 0xffffffff;
            *(undefined4 *)((int)this + 0x7c) = *(undefined4 *)((int)this + 0x78);
            InterlockedExchange((LONG *)((int)this + 0x20),0);
            while (LVar3 = InterlockedExchange((LONG *)((int)this + 0x94),1), LVar3 != 0) {
              Sleep(2);
            }
            pvVar4 = CreateThread((LPSECURITY_ATTRIBUTES)0x0,0,FUN_0043afe0,this,0,
                                  (LPDWORD)((int)this + 0x18));
            *(HANDLE *)((int)this + 0x90) = pvVar4;
            InterlockedExchange((LONG *)((int)this + 0x94),0);
            if (*(int *)((int)this + 0x90) == 0) {
              *(undefined4 *)((int)this + 0x10) = 0;
              uVar1 = 0xffffffff;
            }
            else {
              uVar1 = 0;
            }
          }
          else {
            FUN_0043a20d((int)this);
            FUN_0043a03b((int)this);
            if (*(int *)((int)this + 0x5c) != 0) {
              FUN_0044a820(*(LPVOID *)((int)this + 0x5c));
              *(undefined4 *)((int)this + 0x5c) = 0;
            }
            if (*(int *)((int)this + 0x58) != 0) {
              FUN_0044ac40(*(LPVOID *)((int)this + 0x58));
              *(undefined4 *)((int)this + 0x58) = 0;
            }
            FUN_00439dbb((int)this);
            *(undefined4 *)((int)this + 0x10) = 0;
            uVar1 = 0xffffffff;
          }
        }
        else {
          FUN_0043a03b((int)this);
          *(undefined4 *)((int)this + 0x10) = 0;
          uVar1 = 0xffffffff;
        }
      }
      else {
        *(undefined4 *)((int)this + 0x10) = 0;
        uVar1 = 0xffffffff;
      }
    }
    else {
      *(undefined4 *)((int)this + 0x10) = 0;
      uVar1 = 0xffffffff;
    }
  }
  else {
    uVar1 = 0xffffffff;
  }
  return uVar1;
}

