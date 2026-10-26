// rea decompile_all
// name: FUN_0043b216
// address: 0x43b216
// body: available
// range: 0x43b216-0x43bb76 inclusive
// non_contiguous: false
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 __cdecl FUN_0043b216(undefined4 *param_1,byte param_2,int param_3)

{
  DWORD DVar1;
  int iVar2;
  uint uVar3;
  int local_4c;
  uint local_48;
  undefined2 local_44;
  ushort local_3c;
  ushort local_38 [2];
  uint local_34;
  uint local_30;
  uint local_2c;
  uint local_28;
  int local_24;
  short *local_20;
  tagRECT local_1c;
  uint local_c;
  short *local_8;
  
  local_44 = 0xffff;
  local_38[0] = 0;
  local_4c = 0x15e;
  local_24 = 0;
  local_c = (uint)(param_1 != (undefined4 *)0x0);
  local_34 = local_34 & 0xffff0000;
  while ((short)local_34 < 0x15e) {
    if (*(int *)(&DAT_004649e8 + (short)local_34 * 4) != 0) {
      if ((short)local_34 < local_4c) {
        local_4c = (int)(short)local_34;
      }
      if (local_24 < (short)local_34) {
        local_24 = (int)(short)local_34;
      }
    }
    local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
  }
  if (local_24 < local_4c) {
    local_4c = 0;
    local_24 = 1;
  }
  if (DAT_0045f494 != 0) {
    local_24 = -1;
  }
  DVar1 = GetTickCount();
  uVar3 = DVar1 - _DAT_0046187c;
  DAT_0047ae58 = uVar3;
  if (((param_2 == 1) || (param_2 == 2)) || (0x10 < uVar3)) {
    if (param_2 == 0) {
      DAT_0047ae58 = 0;
      _DAT_0046187c = uVar3;
    }
    local_34 = local_34 & 0xffff0000;
    while ((short)local_34 < 0x15e) {
      (&DAT_0046caf8)[(short)local_34 * 4] = 0xffff;
      local_34 = CONCAT22(local_34._2_2_,(short)local_34 + 1);
    }
    if (DAT_0046edf4 == '\x02') {
      FUN_0042b679(DAT_0046e82c,DAT_0047a8a4,(uint)DAT_004835f8,(uint)DAT_004835fa,0x280,0x280,
                   (uint)DAT_004835f8,(uint)DAT_004835fa,(uint)DAT_0047ae5c,(uint)DAT_0047a828);
    }
    if ((DAT_004859cd == '\0') && (local_c != 0)) {
      if (DAT_0045f204 != 0) {
        iVar2 = (int)(short)local_38[0];
        local_38[0] = local_38[0] + 1;
        FUN_00417d62((LPRECT)(&DAT_0046caf8 + iVar2 * 4));
      }
      local_34 = CONCAT22(local_34._2_2_,(short)local_24);
      while (local_4c <= (short)local_34) {
        local_8 = *(short **)(&DAT_004649e8 + (short)local_34 * 4);
        if ((local_8 != (short *)0x0) &&
           ((((*(byte *)((int)local_8 + 9) & 0x20) != 0 ||
             (((*(char *)(*(int *)(&DAT_0047dd70 + (short)local_34 * 4) + 8) == '\x02' ||
               (*(char *)(*(int *)(&DAT_0047dd70 + (short)local_34 * 4) + 8) == '\x01')) &&
              ((param_2 == 2 || ((*(byte *)((int)local_8 + 9) & 4) == 0)))))) &&
            (*(int *)(local_8 + 0x10) != 0)))) {
          FUN_0043c114(local_34,(undefined2 *)&local_28,(undefined2 *)&local_30,
                       (undefined2 *)&local_48,(undefined2 *)&local_2c,'\x01');
          iVar2 = FUN_0040241c((short *)&local_28,(short *)&local_30,(short)local_48,(short)local_2c
                               ,&local_1c);
          if (iVar2 != 0) {
            local_28 = CONCAT22(local_28._2_2_,(short)local_1c.left);
            local_30 = CONCAT22(local_30._2_2_,(short)local_1c.top);
            local_48 = CONCAT22(local_48._2_2_,((short)local_1c.right - (short)local_1c.left) + 1);
            local_2c = CONCAT22(local_2c._2_2_,((short)local_1c.bottom - (short)local_1c.top) + 1);
            FUN_0042b679(DAT_0046e82c,DAT_0047a8a4,local_28,local_30,0x280,0x280,local_28,local_30,
                         local_48,local_2c);
            iVar2 = (int)(short)local_38[0];
            (&DAT_0046caf8)[iVar2 * 4] = local_1c.left;
            (&DAT_0046cafc)[iVar2 * 4] = local_1c.top;
            (&DAT_0046cb00)[iVar2 * 4] = local_1c.right;
            *(LONG *)(&DAT_0046cb04 + iVar2 * 0x10) = local_1c.bottom;
            local_38[0] = local_38[0] + 1;
          }
          if ((*(byte *)((int)local_8 + 9) & 0x20) != 0) {
            FUN_0043264d(local_8);
            local_8[0x10] = 0;
            local_8[0x11] = 0;
            if (*(char *)((int)local_8 + 0x29) != '\0') {
              *(undefined1 *)((int)local_8 + 0x29) = 0;
              DAT_00482acd = 0;
            }
          }
        }
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + -1);
      }
    }
    for (local_3c = 0; (short)local_3c < 5; local_3c = local_3c + 1) {
      local_34 = CONCAT22(local_34._2_2_,(short)local_24);
      while (local_4c <= (short)local_34) {
        local_8 = *(short **)(&DAT_004649e8 + (short)local_34 * 4);
        if ((local_8 != (short *)0x0) && (*(byte *)(local_8 + 4) == local_3c)) {
          if (param_3 != 0) {
            for (local_20 = *(short **)(local_8 + 0x1e); local_20 != (short *)0x0;
                local_20 = *(short **)(local_20 + 2)) {
              FUN_0043c114(local_34,(undefined2 *)&local_28,(undefined2 *)&local_30,
                           (undefined2 *)&local_48,(undefined2 *)&local_2c,'\0');
              if ((*local_8 != *local_20) || (local_8[1] != local_20[1])) {
                iVar2 = FUN_0040241c(local_20,local_20 + 1,(short)local_48,(short)local_2c,&local_1c
                                    );
                if (iVar2 != 0) {
                  FUN_0043b130((int)local_8,DAT_0046e82c,*local_20,local_20[1],&local_1c.left,0x280,
                               1);
                  iVar2 = (int)(short)local_38[0];
                  (&DAT_0046caf8)[iVar2 * 4] = local_1c.left;
                  (&DAT_0046cafc)[iVar2 * 4] = local_1c.top;
                  (&DAT_0046cb00)[iVar2 * 4] = local_1c.right;
                  *(LONG *)(&DAT_0046cb04 + iVar2 * 0x10) = local_1c.bottom;
                  local_38[0] = local_38[0] + 1;
                }
              }
            }
          }
          if ((*(byte *)((int)local_8 + 9) & 0x20) == 0) {
            if ((*(byte *)((int)local_8 + 9) & 0x40) == 0) {
              if (*(char *)((int)local_8 + 0x29) != '\0') {
                DAT_00482acd = 1;
              }
              FUN_0043c114(local_34,(undefined2 *)&local_28,(undefined2 *)&local_30,
                           (undefined2 *)&local_48,(undefined2 *)&local_2c,'\0');
              if (((*(byte *)((int)local_8 + 9) & 0x10) == 0) &&
                 ((local_8[5] != 0 || (local_8[6] != 0)))) {
                FUN_0042b679(DAT_0046e82c,DAT_0047a8a4,local_28,local_30,0x280,0x280,local_28,
                             local_30,local_48,local_2c);
                iVar2 = (int)(short)local_38[0];
                local_38[0] = local_38[0] + 1;
                SetRect((LPRECT)(&DAT_0046caf8 + iVar2 * 4),(int)(short)local_28,
                        (int)(short)local_30,(int)(short)local_48 + (int)(short)local_28 + -1,
                        (int)(short)local_2c + (int)(short)local_30 + -1);
                *local_8 = local_8[5] + *local_8;
                local_8[1] = local_8[1] + local_8[6];
                FUN_0043c114(local_34,(undefined2 *)&local_28,(undefined2 *)&local_30,
                             (undefined2 *)&local_48,(undefined2 *)&local_2c,'\0');
                iVar2 = FUN_0040241c((short *)&local_28,(short *)&local_30,(short)local_48,
                                     (short)local_2c,&local_1c);
                if (iVar2 == 0) {
                  *(byte *)((int)local_8 + 9) = *(byte *)((int)local_8 + 9) | 0x20;
                }
                else {
                  FUN_0043b130((int)local_8,DAT_0046e82c,(short)local_28,(short)local_30,
                               &local_1c.left,0x280,0);
                }
              }
              else {
                iVar2 = FUN_0040241c((short *)&local_28,(short *)&local_30,(short)local_48,
                                     (short)local_2c,&local_1c);
                if (iVar2 == 0) goto LAB_0043b5ed;
                FUN_0043b130((int)local_8,DAT_0046e82c,(short)local_28,(short)local_30,
                             &local_1c.left,0x280,(uint)param_2);
              }
              iVar2 = (int)(short)local_38[0];
              (&DAT_0046caf8)[iVar2 * 4] = local_1c.left;
              (&DAT_0046cafc)[iVar2 * 4] = local_1c.top;
              (&DAT_0046cb00)[iVar2 * 4] = local_1c.right;
              *(LONG *)(&DAT_0046cb04 + iVar2 * 0x10) = local_1c.bottom;
              local_38[0] = local_38[0] + 1;
              local_8[0x12] = *local_8;
              local_8[0x13] = local_8[1];
              if (param_1 != (undefined4 *)0x0) {
                FUN_0043cd2c((int)local_8,param_1);
              }
              if (param_2 != 1) {
                FUN_0043bb77((int)local_8,param_1);
              }
            }
            else {
              *(byte *)((int)local_8 + 9) = *(byte *)((int)local_8 + 9) | 0x20;
              *(byte *)((int)local_8 + 9) = *(byte *)((int)local_8 + 9) & 0xbf;
              if (*(char *)((int)local_8 + 0x29) != '\0') {
                *(undefined1 *)((int)local_8 + 0x29) = 0;
                DAT_00482acd = 0;
              }
              local_8[0x10] = 0;
              local_8[0x11] = 0;
            }
          }
        }
LAB_0043b5ed:
        local_34 = CONCAT22(local_34._2_2_,(short)local_34 + -1);
      }
    }
    if (DAT_00464878 == 0) {
      FUN_004176d8(0.0,0.0,DAT_00486888);
      iVar2 = (int)(short)local_38[0];
      local_38[0] = local_38[0] + 1;
      SetRect((LPRECT)(&DAT_0046caf8 + iVar2 * 4),0,0x14,0x280,0x1a4);
    }
    if (DAT_0046edf4 != '\0') {
      if (DAT_004859cd != '\0') {
        local_38[0] = 0;
      }
      FUN_0042ba62(DAT_0046e82c,DAT_00477344,(uint)DAT_00485c68,(uint)DAT_00485c6a,0x280,0x280,
                   (uint)DAT_00485c68,(uint)DAT_00485c6a,(uint)DAT_0048304c,(uint)DAT_0047a284,0x20)
      ;
      iVar2 = (int)(short)local_38[0];
      local_38[0] = local_38[0] + 1;
      SetRect((LPRECT)(&DAT_0046caf8 + iVar2 * 4),(uint)DAT_00485c68,(uint)DAT_00485c6a,
              (uint)DAT_0048304c + (uint)DAT_00485c68,(uint)DAT_0047a284 + (uint)DAT_00485c6a);
      if (DAT_0046edf4 == '\x02') {
        DAT_0046edf4 = '\0';
      }
      if (DAT_004859cd != '\0') {
        FUN_0043c3ac(local_38);
      }
    }
    if ((local_38[0] != 0) && (DAT_004859cd == '\0')) {
      FUN_0043c3ac(local_38);
    }
  }
  return local_44;
}

