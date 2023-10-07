/*
	Platform abstraction layer for the project.
	Services the platform provides to the engine & game.

	This should be the only file the engine or game layer can include related to the platform layer.
	(Public Interface essentially...)
*/

#pragma once

#if INTELLISENSE_DIRECTIVES
// TODO(Ed) : REMOVE THESE WHEN CASEY GETS TO THEM
#include <math.h> // TODO : Implement math ourselves
#include <stdio.h> // TODO : Implement output logging ourselves

#include "grime.hpp"
#include "macros.hpp"
#include "generics.hpp"
#include "math_constants.hpp"
#include "types.hpp"
#include "intrinsics.hpp"
#include "strings.hpp"
#include "context.hpp"
#endif

#define NS_PLATFORM_BEGIN namespace platform {
#define NS_PLATFORM_END }

NS_PLATFORM_BEGIN

// On-Demand platform interface.
// Everything exposed here should be based on a feature a game may want to provide a user
// (Example: Letting the user change the refresh-rate of the monitor or the engine's target frame-rate)

#if Build_Development
using DebugSetPauseRenderingFn = void (b32 value);
#endif

struct File
{
	void* opaque_handle;
	Str   path;
	void* data;
	u32   size;
};

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
	void* opaque_handle;
};

using LoadBinaryModuleFn   = BinaryModule ( char const* module_path );
using UnloadBinaryModuleFn = void ( BinaryModule* module );
using GetModuleProcedureFn = void* ( BinaryModule module, char const* symbol );

// The file interface is really just made for the engine to use.
// It will allow for only reading or writting to a file at a time.
// Note: If anything more robust is needed, I'll grab it from the zpl-c library.

// TODO(Ed) : These need to be converted to an async interface.
using FileCheckExistsFn  = b32 ( Str const file_path );
using FileCloseFn 		 = void ( File* file );
using FileDelete         = b32 ( Str const file_path );
using FileReadContentFn  = b32 ( File* file );
using FileReadStreamFn   = b32 ( File* file, u32 content_size, void* content_memory );
using FileWriteContentFn = u32 ( File* file, u32 content_size, void* content_memory );
using FileWriteStreamFn  = u32 ( File* file, u32 content_size, void* content_memory );
using FileRewindFn       = void ( File* file );

using MemoryCopyFn = void( void* dest, u64 src_size, void* src );

using GetWallClockFn = u64();

struct ModuleAPI
{
	Str path_root;
	Str path_binaries;
	Str path_scratch;

#if Build_Development
	DebugSetPauseRenderingFn* debug_set_pause_rendering;
#endif

	GetWallClockFn* get_wall_clock;

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

	MemoryCopyFn* memory_copy;
};

#if Build_Development
void impl_congrats( char const* message );
bool impl_ensure( bool condition, char const* message );
void impl_fatal( char const* message );
#endif

#pragma endregion Settings Exposure

NS_PLATFORM_END
