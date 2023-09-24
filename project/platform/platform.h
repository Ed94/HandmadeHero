/*
	Platform abstraction layer for the project.
	Services the platform provides to the engine & game.

	This should be the only file the engine or game layer can include
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

#include "grime.h"
#include "macros.h"
#include "generics.h"
#include "math_constants.h"
#include "types.h"

#define NS_PLATFORM_BEGIN namespace platform {
#define NS_PLATFORM_END }

NS_PLATFORM_BEGIN

#if Build_Debug
/*
	IMPORTANT : These are not for shipping code - they are blocking and the write isn't protected.
*/

struct Debug_FileContent
{
	void* Data;
	u32   Size;
	char  _PAD_[4];
};

void              debug_file_free_content ( Debug_FileContent* file_content );
Debug_FileContent debug_file_read_content ( char const* file_path );
b32               debug_file_write_content( char const* file_path, u32 content_size, void* content_memory );
#endif

NS_PLATFORM_END

// On-Demand platform interface.
// Everything exposed here should be based on a feature a game may want to provide a user
// (Example: Letting the user change the refresh-rate of the monitor or the engine's target frame-rate)

// TODO(Ed) : Implement this later when settings UI is setup.
#pragma region Settings Exposure
// Exposing specific variables for user configuration in settings

// Returns the current monitor refresh rate.
u32 const get_monitor_refresh_rate();

// Sets the monitor refresh rate
// Must be of the compatiable listing for the monitor the window surface is presenting to.
void set_monitor_refresh_rate( u32 rate_in_hz );

u32 const get_engine_frame_rate_target();

void set_engine_frame_rate_target( u32 rate_in_hz );

#pragma endregion Settings Exposure
