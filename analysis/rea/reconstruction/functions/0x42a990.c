// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x42a990
// name: CPropertySheet::SetWizardMode
// ranges: 0x42a990-0x42a9ac
// calls: -

/* Library Function - Single Match
    public: void __thiscall CPropertySheet::SetWizardMode(void)
   
   Library: Visual Studio 1998 Debug */

void __thiscall CPropertySheet::SetWizardMode(CPropertySheet *this)

{
  *(uint *)(this + 8) = *(uint *)(this + 8) | 1;
  return;
}
