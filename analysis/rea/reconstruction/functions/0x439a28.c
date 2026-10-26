// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x439a28
// name: FUN_00439a28
// ranges: 0x439a28-0x439ccd
// calls: 0x40ae93, 0x40b084, 0x439a28, 0x439d71, 0x439dbb, 0x439df6, 0x43a03b, 0x43a0a8, 0x43a20d, 0x43a248, 0x43afe0, 0x44a820, 0x44ac40

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
