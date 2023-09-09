/*
Alternative header for windows.h
*/

#include "windows/windows_base.h"
#include "windows/window.h"

// #ifdef Build_Debug
#	include "windows/dbghelp.h"
// #endif

#if Build_DLL
#	define WIN_LIBRARY_API extern "C" __declspec(dllexport)
#else
#	define WIN_LIBRARY_API extern "C"
#endif

#ifndef CONST
#	define CONST const
#endif

#define WM_ACTIVATEAPP 0x001C


// namespace win32 {

#ifdef UNICODE
	constexpr auto CreateWindowEx = CreateWindowExW;
#else
	constexpr auto CreateWindowEx = CreateWindowExA;
#endif // !UNICODE

#ifdef UNICODE
	constexpr auto DefWindowProc = DefWindowProcW;
#else
	constexpr auto DefWindowProc = DefWindowProcA;
#endif // !UNICODE

#ifdef UNICODE
	constexpr auto DispatchMessage = DispatchMessageW;
#else
	constexpr auto DispatchMessage = DispatchMessageA;
#endif // !UNICODE

#pragma region Message Function Templates
// From WinUser.h, modular headers lib doesn't have.

WIN_LIBRARY_API BOOL WINAPI GetMessageA(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax);
WIN_LIBRARY_API BOOL WINAPI GetMessageW(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax);
#ifdef UNICODE
	constexpr auto GetMessage = GetMessageW;
#else
	constexpr auto GetMessage = GetMessageA;
#endif // !UNICODE

#ifdef UNICODE
	constexpr auto MessageBox = MessageBoxW;
#else
	constexpr auto MessageBox = MessageBoxA;
#endif // !UNICODE

#ifdef UNICODE
	constexpr auto RegisterClass = RegisterClassW;
#else
	constexpr auto RegisterClass = RegisterClassA;
#endif // !UNICODE
#pragma endregion Message Function Templates

#pragma region Window PAINT
typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

WIN_LIBRARY_API HDC WINAPI
BeginPaint( HWND hWnd, LPPAINTSTRUCT lpPaint );

WIN_LIBRARY_API BOOL WINAPI
EndPaint( HWND hWnd, CONST PAINTSTRUCT *lpPaint );

WIN_LIBRARY_API BOOL WINAPI PatBlt( HDC hdc, int x, int y, int w, int h, DWORD rop );
#pragma endregion Window PAINT

// Class Style Constants
// https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles

enum CS : UINT
{
	CS_Own_Device_Context = CS_OWNDC,
	CS_Horizontal_Redraw  = CS_HREDRAW,
	CS_Vertical_Redraw    = CS_VREDRAW,
};

enum MB : UINT
{
	MB_Ok_Btn = MB_OK,
	MB_Icon_Information = MB_ICONINFORMATION,
};

enum WS : UINT
{
	WS_Overlapped_Window = WS_OVERLAPPEDWINDOW,
	WS_Initially_Visible = WS_VISIBLE,
};

enum RasterOps : DWORD
{
	RO_Blackness = (DWORD)0x00000042,
	RO_Whiteness = (DWORD)0x00FF0062,
};

// }
