# Engine map from the saved exports

Mechanical grouping of decompiler output. No original module names,
directory structure or types are recovered. Regions are 64 KiB address
buckets; they follow the original linker layout, not a source tree.

## Regions

| Region | Functions | Bytes | Largest functions |
| --- | ---: | ---: | --- |
| `0x400000` | 253 | 60267 | `0x40305a` (2696 B), `0x403d54` (1371 B), `0x4013ae` (1216 B) |
| `0x410000` | 217 | 64006 | `0x41e289` (3222 B), `0x410669` (2208 B), `0x41a0c7` (1969 B) |
| `0x420000` | 207 | 70563 | `0x42db22` (15385 B), `0x42c8a0` (2370 B), `0x42913b` (2143 B) |
| `0x430000` | 163 | 54414 | `0x43340a` (11961 B), `0x436b01` (4299 B), `0x438173` (2439 B) |
| `0x440000` | 230 | 41753 | `0x44db10` (2255 B), `0x444000` (1202 B), `0x44ad87` (1183 B) |
| `0x450000` | 167 | 34379 | `0x451d90` (3033 B), `0x4599e0` (2191 B), `0x4579f0` (1578 B) |

## Code bases

- `0x401000-0x43ffff`: **840** functions, 249250 body bytes, 0 with an underscored CRT-style name.
- `0x440000-0x45ffff`: **397** functions, 76132 body bytes, 144 with an underscored CRT-style name.

The address split follows the recovered symbol names: every
function at or above 0x440000 that is named is a CRT or thunk
routine, while the 0x40xxxx-0x43xxxx range contains no
underscored name. Treat the split as an inference.


## Most-called game functions (utility candidates)

| Address | Name | Bytes | Callers |
| --- | --- | ---: | ---: |
| `0x418040` | FUN_00418040 | 90 | 37 |
| `0x4094f2` | FUN_004094f2 | 33 | 35 |
| `0x40ac43` | FUN_0040ac43 | 11 | 32 |
| `0x40ac38` | FUN_0040ac38 | 11 | 30 |
| `0x416220` | FUN_00416220 | 21 | 26 |
| `0x409457` | FUN_00409457 | 155 | 24 |
| `0x436652` | FUN_00436652 | 72 | 24 |
| `0x4161f6` | FUN_004161f6 | 21 | 24 |
| `0x40a40d` | FUN_0040a40d | 99 | 20 |
| `0x41624a` | FUN_0041624a | 75 | 20 |
| `0x40df05` | FUN_0040df05 | 675 | 17 |
| `0x40c624` | FUN_0040c624 | 178 | 17 |
| `0x4181c5` | FUN_004181c5 | 485 | 16 |
| `0x41842b` | FUN_0041842b | 325 | 16 |
| `0x40a4bc` | FUN_0040a4bc | 184 | 15 |
| `0x40a1e0` | FUN_0040a1e0 | 77 | 15 |
| `0x40d9f7` | FUN_0040d9f7 | 40 | 15 |
| `0x40da1f` | FUN_0040da1f | 45 | 14 |
| `0x41d440` | FUN_0041d440 | 26 | 14 |
| `0x407420` | FUN_00407420 | 263 | 13 |

## Largest game functions

| Address | Name | Bytes | Calls | Callers |
| --- | --- | ---: | ---: | ---: |
| `0x42db22` | FUN_0042db22 | 15385 | 97 | 3 |
| `0x43340a` | FUN_0043340a | 11961 | 69 | 4 |
| `0x436b01` | FUN_00436b01 | 4299 | 28 | 2 |
| `0x41e289` | FUN_0041e289 | 3222 | 53 | 2 |
| `0x40305a` | FUN_0040305a | 2696 | 1 | 4 |
| `0x438173` | FUN_00438173 | 2439 | 10 | 3 |
| `0x43b216` | FUN_0043b216 | 2401 | 12 | 8 |
| `0x42c8a0` | FUN_0042c8a0 | 2370 | 45 | 2 |
| `0x410669` | FUN_00410669 | 2208 | 15 | 4 |
| `0x42913b` | FUN_0042913b | 2143 | 3 | 2 |
| `0x41a0c7` | FUN_0041a0c7 | 1969 | 4 | 2 |
| `0x425ab4` | FUN_00425ab4 | 1965 | 45 | 3 |
| `0x43cda0` | FUN_0043cda0 | 1833 | 4 | 2 |
| `0x43a248` | FUN_0043a248 | 1461 | 15 | 2 |
| `0x415c42` | FUN_00415c42 | 1460 | 5 | 2 |
| `0x4131a1` | FUN_004131a1 | 1416 | 2 | 3 |
| `0x438de4` | FUN_00438de4 | 1381 | 7 | 2 |
| `0x403d54` | FUN_00403d54 | 1371 | 5 | 1 |
| `0x41535a` | FUN_0041535a | 1346 | 11 | 2 |
| `0x4013ae` | FUN_004013ae | 1216 | 14 | 3 |

