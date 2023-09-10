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

// #include <stdio.h>

#include "win32.h"

// Using this to get dualsense controllers
#include "JoyShockLibrary/JoyShockLibrary.h"

// TOOD(Ed): Redo these macros properly later.

#define congrats( message ) do {                                               \
	JslSetLightColour( 0, (255 << 16) | (215 << 8) );                          \
	MessageBoxA( 0, message, "Congratulations!", MB_OK | MB_ICONEXCLAMATION ); \
	JslSetLightColour( 0, (255 << 8 ) );                                       \
} while (0)

#define ensure( condition, message ) ensure_impl( condition, message )
inline bool
ensure_impl( bool condition, char const* message ) {
	if ( ! condition ) {
		JslSetLightColour( 0, (255 << 16) );
		MessageBoxA( 0, message, "Ensure Failure", MB_OK | MB_ICONASTERISK );
		JslSetLightColour( 0, ( 255 << 8 ) );
	}
	return condition;
}

#define fatal(message) do {                                         \
	JslSetLightColour( 0, (255 << 16) );                            \
	MessageBoxA( 0, message, "Fatal Error", MB_OK | MB_ICONERROR ); \
	JslSetLightColour( 0, (255 << 8 ) );                            \
} while (0)


NS_WIN32_BEGIN

// TODO(Ed) : This is a global for now.
global bool Running;


struct OffscreenBuffer
{
	BITMAPINFO Info;
	void*      Memory; // Lets use directly mess with the "pixel's memory buffer"
	u32        Width;
	u32        Height;
	u32        Pitch;
	u32        BytesPerPixel;
};

struct WinDimensions
{
	u32 Width;
	u32 Height;
};

global OffscreenBuffer BackBuffer;
global WinDimensions   WindowDimensions;

HRESULT WINAPI
DirectSoundCreate(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );

using DirectSoundCreateFn = HRESULT WINAPI (LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );

global DirectSoundCreateFn* direct_sound_create;


internal void
init_sound(HWND window_handle, s32 samples_per_second, s32 buffer_size )
{
	// Load library
	HMODULE sound_library = LoadLibraryA( "dsound.dll" );
	if ( ! ensure(sound_library, "Failed to load direct sound library" ) )
	{
		// TOOD : Diagnostic
		return;
	}

	// Get direct sound object
	direct_sound_create = rcast( DirectSoundCreateFn*, GetProcAddress( sound_library, "DirectSoundCreate" ));
	if ( ! ensure(direct_sound_create, "Failed to get direct_sound_create_procedure" ) )
	{
		// TOOD : Diagnostic
		return;
	}

	LPDIRECTSOUND direct_sound;
	if ( ! SUCCEEDED(direct_sound_create( 0, & direct_sound, 0 )) )
	{
		// TODO : Diagnostic
	}

	if ( ! SUCCEEDED( direct_sound->SetCooperativeLevel(window_handle, DSSCL_PRIORITY) ) )
	{
		// TODO : Diagnostic
	}

	WAVEFORMATEX
	wave_format {};
	wave_format.wFormatTag      = WAVE_FORMAT_PCM;  /* format type */
	wave_format.nChannels       = 2;  /* number of channels (i.e. mono, stereo...) */
	wave_format.nSamplesPerSec  = samples_per_second;  /* sample rate */
	wave_format.wBitsPerSample  = 16;  /* number of bits per sample of mono data */
	wave_format.nBlockAlign     = wave_format.nChannels      * wave_format.wBitsPerSample / 8 ;  /* block size of data */
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;  /* for buffer estimation */
	wave_format.cbSize          = 0;  /* the count in bytes of the size of */

	LPDIRECTSOUNDBUFFER primary_buffer;
	{
		DSBUFFERDESC
		buffer_description { sizeof(buffer_description) };
		buffer_description.dwFlags       = DSBCAPS_PRIMARYBUFFER;
		buffer_description.dwBufferBytes = 0;

		if ( ! SUCCEEDED( direct_sound->CreateSoundBuffer( & buffer_description, & primary_buffer, 0 ) ))
		{
			// TODO : Diagnostic
		}
		if ( ! SUCCEEDED( primary_buffer->SetFormat( & wave_format ) ) )
		{
			// TODO : Diagnostic
		}

		// Format is finally set!
	}

	LPDIRECTSOUNDBUFFER secondary_buffer;
	{
		DSBUFFERDESC
		buffer_description { sizeof(buffer_description) };
		buffer_description.dwFlags       = 0;
		buffer_description.dwBufferBytes = buffer_size;
		buffer_description.lpwfxFormat   = & wave_format;

		if ( ! SUCCEEDED( direct_sound->CreateSoundBuffer( & buffer_description, & secondary_buffer, 0 ) ))
		{
			// TODO : Diagnostic
		}
		if ( ! SUCCEEDED( secondary_buffer->SetFormat( & wave_format ) ) )
		{
			// TODO : Diagnostic
		}
	}

	// Create primary buffer

	// Create secondary buffer

	// Start playing
}

