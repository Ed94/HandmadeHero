#pragma once

// Joyshock grime wrapper
// JoyShock does not provide a proper c-linkage definition for its structs, so we have to push this warning ignore.

#ifdef COMPILER_CLANG
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#elif COMPILER_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4820 )
#endif

#include "dependencies/JoyShockLibrary/JoyShockLibrary.h"

#ifdef COMPILER_CLANG
#	pragma clang diagnostic pop
#elif COMPILER_MSVC
#	pragma warning( pop )
#endif
