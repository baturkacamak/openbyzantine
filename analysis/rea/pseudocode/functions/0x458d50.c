// rea decompile_all
// name: __nextafter
// address: 0x458d50
// body: available
// range: 0x458d50-0x459054 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    __nextafter
   
   Library: Visual Studio 1998 Release */

double __cdecl __nextafter(double _X,double _Y)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  float10 fVar5;
  undefined8 local_c;
  short local_4 [2];
  
  uVar3 = FUN_004508f0();
  iVar1 = (int)((ulonglong)_X >> 0x20);
  uVar2 = (undefined4)((ulonglong)_Y >> 0x20);
  if (((_X._6_2_ & 0x7ff0) == 0x7ff0) || ((_Y._6_2_ & 0x7ff0) == 0x7ff0)) {
    if ((((_X._6_2_ & 0x7ff8) == 0x7ff0) &&
        ((((ulonglong)_X & 0x7ffff00000000) != 0 || (_X._0_4_ != 0)))) ||
       (((_Y._6_2_ & 0x7ff8) == 0x7ff0 &&
        ((((ulonglong)_Y & 0x7ffff00000000) != 0 || (_Y._0_4_ != 0)))))) {
      fVar5 = __except2(8,0x26,_X._0_4_,iVar1,_Y._0_4_,uVar2,SUB84(_Y + _X,0),
                        (int)((ulonglong)(_Y + _X) >> 0x20),uVar3);
      return (double)fVar5;
    }
    if (((_X._6_2_ & 0x7ff8) == 0x7ff8) || ((_Y._6_2_ & 0x7ff8) == 0x7ff8)) {
      fVar5 = __handle_qnan2(0x26,_X._0_4_,iVar1,_Y._0_4_,uVar2);
      return (double)fVar5;
    }
  }
  if (_Y == _X) {
    FUN_004508f0();
    return _X;
  }
  if (((ulonglong)_X & 0x7fffffff00000000) == 0 && _X._0_4_ == 0) {
    local_c = 4.94065645841247e-324;
    if (_Y <= _X) {
      local_c = -4.94065645841247e-324;
    }
  }
  if (((0.0 < _X) && (_Y < _X)) || ((_X < 0.0 && (_X < _Y)))) {
    local_c = (double)CONCAT44(iVar1,_X._0_4_ - 1);
    if (_X._0_4_ != 0) goto LAB_00458f18;
    iVar4 = iVar1 + -1;
  }
  else {
    if (((_X <= 0.0) || (_Y <= _X)) && ((0.0 <= _X || (_X <= _Y)))) goto LAB_00458f18;
    local_c = (double)CONCAT44(iVar1,_X._0_4_ + 1);
    if (_X._0_4_ + 1 != 0) goto LAB_00458f18;
    iVar4 = iVar1 + 1;
  }
  local_c = (double)CONCAT44(iVar4,(uint)local_c);
LAB_00458f18:
  if ((((ulonglong)local_c & 0x7ff0000000000000) == 0) &&
     ((((ulonglong)local_c & 0xfffff00000000) != 0 || ((uint)local_c != 0)))) {
    fVar5 = __decomp((uint)local_c,local_c._4_4_,(int *)local_4);
    fVar5 = __set_exp(SUB84((double)fVar5,0),(int)((ulonglong)(double)fVar5 >> 0x20),
                      local_4[0] + 0x600);
    local_c._0_4_ = SUB84((double)fVar5,0);
    fVar5 = __except2(0x12,0x26,_X._0_4_,iVar1,_Y._0_4_,uVar2,(uint)local_c,
                      (int)((ulonglong)(double)fVar5 >> 0x20),uVar3);
    return (double)fVar5;
  }
  if (((local_c._4_4_ != 0x7ff00000) || ((uint)local_c != 0)) &&
     ((local_c._4_4_ != 0xfff00000 || ((uint)local_c != 0)))) {
    FUN_004508f0();
    return local_c;
  }
  fVar5 = __decomp((uint)local_c,local_c._4_4_,(int *)local_4);
  fVar5 = __set_exp(SUB84((double)fVar5,0),(int)((ulonglong)(double)fVar5 >> 0x20),
                    local_4[0] + -0x600);
  local_c._0_4_ = SUB84((double)fVar5,0);
  fVar5 = __except2(0x11,0x26,_X._0_4_,iVar1,_Y._0_4_,uVar2,(uint)local_c,
                    (int)((ulonglong)(double)fVar5 >> 0x20),uVar3);
  return (double)fVar5;
}

