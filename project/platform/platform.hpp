/*
	Platform abstraction layer for the project.
	Services the platform provides to the engine & game.

	This should be the only file the engine or game layer can include related to the platform layer.
	(Public Interface essentially...)
*/

#pragma once

#pragma warning( disable: 4201 ) // Support for non-standard nameless struct or union extesnion
#pragma warning( disable: 4100 ) // Support for unreferenced formal parameters
#pragma warning( disable: 4800 ) // Support implicit conversion to bools
#pragma warning( disable: 4365 ) // Support for signed/unsigned mismatch auto-conversion
#pragma warning( disable: 4189 ) // Support for unused variables
#pragma warning( disable: 4514 ) // Support for unused inline functions
#pragma warning( disable: 4505 ) // Support for unused static functions
#pragma warning( disable: 5045 ) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning( disable: 5264 ) // Support for 'const' variables unused
#pragma warning( disable: 4820 ) // Support auto-adding padding to structs

// TODO(Ed) : REMOVE THESE WHEN HE GETS TO THEM
#include <math.h> // TODO : Implement math ourselves
#include <stdio.h> // TODO : Implement output logging ourselves

#include "grime.hpp"
#include "macros.hpp"
#include "generics.hpp"
#include "math_constants.hpp"
#include "types.hpp"
#include "strings.hpp"

#define NS_PLATFORM_BEGIN namespace platform {
#define NS_PLATFORM_END }

NS_PLATFORM_BEGIN

// On-Demand platform interface.
// Everything exposed here should be based on a feature a game may want to provide a user
// (Example: Letting the user change the refresh-rate of the monitor or the engine's target frame-rate)

#if Build_Development
/*
	IMPORTANT : These are not for shipping code - they are blocking and the write isn't protected.
*/

using DebugSetPauseRenderingFn = void (b32 value);

struct File
{
	void* OpaqueHandle;
	Str   Path;
	void* Data;
	u32   Size;
};

// TODO(Ed): This also assumes the symbol name is always within size of the provided buffer, needs to fail if not.
// Note: This is a temporary solution until there is more infrastructure for the engine to use.
void get_symbol_from_module_table( File symbol_table, u32 symbol_ID, char* symbol_name )
{
	struct Token
	{
		char const* Ptr;
		u32         Len;
	};

	Token tokens[256] = {};
	s32 idx = 0;

	char const* scanner = rcast( char const*, symbol_table.Data );
	u32 left = symbol_table.Size;
	while ( left )
	{
		if ( *scanner == '\n' || *scanner == '\r' )
		{
			++ scanner;
			-- left;
		}
		else
		{
			tokens[idx].Ptr = scanner;
			while ( left && *scanner != '\r' && *scanner != '\n' )
			{
				-- left;
				++ scanner;
				++ tokens[idx].Len;
			}
			++ idx;
		}
	}

	Token& token = tokens[symbol_ID];
	while ( token.Len -- )
	{
		*symbol_name = *token.Ptr;
		++ symbol_name;
		++ token.Ptr;
	}
	*symbol_name = '\0';
}
#endif

#pragma region Settings Exposure
// Exposing specific properties for user configuration in settings

// Returns the current monitor refresh rate.
using GetMonitorRefreshRateFn = u32();
// Sets the monitor refresh rate
// Must be of the compatiable listing for the monitor the window surface is presenting to
using SetMonitorRefreshRateFn = void ( u32 rate_in_hz );

using GetEngineFrameTargetFn = u32 ();
using SetEngineFrameTargetFn = void ( u32 rate_in_hz );

// This module api will be used to manage the editor and game modules from the engine side,
// without the platform layer needing to know about it.

struct BinaryModule
{
	void* OpaqueHandle;
};

using LoadBinaryModuleFn   = BinaryModule ( char const* module_path );
using UnloadBinaryModuleFn = void ( BinaryModule* module );
using GetModuleProcedureFn = void* ( BinaryModule module, char const* symbol );

// The file interface is really just made for the engine to use.
// It will allow for only reading or writting to a file at a time.
// Note: If anything more robust is needed, I'll grab it from the zpl-c library.

using FileCheckExistsFn  = b32 ( Str const file_path );
using FileCloseFn 		 = void ( File* file );
using FileDelete         = b32 ( Str const file_path );
using FileReadContentFn  = b32 ( File* file );
using FileReadStreamFn   = b32 ( File* file, u32 content_size, void* content_memory );
using FileWriteContentFn = u32 ( File* file, u32 content_size, void* content_memory );
using FileWriteStreamFn  = u32 ( File* file, u32 content_size, void* content_memory );
using FileRewindFn       = void ( File* file );

struct ModuleAPI
{
	Str PathRoot;
	Str PathBinaries;

#if Build_Development
	DebugSetPauseRenderingFn* debug_set_pause_rendering;
#endif

	GetMonitorRefreshRateFn* get_monitor_refresh_rate;
	SetMonitorRefreshRateFn* set_monitor_refresh_rate;

	GetEngineFrameTargetFn* get_engine_frame_target;
	SetEngineFrameTargetFn* set_engine_frame_target;

	LoadBinaryModuleFn*   load_binary_module;
	UnloadBinaryModuleFn* unload_binary_module;
	GetModuleProcedureFn* get_module_procedure;

	FileCheckExistsFn*  file_check_exists;  // Checks if a file exists
	FileCloseFn* 		file_close;         // Files successfuly provided to the user are not automatically closed, use this to close them.
	FileDelete*         file_delete;        // Deletes a file from the file system
	FileReadContentFn*  file_read_content;  // Read all content within file
	FileReadStreamFn*   file_read_stream;   // Read next chunk of content within file
	FileRewindFn*       file_rewind;        // Rewinds the file stream to the beginning
	FileWriteContentFn* file_write_content; // Writes content to file (overwrites)
	FileWriteStreamFn*  file_write_stream;  // Appends content to file
};

#pragma endregion Settings Exposure

NS_PLATFORM_END
