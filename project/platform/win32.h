/*
Alternative header for windows.h
*/

#include "windows/windows_base.h"
#include "windows/window.h"

#ifdef UNICODE
constexpr auto MessageBox = MessageBoxW;
#else
constexpr auto MessageBox = MessageBoxA;
#endif // !UNICODE
