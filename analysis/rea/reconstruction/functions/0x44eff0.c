// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x44eff0
// name: FUN_0044eff0
// ranges: 0x44eff0-0x44f282
// calls: 0x44eff0, 0x44f290, 0x44f9e0, 0x44fa70

void __cdecl
FUN_0044eff0(EHExceptionRecord *param_1,EHRegistrationNode *param_2,_CONTEXT *param_3,void *param_4,
            _s_FuncInfo *param_5,char param_6,int param_7,EHRegistrationNode *param_8)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  EHExceptionRecord *pEVar5;
  bool bVar6;
  undefined3 extraout_var;
  _s_HandlerType *p_Var7;
  int iVar8;
  int *piVar9;
  byte *pbVar10;
  int iVar11;
  byte *pbVar12;
  _s_TryBlockMapEntry *p_Stack_18;
  uint uStack_c;
  uint uStack_8;
  uint *puStack_4;
  
  iVar2 = *(int *)(param_2 + 8);
  if ((iVar2 < -1) || (*(int *)(param_5 + 4) <= iVar2)) {
    FUN_0044fa70();
  }
  pEVar5 = DAT_00463250;
  if ((((*(int *)param_1 == -0x1f928c9d) && (*(int *)(param_1 + 0x10) == 3)) &&
      (*(int *)(param_1 + 0x14) == 0x19930520)) && (*(int *)(param_1 + 0x1c) == 0)) {
    if (DAT_00463250 == (EHExceptionRecord *)0x0) {
      return;
    }
    param_1 = DAT_00463250;
    param_3 = DAT_00463254;
    bVar6 = FID_conflict__ValidateRead(DAT_00463250,1);
    if (CONCAT31(extraout_var,bVar6) == 0) {
      FUN_0044fa70();
    }
    if (((*(int *)pEVar5 == -0x1f928c9d) && (*(int *)(pEVar5 + 0x10) == 3)) &&
       ((*(int *)(pEVar5 + 0x14) == 0x19930520 && (*(int *)(pEVar5 + 0x1c) == 0)))) {
      FUN_0044fa70();
    }
  }
  if (((*(int *)param_1 == -0x1f928c9d) && (*(int *)(param_1 + 0x10) == 3)) &&
     (*(int *)(param_1 + 0x14) == 0x19930520)) {
    p_Stack_18 = GetRangeOfTrysToCheck(param_5,param_7,iVar2,&uStack_c,&uStack_8);
    if (uStack_c < uStack_8) {
      do {
        if ((*(int *)p_Stack_18 <= iVar2) && (iVar2 <= *(int *)(p_Stack_18 + 4))) {
          p_Var7 = *(_s_HandlerType **)(p_Stack_18 + 0x10);
          for (iVar3 = *(int *)(p_Stack_18 + 0xc); iVar3 != 0; iVar3 = iVar3 + -1) {
            piVar9 = *(int **)(*(int *)(param_1 + 0x1c) + 0xc);
            for (iVar8 = *piVar9; iVar8 != 0; iVar8 = iVar8 + -1) {
              piVar9 = piVar9 + 1;
              iVar11 = *(int *)(p_Var7 + 4);
              puStack_4 = *(uint **)(param_1 + 0x1c);
              pbVar4 = (byte *)*piVar9;
              if ((iVar11 == 0) || (*(char *)(iVar11 + 8) == '\0')) {
LAB_0044f1a4:
                bVar6 = true;
              }
              else {
                if (iVar11 == *(int *)(pbVar4 + 4)) {
LAB_0044f17e:
                  if (((((*pbVar4 & 2) == 0) || (((byte)*p_Var7 & 8) != 0)) &&
                      (((*puStack_4 & 1) == 0 || (((byte)*p_Var7 & 1) != 0)))) &&
                     (((*puStack_4 & 2) == 0 || (((byte)*p_Var7 & 2) != 0)))) goto LAB_0044f1a4;
                }
                else {
                  pbVar10 = (byte *)(iVar11 + 8);
                  pbVar12 = (byte *)(*(int *)(pbVar4 + 4) + 8);
                  do {
                    bVar1 = *pbVar10;
                    bVar6 = bVar1 < *pbVar12;
                    if (bVar1 != *pbVar12) {
LAB_0044f175:
                      iVar11 = (1 - (uint)bVar6) - (uint)(bVar6 != 0);
                      goto LAB_0044f17a;
                    }
                    if (bVar1 == 0) break;
                    bVar1 = pbVar10[1];
                    bVar6 = bVar1 < pbVar12[1];
                    if (bVar1 != pbVar12[1]) goto LAB_0044f175;
                    pbVar10 = pbVar10 + 2;
                    pbVar12 = pbVar12 + 2;
                  } while (bVar1 != 0);
                  iVar11 = 0;
LAB_0044f17a:
                  if (iVar11 == 0) goto LAB_0044f17e;
                }
                bVar6 = false;
              }
              if (bVar6) {
                CatchIt(param_1,param_2,param_3,param_4,param_5,p_Var7,(_s_CatchableType *)*piVar9,
                        p_Stack_18,param_7,param_8);
                goto LAB_0044f202;
              }
            }
            p_Var7 = p_Var7 + 0x10;
          }
        }
LAB_0044f202:
        p_Stack_18 = p_Stack_18 + 0x14;
        uStack_c = uStack_c + 1;
      } while (uStack_c < uStack_8);
    }
    if (param_6 != '\0') {
      DestructExceptionObject(param_1,'\x01');
      return;
    }
  }
  else {
    if (param_6 == '\0') {
      FUN_0044f290(param_1,param_2,param_3,param_4,param_5,iVar2,param_7,param_8);
      return;
    }
    FUN_0044f9e0();
  }
  return;
}
