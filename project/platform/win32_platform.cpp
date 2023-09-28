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

// Platform Layer headers
#include "platform.hpp"
#include "jsl.hpp" // Using this to get dualsense controllers
#include "win32.hpp"

// Engine layer headers
#include "engine/engine.hpp"
#include "platform_engine_api.hpp"

#if 1
// TODO(Ed): Redo these macros properly later.

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
#endif

NS_PLATFORM_BEGIN
using namespace win32;

// This is the "backbuffer" data related to the windowing surface provided by the operating system.
struct OffscreenBuffer
{
	BITMAPINFO Info;
	char       _PAD_[4];
	void*      Memory; // Lets use directly mess with the "pixel's memory buffer"
	s32        Width;
	s32        Height;
	s32        Pitch;
	s32        BytesPerPixel;
};

struct WinDimensions
{
	u32 Width;
	u32 Height;
};

// TODO : This will def need to be looked over.
struct DirectSoundBuffer
{
	LPDIRECTSOUNDBUFFER SecondaryBuffer;
	s16*                Samples;
	u32                 SecondaryBufferSize;
	u32                 SamplesPerSecond;
	u32                 BytesPerSample;

	// TODO(Ed) : Makes math easier...
	u32                 BytesPerSecond;
	u32                 GuardSampleBytes;

	DWORD               IsPlaying;
	u32                 RunningSampleIndex;

	// TODO(Ed) : Should this be in bytes?
	u32                 LatencySampleCount;
};

#pragma region Static Data
global StrFixed< S16_MAX > Path_Root;
global StrFixed< S16_MAX > Path_Binaries;

// TODO(Ed) : This is a global for now.
global b32 Running         = false;
global b32 Pause_Rendering = false;

global WinDimensions   Window_Dimensions;
global OffscreenBuffer Surface_Back_Buffer;

using DirectSoundCreateFn = HRESULT WINAPI (LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );
global DirectSoundCreateFn* direct_sound_create;

constexpr u64 Tick_To_Millisecond = 1000;
constexpr u64 Tick_To_Microsecond = 1000 * 1000;

global u64 Performance_Counter_Frequency;

// As of 2023 the highest refreshrate on the market is 500 hz. I'll just make this higher if something comes out beyond that...
constexpr u32 Monitor_Refresh_Max_Supported = 500;

// Anything at or below the high performance frame-time is too low latency to sleep against the window's scheduler.
constexpr f32 High_Perf_Frametime_MS = 1000.f / 240.f;

global u32 Monitor_Refresh_Hz     = 60;
global u32 Engine_Refresh_Hz      = Monitor_Refresh_Hz;
global f32 Engine_Frame_Target_MS = 1000.f / scast(f32, Engine_Refresh_Hz);
#pragma endregion Static Data

#pragma region Internal
internal
FILETIME file_get_last_write_time( char const* path )
{
	WIN32_FIND_DATAA dll_file_info = {};
	HANDLE dll_file_handle = FindFirstFileA( path, & dll_file_info );
	if ( dll_file_handle == INVALID_HANDLE_VALUE )
	{
		FindClose( dll_file_handle );
	}

	return dll_file_info.ftLastWriteTime;
}

#if Build_Debug
struct DebugTimeMarker
{
	DWORD OutputPlayCusror;
	DWORD OutputWriteCursor;
	DWORD OutputLocation;
	DWORD OutputByteCount;

	DWORD FlipPlayCursor;
	DWORD FlipWriteCursor;

	DWORD ExpectedFlipCursor;
};

internal void
debug_draw_vertical( s32 x_pos, s32 top, s32 bottom, s32 color )
{
	if ( top <= 0 )
	{
		top = 0;
	}

	if ( bottom > Surface_Back_Buffer.Height )
	{
		bottom = Surface_Back_Buffer.Height;
	}

	if ( x_pos >= 0 && x_pos < Surface_Back_Buffer.Width )
	{
		u8*
		pixel_byte  = rcast(u8*, Surface_Back_Buffer.Memory);
		pixel_byte += x_pos * Surface_Back_Buffer.BytesPerPixel;
		pixel_byte += top   * Surface_Back_Buffer.Pitch;

		for ( s32 y = top; y < bottom; ++ y )
		{
			s32* pixel = rcast(s32*, pixel_byte);
			*pixel = color;

			pixel_byte += Surface_Back_Buffer.Pitch;
		}
	}
}

inline void
debug_draw_sound_buffer_marker( DirectSoundBuffer* sound_buffer, f32 ratio
	, u32 pad_x, u32 pad_y
	, u32 top, u32 bottom
	, DWORD value, u32 color )
{
	// assert( value < sound_buffer->SecondaryBufferSize );
	u32 x = pad_x + scast(u32, ratio * scast(f32, value ));
	debug_draw_vertical( x, top, bottom, color );
}

internal void
debug_sync_display( DirectSoundBuffer* sound_buffer
                   , u32 num_markers, DebugTimeMarker* markers
				   , u32 current_marker
                   , f32 ms_per_frame )
{
	u32 pad_x         = 32;
	u32 pad_y         = 16;
	f32 buffers_ratio = scast(f32, Surface_Back_Buffer.Width) / (scast(f32, sound_buffer->SecondaryBufferSize) * 1);

	u32 line_height = 64;
	for ( u32 marker_index = 0; marker_index < num_markers; ++ marker_index )
	{
		DebugTimeMarker* marker = & markers[marker_index];
		assert( marker->OutputPlayCusror  < sound_buffer->SecondaryBufferSize );
		assert( marker->OutputWriteCursor < sound_buffer->SecondaryBufferSize );
		assert( marker->OutputLocation    < sound_buffer->SecondaryBufferSize );
		assert( marker->OutputByteCount   < sound_buffer->SecondaryBufferSize );
		assert( marker->FlipPlayCursor    < sound_buffer->SecondaryBufferSize );
		assert( marker->FlipWriteCursor   < sound_buffer->SecondaryBufferSize );

		DWORD play_color          = 0x88888888;
		DWORD write_color         = 0x88800000;
		DWORD expected_flip_color = 0xFFFFF000;
		DWORD play_window_color   = 0xFFFF00FF;

		u32 top    = pad_y;
		u32 bottom = pad_y + line_height;
		if ( marker_index == current_marker )
		{
			play_color  = 0xFFFFFFFF;
			write_color = 0xFFFF0000;

			top    += pad_y + line_height;
			bottom += pad_y + line_height;

			u32 row_2_top = top;

			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->OutputPlayCusror,  play_color );
			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->OutputWriteCursor, write_color );

			play_color  = 0xFFFFFFFF;
			write_color = 0xFFFF0000;

			top    += pad_y + line_height;
			bottom += pad_y + line_height;

			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->OutputLocation, play_color );
			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->OutputLocation + marker->OutputByteCount, write_color );

			play_color  = 0xFFFFFFFF;
			write_color = 0xFFFF0000;

			top    += pad_y + line_height;
			bottom += pad_y + line_height;

			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, row_2_top, bottom, marker->ExpectedFlipCursor, expected_flip_color );
		}

		DWORD play_window = marker->FlipPlayCursor + 480 * sound_buffer->BytesPerSample;

		debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->FlipPlayCursor,  play_color );
		debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, play_window,             play_window_color );
		debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->FlipWriteCursor, write_color );
	}
}
#endif

