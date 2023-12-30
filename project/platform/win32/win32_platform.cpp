#if INTELLISENSE_DIRECTIVES
#include "platform/platform.hpp"
#include "engine/engine.hpp"
#include "engine/engine_to_platform_api.hpp"
#include "engine/gen/engine_symbols.gen.hpp"
#include "win32.hpp"
#include "jsl.hpp"
#endif
/*
	TODO : This is not a final platform layer

	- Saved game locations
	- Getting a handle to our own executable file
	- Asset loading path
	- Threading (launch a thread)
	- Raw Input (support for multiple keyboards)
	- ClipCursor() (for multimonitor support)
	- QueryCancelAutoplay
	- WM_ACTIVATEAPP (for when not active)
	- Blit speed improvemnts (BitBlt)
	- Hardware acceleration ( OpenGL or Direct3D or both )
	- GetKeyboardLayout (for French keyboards, international WASD support)
*/

NS_PLATFORM_BEGIN
using namespace win32;

struct PlatformContext
{
	using_context()


};

// This is the "backbuffer" data related to the windowing surface provided by the operating system.
struct OffscreenBuffer
{
	BITMAPINFO info;
	char       _PAD_[4];
	void*      memory; // Lets use directly mess with the "pixel's memory buffer"
	s32        width;
	s32        height;
	s32        pitch;
	s32        bytes_per_pixel;
};

struct WinDimensions
{
	u32 width;
	u32 height;
};

#pragma region Static Data
global PlatformContext Platform_Context;

global StrPath Path_Root;
global StrPath Path_Binaries;
global StrPath Path_Content;
global StrPath Path_Scratch;

// TODO(Ed) : This is a global for now.
global b32 Running = false;

global b32             Show_Windows_Cursor;
global HCURSOR         Windows_Cursor;
global WINDOWPLACEMENT Window_Position;

global WinDimensions   Window_Dimensions;
global OffscreenBuffer Surface_Back_Buffer;

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


void impl_congrats( char const* message )
{
	JslSetLightColour( 0, (255 << 16) | (215 << 8) );
	MessageBoxA( 0, message, "Congratulations!", MB_OK | MB_ICONEXCLAMATION );
	JslSetLightColour( 0, (255 << 8 ) );
}

internal
FILETIME file_get_last_write_time( char const* path )
{
	WIN32_FILE_ATTRIBUTE_DATA engine_dll_file_attributes = {};
	GetFileAttributesExA( path, GetFileExInfoStandard, & engine_dll_file_attributes );

	return engine_dll_file_attributes.ftLastWriteTime;
}

#pragma region Timing
inline f32
timing_get_ms_elapsed( u64 start, u64 end )
{
	assert( end - start );
	u64 delta  = (end - start) * Tick_To_Millisecond;
	f32 result = scast(f32, delta) / scast(f32, Performance_Counter_Frequency);
	return result;
}

inline f32
timing_get_seconds_elapsed( u64 start, u64 end )
{
	assert( end > start );
	u64 delta = end - start;
	f32 result = scast(f32, delta) / scast(f32, Performance_Counter_Frequency);
	return result;
}

