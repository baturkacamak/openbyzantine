// rea decompile_all
// name: FUN_00415c42
// address: 0x415c42
// body: available
// range: 0x415c42-0x4161f5 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00415c42(void)

{
  int iVar1;
  uint uVar2;
  DWORD dwExStyle;
  HMENU pHVar3;
  DWORD dwStyle;
  HDC hdc;
  HWND hWnd;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  char local_189c [2048];
  char local_109c [2048];
  char local_89c [2048];
  tagRECT local_9c;
  char *local_8c;
  uint local_88;
  int local_84;
  undefined4 local_80;
  undefined4 local_7c;
  int local_78;
  int local_74;
  undefined4 local_6c;
  undefined4 uStackY_68;
  undefined4 uStackY_64;
  undefined4 uStackY_60;
  char *lpText;
  char *lpCaption;
  UINT uType;
  int local_14;
  
  FUN_0044be20();
  if ((DAT_00486a04 == (int *)0x0) && (iVar1 = DirectDrawCreate(), iVar1 != 0)) {
    return 0x40;
  }
  if (DAT_0045f080 == 0) {
    (**(code **)(*DAT_00486a04 + 0x50))();
  }
  else {
    iVar1 = (**(code **)(*DAT_00486a04 + 0x50))();
    if (iVar1 != 0) {
      return 0x41;
    }
  }
  if (DAT_0045f080 == 0) {
    local_8c = s_You_must_set_your_display_to_16_b_0045f114;
    uVar2 = GetWindowLongA(DAT_0046c4e8,-0x10);
    local_88 = uVar2 & 0x7fffffff | 0xc40000;
    SetWindowLongA(DAT_0046c4e8,-0x10,local_88);
    SetRect(&local_9c,0,0,DAT_004869f0 + -1,DAT_004869f4 + -1);
    dwExStyle = GetWindowLongA(DAT_0046c4e8,-0x14);
    pHVar3 = GetMenu(DAT_0046c4e8);
    uVar2 = (uint)(pHVar3 != (HMENU)0x0);
    dwStyle = GetWindowLongA(DAT_0046c4e8,-0x10);
    AdjustWindowRectEx(&local_9c,dwStyle,uVar2,dwExStyle);
    SetWindowPos(DAT_0046c4e8,(HWND)0x0,0,0,local_9c.right - local_9c.left,
                 local_9c.bottom - local_9c.top,0x16);
    SetWindowPos(DAT_0046c4e8,(HWND)0xfffffffe,0,0,0,0,0x13);
    hdc = GetDC((HWND)0x0);
    local_84 = GetDeviceCaps(hdc,0xc);
    ReleaseDC((HWND)0x0,hdc);
    if (local_84 != 0x10) {
      uType = 0;
      lpCaption = s_Byzantine_0045f190;
      lpText = local_8c;
      hWnd = (HWND)FUN_004161f6();
      MessageBoxA(hWnd,lpText,lpCaption,uType);
      return 0x42;
    }
  }
  else {
    iVar1 = (**(code **)(*DAT_00486a04 + 0x54))();
    if (iVar1 != 0) {
      return 0x42;
    }
  }
  if ((DAT_0045f080 == 0) || (DAT_0045f074 == '\0')) {
    _memset(&local_80,0,0x6c);
    local_80 = 0x6c;
    local_7c = 1;
    iVar1 = (**(code **)(*DAT_00486a04 + 0x18))();
    if (iVar1 != 0) {
      return 0x43;
    }
  }
  else {
    _memset(&local_80,0,0x6c);
    local_80 = 0x6c;
    local_7c = 0x21;
    local_6c = 1;
    local_14 = (**(code **)(*DAT_00486a04 + 0x18))();
    if (local_14 == -0x7789fe84) {
      local_14 = (**(code **)(*DAT_00486a04 + 0x18))();
    }
    if (local_14 != 0) {
      return 0x43;
    }
    _memset(&stack0xfffffff8,0,4);
    iVar1 = (**(code **)(*DAT_004869d4 + 0x30))();
    if (iVar1 != 0) {
      return 0x46;
    }
  }
  _memset(&local_80,0,0x6c);
  local_80 = 0x6c;
  local_7c = 7;
  local_74 = DAT_004869f0;
  local_78 = DAT_004869f4;
  iVar1 = 0;
  while( true ) {
    if (2 < iVar1) {
      DAT_00486888 = DAT_004869e0;
      if (DAT_0045f080 == 0) {
        iVar1 = (**(code **)(*DAT_00486a04 + 0x10))();
        if (iVar1 != 0) {
          FUN_0040305a(iVar1,local_89c);
        }
        iVar1 = (**(code **)(*DAT_00486884 + 0x20))();
        if (iVar1 != 0) {
          uStackY_60 = 0x416186;
          FUN_0040305a(iVar1,local_109c);
        }
        uStackY_60 = 0x41619f;
        iVar1 = (**(code **)(*DAT_004869d4 + 0x70))();
        if (iVar1 != 0) {
          FUN_0040305a(iVar1,local_189c);
        }
      }
      iVar1 = FUN_004162a0();
      if (iVar1 == 0) {
        DAT_0046c4ec = 1;
        _DAT_0046c4f0 = 1;
        uVar6 = 0;
      }
      else {
        uVar6 = 0x45;
      }
      return uVar6;
    }
    iVar4 = (**(code **)(*DAT_00486a04 + 0x18))();
    if (iVar4 != 0) break;
    _memset(&DAT_00486890 + iVar1 * 0x6c,0,0x6c);
    *(undefined4 *)(&DAT_00486890 + iVar1 * 0x6c) = 0x6c;
    *(undefined4 *)(&DAT_00486894 + iVar1 * 0x6c) = 0xff9ee;
    uStackY_60 = 0x4160b8;
    iVar4 = (**(code **)(*(int *)(&DAT_004869e0)[iVar1] + 100))();
    uStackY_60 = (&DAT_004868b4)[iVar1 * 0x1b];
    uStackY_64 = (&DAT_004869e0)[iVar1];
    uStackY_68 = 0x4160e9;
    iVar5 = (**(code **)(*(int *)(&DAT_004869e0)[iVar1] + 0x80))();
    if (iVar4 != 0 || iVar5 != 0) {
      return 0x44;
    }
    iVar1 = iVar1 + 1;
  }
  return 0x44;
}

