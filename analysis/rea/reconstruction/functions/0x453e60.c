// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x453e60
// name: getSystemCP
// ranges: 0x453e60-0x453eac
// calls: -

/* Library Function - Single Match
    _getSystemCP
   
   Library: Visual Studio 1998 Release */

int __cdecl getSystemCP(int param_1)

{
  int iVar1;
  bool bVar2;
  
  if (param_1 == -2) {
    DAT_00463a34 = 1;
                    /* WARNING: Could not recover jumptable at 0x00453e7d. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetOEMCP();
    return iVar1;
  }
  if (param_1 == -3) {
    DAT_00463a34 = 1;
                    /* WARNING: Could not recover jumptable at 0x00453e92. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetACP();
    return iVar1;
  }
  bVar2 = param_1 == -4;
  if (bVar2) {
    param_1 = DAT_00463558;
  }
  DAT_00463a34 = (uint)bVar2;
  return param_1;
}