## Game call-graph roots

No recovered internal caller; can be real entry points or
vtable/callback targets. Ten largest:

- `0x42a840` CDataBoundProperty::`scalar_deleting_destructor' — 57 B, 1 calls
- `0x43b0b0` CDataBoundProperty::`scalar_deleting_destructor' — 57 B, 1 calls
- `0x41ce20` CSplitterWnd::IsTracking — 31 B, 0 calls
- `0x41ce40` CSplitterWnd::IsTracking — 31 B, 0 calls
- `0x41ce60` CSplitterWnd::IsTracking — 31 B, 0 calls
- `0x42a990` CPropertySheet::SetWizardMode — 29 B, 0 calls
- `0x42a9b0` CPropertySheet::SetWizardMode — 29 B, 0 calls
- `0x413e11` CDataBoundProperty::~CDataBoundProperty — 22 B, 0 calls
- `0x42a880` CDataBoundProperty::~CDataBoundProperty — 22 B, 0 calls
- `0x43ddf6` AVIFIL32.DLL::AVIStreamRead — 6 B, 0 calls

## Imported API surface

- **KERNEL32.DLL** (78): CloseHandle, CompareStringA, CompareStringW, CreateFileA, CreateProcessA, CreateThread, DeleteFileA, ExitProcess, FileTimeToLocalFileTime, FileTimeToSystemTime, FindClose, FindFirstFileA, FindNextFileA, FlushFileBuffers, FreeEnvironmentStringsA, FreeEnvironmentStringsW, GetCPInfo, GetCommandLineA, GetDiskFreeSpaceA, GetDriveTypeA, GetEnvironmentStrings, GetEnvironmentStringsW, GetExitCodeProcess, GetFileAttributesA, GetFileType, GetLastError, GetLocalTime, GetLogicalDrives, GetModuleFileNameA, GetModuleHandleA, GetProcAddress, GetStartupInfoA, GetStdHandle, GetStringTypeA, GetStringTypeW, GetTickCount, GetTimeZoneInformation, GetVersion, GlobalAlloc, GlobalFree, GlobalHandle, GlobalLock, GlobalMemoryStatus, GlobalUnlock, HeapAlloc, HeapCreate, HeapDestroy, HeapFree, HeapReAlloc, HeapSize, InterlockedExchange, IsBadCodePtr, IsBadReadPtr, IsBadWritePtr, LCMapStringA, LCMapStringW, LoadLibraryA, MoveFileA, MultiByteToWideChar, OutputDebugStringA, RaiseException, ReadFile, RtlUnwind, SetConsoleCtrlHandler, SetEndOfFile, SetEnvironmentVariableA, SetFilePointer, SetHandleCount, SetStdHandle, SetThreadPriority, SetUnhandledExceptionFilter, Sleep, TerminateThread, UnhandledExceptionFilter, WaitForSingleObject, WideCharToMultiByte, WriteConsoleA, WriteFile
- **USER32.DLL** (36): AdjustWindowRectEx, ClientToScreen, CreateWindowExA, DefWindowProcA, DialogBoxParamA, DispatchMessageA, EndDialog, GetActiveWindow, GetClientRect, GetCursorPos, GetDC, GetDlgItem, GetMenu, GetMessageA, GetSystemMetrics, GetWindowLongA, GetWindowTextA, LoadCursorA, MessageBoxA, PeekMessageA, PostMessageA, RegisterClassA, ReleaseCapture, ReleaseDC, ScreenToClient, SendMessageA, SetCapture, SetFocus, SetRect, SetWindowLongA, SetWindowPos, SetWindowTextA, ShowCursor, SystemParametersInfoA, TranslateMessage, UpdateWindow
- **WINMM.DLL** (16): mmioAdvance, mmioAscend, mmioClose, mmioCreateChunk, mmioDescend, mmioGetInfo, mmioOpenA, mmioRead, mmioSeek, mmioSetInfo, mmioWrite, timeBeginPeriod, timeEndPeriod, timeGetTime, timeKillEvent, timeSetEvent
- **GDI32.DLL** (12): BitBlt, CreateCompatibleDC, CreateDIBSection, DeleteDC, DeleteObject, GdiFlush, GetDeviceCaps, GetStockObject, SelectObject, SelectPalette, SetStretchBltMode, StretchBlt
- **AVIFIL32.DLL** (11): AVIFileGetStream, AVIFileInfoA, AVIFileOpenA, AVIFileRelease, AVIStreamInfoA, AVIStreamLength, AVIStreamOpenFromFileA, AVIStreamRead, AVIStreamReadFormat, AVIStreamRelease, AVIStreamTimeToSample
- **MSVFW32.DLL** (5): DrawDibClose, ICClose, ICLocate, ICOpen, ICSendMessage
- **DDRAW.DLL** (1): DirectDrawCreate
- **DSOUND.DLL** (1): DirectSoundCreate

