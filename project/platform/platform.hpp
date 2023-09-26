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

// TODO(Ed) : REMOVE THESE WHEN HE GETS TO THEM
#include <math.h> // TODO : Implement math ourselves
#include <stdio.h> // TODO : Implement output logging ourselves

#include "grime.hpp"
#include "macros.hpp"
#include "generics.hpp"
#include "math_constants.hpp"
#include "types.hpp"

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

struct Debug_FileContent
{
	void* Data;
	u32   Size;
	char  _PAD_[4];
};

using DebugFileFreeContentFn  = void ( Debug_FileContent* file_content );
using DebugFileReadContentFn  = Debug_FileContent ( char const* file_path );
using DebugFileWriteContentFn = b32 ( char const* file_path, u32 content_size, void* content_memory );

using DebugSetPauseRenderingFn = void (b32 value);
#endif

// TODO(Ed) : Implement this later when settings UI is setup.
#pragma region Settings Exposure
// Exposing specific properties for user configuration in settings

// Returns the current monitor refresh rate.
using GetMonitorRefreshRateFn = u32();
// Sets the monitor refresh rate
// Must be of the compatiable listing for the monitor the window surface is presenting to
using SetMonitorRefreshRateFn = void ( u32 rate_in_hz );

using GetEngineFrameTargetFn = u32 ();
using SetEngineFrameTargetFn = void ( u32 rate_in_hz );

struct ModuleAPI
{
#if Build_Development
	DebugFileFreeContentFn*  debug_file_free_content;
	DebugFileReadContentFn*  debug_file_read_content;
	DebugFileWriteContentFn* debug_file_write_content;

	DebugSetPauseRenderingFn* debug_set_pause_rendering;
#endif

	GetMonitorRefreshRateFn* get_monitor_refresh_rate;
	SetMonitorRefreshRateFn* set_monitor_refresh_rate;

	GetEngineFrameTargetFn* get_engine_frame_target;
	SetEngineFrameTargetFn* set_engine_frame_target;
};

#pragma endregion Settings Exposure

NS_PLATFORM_END
