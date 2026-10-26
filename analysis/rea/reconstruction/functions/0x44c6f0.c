// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44c6f0
// name: _system
// ranges: 0x44c6f0-0x44c7ad
// calls: -

/* Library Function - Single Match
    _system
   
   Library: Visual Studio 1998 Release */

int __cdecl _system(char *_Command)

{
  int iVar1;
  intptr_t iVar2;
  char *local_10;
  undefined *local_c;
  char *local_8;
  undefined4 local_4;
  
  local_10 = _getenv(s_COMSPEC_00462cd4);
  if (_Command == (char *)0x0) {
    if (local_10 != (char *)0x0) {
      iVar1 = __access(local_10,0);
      return (uint)(iVar1 == 0);
    }
    return 0;
  }
  local_c = &DAT_00462cd0;
  local_4 = 0;
  local_8 = _Command;
  if (local_10 != (char *)0x0) {
    iVar2 = __spawnve(0,local_10,&local_10,(char **)0x0);
    if (iVar2 != -1) {
      return iVar2;
    }
    if ((DAT_00462c74 != 2) && (DAT_00462c74 != 0xd)) {
      return -1;
    }
  }
  local_10 = s_command_com_00462cc4;
  if ((DAT_00462c80._1_1_ & 0x80) == 0) {
    local_10 = s_cmd_exe_00462cbc;
  }
  iVar2 = __spawnvpe(0,local_10,&local_10,(char **)0x0);
  return iVar2;
}
