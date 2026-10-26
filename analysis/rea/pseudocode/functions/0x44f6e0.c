// rea decompile_all
// name: BuildCatchObject
// address: 0x44f6e0
// body: available
// range: 0x44f6e0-0x44f8b8 inclusive
// range: 0x44f8c7-0x44f8dd inclusive
// non_contiguous: true
// provider: Ghidra 12.1.4 (ghidra)
// analysis_profile: e1ad533d4cc0333c2136fe0894217de2d61feea098240326e78633f9e4ddcbf9
// limitations: The session serves operations only after Ghidra reports default auto-analysis complete; incomplete analysis does not expose partial results.; The imported Program and temporary project are ephemeral and deleted on close. Annotation operations edit session database metadata; original executable bytes are never written.; Cancelling an active Ghidra request terminates its ephemeral database and discards session annotations. The selected target remains open; the next provider query imports the original artifact again.; Default-space addresses use lowercase 0x-prefixed hexadecimal; other address spaces use <percent-encoded-space>:0x<hex>.; The Ghidra Program references PDB betrayal.pdb (GUID unknown, age (hex) 1, signature 34b6c197, version NB10); Ghidra reports load state unknown. Original names and types may be missing; an empty name search does not prove a procedure is absent. Use list_procedures and address-based analysis to inspect recovered functions. REA does not admit a caller-selected PDB companion.; Pseudocode is Ghidra decompiler output, not original source and not text-equivalent to Hopper output.; External functions and functions without an analyzable body return null; other decompiler failures remain explicit.
// pseudocode: provider-specific; not original source


/* Library Function - Single Match
    void __cdecl BuildCatchObject(struct EHExceptionRecord *,struct EHRegistrationNode *,struct
   _s_HandlerType const *,struct _s_CatchableType const *)
   
   Library: Visual Studio 1998 Release */

void __cdecl
BuildCatchObject(EHExceptionRecord *param_1,EHRegistrationNode *param_2,_s_HandlerType *param_3,
                _s_CatchableType *param_4)

{
  EHRegistrationNode *_Dst;
  bool bVar1;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  void *pvVar2;
  undefined3 extraout_var_01;
  undefined3 extraout_var_02;
  undefined3 extraout_var_03;
  undefined3 extraout_var_04;
  undefined3 extraout_var_05;
  undefined3 extraout_var_06;
  int iVar3;
  undefined4 *unaff_FS_OFFSET;
  size_t _Size;
  undefined4 local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_14 = *unaff_FS_OFFSET;
  puStack_c = &DAT_0045b8a0;
  puStack_10 = &LAB_00453f64;
  *unaff_FS_OFFSET = &local_14;
  if (((*(int *)(param_3 + 4) != 0) && (*(char *)(*(int *)(param_3 + 4) + 8) != '\0')) &&
     (*(int *)(param_3 + 8) != 0)) {
    _Dst = param_2 + *(int *)(param_3 + 8) + 0xc;
    local_8 = 0;
    if (((byte)*param_3 & 8) == 0) {
      if (((byte)*param_4 & 1) == 0) {
        if (*(int *)(param_4 + 0x18) == 0) {
          bVar1 = FID_conflict__ValidateRead(*(void **)(param_1 + 0x18),1);
          if ((CONCAT31(extraout_var_03,bVar1) != 0) &&
             (bVar1 = FID_conflict__ValidateRead(_Dst,1), CONCAT31(extraout_var_04,bVar1) != 0)) {
            _Size = *(size_t *)(param_4 + 0x14);
            pvVar2 = AdjustPointer(*(void **)(param_1 + 0x18),(PMD *)(param_4 + 8));
            FID_conflict__memcpy(_Dst,pvVar2,_Size);
            goto LAB_0044f8ce;
          }
        }
        else {
          bVar1 = FID_conflict__ValidateRead(*(void **)(param_1 + 0x18),1);
          if (((CONCAT31(extraout_var_05,bVar1) != 0) &&
              (bVar1 = FID_conflict__ValidateRead(_Dst,1), CONCAT31(extraout_var_06,bVar1) != 0)) &&
             (iVar3 = _ValidateExecute(*(_func_int **)(param_4 + 0x18)), iVar3 != 0)) {
            if (((byte)*param_4 & 4) == 0) {
              AdjustPointer(*(void **)(param_1 + 0x18),(PMD *)(param_4 + 8));
              FID_conflict__CallMemberFunction1(_Dst,*(undefined **)(param_4 + 0x18));
            }
            else {
              AdjustPointer(*(void **)(param_1 + 0x18),(PMD *)(param_4 + 8));
              FID_conflict__CallMemberFunction1(_Dst,*(undefined **)(param_4 + 0x18));
            }
            goto LAB_0044f8ce;
          }
        }
      }
      else {
        bVar1 = FID_conflict__ValidateRead(*(void **)(param_1 + 0x18),1);
        if ((CONCAT31(extraout_var_01,bVar1) != 0) &&
           (bVar1 = FID_conflict__ValidateRead(_Dst,1), CONCAT31(extraout_var_02,bVar1) != 0)) {
          FID_conflict__memcpy(_Dst,*(void **)(param_1 + 0x18),*(size_t *)(param_4 + 0x14));
          if ((*(int *)(param_4 + 0x14) == 4) && (*(void **)_Dst != (void *)0x0)) {
            pvVar2 = AdjustPointer(*(void **)_Dst,(PMD *)(param_4 + 8));
            *(void **)_Dst = pvVar2;
          }
          goto LAB_0044f8ce;
        }
      }
    }
    else {
      bVar1 = FID_conflict__ValidateRead(*(void **)(param_1 + 0x18),1);
      if ((CONCAT31(extraout_var,bVar1) != 0) &&
         (bVar1 = FID_conflict__ValidateRead(_Dst,1), CONCAT31(extraout_var_00,bVar1) != 0)) {
        pvVar2 = *(void **)(param_1 + 0x18);
        *(void **)_Dst = pvVar2;
        pvVar2 = AdjustPointer(pvVar2,(PMD *)(param_4 + 8));
        *(void **)_Dst = pvVar2;
        goto LAB_0044f8ce;
      }
    }
    FUN_0044fa70();
  }
LAB_0044f8ce:
  *unaff_FS_OFFSET = local_14;
  return;
}