#pragma region Direct Sound
internal void
init_sound(HWND window_handle, DirectSoundBuffer* sound_buffer )
{
	// Load library
	HMODULE sound_library = LoadLibraryA( "dsound.dll" );
	if ( ! ensure(sound_library, "Failed to load direct sound library" ) )
	{
		// TOOD : Diagnostic
		return;
	}

	// Get direct sound object
	direct_sound_create = get_procedure_from_library< DirectSoundCreateFn >( sound_library, "DirectSoundCreate" );
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
	wave_format.nSamplesPerSec  = scast(u32, sound_buffer->SamplesPerSecond);  /* sample rate */
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
	buffer_description.dwFlags       = DSBCAPS_GETCURRENTPOSITION2;
	buffer_description.dwBufferBytes = sound_buffer->SecondaryBufferSize;
	buffer_description.lpwfxFormat   = & wave_format;

	if ( ! SUCCEEDED( direct_sound->CreateSoundBuffer( & buffer_description, & sound_buffer->SecondaryBuffer, 0 ) ))
	{
		// TODO : Diagnostic
	}
	if ( ! SUCCEEDED( sound_buffer->SecondaryBuffer->SetFormat( & wave_format ) ) )
	{
		// TODO : Diagnostic
	}
}

internal void
ds_clear_sound_buffer( DirectSoundBuffer* sound_buffer )
{
	LPVOID region_1;
	DWORD  region_1_size;
	LPVOID region_2;
	DWORD  region_2_size;

	HRESULT ds_lock_result = sound_buffer->SecondaryBuffer->Lock( 0, sound_buffer->SecondaryBufferSize
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

	if ( ! SUCCEEDED( sound_buffer->SecondaryBuffer->Unlock( region_1, region_1_size, region_2, region_2_size ) ))
	{
		return;
	}
}

internal void
ds_fill_sound_buffer( DirectSoundBuffer* sound_buffer, DWORD byte_to_lock, DWORD bytes_to_write )
{
	LPVOID region_1;
	DWORD  region_1_size;
	LPVOID region_2;
	DWORD  region_2_size;

	HRESULT ds_lock_result = sound_buffer->SecondaryBuffer->Lock( byte_to_lock, bytes_to_write
		, & region_1, & region_1_size
		, & region_2, & region_2_size
		, 0 );
	if ( ! SUCCEEDED( ds_lock_result ) )
	{
		return;
	}

	// TODO : Assert that region sizes are valid

	DWORD region_1_sample_count = region_1_size / sound_buffer->BytesPerSample;
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

		++ sound_buffer->RunningSampleIndex;
	}

	DWORD region_2_sample_count = region_2_size / sound_buffer->BytesPerSample;
	      sample_out            = rcast( s16*, region_2 );
	for ( DWORD sample_index = 0; sample_index < region_2_sample_count; ++ sample_index )
	{
		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		++ sound_buffer->RunningSampleIndex;
	}

	if ( ! SUCCEEDED( sound_buffer->SecondaryBuffer->Unlock( region_1, region_1_size, region_2, region_2_size ) ))
	{
		return;
	}
}
#pragma endregion Direct Sound

#pragma region Input

// Max controllers for the platform layer and thus for all other layers is 4. (Sanity and xinput limit)
constexpr u32 Max_Controllers = 4;

using JSL_DeviceHandle = int;
using EngineXInputPadStates = engine::XInputPadState[ Max_Controllers ];
using EngineDSPadStates = engine::DualsensePadState[Max_Controllers];

internal void
input_process_digital_btn( engine::DigitalBtn* old_state, engine::DigitalBtn* new_state, u32 raw_btns, u32 btn_flag )
{
#define had_transition() ( old_state->EndedDown != new_state->EndedDown )
	new_state->EndedDown        = (raw_btns & btn_flag) > 0;
	if ( had_transition() )
		new_state->HalfTransitions += 1;
	else
		new_state->HalfTransitions = 0;
#undef had_transition
}

internal f32
jsl_input_process_axis_value( f32 value, f32 deadzone_threshold )
{
	f32 result = 0;
	if ( value < -deadzone_threshold  )
	{
		result = (value + deadzone_threshold ) / (1.0f - deadzone_threshold );

		if (result < -1.0f)
			result = -1.0f; // Clamp to ensure it doesn't go below -1
	}
	else if ( value > deadzone_threshold )
	{
		result = (value - deadzone_threshold ) / (1.0f - deadzone_threshold );

		if (result > 1.0f)
			result = 1.0f; // Clamp to ensure it doesn't exceed 1
	}
	return result;
}

internal f32
xinput_process_axis_value( s16 value, s16 deadzone_threshold )
{
	f32 result = 0;
	if ( value < -deadzone_threshold )
	{
		result = scast(f32, value + deadzone_threshold) / (32768.0f - scast(f32, deadzone_threshold));
	}
	else if ( value > deadzone_threshold )
	{
		result = scast(f32, value + deadzone_threshold) / (32767.0f - scast(f32, deadzone_threshold));
	}
	return result;
}

