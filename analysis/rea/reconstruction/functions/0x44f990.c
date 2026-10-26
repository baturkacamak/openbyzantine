// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44f990
// name: __CallSettingFrame@12
// ranges: 0x44f990-0x44f9d7
// calls: 0x44a8fd

/* WARNING: Restarted to delay deadcode elimination for space: stack */
/* Library Function - Single Match
    __CallSettingFrame@12
   
   Library: Visual Studio 1998 Release */

void __CallSettingFrame_12(undefined4 param_1,undefined4 param_2,int param_3)

{
  code *pcVar1;
  
  pcVar1 = (code *)FUN_0044a8fd(param_3);
  (*pcVar1)();
  if (param_3 == 0x100) {
    param_3 = 2;
  }
  FUN_0044a8fd(param_3);
  return;
}
