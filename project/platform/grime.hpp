#pragma once

#pragma region Platform Detection

/* Platform architecture */

#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( _M_X64 ) || defined( __64BIT__ ) || defined( __powerpc64__ ) || defined( __ppc64__ ) || defined( __aarch64__ )
#	ifndef ARCH_64_BIT
#		define ARCH_64_BIT 1
#	endif
#else
#	error A 32-bit architecture is not supported
#endif

/* Platform OS */

#if defined( _WIN32 ) || defined( _WIN64 )
#	ifndef System_Windows
#		define System_Windows 1
#	endif
#elif defined( __APPLE__ ) && defined( __MACH__ )
#	ifndef System_OSX
#		define System_OSX 1
#	endif
#	ifndef System_MacOS
#		define System_MacOS 1
#	endif
#elif defined( __unix__ )
#	ifndef System_Unix
#		define System_Unix 1
#	endif
#	if defined( ANDROID ) || defined( __ANDROID__ )
#		ifndef System_Android
#			define System_Android 1
#		endif
#		ifndef SYSTEM_LINUX
#			define System_Linux 1
#		endif
#	elif defined( __linux__ )
#		ifndef System_Linux
#			define System_linux 1
#		endif
#	else
#		error This UNIX operating system is not supported
#	endif
#else
#	error This operating system is not supported
#endif

/* Platform compiler */

#if defined( _MSC_VER )
#	define Compiler_MSVC 1
#elif defined( __clang__ )
#	define Compiler_Clang 1
#else
#	error "Unknown compiler"
#endif

#if defined( __has_attribute )
#	define HAS_ATTRIBUTE( attribute ) __has_attribute( attribute )
#else
#	define HAS_ATTRIBUTE( attribute ) ( 0 )
#endif

#pragma endregion Platform Detection

#pragma region Mandatory Includes

#	include <stdarg.h>
#	include <stddef.h>

#	if defined( System_Windows )
#		include <intrin.h>
#	endif

#pragma endregion Mandatory Includes