## Coarse call-graph clusters (game code only)

Label propagation over undirected call edges, clusters larger than
two functions, ranked by body bytes. Component labels are address
keys, not recovered module names.

| Label | Functions | Bytes | Address span | Largest members |
| --- | ---: | ---: | --- | --- |
| `0x401000` | 380 | 114546 | `0x401000`-`0x43dd96` | `0x436b01` (4299 B), `0x41e289` (3222 B), `0x40305a` (2696 B) |
| `0x401c21` | 100 | 40116 | `0x401c21`-`0x43c8d2` | `0x42db22` (15385 B), `0x43340a` (11961 B), `0x4326eb` (537 B) |
| `0x402a44` | 73 | 27476 | `0x402a44`-`0x438afa` | `0x438173` (2439 B), `0x410669` (2208 B), `0x411dfc` (1034 B) |
| `0x4011de` | 40 | 9576 | `0x4011de`-`0x43f86f` | `0x4242bd` (1186 B), `0x423310` (790 B), `0x40f135` (525 B) |
| `0x40114a` | 24 | 9440 | `0x40114a`-`0x42d594` | `0x42c8a0` (2370 B), `0x425ab4` (1965 B), `0x4131a1` (1416 B) |
| `0x404d10` | 12 | 5320 | `0x404d10`-`0x43d4c9` | `0x43cda0` (1833 B), `0x422c2d` (758 B), `0x404d10` (596 B) |
| `0x40b263` | 14 | 4865 | `0x40b263`-`0x43d847` | `0x413e6b` (741 B), `0x4146a7` (674 B), `0x40b912` (622 B) |
| `0x40ae93` | 13 | 3563 | `0x40ae93`-`0x43d883` | `0x439a28` (678 B), `0x439df6` (581 B), `0x40ae93` (497 B) |
| `0x422823` | 11 | 3253 | `0x422823`-`0x429066` | `0x4283a6` (951 B), `0x427de1` (532 B), `0x42826e` (312 B) |
| `0x42913b` | 5 | 2579 | `0x42913b`-`0x42a836` | `0x42913b` (2143 B), `0x42a6aa` (189 B), `0x429af7` (124 B) |
| `0x413e27` | 9 | 2462 | `0x413e27`-`0x43db29` | `0x43a248` (1461 B), `0x43d9c7` (354 B), `0x414a8b` (160 B) |
| `0x42a9e5` | 5 | 2189 | `0x42a9e5`-`0x42b272` | `0x42aa1a` (853 B), `0x42b039` (569 B), `0x42aebf` (378 B) |
| `0x405b90` | 9 | 2146 | `0x405b90`-`0x406f99` | `0x406cfe` (503 B), `0x405e7e` (451 B), `0x405c19` (409 B) |
| `0x405195` | 5 | 1379 | `0x405195`-`0x405b60` | `0x405195` (488 B), `0x405499` (405 B), `0x405a96` (202 B) |
| `0x429d1a` | 5 | 1363 | `0x429d1a`-`0x42a900` | `0x429d47` (702 B), `0x42a005` (260 B), `0x42a10e` (260 B) |
| `0x40f98c` | 6 | 1110 | `0x40f98c`-`0x4138a8` | `0x40f98c` (618 B), `0x413729` (217 B), `0x41383a` (110 B) |
| `0x41ae57` | 4 | 1054 | `0x41ae57`-`0x41bb0a` | `0x41ae57` (525 B), `0x41b9e7` (291 B), `0x41b064` (119 B) |
| `0x40cbcc` | 4 | 1021 | `0x40cbcc`-`0x426fc4` | `0x40cbcc` (426 B), `0x40cd76` (339 B), `0x4180f4` (209 B) |
| `0x42a349` | 8 | 978 | `0x42a349`-`0x42a98c` | `0x42a579` (295 B), `0x42a349` (290 B), `0x42a46b` (234 B) |
| `0x429080` | 5 | 973 | `0x429080`-`0x42a7af` | `0x429b73` (423 B), `0x42a217` (301 B), `0x429080` (187 B) |
| `0x40ad80` | 4 | 869 | `0x40ad80`-`0x43d509` | `0x4397c0` (425 B), `0x40ad80` (194 B), `0x413d50` (193 B) |
| `0x4259de` | 3 | 589 | `0x4259de`-`0x426b9d` | `0x4269ec` (433 B), `0x4259de` (118 B), `0x4269a0` (38 B) |
| `0x40d54d` | 3 | 385 | `0x40d54d`-`0x42b377` | `0x40d54d` (178 B), `0x42b2fc` (123 B), `0x4164a0` (84 B) |
| `0x40ae42` | 5 | 371 | `0x40ae42`-`0x43d52b` | `0x43999a` (142 B), `0x40ae42` (81 B), `0x43b070` (57 B) |
| `0x432be2` | 3 | 369 | `0x432be2`-`0x43cd9c` | `0x432be2` (131 B), `0x43ccae` (126 B), `0x43cd2c` (112 B) |

