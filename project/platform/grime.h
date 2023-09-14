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
#	ifndef SYSTEM_WINDOWS
#		define SYSTEM_WINDOWS 1
#	endif
#elif defined( __APPLE__ ) && defined( __MACH__ )
#	ifndef SYSTEM_OSX
#		define SYSTEM_OSX 1
#	endif
#	ifndef SYSTEM_MACOS
#		define SYSTEM_MACOS 1
#	endif
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
#		ifndef SYSTEM_IOS
#			define SYSTEM_IOS 1
#		endif
#	endif
#elif defined( __unix__ )
#	ifndef SYSTEM_UNIX
#		define GEN_SYSTEM_UNIX 1
#	endif
#	if defined( ANDROID ) || defined( __ANDROID__ )
#		ifndef SYSTEM_ANDROID
#			define SYSTEM_ANDROID 1
#		endif
#		ifndef SYSTEM_LINUX
#			define SYSTEM_LINUX 1
#		endif
#	elif defined( __linux__ )
#		ifndef SYSTEM_LINUX
#			define SYSTEM_LINUX 1
#		endif
#	else
#		error This UNIX operating system is not supported
#	endif
#else
#	error This operating system is not supported
#endif

/* Platform compiler */

#if defined( _MSC_VER )
#	define COMPILER_MSVC 1
#elif defined( __clang__ )
#	define COMPILER_CLANG 1
#else
#	error Unknown compiler
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

#	if defined( SYSTEM_WINDOWS )
#		include <intrin.h>
#	endif

#pragma endregion Mandatory Includes
