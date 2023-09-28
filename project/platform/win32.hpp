/*
	Windows dependency header
*/
#pragma once

#pragma warning( push )
#pragma warning( disable: 5105 )
#pragma warning( disable: 4820 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xinput.h>
#include <mmeapi.h>
#include <dsound.h>
#include <timeapi.h>
#pragma warning( pop )

// #include "windows/windows_base.h"
// #include "windows/window.h"

// #include "windows/file.h"
// #include "windows/io.h"

// #if Build_Debug
// #	include "windows/dbghelp.h"
// #endif

#if Build_DLL
#	define WIN_LIB_API extern "C" __declspec(dllexport)
#else
#	define WIN_LIB_API extern "C"
#endif

// #ifndef CONST
// #	define CONST const
// #endif

// SAL BS
#ifndef _In_
#	define _In_
#endif

#define NS_WIN32_BEGIN namespace win32 {
#define NS_WIN32_END }

NS_WIN32_BEGIN

enum LWA : DWORD
{
	LWA_Alpha    = 0x00000002,
	LWA_ColorKey = 0x00000001,
};

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

enum CW : s32
{
	CW_Use_Default = CW_USEDEFAULT,
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

enum Mem : DWORD
{
	MEM_Commit_Zeroed = MEM_COMMIT,
	MEM_Reserve	      = MEM_RESERVE,
	MEM_Release 	  = MEM_RELEASE,
};

enum Page : DWORD
{
	Page_Read_Write = PAGE_READWRITE,
};

enum PM : UINT
{
	PM_Remove_Messages_From_Queue = PM_REMOVE,
};

enum RasterOps : DWORD
{
	RO_Source_To_Dest = (DWORD)0x00CC0020,
	RO_Blackness      = (DWORD)0x00000042,
	RO_Whiteness      = (DWORD)0x00FF0062,
};

#define WM_ACTIVATEAPP 0x001C

enum WS : UINT
{
	WS_Overlapped_Window = WS_OVERLAPPEDWINDOW,
	WS_Initially_Visible = WS_VISIBLE,
};

enum XI_State : DWORD
{
	XI_PluggedIn = ERROR_SUCCESS,
};


template< typename ProcSignature >
ProcSignature* get_procedure_from_library( HMODULE library_module, char const* symbol )
{
	void* address = rcast( void*, GetProcAddress( library_module, symbol ) );
	return rcast( ProcSignature*, address );
}

#pragma region XInput
WIN_LIB_API DWORD WINAPI XInputGetState
(
	DWORD         dwUserIndex,  // Index of the gamer associated with the device
	XINPUT_STATE* pState        // Receives the current state
);

WIN_LIB_API DWORD WINAPI XInputSetState
(
	DWORD             dwUserIndex,  // Index of the gamer associated with the device
	XINPUT_VIBRATION* pVibration    // The vibration information to send to the controller
);

DWORD WINAPI xinput_get_state_stub( DWORD dwUserIndex, XINPUT_STATE* pVibration ) {
	do_once_start
		OutputDebugStringA( "xinput_get_state stubbed!\n");
	do_once_end
	return ERROR_DEVICE_NOT_CONNECTED;
}

DWORD WINAPI xinput_set_state_stub( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration ) {
	do_once_start
		OutputDebugStringA( "xinput_set_state stubbed!\n");
	do_once_end
	return ERROR_DEVICE_NOT_CONNECTED;
}

using XInputGetStateFn = DWORD WINAPI( DWORD dwUserIndex, XINPUT_STATE* pVibration );
using XInputSetStateFn = DWORD WINAPI( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration );

global XInputGetStateFn* xinput_get_state = xinput_get_state_stub;
global XInputSetStateFn* xinput_set_state = xinput_set_state_stub;

internal void
xinput_load_library_bindings()
{
	HMODULE xinput_lib = LoadLibraryA( XINPUT_DLL_A );

	XInputGetStateFn* get_state = get_procedure_from_library< XInputGetStateFn >( xinput_lib, "XInputGetState" );
	XInputSetStateFn* set_state = get_procedure_from_library< XInputSetStateFn >( xinput_lib, "XInputSetState" );

	if ( get_state )
		xinput_get_state = get_state;

	if ( set_state )
		xinput_set_state = set_state;
}
#pragma endregion XInput

NS_WIN32_END
#undef _SAL_nop_impl_
#undef _SAL2_Source_
#undef _Deref_post2_impl_
#undef _Outptr_result_bytebuffer_
#undef _At_
#undef _When_
#undef GDI_DIBSIZE
