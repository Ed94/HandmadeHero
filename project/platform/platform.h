/*
	Platform abstraction layer for the project.
	Services the platform provides to the engine & game.
*/

#pragma once
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
	u32   Size;
	void* Data;
};

void              debug_file_free_content ( Debug_FileContent* file_content );
Debug_FileContent debug_file_read_content ( char* file_path );
b32               debug_file_write_content( char* file_path, u32 content_size, void* content_memory );
#endif

NS_PLATFORM_END