inline f32
timing_get_us_elapsed( u64 start, u64 end )
{
	assert( end - start );
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
NS_PLATFORM_END

#include "win32_audio.cpp"
#include "win32_input.cpp"
#include "platform/win32/win32_platform_api.cpp"

NS_PLATFORM_BEGIN
#pragma region Windows Sandbox Interface

internal
void toggle_fullscreen( HWND window_handle )
{
	// Note(Ed) : Follows: https://devblogs.microsoft.com/oldnewthing/20100412-00/?p=14353
	DWORD style = GetWindowLongA( window_handle, GWL_STYLE );
	if ( style & WS_OVERLAPPEDWINDOW )
	{
		MONITORINFO info           = { sizeof(MONITORINFO) };
		HMONITOR    monitor_handle = MonitorFromWindow( window_handle, MONITOR_DEFAULTTOPRIMARY );

		if ( GetWindowPlacement( window_handle, & Window_Position )
		    && GetMonitorInfoA( monitor_handle, & info ) )
		{
			SetWindowLongA( window_handle, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW );
			SetWindowPos( window_handle, HWND_TOP
			             , info.rcMonitor.left,                     info.rcMonitor.top
			             , info.rcWork.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top
			             , SWP_NOOWNERZORDER | SWP_FRAMECHANGED
			);
		}
	}
	else
	{
		SetWindowLongA( window_handle, GWL_STYLE , style | WS_OVERLAPPEDWINDOW );
		SetWindowPlacement( window_handle, & Window_Position );
		SetWindowPos( window_handle, NULL
		             , 0, 0, 0, 0
		             , SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED );
	}
}

internal WinDimensions
get_window_dimensions( HWND window_handle )
{
	RECT client_rect;
	GetClientRect( window_handle, & client_rect );
	WinDimensions result;
	result.width  = client_rect.right  - client_rect.left;
	result.height = client_rect.bottom - client_rect.top;
	return result;
}

internal void
display_buffer_in_window( HDC device_context, s32 window_width, s32 window_height, OffscreenBuffer* buffer
	, s32 x, s32 y
	, s32 width, s32 height )
{
	if ( 	(window_width  % buffer->width ) == 0
	    &&	(window_height % buffer->height) == 0 )
	{
		// TODO(Ed) : Aspect ratio correction
		StretchDIBits( device_context
			, 0, 0, window_width, window_height
			, 0, 0, buffer->width, buffer->height
			, buffer->memory, & buffer->info
			, DIB_ColorTable_RGB, RO_Source_To_Dest );

		return;
	}

	s32 offset_x = 0;
	s32 offset_y = 0;

	if ( window_width > buffer->width )
		offset_x = (window_width - buffer->width) / 2;

	if ( window_height > buffer->height )
		offset_y = (window_height - buffer->height) / 2;

	PatBlt( device_context
		, 0, 0
		, window_width, offset_y
		, BLACKNESS );

	PatBlt( device_context
		, 0, 0
		, offset_x, window_height
		, BLACKNESS );

	PatBlt( device_context
		, offset_x + buffer->width, 0
		, window_width, window_height
		, BLACKNESS);

	PatBlt( device_context
		, 0, offset_y + buffer->height
		, window_width, window_height
		, BLACKNESS );

	// TODO(Ed) : Aspect ratio correction
	StretchDIBits( device_context
	#if 0
		, x, y, width, height
		, x, y, width, height
	#endif
		, offset_x, offset_y, buffer->width, buffer->height
		// , 0, 0, window_width, window_height
		, 0, 0, buffer->width, buffer->height
		, buffer->memory, & buffer->info
		, DIB_ColorTable_RGB, RO_Source_To_Dest );
}

internal void
resize_dib_section( OffscreenBuffer* buffer, u32 width, u32 height )
{
	// TODO(Ed) : Bulletproof memory handling here for the bitmap memory
	if ( buffer->memory )
	{
		VirtualFree( buffer->memory, 0, MEM_RELEASE );
	}

	buffer->width           = width;
	buffer->height          = height;
	buffer->bytes_per_pixel = 4;
	buffer->pitch           = buffer->width * buffer->bytes_per_pixel;

	// Negative means top-down in the context of the biHeight
#	define Top_Down -
	BITMAPINFOHEADER&
	header = buffer->info.bmiHeader;
	header.biSize        = sizeof( buffer->info.bmiHeader );
	header.biWidth       = buffer->width;
	header.biHeight      = Top_Down buffer->height;
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
	u32 BitmapMemorySize = (buffer->width * buffer->height) * buffer->bytes_per_pixel;
	buffer->memory = VirtualAlloc( NULL, BitmapMemorySize, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write );

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
//				 SetLayeredWindowAttributes( handle, RGB(0, 0, 0), 120, LWA_Alpha );
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

			display_buffer_in_window( device_context, dimensions.width, dimensions.height, &Surface_Back_Buffer
				, x, y
				, width, height );
			EndPaint( handle, & info );
		}
		break;

		// TODO(Ed) : Expose cursor toggling to engine via platform api (lets game control it for its ux purposes)
		case WM_MOUSEMOVE:
		{

			while (ShowCursor(FALSE) >= 0);
		}
		break;

		case WM_NCMOUSEMOVE:
		{
			// Show the cursor when it's outside the window's client area (i.e., on the frame or elsewhere)
			while (ShowCursor(TRUE) < 0);
		}
		break;

		case WM_SETCURSOR:
		{
			if ( Show_Windows_Cursor )
			{
//				SetCursor( Windows_Cursor );
				result = DefWindowProc( handle, system_messages, w_param, l_param );
			}
			else
			{
				SetCursor(NULL);
			}
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
process_pending_window_messages( HWND window_handle, engine::KeyboardState* keyboard, engine::MousesState* mouse )
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
					case VK_F10:
					{
						// TODO(Ed) : Expose this feature via platform_api to engine. Let the game toggle via the its action binds.
						if ( is_down )
							toggle_fullscreen( window_handle );
					}
					break;
				}
			}
			break;

			case WM_MOUSEWHEEL:
			{
				// This captures the vertical scroll value
				int verticalScroll = GET_WHEEL_DELTA_WPARAM(window_msg_info.wParam);
				mouse->vertical_wheel.end += scast(f32, verticalScroll);
			}
			break;

			case WM_MOUSEHWHEEL:
			{
				// This captures the horizontal scroll value
				int horizontalScroll = GET_WHEEL_DELTA_WPARAM(window_msg_info.wParam);
				mouse->horizontal_wheel.end += scast( f32, horizontalScroll);
			}
			break;

			default:
				TranslateMessage( & window_msg_info );
				DispatchMessageW( & window_msg_info );
		}
	}
}

