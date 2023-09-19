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
#include "engine.cpp"


// Platform Layer headers
#include "platform.h"
#include "jsl.h" // Using this to get dualsense controllers
#include "win32.h"
#include <malloc.h>

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

// TODO : This will def need to be looked over.
struct SoundOutput
{
	DWORD IsPlaying;
	u32   RunningSampleIndex;
	s32   LatencySampleCount;
};

HRESULT WINAPI DirectSoundCreate(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );

using DirectSoundCreateFn = HRESULT WINAPI (LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );
global DirectSoundCreateFn* direct_sound_create;

global OffscreenBuffer BackBuffer;
global WinDimensions   WindowDimensions;

global LPDIRECTSOUNDBUFFER DS_SecondaryBuffer;
global s32                 DS_SecondaryBuffer_Size;
global s32                 DS_SecondaryBuffer_SamplesPerSecond;
global s32                 DS_SecondaryBuffer_BytesPerSample;

global s16* SoundBufferSamples;

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

internal void
ds_clear_sound_buffer( SoundOutput* sound_output )
{
	LPVOID region_1;
	DWORD  region_1_size;
	LPVOID region_2;
	DWORD  region_2_size;

	HRESULT ds_lock_result = DS_SecondaryBuffer->Lock( 0, DS_SecondaryBuffer_Size
		, & region_1, & region_1_size
		, & region_2, & region_2_size
		, 0 );
	if ( ! SUCCEEDED( ds_lock_result ) )
	{
		return;
	}

	u8* sample_out = rcast( u8*, region_1 );
	for ( DWORD byte_index = 0; byte_index < region_1_size; ++ byte_index )
	{
		*sample_out = 0;
		++ sample_out;
	}

	sample_out = rcast( u8*, region_2 );
	for ( DWORD byte_index = 0; byte_index < region_2_size; ++ byte_index )
	{
		*sample_out = 0;
		++ sample_out;
	}

	if ( ! SUCCEEDED( DS_SecondaryBuffer->Unlock( region_1, region_1_size, region_2, region_2_size ) ))
	{
		return;
	}
}

internal void
ds_fill_sound_buffer( SoundOutput* sound_output, DWORD byte_to_lock, DWORD bytes_to_write, engine::SoundBuffer* sound_buffer )
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

	DWORD region_1_sample_count = region_1_size / DS_SecondaryBuffer_BytesPerSample;
	s16*  sample_out            = rcast( s16*, region_1 );
	s16*  sample_in 		    = sound_buffer->Samples;
	for ( DWORD sample_index = 0; sample_index < region_1_sample_count; ++ sample_index )
	{
		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		++ sound_output->RunningSampleIndex;
	}

	DWORD region_2_sample_count = region_2_size / DS_SecondaryBuffer_BytesPerSample;
	      sample_out            = rcast( s16*, region_2 );
	for ( DWORD sample_index = 0; sample_index < region_2_sample_count; ++ sample_index )
	{
		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		++ sound_output->RunningSampleIndex;
	}

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

internal void
input_process_digital_btn( engine::DigitalBtn* old_state, engine::DigitalBtn* new_state, u32 raw_btns, u32 btn_flag )
{
#define had_transition() ( old_state->State == new_state->State )
	new_state->State           = (raw_btns & btn_flag);
	new_state->HalfTransitions = had_transition() ? 1 : 0;
#undef had_transition
}