## Unimplemented game functions by size (roadmap)

No documented `port/` counterpart. This is a static inventory, not
a statement that each body must be ported verbatim.

| Address | Name | Bytes | Calls | Callers |
| --- | --- | ---: | ---: | ---: |
| `0x436b01` | FUN_00436b01 | 4299 | 28 | 2 |
| `0x41e289` | FUN_0041e289 | 3222 | 53 | 2 |
| `0x40305a` | FUN_0040305a | 2696 | 1 | 4 |
| `0x43b216` | FUN_0043b216 | 2401 | 12 | 8 |
| `0x410669` | FUN_00410669 | 2208 | 15 | 4 |
| `0x42913b` | FUN_0042913b | 2143 | 3 | 2 |
| `0x41a0c7` | FUN_0041a0c7 | 1969 | 4 | 2 |
| `0x425ab4` | FUN_00425ab4 | 1965 | 45 | 3 |
| `0x43cda0` | FUN_0043cda0 | 1833 | 4 | 2 |
| `0x43a248` | FUN_0043a248 | 1461 | 15 | 2 |
| `0x415c42` | FUN_00415c42 | 1460 | 5 | 2 |
| `0x4131a1` | FUN_004131a1 | 1416 | 2 | 3 |
| `0x438de4` | FUN_00438de4 | 1381 | 7 | 2 |
| `0x403d54` | FUN_00403d54 | 1371 | 5 | 1 |
| `0x41535a` | FUN_0041535a | 1346 | 11 | 2 |
| `0x4013ae` | FUN_004013ae | 1216 | 14 | 3 |
| `0x41920f` | FUN_0041920f | 1173 | 5 | 2 |
| `0x414cd2` | FUN_00414cd2 | 1161 | 13 | 2 |
| `0x4045f8` | FUN_004045f8 | 1048 | 5 | 1 |
| `0x411dfc` | FUN_00411dfc | 1034 | 8 | 3 |
| `0x41c704` | FUN_0041c704 | 1013 | 6 | 2 |
| `0x41a962` | FUN_0041a962 | 1010 | 3 | 2 |
| `0x439349` | FUN_00439349 | 999 | 6 | 2 |
| `0x4111c7` | FUN_004111c7 | 984 | 8 | 2 |
| `0x42c1b8` | FUN_0042c1b8 | 966 | 3 | 2 |
| `0x43c8d2` | FUN_0043c8d2 | 959 | 15 | 3 |
| `0x4283a6` | FUN_004283a6 | 951 | 8 | 3 |
| `0x4196a4` | FUN_004196a4 | 941 | 6 | 4 |
| `0x424dc0` | FUN_00424dc0 | 927 | 15 | 5 |
| `0x40186e` | FUN_0040186e | 918 | 23 | 2 |
| `0x408076` | FUN_00408076 | 862 | 10 | 2 |
| `0x42aa1a` | FUN_0042aa1a | 853 | 2 | 2 |
| `0x43fcd0` | FUN_0043fcd0 | 844 | 1 | 1 |
| `0x419d28` | FUN_00419d28 | 835 | 6 | 4 |
| `0x41d7ff` | FUN_0041d7ff | 820 | 9 | 2 |
| `0x426b9d` | FUN_00426b9d | 819 | 9 | 2 |
| `0x426261` | FUN_00426261 | 814 | 7 | 3 |
| `0x42c57e` | FUN_0042c57e | 796 | 2 | 2 |
| `0x42d594` | FUN_0042d594 | 796 | 11 | 3 |
| `0x4069a4` | FUN_004069a4 | 795 | 3 | 2 |

809 of 840 game functions have no documented port counterpart.
