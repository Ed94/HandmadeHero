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

#include <stdio.h>

#include "win32.h"
NS_WIN32_BEGIN

// TODO(Ed) : This is a global for now.
global bool       Running;

global BITMAPINFO ScreenBitmapInfo;
global void*      ScreenBitmapMemory; // Lets use directly mess with the "pixel's memory buffer"
global u32        BitmapWidth;
global u32        BitmapHeight;

constexpr u32 	  BytesPerPixel = 4;

internal void
render_weird_graident( u32 x_offset, u32 y_offset )
{
	struct Pixel {
		u8 Blue;
		u8 Green;
		u8 Red;
		u8 Alpha;
	};

	u32 pitch = BitmapWidth * BytesPerPixel;
	u8* row   = rcast( u8*, ScreenBitmapMemory);
	local_persist float wildcard = 0;
	for ( u32 y = 0; y < BitmapHeight; ++ y )
	{
		// u8* pixel = rcast(u8*, row);
		// Pixel* pixel = rcast( Pixel*, row );
		u32* pixel = rcast(u32*, row);
		for ( u32 x = 0; x < BitmapWidth; ++ x )
		{
			/* Pixel in memory:
			-----------------------------------------------
				Pixel + 0  Pixel + 1  Pixel + 2   Pixel + 3
				RR         GG         GG          XX
			-----------------------------------------------
 				x86-64 : Little Endian Arch
				0x XX BB GG RR
			*/
			// pixel[0] = scast(u8, x + x_offset);
			// pixel[1] = scast(u8, y + y_offset);
			// pixel[2] = 0;
			// pixel[3] = 0;

			// pixel->Blue  = scast(u8, x + x_offset * u8(wildcard) % 256);
			// pixel->Green = scast(u8, y + y_offset - u8(wildcard) % 128);
			// pixel->Red   = scast(u8, wildcard) % 256 - x * 0.1f;
			// pixel->Alpha = 0;

			// ++pixel;
			// pixel += BytesPerPixel;

			u8 blue  = scast(u8, x + x_offset * u8(wildcard) % 256);
			u8 green = scast(u8, y + y_offset - u8(wildcard) % 128);
			u8 red   = scast(u8, wildcard) % 256 - x * 0.4f;

			*pixel++ = (red << 16) | (green << 8) | blue;
		}
		// wildcard += 0.001f;
		wildcard += .25f * 0.5;
		row += pitch;
	}
}

internal void
resize_dib_section( u32 width, u32 height )
{
	// TODO(Ed) : Bulletproof memory handling here for the bitmap memory

	// TODO(Ed) : Free DIB section

	if ( ScreenBitmapMemory )
	{
		VirtualFree( ScreenBitmapMemory, 0, MEM_RELEASE );
	}

	BitmapWidth  = width;
	BitmapHeight = height;

	// Negative means top-down in the context of the biHeight
#	define Top_Down -
	constexpr BITMAPINFOHEADER&
	header = ScreenBitmapInfo.bmiHeader;
	ScreenBitmapInfo.bmiHeader.biSize          = sizeof( BITMAPINFOHEADER );
	ScreenBitmapInfo.bmiHeader.biWidth         = BitmapWidth;
	ScreenBitmapInfo.bmiHeader.biHeight        = Top_Down BitmapHeight;
	ScreenBitmapInfo.bmiHeader.biPlanes        = 1;
	ScreenBitmapInfo.bmiHeader.biBitCount      = 32; // Need 24, but want 32 ( alignment )
	ScreenBitmapInfo.bmiHeader.biCompression   = BI_RGB_Uncompressed;
	// ScreenBitmapInfo.bmiHeader.biSizeImage     = 0;
	// ScreenBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	// ScreenBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	// ScreenBitmapInfo.bmiHeader.biClrUsed	   = 0;
	// ScreenBitmapInfo.bmiHeader.biClrImportant  = 0;
#	undef Top_Down
#if 0
	ScreenBitmapHandle = CreateDIBSection( ScreenDeviceContext, & ScreenBitmapInfo
		, DIB_ColorTable_RGB, & ScreenBitmapMemory
		// Ignoring these last two
		, 0, 0 );
#endif

	// ReleaseContext( 0, ScreenDeviceContext );

	// We want to "touch" a pixel on every 4-byte boundary
	u32 BitmapMemorySize = (BitmapWidth * BitmapHeight) * BytesPerPixel;
	ScreenBitmapMemory = VirtualAlloc( NULL, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE );

	// TODO(Ed) : Clear to black
}

internal void
update_window( HDC device_context, RECT* WindowRect
	, u32 x, u32 y
	, u32 width, u32 height )
{
#if 0
	BitBlt( device_context
		, x, y, width, height
		, ScreenDeviceContext
		, x, y, RO_Source_To_Dest
	);
#else
	u32 window_width  = WindowRect->right  - WindowRect->left;
	u32 window_height = WindowRect->bottom - WindowRect->top;

	StretchDIBits( device_context
	#if 0
		, x, y, width, height
		, x, y, width, height
	#endif
		, 0, 0, BitmapWidth, BitmapHeight
		, 0, 0, window_width, window_height
		, ScreenBitmapMemory, & ScreenBitmapInfo
		, DIB_ColorTable_RGB, RO_Source_To_Dest );
#endif
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
			u32 width  = info.rcPaint.right  - info.rcPaint.left;
			u32 height = info.rcPaint.bottom - info.rcPaint.top;

			RECT client_rect;
			GetClientRect( handle, & client_rect );

			update_window( device_context, & client_rect
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
		}
		break;

		default:
		{
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
	// MessageBox( 0, L"First message!", L"Handmade Hero", MB_Ok_Btn | MB_Icon_Information );

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

	if ( RegisterClassW( & window_class ) )
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

			MSG  msg_info;

			u32 x_offset = 0;
			u32 y_offset = 0;

			while( Running )
			{
				if ( PeekMessageW( & msg_info, 0, 0, 0, PM_Remove_Messages_From_Queue ) )
				{
					if ( msg_info.message == WM_QUIT  )
					{
						OutputDebugStringA("WM_QUIT\n");
						Running = false;
					}

					TranslateMessage( & msg_info );
					DispatchMessage( & msg_info );
				}

				render_weird_graident( x_offset, y_offset );

				RECT window_rect;
				GetClientRect( window_handle, & window_rect );
				HDC device_context = GetDC( window_handle );
				u32 window_width  = window_rect.right  - window_rect.left;
				u32 window_height = window_rect.bottom - window_rect.top;
				update_window( device_context, & window_rect, 0, 0, window_width, window_height );

				++ x_offset;
				++ y_offset;
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
