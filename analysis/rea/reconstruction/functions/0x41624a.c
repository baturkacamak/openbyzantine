// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x41624a
// name: FUN_0041624a
// ranges: 0x41624a-0x416294
// calls: 0x41624a

void FUN_0041624a(void)

{
  BOOL BVar1;
  
  while( true ) {
    BVar1 = PeekMessageA((LPMSG)&DAT_0046c4f8,(HWND)0x0,0,0,1);
    if (BVar1 == 0) break;
    DAT_0046c514 = &DAT_0046c4f8;
    TranslateMessage((MSG *)&DAT_0046c4f8);
    DispatchMessageA((MSG *)&DAT_0046c4f8);
  }
  return;
}