int CALLBACK
WinMain(
	HINSTANCE instance,
	HINSTANCE prev_instance,
	LPSTR     commandline,
	int       show_command
)
{
	using namespace win32;

	// Memory
	engine::Memory engine_memory {};
	{
		engine_memory.PersistentSize = megabytes( 64 );
		// engine_memory.FrameSize	     = megabytes( 64 );
		engine_memory.TransientSize  = gigabytes( 2 );

		u64 total_size = engine_memory.PersistentSize
			// + engine_memory.FrameSize
			+ engine_memory.TransientSize;

	#if Build_Debug
		void* Base_Address      = (void*) terabytes( 1 );
		// void* Frame_Address     = (void*) terabytes( 2 );
		// void* Transient_Address = (void*) terabytes( 2 );
	#else
		void* Base_Address      = 0;
		// void* Frame_Address     = 0;
		// void* Transient_Address = 0;
	#endif

		engine_memory.Persistent = VirtualAlloc( Base_Address, total_size
			, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write );
		engine_memory.Transient = rcast( u8*, engine_memory.Persistent ) + engine_memory.PersistentSize;

	#if 0
		engine_memory.Frame = VirtualAlloc( 0, engine_memory.FrameSize
			, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write );

		engine_memory.Transient = VirtualAlloc( 0, engine_memory.TransientSize
			, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write );
	#endif

		if ( engine_memory.Persistent == nullptr
			// || ! engine_memory.Frame
			|| engine_memory.Transient == nullptr )
		{
			// TODO : Diagnostic Logging
			return -1;
		}
	}

	// MessageBox( 0, L"First message!", L"Handmade Hero", MB_Ok_Btn | MB_Icon_Information );

	WNDCLASSW window_class {};
	HWND window_handle = nullptr;
	MSG window_msg_info;
	{
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

		window_handle = CreateWindowExW(
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
	}

	WinDimensions dimensions = get_window_dimensions( window_handle );
	resize_dib_section( &BackBuffer, 1280, 720 );

	SoundOutput sound_output;
	{
		sound_output.IsPlaying              = 0;
		DS_SecondaryBuffer_SamplesPerSecond = 48000;
		DS_SecondaryBuffer_BytesPerSample   = sizeof(s16) * 2;

		DS_SecondaryBuffer_Size = DS_SecondaryBuffer_SamplesPerSecond * DS_SecondaryBuffer_BytesPerSample;
		init_sound( window_handle, DS_SecondaryBuffer_SamplesPerSecond, DS_SecondaryBuffer_Size );

		SoundBufferSamples = rcast( s16*, VirtualAlloc( 0, 48000 * 2 * sizeof(s16)
			, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write ));

		assert( SoundBufferSamples );

		sound_output.RunningSampleIndex = 0;
		sound_output.LatencySampleCount = DS_SecondaryBuffer_SamplesPerSecond / 15;
		// ds_clear_sound_buffer( & sound_output );
		DS_SecondaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
	}

	// Timing
	u64 perf_counter_frequency;
	u64 last_frame_time;
	QueryPerformanceFrequency( rcast(LARGE_INTEGER*, & perf_counter_frequency) );
	QueryPerformanceCounter( rcast(LARGE_INTEGER*, & last_frame_time) );

	u64 last_cycle_time = __rdtsc();

	// Input shitshow
	constexpr u32 Max_Controllers = 4;
	// Max controllers for the platform layer and thus for all other layers is 4. (Sanity and xinput limit)

	engine::InputState input {};

	using EngineXInputPadStates = engine::XInputPadState[ Max_Controllers ];
	EngineXInputPadStates xpad_states[2];
	EngineXInputPadStates* old_xpads = & xpad_states[0];
	EngineXInputPadStates* new_xpads = & xpad_states[1];

	using EngineDSPadStates = engine::DualsensePadState[Max_Controllers];
	EngineDSPadStates ds_pad_states[2];
	EngineDSPadStates* old_ds_pads = & ds_pad_states[0];
	EngineDSPadStates* new_ds_pads = & ds_pad_states[1];

	using JSL_DeviceHandle = int;
	u32 jsl_num_devices
			= JslConnectDevices();
			// = 0;
	JSL_DeviceHandle jsl_device_handles[4] {};
	{
		xinput_load_library_bindings();

		u32 jsl_getconnected_found = JslGetConnectedDeviceHandles( jsl_device_handles, jsl_num_devices );
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
					JslSetLightColour( jsl_device_handles[ jsl_device_index ], (255 << 8) );
				}
			}
		}

		if ( jsl_num_devices > 4 )
		{
			jsl_num_devices = 4;
			MessageBoxA( window_handle, "More than 4 JSL devices found, this engine will only support the first four found.", "Warning", MB_ICONEXCLAMATION );
		}
	}

	Running = true;
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
			// Swapping at the beginning of the input frame instead of the end.
			swap( old_xpads,   new_xpads );
			swap( old_ds_pads, new_ds_pads );

			// XInput Polling
			// TODO(Ed) : Should we poll this more frequently?
			for ( DWORD controller_index = 0; controller_index < Max_Controllers; ++ controller_index )
			{
				XINPUT_STATE controller_state;
				b32 xinput_detected = xinput_get_state( controller_index, & controller_state ) == XI_PluggedIn;
				if ( xinput_detected )
				{
					XINPUT_GAMEPAD*       xpad = & controller_state.Gamepad;
					engine::XInputPadState* old_xpad = old_xpads[ controller_index ];
					engine::XInputPadState* new_xpad = new_xpads[ controller_index ];

					input_process_digital_btn( & old_xpad->DPad.Up,    & new_xpad->DPad.Up, xpad->wButtons, XINPUT_GAMEPAD_DPAD_UP );
					input_process_digital_btn( & old_xpad->DPad.Down,  & new_xpad->DPad.Down, xpad->wButtons, XINPUT_GAMEPAD_DPAD_DOWN );
					input_process_digital_btn( & old_xpad->DPad.Left,  & new_xpad->DPad.Left, xpad->wButtons, XINPUT_GAMEPAD_DPAD_LEFT );
					input_process_digital_btn( & old_xpad->DPad.Right, & new_xpad->DPad.Right, xpad->wButtons, XINPUT_GAMEPAD_DPAD_RIGHT );

					input_process_digital_btn( & old_xpad->Y, & new_xpad->Y, xpad->wButtons, XINPUT_GAMEPAD_Y );
					input_process_digital_btn( & old_xpad->A, & new_xpad->A, xpad->wButtons, XINPUT_GAMEPAD_A );
					input_process_digital_btn( & old_xpad->B, & new_xpad->B, xpad->wButtons, XINPUT_GAMEPAD_B );
					input_process_digital_btn( & old_xpad->X, & new_xpad->X, xpad->wButtons, XINPUT_GAMEPAD_X );

					input_process_digital_btn( & old_xpad->Back,  & new_xpad->Back,  xpad->wButtons, XINPUT_GAMEPAD_BACK );
					input_process_digital_btn( & old_xpad->Start, & new_xpad->Start, xpad->wButtons, XINPUT_GAMEPAD_START );

					input_process_digital_btn( & old_xpad->LeftShoulder,  & new_xpad->LeftShoulder,  xpad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER );
					input_process_digital_btn( & old_xpad->RightShoulder, & new_xpad->RightShoulder, xpad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER );

					new_xpad->Stick.Left.X.Start = old_xpad->Stick.Left.X.End;
					new_xpad->Stick.Left.Y.Start = old_xpad->Stick.Left.Y.End;

					// TODO(Ed) : Compress this into a proc
					f32 X;
					if ( xpad->sThumbLX < 0 )
					{
						X = scast(f32, xpad->sThumbLX) / scast(f32, -S16_MIN);
					}
					else
					{
						X = scast(f32, xpad->sThumbLX) / scast(f32, S16_MAX);
					}

					// TODO(Ed) : Min/Max macros!!!
					new_xpad->Stick.Left.X.Min = new_xpad->Stick.Left.X.Max = new_xpad->Stick.Left.X.End = X;

					f32 Y;
					if ( xpad->sThumbLY < 0 )
					{
						Y = scast(f32, xpad->sThumbLY) / scast(f32, -S16_MIN);
					}
					else
					{
						Y = scast(f32, xpad->sThumbLY) / scast(f32, S16_MAX);
					}

					// TODO(Ed) : Min/Max macros!!!
					new_xpad->Stick.Left.Y.Min = new_xpad->Stick.Left.Y.Max = new_xpad->Stick.Left.Y.End = Y;



					// epad->Stick.Left.X.End  = xpad->sThumbLX;
					// epad->Stick.Left.Y.End  = xpad->sThumbLY;
					// epad->Stick.Right.X.End = xpad->sThumbRX;
					// epad->Stick.Right.X.End = xpad->sThumbRY;

					// TODO(Ed) : Dead zone processing!!!!!!!!!!!!!!!
					// XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
					// XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE

					// S16_MAX
					// S16_MIN

					input.Controllers[ controller_index ].XPad = new_xpad;
				}
				else
				{
					input.Controllers[ controller_index ].XPad = nullptr;
				}
			}

			// JSL Input Polling
			for ( u32 jsl_device_index = 0; jsl_device_index < jsl_num_devices; ++ jsl_device_index )
			{
				if ( ! JslStillConnected( jsl_device_handles[ jsl_device_index ] ) )
				{
					OutputDebugStringA( "Error: JSLStillConnected returned false\n" );
					continue;
				}

				// TODO : Won't support more than 4 for now... (or prob ever)
				if ( jsl_device_index > 4 )
					break;

				JOY_SHOCK_STATE       state = JslGetSimpleState( jsl_device_handles[ jsl_device_index ] );
				engine::DualsensePadState* old_ds_pad  = old_ds_pads[ jsl_device_index ];
				engine::DualsensePadState* new_ds_pad  = new_ds_pads[ jsl_device_index ];

				input_process_digital_btn( & old_ds_pad->DPad.Up,    & new_ds_pad->DPad.Up,    state.buttons, JSMASK_UP );
				input_process_digital_btn( & old_ds_pad->DPad.Down,  & new_ds_pad->DPad.Down,  state.buttons, JSMASK_DOWN );
				input_process_digital_btn( & old_ds_pad->DPad.Left,  & new_ds_pad->DPad.Left,  state.buttons, JSMASK_LEFT );
				input_process_digital_btn( & old_ds_pad->DPad.Right, & new_ds_pad->DPad.Right, state.buttons, JSMASK_RIGHT );

				input_process_digital_btn( & old_ds_pad->Triangle, & new_ds_pad->Triangle, state.buttons, JSMASK_N );
				input_process_digital_btn( & old_ds_pad->X,        & new_ds_pad->X,        state.buttons, JSMASK_S );
				input_process_digital_btn( & old_ds_pad->Square,   & new_ds_pad->Square,   state.buttons, JSMASK_W );
				input_process_digital_btn( & old_ds_pad->Circle,   & new_ds_pad->Circle,   state.buttons, JSMASK_E );

				input_process_digital_btn( & old_ds_pad->Share,   & new_ds_pad->Share,   state.buttons, JSMASK_SHARE );
				input_process_digital_btn( & old_ds_pad->Options, & new_ds_pad->Options, state.buttons, JSMASK_OPTIONS );

				input_process_digital_btn( & old_ds_pad->L1, & new_ds_pad->L1, state.buttons, JSMASK_L );
				input_process_digital_btn( & old_ds_pad->R1, & new_ds_pad->R1, state.buttons, JSMASK_R );

				// epad->Stick.Left.X.End  = state.stickLX;
				// epad->Stick.Left.Y.End  = state.stickLY;
				// epad->Stick.Right.X.End = state.stickRX;
				// epad->Stick.Right.X.End = state.stickRY;

				input.Controllers[ jsl_device_index ].DSPad = new_ds_pad;
			}
		}

		// Pain...
		b32 sound_is_valid = false;
		DWORD ds_play_cursor;
		DWORD ds_write_cursor;
		DWORD byte_to_lock;
		DWORD bytes_to_write;
		if ( SUCCEEDED( DS_SecondaryBuffer->GetCurrentPosition( & ds_play_cursor, & ds_write_cursor ) ))
		{

			byte_to_lock        = (sound_output.RunningSampleIndex * DS_SecondaryBuffer_BytesPerSample) % DS_SecondaryBuffer_Size;
			DWORD target_cursor = (ds_play_cursor + (sound_output.LatencySampleCount * DS_SecondaryBuffer_BytesPerSample)) % DS_SecondaryBuffer_Size;

			if ( byte_to_lock > target_cursor)
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

			sound_is_valid = true;
		}

		// s16 samples[ 48000 * 2 ];
		engine::SoundBuffer sound_buffer {};
		sound_buffer.NumSamples         = bytes_to_write / DS_SecondaryBuffer_BytesPerSample;
		sound_buffer.RunningSampleIndex = sound_output.RunningSampleIndex;
		sound_buffer.SamplesPerSecond   = DS_SecondaryBuffer_SamplesPerSecond;
		sound_buffer.Samples            = SoundBufferSamples;

		engine::update_and_render( & input, rcast(engine::OffscreenBuffer*, & BackBuffer.Memory), & sound_buffer, & engine_memory );

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
			DWORD ds_status = 0;
			if ( SUCCEEDED( DS_SecondaryBuffer->GetStatus( & ds_status ) ) )
			{
				sound_output.IsPlaying = ds_status & DSBSTATUS_PLAYING;
			}

			if ( ! sound_is_valid )
				break;

			ds_fill_sound_buffer( & sound_output, byte_to_lock, bytes_to_write, & sound_buffer );

			if ( sound_output.IsPlaying )
				break;

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

		// char ms_timing_debug[256] {};
		// wsprintfA( ms_timing_debug, "%d ms\n" "FPS: %d\n" "mega cycles: %d\n", ms_per_frame, fps, mega_cycles_elapsed );
		// OutputDebugStringA( ms_timing_debug );

		last_cycle_time = end_cycle_count;
		last_frame_time = frame_cycle_time_end;
	}

	if ( jsl_num_devices > 0 )
	{
		for ( u32 jsl_device_index = 0; jsl_device_index < jsl_num_devices; ++ jsl_device_index )
		{
			JslSetLightColour( jsl_device_handles[ jsl_device_index ], 0 );
		}
	}

	return 0;
}
