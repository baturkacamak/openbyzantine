// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43c466
// name: FUN_0043c466
// ranges: 0x43c466-0x43c565
// calls: 0x401184, 0x40df05, 0x41620b, 0x41624a, 0x438cd0, 0x43b216, 0x43c466, 0x43c566

undefined2 __cdecl FUN_0043c466(undefined4 *param_1,char param_2)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined2 local_10;
  undefined4 *local_c;
  
  iVar2 = FUN_0041620b();
  if (iVar2 == 0) {
    local_10 = 0xffff;
  }
  else {
    if (DAT_0045c060 == 0) {
      local_c = param_1;
      local_10 = FUN_0043b216(param_1,0,0);
      cVar1 = DAT_00482acd;
    }
    else {
      if (DAT_004859cd == '\0') {
        FUN_00401184(0);
      }
      local_10 = 0xffff;
      cVar1 = DAT_00482acd;
    }
    while ((DAT_00482acd != '\0' && (local_c == param_1))) {
      FUN_0041624a();
      if (param_2 == '\x01') {
        uVar3 = FUN_0043c566(param_1,'\0');
        local_10 = (undefined2)uVar3;
      }
      if (local_c == param_1) {
        local_10 = FUN_0043b216(param_1,0,0);
      }
      FUN_0040df05(0);
    }
    if (cVar1 != '\0') {
      FUN_00438cd0();
    }
  }
  return local_10;
}
