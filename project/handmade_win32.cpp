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
NS_WIN32_BEGIN

// TODO(Ed) : This is a global for now.
global bool       Running;

global BITMAPINFO ScreenBitmapInfo;
global void*      ScreenBitmapMemory; // Lets use directly mess with the "pixel's memory buffer"
global HBITMAP    ScreenBitmapHandle;
global HDC        ScreenDeviceContext;

internal void
resize_dib_section( u32 width, u32 height )
{
	// TODO(Ed) : Bulletproof memory handling here for the bitmap memory

	// TODO(Ed) : Free DIB section

	if ( ScreenBitmapHandle )
	{
		DeleteObject( ScreenBitmapHandle );
	}
	if ( ! ScreenDeviceContext )
	{
		ScreenDeviceContext = CreateCompatibleDC( 0 );
	}

	constexpr BITMAPINFOHEADER& header = ScreenBitmapInfo.bmiHeader;
	header.biSize          = sizeof( header );
	header.biWidth         = width;
	header.biHeight        = height;
	header.biPlanes        = 1;
	header.biBitCount      = 32; // Need 24, but want 32 ( alignment )
	header.biCompression   = BI_RGB_Uncompressed;
	header.biSizeImage     = 0;
	header.biXPelsPerMeter = 0;
	header.biYPelsPerMeter = 0;
	header.biClrUsed	   = 0;
	header.biClrImportant  = 0;
	ScreenBitmapHandle = CreateDIBSection( ScreenDeviceContext, & ScreenBitmapInfo
		, DIB_ColorTable_RGB, & ScreenBitmapMemory
		// Ignoring these last two
		, 0, 0 );

	// ReleaseContext( 0, ScreenDeviceContext );
}

internal void
update_window( HDC device_context
	, u32 x, u32 y
	, u32 width, u32 height )
{
	StretchDIBits( device_context
		, x, y, width, height
		, x, y, width, height
		, ScreenBitmapMemory, & ScreenBitmapInfo
		, DIB_ColorTable_RGB, RO_Source_To_Dest );
}

LRESULT CALLBACK
main_window_callback(
	HWND   handle,
	UINT   system_messages,
	WPARAM w_param,
	LPARAM l_param
)
{
	LRESULT result;

	switch ( system_messages )
	{
		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA( "WM_ACTIVATEAPP\n" );
		}
		break;

		case WM_CLOSE:
		{
			// TODO(Ed) : Handle with a message to the user
			Running = false;
		}
		break;

		case WM_DESTROY:
		{
			// TODO(Ed) : Handle with as an error and recreate the window
			Running = false;
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

			update_window( handle
				, x, y
				, width, height );

			EndPaint( handle, & info );
		}
		break;

		case WM_SIZE:
		{
			RECT client_rect;
			GetClientRect( handle, & client_rect );

			u32 width  = client_rect.right  - client_rect.left;
			u32 height = client_rect.bottom - client_rect.top;

			resize_dib_section( width, height );
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
NS_WIN32_END

int CALLBACK
WinMain(
	HINSTANCE instance,
	HINSTANCE prev_instance,
	LPSTR     commandline,
	int       show_command
)
{
	using namespace win32;
	MessageBox( 0, L"First message!", L"Handmade Hero", MB_Ok_Btn | MB_Icon_Information );

	WNDCLASS window_class {};
		window_class.style = CS_Own_Device_Context | CS_Horizontal_Redraw | CS_Vertical_Redraw;
		window_class.lpfnWndProc = main_window_callback;
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
		HWND window_handle = CreateWindowExW(
			0,
			window_class.lpszClassName,
			L"Handmade Hero",
			WS_Overlapped_Window | WS_Initially_Visible,
			CW_USEDEFAULT, CW_USEDEFAULT, // x, y
			CW_USEDEFAULT, CW_USEDEFAULT, // width, height
			0, 0,                         // parent, menu
			instance, 0                   // instance, param
		);

		if ( window_handle )
		{
			Running = true;

			MSG message;
			while( Running )
			{
				BOOL msg_result = GetMessage( & message, 0, 0, 0 );
				if ( msg_result > 0 )
				{
					TranslateMessage( & message );
					DispatchMessage( & message );
				}
				else
				{
					// TODO(Ed) : Logging
					break;
				}
			}
		}
		else
		{
			// TODO(Ed) : Logging
		}
	}
	else
	{
		// TODO(Ed) : Logging
	}

	return 0;
}