internal WinDimensions
get_window_dimensions( HWND window_handle )
{
	RECT client_rect;
	GetClientRect( window_handle, & client_rect );
	WinDimensions result;
	result.Width  = client_rect.right  - client_rect.left;
	result.Height = client_rect.bottom - client_rect.top;
	return result;
}

internal void
render_weird_graident(OffscreenBuffer* buffer, u32 x_offset, u32 y_offset )
{
	// TODO(Ed): See if with optimizer if buffer should be passed by value.

	struct Pixel {
		u8 Blue;
		u8 Green;
		u8 Red;
		u8 Alpha;
	};

	u8* row   = rcast( u8*, buffer->Memory);
	local_persist float wildcard = 0;
	for ( u32 y = 0; y < buffer->Height; ++ y )
	{
		// u8* pixel = rcast(u8*, row);
		// Pixel* pixel = rcast( Pixel*, row );
		u32* pixel = rcast(u32*, row);
		for ( u32 x = 0; x < buffer->Width; ++ x )
		{
			/* Pixel in memory:
			-----------------------------------------------
				Pixel + 0  Pixel + 1  Pixel + 2   Pixel + 3
				RR         GG         GG          XX
			-----------------------------------------------
 				x86-64 : Little Endian Arch
				0x XX BB GG RR
			*/
		#if 0
			u8 blue  = scast(u8, x + x_offset * u8(wildcard) % 256);
			u8 green = scast(u8, y + y_offset - u8(wildcard) % 128);
			u8 red   = scast(u8, wildcard) % 256 - x * 0.4f;
		#else
			u8 blue  = scast(u8, x + x_offset);
			u8 green = scast(u8, y + y_offset);
			u8 red   = 0;
		#endif

			*pixel++ = (red << 16) | (green << 8) | blue;
		}
		wildcard += 0.5375f;
		row += buffer->Pitch;
	}
}

internal void
resize_dib_section( OffscreenBuffer* buffer, u32 width, u32 height )
{
	// TODO(Ed) : Bulletproof memory handling here for the bitmap memory
	if ( buffer->Memory )
	{
		VirtualFree( buffer->Memory, 0, MEM_RELEASE );
	}

	buffer->Width         = width;
	buffer->Height        = height;
	buffer->BytesPerPixel = 4;
	buffer->Pitch         = buffer->Width * buffer->BytesPerPixel;

	// Negative means top-down in the context of the biHeight
#	define Top_Down -
	BITMAPINFOHEADER&
	header = buffer->Info.bmiHeader;
	header.biSize        = sizeof( buffer->Info.bmiHeader );
	header.biWidth       = buffer->Width;
	header.biHeight      = Top_Down buffer->Height;
	header.biPlanes      = 1;
	header.biBitCount    = 32; // Need 24, but want 32 ( alignment )
	header.biCompression = BI_RGB_Uncompressed;
	// header.biSizeImage     = 0;
	// header.biXPelsPerMeter = 0;
	// header.biYPelsPerMeter = 0;
	// header.biClrUsed	   = 0;
	// header.biClrImportant  = 0;
#	undef Top_Down

	// We want to "touch" a pixel on every 4-byte boundary
	u32 BitmapMemorySize = (buffer->Width * buffer->Height) * buffer->BytesPerPixel;
	buffer->Memory = VirtualAlloc( NULL, BitmapMemorySize, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write );

	// TODO(Ed) : Clear to black
}

