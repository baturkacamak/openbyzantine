// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x40bbfd
// name: FUN_0040bbfd
// ranges: 0x40bbfd-0x40be2a; 0x40be30-0x40be43; 0x40be54-0x40be7f
// calls: 0x40bbfd, 0x42b5b2
// port: RLE run decoding [bounded]

void __cdecl
FUN_0040bbfd(int param_1,int param_2,short param_3,short param_4,int *param_5,int param_6)

{
  ushort uVar1;
  ushort *puVar2;
  uint uVar3;
  int iVar4;
  undefined2 local_18;
  int local_10;
  
  do {
    puVar2 = (ushort *)(DAT_0046c438 * 2 + param_1);
    DAT_0046c438 = DAT_0046c438 + 1;
    uVar1 = *puVar2 & 0x3fff;
    switch((byte)(*puVar2 >> 0xe)) {
    case 0:
      if (param_4 + DAT_0046c428 < param_5[1]) {
        DAT_0046c430 = DAT_0046c430 + (short)uVar1;
        DAT_0046c438 = DAT_0046c438 + (short)uVar1;
      }
      else {
        for (local_10 = 0; local_10 < (short)uVar1; local_10 = local_10 + 1) {
          iVar4 = (param_3 + DAT_0046c430) - DAT_0046c43c;
          if (param_6 == 0) {
            local_18 = *(undefined2 *)(param_1 + DAT_0046c438 * 2);
          }
          else {
            uVar3 = FUN_0042b5b2((uint)*(ushort *)(DAT_0047a8a4 + DAT_0046c430 * 2));
            local_18 = (undefined2)uVar3;
          }
          if ((*param_5 <= iVar4) && (iVar4 <= param_5[2])) {
            *(undefined2 *)(param_2 + DAT_0046c430 * 2) = local_18;
          }
          DAT_0046c430 = DAT_0046c430 + 1;
          DAT_0046c438 = DAT_0046c438 + 1;
        }
      }
      break;
    case 1:
      DAT_0046c43c = DAT_0046c43c + DAT_0046c42c;
      DAT_0046c430 = (short)uVar1 + DAT_0046c43c;
      DAT_0046c428 = DAT_0046c428 + 1;
      break;
    case 2:
      if (param_6 == 0) {
        local_18 = *(undefined2 *)(param_1 + DAT_0046c438 * 2);
      }
      else {
        uVar3 = FUN_0042b5b2((uint)*(ushort *)(DAT_0047a8a4 + DAT_0046c430 * 2));
        local_18 = (undefined2)uVar3;
      }
      DAT_0046c438 = DAT_0046c438 + 1;
      if (param_4 + DAT_0046c428 < param_5[1]) {
        DAT_0046c430 = DAT_0046c430 + (short)uVar1;
      }
      else {
        for (local_10 = 0; local_10 < (short)uVar1; local_10 = local_10 + 1) {
          iVar4 = (param_3 + DAT_0046c430) - DAT_0046c43c;
          if ((*param_5 <= iVar4) && (iVar4 <= param_5[2])) {
            *(undefined2 *)(param_2 + DAT_0046c430 * 2) = local_18;
          }
          DAT_0046c430 = DAT_0046c430 + 1;
        }
      }
      break;
    case 3:
      DAT_0046c430 = DAT_0046c430 + (short)uVar1;
    }
  } while ((DAT_0046c428 < DAT_0046c434) && (param_4 + DAT_0046c428 <= param_5[3]));
  return;
}
