#include "platform/platform.hpp"
#include "platform/jsl.hpp"
#include "win32.hpp"

NS_PLATFORM_BEGIN

global b32 Pause_Rendering = false;

#pragma region Platfom API
#if Build_Development
void congrats_impl( char const* message )
{
	JslSetLightColour( 0, (255 << 16) | (215 << 8) );
	MessageBoxA( 0, message, "Congratulations!", MB_OK | MB_ICONEXCLAMATION );
	JslSetLightColour( 0, (255 << 8 ) );
}

bool impl_ensure( bool condition, char const* message )
{
	if ( ! condition ) {
		JslSetLightColour( 0, (255 << 16) );
		MessageBoxA( 0, message, "Ensure Failure", MB_OK | MB_ICONASTERISK );
		JslSetLightColour( 0, ( 255 << 8 ) );
	}
	return condition;
}

void impl_fatal( char const* message )
{
	JslSetLightColour( 0, (255 << 16) );
	MessageBoxA( 0, message, "Fatal Error", MB_OK | MB_ICONERROR );
	JslSetLightColour( 0, (255 << 8 ) );
}
#endif

#if Build_Development
void debug_set_pause_rendering( b32 value )
{
	Pause_Rendering = value;
}
#endif

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
	HANDLE handle = pcast(HANDLE, file->opaque_handle);

	if ( handle == INVALID_HANDLE_VALUE )
		return;

	CloseHandle( handle );

	if ( file->data )
	{
		// TODO(Ed): This should use our persistent memory block.
		VirtualFree( file->data, 0, MEM_Release);
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
	if ( file->opaque_handle == nullptr )
	{
		file_handle = CreateFileA( file->path
			, GENERIC_READ, FILE_SHARE_READ, 0
			, OPEN_EXISTING, 0, 0
		);
		if ( file_handle == INVALID_HANDLE_VALUE )
		{
			// TODO : Logging
			return {};
		}

		file->opaque_handle = file_handle;
	}
	else
	{
		file_handle = pcast(HANDLE, file->opaque_handle );
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
	HANDLE file_handle = CreateFileA( file->path
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
	file->data = rcast(HANDLE*, VirtualAlloc( 0, sizeof(HANDLE) + size, MEM_Commit_Zeroed | MEM_Reserve, Page_Read_Write ));
	file->size = size;
	file->opaque_handle = file_handle;

	u32 bytes_read;
	if ( ReadFile( file_handle, file->data, file->size, rcast(LPDWORD, &bytes_read), 0 ) == false )
	{
		// TODO(Ed) : Logging
		CloseHandle( file_handle );
		return {};
	}

	if ( bytes_read != file->size )
	{
		// TODO : Logging
		CloseHandle( file_handle );
		return {};
	}
	return bytes_read;
}

void file_rewind( File* file )
{
	HANDLE file_handle = pcast(HANDLE, file->opaque_handle );
	if ( file_handle == INVALID_HANDLE_VALUE )
		return;

	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);
}

u32 file_write_stream( File* file, u32 content_size, void* content_memory )
{
	HANDLE file_handle;
	if ( file->opaque_handle == nullptr )
	{
		file_handle = CreateFileA( file->path
			,GENERIC_WRITE, 0, 0
			, OPEN_ALWAYS, 0, 0
		);
		if ( file_handle == INVALID_HANDLE_VALUE )
		{
			// TODO(Ed) : Logging
			return {};
		}

		file->opaque_handle = file_handle;
	}
	else
	{
		file_handle = pcast(HANDLE, file->opaque_handle );
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
	HANDLE file_handle = CreateFileA( file->path
		, GENERIC_WRITE, 0, 0
		, CREATE_ALWAYS, 0, 0
	);
	if ( file_handle == INVALID_HANDLE_VALUE )
	{
		// TODO : Logging
		return false;
	}
	file->opaque_handle = file_handle;

	DWORD bytes_written;
	if ( WriteFile( file_handle, content_memory, content_size, & bytes_written, 0 ) == false )
	{
		// TODO : Logging
		return false;
	}
	return bytes_written;
}

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
	FreeLibrary( scast(HMODULE, module->opaque_handle) );
	*module = {};
}

void* get_binary_module_symbol( BinaryModule module, char const* symbol_name )
{
	return rcast(void*, GetProcAddress( scast(HMODULE, module.opaque_handle), symbol_name ));
}

void memory_copy( void* dest, u64 src_size, void* src )
{
	CopyMemory( dest, src, src_size );
}
#pragma endregion Platform API

NS_PLATFORM_END
