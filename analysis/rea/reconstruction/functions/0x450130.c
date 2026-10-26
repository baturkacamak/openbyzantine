// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x450130
// name: FUN_00450130
// ranges: 0x450130-0x450489
// calls: 0x450130, 0x4508b0, 0x4508d0

void __cdecl
FUN_00450130(uint *param_1,uint *param_2,uint param_3,int param_4,uint *param_5,uint *param_6)

{
  uint *puVar1;
  uint *puVar2;
  uint uVar3;
  uint *puVar4;
  DWORD local_4;
  
  puVar1 = param_2;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  if ((param_3 & 0x10) != 0) {
    local_4 = 0xc000008f;
    param_1[1] = param_1[1] | 1;
  }
  if ((param_3 & 2) != 0) {
    local_4 = 0xc0000093;
    param_1[1] = param_1[1] | 2;
  }
  if ((param_3 & 1) != 0) {
    local_4 = 0xc0000091;
    param_1[1] = param_1[1] | 4;
  }
  if ((param_3 & 4) != 0) {
    local_4 = 0xc000008e;
    param_1[1] = param_1[1] | 8;
  }
  if ((param_3 & 8) != 0) {
    local_4 = 0xc0000090;
    param_1[1] = param_1[1] | 0x10;
  }
  uVar3 = param_1[2];
  param_1[2] = ((~*param_2 & 1) << 4 ^ uVar3) & 0x10 ^ uVar3;
  uVar3 = param_1[2];
  param_1[2] = ((uint)((*param_2 & 4) == 0) << 3 ^ uVar3) & 8 ^ uVar3;
  uVar3 = param_1[2];
  param_1[2] = ((uint)((*param_2 & 8) == 0) << 2 ^ uVar3) & 4 ^ uVar3;
  uVar3 = param_1[2];
  param_1[2] = ((uint)((*param_2 & 0x10) == 0) * 2 ^ uVar3) & 2 ^ uVar3;
  uVar3 = param_1[2];
  param_1[2] = ((*param_2 & 0x20) == 0 ^ uVar3) & 1 ^ uVar3;
  uVar3 = FUN_004508b0();
  puVar2 = param_6;
  if ((uVar3 & 1) != 0) {
    param_1[3] = param_1[3] | 0x10;
  }
  if ((uVar3 & 4) != 0) {
    param_1[3] = param_1[3] | 8;
  }
  if ((uVar3 & 8) != 0) {
    param_1[3] = param_1[3] | 4;
  }
  if ((uVar3 & 0x10) != 0) {
    param_1[3] = param_1[3] | 2;
  }
  if ((uVar3 & 0x20) != 0) {
    param_1[3] = param_1[3] | 1;
  }
  uVar3 = *puVar1 & 0xc00;
  if (uVar3 < 0x401) {
    if (uVar3 == 0x400) {
      *param_1 = *param_1 & 0xfffffffd | 1;
    }
    else if (uVar3 == 0) {
      *param_1 = *param_1 & 0xfffffffc;
    }
  }
  else if (uVar3 == 0x800) {
    *param_1 = *param_1 & 0xfffffffe | 2;
  }
  else if (uVar3 == 0xc00) {
    *param_1 = *param_1 | 3;
  }
  uVar3 = *puVar1 & 0x300;
  if (uVar3 == 0) {
    *param_1 = *param_1 & 0xffffffeb | 8;
  }
  else if (uVar3 == 0x200) {
    *param_1 = *param_1 & 0xffffffe7 | 4;
  }
  else if (uVar3 == 0x300) {
    *param_1 = *param_1 & 0xffffffe3;
  }
  *param_1 = (param_4 << 5 ^ *param_1) & 0x1ffe0 ^ *param_1;
  param_1[8] = param_1[8] | 1;
  param_1[8] = param_1[8] & 0xffffffe3 | 2;
  param_1[5] = param_5[1];
  param_1[4] = *param_5;
  param_1[0x14] = param_1[0x14] | 1;
  param_1[0x14] = param_1[0x14] & 0xffffffe3 | 2;
  param_1[0x11] = param_6[1];
  param_1[0x10] = *param_6;
  FUN_004508d0();
  RaiseException(local_4,0,1,(ULONG_PTR *)&param_1);
  puVar4 = param_1 + 2;
  if ((*puVar4 & 0x10) != 0) {
    *puVar1 = *puVar1 & 0xfffffffe;
  }
  if ((*puVar4 & 8) != 0) {
    *puVar1 = *puVar1 & 0xfffffffb;
  }
  if ((*puVar4 & 4) != 0) {
    *puVar1 = *puVar1 & 0xfffffff7;
  }
  if ((*puVar4 & 2) != 0) {
    *puVar1 = *puVar1 & 0xffffffef;
  }
  if ((*puVar4 & 1) != 0) {
    *puVar1 = *puVar1 & 0xffffffdf;
  }
  switch(*param_1 & 3) {
  case 0:
    *puVar1 = *puVar1 & 0xfffff3ff;
    break;
  case 1:
    *puVar1 = *puVar1 & 0xfffff7ff | 0x400;
    break;
  case 2:
    *puVar1 = *puVar1 & 0xfffffbff | 0x800;
    break;
  case 3:
    *puVar1 = *puVar1 | 0xc00;
  }
  uVar3 = (*param_1 & 0x1c) >> 2;
  if (uVar3 == 0) {
    *puVar1 = *puVar1 & 0xfffff3ff | 0x300;
  }
  else if (uVar3 == 1) {
    *puVar1 = *puVar1 & 0xfffff3ff | 0x200;
  }
  else if (uVar3 == 2) {
    *puVar1 = *puVar1 & 0xfffff3ff;
  }
  puVar2[1] = param_1[0x11];
  *puVar2 = param_1[0x10];
  return;
}
