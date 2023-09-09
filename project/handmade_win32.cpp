#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wvarargs"
#pragma clang diagnostic ignored "-Wunused-function"
#endif

#include "grime.h"
#include "macros.h"
#include "types.h"

#include "win32.h"
// using namespace win32;

// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc
// https://learn.microsoft.com/en-us/windows/win32/winmsg/about-messages-and-message-queues#system-defined-messages
LRESULT CALLBACK MainWindowCallback(
	HWND   handle,
	UINT   system_messages,
	WPARAM w_param,
	LPARAM l_param
)
{
	LRESULT result;

	// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-notifications
	switch ( system_messages )
	{
		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA( "WM_ACTIVATEAPP\n" );
		}
		break;

		case WM_CLOSE:
		{
			OutputDebugStringA( "WM_CLOSE\n" );
		}
		break;

		case WM_DESTROY:
		{
			OutputDebugStringA( "WM_DESTROY\n" );
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT info;
			HDC device_context = BeginPaint( handle, & info );

			u32 x 	   = info.rcPaint.left;
			u32 y 	   = info.rcPaint.top;
			u32 height = info.rcPaint.bottom - info.rcPaint.top;
			u32 width  = info.rcPaint.right  - info.rcPaint.left;

			global DWORD operation = RO_Whiteness;

			PatBlt( device_context
				, x, y
				, width, height
				, operation );

			operation == RO_Whiteness ?
				operation = RO_Blackness
			:	operation = RO_Whiteness;

			EndPaint( handle, & info );
		}
		break;

		case WM_SIZE:
		{
			OutputDebugStringA( "WM_SIZE\n" );
		}
		break;

		default:
		{
			OutputDebugStringA( "default\n" );
			result = DefWindowProc( handle, system_messages, w_param, l_param );
		}
	}

	return result;
}

int CALLBACK WinMain(
	HINSTANCE instance,
	HINSTANCE prev_instance,
	LPSTR     commandline,
	int       show_command
)
{
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
	// MessageBox( 0, L"First message!", L"Handmade Hero", MB_Ok_Btn | MB_Icon_Information );

	// https://en.wikibooks.org/wiki/Windows_Programming/Window_Creation
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa
	WNDCLASS window_class {};
		// window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		window_class.style = CS_Own_Device_Context | CS_Horizontal_Redraw | CS_Vertical_Redraw;
		window_class.lpfnWndProc = MainWindowCallback;
		// window_class.cbClsExtra  = ;
		// window_class.cbWndExtra  = ;
		window_class.hInstance   = instance;
		// window_class.hIcon = ;
		// window_class.hCursor = ;
		// window_class.hbrBackground = ;
		window_class.lpszMenuName  = L"Handmade Hero!";
		window_class.lpszClassName = L"HandmadeHeroWindowClass";

	if ( RegisterClassW( &window_class ) )
	{
		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
		HWND window_handle = CreateWindowExW(
			0,
			window_class.lpszClassName,
			L"Handmade Hero",
			// WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			WS_Overlapped_Window | WS_Initially_Visible,
			CW_USEDEFAULT, CW_USEDEFAULT, // x, y
			CW_USEDEFAULT, CW_USEDEFAULT, // width, height
			0, 0,                         // parent, menu
			instance, 0                   // instance, param
		);

		if ( window_handle )
		{
			// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
			// ShowWindow( window_handle, show_command );

			// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-updatewindow
			// UpdateWindow( window_handle );

			// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
			MSG message;
			for (;;)
			{
				BOOL msg_result = GetMessage( & message, 0, 0, 0 );
				if ( msg_result > 0 )
				{
					// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
					TranslateMessage( & message );

					// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage
					DispatchMessage( & message );
					// break;
				}
				else
				{
					// TODO (Ed) : Logging
					break;
				}
			}
		}
		else
		{
			// TODO (Ed) : Logging
		}
	}
	else
	{
		// TODO(Ed) : Logging
	}

	return 0;
}