internal void
display_buffer_in_window( HDC device_context, u32 window_width, u32 window_height, OffscreenBuffer* buffer
	, u32 x, u32 y
	, u32 width, u32 height )
{
	// TODO(Ed) : Aspect ratio correction
	StretchDIBits( device_context
	#if 0
		, x, y, width, height
		, x, y, width, height
	#endif
		, 0, 0, window_width, window_height
		, 0, 0, buffer->Width, buffer->Height
		, buffer->Memory, & buffer->Info
		, DIB_ColorTable_RGB, RO_Source_To_Dest );
}

internal LRESULT CALLBACK
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

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			u32  vk_code  = w_param;
			b32 is_down  = (l_param >> 31) == 0;
			b32 was_down = (l_param >> 30);
			b32 alt_down = (l_param & (1 << 29));

			switch ( vk_code )
			{
				case 'Q':
				{
					OutputDebugStringA( "Q\n" );
				}
				break;
				case 'E':
				{
					OutputDebugStringA( "E\n" );
				}
				break;
				case 'W':
				{
					OutputDebugStringA( "W\n" );
				}
				break;
				case 'A':
				{
					OutputDebugStringA( "A\n" );
				}
				break;
				case 'S':
				{
					OutputDebugStringA( "S\n" );
				}
				break;
				case 'D':
				{
					OutputDebugStringA( "D\n" );
				}
				break;
				case VK_ESCAPE:
				{
					OutputDebugStringA( "Escape\n" );
				}
				break;
				case VK_UP:
				{
					OutputDebugStringA( "Up\n" );
				}
				break;
				case VK_DOWN:
				{
					OutputDebugStringA( "Down\n" );
				}
				break;
				case VK_LEFT:
				{
					OutputDebugStringA( "Left\n" );
				}
				break;
				case VK_RIGHT:
				{
					OutputDebugStringA( "Right\n" );
				}
				break;
				case VK_SPACE:
				{
					OutputDebugStringA( "Space\n" );
				}
				break;
				case VK_F4:
				{
					if ( alt_down )
						Running = false;
				}
				break;
			}
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

			WinDimensions dimensions = get_window_dimensions( handle );

			display_buffer_in_window( device_context, dimensions.Width, dimensions.Height, &BackBuffer
				, x, y
				, width, height );
			EndPaint( handle, & info );
		}
		break;

		case WM_SIZE:
		{
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
	// xinput_load_library_bindings();

	using JSL_DeviceHandle = int;
	u32 jsl_num_devices = JslConnectDevices();

	JSL_DeviceHandle device_handles[4] {};
	u32 jsl_getconnected_found = JslGetConnectedDeviceHandles( device_handles, jsl_num_devices );
	if ( jsl_getconnected_found != jsl_num_devices )
	{
		OutputDebugStringA( "Error: JSLGetConnectedDeviceHandles didn't find as many as were stated with JslConnectDevices\n");
	}

	if ( jsl_num_devices > 0 )
	{
		OutputDebugStringA( "JSL Connected Devices:\n" );
		for ( u32 jsl_device_index = 0; jsl_device_index < jsl_num_devices; ++ jsl_device_index )
		{
			JslSetLightColour( device_handles[ jsl_device_index ], (255 << 8) );
			do_once_start
				congrats( "GOT THE CONTROLLER BOIS" );
			do_once_end
		}
	}

	// MessageBox( 0, L"First message!", L"Handmade Hero", MB_Ok_Btn | MB_Icon_Information );

	WNDCLASS window_class {};
		window_class.style = CS_Horizontal_Redraw | CS_Vertical_Redraw;
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

			WinDimensions dimensions = get_window_dimensions( window_handle );
			resize_dib_section( &BackBuffer, 1280, 720 );

			init_sound( window_handle, 48000, 48000 * sizeof(s16) * 2 );

			MSG  msg_info;

			u32 x_offset = 0;
			u32 y_offset = 0;

			bool xinput_detected = false;

			// Controller State
			u8 dpad_up        = false;
			u8 dpad_down      = false;
			u8 dpad_left      = false;
			u8 dpad_right     = false;
			u8 start          = false;
			u8 back           = false;
			u8 left_shoulder  = false;
			u8 right_shoulder = false;
			u8 btn_a_button   = false;
			u8 btn_b_button   = false;
			u8 btn_x_button   = false;
			u8 btn_y_button   = false;
			u16 left_stick_x  = 0;
			u16 left_stick_y  = 0;
			u16 right_stick_x = 0;
			u16 right_stick_y = 0;

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
					DispatchMessageW( & msg_info );
				}

				// XInput Polling
				// TODO(Ed) : Should we poll this more frequently?
				for ( DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; ++ controller_index )
				{
					XINPUT_STATE controller_state;
					xinput_detected = xinput_get_state( controller_index, & controller_state ) == XI_PluggedIn;
					if ( xinput_detected )
					{
						XINPUT_GAMEPAD* pad = & controller_state.Gamepad;

						dpad_up        = pad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
						dpad_down      = pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
						dpad_left      = pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
						dpad_right     = pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
						start          = pad->wButtons & XINPUT_GAMEPAD_START;
						back           = pad->wButtons & XINPUT_GAMEPAD_BACK;
						left_shoulder  = pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
						right_shoulder = pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
						btn_a_button   = pad->wButtons & XINPUT_GAMEPAD_A;
						btn_b_button   = pad->wButtons & XINPUT_GAMEPAD_B;
						btn_x_button   = pad->wButtons & XINPUT_GAMEPAD_X;
						btn_y_button   = pad->wButtons & XINPUT_GAMEPAD_Y;

						left_stick_x  = pad->sThumbLX;
						left_stick_y  = pad->sThumbLY;
						right_stick_x = pad->sThumbRX;
						right_stick_y = pad->sThumbRY;
					}
					else
					{
						// NOTE: Controller is not available
					}
				}

				// JSL Input Polling
				for ( u32 jsl_device_index = 0; jsl_device_index < jsl_num_devices; ++ jsl_device_index )
				{
					if ( ! JslStillConnected( device_handles[ jsl_device_index ] ) )
					{
						OutputDebugStringA( "Error: JSLStillConnected returned false\n" );
						continue;
					}

					JOY_SHOCK_STATE state = JslGetSimpleState( device_handles[ jsl_device_index ] );
					dpad_up        = state.buttons & JSMASK_UP;
					dpad_down      = state.buttons & JSMASK_DOWN;
					dpad_left      = state.buttons & JSMASK_LEFT;
					dpad_right     = state.buttons & JSMASK_RIGHT;
					start          = state.buttons & JSMASK_PLUS;
					back           = state.buttons & JSMASK_MINUS;
					left_shoulder  = state.buttons & JSMASK_L;
					right_shoulder = state.buttons & JSMASK_R;
					btn_a_button   = state.buttons & JSMASK_S;
					btn_b_button   = state.buttons & JSMASK_E;
					btn_x_button   = state.buttons & JSMASK_W;
					btn_y_button   = state.buttons & JSMASK_N;
				}

				x_offset += dpad_right;
				x_offset -= dpad_left;
				y_offset += dpad_up;
				y_offset -= dpad_down;
				// x_offset += left_stick_x;
				// y_offset += left_stick_y;

				if ( start )
				{
					if ( xinput_detected )
					{
						XINPUT_VIBRATION vibration;
						vibration.wLeftMotorSpeed  = 30000;
						xinput_set_state( 0, & vibration );
					}
					else
					{
						JslSetRumble( 0, 1, 0 );
					}
				}
				else
				{
					if ( xinput_detected )
					{
						XINPUT_VIBRATION vibration;
						vibration.wLeftMotorSpeed  = 0;
						xinput_set_state( 0, & vibration );
					}
					else
					{
						JslSetRumble( 0, 0, 0 );
					}
				}

				render_weird_graident( &BackBuffer, x_offset, y_offset );

				WinDimensions dimensions     = get_window_dimensions( window_handle );
				HDC           device_context = GetDC( window_handle );
				display_buffer_in_window( device_context, dimensions.Width, dimensions.Height, &BackBuffer
					, 0, 0
					, dimensions.Width, dimensions.Height );
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

	if ( jsl_num_devices > 0 )
	{
		OutputDebugStringA( "JSL Connected Devices:\n" );
		for ( u32 jsl_device_index = 0; jsl_device_index < jsl_num_devices; ++ jsl_device_index )
		{
			JslSetLightColour( device_handles[ jsl_device_index ], 0 );
		}
	}

	return 0;
}
