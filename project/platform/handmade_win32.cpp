/*
	TODO : This is not a final platform layer

	- Saved game locations
	- Getting a handle to our own executable file
	- Asset loading path
	- Threading (launch a thread)
	- Raw Input (support for multiple keyboards)
	- Sleep / timeBeginPeriod
	- ClipCursor() (for multimonitor support)
	- Fullscreen support
	- WM_SETCURSOR (control cursor visibility)
	- QueryCancelAutoplay
	- WM_ACTIVATEAPP (for when not active)
	- Blit speed improvemnts (BitBlt)
	- Hardware acceleration ( OpenGL or Direct3D or both )
	- GetKeyboardLayout (for French keyboards, international WASD support)
*/

#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wvarargs"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#endif

#include <math.h> // TODO : Implement math ourselves

// Platform Layer headers
#include "platform.h"
#include "jsl.h" // Using this to get dualsense controllers
#include "win32.h"

// Engine layer headers
#include "engine.h"


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

HRESULT WINAPI
DirectSoundCreate(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );

using DirectSoundCreateFn = HRESULT WINAPI (LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );
global DirectSoundCreateFn* direct_sound_create;

global OffscreenBuffer BackBuffer;
global WinDimensions   WindowDimensions;

global LPDIRECTSOUNDBUFFER DS_SecondaryBuffer;
global s32                 DS_SecondaryBuffer_Size;
global s32                 DS_SecondaryBuffer_SamplesPerSecond;
global s32                 DS_SecondaryBuffer_BytesPerSample;

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
	if ( ! ensure( direct_sound_create, "Failed to get direct_sound_create_procedure" ) )
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
	}

	DSBUFFERDESC
	buffer_description { sizeof(buffer_description) };
	buffer_description.dwFlags       = 0;
	buffer_description.dwBufferBytes = buffer_size;
	buffer_description.lpwfxFormat   = & wave_format;

	if ( ! SUCCEEDED( direct_sound->CreateSoundBuffer( & buffer_description, & DS_SecondaryBuffer, 0 ) ))
	{
		// TODO : Diagnostic
	}
	if ( ! SUCCEEDED( DS_SecondaryBuffer->SetFormat( & wave_format ) ) )
	{
		// TODO : Diagnostic
	}
}


struct DS_SoundOutputTest
{
	DWORD IsPlaying;
	u32   RunningSampleIndex;
	s32   WaveToneHz;
	s32   WavePeriod;
	s32   ToneVolume;
	s32   LatencySampleCount;
};

using DS_FillSoundBuffer_GetSampleValueFn = s16( DS_SoundOutputTest* sound_output );

internal s16
square_wave_sample_value( DS_SoundOutputTest* sound_output )
{
	s16 sample_value = (sound_output->RunningSampleIndex /  (sound_output->WavePeriod /2)) % 2 ?
		sound_output->ToneVolume : - sound_output->ToneVolume;

	return sample_value;
}

internal s16
sine_wave_sample_value( DS_SoundOutputTest* sound_output )
{
	local_persist f32 time = 0.f;

	f32 sine_value   = sinf( time );
	s16 sample_value = scast(u16, sine_value * sound_output->ToneVolume);

	time += TAU * 1.0f / scast(f32, sound_output->WavePeriod );
	return sample_value;
}

internal void
ds_fill_soundbuffer_region( LPVOID region, DWORD region_size
	, DS_SoundOutputTest* sound_output, DS_FillSoundBuffer_GetSampleValueFn* get_sample_value )
{
	DWORD region_sample_count = region_size / DS_SecondaryBuffer_BytesPerSample;
	s16*  sample_out          = rcast( s16*, region );
	for ( DWORD sample_index = 0; sample_index < region_sample_count; ++ sample_index )
	{
		s16 sample_value = get_sample_value( sound_output );
		++ sound_output->RunningSampleIndex;

		*sample_out = sample_value;
		++ sample_out;

		*sample_out = sample_value;
		++ sample_out;
	}
}