LONG WINAPI unhandled_exeception( EXCEPTION_POINTERS* exception_info )
{
    if ( exception_info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION )
	{
        congrats("Access violation detected! \n");
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

#pragma endregion Windows Sandbox Interface

#pragma region Engine Module

constexpr const Str FName_Engine_DLL       = str_ascii("handmade_engine.dll");
constexpr const Str FName_Engine_DLL_InUse = str_ascii("handmade_engine_in_use.dll");
constexpr const Str FName_Engine_PDB_Lock  = str_ascii("handmade_engine.pdb.lock");

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

	engine::ModuleAPI engine_api {};
	engine_api.on_module_reload  = get_procedure_from_library< engine::OnModuleRelaodFn > ( Lib_Handmade_Engine, engine::symbol_on_module_load );
	engine_api.startup           = get_procedure_from_library< engine::StartupFn >        ( Lib_Handmade_Engine, engine::symbol_startup );
	engine_api.shutdown          = get_procedure_from_library< engine::ShutdownFn >       ( Lib_Handmade_Engine, engine::symbol_shutdown );
	engine_api.update_and_render = get_procedure_from_library< engine::UpdateAndRenderFn >( Lib_Handmade_Engine, engine::symbol_update_and_render );
	engine_api.update_audio      = get_procedure_from_library< engine::UpdateAudioFn >    ( Lib_Handmade_Engine, engine::symbol_update_audio );

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
	else {
		fatal( "Failed to load engine module API!\n" );
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
#pragma endregion Engine Module

NS_PLATFORM_END

int CALLBACK
WinMain( HINSTANCE instance, HINSTANCE prev_instance, LPSTR commandline, int show_command )
{
	using namespace win32;
	using namespace platform;

	SetUnhandledExceptionFilter(unhandled_exeception);

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
		Path_Binaries.len = str_length( Path_Binaries );
		Path_Binaries[ Path_Binaries.len ] = '\\';
		++ Path_Binaries.len;

		if ( SetCurrentDirectoryA( ".." ) == 0 )
		{
			fatal( "Failed to set current directory to root!");
		}

		if ( GetCurrentDirectoryA( S16_MAX, Path_Root.ptr ) == 0 )
		{
			fatal( "Failed to get the root directory!" );
		}
		Path_Root.len = str_length(Path_Root.ptr);
		Path_Root.ptr[ Path_Root.len ] = '\\';
		++ Path_Root.len;

		Path_Engine_DLL.      concat( Path_Binaries, FName_Engine_DLL );
		Path_Engine_DLL_InUse.concat( Path_Binaries, FName_Engine_DLL_InUse );

		path_pdb_lock.concat( Path_Binaries, FName_Engine_PDB_Lock );

		Path_Scratch.concat( Path_Root, str_ascii("scratch") );
		Path_Scratch.ptr[ Path_Scratch.len ] = '\\';
		++ Path_Scratch.len;

		CreateDirectoryA( Path_Scratch, 0 );

		Path_Content.concat( Path_Root, str_ascii("content") );
		Path_Content.ptr[ Path_Content.len ] = '\\';
		++ Path_Content.len;
	}

	// Memory
	engine::Memory engine_memory {};
	{
		engine_memory.persistent_size = megabytes( 128 );
		// engine_memory.FrameSize	  = megabytes( 64 );
		engine_memory.transient_size  = gigabytes( 2 );

		u64 total_size = engine_memory.persistent_size
			// + engine_memory.FrameSize
			+ engine_memory.transient_size;

	#if Build_Debug
		void* base_address = rcast(void*, terabytes( 1 ));
	#else
		void* base_address = 0;
	#endif

		engine_memory.persistent = rcast( Byte*, VirtualAlloc( base_address, total_size , MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write ));
		engine_memory.transient  = rcast( Byte*, engine_memory.persistent ) + engine_memory.persistent_size;

	#if Build_Development
		// First slot is for restore
		for (u32 slot = 0; slot < engine_memory.Num_Snapshot_Slots; ++slot)
		{
			engine::MemorySnapshot& snapshot = engine_memory.snapshots[ slot ];

			snapshot.file_path.concat( Path_Scratch, str_ascii("snapshot_") );
			wsprintfA( snapshot.file_path.ptr, "%s%d.hm_snapshot", snapshot.file_path.ptr, slot );

			HANDLE snapshot_file = CreateFileA( snapshot.file_path
				, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0
				, OPEN_ALWAYS, 0, 0 );

			LARGE_INTEGER file_size {};
			file_size.QuadPart = total_size;

			HANDLE snapshot_mapping = CreateFileMappingA( snapshot_file, 0
				, Page_Read_Write
				, file_size.HighPart, file_size.LowPart
				, 0 );

			if (!snapshot_mapping)
			{
				// Handle the error, perhaps log it or display a message
				DWORD error = GetLastError();

				char text[256];
				wsprintfA(text, "FlushFileBuffers failed with error code: %lu\n", error);
				OutputDebugStringA(text);
				congrats( text );

				CloseHandle(snapshot_file); // Close the file handle before continuing
				continue; // Skip the current iteration
			}

			snapshot.memory = MapViewOfFile( snapshot_mapping, FILE_MAP_ALL_ACCESS, 0, 0, total_size );

			if (!snapshot.memory)
			{
				// Handle the error, perhaps log it or display a message
				DWORD error = GetLastError();

				char text[256];
				wsprintfA(text, "FlushFileBuffers failed with error code: %lu\n", error);
				OutputDebugStringA(text);
				congrats( text );

				CloseHandle(snapshot_mapping); // Close the mapping handle
				CloseHandle(snapshot_file);    // Close the file handle
				continue; // Skip the current iteration
			}

			snapshot.opaque_handle   = snapshot_file;
			snapshot.opaque_handle_2 = snapshot_mapping;
		}
	#endif

		if (	engine_memory.persistent == nullptr
			||	engine_memory.transient  == nullptr )
		{
			// TODO : Diagnostic Logging
			return -1;
		}
	}

	WNDCLASSW window_class {};
	HWND window_handle = nullptr;
	b32 window_in_foreground = false;
	{
		window_class.style = CS_Horizontal_Redraw | CS_Vertical_Redraw;
		window_class.lpfnWndProc = main_window_callback;
		// window_class.cbClsExtra  = ;
		// window_class.cbWndExtra  = ;
		window_class.hInstance   = instance;
		// window_class.hIcon = ;
		 window_class.hCursor = LoadCursorW( 0, IDC_ARROW );
		// window_class.hbrBackground = ;
		window_class.lpszMenuName  = L"Handmade Hero!";
		window_class.lpszClassName = L"HandmadeHeroWindowClass";

		Show_Windows_Cursor = true;
		Windows_Cursor      = LoadCursorW( 0, IDC_CROSS );
		Window_Position     = {sizeof(WINDOWPLACEMENT)};

		if ( ! RegisterClassW( & window_class ) )
		{
			// TODO : Diagnostic Logging
			return 0;
		}

		window_handle = CreateWindowExW(
			// WS_EX_LAYERED | WS_EX_TOPMOST,
			WS_EX_LAYERED,
			window_class.lpszClassName,
			L"Handmade Hero",
			WS_Overlapped_Window | WS_Initially_Visible,
			300, 300, // x, y
			1280, 720, // width, height
			0, 0,                         // parent, menu
			instance, 0                   // instance, param
		);

		if ( ! window_handle )
		{
			// TODO : Diagnostic Logging
			return 0;
		}

		// WinDimensions dimensions = get_window_dimensions( window_handle );
		resize_dib_section( &Surface_Back_Buffer, 1280, 720	 );

		// Setup monitor refresh and associated timers
		HDC refresh_dc = GetDC( window_handle );
		u32 monitor_refresh_hz = GetDeviceCaps( refresh_dc, VREFRESH );
		if ( monitor_refresh_hz > 1 )
		{
			Monitor_Refresh_Hz = monitor_refresh_hz;
		}
		ReleaseDC( window_handle, refresh_dc );

		Engine_Refresh_Hz      = monitor_refresh_hz;
		Engine_Frame_Target_MS = 1000.f / scast(f32, Engine_Refresh_Hz);
	}

	// Prepare platform API
	ModuleAPI platform_api {};
	{
		platform_api.path_root     = Path_Root;
		platform_api.path_binaries = Path_Binaries;
		platform_api.path_content  = Path_Content;
		platform_api.path_scratch  = Path_Scratch;

	#if Build_Development
		platform_api.debug_set_pause_rendering = & debug_set_pause_rendering;
	#endif

		platform_api.get_wall_clock = & timing_get_wall_clock;

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

		platform_api.memory_copy = & memory_copy;
	}

	// Load engine module
	FILETIME          engine_api_load_time = file_get_last_write_time( Path_Engine_DLL );
	engine::ModuleAPI engine_api           = load_engine_module_api();

	b32               sound_is_valid       = false;
	DWORD             ds_cursor_byte_delta = 0;
	f32               ds_latency_ms        = 0;
	DirectSoundBuffer ds_sound_buffer;
	u32               audio_marker_index = 0;
	AudioTimeMarker   audio_time_markers[ Monitor_Refresh_Max_Supported ] {};
	u32               audio_time_markers_size = Engine_Refresh_Hz / 2;
	assert( audio_time_markers_size <= Monitor_Refresh_Max_Supported )
	{
		ds_sound_buffer.is_playing         = 0;
		ds_sound_buffer.samples_per_second = 48000;
		ds_sound_buffer.bytes_per_sample   = sizeof(s16) * 2;

		ds_sound_buffer.secondary_buffer_size = ds_sound_buffer.samples_per_second * ds_sound_buffer.bytes_per_sample;
		init_sound( window_handle, & ds_sound_buffer );

		ds_sound_buffer.samples = rcast( s16*, VirtualAlloc( 0, 48000 * 2 * sizeof(s16)
			, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write ));

		assert( ds_sound_buffer.samples );
		ds_sound_buffer.running_sample_index = 0;
		// ds_clear_sound_buffer( & sound_output );
		ds_sound_buffer.secondary_buffer->Play( 0, 0, DSBPLAY_LOOPING );

		ds_sound_buffer.bytes_per_second   = ds_sound_buffer.samples_per_second * ds_sound_buffer.bytes_per_sample;
		ds_sound_buffer.guard_sample_bytes = (ds_sound_buffer.bytes_per_second / Engine_Refresh_Hz) / 2;

		// TODO(Ed): When switching to core audio at minimum, this will be 1 ms of lag and guard samples wont really be needed.
		u32 min_guard_sample_bytes = 1540;
		if ( ds_sound_buffer.guard_sample_bytes < min_guard_sample_bytes )
		{
			ds_sound_buffer.guard_sample_bytes = min_guard_sample_bytes;
		}
	}

	engine::InputState input {};

	// There can be 4 of any of each input API type : KB & Mouse, XInput, JSL.
#if 0
	using EngineKeyboardStates = engine::KeyboardState[ Max_Controllers ];
	EngineKeyboardStates* old_keyboards = & keyboard_states[0];
	EngineKeyboardStates* new_keyboards = & keyboard_states[1];
	EngineKeyboardStates keyboard_states[2] {};
#endif

	engine::KeyboardState  keyboard_states[2] {};
	engine::KeyboardState* old_keyboard = & keyboard_states[0];
	engine::KeyboardState* new_keyboard = & keyboard_states[1];
	// Important: Assuming keyboard always connected for now, and assigning to first controller.

	engine::MousesState mouse_states[2] {};
	engine::MousesState* old_mouse = & mouse_states[0];
	engine::MousesState* new_mouse = & mouse_states[1];

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

	// Populate an initial polling state for the inputs
	poll_input( window_handle, & input, jsl_num_devices, jsl_device_handles
		, old_keyboard, new_keyboard
		, old_mouse, new_mouse
		, old_xpads, new_xpads
		, old_ds_pads, new_ds_pads );

	engine_api.startup( rcast(engine::OffscreenBuffer*, & Surface_Back_Buffer.memory), & engine_memory, & platform_api );

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

	// Placeholder
	engine::ThreadContext thread_context_placeholder {};

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
		f32 delta_time = Engine_Frame_Target_MS / 1000.f;

		window_in_foreground = ( GetForegroundWindow() == window_handle );

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
			engine_api.on_module_reload( & engine_memory, & platform_api );
		} while (0);


		if ( window_in_foreground )
		{
			// Swapping at the beginning of the input frame instead of the end.
			swap( old_keyboard, new_keyboard );
			swap( old_mouse,    new_mouse );
			swap( old_xpads,    new_xpads );
			swap( old_ds_pads,  new_ds_pads );

			poll_input( window_handle, & input, jsl_num_devices, jsl_device_handles
				, old_keyboard, new_keyboard
				, old_mouse, new_mouse
				, old_xpads, new_xpads
				, old_ds_pads, new_ds_pads );
		}
		else
		{
			keyboard_states[0] = {};
			keyboard_states[1] = {};

			mouse_states[0] = {};
			mouse_states[0] = {};

			for ( s32 id = 0; id < engine::Max_Controllers; ++ id )
			{
				xpad_states[0][ id ] = {};
				xpad_states[1][ id ] = {};

				ds_pad_states[0][ id ] = {};
				ds_pad_states[1][ id ] = {};
			}
		}

		process_pending_window_messages( window_handle, new_keyboard, new_mouse );

		// f32 delta_time = timing_get_seconds_elapsed( last_frame_clock, timing_get_wall_clock() );

		// Engine's logical iteration and rendering process
		engine_api.update_and_render( delta_time, & input, rcast(engine::OffscreenBuffer*, & Surface_Back_Buffer.memory )
			, & engine_memory, & platform_api, & thread_context_placeholder );

		u64   audio_frame_start = timing_get_wall_clock();
		f32   flip_to_audio_ms  = timing_get_ms_elapsed( flip_wall_clock, audio_frame_start );

		DWORD ds_play_cursor;
		DWORD ds_write_cursor;
		process_audio_frame( ds_sound_buffer, ds_play_cursor, ds_write_cursor, ds_latency_ms
			, sound_is_valid
			, audio_time_markers, audio_marker_index
			, flip_to_audio_ms, last_frame_clock
			, engine_api, & engine_memory, & platform_api, & thread_context_placeholder );

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

		#if Build_Development && 0
			// Note: debug_marker_index is wrong for the 0th index
			debug_sync_display( & ds_sound_buffer
				, audio_time_markers_size, audio_time_markers, audio_marker_index - 1
				, Engine_Frame_Target_MS );
		#endif

			display_buffer_in_window( device_context, dimensions.width, dimensions.height, &Surface_Back_Buffer
				, 0, 0
				, dimensions.width, dimensions.height );
			ReleaseDC( window_handle, device_context );
		}

		flip_wall_clock = timing_get_wall_clock();
		#if Build_Development
		{
			// Audio Debug
			DWORD play_cursor  = 0;
			DWORD write_cursor = 0;
			if ( SUCCEEDED( ds_sound_buffer.secondary_buffer->GetCurrentPosition( & play_cursor, & write_cursor ) ) )
			{
				if ( ! sound_is_valid )
				{
					ds_sound_buffer.running_sample_index = write_cursor / ds_sound_buffer.bytes_per_sample;
					sound_is_valid = true;
				}

				assert( audio_marker_index < audio_time_markers_size )
				AudioTimeMarker* marker = & audio_time_markers[ audio_marker_index ];

				marker->flip_play_curosr = play_cursor;
				marker->flip_write_cursor = write_cursor;
			}
		}
		#endif

		#if Build_Development
			audio_marker_index++;
			if ( audio_marker_index >= audio_time_markers_size )
				audio_marker_index = 0;
		#endif
	}

	engine_api.shutdown( & engine_memory, & platform_api );

#if Build_Development
	for ( s32 slot = 0; slot < engine_memory.Num_Snapshot_Slots; ++slot )
	{
		engine::MemorySnapshot& snapshot = engine_memory.snapshots[ slot ];

		UnmapViewOfFile( snapshot.memory );
		CloseHandle( snapshot.opaque_handle_2 );
		CloseHandle( snapshot.opaque_handle );
	}
#endif

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
