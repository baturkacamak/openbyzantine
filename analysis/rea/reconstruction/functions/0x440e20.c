// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x440e20
// name: FUN_00440e20
// ranges: 0x440e20-0x440f5d
// calls: 0x440e20, 0x440f60, 0x4410a0, 0x441140, 0x4411f0, 0x441390, 0x4418d0, 0x4422e0, 0x442350, 0x442360

void __cdecl FUN_00440e20(int *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  char *_Src;
  int iVar3;
  char cStack_5;
  int local_4;
  
  param_1[1] = 0;
  local_4 = FUN_00442350();
  puVar1 = (undefined4 *)FUN_004422e0(param_1,0x50);
  if (puVar1 == (undefined4 *)0x0) {
    FUN_00442360();
    *(undefined4 *)(*param_1 + 0x14) = 0x35;
    *(undefined4 *)(*param_1 + 0x18) = 0;
    (**(code **)*param_1)(param_1);
  }
  *puVar1 = FUN_00440f60;
  puVar1[1] = FUN_004410a0;
  puVar1[2] = FUN_00441140;
  puVar1[3] = FUN_004411f0;
  puVar1[4] = &LAB_004412b0;
  puVar1[5] = &LAB_00441320;
  puVar1[6] = FUN_00441390;
  puVar1[7] = &LAB_00441530;
  puVar1[8] = &LAB_00441700;
  iVar3 = 1;
  puVar1[9] = FUN_004418d0;
  puVar1[10] = &LAB_004419d0;
  puVar1[0xb] = local_4;
  puVar2 = puVar1 + 0xd;
  do {
    *puVar2 = 0;
    iVar3 = iVar3 + -1;
    puVar2[2] = 0;
    puVar2 = puVar2 + -1;
  } while (-1 < iVar3);
  puVar1[0x10] = 0;
  puVar1[0x11] = 0;
  puVar1[0x12] = 0x50;
  param_1[1] = (int)puVar1;
  _Src = _getenv(s_JPEGMEM_00462ad4);
  if (_Src != (char *)0x0) {
    cStack_5 = 'x';
    iVar3 = _sscanf(_Src,s__ld_c_00462acc,&local_4,&cStack_5);
    if (0 < iVar3) {
      if ((cStack_5 == 'm') || (cStack_5 == 'M')) {
        local_4 = local_4 * 1000;
      }
      puVar1[0xb] = local_4 * 1000;
    }
  }
  return;
}