internal void
ds_fill_sound_buffer_test( DS_SoundOutputTest* sound_output, DWORD byte_to_lock, DWORD bytes_to_write, DS_FillSoundBuffer_GetSampleValueFn* get_sample_value )
{
	LPVOID region_1;
	DWORD  region_1_size;
	LPVOID region_2;
	DWORD  region_2_size;

	HRESULT ds_lock_result = DS_SecondaryBuffer->Lock( byte_to_lock, bytes_to_write
		, & region_1, & region_1_size
		, & region_2, & region_2_size
		, 0 );
	if ( ! SUCCEEDED( ds_lock_result ) )
	{
		return;
	}

	// TODO : Assert that region sizes are valid

	ds_fill_soundbuffer_region( region_1, region_1_size, sound_output, get_sample_value );
	ds_fill_soundbuffer_region( region_2, region_2_size, sound_output, get_sample_value );

	if ( ! SUCCEEDED( DS_SecondaryBuffer->Unlock( region_1, region_1_size, region_2, region_2_size ) ))
	{
		return;
	}
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
	LRESULT result = 0;

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
	{
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
			}
		}
	}

	// MessageBox( 0, L"First message!", L"Handmade Hero", MB_Ok_Btn | MB_Icon_Information );

	WNDCLASSW
	window_class {};
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

	if ( ! RegisterClassW( & window_class ) )
	{
		// TODO : Diagnostic Logging
		return 0;
	}

	HWND window_handle = CreateWindowExW(
		0,
		window_class.lpszClassName,
		L"Handmade Hero",
		WS_Overlapped_Window | WS_Initially_Visible,
		CW_Use_Default, CW_Use_Default, // x, y
		CW_Use_Default, CW_Use_Default, // width, height
		0, 0,                         // parent, menu
		instance, 0                   // instance, param
	);

	if ( ! window_handle )
	{
		// TODO : Diagnostic Logging
		return 0;
	}

	Running = true;

	WinDimensions dimensions = get_window_dimensions( window_handle );
	resize_dib_section( &BackBuffer, 1280, 720 );

	DS_SoundOutputTest sound_output;
	sound_output.IsPlaying              = 0;
	DS_SecondaryBuffer_SamplesPerSecond = 48000;
	DS_SecondaryBuffer_BytesPerSample   = sizeof(s16) * 2;

	DS_SecondaryBuffer_Size = DS_SecondaryBuffer_SamplesPerSecond * DS_SecondaryBuffer_BytesPerSample;
	init_sound( window_handle, DS_SecondaryBuffer_SamplesPerSecond, DS_SecondaryBuffer_Size );

	// Wave Sound Test
	bool wave_switch = false;
	sound_output.RunningSampleIndex = 0;
	sound_output.WaveToneHz 	    = 262;
	sound_output.WavePeriod         = DS_SecondaryBuffer_SamplesPerSecond / sound_output.WaveToneHz;
	sound_output.ToneVolume         = 3000;
	sound_output.LatencySampleCount = DS_SecondaryBuffer_SamplesPerSecond / 15;
	ds_fill_sound_buffer_test( & sound_output, 0, sound_output.LatencySampleCount * DS_SecondaryBuffer_BytesPerSample, & sine_wave_sample_value );
	DS_SecondaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );

	// Graphics & Input Test
	u32 x_offset = 0;
	u32 y_offset = 0;

	// Controller State
	bool xinput_detected = false;

	b32 dpad_up        = false;
	b32 dpad_down      = false;
	b32 dpad_left      = false;
	b32 dpad_right     = false;
	b32 start          = false;
	b32 back           = false;
	b32 left_shoulder  = false;
	b32 right_shoulder = false;
	b32 btn_a          = false;
	b32 btn_b          = false;
	b32 btn_x          = false;
	b32 btn_y          = false;
	u16 stick_left_x  = 0;
	u16 stick_left_y  = 0;
	u16 stick_right_x = 0;
	u16 stick_right_y = 0;

	// TODO : Add sine wave test

	// Windows
	MSG window_msg_info;

	u64 perf_counter_frequency;
	QueryPerformanceFrequency( rcast(LARGE_INTEGER*, & perf_counter_frequency) );

	u64 last_frame_time;
	QueryPerformanceCounter( rcast(LARGE_INTEGER*, & last_frame_time) );

	u64 last_cycle_time = __rdtsc();

	while( Running )
	{
		// Window Management
		{
			if ( PeekMessageW( & window_msg_info, 0, 0, 0, PM_Remove_Messages_From_Queue ) )
			{
				if ( window_msg_info.message == WM_QUIT  )
				{
					OutputDebugStringA("WM_QUIT\n");
					Running = false;
				}

				TranslateMessage( & window_msg_info );
				DispatchMessageW( & window_msg_info );
			}
		}

		// Input
		{
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
					btn_a          = pad->wButtons & XINPUT_GAMEPAD_A;
					btn_b          = pad->wButtons & XINPUT_GAMEPAD_B;
					btn_x          = pad->wButtons & XINPUT_GAMEPAD_X;
					btn_y          = pad->wButtons & XINPUT_GAMEPAD_Y;

					stick_left_x  = pad->sThumbLX;
					stick_left_y  = pad->sThumbLY;
					stick_right_x = pad->sThumbRX;
					stick_right_y = pad->sThumbRY;
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
				btn_a          = state.buttons & JSMASK_S;
				btn_b          = state.buttons & JSMASK_E;
				btn_x          = state.buttons & JSMASK_W;
				btn_y          = state.buttons & JSMASK_N;

				stick_left_x  = state.stickLX;
				stick_left_y  = state.stickLY;
				stick_right_x = state.stickRX;
				stick_right_y = state.stickRY;
			}

			x_offset += dpad_right;
			x_offset -= dpad_left;
			y_offset += dpad_up;
			y_offset -= dpad_down;

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
		}

		engine::update_and_render( rcast(engine::OffscreenBuffer*, & BackBuffer.Memory), x_offset, y_offset );

		// Rendering
		{
			WinDimensions dimensions     = get_window_dimensions( window_handle );
			HDC           device_context = GetDC( window_handle );
			display_buffer_in_window( device_context, dimensions.Width, dimensions.Height, &BackBuffer
				, 0, 0
				, dimensions.Width, dimensions.Height );
		}

		// Audio
		do {
			if ( btn_y )
			{
				sound_output.ToneVolume += 10;
			}
			if ( btn_b )
			{
				sound_output.ToneVolume -= 10;
			}
			if ( btn_x )
			{
				sound_output.WaveToneHz += 1;
				sound_output.WavePeriod  = DS_SecondaryBuffer_SamplesPerSecond / sound_output.WaveToneHz;
			}
			if ( btn_a )
			{
				sound_output.WaveToneHz -= 1;
				sound_output.WavePeriod  = DS_SecondaryBuffer_SamplesPerSecond / sound_output.WaveToneHz;
			}
			if ( back )
			{
				wave_switch ^= true;
			}

			DWORD ds_status = 0;
			if ( SUCCEEDED( DS_SecondaryBuffer->GetStatus( & ds_status ) ) )
			{
				sound_output.IsPlaying = ds_status & DSBSTATUS_PLAYING;
			}

			DWORD ds_play_cursor;
			DWORD ds_write_cursor;
			if ( ! SUCCEEDED( DS_SecondaryBuffer->GetCurrentPosition( & ds_play_cursor, & ds_write_cursor ) ))
			{
				break;
			}

			DWORD target_cursor = (ds_play_cursor + sound_output.LatencySampleCount * DS_SecondaryBuffer_BytesPerSample) % DS_SecondaryBuffer_Size;

			DWORD byte_to_lock   = (sound_output.RunningSampleIndex * DS_SecondaryBuffer_BytesPerSample) % DS_SecondaryBuffer_Size;
			DWORD bytes_to_write;

			if ( byte_to_lock == target_cursor )
			{
				// We are in the middle of playing. Wait for the write cursor to catch up.
				bytes_to_write = 0;
			}
			else if ( byte_to_lock > target_cursor)
			{
				// Infront of play cursor |--play--byte_to_write-->--|
				bytes_to_write =  DS_SecondaryBuffer_Size - byte_to_lock;
				bytes_to_write += target_cursor;
			}
			else
			{
				// Behind play cursor |--byte_to_write-->--play--|
				bytes_to_write = target_cursor - byte_to_lock;
			}

			if ( wave_switch )
			{
				ds_fill_sound_buffer_test( & sound_output, byte_to_lock, bytes_to_write, square_wave_sample_value );
			}
			else
			{
				ds_fill_sound_buffer_test( & sound_output, byte_to_lock, bytes_to_write, sine_wave_sample_value );
			}

		#if 1
			if ( sound_output.IsPlaying )
			{
				break;
			}
		#endif
			DS_SecondaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
		} while(0);

		u64 end_cycle_count = __rdtsc();

		u64 frame_cycle_time_end;
		QueryPerformanceCounter( rcast( LARGE_INTEGER*, & frame_cycle_time_end) );

		// TODO : Display value here

		#define MS_PER_SECOND 1000
		#define MegaCycles_Per_Second (1000 * 1000)
		u64 cycles_elapsed      = end_cycle_count - last_cycle_time;
		s32 mega_cycles_elapsed = cycles_elapsed / MegaCycles_Per_Second;
		u64 frame_time_elapsed  = frame_cycle_time_end - last_frame_time;
		u32 ms_per_frame        = MS_PER_SECOND * frame_time_elapsed / perf_counter_frequency;
		u32 fps                 = perf_counter_frequency / frame_time_elapsed;

		char ms_timing_debug[256] {};
		wsprintfA( ms_timing_debug, "%d ms\n" "FPS: %d\n" "mega cycles: %d\n", ms_per_frame, fps, mega_cycles_elapsed );
		OutputDebugStringA( ms_timing_debug );

		last_cycle_time = end_cycle_count;
		last_frame_time = frame_cycle_time_end;
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

// Engine layer translation unit.
#include "engine.cpp"
