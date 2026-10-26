// Reconstructed pseudocode (decompiler output, not original source)
// address: 0x43b0b0
// name: CDataBoundProperty::`scalar_deleting_destructor'
// ranges: 0x43b0b0-0x43b0e8
// calls: 0x44a820

/* Library Function - Single Match
    public: void * __thiscall CDataBoundProperty::`scalar deleting destructor'(unsigned int)
   
   Library: Visual Studio 1998 Debug */

void * __thiscall
CDataBoundProperty::_scalar_deleting_destructor_(CDataBoundProperty *this,uint param_1)

{
  ~CDataBoundProperty(this);
  if ((param_1 & 1) != 0) {
    FUN_0044a820(this);
  }
  return this;
}