internal void
poll_input( engine::InputState* input, u32 jsl_num_devices, JSL_DeviceHandle* jsl_device_handles
	, engine::KeyboardState* old_keyboard, engine::KeyboardState* new_keyboard
	, EngineXInputPadStates* old_xpads, EngineXInputPadStates* new_xpads
	, EngineDSPadStates* old_ds_pads, EngineDSPadStates* new_ds_pads )
{
	// TODO(Ed) : Setup user definable deadzones for triggers and sticks.

	// Swapping at the beginning of the input frame instead of the end.
	swap( old_keyboard, new_keyboard );
	swap( old_xpads,    new_xpads );
	swap( old_ds_pads,  new_ds_pads );

	// Keyboard Polling
	// Keyboards are unified for now.
	{
		constexpr u32 is_down = 0x80000000;
		input_process_digital_btn( & old_keyboard->Q,         & new_keyboard->Q,         GetAsyncKeyState( 'Q' ),       is_down );
		input_process_digital_btn( & old_keyboard->E,         & new_keyboard->E,         GetAsyncKeyState( 'E' ),       is_down );
		input_process_digital_btn( & old_keyboard->W,         & new_keyboard->W,         GetAsyncKeyState( 'W' ),       is_down );
		input_process_digital_btn( & old_keyboard->A,         & new_keyboard->A,         GetAsyncKeyState( 'A' ),       is_down );
		input_process_digital_btn( & old_keyboard->S,         & new_keyboard->S,         GetAsyncKeyState( 'S' ),       is_down );
		input_process_digital_btn( & old_keyboard->D,         & new_keyboard->D,         GetAsyncKeyState( 'D' ),       is_down );
		input_process_digital_btn( & old_keyboard->L,         & new_keyboard->L,         GetAsyncKeyState( 'L' ),       is_down );
		input_process_digital_btn( & old_keyboard->Escape,    & new_keyboard->Escape,    GetAsyncKeyState( VK_ESCAPE ), is_down );
		input_process_digital_btn( & old_keyboard->Backspace, & new_keyboard->Backspace, GetAsyncKeyState( VK_BACK ),   is_down );
		input_process_digital_btn( & old_keyboard->Up,        & new_keyboard->Up,        GetAsyncKeyState( VK_UP ),     is_down );
		input_process_digital_btn( & old_keyboard->Down,      & new_keyboard->Down,      GetAsyncKeyState( VK_DOWN ),   is_down );
		input_process_digital_btn( & old_keyboard->Left,      & new_keyboard->Left,      GetAsyncKeyState( VK_LEFT ),   is_down );
		input_process_digital_btn( & old_keyboard->Right,     & new_keyboard->Right,     GetAsyncKeyState( VK_RIGHT ),  is_down );
		input_process_digital_btn( & old_keyboard->Space,     & new_keyboard->Space,     GetAsyncKeyState( VK_SPACE ),  is_down );
		input_process_digital_btn( & old_keyboard->Pause,     & new_keyboard->Pause,     GetAsyncKeyState( VK_PAUSE ),  is_down );

		input->Controllers[0].Keyboard = new_keyboard;
	}

	// XInput Polling
	// TODO(Ed) : Should we poll this more frequently?
	for ( DWORD controller_index = 0; controller_index < Max_Controllers; ++ controller_index )
	{
		XINPUT_STATE controller_state;
		b32 xinput_detected = xinput_get_state( controller_index, & controller_state ) == XI_PluggedIn;
		if ( xinput_detected )
		{
			XINPUT_GAMEPAD*         xpad     = & controller_state.Gamepad;
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

			f32 left_x = xinput_process_axis_value( xpad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
			f32 left_y = xinput_process_axis_value( xpad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );

			// TODO(Ed) : Min/Max macros!!!
			new_xpad->Stick.Left.X.Min = new_xpad->Stick.Left.X.Max = new_xpad->Stick.Left.X.End = left_x;
			new_xpad->Stick.Left.Y.Min = new_xpad->Stick.Left.Y.Max = new_xpad->Stick.Left.Y.End = left_y;

			// TODO(Ed): Make this actually an average for later
			new_xpad->Stick.Left.X.Average = left_x;
			new_xpad->Stick.Left.Y.Average = left_y;

			input->Controllers[ controller_index ].XPad = new_xpad;
		}
		else
		{
			input->Controllers[ controller_index ].XPad = nullptr;
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

		JOY_SHOCK_STATE state = JslGetSimpleState( jsl_device_handles[ jsl_device_index ] );

		// For now we're assuming anything that is detected via JSL is a dualsense pad.
		// We'll eventually add support possibly for the nintendo pro controller.
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

		new_ds_pad->Stick.Left.X.Start = old_ds_pad->Stick.Left.X.End;
		new_ds_pad->Stick.Left.Y.Start = old_ds_pad->Stick.Left.Y.End;

		// Joyshock abstracts the sticks to a float value already for us of -1.f to 1.f.
		// We'll assume a deadzone of 10% for now.
		f32 left_x = jsl_input_process_axis_value( state.stickLX, 0.1f );
		f32 left_y = jsl_input_process_axis_value( state.stickLY, 0.1f );

		new_ds_pad->Stick.Left.X.Min = new_ds_pad->Stick.Left.X.Max = new_ds_pad->Stick.Left.X.End = left_x;
		new_ds_pad->Stick.Left.Y.Min = new_ds_pad->Stick.Left.Y.Max = new_ds_pad->Stick.Left.Y.End = left_y;

		// TODO(Ed): Make this actually an average for later
		new_ds_pad->Stick.Left.X.Average = left_x;
		new_ds_pad->Stick.Left.Y.Average = left_y;

		input->Controllers[ jsl_device_index ].DSPad = new_ds_pad;
	}
}
#pragma endregion Input

#pragma region Timing
inline f32
timing_get_ms_elapsed( u64 start, u64 end )
{
	u64 delta  = (end - start) * Tick_To_Millisecond;
	f32 result = scast(f32, delta) / scast(f32, Performance_Counter_Frequency);
	return result;
}

inline f32
timing_get_seconds_elapsed( u64 start, u64 end )
{
	u64 delta = end - start;
	f32 result = scast(f32, delta) / scast(f32, Performance_Counter_Frequency);
	return result;
}

inline f32
timing_get_us_elapsed( u64 start, u64 end )
{
	u64 delta = (end - start) * Tick_To_Microsecond;
	f32 result = scast(f32, delta) / scast(f32, Performance_Counter_Frequency);
	return result;
}

inline u64
timing_get_wall_clock()
{
	u64 clock;
	QueryPerformanceCounter( rcast( LARGE_INTEGER*, & clock) );
	return clock;
}
#pragma endregion Timing

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

internal LRESULT CALLBACK
main_window_callback( HWND handle
	, UINT   system_messages
	, WPARAM w_param
	, LPARAM l_param )
{
	LRESULT result = 0;

	switch ( system_messages )
	{
		case WM_ACTIVATEAPP:
		{
			if ( scast( bool, w_param ) == true )
			{
				SetLayeredWindowAttributes( handle, RGB(0, 0, 0), 255, LWA_Alpha );
			}
			else
			{
				SetLayeredWindowAttributes( handle, RGB(0, 0, 0), 100, LWA_Alpha );
			}
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

			WinDimensions dimensions = get_window_dimensions( handle );

			display_buffer_in_window( device_context, dimensions.Width, dimensions.Height, &Surface_Back_Buffer
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

internal void
process_pending_window_messages( engine::KeyboardState* keyboard )
{
	MSG window_msg_info;
	while ( PeekMessageA( & window_msg_info, 0, 0, 0, PM_Remove_Messages_From_Queue ) )
	{
		if ( window_msg_info.message == WM_QUIT  )
		{
			OutputDebugStringA("WM_QUIT\n");
			Running = false;
		}

		// Keyboard input handling
		switch (window_msg_info.message)
		{
			// I rather do this with GetAsyncKeyState...
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			{
				WPARAM vk_code  = window_msg_info.wParam;
				b32    is_down  = scast(b32, (window_msg_info.lParam >> 31) == 0 );
				b32    was_down = scast(b32, (window_msg_info.lParam >> 30) );
				b32    alt_down = scast(b32, (window_msg_info.lParam & (1 << 29)) );

				switch ( vk_code )
				{
					case VK_F4:
					{
						if ( alt_down )
							Running = false;
					}
					break;
				}
			}
			break;

			default:
				TranslateMessage( & window_msg_info );
				DispatchMessageW( & window_msg_info );
		}
	}
}

#pragma endregion Internal

#pragma region Platfom API
#if Build_Development
b32 file_check_exists( Str path )
{
	HANDLE file_handle = CreateFileA( path
		, GENERIC_READ, FILE_SHARE_READ, 0
		, OPEN_EXISTING, 0, 0
	);
	if ( file_handle != INVALID_HANDLE_VALUE )
	{
		CloseHandle( file_handle );
		return true;
	}
	return false;
}

void file_close( File* file )
{
	HANDLE handle = pcast(HANDLE, file->OpaqueHandle);

	if ( handle == INVALID_HANDLE_VALUE )
		return;

	CloseHandle( handle );

	if ( file->Data )
	{
		// TODO(Ed): This should use our persistent memory block.
		VirtualFree( file->Data, 0, MEM_Release);
	}
	*file = {};
}

b32 file_delete( Str path )
{
	return DeleteFileA( path );
}

b32 file_read_stream( File* file, u32 content_size, void* content_memory )
{
	HANDLE file_handle;
	if ( file->OpaqueHandle == nullptr )
	{
		file_handle = CreateFileA( file->Path
			, GENERIC_READ, FILE_SHARE_READ, 0
			, OPEN_EXISTING, 0, 0
		);
		if ( file_handle == INVALID_HANDLE_VALUE )
		{
			// TODO : Logging
			return {};
		}

		file->OpaqueHandle = file_handle;
	}
	else
	{
		file_handle = pcast(HANDLE, file->OpaqueHandle);
	}

	u32 bytes_read;
	if ( ReadFile( file_handle, content_memory, content_size, rcast(LPDWORD, &bytes_read), 0 ) == false )
	{
		// TODO : Logging
		return {};
	}

	if ( bytes_read != content_size )
	{
		// TODO : Logging
		return {};
	}
	return bytes_read;
}

b32 file_read_content( File* file )
{
	HANDLE file_handle = CreateFileA( file->Path
		, GENERIC_READ, FILE_SHARE_READ, 0
		, OPEN_EXISTING, 0, 0
	);
	if ( file_handle == INVALID_HANDLE_VALUE )
	{
		// TODO(Ed) : Logging
		return {};
	}

	u32 size;
	GetFileSizeEx( file_handle, rcast(LARGE_INTEGER*, &size) );
	if ( size == 0 )
	{
		// TODO(Ed) : Logging
		CloseHandle( file_handle );
		return {};
	}

	// TODO(Ed) : This should use our memory block.
	file->Data = rcast(HANDLE*, VirtualAlloc( 0, sizeof(HANDLE) + size, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write ));
	file->Size = size;
	file->OpaqueHandle = file_handle;

	u32 bytes_read;
	if ( ReadFile( file_handle, file->Data, file->Size, rcast(LPDWORD, &bytes_read), 0 ) == false )
	{
		// TODO(Ed) : Logging
		CloseHandle( file_handle );
		return {};
	}

	if ( bytes_read != file->Size )
	{
		// TODO : Logging
		CloseHandle( file_handle );
		return {};
	}
	return bytes_read;
}

void file_rewind( File* file )
{
	HANDLE file_handle = pcast(HANDLE, file->OpaqueHandle);
	if ( file_handle == INVALID_HANDLE_VALUE )
		return;

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);
}

u32 file_write_stream( File* file, u32 content_size, void* content_memory )
{
	HANDLE file_handle;
	if ( file->OpaqueHandle == nullptr )
	{
		file_handle = CreateFileA( file->Path
			,GENERIC_WRITE, 0, 0
			, OPEN_ALWAYS, 0, 0
		);
		if ( file_handle == INVALID_HANDLE_VALUE )
		{
			// TODO(Ed) : Logging
			return {};
		}

		file->OpaqueHandle = file_handle;
	}
	else
	{
		file_handle = pcast(HANDLE, file->OpaqueHandle);
	}

	DWORD bytes_written;
	if ( WriteFile( file_handle, content_memory, content_size, & bytes_written, 0 ) == false )
	{
		// TODO : Logging
		return false;
	}

	return bytes_written;
}

u32 file_write_content( File* file, u32 content_size, void* content_memory )
{
	HANDLE file_handle = CreateFileA( file->Path
		, GENERIC_WRITE, 0, 0
		, CREATE_ALWAYS, 0, 0
	);
	if ( file_handle == INVALID_HANDLE_VALUE )
	{
		// TODO : Logging
		return false;
	}
	file->OpaqueHandle = file_handle;

	DWORD bytes_written;
	if ( WriteFile( file_handle, content_memory, content_size, & bytes_written, 0 ) == false )
	{
		// TODO : Logging
		return false;
	}
	return bytes_written;
}

void debug_set_pause_rendering( b32 value )
{
	Pause_Rendering = value;
}
#endif

u32 get_monitor_refresh_rate()
{
	return 0;
}
void set_monitor_refresh_rate( u32 refresh_rate )
{
}
u32 get_engine_refresh_rate()
{
	return 0;
}
void set_engine_refresh_rate( u32 refresh_rate )
{

}

BinaryModule load_binary_module( char const* module_path )
{
	HMODULE lib = LoadLibraryA( module_path );
	return BinaryModule { scast(void*, lib) };
}

void unload_binary_module( BinaryModule* module )
{
	FreeLibrary( scast(HMODULE, module->OpaqueHandle) );
	*module = {};
}

void* get_binary_module_symbol( BinaryModule module, char const* symbol_name )
{
	return rcast(void*, GetProcAddress( scast(HMODULE, module.OpaqueHandle), symbol_name ));
}
#pragma endregion Platform API

#pragma region Engine Module API

constexpr Str FName_Engine_DLL       = str_ascii("handmade_engine.dll");
constexpr Str FName_Engine_DLL_InUse = str_ascii("handmade_engine_in_use.dll");
constexpr Str FName_Engine_PDB_Lock  = str_ascii("handmade_engine.pdb.lock");

global HMODULE             Lib_Handmade_Engine = nullptr;
global StrFixed< S16_MAX > Path_Engine_DLL;
global StrFixed< S16_MAX > Path_Engine_DLL_InUse;

internal
engine::ModuleAPI load_engine_module_api()
{
	using ModuleAPI = engine::ModuleAPI;

	CopyFileA( Path_Engine_DLL, Path_Engine_DLL_InUse, FALSE );

	// Engine
	Lib_Handmade_Engine = LoadLibraryA( Path_Engine_DLL_InUse );
	if ( ! Lib_Handmade_Engine )
	{
		return {};
	}

	constexpr Str fname_handmade_engine_symbols = str_ascii("handmade_engine.symbols");

	StrFixed< S16_MAX > path_handmade_engine_symbols { 0, {} };
	path_handmade_engine_symbols.concat( Path_Binaries, fname_handmade_engine_symbols );

	File symbol_table {};
	symbol_table.Path = path_handmade_engine_symbols;
	if ( ! file_read_content( & symbol_table ) )
	{
		fatal( "Failed to load symbol table for handmade engine module!" );
		return {};
	}

	// TODO(Ed) : Clean this up later when Casey makes strings. (If he doesn't we'll do it)
	char symbol_on_module_reload[256];
	char symboL_startup[256];
	char symboL_shutdown[256];
	char symboL_update_and_render[256];
	char symbol_update_audio[256];
	get_symbol_from_module_table( symbol_table, ModuleAPI::Sym_OnModuleReload,  symbol_on_module_reload );
	get_symbol_from_module_table( symbol_table, ModuleAPI::Sym_Startup,         symboL_startup );
	get_symbol_from_module_table( symbol_table, ModuleAPI::Sym_Shutdown,        symboL_shutdown );
	get_symbol_from_module_table( symbol_table, ModuleAPI::Sym_UpdateAndRender, symboL_update_and_render );
	get_symbol_from_module_table( symbol_table, ModuleAPI::Sym_UpdateAudio,     symbol_update_audio );

	file_close( & symbol_table );

	engine::ModuleAPI engine_api {};
	engine_api.on_module_reload  = get_procedure_from_library< engine::OnModuleRelaodFn > ( Lib_Handmade_Engine, symbol_on_module_reload );
	engine_api.startup           = get_procedure_from_library< engine::StartupFn >        ( Lib_Handmade_Engine, symboL_startup );
	engine_api.shutdown          = get_procedure_from_library< engine::ShutdownFn >       ( Lib_Handmade_Engine, symboL_shutdown );
	engine_api.update_and_render = get_procedure_from_library< engine::UpdateAndRenderFn >( Lib_Handmade_Engine, symboL_update_and_render );
	engine_api.update_audio      = get_procedure_from_library< engine::UpdateAudioFn >    ( Lib_Handmade_Engine, symbol_update_audio );

	engine_api.IsValid =
			engine_api.on_module_reload
		&&	engine_api.startup
		&&	engine_api.shutdown
		&&	engine_api.update_and_render
		&&	engine_api.update_audio;
	if ( engine_api.IsValid )
	{
		OutputDebugStringA( "Loaded engine module API\n" );
	}
	return engine_api;
}

internal
void unload_engine_module_api( engine::ModuleAPI* engine_api )
{
	if ( engine_api->IsValid )
	{
		FreeLibrary( Lib_Handmade_Engine );
		*engine_api = {};
		OutputDebugStringA( "Unloaded engine module API\n" );
	}
}
#pragma endregion Engine Module API

NS_PLATFORM_END

int CALLBACK
WinMain( HINSTANCE instance, HINSTANCE prev_instance, LPSTR commandline, int show_command )
{
	using namespace win32;
	using namespace platform;

#pragma region Startup
	// Timing
#if Build_Development
	u64 launch_clock = timing_get_wall_clock();
	u64 launch_cycle = __rdtsc();
#endif

	// Sets the windows scheduler granulaity for this process to 1 ms
	constexpr u32 desired_scheduler_ms = 1;
	b32 sleep_is_granular = ( timeBeginPeriod( desired_scheduler_ms ) == TIMERR_NOERROR );

	// If its a high-perofmrance frame-time (a refresh rate that produces a target frametime at or below 4.16~ ms, we cannot allow the scheduler to mess things up)
	b32 sub_ms_granularity_required = scast(f32, Engine_Refresh_Hz) <= High_Perf_Frametime_MS;

	QueryPerformanceFrequency( rcast(LARGE_INTEGER*, & Performance_Counter_Frequency) );

	// Memory
	engine::Memory engine_memory {};
	{
		engine_memory.PersistentSize = megabytes( 128 );
		// engine_memory.FrameSize	     = megabytes( 64 );
		engine_memory.TransientSize  = gigabytes( 2 );

		u64 total_size = engine_memory.PersistentSize
			// + engine_memory.FrameSize
			+ engine_memory.TransientSize;

	#if Build_Debug
		void* base_address = rcast(void*, terabytes( 1 ));
	#else
		void* base_address = 0;
	#endif

		engine_memory.Persistent = VirtualAlloc( base_address, total_size , MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write );
		engine_memory.Transient = rcast( u8*, engine_memory.Persistent ) + engine_memory.PersistentSize;

		if (	engine_memory.Persistent == nullptr
			||	engine_memory.Transient  == nullptr )
		{
			// TODO : Diagnostic Logging
			return -1;
		}
	}

	WNDCLASSW window_class {};
	HWND window_handle = nullptr;
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
			WS_EX_LAYERED | WS_EX_TOPMOST,
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
	// WinDimensions dimensions = get_window_dimensions( window_handle );
	resize_dib_section( &Surface_Back_Buffer, 1280, 720 );

	// Setup pathing
	StrFixed< S16_MAX > path_pdb_lock {};
	{
		// TODO(Ed): This will not support long paths, NEEDS to be changed to support long paths.

		char path_buffer[S16_MAX];
		GetModuleFileNameA( 0, path_buffer, sizeof(path_buffer) );

		if ( GetCurrentDirectoryA( S16_MAX, Path_Binaries ) == 0 )
		{
			fatal( "Failed to get the root directory!" );
		}
		Path_Binaries.Len = str_length( Path_Binaries );
		Path_Binaries[ Path_Binaries.Len ] = '\\';
		++ Path_Binaries.Len;

		if ( SetCurrentDirectoryA( ".." ) == 0 )
		{
			fatal( "Failed to set current directory to root!");
		}

		if ( GetCurrentDirectoryA( S16_MAX, Path_Root.Data ) == 0 )
		{
			fatal( "Failed to get the root directory!" );
		}
		Path_Root.Len = str_length(Path_Root.Data);
		Path_Root.Data[ Path_Root.Len ] = '\\';
		++ Path_Root.Len;

		Path_Engine_DLL.      concat( Path_Binaries, FName_Engine_DLL );
		Path_Engine_DLL_InUse.concat( Path_Binaries, FName_Engine_DLL_InUse );

		path_pdb_lock.concat( Path_Binaries, FName_Engine_PDB_Lock );
	}

	// Prepare platform API
	ModuleAPI platform_api {};
	{
	#if Build_Development
		platform_api.debug_set_pause_rendering = & debug_set_pause_rendering;
	#endif
		// Not implemented yet
		platform_api.get_monitor_refresh_rate = nullptr;
		platform_api.set_monitor_refresh_rate = nullptr;
		platform_api.get_engine_frame_target  = nullptr;
		platform_api.set_engine_frame_target  = nullptr;

		platform_api.load_binary_module	  = & load_binary_module;
		platform_api.unload_binary_module = & unload_binary_module;
		platform_api.get_module_procedure = & get_binary_module_symbol;

		platform_api.file_check_exists  = & file_check_exists;
		platform_api.file_close         = & file_close;
		platform_api.file_delete        = & file_delete;
		platform_api.file_read_content  = & file_read_content;
		platform_api.file_read_stream   = & file_read_stream;
		platform_api.file_rewind        = & file_rewind;
		platform_api.file_write_content = & file_write_content;
		platform_api.file_write_stream  = & file_write_stream;
	}

	// Load engine module
	FILETIME          engine_api_load_time = file_get_last_write_time( Path_Engine_DLL );
	engine::ModuleAPI engine_api           = load_engine_module_api();

	b32   sound_is_valid       = false;
	DWORD ds_cursor_byte_delta = 0;
	f32   ds_latency_ms        = 0;
	DirectSoundBuffer ds_sound_buffer;
	{
		ds_sound_buffer.IsPlaying        = 0;
		ds_sound_buffer.SamplesPerSecond = 48000;
		ds_sound_buffer.BytesPerSample   = sizeof(s16) * 2;

		ds_sound_buffer.SecondaryBufferSize = ds_sound_buffer.SamplesPerSecond * ds_sound_buffer.BytesPerSample;
		init_sound( window_handle, & ds_sound_buffer );

		ds_sound_buffer.Samples = rcast( s16*, VirtualAlloc( 0, 48000 * 2 * sizeof(s16)
			, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write ));

		assert( ds_sound_buffer.Samples );
		ds_sound_buffer.RunningSampleIndex = 0;
		// ds_clear_sound_buffer( & sound_output );
		ds_sound_buffer.SecondaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );

		ds_sound_buffer.BytesPerSecond   = ds_sound_buffer.SamplesPerSecond * ds_sound_buffer.BytesPerSample;
		ds_sound_buffer.GuardSampleBytes = (ds_sound_buffer.BytesPerSecond / Engine_Refresh_Hz) / 2;

		// TODO(Ed): When switching to core audio at minimum, this will be 1 ms of lag and guard samples wont really be needed.
		u32 min_guard_sample_bytes = 1540;
		if ( ds_sound_buffer.GuardSampleBytes < min_guard_sample_bytes )
		{
			ds_sound_buffer.GuardSampleBytes = min_guard_sample_bytes;
		}
	}
#if Build_Development
	u32             debug_marker_index = 0;
	DebugTimeMarker debug_markers[ Monitor_Refresh_Max_Supported ] {};
	u32             debug_marker_history_size = Engine_Refresh_Hz / 2;
	assert( debug_marker_history_size <= Monitor_Refresh_Max_Supported )
#endif

	engine::InputState input {};

	// There can be 4 of any of each input API type : KB & Mouse, XInput, JSL.
#if 0
	using EngineKeyboardStates = engine::KeyboardState[ Max_Controllers ];
	EngineKeyboardStates keyboard_states[2] {};
	EngineKeyboardStates* old_keyboards = & keyboard_states[0];
	EngineKeyboardStates* new_keyboards = & keyboard_states[1];
#endif

	engine::KeyboardState  keyboard_states[2] {};
	engine::KeyboardState* old_keyboard = & keyboard_states[0];
	engine::KeyboardState* new_keyboard = & keyboard_states[1];
	// Important: Assuming keyboard always connected for now, and assigning to first controller.

	EngineXInputPadStates  xpad_states[2] {};
	EngineXInputPadStates* old_xpads = & xpad_states[0];
	EngineXInputPadStates* new_xpads = & xpad_states[1];

	EngineDSPadStates  ds_pad_states[2] {};
	EngineDSPadStates* old_ds_pads = & ds_pad_states[0];
	EngineDSPadStates* new_ds_pads = & ds_pad_states[1];

	u32 jsl_num_devices = JslConnectDevices();
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
			MessageBoxA( window_handle, "More than 4 JSL devices found, this engine will only support the first four found."
				, "Warning", MB_ICONEXCLAMATION );
		}
	}

	engine_api.startup( & engine_memory, & platform_api );

	u64 last_frame_clock = timing_get_wall_clock();
	u64 last_frame_cycle = __rdtsc();
	u64 flip_wall_clock = last_frame_clock;

#if Build_Development
	u64 startup_cycles = last_frame_cycle - launch_cycle;
	f32 startup_ms     = timing_get_ms_elapsed( launch_clock, last_frame_clock );

	char text_buffer[256];
	sprintf_s( text_buffer, sizeof(text_buffer), "Startup MS: %f\n", startup_ms );
	OutputDebugStringA( text_buffer );
#endif
#pragma endregion Startup

	Running = true;
#if 0
// This tests the play & write cursor update frequency.
	while ( Running )
	{
		DWORD play_cursor;
		DWORD write_cursor;

		ds_sound_buffer.SecondaryBuffer->GetCurrentPosition( & play_cursor, & write_cursor );
		char text_buffer[256];
		sprintf_s( text_buffer, sizeof(text_buffer), "PC:%u WC:%u\n", (u32)play_cursor, (u32)write_cursor );
		OutputDebugStringA( text_buffer );
	}
#endif
	while( Running )
	{
		// Engine Module Hot-Reload
		do {
			FILETIME engine_api_current_time = file_get_last_write_time( Path_Engine_DLL );
			if ( CompareFileTime( & engine_api_load_time, & engine_api_current_time ) == 0 )
				break;

			WIN32_FIND_DATAA lock_file_info = {};
			for(;;)
			{
				HANDLE lock_file = FindFirstFileA( path_pdb_lock, & lock_file_info );
				if ( lock_file != INVALID_HANDLE_VALUE )
				{
					FindClose( lock_file );
					Sleep( 1 );
					continue;
				}
				break;
			}

			engine_api_load_time = engine_api_current_time;
			unload_engine_module_api( & engine_api );
			engine_api = load_engine_module_api();
		} while (0);

		process_pending_window_messages( new_keyboard );

		poll_input( & input, jsl_num_devices, jsl_device_handles
			, old_keyboard, new_keyboard
			, old_xpads, new_xpads
			, old_ds_pads, new_ds_pads );

		// Engine's logical iteration and rendering process
		engine_api.update_and_render( & input, rcast(engine::OffscreenBuffer*, & Surface_Back_Buffer.Memory), & engine_memory, & platform_api );

		u64   audio_frame_start = timing_get_wall_clock();
		f32   flip_to_audio_ms  = timing_get_ms_elapsed( flip_wall_clock, audio_frame_start );

		DWORD ds_play_cursor;
		DWORD ds_write_cursor;
		do {
		/*
			Audio Processing:
			There is a sync boundary value, that is the number of samples that the engine's frame-time may vary by
			(ex: approx 2ms of variance between frame-times).

			On wakeup : Check play cursor position and forcast ahead where the cursor will be for the next sync boundary.
			Based on that, check the write cursor position, if its (at least) before the synch boundary, the target write position is
			the frame boundary plus one frame. (Low latency)

			If its after (sync boundary), we cannot sync audio.
			Write a frame's worth of audio plus some number of "guard" samples. (High Latency)
		*/
			if ( ! SUCCEEDED( ds_sound_buffer.SecondaryBuffer->GetCurrentPosition( & ds_play_cursor, & ds_write_cursor ) ))
			{
				sound_is_valid = false;
				break;
			}

			if ( ! sound_is_valid )
			{
				ds_sound_buffer.RunningSampleIndex = ds_write_cursor / ds_sound_buffer.BytesPerSample;
				sound_is_valid = true;
			}

			DWORD byte_to_lock   = 0;
			DWORD target_cursor  = 0;
			DWORD bytes_to_write = 0;

			byte_to_lock = (ds_sound_buffer.RunningSampleIndex * ds_sound_buffer.BytesPerSample) % ds_sound_buffer.SecondaryBufferSize;

			DWORD bytes_per_second = ds_sound_buffer.BytesPerSample * ds_sound_buffer.SamplesPerSecond;

			DWORD expected_samplebytes_per_frame = bytes_per_second / Engine_Refresh_Hz;

			f32   left_until_flip_ms        = Engine_Frame_Target_MS - flip_to_audio_ms;
			DWORD expected_bytes_until_flip = scast(DWORD, (left_until_flip_ms / Engine_Frame_Target_MS) * scast(f32, expected_samplebytes_per_frame));

			DWORD expected_sync_boundary_byte = ds_play_cursor + expected_bytes_until_flip;

			DWORD sync_write_cursor = ds_write_cursor;
			if ( sync_write_cursor < ds_play_cursor )
			{
				// unwrap the cursor so its ahead of the play curosr linearly.
				sync_write_cursor += ds_sound_buffer.SecondaryBufferSize;
			}
			assert( sync_write_cursor >= ds_play_cursor );

			sync_write_cursor += ds_sound_buffer.GuardSampleBytes;

			b32 audio_interface_is_low_latency = sync_write_cursor < expected_sync_boundary_byte;
			if ( audio_interface_is_low_latency )
			{
				target_cursor = ( expected_sync_boundary_byte + expected_samplebytes_per_frame );
			}
			else
			{
				target_cursor = (ds_write_cursor +  expected_samplebytes_per_frame + ds_sound_buffer.GuardSampleBytes);
			}
			target_cursor %= ds_sound_buffer.SecondaryBufferSize;

			if ( byte_to_lock > target_cursor)
			{
				// Infront of play cursor |--play--byte_to_write-->--|
				bytes_to_write =  ds_sound_buffer.SecondaryBufferSize - byte_to_lock;
				bytes_to_write += target_cursor;
			}
			else
			{
				// Behind play cursor |--byte_to_write-->--play--|
				bytes_to_write = target_cursor - byte_to_lock;
			}

		// Engine Sound
			// s16 samples[ 48000 * 2 ];
			engine::AudioBuffer sound_buffer {};
			sound_buffer.NumSamples         = bytes_to_write / ds_sound_buffer.BytesPerSample;
			sound_buffer.RunningSampleIndex = ds_sound_buffer.RunningSampleIndex;
			sound_buffer.SamplesPerSecond   = ds_sound_buffer.SamplesPerSecond;
			sound_buffer.Samples            = ds_sound_buffer.Samples;
			engine_api.update_audio( & sound_buffer, & engine_memory, & platform_api );

			DebugTimeMarker* marker = & debug_markers[ debug_marker_index ];
			marker->OutputPlayCusror   = ds_play_cursor;
			marker->OutputWriteCursor  = ds_write_cursor;
			marker->OutputLocation     = byte_to_lock;
			marker->OutputByteCount    = bytes_to_write;
			marker->ExpectedFlipCursor = expected_sync_boundary_byte;

		// Update audio buffer
			if ( ! sound_is_valid )
				break;

		#if Build_Development && 0
		#if 0
			DWORD play_cursor;
			DWORD write_cursor;
			ds_sound_buffer.SecondaryBuffer->GetCurrentPosition( & play_cursor, & write_cursor );
		#endif
			DWORD unwrapped_write_cursor = ds_write_cursor;
			if ( unwrapped_write_cursor < ds_play_cursor )
			{
				unwrapped_write_cursor += ds_sound_buffer.SecondaryBufferSize;
			}
			ds_cursor_byte_delta = unwrapped_write_cursor - ds_play_cursor;

			constexpr f32 to_milliseconds = 1000.f;
			f32 sample_delta  = scast(f32, ds_cursor_byte_delta) / scast(f32, ds_sound_buffer.BytesPerSample);
			f32 ds_latency_s  = sample_delta / scast(f32, ds_sound_buffer.SamplesPerSecond);
			    ds_latency_ms = ds_latency_s * to_milliseconds;

			char text_buffer[256];
			sprintf_s( text_buffer, sizeof(text_buffer), "BTL:%u TC:%u BTW:%u - PC:%u WC:%u DELTA:%u bytes %f ms\n"
				, (u32)byte_to_lock, (u32)target_cursor, (u32)bytes_to_write
				, (u32)play_cursor, (u32)write_cursor, (u32)ds_cursor_byte_delta, ds_latency_ms );
			OutputDebugStringA( text_buffer );
		#endif
			ds_fill_sound_buffer( & ds_sound_buffer, byte_to_lock, bytes_to_write  );

			DWORD ds_status = 0;
			if ( SUCCEEDED( ds_sound_buffer.SecondaryBuffer->GetStatus( & ds_status ) ) )
			{
				ds_sound_buffer.IsPlaying = ds_status & DSBSTATUS_PLAYING;
			}
			if ( ds_sound_buffer.IsPlaying )
				break;

			ds_sound_buffer.SecondaryBuffer->Play( 0, 0, DSBPLAY_LOOPING );
		} while(0);

		// Timing Update
		{
			u64 work_frame_end_cycle = __rdtsc();
			u64 work_frame_end_clock = timing_get_wall_clock();

			f32 work_frame_ms  = timing_get_ms_elapsed( last_frame_clock, work_frame_end_clock );  // WorkSecondsElapsed
			f32 work_cycles    = timing_get_ms_elapsed( last_frame_cycle, work_frame_end_cycle );

			f32 frame_elapsed_ms = work_frame_ms; // SecondsElapsedForFrame
			if ( frame_elapsed_ms < Engine_Frame_Target_MS )
			{
				s32 sleep_ms = scast(DWORD, (Engine_Frame_Target_MS - frame_elapsed_ms)) - 1;
				if ( sleep_ms > 0 && ! sub_ms_granularity_required && sleep_is_granular )
				{
					Sleep( scast(DWORD, sleep_ms) );
				}

				u64 frame_clock  = timing_get_wall_clock();
				frame_elapsed_ms = timing_get_ms_elapsed( last_frame_clock, frame_clock );
				if ( frame_elapsed_ms < Engine_Frame_Target_MS )
				{
					// TODO(Ed) : Log missed sleep here.
				}

				while ( frame_elapsed_ms < Engine_Frame_Target_MS )
				{
					frame_clock      = timing_get_wall_clock();
					frame_elapsed_ms = timing_get_ms_elapsed( last_frame_clock, frame_clock );
				}
			}
			else
			{
				// TODO(Ed) : Missed the display sync window!
			}

			last_frame_clock = timing_get_wall_clock(); // LastCouner
			last_frame_cycle = __rdtsc();
		}

		// Update surface back buffer
		if ( ! Pause_Rendering )
		{
			WinDimensions dimensions     = get_window_dimensions( window_handle );
			HDC           device_context = GetDC( window_handle );

		#if Build_Development
			// Note: debug_marker_index is wrong for the 0th index
			debug_sync_display( & ds_sound_buffer
				, debug_marker_history_size, debug_markers, debug_marker_index - 1
				, Engine_Frame_Target_MS );
		#endif

			display_buffer_in_window( device_context, dimensions.Width, dimensions.Height, &Surface_Back_Buffer
				, 0, 0
				, dimensions.Width, dimensions.Height );
			ReleaseDC( window_handle, device_context );
		}

		flip_wall_clock = timing_get_wall_clock();
		#if Build_Development
		{
			// Audio Debug
			DWORD play_cursor  = 0;
			DWORD write_cursor = 0;
			if ( SUCCEEDED( ds_sound_buffer.SecondaryBuffer->GetCurrentPosition( & play_cursor, & write_cursor ) ) )
			{
				if ( ! sound_is_valid )
				{
					ds_sound_buffer.RunningSampleIndex = write_cursor / ds_sound_buffer.BytesPerSample;
					sound_is_valid = true;
				}

				assert( debug_marker_index < debug_marker_history_size )
				DebugTimeMarker* marker = & debug_markers[ debug_marker_index ];

				marker->FlipPlayCursor  = play_cursor;
				marker->FlipWriteCursor = write_cursor;
			}
		}
		#endif

		#if Build_Development
			debug_marker_index++;
			if ( debug_marker_index >= debug_marker_history_size )
				debug_marker_index = 0;
		#endif
	}

	engine_api.shutdown( & engine_memory, & platform_api );

	unload_engine_module_api( & engine_api );
	DeleteFileA( Path_Engine_DLL_InUse );

	if ( jsl_num_devices > 0 )
	{
		for ( u32 jsl_device_index = 0; jsl_device_index < jsl_num_devices; ++ jsl_device_index )
		{
			JslSetLightColour( jsl_device_handles[ jsl_device_index ], 0 );
		}
	}

	return 0;
}
