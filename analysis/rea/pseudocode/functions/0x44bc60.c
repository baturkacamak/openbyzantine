// rea decompile_all
// name: _floor
// address: 0x44bc60
// body: available
// range: 0x44bc60-0x44bd74 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    _floor
   
   Library: Visual Studio 1998 Release */

double __cdecl _floor(double _X)

{
  double dVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  float10 fVar5;
  undefined4 local_8;
  undefined4 uStack_4;
  
  uVar3 = FUN_004508f0();
  uVar2 = (uint)((ulonglong)_X >> 0x20);
  if ((_X._6_2_ & 0x7ff0) != 0x7ff0) {
    fVar5 = __frnd(_X);
    dVar1 = (double)fVar5;
    if (fVar5 == (float10)_X) {
      FUN_004508f0();
      return dVar1;
    }
    if ((uVar3 & 0x20) != 0) {
      FUN_004508f0();
      return dVar1;
    }
    uStack_4 = (undefined4)((ulonglong)dVar1 >> 0x20);
    local_8 = SUB84(dVar1,0);
    fVar5 = __except1(0x10,0xb,_X._0_4_,uVar2,local_8,uStack_4,uVar3);
    return (double)fVar5;
  }
  iVar4 = __sptype(_X._0_4_,uVar2);
  if (0 < iVar4) {
    if (iVar4 < 3) {
      FUN_004508f0();
      return _X;
    }
    if (iVar4 == 3) {
      fVar5 = __handle_qnan1(0xb,_X._0_4_,uVar2);
      return (double)fVar5;
    }
  }
  fVar5 = __except1(8,0xb,_X._0_4_,uVar2,SUB84(_X + 1.0,0),(int)((ulonglong)(_X + 1.0) >> 0x20),
                    uVar3);
  return (double)fVar5;
}

