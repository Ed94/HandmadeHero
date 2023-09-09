/*
Alternative header for windows.h
*/

#include "windows/windows_base.h"
#include "windows/window.h"

// #ifdef Build_Debug
#	include "windows/dbghelp.h"
// #endif

#if Build_DLL
#	define WIN_LIBRARY_API_START extern "C" __declspec(dllexport)
#	define WIN_LIBRARY_API_END
#else
#	define WIN_LIBRARY_API_START extern "C" {
#	define WIN_LIBRARY_API_END }
#endif

#ifndef CONST
#	define CONST const
#endif

#define NS_WIN32_BEGIN namespace win32 {
#define NS_WIN32_END }

NS_WIN32_BEGIN
WIN_LIBRARY_API_START

#pragma region Gdi32

#define _SAL_nop_impl_                      X
#define _Deref_post2_impl_(p1,p2)
#define _SAL2_Source_(Name, args, annotes) _SA_annotes3(SAL_name, #Name, "", "2") _Group_(annotes _SAL_nop_impl_)
#define _Outptr_result_bytebuffer_(size)   _SAL2_Source_(_Outptr_result_bytebuffer_, (size), _Out_impl_ _Deref_post2_impl_(__notnull_impl_notref, __bytecap_impl(size)))

DECLARE_HANDLE(HBITMAP);

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

#define GDI_DIBSIZE(bi) ((bi).biHeight < 0 ? (-1)*(GDI__DIBSIZE(bi)) : GDI__DIBSIZE(bi))

HDC WINAPI CreateCompatibleDC( HDC hdc);

HBITMAP WINAPI
CreateDIBSection(
    HDC               hdc,
    CONST BITMAPINFO *pbmi,
    UINT              usage,
    _When_((pbmi->bmiHeader.biBitCount != 0), _Outptr_result_bytebuffer_(_Inexpressible_(GDI_DIBSIZE((pbmi->bmiHeader)))))
    _When_((pbmi->bmiHeader.biBitCount == 0), _Outptr_result_bytebuffer_((pbmi->bmiHeader).biSizeImage))
    VOID            **ppvBits,
    HANDLE            hSection,
    DWORD             offset
);

typedef HANDLE HGDIOBJ;
BOOL WINAPI DeleteObject( HGDIOBJ ho);

int WINAPI StretchDIBits( HDC hdc
	, int xDest, int yDest, int DestWidth, int DestHeight
	, int xSrc,  int ySrc,  int SrcWidth,  int SrcHeight,
    CONST VOID* lpBits, CONST BITMAPINFO* lpbmi
	, UINT iUsage, DWORD rop );

typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

HDC WINAPI
BeginPaint( HWND hWnd, LPPAINTSTRUCT lpPaint );

BOOL WINAPI
EndPaint( HWND hWnd, CONST PAINTSTRUCT *lpPaint );

BOOL WINAPI
PatBlt( HDC hdc
	, int x, int y
	, int w, int h
	, DWORD rop );
#pragma endregion Gdi32

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

BOOL WINAPI
GetMessageA(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax);
BOOL WINAPI
GetMessageW(
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

// Class Style Constants
// https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles

enum BI : DWORD
{
	BI_RGB_Uncompressed = 0L,
	BI_RunLength_Encoded_8bpp = 1L,
	BI_RunLength_Encoded_4bpp = 2L,
};

enum CS : UINT
{
	CS_Own_Device_Context = CS_OWNDC,
	CS_Horizontal_Redraw  = CS_HREDRAW,
	CS_Vertical_Redraw    = CS_VREDRAW,
};

enum DIB : UINT
{
	DIB_ColorTable_RGB     = 0,
	DIB_ColorTable_Palette = 1

};

enum MB : UINT
{
	MB_Ok_Btn = MB_OK,
	MB_Icon_Information = MB_ICONINFORMATION,
};

#define WM_ACTIVATEAPP 0x001C

enum WS : UINT
{
	WS_Overlapped_Window = WS_OVERLAPPEDWINDOW,
	WS_Initially_Visible = WS_VISIBLE,
};

enum RasterOps : DWORD
{
	RO_Source_To_Dest = (DWORD)0x00CC0020,
	RO_Blackness      = (DWORD)0x00000042,
	RO_Whiteness      = (DWORD)0x00FF0062,
};

WIN_LIBRARY_API_END
NS_WIN32_END

#undef _SAL_nop_impl_
#undef _SAL2_Source_
#undef _Deref_post2_impl_
#undef _Outptr_result_bytebuffer_
#undef _At_
#undef _When_
#undef GDI_DIBSIZE
