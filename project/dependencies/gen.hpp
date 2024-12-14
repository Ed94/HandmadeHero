// This file was generated automatially by gencpp's singleheader.cpp(See: https://github.com/Ed94/gencpp)

#pragma once

#ifdef __clang__
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wunused-const-variable"
#	pragma clang diagnostic ignored "-Wunused-but-set-variable"
#	pragma clang diagnostic ignored "-Wswitch"
#	pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wunknown-pragmas"
#	pragma clang diagnostic ignored "-Wvarargs"
#	pragma clang diagnostic ignored "-Wunused-function"
#	pragma clang diagnostic ignored "-Wbraced-scalar-init"
#   pragma clang diagnostic ignored "-W#pragma-messages"
#	pragma clang diagnostic ignored "-Wstatic-in-inline"
#endif

#ifdef __GNUC__
#	pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunknown-pragmas"
#	pragma GCC diagnostic ignored "-Wcomment"
#	pragma GCC diagnostic ignored "-Wswitch"
#	pragma GCC diagnostic ignored "-Wunused-variable"
#endif
/*
	gencpp: An attempt at "simple" staged metaprogramming for c/c++.

	See Readme.md for more information from the project repository.

	Define GEN_IMPLEMENTATION before including this file in a single compilation unit.

	Public Address:
	https://github.com/Ed94/gencpp  --------------------------------------------------------------.
	|   _____                               _____ _                       _                        |
	|  / ____)                             / ____} |                     | |                       |
	| | / ___  ___ _ __   ___ _ __  _ __  | {___ | |__ _ _, __ _, ___  __| |                       |
	| | |{_  |/ _ \ '_ \ / __} '_ l| '_ l `\___ \| __/ _` |/ _` |/ _ \/ _` |                       |
	| | l__j | ___/ | | | {__; |+l } |+l | ____) | l| (_| | {_| | ___/ (_| |                       |
	|  \_____|\___}_l |_|\___} ,__/| ,__/ (_____/ \__\__/_|\__, |\___}\__,_l                       |
	|     Singleheader       | |   | |                      __} |                                  |
	|                        l_l   l_l                     {___/                                   |
	! ----------------------------------------------------------------------- VERSION: v0.20-Alpha |
	! ============================================================================================ |
	! WARNING: THIS IS AN ALPHA VERSION OF THE LIBRARY, USE AT YOUR OWN DISCRETION                 |
	! NEVER DO CODE GENERATION WITHOUT AT LEAST HAVING CONTENT IN A CODEBASE UNDER VERSION CONTROL |
	! ============================================================================================ /
*/
#if ! defined(GEN_DONT_ENFORCE_GEN_TIME_GUARD) && ! defined(GEN_TIME)
#	error Gen.hpp : GEN_TIME not defined
#endif

//! If its desired to roll your own dependencies, define GEN_ROLL_OWN_DEPENDENCIES before including this file.
// Dependencies are derived from the c-zpl library: https://github.com/zpl-c/zpl
#ifndef GEN_ROLL_OWN_DEPENDENCIES

#pragma region Platform Detection

/* Platform architecture */

#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( _M_X64 ) || defined( __64BIT__ ) || defined( __powerpc64__ ) || defined( __ppc64__ ) || defined( __aarch64__ )
#	ifndef GEN_ARCH_64_BIT
#		define GEN_ARCH_64_BIT 1
#	endif
#else
#	ifndef GEN_ARCH_32_BItxt_StrCaT
#		define GEN_ARCH_32_BIT 1
#	endif
#endif

/* Platform OS */

#if defined( _WIN32 ) || defined( _WIN64 )
#	ifndef GEN_SYSTEM_WINDOWS
#		define GEN_SYSTEM_WINDOWS 1
#	endif
#elif defined( __APPLE__ ) && defined( __MACH__ )
#	ifndef GEN_SYSTEM_OSX
#		define GEN_SYSTEM_OSX 1
#	endif
#	ifndef GEN_SYSTEM_MACOS
#		define GEN_SYSTEM_MACOS 1
#	endif
#elif defined( __unix__ )
#	ifndef GEN_SYSTEM_UNIX
#		define GEN_SYSTEM_UNIX 1
#	endif
#	if defined( ANDROID ) || defined( __ANDROID__ )
#		ifndef GEN_SYSTEM_ANDROID
#			define GEN_SYSTEM_ANDROID 1
#		endif
#		ifndef GEN_SYSTEM_LINUX
#			define GEN_SYSTEM_LINUX 1
#		endif
#	elif defined( __linux__ )
#		ifndef GEN_SYSTEM_LINUX
#			define GEN_SYSTEM_LINUX 1
#		endif
#	elif defined( __FreeBSD__ ) || defined( __FreeBSD_kernel__ )
#		ifndef GEN_SYSTEM_FREEBSD
#			define GEN_SYSTEM_FREEBSD 1
#		endif
#	elif defined( __OpenBSD__ )
#		ifndef GEN_SYSTEM_OPENBSD
#			define GEN_SYSTEM_OPENBSD 1
#		endif
#	elif defined( __EMSCRIPTEN__ )
#		ifndef GEN_SYSTEM_EMSCRIPTEN
#			define GEN_SYSTEM_EMSCRIPTEN 1
#		endif
#	elif defined( __CYGWIN__ )
#		ifndef GEN_SYSTEM_CYGWIN
#			define GEN_SYSTEM_CYGWIN 1
#		endif
#	else
#		error This UNIX operating system is not supported
#	endif
#else
#	error This operating system is not supported
#endif

/* Platform compiler */

#if defined( _MSC_VER )
#	pragma message("Detected MSVC")
// #	define GEN_COMPILER_CLANG 0
#	define GEN_COMPILER_MSVC  1
// #	define GEN_COMPILER_GCC   0
#elif defined( __GNUC__ )
#	pragma message("Detected GCC")
// #	define GEN_COMPILER_CLANG 0
// #	define GEN_COMPILER_MSVC  0
#	define GEN_COMPILER_GCC   1
#elif defined( __clang__ )
#	pragma message("Detected CLANG")
#	define GEN_COMPILER_CLANG 1
// #	define GEN_COMPILER_MSVC  0
// #	define GEN_COMPILER_GCC   0
#else
#	error Unknown compiler
#endif

#if defined( __has_attribute )
#	define GEN_HAS_ATTRIBUTE( attribute ) __has_attribute( attribute )
#else
#	define GEN_HAS_ATTRIBUTE( attribute ) ( 0 )
#endif

#if defined(GEN_GCC_VERSION_CHECK)
#  undef GEN_GCC_VERSION_CHECK
#endif
#if defined(GEN_GCC_VERSION)
#  define GEN_GCC_VERSION_CHECK(major,minor,patch) (GEN_GCC_VERSION >= GEN_VERSION_ENCODE(major, minor, patch))
#else
#  define GEN_GCC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if !defined(GEN_COMPILER_C)
#	ifdef __cplusplus
#		define GEN_COMPILER_C   0
#		define GEN_COMPILER_CPP 1
#	else
#		if defined(__STDC__)
#			define GEN_COMPILER_C   1
#		    define GEN_COMPILER_CPP 0
#		else
            // Fallback for very old C compilers
#			define GEN_COMPILER_C   1
#		    define GEN_COMPILER_CPP 0
#		endif
#   endif
#endif

#if GEN_COMPILER_C
#pragma message("GENCPP: Detected C")
#endif

#pragma endregion Platform Detection

#pragma region Mandatory Includes

#	include <stdarg.h>
#	include <stddef.h>

#	if defined( GEN_SYSTEM_WINDOWS )
#		include <intrin.h>
#	endif

#if GEN_COMPILER_C
#include <assert.h>
#include <stdbool.h>
#endif

#pragma endregion Mandatory Includes

#if GEN_DONT_USE_NAMESPACE || GEN_COMPILER_C
#	if GEN_COMPILER_C
#		define GEN_NS
#		define GEN_NS_BEGIN
#		define GEN_NS_END
#	else
#		define GEN_NS              ::
#		define GEN_NS_BEGIN
#		define GEN_NS_END
#	endif
#else
#	define GEN_NS              gen::
#	define GEN_NS_BEGIN        namespace gen {
#	define GEN_NS_END          }
#endif

GEN_NS_BEGIN

#pragma region Macros

#if GEN_COMPILER_MSVC
    #ifdef GEN_DYN_LINK
        #ifdef GEN_DYN_EXPORT
            #define GEN_API __declspec(dllexport)
        #else
            #define GEN_API __declspec(dllimport)
        #endif
    #else
        #define GEN_API  // Empty for static builds
    #endif
#else
    #ifdef GEN_DYN_LINK
        #define GEN_API __attribute__((visibility("default")))
    #else
        #define GEN_API  // Empty for static builds
    #endif
#endif

#ifndef global
#define global        static    // Global variables
#endif
#ifndef internal
#define internal      static    // Internal linkage
#endif
#ifndef local_persist
#define local_persist static    // Local Persisting variables
#endif

#ifndef bit
#define bit( Value )                             ( 1 << Value )
#define bitfield_is_equal( Type, Field, Mask ) ( (scast(Type, Mask) & scast(Type, Field)) == scast(Type, Mask) )
#endif

// Mainly intended for forcing the base library to utilize only C-valid constructs or type coercion
#ifndef GEN_C_LIKE_CPP
#define GEN_C_LIKE_CPP 0
#endif

#if GEN_COMPILER_CPP
#	ifndef cast
#	define cast( type, value ) (tmpl_cast<type>( value ))
#	endif
#else
#	ifndef cast
#	define cast( type, value )  ( (type)(value) )
#	endif
#endif

#if GEN_COMPILER_CPP
#	ifndef ccast
#	define ccast( type, value ) ( const_cast< type >( (value) ) )
#	endif
#	ifndef pcast
#	define pcast( type, value ) ( * reinterpret_cast< type* >( & ( value ) ) )
#	endif
#	ifndef rcast
#	define rcast( type, value ) reinterpret_cast< type >( value )
#	endif
#	ifndef scast
#	define scast( type, value ) static_cast< type >( value )
#	endif
#else
#	ifndef ccast
#	define ccast( type, value ) ( (type)(value) )
#	endif
#	ifndef pcast
#	define pcast( type, value ) ( * (type*)(& value) )
#	endif
#	ifndef rcast
#	define rcast( type, value ) ( (type)(value) )
#	endif
#	ifndef scast
#	define scast( type, value ) ( (type)(value) )
#	endif
#endif

#ifndef stringize
#define stringize_va( ... ) #__VA_ARGS__
#define stringize( ... )    stringize_va( __VA_ARGS__ )
#endif

#ifndef do_once
#define do_once()                                                               \
    static int __do_once_counter_##__LINE__ = 0;                                \
    for(; __do_once_counter_##__LINE__ != 1; __do_once_counter_##__LINE__ = 1 ) \

#define do_once_defer( expression )                                                          \
    static int __do_once_counter_##__LINE__ = 0;                                             \
    for(; __do_once_counter_##__LINE__ != 1; __do_once_counter_##__LINE__ = 1, (expression)) \

#define do_once_start      \
	do                     \
	{                      \
		local_persist      \
		bool done = false; \
		if ( done )        \
			break;         \
		done = true;

#define do_once_end        \
	}                      \
	while(0);
#endif

#ifndef labeled_scope_start
#define labeled_scope_start if ( false ) {
#define labeled_scope_end   }
#endif

#ifndef         compiler_decorated_func_name
#   ifdef COMPILER_CLANG
#       define  compiler_decorated_func_name __PRETTY_NAME__
#   elif defined(COMPILER_MSVC)
#   	define compiler_decorated_func_name __FUNCDNAME__
#   endif
#endif

#ifndef num_args_impl

// This is essentially an arg couneter version of GEN_SELECT_ARG macros
// See section : _Generic function overloading for that usage (explains this heavier case)

#define num_args_impl( _0,                                 \
		_1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10,   \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
		_21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
		_31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
		_41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
		_51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
		_61, _62, _63, _64, _65, _66, _67, _68, _69, _70,  \
		_71, _72, _73, _74, _75, _76, _77, _78, _79, _80,  \
		_81, _82, _83, _84, _85, _86, _87, _88, _89, _90,  \
		_91, _92, _93, _94, _95, _96, _97, _98, _99, _100, \
		N, ...                                             \
	) N

// ## deletes preceding comma if _VA_ARGS__ is empty (GCC, Clang)
#define num_args(...)                            \
	num_args_impl(_, ## __VA_ARGS__,             \
		100, 99, 98, 97, 96, 95, 94, 93, 92, 91, \
		 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, \
		 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, \
		 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, \
		 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, \
		 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
		 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, \
		 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
		 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
		 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, \
		 0                                       \
	)
#endif

#ifndef clamp
#define clamp( x, lower, upper )      min( max( ( x ), ( lower ) ), ( upper ) )
#endif
#ifndef count_of
#define count_of( x )                 ( ( size_of( x ) / size_of( 0 [ x ] ) ) / ( ( ssize )( ! ( size_of( x ) % size_of( 0 [ x ] ) ) ) ) )
#endif
#ifndef is_between
#define is_between( x, lower, upper ) ( ( ( lower ) <= ( x ) ) && ( ( x ) <= ( upper ) ) )
#endif
#ifndef size_of
#define size_of( x )                  ( ssize )( sizeof( x ) )
#endif

#ifndef max
#define max( a, b ) ( (a > b) ? (a) : (b) )
#endif
#ifndef min
#define min( a, b ) ( (a < b) ? (a) : (b) )
#endif

#if GEN_COMPILER_MSVC || GEN_COMPILER_TINYC
#	define offset_of( Type, element ) ( ( GEN_NS( ssize ) ) & ( ( ( Type* )0 )->element ) )
#else
#	define offset_of( Type, element ) __builtin_offsetof( Type, element )
#endif

#ifndef        forceinline
#	if GEN_COMPILER_MSVC
#		define forceinline __forceinline
#		define neverinline __declspec( noinline )
#	elif GEN_COMPILER_GCC
#		define forceinline inline __attribute__((__always_inline__))
#		define neverinline __attribute__( ( __noinline__ ) )
#	elif GEN_COMPILER_CLANG
#	if __has_attribute(__always_inline__)
#		define forceinline inline __attribute__((__always_inline__))
#		define neverinline __attribute__( ( __noinline__ ) )
#	else
#		define forceinline
#		define neverinline
#	endif
#	else
#		define forceinline
#		define neverinline
#	endif
#endif

#ifndef        neverinline
#	if GEN_COMPILER_MSVC
#		define neverinline __declspec( noinline )
#	elif GEN_COMPILER_GCC
#		define neverinline __attribute__( ( __noinline__ ) )
#	elif GEN_COMPILER_CLANG
#	if __has_attribute(__always_inline__)
#		define neverinline __attribute__( ( __noinline__ ) )
#	else
#		define neverinline
#	endif
#	else
#		define neverinline
#	endif
#endif

#if GEN_COMPILER_C
#ifndef static_assert
#undef  static_assert
    #if GEN_COMPILER_C && __STDC_VERSION__ >= 201112L
        #define static_assert(condition, message) _Static_assert(condition, message)
    #else
        #define static_assert(condition, message) typedef char static_assertion_##__LINE__[(condition)?1:-1]
	#endif
#endif
#endif

#if GEN_COMPILER_CPP
// Already Defined
#elif GEN_COMPILER_C && __STDC_VERSION__ >= 201112L
#	define thread_local _Thread_local
#elif GEN_COMPILER_MSVC
#	define thread_local __declspec(thread)
#elif GEN_COMPILER_CLANG
#	define thread_local __thread
#else
#	error "No thread local support"
#endif

#if ! defined(typeof) && (!GEN_COMPILER_C || __STDC_VERSION__ < 202311L)
#	if ! GEN_COMPILER_C
#		define typeof decltype
#	elif defined(_MSC_VER)
#		define typeof(x) __typeof__(x)
#	elif defined(__GNUC__) || defined(__clang__)
#		define typeof(x) __typeof__(x)
#	else
#		error "Compiler not supported"
#	endif
#endif

#ifndef GEN_API_C_BEGIN
#	if GEN_COMPILER_C
#		define GEN_API_C_BEGIN
#		define GEN_API_C_END
#	else
#		define GEN_API_C_BEGIN extern "C" {
#		define GEN_API_C_END   }
#	endif
#endif

#if GEN_COMPILER_C
#	if __STDC_VERSION__ >= 202311L
#		define enum_underlying(type) : type
#	else
#		define enum_underlying(type)
#   endif
#else
#	define enum_underlying(type) : type
#endif

#if GEN_COMPILER_C
#	ifndef nullptr
#		define nullptr NULL
#	endif

#	ifndef GEN_REMOVE_PTR
#		define GEN_REMOVE_PTR(type) typeof(* ( (type) NULL) )
#	endif
#endif

#if ! defined(GEN_PARAM_DEFAULT) && GEN_COMPILER_CPP
#	define GEN_PARAM_DEFAULT = {}
#else
#	define GEN_PARAM_DEFAULT
#endif

#if GEN_COMPILER_CPP
    #define struct_init(type, value) {value}
#else
    #define struct_init(type, value) {value}
#endif

#if 0
#ifndef GEN_OPTIMIZE_MAPPINGS_BEGIN
#	define GEN_OPTIMIZE_MAPPINGS_BEGIN _pragma(optimize("gt", on))
#	define GEN_OPITMIZE_MAPPINGS_END   _pragma(optimize("", on))
#endif
#else
#	define GEN_OPTIMIZE_MAPPINGS_BEGIN
#	define GEN_OPITMIZE_MAPPINGS_END
#endif

#pragma endregion Macros

#pragma region Basic Types

#define GEN_U8_MIN 0u
#define GEN_U8_MAX 0xffu
#define GEN_I8_MIN ( -0x7f - 1 )
#define GEN_I8_MAX 0x7f

#define GEN_U16_MIN 0u
#define GEN_U16_MAX 0xffffu
#define GEN_I16_MIN ( -0x7fff - 1 )
#define GEN_I16_MAX 0x7fff

#define GEN_U32_MIN 0u
#define GEN_U32_MAX 0xffffffffu
#define GEN_I32_MIN ( -0x7fffffff - 1 )
#define GEN_I32_MAX 0x7fffffff

#define GEN_U64_MIN 0ull
#define GEN_U64_MAX 0xffffffffffffffffull
#define GEN_I64_MIN ( -0x7fffffffffffffffll - 1 )
#define GEN_I64_MAX 0x7fffffffffffffffll

#if defined( GEN_ARCH_32_BIT )
#	define GEN_USIZE_MIN GEN_U32_MIN
#	define GEN_USIZE_MAX GEN_U32_MAX
#	define GEN_ISIZE_MIN GEN_S32_MIN
#	define GEN_ISIZE_MAX GEN_S32_MAX
#elif defined( GEN_ARCH_64_BIT )
#	define GEN_USIZE_MIN GEN_U64_MIN
#	define GEN_USIZE_MAX GEN_U64_MAX
#	define GEN_ISIZE_MIN GEN_I64_MIN
#	define GEN_ISIZE_MAX GEN_I64_MAX
#else
#	error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define GEN_F32_MIN 1.17549435e-38f
#define GEN_F32_MAX 3.40282347e+38f
#define GEN_F64_MIN 2.2250738585072014e-308
#define GEN_F64_MAX 1.7976931348623157e+308

#if defined( GEN_COMPILER_MSVC )
#	if _MSC_VER < 1300
typedef unsigned char  u8;
typedef signed   char  s8;
typedef unsigned short u16;
typedef signed   short s16;
typedef unsigned int   u32;
typedef signed   int   s32;
#    else
typedef unsigned __int8  u8;
typedef signed   __int8  s8;
typedef unsigned __int16 u16;
typedef signed   __int16 s16;
typedef unsigned __int32 u32;
typedef signed   __int32 s32;
#    endif
typedef unsigned __int64 u64;
typedef signed   __int64 s64;
#else
#	include <stdint.h>

typedef uint8_t  u8;
typedef int8_t   s8;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint32_t u32;
typedef int32_t  s32;
typedef uint64_t u64;
typedef int64_t  s64;
#endif

static_assert( sizeof( u8 )  == sizeof( s8 ),  "sizeof(u8) != sizeof(s8)" );
static_assert( sizeof( u16 ) == sizeof( s16 ), "sizeof(u16) != sizeof(s16)" );
static_assert( sizeof( u32 ) == sizeof( s32 ), "sizeof(u32) != sizeof(s32)" );
static_assert( sizeof( u64 ) == sizeof( s64 ), "sizeof(u64) != sizeof(s64)" );

static_assert( sizeof( u8 )  == 1, "sizeof(u8) != 1" );
static_assert( sizeof( u16 ) == 2, "sizeof(u16) != 2" );
static_assert( sizeof( u32 ) == 4, "sizeof(u32) != 4" );
static_assert( sizeof( u64 ) == 8, "sizeof(u64) != 8" );

typedef size_t    usize;
typedef ptrdiff_t ssize;

static_assert( sizeof( usize ) == sizeof( ssize ), "sizeof(usize) != sizeof(ssize)" );

// NOTE: (u)zpl_intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
#if defined( _WIN64 )
typedef signed __int64   sptr;
typedef unsigned __int64 uptr;
#elif defined( _WIN32 )
// NOTE; To mark types changing their size, e.g. zpl_intptr
#	ifndef _W64
#		if ! defined( __midl ) && ( defined( _X86_ ) || defined( _M_IX86 ) ) && _MSC_VER >= 1300
#			define _W64 __w64
#		else
#			define _W64
#		endif
#	endif
typedef _W64 signed int   sptr;
typedef _W64 unsigned int uptr;
#else
typedef uintptr_t uptr;
typedef intptr_t  sptr;
#endif

static_assert( sizeof( uptr ) == sizeof( sptr ), "sizeof(uptr) != sizeof(sptr)" );

typedef float  f32;
typedef double f64;

static_assert( sizeof( f32 ) == 4, "sizeof(f32) != 4" );
static_assert( sizeof( f64 ) == 8, "sizeof(f64) != 8" );

typedef s8  b8;
typedef s16 b16;
typedef s32 b32;

typedef void*       mem_ptr;
typedef void const* mem_ptr_const ;

#if GEN_COMPILER_CPP
template<typename Type> uptr to_uptr( Type* ptr ) { return (uptr)ptr; }
template<typename Type> sptr to_sptr( Type* ptr ) { return (sptr)ptr; }

template<typename Type> mem_ptr       to_mem_ptr      ( Type ptr ) { return (mem_ptr)      ptr; }
template<typename Type> mem_ptr_const to_mem_ptr_const( Type ptr ) { return (mem_ptr_const)ptr; }
#else
#define to_uptr( ptr ) ((uptr)(ptr))
#define to_sptr( ptr ) ((sptr)(ptr))

#define to_mem_ptr( ptr)       ((mem_ptr)ptr)
#define to_mem_ptr_const( ptr) ((mem_ptr)ptr)
#endif

#pragma endregion Basic Types

#pragma region Debug

#if GEN_BUILD_DEBUG
#	if defined( GEN_COMPILER_MSVC )
#		if _MSC_VER < 1300
#pragma message("GEN_BUILD_DEBUG: __asm int 3")
#			define GEN_DEBUG_TRAP() __asm int 3 /* Trap to debugger! */
#		else
#pragma message("GEN_BUILD_DEBUG: __debugbreak()")
#			define GEN_DEBUG_TRAP() __debugbreak()
#		endif
#	elif defined( GEN_COMPILER_TINYC )
#		define GEN_DEBUG_TRAP() process_exit( 1 )
#	else
#		define GEN_DEBUG_TRAP() __builtin_trap()
#	endif
#else
#pragma message("GEN_BUILD_DEBUG: omitted")
#	define GEN_DEBUG_TRAP()
#endif

#define GEN_ASSERT( cond ) GEN_ASSERT_MSG( cond, NULL )

#define GEN_ASSERT_MSG( cond, msg, ... )                                                              \
	do                                                                                                \
	{                                                                                                 \
		if ( ! ( cond ) )                                                                             \
		{                                                                                             \
			assert_handler( #cond, __FILE__, __func__, scast( s64, __LINE__ ), msg, ##__VA_ARGS__ );  \
			GEN_DEBUG_TRAP();                                                                         \
		}                                                                                             \
	} while ( 0 )

#define GEN_ASSERT_NOT_NULL( ptr ) GEN_ASSERT_MSG( ( ptr ) != NULL, #ptr " must not be NULL" )

// NOTE: Things that shouldn't happen with a message!
#define GEN_PANIC( msg, ... ) GEN_ASSERT_MSG( 0, msg, ##__VA_ARGS__ )

#if GEN_BULD_DEBUG
	#define GEN_FATAL( ... )                               \
	do                                                     \
	{                                                      \
		local_persist thread_local                         \
		char buf[GEN_PRINTF_MAXLEN] = { 0 };               \
		                                                   \
		c_str_fmt(buf, GEN_PRINTF_MAXLEN, __VA_ARGS__);    \
		GEN_PANIC(buf);                                    \
	}                                                      \
	while (0)
#else

#	define GEN_FATAL( ... )                  \
	do                                       \
	{                                        \
		c_str_fmt_out_err( __VA_ARGS__ );    \
		GEN_DEBUG_TRAP();                    \
		process_exit(1);                     \
	}                                        \
	while (0)
#endif

void assert_handler( char const* condition, char const* file, char const* function, s32 line, char const* msg, ... );
s32  assert_crash( char const* condition );
void process_exit( u32 code );

#pragma endregion Debug

#pragma region Memory

#define kilobytes( x ) (          ( x ) * ( s64 )( 1024 ) )
#define megabytes( x ) ( kilobytes( x ) * ( s64 )( 1024 ) )
#define gigabytes( x ) ( megabytes( x ) * ( s64 )( 1024 ) )
#define terabytes( x ) ( gigabytes( x ) * ( s64 )( 1024 ) )

#define GEN__ONES          ( scast( GEN_NS usize, - 1) / GEN_U8_MAX )
#define GEN__HIGHS         ( GEN__ONES * ( GEN_U8_MAX / 2 + 1 ) )
#define GEN__HAS_ZERO( x ) ( ( ( x ) - GEN__ONES ) & ~( x ) & GEN__HIGHS )

template< class Type >
void swap( Type& a, Type& b )
{
	Type tmp = a;
	a = b;
	b = tmp;
}

//! Checks if value is power of 2.
b32 is_power_of_two( ssize x );

//! Aligns address to specified alignment.
void* align_forward( void* ptr, ssize alignment );

//! Aligns value to a specified alignment.
s64 align_forward_by_value( s64 value, ssize alignment );

//! Moves pointer forward by bytes.
void* pointer_add( void* ptr, ssize bytes );

//! Moves pointer forward by bytes.
void const* pointer_add_const( void const* ptr, ssize bytes );

//! Calculates difference between two addresses.
ssize pointer_diff( void const* begin, void const* end );

//! Copy non-overlapping memory from source to destination.
void* mem_copy( void* dest, void const* source, ssize size );

//! Search for a constant value within the size limit at memory location.
void const* mem_find( void const* data, u8 byte_value, ssize size );

//! Copy memory from source to destination.
void* mem_move( void* dest, void const* source, ssize size );

//! Set constant value at memory location with specified size.
void* mem_set( void* data, u8 byte_value, ssize size );

//! @param ptr Memory location to clear up.
//! @param size The size to clear up with.
void zero_size( void* ptr, ssize size );

//! Clears up an item.
#define zero_item( t ) zero_size( ( t ), size_of( *( t ) ) )    // NOTE: Pass pointer of struct

//! Clears up an array.
#define zero_array( a, count ) zero_size( ( a ), size_of( *( a ) ) * count )

enum AllocType : u8
{
	EAllocation_ALLOC,
	EAllocation_FREE,
	EAllocation_FREE_ALL,
	EAllocation_RESIZE,
};

typedef void*(AllocatorProc)( void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags );

struct AllocatorInfo
{
	AllocatorProc* Proc;
	void*          Data;
};

enum AllocFlag
{
	ALLOCATOR_FLAG_CLEAR_TO_ZERO = bit( 0 ),
};

#ifndef GEN_DEFAULT_MEMORY_ALIGNMENT
#	define GEN_DEFAULT_MEMORY_ALIGNMENT ( 2 * size_of( void* ) )
#endif

#ifndef GEN_DEFAULT_ALLOCATOR_FLAGS
#	define GEN_DEFAULT_ALLOCATOR_FLAGS ( ALLOCATOR_FLAG_CLEAR_TO_ZERO )
#endif

//! Allocate memory with default alignment.
void* alloc( AllocatorInfo a, ssize size );

//! Allocate memory with specified alignment.
void* alloc_align( AllocatorInfo a, ssize size, ssize alignment );

//! Free allocated memory.
void allocator_free( AllocatorInfo a, void* ptr );

//! Free all memory allocated by an allocator.
void free_all( AllocatorInfo a );

//! Resize an allocated memory.
void* resize( AllocatorInfo a, void* ptr, ssize old_size, ssize new_size );

//! Resize an allocated memory with specified alignment.
void* resize_align( AllocatorInfo a, void* ptr, ssize old_size, ssize new_size, ssize alignment );

//! Allocate memory for an item.
#define alloc_item( allocator_, Type ) ( Type* )alloc( allocator_, size_of( Type ) )

//! Allocate memory for an array of items.
#define alloc_array( allocator_, Type, count ) ( Type* )alloc( allocator_, size_of( Type ) * ( count ) )

/* heap memory analysis tools */
/* define GEN_HEAP_ANALYSIS to enable this feature */
/* call zpl_heap_stats_init at the beginning of the entry point */
/* you can call zpl_heap_stats_check near the end of the execution to validate any possible leaks */
void  heap_stats_init( void );
ssize heap_stats_used_memory( void );
ssize heap_stats_alloc_count( void );
void  heap_stats_check( void );

//! Allocate/Resize memory using default options.

//! Use this if you don't need a "fancy" resize allocation
void* default_resize_align( AllocatorInfo a, void* ptr, ssize old_size, ssize new_size, ssize alignment );

void* heap_allocator_proc( void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags );

//! The heap allocator backed by operating system's memory manager.
constexpr AllocatorInfo heap( void ) { AllocatorInfo allocator = { heap_allocator_proc, nullptr }; return allocator; }

//! Helper to allocate memory using heap allocator.
#define malloc( sz ) alloc( heap(), sz )

//! Helper to free memory allocated by heap allocator.
#define mfree( ptr ) free( heap(), ptr )

struct VirtualMemory
{
	void*  data;
	ssize size;
};

//! Initialize virtual memory from existing data.
VirtualMemory vm_from_memory( void* data, ssize size );

//! Allocate virtual memory at address with size.

//! @param addr The starting address of the region to reserve. If NULL, it lets operating system to decide where to allocate it.
//! @param size The size to serve.
VirtualMemory vm_alloc( void* addr, ssize size );

//! Release the virtual memory.
b32 vm_free( VirtualMemory vm );

//! Trim virtual memory.
VirtualMemory vm_trim( VirtualMemory vm, ssize lead_size, ssize size );

//! Purge virtual memory.
b32 vm_purge( VirtualMemory vm );

//! Retrieve VM's page size and alignment.
ssize virtual_memory_page_size( ssize* alignment_out );

#pragma region Arena
struct Arena;

AllocatorInfo arena_allocator_info( Arena* arena );

// Remove static keyword and rename allocator_proc
void* arena_allocator_proc(void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags);

// Add these declarations after the Arena struct
Arena arena_init_from_allocator(AllocatorInfo backing, ssize size);
Arena arena_init_from_memory   ( void* start, ssize size );

Arena arena_init_sub      (Arena* parent, ssize size);
ssize arena_alignment_of  (Arena* arena, ssize alignment);
void  arena_check         (Arena* arena);
void  arena_free          (Arena* arena);
ssize arena_size_remaining(Arena* arena, ssize alignment);

struct Arena
{
	AllocatorInfo Backing;
	void*         PhysicalStart;
	ssize         TotalSize;
	ssize         TotalUsed;
	ssize         TempCount;

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
#pragma region Member Mapping
	forceinline operator AllocatorInfo() { return arena_allocator_info(this); }

	forceinline static void* allocator_proc( void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags ) { return arena_allocator_proc( allocator_data, type, size, alignment, old_memory, old_size, flags ); }
	forceinline static Arena init_from_memory( void* start, ssize size )                                                                                      { return arena_init_from_memory( start, size ); }
	forceinline static Arena init_from_allocator( AllocatorInfo backing, ssize size )                                                                         { return arena_init_from_allocator( backing, size ); }
	forceinline static Arena init_sub( Arena& parent, ssize size )                                                                                            { return arena_init_from_allocator( parent.Backing, size ); }
	forceinline        ssize alignment_of( ssize alignment )                                                                                                  { return arena_alignment_of(this, alignment); }
	forceinline        void  free()                                                                                                                           { return arena_free(this);  }
	forceinline        ssize size_remaining( ssize alignment )                                                                                                { return arena_size_remaining(this, alignment); }

// This id is defined by Unreal for asserts
#pragma push_macro("check")
#undef check
	forceinline void check() { arena_check(this); }
#pragma pop_macro("check")

#pragma endregion Member Mapping
#endif
};

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
forceinline AllocatorInfo allocator_info(Arena& arena )                 { return arena_allocator_info(& arena); }
forceinline Arena         init_sub      (Arena& parent, ssize size)     { return arena_init_sub( & parent, size); }
forceinline ssize         alignment_of  (Arena& arena, ssize alignment) { return arena_alignment_of( & arena, alignment); }
forceinline void          free          (Arena& arena)                  { return arena_free(& arena); }
forceinline ssize         size_remaining(Arena& arena, ssize alignment) { return arena_size_remaining(& arena, alignment); }

// This id is defined by Unreal for asserts
#pragma push_macro("check")
#undef check
forceinline void check(Arena& arena) { return arena_check(& arena); };
#pragma pop_macro("check")
#endif


inline
AllocatorInfo arena_allocator_info( Arena* arena ) {
	GEN_ASSERT(arena != nullptr);
	AllocatorInfo info = { arena_allocator_proc, arena };
	return info;
}

inline
Arena arena_init_from_memory( void* start, ssize size )
{
	Arena arena = {
		{ nullptr, nullptr },
		start,
		size,
		0,
		0
	};
	return arena;
}

inline
Arena arena_init_from_allocator(AllocatorInfo backing, ssize size) {
	Arena result = {
		backing,
		alloc(backing, size),
		size,
		0,
		0
	};
	return result;
}

inline
Arena arena_init_sub(Arena* parent, ssize size) {
	GEN_ASSERT(parent != nullptr);
	return arena_init_from_allocator(parent->Backing, size);
}

inline
ssize arena_alignment_of(Arena* arena, ssize alignment)
{
	GEN_ASSERT(arena != nullptr);
	ssize alignment_offset, result_pointer, mask;
	GEN_ASSERT(is_power_of_two(alignment));

	alignment_offset = 0;
	result_pointer  = (ssize)arena->PhysicalStart + arena->TotalUsed;
	mask            = alignment - 1;

	if (result_pointer & mask)
	alignment_offset = alignment - (result_pointer & mask);

	return alignment_offset;
}

inline
void arena_check(Arena* arena)
{
    GEN_ASSERT(arena != nullptr );
    GEN_ASSERT(arena->TempCount == 0);
}

inline
void arena_free(Arena* arena)
{
	GEN_ASSERT(arena != nullptr);
	if (arena->Backing.Proc)
	{
		allocator_free(arena->Backing, arena->PhysicalStart);
		arena->PhysicalStart = nullptr;
	}
}

inline
ssize arena_size_remaining(Arena* arena, ssize alignment)
{
	GEN_ASSERT(arena != nullptr);
	ssize result = arena->TotalSize - (arena->TotalUsed + arena_alignment_of(arena, alignment));
	return result;
}
#pragma endregion Arena

#pragma region FixedArena
template<s32 Size>
struct FixedArena;

template<s32 Size> FixedArena<Size> fixed_arena_init();
template<s32 Size> AllocatorInfo    fixed_arena_allocator_info(FixedArena<Size>* fixed_arena );
template<s32 Size> ssize            fixed_arena_size_remaining(FixedArena<Size>* fixed_arena, ssize alignment);
template<s32 Size> void             fixed_arena_free(FixedArena<Size>* fixed_arena);

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
template<s32 Size> AllocatorInfo    allocator_info( FixedArena<Size>& fixed_arena )                { return allocator_info(& fixed_arena); }
template<s32 Size> ssize            size_remaining(FixedArena<Size>& fixed_arena, ssize alignment) { return size_remaining( & fixed_arena, alignment); }
#endif

// Just a wrapper around using an arena with memory associated with its scope instead of from an allocator.
// Used for static segment or stack allocations.
template< s32 Size >
struct FixedArena
{
	char  memory[Size];
	Arena arena;

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
#pragma region Member Mapping
	forceinline operator AllocatorInfo() { return fixed_arena_allocator_info(this); }

	forceinline static FixedArena init()                          { FixedArena result; fixed_arena_init<Size>(result); return result; }
	forceinline ssize             size_remaining(ssize alignment) { fixed_arena_size_remaining(this, alignment); }
#pragma endregion Member Mapping
#endif
};

template<s32 Size> inline
AllocatorInfo fixed_arena_allocator_info( FixedArena<Size>* fixed_arena ) {
	GEN_ASSERT(fixed_arena);
	return { arena_allocator_proc, & fixed_arena->arena };
}

template<s32 Size> inline
void fixed_arena_init(FixedArena<Size>* result) {
    zero_size(& result->memory[0], Size);
    result->arena = arena_init_from_memory(& result->memory[0], Size);
}

template<s32 Size> inline
void fixed_arena_free(FixedArena<Size>* fixed_arena) {
	arena_free( & fixed_arena->arena);
}

template<s32 Size> inline
ssize fixed_arena_size_remaining(FixedArena<Size>* fixed_arena, ssize alignment) {
    return size_remaining(fixed_arena->arena, alignment);
}

using FixedArena_1KB   = FixedArena< kilobytes( 1 ) >;
using FixedArena_4KB   = FixedArena< kilobytes( 4 ) >;
using FixedArena_8KB   = FixedArena< kilobytes( 8 ) >;
using FixedArena_16KB  = FixedArena< kilobytes( 16 ) >;
using FixedArena_32KB  = FixedArena< kilobytes( 32 ) >;
using FixedArena_64KB  = FixedArena< kilobytes( 64 ) >;
using FixedArena_128KB = FixedArena< kilobytes( 128 ) >;
using FixedArena_256KB = FixedArena< kilobytes( 256 ) >;
using FixedArena_512KB = FixedArena< kilobytes( 512 ) >;
using FixedArena_1MB   = FixedArena< megabytes( 1 ) >;
using FixedArena_2MB   = FixedArena< megabytes( 2 ) >;
using FixedArena_4MB   = FixedArena< megabytes( 4 ) >;
#pragma endregion FixedArena

#pragma region Pool
struct Pool;

void* pool_allocator_proc(void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags);

Pool          pool_init(AllocatorInfo backing, ssize num_blocks, ssize block_size);
Pool          pool_init_align(AllocatorInfo backing, ssize num_blocks, ssize block_size, ssize block_align);
AllocatorInfo pool_allocator_info(Pool* pool);
void          pool_clear(Pool* pool);
void          pool_free(Pool* pool);

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
AllocatorInfo allocator_info(Pool& pool) { return pool_allocator_info(& pool); }
void          clear(Pool& pool)          { return pool_clear(& pool); }
void          free(Pool& pool)           { return pool_free(& pool); }
#endif

struct Pool
{
	AllocatorInfo Backing;
	void*         PhysicalStart;
	void*         FreeList;
	ssize         BlockSize;
	ssize         BlockAlign;
	ssize         TotalSize;
	ssize         NumBlocks;

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
#pragma region Member Mapping
    forceinline operator AllocatorInfo() { return pool_allocator_info(this); }

    forceinline static void* allocator_proc(void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags) { return pool_allocator_proc(allocator_data, type, size, alignment, old_memory, old_size, flags); }
    forceinline static Pool  init(AllocatorInfo backing, ssize num_blocks, ssize block_size)                                                                { return pool_init(backing, num_blocks, block_size); }
    forceinline static Pool  init_align(AllocatorInfo backing, ssize num_blocks, ssize block_size, ssize block_align)                                       { return pool_init_align(backing, num_blocks, block_size, block_align); }
    forceinline        void  clear() { pool_clear( this); }
    forceinline        void  free()  { pool_free( this); }
#pragma endregion
#endif
};

inline
AllocatorInfo pool_allocator_info(Pool* pool) {
	AllocatorInfo info = { pool_allocator_proc, pool };
	return info;
}

inline
Pool pool_init(AllocatorInfo backing, ssize num_blocks, ssize block_size) {
	return pool_init_align(backing, num_blocks, block_size, GEN_DEFAULT_MEMORY_ALIGNMENT);
}

inline
void pool_free(Pool* pool) {
	if(pool->Backing.Proc) {
		allocator_free(pool->Backing, pool->PhysicalStart);
	}
}
#pragma endregion Pool

inline
b32 is_power_of_two( ssize x ) {
	if ( x <= 0 )
		return false;
	return ! ( x & ( x - 1 ) );
}

inline
mem_ptr align_forward( void* ptr, ssize alignment )
{
	GEN_ASSERT( is_power_of_two( alignment ) );
	uptr p       = to_uptr(ptr);
	uptr forward = (p + ( alignment - 1 ) ) & ~( alignment - 1 );

	return to_mem_ptr(forward);
}

inline s64 align_forward_s64( s64 value, ssize alignment ) { return value + ( alignment - value % alignment ) % alignment; }

inline void*       pointer_add      ( void*       ptr, ssize bytes ) { return rcast(void*,         rcast( u8*,        ptr) + bytes ); }
inline void const* pointer_add_const( void const* ptr, ssize bytes ) { return rcast(void const*, rcast( u8 const*,  ptr) + bytes ); }

inline sptr pointer_diff( mem_ptr_const begin, mem_ptr_const end ) {
	return scast( ssize, rcast( u8 const*, end) - rcast(u8 const*, begin) );
}

inline
void* mem_move( void* destination, void const* source, ssize byte_count )
{
	if ( destination == NULL )
	{
		return NULL;
	}

	u8*       dest_ptr = rcast( u8*, destination);
	u8 const* src_ptr  = rcast( u8 const*, source);

	if ( dest_ptr == src_ptr )
		return dest_ptr;

	if ( src_ptr + byte_count <= dest_ptr || dest_ptr + byte_count <= src_ptr )    // NOTE: Non-overlapping
		return mem_copy( dest_ptr, src_ptr, byte_count );

	if ( dest_ptr < src_ptr )
	{
		if ( to_uptr(src_ptr) % size_of( ssize ) == to_uptr(dest_ptr) % size_of( ssize ) )
		{
			while ( pcast( uptr, dest_ptr) % size_of( ssize ) )
			{
				if ( ! byte_count-- )
					return destination;

				*dest_ptr++ = *src_ptr++;
			}
			while ( byte_count >= size_of( ssize ) )
			{
				* rcast(ssize*, dest_ptr)  = * rcast(ssize const*, src_ptr);
				byte_count -= size_of( ssize );
				dest_ptr   += size_of( ssize );
				src_ptr    += size_of( ssize );
			}
		}
		for ( ; byte_count; byte_count-- )
			*dest_ptr++ = *src_ptr++;
	}
	else
	{
		if ( ( to_uptr(src_ptr) % size_of( ssize ) ) == ( to_uptr(dest_ptr) % size_of( ssize ) ) )
		{
			while ( to_uptr( dest_ptr + byte_count ) % size_of( ssize ) )
			{
				if ( ! byte_count-- )
					return destination;

				dest_ptr[ byte_count ] = src_ptr[ byte_count ];
			}
			while ( byte_count >= size_of( ssize ) )
			{
				byte_count                              -= size_of( ssize );
				* rcast(ssize*, dest_ptr + byte_count )  = * rcast( ssize const*, src_ptr + byte_count );
			}
		}
		while ( byte_count )
			byte_count--, dest_ptr[ byte_count ] = src_ptr[ byte_count ];
	}

	return destination;
}

inline
void* mem_set( void* destination, u8 fill_byte, ssize byte_count )
{
	if ( destination == NULL )
	{
		return NULL;
	}

	ssize align_offset;
	u8*   dest_ptr  = rcast( u8*, destination);
	u32   fill_word = ( ( u32 )-1 ) / 255 * fill_byte;

	if ( byte_count == 0 )
		return destination;

	dest_ptr[ 0 ] = dest_ptr[ byte_count - 1 ] = fill_byte;
	if ( byte_count < 3 )
		return destination;

	dest_ptr[ 1 ] = dest_ptr[ byte_count - 2 ] = fill_byte;
	dest_ptr[ 2 ] = dest_ptr[ byte_count - 3 ] = fill_byte;
	if ( byte_count < 7 )
		return destination;

	dest_ptr[ 3 ] = dest_ptr[ byte_count - 4 ] = fill_byte;
	if ( byte_count < 9 )
		return destination;

	align_offset  = -to_sptr( dest_ptr ) & 3;
	dest_ptr     += align_offset;
	byte_count   -= align_offset;
	byte_count   &= -4;

	* rcast( u32*, ( dest_ptr + 0              ) ) = fill_word;
	* rcast( u32*, ( dest_ptr + byte_count - 4 ) ) = fill_word;
	if ( byte_count < 9 )
		return destination;

	* rcast( u32*, dest_ptr + 4 )               = fill_word;
	* rcast( u32*, dest_ptr + 8 )               = fill_word;
	* rcast( u32*, dest_ptr + byte_count - 12 ) = fill_word;
	* rcast( u32*, dest_ptr + byte_count - 8 )  = fill_word;
	if ( byte_count < 25 )
		return destination;

	* rcast( u32*, dest_ptr + 12 )              = fill_word;
	* rcast( u32*, dest_ptr + 16 )              = fill_word;
	* rcast( u32*, dest_ptr + 20 )              = fill_word;
	* rcast( u32*, dest_ptr + 24 )              = fill_word;
	* rcast( u32*, dest_ptr + byte_count - 28 ) = fill_word;
	* rcast( u32*, dest_ptr + byte_count - 24 ) = fill_word;
	* rcast( u32*, dest_ptr + byte_count - 20 ) = fill_word;
	* rcast( u32*, dest_ptr + byte_count - 16 ) = fill_word;

	align_offset  = 24 + to_uptr( dest_ptr ) & 4;
	dest_ptr     += align_offset;
	byte_count   -= align_offset;

	{
		u64 fill_doubleword = ( scast( u64, fill_word) << 32 ) | fill_word;
		while ( byte_count > 31 )
		{
			* rcast( u64*, dest_ptr + 0 )  = fill_doubleword;
			* rcast( u64*, dest_ptr + 8 )  = fill_doubleword;
			* rcast( u64*, dest_ptr + 16 ) = fill_doubleword;
			* rcast( u64*, dest_ptr + 24 ) = fill_doubleword;

			byte_count -= 32;
			dest_ptr += 32;
		}
	}

	return destination;
}

inline
void* alloc_align( AllocatorInfo a, ssize size, ssize alignment ) {
	return a.Proc( a.Data, EAllocation_ALLOC, size, alignment, nullptr, 0, GEN_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* alloc( AllocatorInfo a, ssize size ) {
	return alloc_align( a, size, GEN_DEFAULT_MEMORY_ALIGNMENT );
}

inline
void allocator_free( AllocatorInfo a, void* ptr ) {
	if ( ptr != nullptr )
		a.Proc( a.Data, EAllocation_FREE, 0, 0, ptr, 0, GEN_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void free_all( AllocatorInfo a ) {
	a.Proc( a.Data, EAllocation_FREE_ALL, 0, 0, nullptr, 0, GEN_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* resize( AllocatorInfo a, void* ptr, ssize old_size, ssize new_size ) {
	return resize_align( a, ptr, old_size, new_size, GEN_DEFAULT_MEMORY_ALIGNMENT );
}

inline
void* resize_align( AllocatorInfo a, void* ptr, ssize old_size, ssize new_size, ssize alignment ) {
	return a.Proc( a.Data, EAllocation_RESIZE, new_size, alignment, ptr, old_size, GEN_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* default_resize_align( AllocatorInfo a, void* old_memory, ssize old_size, ssize new_size, ssize alignment )
{
	if ( ! old_memory )
		return alloc_align( a, new_size, alignment );

	if ( new_size == 0 )
	{
		allocator_free( a, old_memory );
		return nullptr;
	}

	if ( new_size < old_size )
		new_size = old_size;

	if ( old_size == new_size )
	{
		return old_memory;
	}
	else
	{
		void*  new_memory = alloc_align( a, new_size, alignment );
		if ( ! new_memory )
			return nullptr;

		mem_move( new_memory, old_memory, min( new_size, old_size ) );
		allocator_free( a, old_memory );
		return new_memory;
	}
}

inline
void zero_size( void* ptr, ssize size ) {
	mem_set( ptr, 0, size );
}

#pragma endregion Memory

#pragma region String Ops

const char* char_first_occurence( const char* str, char c );

b32   char_is_alpha( char c );
b32   char_is_alphanumeric( char c );
b32   char_is_digit( char c );
b32   char_is_hex_digit( char c );
b32   char_is_space( char c );
char  char_to_lower( char c );
char  char_to_upper( char c );

s32  digit_to_int( char c );
s32  hex_digit_to_int( char c );

s32         c_str_compare( const char* s1, const char* s2 );
s32         c_str_compare_len( const char* s1, const char* s2, ssize len );
char*       c_str_copy( char* dest, const char* source, ssize len );
ssize       c_str_copy_nulpad( char* dest, const char* source, ssize len );
ssize       c_str_len( const char* str );
ssize       c_str_len_capped( const char* str, ssize max_len );
char*       c_str_reverse( char* str );    // NOTE: ASCII only
char const* c_str_skip( char const* str, char c );
char const* c_str_skip_any( char const* str, char const* char_list );
char const* c_str_trim( char const* str, b32 catch_newline );

// NOTE: ASCII only
void c_str_to_lower( char* str );
void c_str_to_upper( char* str );

s64  c_str_to_i64( const char* str, char** end_ptr, s32 base );
void i64_to_str( s64 value, char* string, s32 base );
void u64_to_str( u64 value, char* string, s32 base );
f64  c_str_to_f64( const char* str, char** end_ptr );

inline
const char* char_first_occurence( const char* s, char c )
{
	char ch = c;
	for ( ; *s != ch; s++ )
	{
		if ( *s == '\0' )
			return NULL;
	}
	return s;
}

inline
b32 char_is_alpha( char c )
{
	if ( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) )
		return true;
	return false;
}

inline
b32 char_is_alphanumeric( char c )
{
	return char_is_alpha( c ) || char_is_digit( c );
}

inline
b32 char_is_digit( char c )
{
	if ( c >= '0' && c <= '9' )
		return true;
	return false;
}

inline
b32 char_is_hex_digit( char c )
{
	if ( char_is_digit( c ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) )
		return true;
	return false;
}

inline
b32 char_is_space( char c )
{
	if ( c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v' )
		return true;
	return false;
}

inline
char char_to_lower( char c )
{
	if ( c >= 'A' && c <= 'Z' )
		return 'a' + ( c - 'A' );
	return c;
}

inline char char_to_upper( char c )
{
	if ( c >= 'a' && c <= 'z' )
		return 'A' + ( c - 'a' );
	return c;
}

inline
s32 digit_to_int( char c )
{
	return char_is_digit( c ) ? c - '0' : c - 'W';
}

inline
s32 hex_digit_to_int( char c )
{
	if ( char_is_digit( c ) )
		return digit_to_int( c );
	else if ( is_between( c, 'a', 'f' ) )
		return c - 'a' + 10;
	else if ( is_between( c, 'A', 'F' ) )
		return c - 'A' + 10;
	return -1;
}

inline
s32 c_str_compare( const char* s1, const char* s2 )
{
	while ( *s1 && ( *s1 == *s2 ) )
	{
		s1++, s2++;
	}
	return *( u8* )s1 - *( u8* )s2;
}

inline
s32 c_str_compare_len( const char* s1, const char* s2, ssize len )
{
	for ( ; len > 0; s1++, s2++, len-- )
	{
		if ( *s1 != *s2 )
			return ( ( s1 < s2 ) ? -1 : +1 );
		else if ( *s1 == '\0' )
			return 0;
	}
	return 0;
}

inline
char* c_str_copy( char* dest, const char* source, ssize len )
{
	GEN_ASSERT_NOT_NULL( dest );
	if ( source )
	{
		char* str = dest;
		while ( len > 0 && *source )
		{
			*str++ = *source++;
			len--;
		}
		while ( len > 0 )
		{
			*str++ = '\0';
			len--;
		}
	}
	return dest;
}

inline
ssize c_str_copy_nulpad( char* dest, const char* source, ssize len )
{
	ssize result = 0;
	GEN_ASSERT_NOT_NULL( dest );
	if ( source )
	{
		const char* source_start = source;
		char*       str          = dest;
		while ( len > 0 && *source )
		{
			*str++ = *source++;
			len--;
		}
		while ( len > 0 )
		{
			*str++ = '\0';
			len--;
		}

		result = source - source_start;
	}
	return result;
}

inline
ssize c_str_len( const char* str )
{
	if ( str == NULL )
	{
		return 0;
	}
	const char* p = str;
	while ( *str )
		str++;
	return str - p;
}

inline
ssize c_str_len_capped( const char* str, ssize max_len )
{
	const char* end = rcast(const char*, mem_find( str, 0, max_len ));
	if ( end )
		return end - str;
	return max_len;
}

inline
char* c_str_reverse( char* str )
{
	ssize    len  = c_str_len( str );
	char* a    = str + 0;
	char* b    = str + len - 1;
	len       /= 2;
	while ( len-- )
	{
		swap( *a, *b );
		a++, b--;
	}
	return str;
}

inline
char const* c_str_skip( char const* str, char c )
{
	while ( *str && *str != c )
	{
		++str;
	}
	return str;
}

inline
char const* c_str_skip_any( char const* str, char const* char_list )
{
	char const* closest_ptr     = rcast( char const*, pointer_add_const( rcast(mem_ptr_const, str), c_str_len( str ) ));
	ssize       char_list_count = c_str_len( char_list );
	for ( ssize i = 0; i < char_list_count; i++ )
	{
		char const* p = c_str_skip( str, char_list[ i ] );
		closest_ptr   = min( closest_ptr, p );
	}
	return closest_ptr;
}

inline
char const* c_str_trim( char const* str, b32 catch_newline )
{
	while ( *str && char_is_space( *str ) && ( ! catch_newline || ( catch_newline && *str != '\n' ) ) )
	{
		++str;
	}
	return str;
}

inline
void c_str_to_lower( char* str )
{
	if ( ! str )
		return;
	while ( *str )
	{
		*str = char_to_lower( *str );
		str++;
	}
}

inline
void c_str_to_upper( char* str )
{
	if ( ! str )
		return;
	while ( *str )
	{
		*str = char_to_upper( *str );
		str++;
	}
}

#pragma endregion String Ops

#pragma region Printing

typedef struct FileInfo FileInfo;

#ifndef GEN_PRINTF_MAXLEN
#	define GEN_PRINTF_MAXLEN kilobytes(128)
#endif
typedef char PrintF_Buffer[GEN_PRINTF_MAXLEN];

// NOTE: A locally persisting buffer is used internally
char*  c_str_fmt_buf       ( char const* fmt, ... );
char*  c_str_fmt_buf_va    ( char const* fmt, va_list va );
ssize  c_str_fmt           ( char* str, ssize n, char const* fmt, ... );
ssize  c_str_fmt_va        ( char* str, ssize n, char const* fmt, va_list va );
ssize  c_str_fmt_out_va    ( char const* fmt, va_list va );
ssize  c_str_fmt_out_err   ( char const* fmt, ... );
ssize  c_str_fmt_out_err_va( char const* fmt, va_list va );
ssize  c_str_fmt_file      ( FileInfo* f, char const* fmt, ... );
ssize  c_str_fmt_file_va   ( FileInfo* f, char const* fmt, va_list va );

constexpr
char const* Msg_Invalid_Value = "INVALID VALUE PROVIDED";

inline
ssize log_fmt(char const* fmt, ...)
{
	ssize res;
	va_list va;

	va_start(va, fmt);
	res = c_str_fmt_out_va(fmt, va);
	va_end(va);

	return res;
}

#pragma endregion Printing

#pragma region Containers

template<class TType>             struct RemoveConst                    { typedef TType Type;       };
template<class TType>             struct RemoveConst<const TType>       { typedef TType Type;       };
template<class TType>             struct RemoveConst<const TType[]>     { typedef TType Type[];     };
template<class TType, usize Size> struct RemoveConst<const TType[Size]> { typedef TType Type[Size]; };

template<class TType> using TRemoveConst = typename RemoveConst<TType>::Type;

template <class TType> struct RemovePtr         { typedef TType Type; };
template <class TType> struct RemovePtr<TType*> { typedef TType Type; };

template <class TType> using TRemovePtr = typename RemovePtr<TType>::Type;


#pragma region Array
#define Array(Type) Array<Type>

// #define array_init(Type, ...)         array_init        <Type>(__VA_ARGS__)
// #define array_init_reserve(Type, ...) array_init_reserve<Type>(__VA_ARGS__)

struct ArrayHeader;

#if GEN_COMPILER_CPP
	template<class Type> struct Array;
#	define get_array_underlying_type(array) typename TRemovePtr<typeof(array)>:: DataType
#endif

usize array_grow_formula(ssize value);

template<class Type> Array<Type>  array_init           (AllocatorInfo allocator);
template<class Type> Array<Type>  array_init_reserve   (AllocatorInfo allocator, ssize capacity);
template<class Type> bool         array_append_array   (Array<Type>* array, Array<Type> other);
template<class Type> bool         array_append         (Array<Type>* array, Type value);
template<class Type> bool         array_append_items   (Array<Type>* array, Type* items, usize item_num);
template<class Type> bool         array_append_at      (Array<Type>* array, Type item, usize idx);
template<class Type> bool         array_append_items_at(Array<Type>* array, Type* items, usize item_num, usize idx);
template<class Type> Type*        array_back           (Array<Type>  array);
template<class Type> void         array_clear          (Array<Type>  array);
template<class Type> bool         array_fill           (Array<Type>  array, usize begin, usize end, Type value);
template<class Type> void         array_free           (Array<Type>* array);
template<class Type> bool         arary_grow           (Array<Type>* array, usize min_capacity);
template<class Type> usize        array_num            (Array<Type>  array);
template<class Type> void         arary_pop            (Array<Type>  array);
template<class Type> void         arary_remove_at      (Array<Type>  array, usize idx);
template<class Type> bool         arary_reserve        (Array<Type>* array, usize new_capacity);
template<class Type> bool         arary_resize         (Array<Type>* array, usize num);
template<class Type> bool         arary_set_capacity   (Array<Type>* array, usize new_capacity);
template<class Type> ArrayHeader* arary_get_header     (Array<Type>  array);

struct ArrayHeader {
	AllocatorInfo Allocator;
	usize         Capacity;
	usize         Num;
};

#if GEN_COMPILER_CPP
template<class Type>
struct Array
{
	Type* Data;

#pragma region Member Mapping
	forceinline static Array  init(AllocatorInfo allocator)                         { return array_init<Type>(allocator); }
	forceinline static Array  init_reserve(AllocatorInfo allocator, ssize capacity) { return array_init_reserve<Type>(allocator, capacity); }
	forceinline static usize  grow_formula(ssize value)                             { return array_grow_formula<Type>(value); }

	forceinline bool         append(Array other)                               { return array_append_array<Type>(this, other); }
	forceinline bool         append(Type value)                                { return array_append<Type>(this, value); }
	forceinline bool         append(Type* items, usize item_num)               { return array_append_items<Type>(this, items, item_num); }
	forceinline bool         append_at(Type item, usize idx)                   { return array_append_at<Type>(this, item, idx); }
	forceinline bool         append_at(Type* items, usize item_num, usize idx) { return array_append_items_at<Type>(this, items, item_num, idx); }
	forceinline Type*        back()                                            { return array_back<Type>(* this); }
	forceinline void         clear()                                           {        array_clear<Type>(* this); }
	forceinline bool         fill(usize begin, usize end, Type value)          { return array_fill<Type>(* this, begin, end, value); }
	forceinline void         free()                                            {        array_free<Type>(this); }
	forceinline ArrayHeader* get_header()                                      { return array_get_header<Type>(* this); }
	forceinline bool         grow(usize min_capacity)                          { return array_grow<Type>(this, min_capacity); }
	forceinline usize        num()                                             { return array_num<Type>(*this); }
	forceinline void         pop()                                             {        array_pop<Type>(* this); }
	forceinline void         remove_at(usize idx)                              {        array_remove_at<Type>(* this, idx); }
	forceinline bool         reserve(usize new_capacity)                       { return array_reserve<Type>(this, new_capacity); }
	forceinline bool         resize(usize num)                                 { return array_resize<Type>(this, num); }
	forceinline bool         set_capacity(usize new_capacity)                  { return array_set_capacity<Type>(this, new_capacity); }
#pragma endregion Member Mapping

	forceinline operator Type*()             { return Data; }
	forceinline operator Type const*() const { return Data; }
	forceinline Type* begin()                { return Data; }
	forceinline Type* end()                  { return Data + get_header()->Num; }

	forceinline Type&       operator[](ssize index)       { return Data[index]; }
	forceinline Type const& operator[](ssize index) const { return Data[index]; }

	using DataType = Type;
};
#endif

#if GEN_COMPILER_CPP && 0
template<class Type> bool append(Array<Type>& array, Array<Type> other)                         { return append( & array, other ); }
template<class Type> bool append(Array<Type>& array, Type value)                                { return append( & array, value ); }
template<class Type> bool append(Array<Type>& array, Type* items, usize item_num)               { return append( & array, items, item_num ); }
template<class Type> bool append_at(Array<Type>& array, Type item, usize idx)                   { return append_at( & array, item, idx ); }
template<class Type> bool append_at(Array<Type>& array, Type* items, usize item_num, usize idx) { return append_at( & array, items, item_num, idx ); }
template<class Type> void free(Array<Type>& array)                                              { return free( & array ); }
template<class Type> bool grow(Array<Type>& array, usize min_capacity)                          { return grow( & array, min_capacity); }
template<class Type> bool reserve(Array<Type>& array, usize new_capacity)                       { return reserve( & array, new_capacity); }
template<class Type> bool resize(Array<Type>& array, usize num)                                 { return resize( & array, num); }
template<class Type> bool set_capacity(Array<Type>& array, usize new_capacity)                  { return set_capacity( & array, new_capacity); }

template<class Type> forceinline Type* begin(Array<Type>& array)             { return array;      }
template<class Type> forceinline Type* end(Array<Type>& array)               { return array + array_get_header(array)->Num; }
template<class Type> forceinline Type* next(Array<Type>& array, Type* entry) { return entry + 1; }
#endif

template<class Type> forceinline Type* array_begin(Array<Type> array)             { return array;      }
template<class Type> forceinline Type* array_end(Array<Type> array)               { return array + array_get_header(array)->Num; }
template<class Type> forceinline Type* array_next(Array<Type> array, Type* entry) { return ++ entry; }

template<class Type> inline
Array<Type> array_init(AllocatorInfo allocator) {
	return array_init_reserve<Type>(allocator, array_grow_formula(0));
}

template<class Type> inline
Array<Type> array_init_reserve(AllocatorInfo allocator, ssize capacity)
{
	GEN_ASSERT(capacity > 0);
	ArrayHeader* header = rcast(ArrayHeader*, alloc(allocator, sizeof(ArrayHeader) + sizeof(Type) * capacity));

	if (header == nullptr)
		return {nullptr};

	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;

	return {rcast(Type*, header + 1)};
}

usize array_grow_formula(ssize value) {
	return 2 * value + 8;
}

template<class Type> inline
bool array_append_array(Array<Type>* array, Array<Type> other) {
	return array_append_items(array, (Type*)other, num(other));
}

template<class Type> inline
bool array_append(Array<Type>* array, Type value)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	ArrayHeader* header = array_get_header(* array);

	if (header->Num == header->Capacity)
	{
		if ( ! array_grow(array, header->Capacity))
			return false;
		header = array_get_header(* array);
	}

	(*array)[ header->Num] = value;
	header->Num++;

	return true;
}

template<class Type> inline
bool array_append_items(Array<Type>* array, Type* items, usize item_num)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	GEN_ASSERT(items != nullptr);
	GEN_ASSERT(item_num > 0);
	ArrayHeader* header = array_get_header(array);

	if (header->Num + item_num > header->Capacity)
	{
		if ( ! grow(array, header->Capacity + item_num))
			return false;
		header = array_get_header(array);
	}

	mem_copy((Type*)array + header->Num, items, item_num * sizeof(Type));
	header->Num += item_num;

	return true;
}

template<class Type> inline
bool array_append_at(Array<Type>* array, Type item, usize idx)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	ArrayHeader* header = array_get_header(* array);

	ssize slot = idx;
	if (slot >= header->Num)
		slot = header->Num - 1;

	if (slot < 0)
		slot = 0;

	if (header->Capacity < header->Num + 1)
	{
		if ( ! array_grow(array, header->Capacity + 1))
			return false;

		header = array_get_header(* array);
	}

	Type* target = &(*array)[slot];

	mem_move(target + 1, target, (header->Num - slot) * sizeof(Type));
	header->Num++;

	return true;
}

template<class Type> inline
bool array_append_items_at(Array<Type>* array, Type* items, usize item_num, usize idx)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	ArrayHeader* header = get_header(array);

	if (idx >= header->Num)
	{
		return array_append_items(array, items, item_num);
	}

	if (item_num > header->Capacity)
	{
		if (! grow(array, header->Capacity + item_num))
			return false;

		header = get_header(array);
	}

	Type* target = array.Data + idx + item_num;
	Type* src    = array.Data + idx;

	mem_move(target, src, (header->Num - idx) * sizeof(Type));
	mem_copy(src, items, item_num * sizeof(Type));
	header->Num += item_num;

	return true;
}

template<class Type> inline
Type* array_back(Array<Type> array)
{
	GEN_ASSERT(array != nullptr);

	ArrayHeader* header = array_get_header(array);
	if (header->Num <= 0)
		return nullptr;

	return & (array)[header->Num - 1];
}

template<class Type> inline
void array_clear(Array<Type> array) {
	GEN_ASSERT(array != nullptr);
	ArrayHeader* header = array_get_header(array);
	header->Num = 0;
}

template<class Type> inline
bool array_fill(Array<Type> array, usize begin, usize end, Type value)
{
	GEN_ASSERT(array != nullptr);
	GEN_ASSERT(begin <= end);
	ArrayHeader* header = array_get_header(array);

	if (begin < 0 || end > header->Num)
	return false;

	for (ssize idx = ssize(begin); idx < ssize(end); idx++)
	{
		array[idx] = value;
	}

	return true;
}

template<class Type> forceinline
void array_free(Array<Type>* array) {
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	ArrayHeader* header = array_get_header(* array);
	allocator_free(header->Allocator, header);
	Type** Data = (Type**)array;
	*Data = nullptr;
}

template<class Type> forceinline
ArrayHeader* array_get_header(Array<Type> array) {
	GEN_ASSERT(array != nullptr);
    Type* Data = array;

	using NonConstType = TRemoveConst<Type>;
    return rcast(ArrayHeader*, const_cast<NonConstType*>(Data)) - 1;
}
template<class Type> forceinline
bool array_grow(Array<Type>* array, usize min_capacity)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	GEN_ASSERT( min_capacity > 0 );
	ArrayHeader* header       = array_get_header(* array);
	usize        new_capacity = array_grow_formula(header->Capacity);

	if (new_capacity < min_capacity)
		new_capacity = min_capacity;

	return array_set_capacity(array, new_capacity);
}

template<class Type> forceinline
usize array_num(Array<Type> array) {
	GEN_ASSERT(array != nullptr);
	return array_get_header(array)->Num;
}

template<class Type> forceinline
void array_pop(Array<Type> array) {
	GEN_ASSERT(array != nullptr);
	ArrayHeader* header = array_get_header(array);
	GEN_ASSERT(header->Num > 0);
	header->Num--;
}

template<class Type> inline
void array_remove_at(Array<Type> array, usize idx)
{
	GEN_ASSERT(array != nullptr);
	ArrayHeader* header = array_get_header(array);
	GEN_ASSERT(idx < header->Num);

	mem_move(array + idx, array + idx + 1, sizeof(Type) * (header->Num - idx - 1));
	header->Num--;
}

template<class Type> inline
bool array_reserve(Array<Type>* array, usize new_capacity)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	GEN_ASSERT(num > 0)
	ArrayHeader* header = array_get_header(array);

	if (header->Capacity < new_capacity)
		return set_capacity(array, new_capacity);

	return true;
}

template<class Type> inline
bool array_resize(Array<Type>* array, usize num)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	ArrayHeader* header = array_get_header(* array);

	if (header->Capacity < num) {
		if (! array_grow( array, num))
			return false;
		header = array_get_header(* array);
	}

	header->Num = num;
	return true;
}

template<class Type> inline
bool array_set_capacity(Array<Type>* array, usize new_capacity)
{
	GEN_ASSERT(  array != nullptr);
	GEN_ASSERT(* array != nullptr);
	ArrayHeader* header = array_get_header(* array);

	if (new_capacity == header->Capacity)
		return true;

	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}

	ssize        size       = sizeof(ArrayHeader) + sizeof(Type) * new_capacity;
	ArrayHeader* new_header = rcast(ArrayHeader*, alloc(header->Allocator, size));

	if (new_header == nullptr)
		return false;

	mem_move(new_header, header, sizeof(ArrayHeader) + sizeof(Type) * header->Num);

	new_header->Capacity = new_capacity;

	allocator_free(header->Allocator, header);

	Type** Data = (Type**)array;
	* Data = rcast(Type*, new_header + 1);
	return true;
}

// These are intended for use in the base library of gencpp and the C-variant of the library
// It provides a interoperability between the C++ and C implementation of arrays. (not letting these do any crazy substiution though)
// They are undefined in gen.hpp and gen.cpp at the end of the files.
// We cpp library expects the user to use the regular calls as they can resolve the type fine.

#define array_init(type, allocator)                        array_init           <type>                               (allocator )
#define array_init_reserve(type, allocator, cap)           array_init_reserve   <type>                               (allocator, cap)
#define array_append_array(array, other)                   array_append_array   < get_array_underlying_type(array) > (& array, other )
#define array_append(array, value)                         array_append         < get_array_underlying_type(array) > (& array, value )
#define array_append_items(array, items, item_num)         array_append_items   < get_array_underlying_type(array) > (& array, items, item_num )
#define array_append_at(array, item, idx )                 array_append_at      < get_array_underlying_type(array) > (& array, item, idx )
#define array_append_at_items(array, items, item_num, idx) array_append_at_items< get_array_underlying_type(array) > (& items, item_num, idx )
#define array_back(array)                                  array_back           < get_array_underlying_type(array) > (array )
#define array_clear(array)                                 array_clear          < get_array_underlying_type(array) > (array )
#define array_fill(array, begin, end, value)               array_fill           < get_array_underlying_type(array) > (array, begin, end, value )
#define array_free(array)                                  array_free           < get_array_underlying_type(array) > (& array )
#define arary_grow(array, min_capacity)                    arary_grow           < get_array_underlying_type(array) > (& array, min_capacity)
#define array_num(array)                                   array_num            < get_array_underlying_type(array) > (array )
#define arary_pop(array)                                   arary_pop            < get_array_underlying_type(array) > (array )
#define arary_remove_at(array, idx)                        arary_remove_at      < get_array_underlying_type(array) > (idx)
#define arary_reserve(array, new_capacity)                 arary_reserve        < get_array_underlying_type(array) > (& array, new_capacity )
#define arary_resize(array, num)                           arary_resize         < get_array_underlying_type(array) > (& array, num)
#define arary_set_capacity(new_capacity)                   arary_set_capacity   < get_array_underlying_type(array) > (& array, new_capacity )
#define arary_get_header(array)                            arary_get_header     < get_array_underlying_type(array) > (array )

#pragma endregion Array

#pragma region HashTable
#define HashTable(Type) HashTable<Type>

template<class Type> struct HashTable;

#ifndef get_hashtable_underlying_type
#define get_hashtable_underlying_type(table) typename TRemovePtr<typeof(table)>:: DataType
#endif

struct HashTableFindResult {
	ssize HashIndex;
	ssize PrevIndex;
	ssize EntryIndex;
};

template<class Type>
struct HashTableEntry {
	u64   Key;
	ssize Next;
	Type  Value;
};

#define HashTableEntry(Type) HashTableEntry<Type>

template<class Type> HashTable<Type>       hashtable_init        (AllocatorInfo allocator);
template<class Type> HashTable<Type>       hashtable_init_reserve(AllocatorInfo allocator, usize num);
template<class Type> void                  hashtable_clear       (HashTable<Type>  table);
template<class Type> void                  hashtable_destroy     (HashTable<Type>* table);
template<class Type> Type*                 hashtable_get         (HashTable<Type>  table, u64 key);
template<class Type> void                  hashtable_grow        (HashTable<Type>* table);
template<class Type> void                  hashtable_rehash      (HashTable<Type>* table, ssize new_num);
template<class Type> void                  hashtable_rehash_fast (HashTable<Type>  table);
template<class Type> void                  hashtable_remove      (HashTable<Type>  table, u64 key);
template<class Type> void                  hashtable_remove_entry(HashTable<Type>  table, ssize idx);
template<class Type> void                  hashtable_set         (HashTable<Type>* table, u64 key, Type value);
template<class Type> ssize                 hashtable_slot        (HashTable<Type>  table, u64 key);
template<class Type> void                  hashtable_map         (HashTable<Type>  table, void (*map_proc)(u64 key, Type value));
template<class Type> void                  hashtable_map_mut     (HashTable<Type>  table, void (*map_proc)(u64 key, Type* value));

template<class Type> ssize                 hashtable__add_entry  (HashTable<Type>* table, u64 key);
template<class Type> HashTableFindResult   hashtable__find       (HashTable<Type>  table, u64 key);
template<class Type> bool                  hashtable__full       (HashTable<Type>  table);

static constexpr f32 HashTable_CriticalLoadScale = 0.7f;

template<typename Type>
struct HashTable
{
	Array<ssize>                Hashes;
	Array<HashTableEntry<Type>> Entries;

#if ! GEN_C_LIKE_CPP
#pragma region Member Mapping
	forceinline static HashTable init(AllocatorInfo allocator)                    { return	hashtable_init<Type>(allocator); }
	forceinline static HashTable init_reserve(AllocatorInfo allocator, usize num) { return	hashtable_init_reserve<Type>(allocator, num); }

	forceinline void  clear()                           {        clear<Type>(*this); }
	forceinline void  destroy()                         {        destroy<Type>(*this); }
	forceinline Type* get(u64 key)                      { return get<Type>(*this, key); }
	forceinline void  grow()                            {        grow<Type>(*this); }
	forceinline void  rehash(ssize new_num)             {        rehash<Type>(*this, new_num); }
	forceinline void  rehash_fast()                     {        rehash_fast<Type>(*this); }
	forceinline void  remove(u64 key)                   {        remove<Type>(*this, key); }
	forceinline void  remove_entry(ssize idx)           {        remove_entry<Type>(*this, idx); }
	forceinline void  set(u64 key, Type value)          {        set<Type>(*this, key, value); }
	forceinline ssize slot(u64 key)                     { return slot<Type>(*this, key); }
	forceinline void  map(void (*proc)(u64, Type))      {        map<Type>(*this, proc); }
	forceinline void  map_mut(void (*proc)(u64, Type*)) {        map_mut<Type>(*this, proc); }
#pragma endregion Member Mapping
#endif

	using DataType = Type;
};

#if GEN_SUPPORT_CPP_REFERENCES
template<class Type> void  destroy  (HashTable<Type>& table)                      { destroy(& table); }
template<class Type> void  grow     (HashTable<Type>& table)                      { grow(& table); }
template<class Type> void  rehash   (HashTable<Type>& table, ssize new_num)       { rehash(& table, new_num); }
template<class Type> void  set      (HashTable<Type>& table, u64 key, Type value) { set(& table, key, value); }
template<class Type> ssize add_entry(HashTable<Type>& table, u64 key)             { add_entry(& table, key); }
#endif

template<typename Type> inline
HashTable<Type> hashtable_init(AllocatorInfo allocator) {
	HashTable<Type> result = hashtable_init_reserve<Type>(allocator, 8);
	return result;
}

template<typename Type> inline
HashTable<Type> hashtable_init_reserve(AllocatorInfo allocator, usize num)
{
	HashTable<Type> result = { { nullptr }, { nullptr } };

	result.Hashes = array_init_reserve<ssize>(allocator, num);
	array_get_header(result.Hashes)->Num = num;
	array_resize(& result.Hashes, num);
	array_fill(result.Hashes, 0, num, (ssize)-1);

	result.Entries = array_init_reserve<HashTableEntry<Type>>(allocator, num);
	return result;
}

template<typename Type> forceinline
void hashtable_clear(HashTable<Type> table) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	array_clear(table.Entries);
	array_fill(table.Hashes, 0, array_num(table.Hashes), (ssize)-1);
}

template<typename Type> forceinline
void hashtable_destroy(HashTable<Type>* table) {
	GEN_ASSERT_NOT_NULL(table->Hashes);
	GEN_ASSERT_NOT_NULL(table->Entries);
	if (table->Hashes && array_get_header(table->Hashes)->Capacity) {
		array_free(table->Hashes);
		array_free(table->Entries);
	}
}

template<typename Type> forceinline
Type* hashtable_get(HashTable<Type> table, u64 key) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	ssize idx = hashtable__find(table, key).EntryIndex;
	if (idx >= 0)
		return & table.Entries[idx].Value;

	return nullptr;
}

template<typename Type> forceinline
void hashtable_map(HashTable<Type> table, void (*map_proc)(u64 key, Type value)) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	GEN_ASSERT_NOT_NULL(map_proc);

	for (ssize idx = 0; idx < ssize(num(table.Entries)); ++idx) {
		map_proc(table.Entries[idx].Key, table.Entries[idx].Value);
	}
}

template<typename Type> forceinline
void hashtable_map_mut(HashTable<Type> table, void (*map_proc)(u64 key, Type* value)) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	GEN_ASSERT_NOT_NULL(map_proc);

	for (ssize idx = 0; idx < ssize(num(table.Entries)); ++idx) {
		map_proc(table.Entries[idx].Key, & table.Entries[idx].Value);
	}
}

template<typename Type> forceinline
void hashtable_grow(HashTable<Type>* table) {
	GEN_ASSERT_NOT_NULL(table);
	GEN_ASSERT_NOT_NULL(table->Hashes);
	GEN_ASSERT_NOT_NULL(table->Entries);
	ssize new_num = array_grow_formula( array_num(table->Entries));
	hashtable_rehash(table, new_num);
}

template<typename Type> inline
void hashtable_rehash(HashTable<Type>* table, ssize new_num)
{
	GEN_ASSERT_NOT_NULL(table);
	GEN_ASSERT_NOT_NULL(table->Hashes);
	GEN_ASSERT_NOT_NULL(table->Entries);
	ssize last_added_index;
	HashTable<Type> new_ht = hashtable_init_reserve<Type>( array_get_header(table->Hashes)->Allocator, new_num);

	for (ssize idx = 0; idx < ssize( array_num(table->Entries)); ++idx)
	{
		HashTableFindResult find_result;
		HashTableEntry<Type>& entry = table->Entries[idx];

		find_result = hashtable__find(new_ht, entry.Key);
		last_added_index = hashtable__add_entry(& new_ht, entry.Key);

		if (find_result.PrevIndex < 0)
			new_ht.Hashes[find_result.HashIndex] = last_added_index;
		else
			new_ht.Entries[find_result.PrevIndex].Next = last_added_index;

		new_ht.Entries[last_added_index].Next = find_result.EntryIndex;
		new_ht.Entries[last_added_index].Value = entry.Value;
	}

	hashtable_destroy(table);
	* table = new_ht;
}

template<typename Type> inline
void hashtable_rehash_fast(HashTable<Type> table)
{
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	ssize idx;

	for (idx = 0; idx < ssize(num(table.Entries)); idx++)
		table.Entries[idx].Next = -1;

	for (idx = 0; idx < ssize(num(table.Hashes)); idx++)
		table.Hashes[idx] = -1;

	for (idx = 0; idx < ssize(num(table.Entries)); idx++)
	{
		HashTableEntry<Type>* entry;
		HashTableFindResult find_result;

		entry = &table.Entries[idx];
		find_result = find(table, entry->Key);

		if (find_result.PrevIndex < 0)
			table.Hashes[find_result.HashIndex] = idx;
		else
			table.Entries[find_result.PrevIndex].Next = idx;
	}
}

template<typename Type> forceinline
void hashtable_remove(HashTable<Type> table, u64 key) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	HashTableFindResult find_result = find(table, key);

	if (find_result.EntryIndex >= 0) {
		remove_at(table.Entries, find_result.EntryIndex);
		rehash_fast(table);
	}
}

template<typename Type> forceinline
void hashtable_remove_entry(HashTable<Type> table, ssize idx) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	remove_at(table.Entries, idx);
}

template<typename Type> inline
void hashtable_set(HashTable<Type>* table, u64 key, Type value)
{
	GEN_ASSERT_NOT_NULL(table);
	GEN_ASSERT_NOT_NULL(table->Hashes);
	GEN_ASSERT_NOT_NULL(table->Entries);
	ssize idx;
	HashTableFindResult find_result;

	if (hashtable_full(* table))
		hashtable_grow(table);

	find_result = hashtable__find(* table, key);
	if (find_result.EntryIndex >= 0) {
		idx = find_result.EntryIndex;
	}
	else
	{
		idx = hashtable__add_entry(table, key);

		if (find_result.PrevIndex >= 0) {
			table->Entries[find_result.PrevIndex].Next = idx;
		}
		else {
			table->Hashes[find_result.HashIndex] = idx;
		}
	}

	table->Entries[idx].Value = value;

	if (hashtable_full(* table))
		hashtable_grow(table);
}

template<typename Type> forceinline
ssize hashtable_slot(HashTable<Type> table, u64 key) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	for (ssize idx = 0; idx < ssize(num(table.Hashes)); ++idx)
		if (table.Hashes[idx] == key)
			return idx;

	return -1;
}

template<typename Type> forceinline
ssize hashtable__add_entry(HashTable<Type>* table, u64 key) {
	GEN_ASSERT_NOT_NULL(table);
	GEN_ASSERT_NOT_NULL(table->Hashes);
	GEN_ASSERT_NOT_NULL(table->Entries);
	ssize idx;
	HashTableEntry<Type> entry = { key, -1 };

	idx = array_num(table->Entries);
	array_append( table->Entries, entry);
	return idx;
}

template<typename Type> inline
HashTableFindResult hashtable__find(HashTable<Type> table, u64 key)
{
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	HashTableFindResult result = { -1, -1, -1 };

	if (array_num(table.Hashes) > 0)
	{
		result.HashIndex = key % array_num(table.Hashes);
		result.EntryIndex = table.Hashes[result.HashIndex];

		while (result.EntryIndex >= 0)
		{
			if (table.Entries[result.EntryIndex].Key == key)
				break;

			result.PrevIndex = result.EntryIndex;
			result.EntryIndex = table.Entries[result.EntryIndex].Next;
		}
	}

	return result;
}

template<typename Type> forceinline
bool hashtable_full(HashTable<Type> table) {
	GEN_ASSERT_NOT_NULL(table.Hashes);
	GEN_ASSERT_NOT_NULL(table.Entries);
	usize critical_load = usize(HashTable_CriticalLoadScale * f32(array_num(table.Hashes)));
	b32 result = array_num(table.Entries) > critical_load;
	return result;
}

#define hashtable_init(type, allocator)              hashtable_init        <type              >(allocator)
#define hashtable_init_reserve(type, allocator, num) hashtable_init_reserve<type              >(allocator, num)
#define hashtable_clear(table)                       hashtable_clear       < get_hashtable_underlying_type(table) >(table)
#define hashtable_destroy(table)                     hashtable_destroy     < get_hashtable_underlying_type(table) >(& table)
#define hashtable_get(table, key)                    hashtable_get         < get_hashtable_underlying_type(table) >(table, key)
#define hashtable_grow(table)                        hashtable_grow        < get_hashtable_underlying_type(table) >(& table)
#define hashtable_rehash(table, new_num)             hashtable_rehash      < get_hashtable_underlying_type(table) >(& table, new_num)
#define hashtable_rehash_fast(table)                 hashtable_rehash_fast < get_hashtable_underlying_type(table) >(table)
#define hashtable_remove(table, key)                 hashtable_remove      < get_hashtable_underlying_type(table) >(table, key)
#define hashtable_remove_entry(table, idx)           hashtable_remove_entry< get_hashtable_underlying_type(table) >(table, idx)
#define hashtable_set(table, key, value)             hashtable_set         < get_hashtable_underlying_type(table) >(& table, key, value)
#define hashtable_slot(table, key)                   hashtable_slot        < get_hashtable_underlying_type(table) >(table, key)
#define hashtable_map(table, map_proc)               hashtable_map         < get_hashtable_underlying_type(table) >(table, map_proc)
#define hashtable_map_mut(table, map_proc)           hashtable_map_mut     < get_hashtable_underlying_type(table) >(table, map_proc)

//#define hashtable_add_entry(table, key)              hashtable_add_entry   < get_hashtable_underlying_type(table) >(& table, key)
//#define hashtable_find(table, key)                   hashtable_find        < get_hashtable_underlying_type(table) >(table, key)
//#define hashtable_full(table)                        hashtable_full        < get_hashtable_underlying_type(table) >(table)

#pragma endregion HashTable

#pragma endregion Containers

#pragma region Hashing

u32 crc32( void const* data, ssize len );
u64 crc64( void const* data, ssize len );

#pragma endregion Hashing

#pragma region Strings

struct Str;

Str         to_str_from_c_str       (char const* bad_string);
bool        str_are_equal           (Str lhs, Str rhs);
char const* str_back                (Str str);
bool        str_contains            (Str str, Str substring);
Str         str_duplicate           (Str str, AllocatorInfo allocator);
b32         str_starts_with         (Str str, Str substring);
Str         str_visualize_whitespace(Str str, AllocatorInfo allocator);

// Constant string with length.
struct Str
{
	char const* Ptr;
	ssize       Len;

#if GEN_COMPILER_CPP
	forceinline operator char const* ()               const { return Ptr; }
	forceinline char const& operator[]( ssize index ) const { return Ptr[index]; }

#if ! GEN_C_LIKE_CPP
	forceinline bool        is_equal            (Str rhs)                 const { return str_are_equal(* this, rhs); }
	forceinline char const* back                ()                        const { return str_back(* this); }
	forceinline bool        contains            (Str substring)           const { return str_contains(* this, substring); }
	forceinline Str         duplicate           (AllocatorInfo allocator) const { return str_duplicate(* this, allocator); }
	forceinline b32         starts_with         (Str substring)           const { return str_starts_with(* this, substring); }
	forceinline Str         visualize_whitespace(AllocatorInfo allocator) const { return str_visualize_whitespace(* this, allocator); }
#endif
#endif
};

#define cast_to_str( str ) * rcast( Str*, (str) - sizeof(ssize) )

#ifndef txt
#	if GEN_COMPILER_CPP
#		define txt( text )          GEN_NS Str { ( text ), sizeof( text ) - 1 }
#	else
#		define txt( text )         (GEN_NS Str){ ( text ), sizeof( text ) - 1 }
#	endif
#endif

GEN_API_C_BEGIN
forceinline char const* str_begin(Str str)                   { return str.Ptr; }
forceinline char const* str_end  (Str str)                   { return str.Ptr + str.Len; }
forceinline char const* str_next (Str str, char const* iter) { return iter + 1; }
GEN_API_C_END

#if GEN_COMPILER_CPP
forceinline char const* begin(Str str)                   { return str.Ptr; }
forceinline char const* end  (Str str)                   { return str.Ptr + str.Len; }
forceinline char const* next (Str str, char const* iter) { return iter + 1; }
#endif

inline
bool str_are_equal(Str lhs, Str rhs)
{
	if (lhs.Len != rhs.Len)
		return false;

	for (ssize idx = 0; idx < lhs.Len; ++idx)
		if (lhs.Ptr[idx] != rhs.Ptr[idx])
			return false;

	return true;
}

inline
char const* str_back(Str str) {
	return & str.Ptr[str.Len - 1];
}

inline
bool str_contains(Str str, Str substring)
{
	if (substring.Len > str.Len)
		return false;

	ssize main_len = str.Len;
	ssize sub_len  = substring.Len;
	for (ssize idx = 0; idx <= main_len - sub_len; ++idx)
	{
		if (c_str_compare_len(str.Ptr + idx, substring.Ptr, sub_len) == 0)
			return true;
	}
	return false;
}

inline
b32 str_starts_with(Str str, Str substring) {
	if (substring.Len > str.Len)
		return false;

	b32 result = c_str_compare_len(str.Ptr, substring.Ptr, substring.Len) == 0;
		return result;
}

inline
Str to_str_from_c_str( char const* bad_str ) {
	Str result = { bad_str, c_str_len( bad_str ) };
	return result;
}

// Dynamic StrBuilder
// This is directly based off the ZPL string api.
// They used a header pattern
// I kept it for simplicty of porting but its not necessary to keep it that way.
#pragma region StrBuilder
struct StrBuilderHeader;

#if GEN_COMPILER_C
typedef char* StrBuilder;
#else
struct StrBuilder;
#endif

forceinline usize strbuilder_grow_formula(usize value);

StrBuilder        strbuilder_make_c_str          (AllocatorInfo allocator, char const*  str);
StrBuilder        strbuilder_make_str            (AllocatorInfo allocator, Str         str);
StrBuilder        strbuilder_make_reserve        (AllocatorInfo allocator, ssize        capacity);
StrBuilder        strbuilder_make_length         (AllocatorInfo allocator, char const*  str,   ssize length);
StrBuilder        strbuilder_fmt                 (AllocatorInfo allocator, char*        buf,   ssize buf_size,  char const* fmt, ...);
StrBuilder        strbuilder_fmt_buf             (AllocatorInfo allocator, char const*  fmt, ...);
StrBuilder        strbuilder_join                (AllocatorInfo allocator, char const** parts, ssize num_parts, char const* glue);
bool              strbuilder_are_equal           (StrBuilder const lhs, StrBuilder const rhs);
bool              strbuilder_are_equal_str       (StrBuilder const lhs, Str rhs);
bool              strbuilder_make_space_for      (StrBuilder*      str, char const*  to_append, ssize add_len);
bool              strbuilder_append_char         (StrBuilder*      str, char         c);
bool              strbuilder_append_c_str        (StrBuilder*      str, char const*  c_str_to_append);
bool              strbuilder_append_c_str_len    (StrBuilder*      str, char const*  c_str_to_append, ssize length);
bool              strbuilder_append_str          (StrBuilder*      str, Str         c_str_to_append);
bool              strbuilder_append_string       (StrBuilder*      str, StrBuilder const other);
bool              strbuilder_append_fmt          (StrBuilder*      str, char const*  fmt, ...);
ssize             strbuilder_avail_space         (StrBuilder const str);
char*             strbuilder_back                (StrBuilder       str);
bool              strbuilder_contains_str        (StrBuilder const str, Str         substring);
bool              strbuilder_contains_string     (StrBuilder const str, StrBuilder const substring);
ssize             strbuilder_capacity            (StrBuilder const str);
void              strbuilder_clear               (StrBuilder       str);
StrBuilder        strbuilder_duplicate           (StrBuilder const str, AllocatorInfo allocator);
void              strbuilder_free                (StrBuilder*      str);
StrBuilderHeader* strbuilder_get_header          (StrBuilder       str);
ssize             strbuilder_length              (StrBuilder const str);
b32               strbuilder_starts_with_str     (StrBuilder const str, Str   substring);
b32               strbuilder_starts_with_string  (StrBuilder const str, StrBuilder substring);
void              strbuilder_skip_line           (StrBuilder       str);
void              strbuilder_strip_space         (StrBuilder       str);
Str               strbuilder_to_str              (StrBuilder       str);
void              strbuilder_trim                (StrBuilder       str, char const* cut_set);
void              strbuilder_trim_space          (StrBuilder       str);
StrBuilder        strbuilder_visualize_whitespace(StrBuilder const str);

struct StrBuilderHeader {
	AllocatorInfo Allocator;
	ssize         Capacity;
	ssize         Length;
};

#if GEN_COMPILER_CPP
struct StrBuilder
{
	char* Data;

	forceinline operator char*()             { return Data; }
	forceinline operator char const*() const { return Data; }
	forceinline operator Str()         const { return { Data, strbuilder_length(* this) }; }

	StrBuilder const& operator=(StrBuilder const& other) const {
		if (this == &other)
			return *this;

		StrBuilder* this_ = ccast(StrBuilder*, this);
		this_->Data = other.Data;

		return *this;
	}

	forceinline char&       operator[](ssize index)       { return Data[index]; }
	forceinline char const& operator[](ssize index) const { return Data[index]; }

	       forceinline bool operator==(std::nullptr_t) const                 { return     Data == nullptr; }
	       forceinline bool operator!=(std::nullptr_t) const                 { return     Data != nullptr; }
	friend forceinline bool operator==(std::nullptr_t, const StrBuilder str) { return str.Data == nullptr; }
	friend forceinline bool operator!=(std::nullptr_t, const StrBuilder str) { return str.Data != nullptr; }

#if ! GEN_C_LIKE_CPP
	forceinline char* begin() const { return Data; }
	forceinline char* end()   const { return Data + strbuilder_length(* this); }

#pragma region Member Mapping
	forceinline static StrBuilder make(AllocatorInfo allocator, char const* str)                { return strbuilder_make_c_str(allocator, str); }
	forceinline static StrBuilder make(AllocatorInfo allocator, Str str)                        { return strbuilder_make_str(allocator, str); }
	forceinline static StrBuilder make_reserve(AllocatorInfo allocator, ssize cap)              { return strbuilder_make_reserve(allocator, cap); }
	forceinline static StrBuilder make_length(AllocatorInfo a, char const* s, ssize l)          { return strbuilder_make_length(a, s, l); }
	forceinline static StrBuilder join(AllocatorInfo a, char const** p, ssize n, char const* g) { return strbuilder_join(a, p, n, g); }
	forceinline static usize      grow_formula(usize value)                                     { return strbuilder_grow_formula(value); }

	static
	StrBuilder fmt(AllocatorInfo allocator, char* buf, ssize buf_size, char const* fmt, ...) {
		va_list va;
		va_start(va, fmt);
		ssize res = c_str_fmt_va(buf, buf_size, fmt, va) - 1;
		va_end(va);
		return strbuilder_make_length(allocator, buf, res);
	}

	static
	StrBuilder fmt_buf(AllocatorInfo allocator, char const* fmt, ...) {
		local_persist thread_local
		char buf[GEN_PRINTF_MAXLEN] = { 0 };
		va_list va;
		va_start(va, fmt);
		ssize res = c_str_fmt_va(buf, GEN_PRINTF_MAXLEN, fmt, va) - 1;
		va_end(va);
		return strbuilder_make_length(allocator, buf, res);
	}

	forceinline bool              make_space_for(char const* str, ssize add_len) { return strbuilder_make_space_for(this, str, add_len); }
	forceinline bool              append(char c)                                 { return strbuilder_append_char(this, c); }
	forceinline bool              append(char const* str)                        { return strbuilder_append_c_str(this, str); }
	forceinline bool              append(char const* str, ssize length)          { return strbuilder_append_c_str_len(this, str, length); }
	forceinline bool              append(Str str)                                { return strbuilder_append_str(this, str); }
	forceinline bool              append(const StrBuilder other)                 { return strbuilder_append_string(this, other); }
	forceinline ssize             avail_space() const                            { return strbuilder_avail_space(* this); }
	forceinline char*             back()                                         { return strbuilder_back(* this); }
	forceinline bool              contains(Str substring) const                  { return strbuilder_contains_str(* this, substring); }
	forceinline bool              contains(StrBuilder const& substring) const    { return strbuilder_contains_string(* this, substring); }
	forceinline ssize             capacity() const                               { return strbuilder_capacity(* this); }
	forceinline void              clear()                                        {        strbuilder_clear(* this); }
	forceinline StrBuilder        duplicate(AllocatorInfo allocator) const       { return strbuilder_duplicate(* this, allocator); }
	forceinline void              free()                                         {        strbuilder_free(this); }
	forceinline bool              is_equal(StrBuilder const& other) const        { return strbuilder_are_equal(* this, other); }
	forceinline bool              is_equal(Str other) const                      { return strbuilder_are_equal_str(* this, other); }
	forceinline ssize             length() const                                 { return strbuilder_length(* this); }
	forceinline b32               starts_with(Str substring) const               { return strbuilder_starts_with_str(* this, substring); }
	forceinline b32               starts_with(StrBuilder substring) const        { return strbuilder_starts_with_string(* this, substring); }
	forceinline void              skip_line()                                    {        strbuilder_skip_line(* this); }
	forceinline void              strip_space()                                  {        strbuilder_strip_space(* this); }
	forceinline Str               to_str()                                       { return { Data, strbuilder_length(*this) }; }
	forceinline void              trim(char const* cut_set)                      {        strbuilder_trim(* this, cut_set); }
	forceinline void              trim_space()                                   {        strbuilder_trim_space(* this); }
	forceinline StrBuilder        visualize_whitespace() const                   { return strbuilder_visualize_whitespace(* this); }
	forceinline StrBuilderHeader& get_header()                                   { return * strbuilder_get_header(* this); }

	bool append_fmt(char const* fmt, ...) {
		ssize res;
		char buf[GEN_PRINTF_MAXLEN] = { 0 };

		va_list va;
		va_start(va, fmt);
		res = c_str_fmt_va(buf, count_of(buf) - 1, fmt, va) - 1;
		va_end(va);

		return strbuilder_append_c_str_len(this, buf, res);
	}
#pragma endregion Member Mapping
#endif
};
#endif

forceinline char* strbuilder_begin(StrBuilder str)                   { return ((char*) str); }
forceinline char* strbuilder_end  (StrBuilder str)                   { return ((char*) str + strbuilder_length(str)); }
forceinline char* strbuilder_next (StrBuilder str, char const* iter) { return ((char*) iter + 1); }

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
forceinline char* begin(StrBuilder str)             { return ((char*) str); }
forceinline char* end  (StrBuilder str)             { return ((char*) str + strbuilder_length(str)); }
forceinline char* next (StrBuilder str, char* iter) { return ((char*) iter + 1); }
#endif

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
forceinline bool  make_space_for(StrBuilder& str, char const* to_append, ssize add_len);
forceinline bool  append(StrBuilder& str, char c);
forceinline bool  append(StrBuilder& str, char const* c_str_to_append);
forceinline bool  append(StrBuilder& str, char const* c_str_to_append, ssize length);
forceinline bool  append(StrBuilder& str, Str c_str_to_append);
forceinline bool  append(StrBuilder& str, const StrBuilder other);
forceinline bool  append_fmt(StrBuilder& str, char const* fmt, ...);
forceinline char& back(StrBuilder& str);
forceinline void  clear(StrBuilder& str);
forceinline void  free(StrBuilder& str);
#endif

forceinline
usize strbuilder_grow_formula(usize value) {
	// Using a very aggressive growth formula to reduce time mem_copying with recursive calls to append in this library.
	return 4 * value + 8;
}

forceinline
StrBuilder strbuilder_make_c_str(AllocatorInfo allocator, char const* str) {
	ssize length = str ? c_str_len(str) : 0;
	return strbuilder_make_length(allocator, str, length);
}

forceinline
StrBuilder strbuilder_make_str(AllocatorInfo allocator, Str str) {
	return strbuilder_make_length(allocator, str.Ptr, str.Len);
}

inline
StrBuilder strbuilder_fmt(AllocatorInfo allocator, char* buf, ssize buf_size, char const* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	ssize res = c_str_fmt_va(buf, buf_size, fmt, va) - 1;
	va_end(va);

	return strbuilder_make_length(allocator, buf, res);
}

inline
StrBuilder strbuilder_fmt_buf(AllocatorInfo allocator, char const* fmt, ...)
{
	local_persist thread_local
	PrintF_Buffer buf = struct_init(PrintF_Buffer, {0});

	va_list va;
	va_start(va, fmt);
	ssize res = c_str_fmt_va(buf, GEN_PRINTF_MAXLEN, fmt, va) -1;
	va_end(va);

	return strbuilder_make_length(allocator, buf, res);
}

inline
StrBuilder strbuilder_join(AllocatorInfo allocator, char const** parts, ssize num_parts, char const* glue)
{
	StrBuilder result = strbuilder_make_c_str(allocator, "");

	for (ssize idx = 0; idx < num_parts; ++idx)
	{
		strbuilder_append_c_str(& result, parts[idx]);

		if (idx < num_parts - 1)
			strbuilder_append_c_str(& result, glue);
	}

	return result;
}

forceinline
bool strbuilder_append_char(StrBuilder* str, char c) {
	GEN_ASSERT(str != nullptr);
	return strbuilder_append_c_str_len( str, (char const*)& c, (ssize)1);
}

forceinline
bool strbuilder_append_c_str(StrBuilder* str, char const* c_str_to_append) {
	GEN_ASSERT(str != nullptr);
	return strbuilder_append_c_str_len(str, c_str_to_append, c_str_len(c_str_to_append));
}

inline
bool strbuilder_append_c_str_len(StrBuilder* str, char const* c_str_to_append, ssize append_length)
{
	GEN_ASSERT(str != nullptr);
	if ( rcast(sptr, c_str_to_append) > 0)
	{
		ssize curr_len = strbuilder_length(* str);

		if ( ! strbuilder_make_space_for(str, c_str_to_append, append_length))
			return false;

		StrBuilderHeader* header = strbuilder_get_header(* str);

		char* Data = * str;
		mem_copy( Data + curr_len, c_str_to_append, append_length);

		Data[curr_len + append_length] = '\0';

		header->Length = curr_len + append_length;
	}
	return c_str_to_append != nullptr;
}

forceinline
bool strbuilder_append_str(StrBuilder* str, Str c_str_to_append) {
	GEN_ASSERT(str != nullptr);
	return strbuilder_append_c_str_len(str, c_str_to_append.Ptr, c_str_to_append.Len);
}

forceinline
bool strbuilder_append_string(StrBuilder* str, StrBuilder const other) {
	GEN_ASSERT(str != nullptr);
	return strbuilder_append_c_str_len(str, (char const*)other, strbuilder_length(other));
}

bool strbuilder_append_fmt(StrBuilder* str, char const* fmt, ...) {
	GEN_ASSERT(str != nullptr);
	ssize res;
	char buf[GEN_PRINTF_MAXLEN] = { 0 };

	va_list va;
	va_start(va, fmt);
	res = c_str_fmt_va(buf, count_of(buf) - 1, fmt, va) - 1;
	va_end(va);

	return strbuilder_append_c_str_len(str, (char const*)buf, res);
}

inline
bool strbuilder_are_equal_string(StrBuilder const lhs, StrBuilder const rhs)
{
	if (strbuilder_length(lhs) != strbuilder_length(rhs))
		return false;

	for (ssize idx = 0; idx < strbuilder_length(lhs); ++idx)
		if (lhs[idx] != rhs[idx])
			return false;

	return true;
}

inline
bool strbuilder_are_equal_str(StrBuilder const lhs, Str rhs)
{
	if (strbuilder_length(lhs) != (rhs.Len))
		return false;

	for (ssize idx = 0; idx < strbuilder_length(lhs); ++idx)
		if (lhs[idx] != rhs.Ptr[idx])
			return false;

	return true;
}

forceinline
ssize strbuilder_avail_space(StrBuilder const str) {
	StrBuilderHeader const* header = rcast(StrBuilderHeader const*, scast(char const*, str) - sizeof(StrBuilderHeader));
	return header->Capacity - header->Length;
}

forceinline
char* strbuilder_back(StrBuilder str) {
	return & (str)[strbuilder_length(str) - 1];
}

inline
bool strbuilder_contains_StrC(StrBuilder const str, Str substring)
{
	StrBuilderHeader const* header = rcast(StrBuilderHeader const*, scast(char const*, str) - sizeof(StrBuilderHeader));

	if (substring.Len > header->Length)
		return false;

	ssize main_len = header->Length;
	ssize sub_len  = substring.Len;

	for (ssize idx = 0; idx <= main_len - sub_len; ++idx)
	{
		if (c_str_compare_len(str + idx, substring.Ptr, sub_len) == 0)
			return true;
	}

	return false;
}

inline
bool strbuilder_contains_string(StrBuilder const str, StrBuilder const substring)
{
	StrBuilderHeader const* header = rcast(StrBuilderHeader const*, scast(char const*, str) - sizeof(StrBuilderHeader));

	if (strbuilder_length(substring) > header->Length)
		return false;

	ssize main_len = header->Length;
	ssize sub_len  = strbuilder_length(substring);

	for (ssize idx = 0; idx <= main_len - sub_len; ++idx)
	{
		if (c_str_compare_len(str + idx, substring, sub_len) == 0)
			return true;
	}

	return false;
}

forceinline
ssize strbuilder_capacity(StrBuilder const str) {
	StrBuilderHeader const* header = rcast(StrBuilderHeader const*, scast(char const*, str) - sizeof(StrBuilderHeader));
	return header->Capacity;
}

forceinline
void strbuilder_clear(StrBuilder str) {
	strbuilder_get_header(str)->Length = 0;
}

forceinline
StrBuilder strbuilder_duplicate(StrBuilder const str, AllocatorInfo allocator) {
	return strbuilder_make_length(allocator, str, strbuilder_length(str));
}

forceinline
void strbuilder_free(StrBuilder* str) {
	GEN_ASSERT(str != nullptr);
	if (! (* str))
		return;

	StrBuilderHeader* header = strbuilder_get_header(* str);
	allocator_free(header->Allocator, header);
}

forceinline
StrBuilderHeader* strbuilder_get_header(StrBuilder str) {
	return (StrBuilderHeader*)(scast(char*, str) - sizeof(StrBuilderHeader));
}

forceinline
ssize strbuilder_length(StrBuilder const str)
{
	StrBuilderHeader const* header = rcast(StrBuilderHeader const*, scast(char const*, str) - sizeof(StrBuilderHeader));
	return header->Length;
}

inline
bool strbuilder_make_space_for(StrBuilder* str, char const* to_append, ssize add_len)
{
	ssize available = strbuilder_avail_space(* str);

	if (available >= add_len) {
		return true;
	}
	else
	{
		ssize new_len, old_size, new_size;
		void* ptr;
		void* new_ptr;

		AllocatorInfo allocator = strbuilder_get_header(* str)->Allocator;
		StrBuilderHeader* header    = nullptr;

		new_len  = strbuilder_grow_formula(strbuilder_length(* str) + add_len);
		ptr      = strbuilder_get_header(* str);
		old_size = size_of(StrBuilderHeader) + strbuilder_length(* str) + 1;
		new_size = size_of(StrBuilderHeader) + new_len + 1;

		new_ptr = resize(allocator, ptr, old_size, new_size);

		if (new_ptr == nullptr)
			return false;

		header = rcast(StrBuilderHeader*, new_ptr);
		header->Allocator = allocator;
		header->Capacity  = new_len;

		char** Data = rcast(char**, str);
		* Data = rcast(char*, header + 1);

		return true;
	}
}

forceinline
b32 strbuilder_starts_with_str(StrBuilder const str, Str substring) {
	if (substring.Len > strbuilder_length(str))
	return false;

	b32 result = c_str_compare_len(str, substring.Ptr, substring.Len) == 0;
	return result;
}

forceinline
b32 strbuilder_starts_with_string(StrBuilder const str, StrBuilder substring) {
	if (strbuilder_length(substring) > strbuilder_length(str))
		return false;

	b32 result = c_str_compare_len(str, substring, strbuilder_length(substring) - 1) == 0;
	return result;
}

inline
void strbuilder_skip_line(StrBuilder str)
{
#define current (*scanner)
	char* scanner = str;
	while (current != '\r' && current != '\n') {
		++scanner;
	}

	s32 new_length = scanner - str;

	if (current == '\r') {
		new_length += 1;
	}

	mem_move((char*)str, scanner, new_length);

	StrBuilderHeader* header = strbuilder_get_header(str);
	header->Length = new_length;
#undef current
}

inline
void strbuilder_strip_space(StrBuilder str)
{
	char* write_pos = str;
	char* read_pos  = str;

	while (* read_pos)
	{
		if (! char_is_space(* read_pos))
		{
   			* write_pos = * read_pos;
			write_pos++;
		}
		read_pos++;
	}
   write_pos[0] = '\0';  // Null-terminate the modified string

	// Update the length if needed
	strbuilder_get_header(str)->Length = write_pos - str;
}

forceinline
Str strbuilder_to_str(StrBuilder str) {
	Str result = { (char const*)str, strbuilder_length(str) };
	return result;
}

inline
void strbuilder_trim(StrBuilder str, char const* cut_set)
{
	ssize len = 0;

	char* start_pos = str;
	char* end_pos   = scast(char*, str) + strbuilder_length(str) - 1;

	while (start_pos <= end_pos && char_first_occurence(cut_set, *start_pos))
	start_pos++;

	while (end_pos > start_pos && char_first_occurence(cut_set, *end_pos))
	end_pos--;

	len = scast(ssize, (start_pos > end_pos) ? 0 : ((end_pos - start_pos) + 1));

	if (str != start_pos)
		mem_move(str, start_pos, len);

	str[len] = '\0';

	strbuilder_get_header(str)->Length = len;
}

forceinline
void strbuilder_trim_space(StrBuilder str) {
	strbuilder_trim(str, " \t\r\n\v\f");
}

inline
StrBuilder strbuilder_visualize_whitespace(StrBuilder const str)
{
	StrBuilderHeader* header = (StrBuilderHeader*)(scast(char const*, str) - sizeof(StrBuilderHeader));
	StrBuilder        result = strbuilder_make_reserve(header->Allocator, strbuilder_length(str) * 2); // Assume worst case for space requirements.

	for (char const* c = strbuilder_begin(str); c != strbuilder_end(str); c = strbuilder_next(str, c))
	switch ( * c )
	{
		case ' ':
			strbuilder_append_str(& result, txt("·"));
		break;
		case '\t':
			strbuilder_append_str(& result, txt("→"));
		break;
		case '\n':
			strbuilder_append_str(& result, txt("↵"));
		break;
		case '\r':
			strbuilder_append_str(& result, txt("⏎"));
		break;
		case '\v':
			strbuilder_append_str(& result, txt("⇕"));
		break;
		case '\f':
			strbuilder_append_str(& result, txt("⌂"));
		break;
		default:
			strbuilder_append_char(& result, * c);
		break;
	}

	return result;
}
#pragma endregion StrBuilder

#if GEN_COMPILER_CPP
struct StrBuilder_POD {
	char* Data;
};
static_assert( sizeof( StrBuilder_POD ) == sizeof( StrBuilder ), "StrBuilder is not a POD" );
#endif

forceinline
Str str_duplicate(Str str, AllocatorInfo allocator) {
	Str result = strbuilder_to_str( strbuilder_make_length(allocator, str.Ptr, str.Len));
	return result;
}

inline
Str str_visualize_whitespace(Str str, AllocatorInfo allocator)
{
	StrBuilder result = strbuilder_make_reserve(allocator, str.Len * 2); // Assume worst case for space requirements.
	for (char const* c = str_begin(str); c != str_end(str); c = str_next(str, c))
	switch ( * c )
	{
		case ' ':
			strbuilder_append_str(& result, txt("·"));
		break;
		case '\t':
			strbuilder_append_str(& result, txt("→"));
		break;
		case '\n':
			strbuilder_append_str(& result, txt("↵"));
		break;
		case '\r':
			strbuilder_append_str(& result, txt("⏎"));
		break;
		case '\v':
			strbuilder_append_str(& result, txt("⇕"));
		break;
		case '\f':
			strbuilder_append_str(& result, txt("⌂"));
		break;
		default:
			strbuilder_append_char(& result, * c);
		break;
}
	return strbuilder_to_str(result);
}

// Represents strings cached with the string table.
// Should never be modified, if changed string is desired, cache_string( str ) another.
typedef Str StrCached;

// Implements basic string interning. Data structure is based off the ZPL Hashtable.
typedef HashTable(StrCached) StringTable;
#pragma endregion Strings

#pragma region File Handling

enum FileModeFlag
{
	EFileMode_READ   = bit( 0 ),
	EFileMode_WRITE  = bit( 1 ),
	EFileMode_APPEND = bit( 2 ),
	EFileMode_RW     = bit( 3 ),
	GEN_FILE_MODES   = EFileMode_READ | EFileMode_WRITE | EFileMode_APPEND | EFileMode_RW,
};

// NOTE: Only used internally and for the file operations
enum SeekWhenceType
{
	ESeekWhence_BEGIN   = 0,
	ESeekWhence_CURRENT = 1,
	ESeekWhence_END     = 2,
};

enum FileError
{
	EFileError_NONE,
	EFileError_INVALID,
	EFileError_INVALID_FILENAME,
	EFileError_EXISTS,
	EFileError_NOT_EXISTS,
	EFileError_PERMISSION,
	EFileError_TRUNCATION_FAILURE,
	EFileError_NOT_EMPTY,
	EFileError_NAME_TOO_LONG,
	EFileError_UNKNOWN,
};

union FileDescriptor
{
	void* p;
	sptr  i;
	uptr  u;
};

typedef u32                   FileMode;
typedef struct FileOperations FileOperations;

#define GEN_FILE_OPEN_PROC( name )     FileError name( FileDescriptor* fd, FileOperations* ops, FileMode mode, char const* filename )
#define GEN_FILE_READ_AT_PROC( name )  b32 name( FileDescriptor fd, void* buffer, ssize size, s64 offset, ssize* bytes_read, b32 stop_at_newline )
#define GEN_FILE_WRITE_AT_PROC( name ) b32 name( FileDescriptor fd, mem_ptr_const buffer, ssize size, s64 offset, ssize* bytes_written )
#define GEN_FILE_SEEK_PROC( name )     b32 name( FileDescriptor fd, s64 offset, SeekWhenceType whence, s64* new_offset )
#define GEN_FILE_CLOSE_PROC( name )    void name( FileDescriptor fd )

typedef GEN_FILE_OPEN_PROC( file_open_proc );
typedef GEN_FILE_READ_AT_PROC( FileReadProc );
typedef GEN_FILE_WRITE_AT_PROC( FileWriteProc );
typedef GEN_FILE_SEEK_PROC( FileSeekProc );
typedef GEN_FILE_CLOSE_PROC( FileCloseProc );

struct FileOperations
{
	FileReadProc*  read_at;
	FileWriteProc* write_at;
	FileSeekProc*  seek;
	FileCloseProc* close;
};

extern FileOperations const default_file_operations;

typedef u64 FileTime;

enum DirType
{
	GEN_DIR_TYPE_FILE,
	GEN_DIR_TYPE_FOLDER,
	GEN_DIR_TYPE_UNKNOWN,
};

struct DirInfo;

struct DirEntry
{
	char const* filename;
	DirInfo*    dir_info;
	u8          type;
};

struct DirInfo
{
	char const* fullpath;
	DirEntry*   entries;    // zpl_array

	// Internals
	char** filenames;    // zpl_array
	StrBuilder buf;
};

struct FileInfo
{
	FileOperations ops;
	FileDescriptor fd;
	b32            is_temp;

	char const* filename;
	FileTime    last_write_time;
	DirEntry*   dir;
};

enum FileStandardType
{
	EFileStandard_INPUT,
	EFileStandard_OUTPUT,
	EFileStandard_ERROR,

	EFileStandard_COUNT,
};

/**
	* Get standard file I/O.
	* @param  std Check zpl_file_standard_type
	* @return     File handle to standard I/O
	*/
FileInfo* file_get_standard( FileStandardType std );

/**
	* Closes the file
	* @param  file
	*/
FileError file_close( FileInfo* file );

/**
	* Returns the currently opened file's name
	* @param  file
	*/
inline
	char const* file_name( FileInfo* file )
{
	return file->filename ? file->filename : "";
}

/**
	* Opens a file
	* @param  file
	* @param  filename
	*/
FileError file_open( FileInfo* file, char const* filename );

/**
	* Opens a file using a specified mode
	* @param  file
	* @param  mode     Access mode to use
	* @param  filename
	*/
FileError file_open_mode( FileInfo* file, FileMode mode, char const* filename );

/**
	* Reads from a file
	* @param  file
	* @param  buffer Buffer to read to
	* @param  size   Size to read
	*/
b32 file_read( FileInfo* file, void* buffer, ssize size );

/**
	* Reads file at a specific offset
	* @param  file
	* @param  buffer     Buffer to read to
	* @param  size       Size to read
	* @param  offset     Offset to read from
	* @param  bytes_read How much data we've actually read
	*/
b32 file_read_at( FileInfo* file, void* buffer, ssize size, s64 offset );

/**
	* Reads file safely
	* @param  file
	* @param  buffer     Buffer to read to
	* @param  size       Size to read
	* @param  offset     Offset to read from
	* @param  bytes_read How much data we've actually read
	*/
b32 file_read_at_check( FileInfo* file, void* buffer, ssize size, s64 offset, ssize* bytes_read );

typedef struct FileContents FileContents;
struct FileContents
{
	AllocatorInfo allocator;
	void*         data;
	ssize            size;
};

constexpr b32 file_zero_terminate    = true;
constexpr b32 file_no_zero_terminate = false;

/**
	* Reads the whole file contents
	* @param  a              Allocator to use
	* @param  zero_terminate End the read data with null terminator
	* @param  filepath       Path to the file
	* @return                File contents data
	*/
FileContents file_read_contents( AllocatorInfo a, b32 zero_terminate, char const* filepath );

/**
	* Returns a size of the file
	* @param  file
	* @return      File size
	*/
s64 file_size( FileInfo* file );

/**
	* Seeks the file cursor from the beginning of file to a specific position
	* @param  file
	* @param  offset Offset to seek to
	*/
s64 file_seek( FileInfo* file, s64 offset );

/**
	* Seeks the file cursor to the end of the file
	* @param  file
	*/
s64 file_seek_to_end( FileInfo* file );

/**
	* Returns the length from the beginning of the file we've read so far
	* @param  file
	* @return      Our current position in file
	*/
s64 file_tell( FileInfo* file );

/**
	* Writes to a file
	* @param  file
	* @param  buffer Buffer to read from
	* @param  size   Size to read
	*/
b32 file_write( FileInfo* file, void const* buffer, ssize size );

/**
	* Writes to file at a specific offset
	* @param  file
	* @param  buffer        Buffer to read from
	* @param  size          Size to write
	* @param  offset        Offset to write to
	* @param  bytes_written How much data we've actually written
	*/
b32 file_write_at( FileInfo* file, void const* buffer, ssize size, s64 offset );

/**
	* Writes to file safely
	* @param  file
	* @param  buffer        Buffer to read from
	* @param  size          Size to write
	* @param  offset        Offset to write to
	* @param  bytes_written How much data we've actually written
	*/
b32 file_write_at_check( FileInfo* file, void const* buffer, ssize size, s64 offset, ssize* bytes_written );

enum FileStreamFlags : u32
{
	/* Allows us to write to the buffer directly. Beware: you can not append a new data! */
	EFileStream_WRITABLE = bit( 0 ),

	/* Clones the input buffer so you can write (zpl_file_write*) data into it. */
	/* Since we work with a clone, the buffer size can dynamically grow as well. */
	EFileStream_CLONE_WRITABLE = bit( 1 ),

	EFileStream_UNDERLYING = GEN_U32_MAX,
};

/**
	* Opens a new memory stream
	* @param file
	* @param allocator
	*/
b8 file_stream_new( FileInfo* file, AllocatorInfo allocator );

/**
	* Opens a memory stream over an existing buffer
	* @param  file
	* @param  allocator
	* @param  buffer   Memory to create stream from
	* @param  size     Buffer's size
	* @param  flags
	*/
b8 file_stream_open( FileInfo* file, AllocatorInfo allocator, u8* buffer, ssize size, FileStreamFlags flags );

/**
	* Retrieves the stream's underlying buffer and buffer size.
	* @param file memory stream
	* @param size (Optional) buffer size
	*/
u8* file_stream_buf( FileInfo* file, ssize* size );

extern FileOperations const memory_file_operations;

inline
s64 file_seek( FileInfo* f, s64 offset )
{
	s64 new_offset = 0;

	if ( ! f->ops.read_at )
		f->ops = default_file_operations;

	f->ops.seek( f->fd, offset, ESeekWhence_BEGIN, &new_offset );

	return new_offset;
}

inline
s64 file_seek_to_end( FileInfo* f )
{
	s64 new_offset = 0;

	if ( ! f->ops.read_at )
		f->ops = default_file_operations;

	f->ops.seek( f->fd, 0, ESeekWhence_END, &new_offset );

	return new_offset;
}

inline
s64 file_tell( FileInfo* f )
{
	s64 new_offset = 0;

	if ( ! f->ops.read_at )
		f->ops = default_file_operations;

	f->ops.seek( f->fd, 0, ESeekWhence_CURRENT, &new_offset );

	return new_offset;
}

inline
b32 file_read( FileInfo* f, void* buffer, ssize size )
{
	s64 cur_offset = file_tell( f );
	b32 result     = file_read_at( f, buffer, size, file_tell( f ) );
	file_seek( f, cur_offset + size );
	return result;
}

inline
b32 file_read_at( FileInfo* f, void* buffer, ssize size, s64 offset )
{
	return file_read_at_check( f, buffer, size, offset, NULL );
}

inline
b32 file_read_at_check( FileInfo* f, void* buffer, ssize size, s64 offset, ssize* bytes_read )
{
	if ( ! f->ops.read_at )
		f->ops = default_file_operations;
	return f->ops.read_at( f->fd, buffer, size, offset, bytes_read, false );
}

inline
b32 file_write( FileInfo* f, void const* buffer, ssize size )
{
	s64 cur_offset = file_tell( f );
	b32 result     = file_write_at( f, buffer, size, file_tell( f ) );

	file_seek( f, cur_offset + size );

	return result;
}

inline
b32 file_write_at( FileInfo* f, void const* buffer, ssize size, s64 offset )
{
	return file_write_at_check( f, buffer, size, offset, NULL );
}

inline
b32 file_write_at_check( FileInfo* f, void const* buffer, ssize size, s64 offset, ssize* bytes_written )
{
	if ( ! f->ops.read_at )
		f->ops = default_file_operations;

	return f->ops.write_at( f->fd, buffer, size, offset, bytes_written );
}

#pragma endregion File Handling

#pragma region Timing

#ifdef GEN_BENCHMARK
//! Return CPU timestamp.
u64 read_cpu_time_stamp_counter( void );

//! Return relative time (in seconds) since the application start.
f64 time_rel( void );

//! Return relative time since the application start.
u64 time_rel_ms( void );
#endif

#pragma endregion Timing

#pragma region ADT

enum ADT_Type : u32
{
	EADT_TYPE_UNINITIALISED, /* node was not initialised, this is a programming error! */
	EADT_TYPE_ARRAY,
	EADT_TYPE_OBJECT,
	EADT_TYPE_STRING,
	EADT_TYPE_MULTISTRING,
	EADT_TYPE_INTEGER,
	EADT_TYPE_REAL,
};

enum ADT_Props : u32
{
	EADT_PROPS_NONE,
	EADT_PROPS_NAN,
	EADT_PROPS_NAN_NEG,
	EADT_PROPS_INFINITY,
	EADT_PROPS_INFINITY_NEG,
	EADT_PROPS_FALSE,
	EADT_PROPS_TRUE,
	EADT_PROPS_NULL,
	EADT_PROPS_IS_EXP,
	EADT_PROPS_IS_HEX,

	// Used internally so that people can fill in real numbers they plan to write.
	EADT_PROPS_IS_PARSED_REAL,
};

enum ADT_NamingStyle : u32
{
	EADT_NAME_STYLE_DOUBLE_QUOTE,
	EADT_NAME_STYLE_SINGLE_QUOTE,
	EADT_NAME_STYLE_NO_QUOTES,
};

enum ADT_AssignStyle : u32
{
	EADT_ASSIGN_STYLE_COLON,
	EADT_ASSIGN_STYLE_EQUALS,
	EADT_ASSIGN_STYLE_LINE,
};

enum ADT_DelimStyle : u32
{
	EADT_DELIM_STYLE_COMMA,
	EADT_DELIM_STYLE_LINE,
	EADT_DELIM_STYLE_NEWLINE,
};

enum ADT_Error : u32
{
	EADT_ERROR_NONE,
	EADT_ERROR_INTERNAL,
	EADT_ERROR_ALREADY_CONVERTED,
	EADT_ERROR_INVALID_TYPE,
	EADT_ERROR_OUT_OF_MEMORY,
};

struct ADT_Node
{
	char const*      name;
	struct ADT_Node* parent;

	/* properties */
	ADT_Type type  : 4;
	u8 props : 4;
#ifndef GEN_PARSER_DISABLE_ANALYSIS
	u8 cfg_mode          : 1;
	u8 name_style        : 2;
	u8 assign_style      : 2;
	u8 delim_style       : 2;
	u8 delim_line_width  : 4;
	u8 assign_line_width : 4;
#endif

	/* adt data */
	union
	{
		char const*     string;
		Array(ADT_Node) nodes;    ///< zpl_array

		struct
		{
			union
			{
				f64 real;
				s64 integer;
			};

#ifndef GEN_PARSER_DISABLE_ANALYSIS
			/* number analysis */
			s32 base;
			s32 base2;
			u8  base2_offset : 4;
			s8  exp          : 4;
			u8  neg_zero     : 1;
			u8  lead_digit   : 1;
#endif
		};
	};
};

/* ADT NODE LIMITS
	* delimiter and assignment segment width is limited to 128 whitespace symbols each.
	* real number limits decimal position to 128 places.
	* real number exponent is limited to 64 digits.
	*/

/**
	* @brief Initialise an ADT object or array
	*
	* @param node
	* @param backing Memory allocator used for descendants
	* @param name Node's name
	* @param is_array
	* @return error code
	*/
u8 adt_make_branch( ADT_Node* node, AllocatorInfo backing, char const* name, b32 is_array );

/**
	* @brief Destroy an ADT branch and its descendants
	*
	* @param node
	* @return error code
	*/
u8 adt_destroy_branch( ADT_Node* node );

/**
	* @brief Initialise an ADT leaf
	*
	* @param node
	* @param name Node's name
	* @param type Node's type (use zpl_adt_make_branch for container nodes)
	* @return error code
	*/
u8 adt_make_leaf( ADT_Node* node, char const* name, ADT_Type type );


/**
	* @brief Fetch a node using provided URI string.
	*
	* This method uses a basic syntax to fetch a node from the ADT. The following features are available
	* to retrieve the data:
	*
	* - "a/b/c" navigates through objects "a" and "b" to get to "c"
	* - "arr/[foo=123]/bar" iterates over "arr" to find any object with param "foo" that matches the value "123", then gets its field called "bar"
	* - "arr/3" retrieves the 4th element in "arr"
	* - "arr/[apple]" retrieves the first element of value "apple" in "arr"
	*
	* @param node ADT node
	* @param uri Locator string as described above
	* @return zpl_adt_node*
	*
	* @see code/apps/examples/json_get.c
	*/
ADT_Node* adt_query( ADT_Node* node, char const* uri );

/**
	* @brief Find a field node within an object by the given name.
	*
	* @param node
	* @param name
	* @param deep_search Perform search recursively
	* @return zpl_adt_node * node
	*/
ADT_Node* adt_find( ADT_Node* node, char const* name, b32 deep_search );

/**
	* @brief Allocate an unitialised node within a container at a specified index.
	*
	* @param parent
	* @param index
	* @return zpl_adt_node * node
	*/
ADT_Node* adt_alloc_at( ADT_Node* parent, ssize index );

/**
	* @brief Allocate an unitialised node within a container.
	*
	* @param parent
	* @return zpl_adt_node * node
	*/
ADT_Node* adt_alloc( ADT_Node* parent );

/**
	* @brief Move an existing node to a new container at a specified index.
	*
	* @param node
	* @param new_parent
	* @param index
	* @return zpl_adt_node * node
	*/
ADT_Node* adt_move_node_at( ADT_Node* node, ADT_Node* new_parent, ssize index );

/**
	* @brief Move an existing node to a new container.
	*
	* @param node
	* @param new_parent
	* @return zpl_adt_node * node
	*/
ADT_Node* adt_move_node( ADT_Node* node, ADT_Node* new_parent );

/**
	* @brief Swap two nodes.
	*
	* @param node
	* @param other_node
	* @return
	*/
void adt_swap_nodes( ADT_Node* node, ADT_Node* other_node );

/**
	* @brief Remove node from container.
	*
	* @param node
	* @return
	*/
void adt_remove_node( ADT_Node* node );

/**
	* @brief Initialise a node as an object
	*
	* @param obj
	* @param name
	* @param backing
	* @return
	*/
b8 adt_set_obj( ADT_Node* obj, char const* name, AllocatorInfo backing );

/**
	* @brief Initialise a node as an array
	*
	* @param obj
	* @param name
	* @param backing
	* @return
	*/
b8 adt_set_arr( ADT_Node* obj, char const* name, AllocatorInfo backing );

/**
	* @brief Initialise a node as a string
	*
	* @param obj
	* @param name
	* @param value
	* @return
	*/
b8 adt_set_str( ADT_Node* obj, char const* name, char const* value );

/**
	* @brief Initialise a node as a float
	*
	* @param obj
	* @param name
	* @param value
	* @return
	*/
b8 adt_set_flt( ADT_Node* obj, char const* name, f64 value );

/**
	* @brief Initialise a node as a signed integer
	*
	* @param obj
	* @param name
	* @param value
	* @return
	*/
b8 adt_set_int( ADT_Node* obj, char const* name, s64 value );

/**
	* @brief Append a new node to a container as an object
	*
	* @param parent
	* @param name
	* @return*
	*/
ADT_Node* adt_append_obj( ADT_Node* parent, char const* name );

/**
	* @brief Append a new node to a container as an array
	*
	* @param parent
	* @param name
	* @return*
	*/
ADT_Node* adt_append_arr( ADT_Node* parent, char const* name );

/**
	* @brief Append a new node to a container as a string
	*
	* @param parent
	* @param name
	* @param value
	* @return*
	*/
ADT_Node* adt_append_str( ADT_Node* parent, char const* name, char const* value );

/**
	* @brief Append a new node to a container as a float
	*
	* @param parent
	* @param name
	* @param value
	* @return*
	*/
ADT_Node* adt_append_flt( ADT_Node* parent, char const* name, f64 value );

/**
	* @brief Append a new node to a container as a signed integer
	*
	* @param parent
	* @param name
	* @param value
	* @return*
	*/
ADT_Node* adt_append_int( ADT_Node* parent, char const* name, s64 value );

/* parser helpers */

/**
	* @brief Parses a text and stores the result into an unitialised node.
	*
	* @param node
	* @param base
	* @return*
	*/
char* adt_parse_number( ADT_Node* node, char* base );

/**
	* @brief Parses a text and stores the result into an unitialised node.
	* This function expects the entire input to be a number.
	*
	* @param node
	* @param base
	* @return*
	*/
char* adt_parse_number_strict( ADT_Node* node, char* base_str );

/**
	* @brief Parses and converts an existing string node into a number.
	*
	* @param node
	* @return
	*/
ADT_Error adt_c_str_to_number( ADT_Node* node );

/**
	* @brief Parses and converts an existing string node into a number.
	* This function expects the entire input to be a number.
	*
	* @param node
	* @return
	*/
ADT_Error adt_c_str_to_number_strict( ADT_Node* node );

/**
	* @brief Prints a number into a file stream.
	*
	* The provided file handle can also be a memory mapped stream.
	*
	* @see zpl_file_stream_new
	* @param file
	* @param node
	* @return
	*/
ADT_Error adt_print_number( FileInfo* file, ADT_Node* node );

/**
	* @brief Prints a string into a file stream.
	*
	* The provided file handle can also be a memory mapped stream.
	*
	* @see zpl_file_stream_new
	* @param file
	* @param node
	* @param escaped_chars
	* @param escape_symbol
	* @return
	*/
ADT_Error adt_print_string( FileInfo* file, ADT_Node* node, char const* escaped_chars, char const* escape_symbol );

#pragma endregion ADT

#pragma region CSV

enum CSV_Error : u32
{
	ECSV_Error__NONE,
	ECSV_Error__INTERNAL,
	ECSV_Error__UNEXPECTED_END_OF_INPUT,
	ECSV_Error__MISMATCHED_ROWS,
};

typedef ADT_Node CSV_Object;

u8   csv_parse( CSV_Object* root, char* text, AllocatorInfo allocator, b32 has_header );
u8   csv_parse_delimiter( CSV_Object* root, char* text, AllocatorInfo allocator, b32 has_header, char delim );
void csv_free( CSV_Object* obj );

void   csv_write( FileInfo* file, CSV_Object* obj );
StrBuilder csv_write_string( AllocatorInfo a, CSV_Object* obj );
void   csv_write_delimiter( FileInfo* file, CSV_Object* obj, char delim );
StrBuilder csv_write_strbuilder_delimiter( AllocatorInfo a, CSV_Object* obj, char delim );

/* inline */

inline
u8 csv_parse( CSV_Object* root, char* text, AllocatorInfo allocator, b32 has_header )
{
	return csv_parse_delimiter( root, text, allocator, has_header, ',' );
}

inline
void csv_write( FileInfo* file, CSV_Object* obj )
{
	csv_write_delimiter( file, obj, ',' );
}

inline
StrBuilder csv_write_string( AllocatorInfo a, CSV_Object* obj )
{
	return csv_write_strbuilder_delimiter( a, obj, ',' );
}

#pragma endregion CSV

GEN_NS_END

// GEN_ROLL_OWN_DEPENDENCIES
#endif

GEN_NS_BEGIN

#pragma region Types

/*
 ________                                              __    __      ________
|        \                                            |  \  |  \    |        \
| ▓▓▓▓▓▓▓▓_______  __    __ ______ ____   _______     | ▓▓\ | ▓▓     \▓▓▓▓▓▓▓▓__    __  ______   ______   _______
| ▓▓__   |       \|  \  |  \      \    \ /       \    | ▓▓▓\| ▓▓       | ▓▓  |  \  |  \/      \ /      \ /       \
| ▓▓  \  | ▓▓▓▓▓▓▓\ ▓▓  | ▓▓ ▓▓▓▓▓▓\▓▓▓▓\  ▓▓▓▓▓▓▓    | ▓▓▓▓\ ▓▓       | ▓▓  | ▓▓  | ▓▓  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓
| ▓▓▓▓▓  | ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓ | ▓▓ | ▓▓\▓▓    \     | ▓▓\▓▓ ▓▓       | ▓▓  | ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓\▓▓    \
| ▓▓_____| ▓▓  | ▓▓ ▓▓__/ ▓▓ ▓▓ | ▓▓ | ▓▓_\▓▓▓▓▓▓\    | ▓▓ \▓▓▓▓       | ▓▓  | ▓▓__/ ▓▓ ▓▓__/ ▓▓ ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\
| ▓▓     \ ▓▓  | ▓▓\▓▓    ▓▓ ▓▓ | ▓▓ | ▓▓       ▓▓    | ▓▓  \▓▓▓       | ▓▓   \▓▓    ▓▓ ▓▓    ▓▓\▓▓     \       ▓▓
 \▓▓▓▓▓▓▓▓\▓▓   \▓▓ \▓▓▓▓▓▓ \▓▓  \▓▓  \▓▓\▓▓▓▓▓▓▓      \▓▓   \▓▓        \▓▓   _\▓▓▓▓▓▓▓ ▓▓▓▓▓▓▓  \▓▓▓▓▓▓▓\▓▓▓▓▓▓▓
                                                                             |  \__| ▓▓ ▓▓
                                                                              \▓▓    ▓▓ ▓▓
                                                                               \▓▓▓▓▓▓ \▓▓

*/

using LogFailType = ssize(*)(char const*, ...);

// By default this library will either crash or exit if an error is detected while generating codes.
// Even if set to not use GEN_FATAL, GEN_FATAL will still be used for memory failures as the library is unusable when they occur.
#ifdef GEN_DONT_USE_FATAL
	#define log_failure log_fmt
#else
	#define log_failure GEN_FATAL
#endif

enum AccessSpec : u32
{
	AccessSpec_Default,
	AccessSpec_Private,
	AccessSpec_Protected,
	AccessSpec_Public,

	AccessSpec_Num_AccessSpec,
	AccessSpec_Invalid,

	AccessSpec_SizeDef = GEN_U32_MAX,
};
static_assert( size_of(AccessSpec) == size_of(u32), "AccessSpec not u32 size" );

inline
Str access_spec_to_str( AccessSpec type )
{
	local_persist
	Str lookup[ (u32)AccessSpec_Num_AccessSpec ] = {
		{ "",        sizeof( "" )        - 1 },
		{ "private", sizeof("prviate")   - 1 },
		{ "private", sizeof("protected") - 1 },
		{ "public",  sizeof("public")    - 1 },
	};

	Str invalid = { "Invalid", sizeof("Invalid") - 1 };
	if ( type > AccessSpec_Public )
		return invalid;

	return lookup[ (u32)type ];
}

enum CodeFlag : u32
{
	CodeFlag_None          = 0,
	CodeFlag_FunctionType  = bit(0),
	CodeFlag_ParamPack     = bit(1),
	CodeFlag_Module_Export = bit(2),
	CodeFlag_Module_Import = bit(3),

	CodeFlag_SizeDef = GEN_U32_MAX,
};
static_assert( size_of(CodeFlag) == size_of(u32), "CodeFlag not u32 size" );

// Used to indicate if enum definitoin is an enum class or regular enum.
enum EnumDecl : u8
{
	EnumDecl_Regular,
	EnumDecl_Class,

	EnumT_SizeDef = GEN_U8_MAX,
};
typedef u8 EnumT;

enum ModuleFlag : u32
{
	ModuleFlag_None    = 0,
	ModuleFlag_Export  = bit(0),
	ModuleFlag_Import  = bit(1),

	Num_ModuleFlags,
	ModuleFlag_Invalid,

	ModuleFlag_SizeDef = GEN_U32_MAX,
};
static_assert( size_of(ModuleFlag) == size_of(u32), "ModuleFlag not u32 size" );

inline
Str module_flag_to_str( ModuleFlag flag )
{
	local_persist
	Str lookup[ (u32)Num_ModuleFlags ] = {
		{ "__none__", sizeof("__none__") - 1 },
		{ "export",   sizeof("export")   - 1 },
		{ "import",   sizeof("import")   - 1 },
	};

	local_persist
	Str invalid_flag = { "invalid", sizeof("invalid") };
	if ( flag > ModuleFlag_Import )
		return invalid_flag;

	return lookup[ (u32)flag ];
}

enum EPreprocessCond : u32
{
	PreprocessCond_If,
	PreprocessCond_IfDef,
	PreprocessCond_IfNotDef,
	PreprocessCond_ElIf,

	EPreprocessCond_SizeDef = GEN_U32_MAX,
};
static_assert( size_of(EPreprocessCond) == size_of(u32), "EPreprocessCond not u32 size" );

enum ETypenameTag : u16
{
	Tag_None,
	Tag_Class,
	Tag_Enum,
	Tag_Struct,
	Tag_Union,

	Tag_UnderlyingType = GEN_U16_MAX,
};
static_assert( size_of(ETypenameTag) == size_of(u16), "ETypenameTag is not u16 size");

enum CodeType : u32
{
	CT_Invalid,
	CT_Untyped,
	CT_NewLine,
	CT_Comment,
	CT_Access_Private,
	CT_Access_Protected,
	CT_Access_Public,
	CT_PlatformAttributes,
	CT_Class,
	CT_Class_Fwd,
	CT_Class_Body,
	CT_Constructor,
	CT_Constructor_Fwd,
	CT_Destructor,
	CT_Destructor_Fwd,
	CT_Enum,
	CT_Enum_Fwd,
	CT_Enum_Body,
	CT_Enum_Class,
	CT_Enum_Class_Fwd,
	CT_Execution,
	CT_Export_Body,
	CT_Extern_Linkage,
	CT_Extern_Linkage_Body,
	CT_Friend,
	CT_Function,
	CT_Function_Fwd,
	CT_Function_Body,
	CT_Global_Body,
	CT_Module,
	CT_Namespace,
	CT_Namespace_Body,
	CT_Operator,
	CT_Operator_Fwd,
	CT_Operator_Member,
	CT_Operator_Member_Fwd,
	CT_Operator_Cast,
	CT_Operator_Cast_Fwd,
	CT_Parameters,
	CT_Preprocess_Define,
	CT_Preprocess_Include,
	CT_Preprocess_If,
	CT_Preprocess_IfDef,
	CT_Preprocess_IfNotDef,
	CT_Preprocess_ElIf,
	CT_Preprocess_Else,
	CT_Preprocess_EndIf,
	CT_Preprocess_Pragma,
	CT_Specifiers,
	CT_Struct,
	CT_Struct_Fwd,
	CT_Struct_Body,
	CT_Template,
	CT_Typedef,
	CT_Typename,
	CT_Union,
	CT_Union_Fwd,
	CT_Union_Body,
	CT_Using,
	CT_Using_Namespace,
	CT_Variable,
	CT_NumTypes,
	CT_UnderlyingType = GEN_U32_MAX
};

inline Str codetype_to_str( CodeType type )
{
	local_persist Str lookup[61] = {
		{ "Invalid",             sizeof( "Invalid" ) - 1             },
		{ "Untyped",             sizeof( "Untyped" ) - 1             },
		{ "NewLine",             sizeof( "NewLine" ) - 1             },
		{ "Comment",             sizeof( "Comment" ) - 1             },
		{ "Access_Private",      sizeof( "Access_Private" ) - 1      },
		{ "Access_Protected",    sizeof( "Access_Protected" ) - 1    },
		{ "Access_Public",       sizeof( "Access_Public" ) - 1       },
		{ "PlatformAttributes",  sizeof( "PlatformAttributes" ) - 1  },
		{ "Class",               sizeof( "Class" ) - 1               },
		{ "Class_Fwd",           sizeof( "Class_Fwd" ) - 1           },
		{ "Class_Body",          sizeof( "Class_Body" ) - 1          },
		{ "Constructor",         sizeof( "Constructor" ) - 1         },
		{ "Constructor_Fwd",     sizeof( "Constructor_Fwd" ) - 1     },
		{ "Destructor",          sizeof( "Destructor" ) - 1          },
		{ "Destructor_Fwd",      sizeof( "Destructor_Fwd" ) - 1      },
		{ "Enum",                sizeof( "Enum" ) - 1                },
		{ "Enum_Fwd",            sizeof( "Enum_Fwd" ) - 1            },
		{ "Enum_Body",           sizeof( "Enum_Body" ) - 1           },
		{ "Enum_Class",          sizeof( "Enum_Class" ) - 1          },
		{ "Enum_Class_Fwd",      sizeof( "Enum_Class_Fwd" ) - 1      },
		{ "Execution",           sizeof( "Execution" ) - 1           },
		{ "Export_Body",         sizeof( "Export_Body" ) - 1         },
		{ "Extern_Linkage",      sizeof( "Extern_Linkage" ) - 1      },
		{ "Extern_Linkage_Body", sizeof( "Extern_Linkage_Body" ) - 1 },
		{ "Friend",              sizeof( "Friend" ) - 1              },
		{ "Function",            sizeof( "Function" ) - 1            },
		{ "Function_Fwd",        sizeof( "Function_Fwd" ) - 1        },
		{ "Function_Body",       sizeof( "Function_Body" ) - 1       },
		{ "Global_Body",         sizeof( "Global_Body" ) - 1         },
		{ "Module",              sizeof( "Module" ) - 1              },
		{ "Namespace",           sizeof( "Namespace" ) - 1           },
		{ "Namespace_Body",      sizeof( "Namespace_Body" ) - 1      },
		{ "Operator",            sizeof( "Operator" ) - 1            },
		{ "Operator_Fwd",        sizeof( "Operator_Fwd" ) - 1        },
		{ "Operator_Member",     sizeof( "Operator_Member" ) - 1     },
		{ "Operator_Member_Fwd", sizeof( "Operator_Member_Fwd" ) - 1 },
		{ "Operator_Cast",       sizeof( "Operator_Cast" ) - 1       },
		{ "Operator_Cast_Fwd",   sizeof( "Operator_Cast_Fwd" ) - 1   },
		{ "Parameters",          sizeof( "Parameters" ) - 1          },
		{ "Preprocess_Define",   sizeof( "Preprocess_Define" ) - 1   },
		{ "Preprocess_Include",  sizeof( "Preprocess_Include" ) - 1  },
		{ "Preprocess_If",       sizeof( "Preprocess_If" ) - 1       },
		{ "Preprocess_IfDef",    sizeof( "Preprocess_IfDef" ) - 1    },
		{ "Preprocess_IfNotDef", sizeof( "Preprocess_IfNotDef" ) - 1 },
		{ "Preprocess_ElIf",     sizeof( "Preprocess_ElIf" ) - 1     },
		{ "Preprocess_Else",     sizeof( "Preprocess_Else" ) - 1     },
		{ "Preprocess_EndIf",    sizeof( "Preprocess_EndIf" ) - 1    },
		{ "Preprocess_Pragma",   sizeof( "Preprocess_Pragma" ) - 1   },
		{ "Specifiers",          sizeof( "Specifiers" ) - 1          },
		{ "Struct",              sizeof( "Struct" ) - 1              },
		{ "Struct_Fwd",          sizeof( "Struct_Fwd" ) - 1          },
		{ "Struct_Body",         sizeof( "Struct_Body" ) - 1         },
		{ "Template",            sizeof( "Template" ) - 1            },
		{ "Typedef",             sizeof( "Typedef" ) - 1             },
		{ "Typename",            sizeof( "Typename" ) - 1            },
		{ "Union",               sizeof( "Union" ) - 1               },
		{ "Union_Fwd",           sizeof( "Union_Fwd" ) - 1           },
		{ "Union_Body",          sizeof( "Union_Body" ) - 1          },
		{ "Using",               sizeof( "Using" ) - 1               },
		{ "Using_Namespace",     sizeof( "Using_Namespace" ) - 1     },
		{ "Variable",            sizeof( "Variable" ) - 1            },
	};
	return lookup[type];
}

inline Str codetype_to_keyword_str( CodeType type )
{
	local_persist Str lookup[61] = {
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "//",              sizeof( "//" ) - 1              },
		{ "private",         sizeof( "private" ) - 1         },
		{ "protected",       sizeof( "protected" ) - 1       },
		{ "public",          sizeof( "public" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "class",           sizeof( "class" ) - 1           },
		{ "clsss",           sizeof( "clsss" ) - 1           },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "enum",            sizeof( "enum" ) - 1            },
		{ "enum",            sizeof( "enum" ) - 1            },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "enum class",      sizeof( "enum class" ) - 1      },
		{ "enum class",      sizeof( "enum class" ) - 1      },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "extern",          sizeof( "extern" ) - 1          },
		{ "extern",          sizeof( "extern" ) - 1          },
		{ "friend",          sizeof( "friend" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "module",          sizeof( "module" ) - 1          },
		{ "namespace",       sizeof( "namespace" ) - 1       },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "operator",        sizeof( "operator" ) - 1        },
		{ "operator",        sizeof( "operator" ) - 1        },
		{ "operator",        sizeof( "operator" ) - 1        },
		{ "operator",        sizeof( "operator" ) - 1        },
		{ "operator",        sizeof( "operator" ) - 1        },
		{ "operator",        sizeof( "operator" ) - 1        },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "define",          sizeof( "define" ) - 1          },
		{ "include",         sizeof( "include" ) - 1         },
		{ "if",              sizeof( "if" ) - 1              },
		{ "ifdef",           sizeof( "ifdef" ) - 1           },
		{ "ifndef",          sizeof( "ifndef" ) - 1          },
		{ "elif",            sizeof( "elif" ) - 1            },
		{ "else",            sizeof( "else" ) - 1            },
		{ "endif",           sizeof( "endif" ) - 1           },
		{ "pragma",          sizeof( "pragma" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "struct",          sizeof( "struct" ) - 1          },
		{ "struct",          sizeof( "struct" ) - 1          },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "template",        sizeof( "template" ) - 1        },
		{ "typedef",         sizeof( "typedef" ) - 1         },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "union",           sizeof( "union" ) - 1           },
		{ "union",           sizeof( "union" ) - 1           },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
		{ "using",           sizeof( "using" ) - 1           },
		{ "using namespace", sizeof( "using namespace" ) - 1 },
		{ "__NA__",          sizeof( "__NA__" ) - 1          },
	};
	return lookup[type];
}

forceinline Str to_str( CodeType type )
{
	return codetype_to_str( type );
}

forceinline Str to_keyword_str( CodeType type )
{
	return codetype_to_keyword_str( type );
}

enum Operator : u32
{
	Op_Invalid,
	Op_Assign,
	Op_Assign_Add,
	Op_Assign_Subtract,
	Op_Assign_Multiply,
	Op_Assign_Divide,
	Op_Assign_Modulo,
	Op_Assign_BAnd,
	Op_Assign_BOr,
	Op_Assign_BXOr,
	Op_Assign_LShift,
	Op_Assign_RShift,
	Op_Increment,
	Op_Decrement,
	Op_Unary_Plus,
	Op_Unary_Minus,
	Op_UnaryNot,
	Op_Add,
	Op_Subtract,
	Op_Multiply,
	Op_Divide,
	Op_Modulo,
	Op_BNot,
	Op_BAnd,
	Op_BOr,
	Op_BXOr,
	Op_LShift,
	Op_RShift,
	Op_LAnd,
	Op_LOr,
	Op_LEqual,
	Op_LNot,
	Op_Lesser,
	Op_Greater,
	Op_LesserEqual,
	Op_GreaterEqual,
	Op_Subscript,
	Op_Indirection,
	Op_AddressOf,
	Op_MemberOfPointer,
	Op_PtrToMemOfPtr,
	Op_FunctionCall,
	Op_Comma,
	Op_New,
	Op_NewArray,
	Op_Delete,
	Op_DeleteArray,
	Op_NumOps,
	Op_UnderlyingType = 0xffffffffu
};

inline Str operator_to_str( Operator op )
{
	local_persist Str lookup[47] = {
		{ "INVALID",  sizeof( "INVALID" ) - 1  },
		{ "=",        sizeof( "=" ) - 1        },
		{ "+=",       sizeof( "+=" ) - 1       },
		{ "-=",       sizeof( "-=" ) - 1       },
		{ "*=",       sizeof( "*=" ) - 1       },
		{ "/=",       sizeof( "/=" ) - 1       },
		{ "%=",       sizeof( "%=" ) - 1       },
		{ "&=",       sizeof( "&=" ) - 1       },
		{ "|=",       sizeof( "|=" ) - 1       },
		{ "^=",       sizeof( "^=" ) - 1       },
		{ "<<=",      sizeof( "<<=" ) - 1      },
		{ ">>=",      sizeof( ">>=" ) - 1      },
		{ "++",       sizeof( "++" ) - 1       },
		{ "--",       sizeof( "--" ) - 1       },
		{ "+",        sizeof( "+" ) - 1        },
		{ "-",        sizeof( "-" ) - 1        },
		{ "!",        sizeof( "!" ) - 1        },
		{ "+",        sizeof( "+" ) - 1        },
		{ "-",        sizeof( "-" ) - 1        },
		{ "*",        sizeof( "*" ) - 1        },
		{ "/",        sizeof( "/" ) - 1        },
		{ "%",        sizeof( "%" ) - 1        },
		{ "~",        sizeof( "~" ) - 1        },
		{ "&",        sizeof( "&" ) - 1        },
		{ "|",        sizeof( "|" ) - 1        },
		{ "^",        sizeof( "^" ) - 1        },
		{ "<<",       sizeof( "<<" ) - 1       },
		{ ">>",       sizeof( ">>" ) - 1       },
		{ "&&",       sizeof( "&&" ) - 1       },
		{ "||",       sizeof( "||" ) - 1       },
		{ "==",       sizeof( "==" ) - 1       },
		{ "!=",       sizeof( "!=" ) - 1       },
		{ "<",        sizeof( "<" ) - 1        },
		{ ">",        sizeof( ">" ) - 1        },
		{ "<=",       sizeof( "<=" ) - 1       },
		{ ">=",       sizeof( ">=" ) - 1       },
		{ "[]",       sizeof( "[]" ) - 1       },
		{ "*",        sizeof( "*" ) - 1        },
		{ "&",        sizeof( "&" ) - 1        },
		{ "->",       sizeof( "->" ) - 1       },
		{ "->*",      sizeof( "->*" ) - 1      },
		{ "()",       sizeof( "()" ) - 1       },
		{ ",",        sizeof( "," ) - 1        },
		{ "new",      sizeof( "new" ) - 1      },
		{ "new[]",    sizeof( "new[]" ) - 1    },
		{ "delete",   sizeof( "delete" ) - 1   },
		{ "delete[]", sizeof( "delete[]" ) - 1 },
	};
	return lookup[op];
}

forceinline Str to_str( Operator op )
{
	return operator_to_str( op );
}

enum Specifier : u32
{
	Spec_Invalid,
	Spec_Consteval,
	Spec_Constexpr,
	Spec_Constinit,
	Spec_Explicit,
	Spec_External_Linkage,
	Spec_ForceInline,
	Spec_Global,
	Spec_Inline,
	Spec_Internal_Linkage,
	Spec_Local_Persist,
	Spec_Mutable,
	Spec_NeverInline,
	Spec_Ptr,
	Spec_Ref,
	Spec_Register,
	Spec_RValue,
	Spec_Static,
	Spec_Thread_Local,
	Spec_Virtual,
	Spec_Const,
	Spec_Final,
	Spec_NoExceptions,
	Spec_Override,
	Spec_Pure,
	Spec_Volatile,
	Spec_NumSpecifiers,
	Spec_UnderlyingType = 0xffffffffu
};

inline Str spec_to_str( Specifier type )
{
	local_persist Str lookup[26] = {
		{ "INVALID",       sizeof( "INVALID" ) - 1       },
		{ "consteval",     sizeof( "consteval" ) - 1     },
		{ "constexpr",     sizeof( "constexpr" ) - 1     },
		{ "constinit",     sizeof( "constinit" ) - 1     },
		{ "explicit",      sizeof( "explicit" ) - 1      },
		{ "extern",        sizeof( "extern" ) - 1        },
		{ "forceinline",   sizeof( "forceinline" ) - 1   },
		{ "global",        sizeof( "global" ) - 1        },
		{ "inline",        sizeof( "inline" ) - 1        },
		{ "internal",      sizeof( "internal" ) - 1      },
		{ "local_persist", sizeof( "local_persist" ) - 1 },
		{ "mutable",       sizeof( "mutable" ) - 1       },
		{ "neverinline",   sizeof( "neverinline" ) - 1   },
		{ "*",             sizeof( "*" ) - 1             },
		{ "&",             sizeof( "&" ) - 1             },
		{ "register",      sizeof( "register" ) - 1      },
		{ "&&",            sizeof( "&&" ) - 1            },
		{ "static",        sizeof( "static" ) - 1        },
		{ "thread_local",  sizeof( "thread_local" ) - 1  },
		{ "virtual",       sizeof( "virtual" ) - 1       },
		{ "const",         sizeof( "const" ) - 1         },
		{ "final",         sizeof( "final" ) - 1         },
		{ "noexcept",      sizeof( "noexcept" ) - 1      },
		{ "override",      sizeof( "override" ) - 1      },
		{ "= 0",           sizeof( "= 0" ) - 1           },
		{ "volatile",      sizeof( "volatile" ) - 1      },
	};
	return lookup[type];
}

inline bool spec_is_trailing( Specifier specifier )
{
	return specifier > Spec_Virtual;
}

inline Specifier str_to_specifier( Str str )
{
	local_persist u32 keymap[Spec_NumSpecifiers];
	do_once_start for ( u32 index = 0; index < Spec_NumSpecifiers; index++ )
	{
		Str enum_str  = spec_to_str( (Specifier)index );
		keymap[index] = crc32( enum_str.Ptr, enum_str.Len );
	}
	do_once_end u32 hash = crc32( str.Ptr, str.Len );
	for ( u32 index = 0; index < Spec_NumSpecifiers; index++ )
	{
		if ( keymap[index] == hash )
			return (Specifier)index;
	}
	return Spec_Invalid;
}

forceinline Str to_str( Specifier spec )
{
	return spec_to_str( spec );
}

forceinline Specifier to_type( Str str )
{
	return str_to_specifier( str );
}

forceinline bool is_trailing( Specifier specifier )
{
	return spec_is_trailing( specifier );
}

#define GEN_DEFINE_ATTRIBUTE_TOKENS Entry( Tok_Attribute_GEN_API, "GEN_API" )

enum TokType : u32
{
	Tok_Invalid,
	Tok_Access_Private,
	Tok_Access_Protected,
	Tok_Access_Public,
	Tok_Access_MemberSymbol,
	Tok_Access_StaticSymbol,
	Tok_Ampersand,
	Tok_Ampersand_DBL,
	Tok_Assign_Classifer,
	Tok_Attribute_Open,
	Tok_Attribute_Close,
	Tok_BraceCurly_Open,
	Tok_BraceCurly_Close,
	Tok_BraceSquare_Open,
	Tok_BraceSquare_Close,
	Tok_Capture_Start,
	Tok_Capture_End,
	Tok_Comment,
	Tok_Comment_End,
	Tok_Comment_Start,
	Tok_Char,
	Tok_Comma,
	Tok_Decl_Class,
	Tok_Decl_GNU_Attribute,
	Tok_Decl_MSVC_Attribute,
	Tok_Decl_Enum,
	Tok_Decl_Extern_Linkage,
	Tok_Decl_Friend,
	Tok_Decl_Module,
	Tok_Decl_Namespace,
	Tok_Decl_Operator,
	Tok_Decl_Struct,
	Tok_Decl_Template,
	Tok_Decl_Typedef,
	Tok_Decl_Using,
	Tok_Decl_Union,
	Tok_Identifier,
	Tok_Module_Import,
	Tok_Module_Export,
	Tok_NewLine,
	Tok_Number,
	Tok_Operator,
	Tok_Preprocess_Hash,
	Tok_Preprocess_Define,
	Tok_Preprocess_If,
	Tok_Preprocess_IfDef,
	Tok_Preprocess_IfNotDef,
	Tok_Preprocess_ElIf,
	Tok_Preprocess_Else,
	Tok_Preprocess_EndIf,
	Tok_Preprocess_Include,
	Tok_Preprocess_Pragma,
	Tok_Preprocess_Content,
	Tok_Preprocess_Macro,
	Tok_Preprocess_Unsupported,
	Tok_Spec_Alignas,
	Tok_Spec_Const,
	Tok_Spec_Consteval,
	Tok_Spec_Constexpr,
	Tok_Spec_Constinit,
	Tok_Spec_Explicit,
	Tok_Spec_Extern,
	Tok_Spec_Final,
	Tok_Spec_ForceInline,
	Tok_Spec_Global,
	Tok_Spec_Inline,
	Tok_Spec_Internal_Linkage,
	Tok_Spec_LocalPersist,
	Tok_Spec_Mutable,
	Tok_Spec_NeverInline,
	Tok_Spec_Override,
	Tok_Spec_Static,
	Tok_Spec_ThreadLocal,
	Tok_Spec_Volatile,
	Tok_Spec_Virtual,
	Tok_Star,
	Tok_Statement_End,
	Tok_StaticAssert,
	Tok_String,
	Tok_Type_Typename,
	Tok_Type_Unsigned,
	Tok_Type_Signed,
	Tok_Type_Short,
	Tok_Type_Long,
	Tok_Type_bool,
	Tok_Type_char,
	Tok_Type_int,
	Tok_Type_double,
	Tok_Type_MS_int8,
	Tok_Type_MS_int16,
	Tok_Type_MS_int32,
	Tok_Type_MS_int64,
	Tok_Type_MS_W64,
	Tok_Varadic_Argument,
	Tok___Attributes_Start,
	Tok_Attribute_GEN_API,
	Tok_NumTokens
};

inline Str toktype_to_str( TokType type )
{
	local_persist Str lookup[] = {
		{ "__invalid__",       sizeof( "__invalid__" ) - 1       },
		{ "private",           sizeof( "private" ) - 1           },
		{ "protected",         sizeof( "protected" ) - 1         },
		{ "public",            sizeof( "public" ) - 1            },
		{ ".",		         sizeof( "." ) - 1                 },
		{ "::",		        sizeof( "::" ) - 1                },
		{ "&",		         sizeof( "&" ) - 1                 },
		{ "&&",		        sizeof( "&&" ) - 1                },
		{ ":",		         sizeof( ":" ) - 1                 },
		{ "[[",		        sizeof( "[[" ) - 1                },
		{ "]]",		        sizeof( "]]" ) - 1                },
		{ "{",		         sizeof( "{" ) - 1                 },
		{ "}",		         sizeof( "}" ) - 1                 },
		{ "[",		         sizeof( "[" ) - 1                 },
		{ "]",		         sizeof( "]" ) - 1                 },
		{ "(",		         sizeof( "(" ) - 1                 },
		{ ")",		         sizeof( ")" ) - 1                 },
		{ "__comment__",       sizeof( "__comment__" ) - 1       },
		{ "__comment_end__",   sizeof( "__comment_end__" ) - 1   },
		{ "__comment_start__", sizeof( "__comment_start__" ) - 1 },
		{ "__character__",     sizeof( "__character__" ) - 1     },
		{ ",",		         sizeof( "," ) - 1                 },
		{ "class",             sizeof( "class" ) - 1             },
		{ "__attribute__",     sizeof( "__attribute__" ) - 1     },
		{ "__declspec",        sizeof( "__declspec" ) - 1        },
		{ "enum",              sizeof( "enum" ) - 1              },
		{ "extern",            sizeof( "extern" ) - 1            },
		{ "friend",            sizeof( "friend" ) - 1            },
		{ "module",            sizeof( "module" ) - 1            },
		{ "namespace",         sizeof( "namespace" ) - 1         },
		{ "operator",          sizeof( "operator" ) - 1          },
		{ "struct",            sizeof( "struct" ) - 1            },
		{ "template",          sizeof( "template" ) - 1          },
		{ "typedef",           sizeof( "typedef" ) - 1           },
		{ "using",             sizeof( "using" ) - 1             },
		{ "union",             sizeof( "union" ) - 1             },
		{ "__identifier__",    sizeof( "__identifier__" ) - 1    },
		{ "import",            sizeof( "import" ) - 1            },
		{ "export",            sizeof( "export" ) - 1            },
		{ "__new_line__",      sizeof( "__new_line__" ) - 1      },
		{ "__number__",        sizeof( "__number__" ) - 1        },
		{ "__operator__",      sizeof( "__operator__" ) - 1      },
		{ "#",		         sizeof( "#" ) - 1                 },
		{ "define",            sizeof( "define" ) - 1            },
		{ "if",		        sizeof( "if" ) - 1                },
		{ "ifdef",             sizeof( "ifdef" ) - 1             },
		{ "ifndef",            sizeof( "ifndef" ) - 1            },
		{ "elif",              sizeof( "elif" ) - 1              },
		{ "else",              sizeof( "else" ) - 1              },
		{ "endif",             sizeof( "endif" ) - 1             },
		{ "include",           sizeof( "include" ) - 1           },
		{ "pragma",            sizeof( "pragma" ) - 1            },
		{ "__macro_content__", sizeof( "__macro_content__" ) - 1 },
		{ "__macro__",         sizeof( "__macro__" ) - 1         },
		{ "__unsupported__",   sizeof( "__unsupported__" ) - 1   },
		{ "alignas",           sizeof( "alignas" ) - 1           },
		{ "const",             sizeof( "const" ) - 1             },
		{ "consteval",         sizeof( "consteval" ) - 1         },
		{ "constexpr",         sizeof( "constexpr" ) - 1         },
		{ "constinit",         sizeof( "constinit" ) - 1         },
		{ "explicit",          sizeof( "explicit" ) - 1          },
		{ "extern",            sizeof( "extern" ) - 1            },
		{ "final",             sizeof( "final" ) - 1             },
		{ "forceinline",       sizeof( "forceinline" ) - 1       },
		{ "global",            sizeof( "global" ) - 1            },
		{ "inline",            sizeof( "inline" ) - 1            },
		{ "internal",          sizeof( "internal" ) - 1          },
		{ "local_persist",     sizeof( "local_persist" ) - 1     },
		{ "mutable",           sizeof( "mutable" ) - 1           },
		{ "neverinline",       sizeof( "neverinline" ) - 1       },
		{ "override",          sizeof( "override" ) - 1          },
		{ "static",            sizeof( "static" ) - 1            },
		{ "thread_local",      sizeof( "thread_local" ) - 1      },
		{ "volatile",          sizeof( "volatile" ) - 1          },
		{ "virtual",           sizeof( "virtual" ) - 1           },
		{ "*",		         sizeof( "*" ) - 1                 },
		{ ";",		         sizeof( ";" ) - 1                 },
		{ "static_assert",     sizeof( "static_assert" ) - 1     },
		{ "__string__",        sizeof( "__string__" ) - 1        },
		{ "typename",          sizeof( "typename" ) - 1          },
		{ "unsigned",          sizeof( "unsigned" ) - 1          },
		{ "signed",            sizeof( "signed" ) - 1            },
		{ "short",             sizeof( "short" ) - 1             },
		{ "long",              sizeof( "long" ) - 1              },
		{ "bool",              sizeof( "bool" ) - 1              },
		{ "char",              sizeof( "char" ) - 1              },
		{ "int",               sizeof( "int" ) - 1               },
		{ "double",            sizeof( "double" ) - 1            },
		{ "__int8",            sizeof( "__int8" ) - 1            },
		{ "__int16",           sizeof( "__int16" ) - 1           },
		{ "__int32",           sizeof( "__int32" ) - 1           },
		{ "__int64",           sizeof( "__int64" ) - 1           },
		{ "_W64",              sizeof( "_W64" ) - 1              },
		{ "...",               sizeof( "..." ) - 1               },
		{ "__attrib_start__",  sizeof( "__attrib_start__" ) - 1  },
		{ "GEN_API",           sizeof( "GEN_API" ) - 1           },
	};
	return lookup[type];
}

inline TokType str_to_toktype( Str str )
{
	local_persist u32 keymap[Tok_NumTokens];
	do_once_start for ( u32 index = 0; index < Tok_NumTokens; index++ )
	{
		Str enum_str  = toktype_to_str( (TokType)index );
		keymap[index] = crc32( enum_str.Ptr, enum_str.Len );
	}
	do_once_end u32 hash = crc32( str.Ptr, str.Len );
	for ( u32 index = 0; index < Tok_NumTokens; index++ )
	{
		if ( keymap[index] == hash )
			return (TokType)index;
	}
	return Tok_Invalid;
}

enum TokFlags : u32
{
	TF_Operator		   = bit(0),
	TF_Assign          = bit(1),
	TF_Preprocess      = bit(2),
	TF_Preprocess_Cond = bit(3),
	TF_Attribute       = bit(6),
	TF_AccessOperator  = bit( 7 ),
	TF_AccessSpecifier = bit( 8 ),
	TF_Specifier       = bit( 9 ),
	TF_EndDefinition   = bit( 10 ),    // Either ; or }
	TF_Formatting      = bit( 11 ),
	TF_Literal         = bit( 12 ),

	TF_Null = 0,
	TF_UnderlyingType = GEN_U32_MAX,
};

struct Token
{
	Str     Text;
	TokType Type;
	s32     Line;
	s32     Column;
	u32     Flags;
};

constexpr Token NullToken { {}, Tok_Invalid, 0, 0, TF_Null };

forceinline
AccessSpec tok_to_access_specifier(Token tok) {
	return scast(AccessSpec, tok.Type);
}

forceinline
Str tok_to_str(Token tok) {
	return tok.Text;
}

forceinline
bool tok_is_valid( Token tok ) {
	return tok.Text.Ptr && tok.Text.Len && tok.Type != Tok_Invalid;
}

forceinline
bool tok_is_access_operator(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_AccessOperator );
}

forceinline
bool tok_is_access_specifier(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_AccessSpecifier );
}

forceinline
bool tok_is_attribute(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_Attribute );
}

forceinline
bool tok_is_operator(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_Operator );
}

forceinline
bool tok_is_preprocessor(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_Preprocess );
}

forceinline
bool tok_is_preprocess_cond(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_Preprocess_Cond );
}

forceinline
bool tok_is_specifier(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_Specifier );
}

forceinline
bool tok_is_end_definition(Token tok) {
	return bitfield_is_equal( u32, tok.Flags, TF_EndDefinition );
}

StrBuilder tok_to_strbuilder(Token tok);

struct TokArray 
{
	Array(Token) Arr;
	s32          Idx;
};

struct LexContext
{
	Str             content;
	s32             left;
	char const*     scanner;
	s32             line;
	s32             column;
	StringTable     defines;
	Token           token;
};

struct StackNode
{
	StackNode* Prev;

	Token* Start;
	Str    Name;          // The name of the AST node (if parsed)
	Str    ProcName;    // The name of the procedure
};

struct ParseContext
{
	TokArray   Tokens;
	StackNode* Scope;
};

#pragma endregion Types

#pragma region AST

/*
  ______   ______  ________      __    __       ______                 __
 /      \ /      \|        \    |  \  |  \     /      \               |  \
|  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\\▓▓▓▓▓▓▓▓    | ▓▓\ | ▓▓    |  ▓▓▓▓▓▓\ ______   ____| ▓▓ ______
| ▓▓__| ▓▓ ▓▓___\▓▓  | ▓▓       | ▓▓▓\| ▓▓    | ▓▓   \▓▓/      \ /      ▓▓/      \
| ▓▓    ▓▓\▓▓    \   | ▓▓       | ▓▓▓▓\ ▓▓    | ▓▓     |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\
| ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\  | ▓▓       | ▓▓\▓▓ ▓▓    | ▓▓   __| ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓
| ▓▓  | ▓▓  \__| ▓▓  | ▓▓       | ▓▓ \▓▓▓▓    | ▓▓__/  \ ▓▓__/ ▓▓ ▓▓__| ▓▓ ▓▓▓▓▓▓▓▓
| ▓▓  | ▓▓\▓▓    ▓▓  | ▓▓       | ▓▓  \▓▓▓     \▓▓    ▓▓\▓▓    ▓▓\▓▓    ▓▓\▓▓     \
 \▓▓   \▓▓ \▓▓▓▓▓▓    \▓▓        \▓▓   \▓▓      \▓▓▓▓▓▓  \▓▓▓▓▓▓  \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓
*/

struct AST;
struct AST_Body;
struct AST_Attributes;
struct AST_Comment;
struct AST_Constructor;
// struct AST_BaseClass;
struct AST_Class;
struct AST_Define;
struct AST_Destructor;
struct AST_Enum;
struct AST_Exec;
struct AST_Extern;
struct AST_Include;
struct AST_Friend;
struct AST_Fn;
struct AST_Module;
struct AST_NS;
struct AST_Operator;
struct AST_OpCast;
struct AST_Params;
struct AST_Pragma;
struct AST_PreprocessCond;
struct AST_Specifiers;

#if GEN_EXECUTION_EXPRESSION_SUPPORT
struct AST_Expr;
struct AST_Expr_Assign;
struct AST_Expr_Alignof;
struct AST_Expr_Binary;
struct AST_Expr_CStyleCast;
struct AST_Expr_FunctionalCast;
struct AST_Expr_CppCast;
struct AST_Expr_ProcCall;
struct AST_Expr_Decltype;
struct AST_Expr_Comma;  // TODO(Ed) : This is a binary op not sure if it needs its own AST...
struct AST_Expr_AMS;    // Access Member Symbol
struct AST_Expr_Sizeof;
struct AST_Expr_Subscript;
struct AST_Expr_Ternary;
struct AST_Expr_UnaryPrefix;
struct AST_Expr_UnaryPostfix;
struct AST_Expr_Element;

struct AST_Stmt;
struct AST_Stmt_Break;
struct AST_Stmt_Case;
struct AST_Stmt_Continue;
struct AST_Stmt_Decl;
struct AST_Stmt_Do;
struct AST_Stmt_Expr;  // TODO(Ed) : Is this distinction needed? (Should it be a flag instead?)
struct AST_Stmt_Else;
struct AST_Stmt_If;
struct AST_Stmt_For;
struct AST_Stmt_Goto;
struct AST_Stmt_Label;
struct AST_Stmt_Switch;
struct AST_Stmt_While;
#endif

struct AST_Struct;
struct AST_Template;
struct AST_Typename;
struct AST_Typedef;
struct AST_Union;
struct AST_Using;
struct AST_Var;

#if GEN_COMPILER_C
typedef AST* Code;
#else
struct Code;
#endif

#if GEN_COMPILER_C
typedef AST_Body*           CodeBody;
typedef AST_Attributes*     CodeAttributes;
typedef AST_Comment*        CodeComment;
typedef AST_Class*          CodeClass;
typedef AST_Constructor*    CodeConstructor;
typedef AST_Define*         CodeDefine;
typedef AST_Destructor*     CodeDestructor;
typedef AST_Enum*           CodeEnum;
typedef AST_Exec*           CodeExec;
typedef AST_Extern*         CodeExtern;
typedef AST_Include*        CodeInclude;
typedef AST_Friend*         CodeFriend;
typedef AST_Fn*             CodeFn;
typedef AST_Module*         CodeModule;
typedef AST_NS*             CodeNS;
typedef AST_Operator*       CodeOperator;
typedef AST_OpCast*         CodeOpCast;
typedef AST_Params*         CodeParams;
typedef AST_PreprocessCond* CodePreprocessCond;
typedef AST_Pragma*         CodePragma;
typedef AST_Specifiers*     CodeSpecifiers;
#else
struct CodeBody;
struct CodeAttributes;
struct CodeComment;
struct CodeClass;
struct CodeConstructor;
struct CodeDefine;
struct CodeDestructor;
struct CodeEnum;
struct CodeExec;
struct CodeExtern;
struct CodeInclude;
struct CodeFriend;
struct CodeFn;
struct CodeModule;
struct CodeNS;
struct CodeOperator;
struct CodeOpCast;
struct CodeParams;
struct CodePreprocessCond;
struct CodePragma;
struct CodeSpecifiers;
#endif

#if GEN_EXECUTION_EXPRESSION_SUPPORT

#if GEN_COMPILER_C
typedef AST_Expr*                CodeExpr;
typedef AST_Expr_Assign*         CodeExpr_Assign;
typedef AST_Expr_Alignof*        CodeExpr_Alignof;
typedef AST_Expr_Binary*         CodeExpr_Binary;
typedef AST_Expr_CStyleCast*     CodeExpr_CStyleCast;
typedef AST_Expr_FunctionalCast* CodeExpr_FunctionalCast;
typedef AST_Expr_CppCast*        CodeExpr_CppCast;
typedef AST_Expr_Element*        CodeExpr_Element;
typedef AST_Expr_ProcCall*       CodeExpr_ProcCall;
typedef AST_Expr_Decltype*       CodeExpr_Decltype;
typedef AST_Expr_Comma*          CodeExpr_Comma;
typedef AST_Expr_AMS*            CodeExpr_AMS; // Access Member Symbol
typedef AST_Expr_Sizeof*         CodeExpr_Sizeof;
typedef AST_Expr_Subscript*      CodeExpr_Subscript;
typedef AST_Expr_Ternary*        CodeExpr_Ternary;
typedef AST_Expr_UnaryPrefix*    CodeExpr_UnaryPrefix;
typedef AST_Expr_UnaryPostfix*   CodeExpr_UnaryPostfix;
#else
struct CodeExpr;
struct CodeExpr_Assign;
struct CodeExpr_Alignof;
struct CodeExpr_Binary;
struct CodeExpr_CStyleCast;
struct CodeExpr_FunctionalCast;
struct CodeExpr_CppCast;
struct CodeExpr_Element;
struct CodeExpr_ProcCall;
struct CodeExpr_Decltype;
struct CodeExpr_Comma;
struct CodeExpr_AMS; // Access Member Symbol
struct CodeExpr_Sizeof;
struct CodeExpr_Subscript;
struct CodeExpr_Ternary;
struct CodeExpr_UnaryPrefix;
struct CodeExpr_UnaryPostfix;
#endif

#if GEN_COMPILER_C
typedef AST_Stmt*          CodeStmt;
typedef AST_Stmt_Break*    CodeStmt_Break;
typedef AST_Stmt_Case*     CodeStmt_Case;
typedef AST_Stmt_Continue* CodeStmt_Continue;
typedef AST_Stmt_Decl*     CodeStmt_Decl;
typedef AST_Stmt_Do*       CodeStmt_Do;
typedef AST_Stmt_Expr*     CodeStmt_Expr;
typedef AST_Stmt_Else*     CodeStmt_Else;
typedef AST_Stmt_If*       CodeStmt_If;
typedef AST_Stmt_For*      CodeStmt_For;
typedef AST_Stmt_Goto*     CodeStmt_Goto;
typedef AST_Stmt_Label*    CodeStmt_Label;
typedef AST_Stmt_Switch*   CodeStmt_Switch;
typedef AST_Stmt_While*    CodeStmt_While;
#else
struct CodeStmt;
struct CodeStmt_Break;
struct CodeStmt_Case;
struct CodeStmt_Continue;
struct CodeStmt_Decl;
struct CodeStmt_Do;
struct CodeStmt_Expr;
struct CodeStmt_Else;
struct CodeStmt_If;
struct CodeStmt_For;
struct CodeStmt_Goto;
struct CodeStmt_Label;
struct CodeStmt_Switch;
struct CodeStmt_While;
#endif

// GEN_EXECUTION_EXPRESSION_SUPPORT
#endif

#if GEN_COMPILER_C
typedef AST_Struct*   CodeStruct;
typedef AST_Template* CodeTemplate;
typedef AST_Typename* CodeTypename;
typedef AST_Typedef*  CodeTypedef;
typedef AST_Union*    CodeUnion;
typedef AST_Using*    CodeUsing;
typedef AST_Var*      CodeVar;
#else
struct CodeStruct;
struct CodeTemplate;
struct CodeTypename;
struct CodeTypedef;
struct CodeUnion;
struct CodeUsing;
struct CodeVar;
#endif

#if GEN_COMPILER_CPP
template< class Type> forceinline Type tmpl_cast( Code self ) { return * rcast( Type*, & self ); }
#endif

#pragma region Code C-Interface

GEN_API void       code_append           (Code code, Code other );
GEN_API Str        code_debug_str        (Code code);
GEN_API Code       code_duplicate        (Code code);
GEN_API Code*      code_entry            (Code code, u32 idx );
GEN_API bool       code_has_entries      (Code code);
GEN_API bool       code_is_body          (Code code);
GEN_API bool       code_is_equal         (Code code, Code other);
GEN_API bool       code_is_valid         (Code code);
GEN_API void       code_set_global       (Code code);
GEN_API StrBuilder code_to_strbuilder    (Code self );
GEN_API void       code_to_strbuilder_ptr(Code self, StrBuilder* result );
GEN_API Str        code_type_str         (Code self );
GEN_API bool       code_validate_body    (Code self );

#pragma endregion Code C-Interface

#if GEN_COMPILER_CPP
/*
	AST* wrapper
	- Not constantly have to append the '*' as this is written often..
	- Allows for implicit conversion to any of the ASTs (raw or filtered).
*/
struct Code
{
	AST* ast;

#	define Using_Code( Typename )                                                        \
	forceinline Str  debug_str()                { return code_debug_str(* this); }       \
	forceinline Code duplicate()                { return code_duplicate(* this); }	     \
	forceinline bool is_equal( Code other )     { return code_is_equal(* this, other); } \
	forceinline bool is_body()                  { return code_is_body(* this); }         \
	forceinline bool is_valid()                 { return code_is_valid(* this); }        \
	forceinline void set_global()               { return code_set_global(* this); }

#	define Using_CodeOps( Typename )                                                                           \
	forceinline Typename&  operator = ( Code other );                                                          \
	forceinline bool       operator ==( Code other )                        { return (AST*)ast == other.ast; } \
	forceinline bool       operator !=( Code other )                        { return (AST*)ast != other.ast; } \
	forceinline bool       operator ==(std::nullptr_t) const                { return ast == nullptr; }         \
	forceinline bool       operator !=(std::nullptr_t) const                { return ast != nullptr;  }        \
	operator bool();

#if ! GEN_C_LIKE_CPP
	Using_Code( Code );
	forceinline void       append(Code other)                { return code_append(* this, other); }
	forceinline Code*      entry(u32 idx)                    { return code_entry(* this, idx); }
	forceinline bool       has_entries()                     { return code_has_entries(* this); }
	forceinline StrBuilder to_strbuilder()                   { return code_to_strbuilder(* this); }
	forceinline void       to_strbuilder(StrBuilder& result) { return code_to_strbuilder_ptr(* this, & result); }
	forceinline Str        type_str()                        { return code_type_str(* this); }
	forceinline bool       validate_body()                   { return code_validate_body(*this); }
#endif

	Using_CodeOps( Code );
	forceinline Code operator *() { return * this; } // Required to support for-range iteration.
	forceinline AST* operator ->() { return ast; }

	Code& operator ++();

#ifdef GEN_ENFORCE_STRONG_CODE_TYPES
#	define operator explicit operator
#endif
	operator CodeBody() const;
	operator CodeAttributes() const;
	// operator CodeBaseClass() const;
	operator CodeComment() const;
	operator CodeClass() const;
	operator CodeConstructor() const;
	operator CodeDefine() const;
	operator CodeDestructor() const;
	operator CodeExec() const;
	operator CodeEnum() const;
	operator CodeExtern() const;
	operator CodeInclude() const;
	operator CodeFriend() const;
	operator CodeFn() const;
	operator CodeModule() const;
	operator CodeNS() const;
	operator CodeOperator() const;
	operator CodeOpCast() const;
	operator CodeParams() const;
	operator CodePragma() const;
	operator CodePreprocessCond() const;
	operator CodeSpecifiers() const;
	operator CodeStruct() const;
	operator CodeTemplate() const;
	operator CodeTypename() const;
	operator CodeTypedef() const;
	operator CodeUnion() const;
	operator CodeUsing() const;
	operator CodeVar() const;
	#undef operator
};
#endif

#pragma region Statics
// Used to identify ASTs that should always be duplicated. (Global constant ASTs)
extern Code Code_Global;

// Used to identify invalid generated code.
extern Code Code_Invalid;
#pragma endregion Statics

struct Code_POD
{
	AST* ast;
};
static_assert( sizeof(Code) == sizeof(Code_POD), "ERROR: Code is not POD" );

// Desired width of the AST data structure.
constexpr int const AST_POD_Size = 128;

constexpr static
int AST_ArrSpecs_Cap =
(
	AST_POD_Size
	- sizeof(Code)
	- sizeof(StrCached)
	- sizeof(Code) * 2
	- sizeof(Token*)
	- sizeof(Code)
	- sizeof(CodeType)
	- sizeof(ModuleFlag)
	- sizeof(u32)
)
/ sizeof(Specifier) - 1;

/*
	Simple AST POD with functionality to seralize into C++ syntax.
*/
struct AST
{
	union {
		struct
		{
			Code      InlineCmt;       // Class, Constructor, Destructor, Enum, Friend, Functon, Operator, OpCast, Struct, Typedef, Using, Variable
			Code      Attributes;      // Class, Enum, Function, Struct, Typedef, Union, Using, Variable
			Code      Specs;           // Destructor, Function, Operator, Typename, Variable
			union {
				Code  InitializerList; // Constructor
				Code  ParentType;      // Class, Struct, ParentType->Next has a possible list of interfaces.
				Code  ReturnType;      // Function, Operator, Typename
				Code  UnderlyingType;  // Enum, Typedef
				Code  ValueType;       // Parameter, Variable
			};
			union {
				Code  Macro;               // Parameter
				Code  BitfieldSize;        // Variable (Class/Struct Data Member)
				Code  Params;              // Constructor, Function, Operator, Template, Typename
				Code  UnderlyingTypeMacro; // Enum
			};
			union {
				Code  ArrExpr;          // Typename
				Code  Body;             // Class, Constructor, Destructor, Enum, Friend, Function, Namespace, Struct, Union
				Code  Declaration;      // Friend, Template
				Code  Value;            // Parameter, Variable
			};
			union {
				Code  NextVar;          // Variable; Possible way to handle comma separated variables declarations. ( , NextVar->Specs NextVar->Name NextVar->ArrExpr = NextVar->Value )
				Code  SuffixSpecs;      // Only used with typenames, to store the function suffix if typename is function signature. ( May not be needed )
				Code  PostNameMacro;    // Only used with parameters for specifically UE_REQUIRES (Thanks Unreal)
			};
		};
		StrCached  Content;          // Attributes, Comment, Execution, Include
		struct {
			Specifier  ArrSpecs[AST_ArrSpecs_Cap]; // Specifiers
			Code       NextSpecs;              // Specifiers; If ArrSpecs is full, then NextSpecs is used.
		};
	};
	StrCached      Name;
	union {
		Code Prev;
		Code Front;
		Code Last;
	};
	union {
		Code Next;
		Code Back;
	};
	Token*            Token; // Reference to starting token, only avaialble if it was derived from parsing.
	Code              Parent;
	CodeType          Type;
//	CodeFlag          CodeFlags;
	ModuleFlag        ModuleFlags;
	union {
		b32           IsFunction;  // Used by typedef to not serialize the name field.
		struct {
			b16           IsParamPack;   // Used by typename to know if type should be considered a parameter pack.
			ETypenameTag  TypeTag;       // Used by typename to keep track of explicitly declared tags for the identifier (enum, struct, union)
		};
		Operator      Op;
		AccessSpec    ParentAccess;
		s32           NumEntries;
		s32           VarParenthesizedInit;  // Used by variables to know that initialization is using a constructor expression instead of an assignment expression.
	};
};
static_assert( sizeof(AST) == AST_POD_Size, "ERROR: AST is not size of AST_POD_Size" );

#if GEN_COMPILER_CPP
// Uses an implicitly overloaded cast from the AST to the desired code type.
// Necessary if the user wants GEN_ENFORCE_STRONG_CODE_TYPES
struct  InvalidCode_ImplictCaster;
#define InvalidCode (InvalidCode_ImplictCaster{})
#else
#define InvalidCode (void*){ (void*)Code_Invalid }
#endif

#if GEN_COMPILER_CPP
struct NullCode_ImplicitCaster;
// Used when the its desired when omission is allowed in a definition.
#define NullCode    (NullCode_ImplicitCaster{})
#else
#define NullCode    nullptr
#endif

/*
  ______                 __               ______            __                        ______
 /      \               |  \             |      \          |  \                      /      \
|  ▓▓▓▓▓▓\ ______   ____| ▓▓ ______       \▓▓▓▓▓▓_______  _| ▓▓_    ______   ______ |  ▓▓▓▓▓▓\ ______   _______  ______
| ▓▓   \▓▓/      \ /      ▓▓/      \       | ▓▓ |       \|   ▓▓ \  /      \ /      \| ▓▓_  \▓▓|      \ /       \/      \
| ▓▓     |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\      | ▓▓ | ▓▓▓▓▓▓▓\\▓▓▓▓▓▓ |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\ ▓▓ \     \▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\
| ▓▓   __| ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓      | ▓▓ | ▓▓  | ▓▓ | ▓▓ __| ▓▓    ▓▓ ▓▓   \▓▓ ▓▓▓▓    /      ▓▓ ▓▓     | ▓▓    ▓▓
| ▓▓__/  \ ▓▓__/ ▓▓ ▓▓__| ▓▓ ▓▓▓▓▓▓▓▓     _| ▓▓_| ▓▓  | ▓▓ | ▓▓|  \ ▓▓▓▓▓▓▓▓ ▓▓     | ▓▓     |  ▓▓▓▓▓▓▓ ▓▓_____| ▓▓▓▓▓▓▓▓
 \▓▓    ▓▓\▓▓    ▓▓\▓▓    ▓▓\▓▓     \    |   ▓▓ \ ▓▓  | ▓▓  \▓▓  ▓▓\▓▓     \ ▓▓     | ▓▓      \▓▓    ▓▓\▓▓     \\▓▓     \
  \▓▓▓▓▓▓  \▓▓▓▓▓▓  \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓     \▓▓▓▓▓▓\▓▓   \▓▓   \▓▓▓▓  \▓▓▓▓▓▓▓\▓▓      \▓▓       \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓
*/

#pragma region Code Type C-Interface

GEN_API void       body_append              ( CodeBody body, Code     other );
GEN_API void       body_append_body         ( CodeBody body, CodeBody other );
GEN_API StrBuilder body_to_strbuilder       ( CodeBody body );
GEN_API void       body_to_strbuilder_ref   ( CodeBody body, StrBuilder* result );
GEN_API void       body_to_strbuilder_export( CodeBody body, StrBuilder* result );

GEN_API Code begin_CodeBody( CodeBody body);
GEN_API Code end_CodeBody  ( CodeBody body );
GEN_API Code next_CodeBody ( CodeBody body, Code entry_iter );

GEN_API void       class_add_interface    ( CodeClass self, CodeTypename interface );
GEN_API StrBuilder class_to_strbuilder    ( CodeClass self );
GEN_API void       class_to_strbuilder_def( CodeClass self, StrBuilder* result );
GEN_API void       class_to_strbuilder_fwd( CodeClass self, StrBuilder* result );

GEN_API void       params_append           (CodeParams params, CodeParams param );
GEN_API CodeParams params_get              (CodeParams params, s32 idx);
GEN_API bool       params_has_entries      (CodeParams params );
GEN_API StrBuilder params_to_strbuilder    (CodeParams params );
GEN_API void       params_to_strbuilder_ref(CodeParams params, StrBuilder* result );

GEN_API CodeParams begin_CodeParams(CodeParams params);
GEN_API CodeParams end_CodeParams  (CodeParams params);
GEN_API CodeParams next_CodeParams (CodeParams params, CodeParams entry_iter);

GEN_API bool       specifiers_append           (CodeSpecifiers specifiers, Specifier spec);
GEN_API s32        specifiers_has              (CodeSpecifiers specifiers, Specifier spec);
GEN_API s32        specifiers_remove           (CodeSpecifiers specifiers, Specifier to_remove );
GEN_API StrBuilder specifiers_to_strbuilder    (CodeSpecifiers specifiers);
GEN_API void       specifiers_to_strbuilder_ref(CodeSpecifiers specifiers, StrBuilder* result);

GEN_API Specifier* begin_CodeSpecifiers(CodeSpecifiers specifiers);
GEN_API Specifier* end_CodeSpecifiers  (CodeSpecifiers specifiers);
GEN_API Specifier* next_CodeSpecifiers (CodeSpecifiers specifiers, Specifier* spec_iter);

GEN_API void       struct_add_interface    (CodeStruct self, CodeTypename interface);
GEN_API StrBuilder struct_to_strbuilder    (CodeStruct self);
GEN_API void       struct_to_strbuilder_fwd(CodeStruct self, StrBuilder* result);
GEN_API void       struct_to_strbuilder_def(CodeStruct self, StrBuilder* result);

GEN_API StrBuilder attributes_to_strbuilder    (CodeAttributes attributes);
GEN_API void       attributes_to_strbuilder_ref(CodeAttributes attributes, StrBuilder* result);

GEN_API StrBuilder comment_to_strbuilder    (CodeComment comment );
GEN_API void       comment_to_strbuilder_ref(CodeComment comment, StrBuilder* result );

GEN_API StrBuilder constructor_to_strbuilder    (CodeConstructor constructor);
GEN_API void       constructor_to_strbuilder_def(CodeConstructor constructor, StrBuilder* result );
GEN_API void       constructor_to_strbuilder_fwd(CodeConstructor constructor, StrBuilder* result );

GEN_API StrBuilder define_to_strbuilder    (CodeDefine self);
GEN_API void       define_to_strbuilder_ref(CodeDefine self, StrBuilder* result);

GEN_API StrBuilder destructor_to_strbuilder    (CodeDestructor destructor);
GEN_API void       destructor_to_strbuilder_fwd(CodeDestructor destructor, StrBuilder* result );
GEN_API void       destructor_to_strbuilder_def(CodeDestructor destructor, StrBuilder* result );

GEN_API StrBuilder enum_to_strbuilder          (CodeEnum self);
GEN_API void       enum_to_strbuilder_def      (CodeEnum self, StrBuilder* result );
GEN_API void       enum_to_strbuilder_fwd      (CodeEnum self, StrBuilder* result );
GEN_API void       enum_to_strbuilder_class_def(CodeEnum self, StrBuilder* result );
GEN_API void       enum_to_strbuilder_class_fwd(CodeEnum self, StrBuilder* result );

GEN_API StrBuilder exec_to_strbuilder    (CodeExec exec);
GEN_API void       exec_to_strbuilder_ref(CodeExec exec, StrBuilder* result);

GEN_API void extern_to_strbuilder(CodeExtern self, StrBuilder* result);

GEN_API StrBuilder include_to_strbuilder    (CodeInclude self);
GEN_API void       include_to_strbuilder_ref(CodeInclude self, StrBuilder* result);

GEN_API StrBuilder friend_to_strbuilder     (CodeFriend self);
GEN_API void       friend_to_strbuilder_ref(CodeFriend self, StrBuilder* result);

GEN_API StrBuilder fn_to_strbuilder    (CodeFn self);
GEN_API void       fn_to_strbuilder_def(CodeFn self, StrBuilder* result);
GEN_API void       fn_to_strbuilder_fwd(CodeFn self, StrBuilder* result);

GEN_API StrBuilder module_to_strbuilder    (CodeModule self);
GEN_API void       module_to_strbuilder_ref(CodeModule self, StrBuilder* result);

GEN_API StrBuilder namespace_to_strbuilder    (CodeNS self);
GEN_API void       namespace_to_strbuilder_ref(CodeNS self, StrBuilder* result);

GEN_API StrBuilder code_op_to_strbuilder    (CodeOperator self);
GEN_API void       code_op_to_strbuilder_fwd(CodeOperator self, StrBuilder* result );
GEN_API void       code_op_to_strbuilder_def(CodeOperator self, StrBuilder* result );

GEN_API StrBuilder opcast_to_strbuilder     (CodeOpCast op_cast );
GEN_API void       opcast_to_strbuilder_def(CodeOpCast op_cast, StrBuilder* result );
GEN_API void       opcast_to_strbuilder_fwd(CodeOpCast op_cast, StrBuilder* result );

GEN_API StrBuilder pragma_to_strbuilder    (CodePragma self);
GEN_API void       pragma_to_strbuilder_ref(CodePragma self, StrBuilder* result);

GEN_API StrBuilder preprocess_to_strbuilder       (CodePreprocessCond cond);
GEN_API void       preprocess_to_strbuilder_if    (CodePreprocessCond cond, StrBuilder* result );
GEN_API void       preprocess_to_strbuilder_ifdef (CodePreprocessCond cond, StrBuilder* result );
GEN_API void       preprocess_to_strbuilder_ifndef(CodePreprocessCond cond, StrBuilder* result );
GEN_API void       preprocess_to_strbuilder_elif  (CodePreprocessCond cond, StrBuilder* result );
GEN_API void       preprocess_to_strbuilder_else  (CodePreprocessCond cond, StrBuilder* result );
GEN_API void       preprocess_to_strbuilder_endif (CodePreprocessCond cond, StrBuilder* result );

GEN_API StrBuilder template_to_strbuilder    (CodeTemplate self);
GEN_API void       template_to_strbuilder_ref(CodeTemplate self, StrBuilder* result);

GEN_API StrBuilder typename_to_strbuilder    (CodeTypename self);
GEN_API void       typename_to_strbuilder_ref(CodeTypename self, StrBuilder* result);

GEN_API StrBuilder typedef_to_strbuilder    (CodeTypedef self);
GEN_API void       typedef_to_strbuilder_ref(CodeTypedef self, StrBuilder* result );

GEN_API StrBuilder union_to_strbuilder    (CodeUnion self);
GEN_API void       union_to_strbuilder_def(CodeUnion self, StrBuilder* result);
GEN_API void       union_to_strbuilder_fwd(CodeUnion self, StrBuilder* result);

GEN_API StrBuilder using_to_strbuilder    (CodeUsing op_cast );
GEN_API void       using_to_strbuilder_ref(CodeUsing op_cast, StrBuilder* result );
GEN_API void       using_to_strbuilder_ns (CodeUsing op_cast, StrBuilder* result );

GEN_API StrBuilder var_to_strbuilder    (CodeVar self);
GEN_API void       var_to_strbuilder_ref(CodeVar self, StrBuilder* result);

#pragma endregion Code Type C-Interface

#if GEN_COMPILER_CPP
#pragma region Code Types C++

// These structs are not used at all by the C vairant.
static_assert( GEN_COMPILER_CPP, "This should not be compiled with the C-library" );

#define Verify_POD(Type) static_assert(size_of(Code##Type) == size_of(AST_##Type), "ERROR: Code##Type is not a POD")

struct CodeBody
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeBody );
	forceinline void       append( Code other )                       { return body_append( *this, other ); }
	forceinline void       append( CodeBody body )                    { return body_append(*this, body); }
	forceinline bool       has_entries()                              { return code_has_entries(* this); }
	forceinline StrBuilder to_strbuilder()                            { return body_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result )        { return body_to_strbuilder_ref(* this, & result ); }
	forceinline void       to_strbuilder_export( StrBuilder& result ) { return body_to_strbuilder_export(* this, & result); }

#endif
	forceinline Code begin() { return begin_CodeBody(* this); }
	forceinline Code end()   { return end_CodeBody(* this); }
	Using_CodeOps( CodeBody );
	forceinline operator Code() { return * rcast( Code*, this ); }
	forceinline AST_Body* operator->() { return ast; }
	AST_Body* ast;
};

struct CodeClass
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeClass );
	forceinline void       add_interface( CodeType interface );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder_def( StrBuilder& result );
	forceinline void       to_strbuilder_fwd( StrBuilder& result );
#endif
	Using_CodeOps( CodeClass );
	forceinline operator Code() { return * rcast( Code*, this ); }
	forceinline AST_Class* operator->() {
		GEN_ASSERT(ast);
		return ast;
	}
	AST_Class* ast;
};

struct CodeParams
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeParams );
	forceinline void          append( CodeParams other );
	forceinline CodeParams    get( s32 idx );
	forceinline bool          has_entries();
	forceinline StrBuilder    to_strbuilder();
	forceinline void          to_strbuilder( StrBuilder& result );

#endif
	Using_CodeOps( CodeParams );
	forceinline CodeParams begin() { return begin_CodeParams(* this); }
	forceinline CodeParams end()   { return end_CodeParams(* this); }
	forceinline operator Code() { return { (AST*)ast }; }
	forceinline CodeParams  operator *() { return * this; } // Required to support for-range iteration.
	forceinline AST_Params* operator->() {
		GEN_ASSERT(ast);
		return ast;
	}
	CodeParams& operator++();
	AST_Params* ast;
};

struct CodeSpecifiers
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeSpecifiers );
	bool       append( Specifier spec )            { return specifiers_append(* this, spec); }
	s32        has( Specifier spec )               { return specifiers_has(* this, spec); }
	s32        remove( Specifier to_remove )       { return specifiers_remove(* this, to_remove); }
	StrBuilder to_strbuilder()                     { return specifiers_to_strbuilder(* this ); }
	void       to_strbuilder( StrBuilder& result ) { return specifiers_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeSpecifiers);
	forceinline operator Code() { return { (AST*) ast }; }
	forceinline Code            operator *() { return * this; } // Required to support for-range iteration.
	forceinline AST_Specifiers* operator->() {
		GEN_ASSERT(ast);
		return ast;
	}
	AST_Specifiers* ast;
};

struct CodeAttributes
{
#if ! GEN_C_LIKE_CPP
	Using_Code(CodeAttributes);
	forceinline StrBuilder to_strbuilder()                   { return attributes_to_strbuilder(* this); }
	forceinline void       to_strbuilder(StrBuilder& result) { return attributes_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeAttributes);
	operator Code();
	AST_Attributes *operator->();
	AST_Attributes *ast;
};

// Define_CodeType( BaseClass );

struct CodeComment
{
#if ! GEN_C_LIKE_CPP
	Using_Code(CodeComment);
	forceinline StrBuilder to_strbuilder()                   { return comment_to_strbuilder    (* this); }
	forceinline void       to_strbuilder(StrBuilder& result) { return comment_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeComment);
	operator Code();
	AST_Comment *operator->();
	AST_Comment *ast;
};

struct CodeConstructor
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeConstructor );
	forceinline StrBuilder to_strbuilder()                         { return constructor_to_strbuilder(* this); }
	forceinline void       to_strbuilder_def( StrBuilder& result ) { return constructor_to_strbuilder_def(* this, & result); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result ) { return constructor_to_strbuilder_fwd(* this, & result); }
#endif
	Using_CodeOps(CodeConstructor);
	operator         Code();
	AST_Constructor* operator->();
	AST_Constructor* ast;
};

struct CodeDefine
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeDefine );
	forceinline StrBuilder to_strbuilder()                     { return define_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return define_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeDefine);
	operator    Code();
	AST_Define* operator->();
	AST_Define* ast;
};

struct CodeDestructor
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeDestructor );
	forceinline StrBuilder to_strbuilder()                         { return destructor_to_strbuilder(* this); }
	forceinline void       to_strbuilder_def( StrBuilder& result ) { return destructor_to_strbuilder_def(* this, & result); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result ) { return destructor_to_strbuilder_fwd(* this, & result); }
#endif
	Using_CodeOps(CodeDestructor);
	operator         Code();
	AST_Destructor* operator->();
	AST_Destructor* ast;
};

struct CodeEnum
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeEnum );
	forceinline StrBuilder to_strbuilder()                                { return enum_to_strbuilder(* this); }
	forceinline void       to_strbuilder_def( StrBuilder& result )        { return enum_to_strbuilder_def(* this, & result); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result )        { return enum_to_strbuilder_fwd(* this, & result); }
	forceinline void       to_strbuilder_class_def( StrBuilder& result )  { return enum_to_strbuilder_class_def(* this, & result); }
	forceinline void       to_strbuilder_class_fwd( StrBuilder& result )  { return enum_to_strbuilder_class_fwd(* this, & result); }
#endif
	Using_CodeOps(CodeEnum);
	operator  Code();
	AST_Enum* operator->();
	AST_Enum* ast;
};

struct CodeExec
{
#if ! GEN_C_LIKE_CPP
	Using_Code(CodeExec);
	forceinline StrBuilder to_strbuilder()               { return exec_to_strbuilder(* this); }
	forceinline void   to_strbuilder(StrBuilder& result) { return exec_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeExec);
	operator Code();
	AST_Exec *operator->();
	AST_Exec *ast;
};

#if GEN_EXECUTION_EXPRESSION_SUPPORT
struct CodeExpr
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator  Code();
	AST_Expr* operator->();
	AST_Expr* ast;
};

struct CodeExpr_Assign
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Assign );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator         Code();
	AST_Expr_Assign* operator->();
	AST_Expr_Assign* ast;
};

struct CodeExpr_Alignof
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Alignof );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator          Code();
	AST_Expr_Alignof* operator->();
	AST_Expr_Alignof* ast;
};

struct CodeExpr_Binary
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Binary );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator         Code();
	AST_Expr_Binary* operator->();
	AST_Expr_Binary* ast;
};

struct CodeExpr_CStyleCast
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_CStyleCast );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator             Code();
	AST_Expr_CStyleCast* operator->();
	AST_Expr_CStyleCast* ast;
};

struct CodeExpr_FunctionalCast
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_FunctionalCast );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator                 Code();
	AST_Expr_FunctionalCast* operator->();
	AST_Expr_FunctionalCast* ast;
};

struct CodeExpr_CppCast
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_CppCast );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator          Code();
	AST_Expr_CppCast* operator->();
	AST_Expr_CppCast* ast;
};

struct CodeExpr_Element
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Element );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator          Code();
	AST_Expr_Element* operator->();
	AST_Expr_Element* ast;
};

struct CodeExpr_ProcCall
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_ProcCall );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator           Code();
	AST_Expr_ProcCall* operator->();
	AST_Expr_ProcCall* ast;
};

struct CodeExpr_Decltype
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Decltype );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator  Code();
	AST_Expr_Decltype* operator->();
	AST_Expr_Decltype* ast;
};

struct CodeExpr_Comma
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Comma );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator  Code();
	AST_Expr_Comma* operator->();
	AST_Expr_Comma* ast;
};

struct CodeExpr_AMS
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_AMS );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator      Code();
	AST_Expr_AMS* operator->();
	AST_Expr_AMS* ast;
};

struct CodeExpr_Sizeof
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Sizeof );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator         Code();
	AST_Expr_Sizeof* operator->();
	AST_Expr_Sizeof* ast;
};

struct CodeExpr_Subscript
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Subscript );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator            Code();
	AST_Expr_Subscript* operator->();
	AST_Expr_Subscript* ast;
};

struct CodeExpr_Ternary
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_Ternary );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator          Code();
	AST_Expr_Ternary* operator->();
	AST_Expr_Ternary* ast;
};

struct CodeExpr_UnaryPrefix
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_UnaryPrefix );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	operator              Code();
	AST_Expr_UnaryPrefix* operator->();
	AST_Expr_UnaryPrefix* ast;
};

struct CodeExpr_UnaryPostfix
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExpr_UnaryPostfix );
	forceinline void to_strbuilder( StrBuilder& result );
#endif
	AST*                   raw();
	operator               Code();
	AST_Expr_UnaryPostfix* operator->();
	AST_Expr_UnaryPostfix* ast;
};
#endif

struct CodeExtern
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeExtern );
	forceinline void to_strbuilder( StrBuilder& result ) { return extern_to_strbuilder(* this, & result); }
#endif
	Using_CodeOps(CodeExtern);
	operator    Code();
	AST_Extern* operator->();
	AST_Extern* ast;
};

struct CodeInclude
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeInclude );
	forceinline StrBuilder to_strbuilder()                      { return include_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result )  { return include_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeInclude);
	operator     Code();
	AST_Include* operator->();
	AST_Include* ast;
};

struct CodeFriend
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeFriend );
	forceinline StrBuilder to_strbuilder()                     { return friend_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return friend_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeFriend);
	operator    Code();
	AST_Friend* operator->();
	AST_Friend* ast;
};

struct CodeFn
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeFn );
	forceinline StrBuilder to_strbuilder()                         { return fn_to_strbuilder(* this); }
	forceinline void       to_strbuilder_def( StrBuilder& result ) { return fn_to_strbuilder_def(* this, & result); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result ) { return fn_to_strbuilder_fwd(* this, & result); }
#endif
	Using_CodeOps(CodeFn);
	operator Code();
	AST_Fn*  operator->();
	AST_Fn*  ast;
};

struct CodeModule
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeModule );
	forceinline StrBuilder to_strbuilder()                     { return module_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return module_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeModule);
	operator    Code();
	AST_Module* operator->();
	AST_Module* ast;
};

struct CodeNS
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeNS );
	forceinline StrBuilder to_strbuilder()                     { return namespace_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return namespace_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeNS);
	operator Code();
	AST_NS*  operator->();
	AST_NS*  ast;
};

struct CodeOperator
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeOperator );
	forceinline StrBuilder to_strbuilder()                         { return code_op_to_strbuilder(* this); }
	forceinline void       to_strbuilder_def( StrBuilder& result ) { return code_op_to_strbuilder_def(* this, & result); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result ) { return code_op_to_strbuilder_fwd(* this, & result); }
#endif
	Using_CodeOps(CodeOperator);
	operator      Code();
	AST_Operator* operator->();
	AST_Operator* ast;
};

struct CodeOpCast
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeOpCast );
	forceinline StrBuilder to_strbuilder()                         { return opcast_to_strbuilder(* this); }
	forceinline void       to_strbuilder_def( StrBuilder& result ) { return opcast_to_strbuilder_def(* this, & result); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result ) { return opcast_to_strbuilder_fwd(* this, & result); }
#endif
	Using_CodeOps(CodeOpCast);
	operator    Code();
	AST_OpCast* operator->();
	AST_OpCast* ast;
};

struct CodePragma
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodePragma );
	forceinline StrBuilder to_strbuilder()                     { return pragma_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return pragma_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps( CodePragma );
	operator    Code();
	AST_Pragma* operator->();
	AST_Pragma* ast;
};

struct CodePreprocessCond
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodePreprocessCond );
	forceinline StrBuilder to_strbuilder()                            { return preprocess_to_strbuilder(* this); }
	forceinline void       to_strbuilder_if( StrBuilder& result )     { return preprocess_to_strbuilder_if(* this, & result); }
	forceinline void       to_strbuilder_ifdef( StrBuilder& result )  { return preprocess_to_strbuilder_ifdef(* this, & result); }
	forceinline void       to_strbuilder_ifndef( StrBuilder& result ) { return preprocess_to_strbuilder_ifndef(* this, & result); }
	forceinline void       to_strbuilder_elif( StrBuilder& result )   { return preprocess_to_strbuilder_elif(* this, & result); }
	forceinline void       to_strbuilder_else( StrBuilder& result )   { return preprocess_to_strbuilder_else(* this, & result); }
	forceinline void       to_strbuilder_endif( StrBuilder& result )  { return preprocess_to_strbuilder_endif(* this, & result); }
#endif
	Using_CodeOps( CodePreprocessCond );
	operator            Code();
	AST_PreprocessCond* operator->();
	AST_PreprocessCond* ast;
};

#if GEN_EXECUTION_EXPRESSION_SUPPORT
struct CodeStmt
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator  Code();
	AST_Stmt* operator->();
	AST_Stmt* ast;
};

struct CodeStmt_Break
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Break );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator        Code();
	AST_Stmt_Break* operator->();
	AST_Stmt_Break* ast;
};

struct CodeStmt_Case
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Case );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator       Code();
	AST_Stmt_Case* operator->();
	AST_Stmt_Case* ast;
};

struct CodeStmt_Continue
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Continue );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator           Code();
	AST_Stmt_Continue* operator->();
	AST_Stmt_Continue* ast;
};

struct CodeStmt_Decl
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Decl );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator       Code();
	AST_Stmt_Decl* operator->();
	AST_Stmt_Decl* ast;
};

struct CodeStmt_Do
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Do );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator     Code();
	AST_Stmt_Do* operator->();
	AST_Stmt_Do* ast;
};

struct CodeStmt_Expr
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Expr );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator       Code();
	AST_Stmt_Expr* operator->();
	AST_Stmt_Expr* ast;
};

struct CodeStmt_Else
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Else );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator       Code();
	AST_Stmt_Else* operator->();
	AST_Stmt_Else* ast;
};

struct CodeStmt_If
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_If );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator     Code();
	AST_Stmt_If* operator->();
	AST_Stmt_If* ast;
};

struct CodeStmt_For
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_For );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator      Code();
	AST_Stmt_For* operator->();
	AST_Stmt_For* ast;
};

struct CodeStmt_Goto
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Goto );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator       Code();
	AST_Stmt_Goto* operator->();
	AST_Stmt_Goto* ast;
};

struct CodeStmt_Label
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Label );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator        Code();
	AST_Stmt_Label* operator->();
	AST_Stmt_Label* ast;
};

struct CodeStmt_Switch
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_Switch );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator       Code();
	AST_Stmt_Switch* operator->();
	AST_Stmt_Switch* ast;
};

struct CodeStmt_While
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStmt_While );
	forceinline StrBuilder to_strbuilder();
	forceinline void       to_strbuilder( StrBuilder& result );
#endif
	operator       Code();
	AST_Stmt_While* operator->();
	AST_Stmt_While* ast;
};
#endif

struct CodeTemplate
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeTemplate );
	forceinline StrBuilder to_strbuilder()                     { return template_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return template_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps( CodeTemplate );
	operator      Code();
	AST_Template* operator->();
	AST_Template* ast;
};

struct CodeTypename
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeTypename );
	forceinline StrBuilder to_strbuilder()                     { return typename_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return typename_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps( CodeTypename );
	operator      Code();
	AST_Typename* operator->();
	AST_Typename* ast;
};

struct CodeTypedef
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeTypedef );
	forceinline StrBuilder to_strbuilder()                     { return typedef_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return typedef_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps( CodeTypedef );
	operator     Code();
	AST_Typedef* operator->();
	AST_Typedef* ast;
};

struct CodeUnion
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeUnion );
	forceinline StrBuilder to_strbuilder()                         { return union_to_strbuilder(* this); }
	forceinline void       to_strbuilder_def( StrBuilder& result ) { return union_to_strbuilder_def(* this, & result); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result ) { return union_to_strbuilder_fwd(* this, & result); }
#endif
	Using_CodeOps(CodeUnion);
	operator   Code();
	AST_Union* operator->();
	AST_Union* ast;
};

struct CodeUsing
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeUsing );
	forceinline StrBuilder to_strbuilder()                        { return using_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result )    { return using_to_strbuilder_ref(* this, & result); }
	forceinline void       to_strbuilder_ns( StrBuilder& result ) { return using_to_strbuilder_ns(* this, & result); }
#endif
	Using_CodeOps(CodeUsing);
	operator   Code();
	AST_Using* operator->();
	AST_Using* ast;
};

struct CodeVar
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeVar );
	forceinline StrBuilder to_strbuilder()                     { return var_to_strbuilder(* this); }
	forceinline void       to_strbuilder( StrBuilder& result ) { return var_to_strbuilder_ref(* this, & result); }
#endif
	Using_CodeOps(CodeVar);
	operator Code();
	AST_Var* operator->();
	AST_Var* ast;
};

struct CodeStruct
{
#if ! GEN_C_LIKE_CPP
	Using_Code( CodeStruct );
	forceinline void       add_interface( CodeTypename interface ) { return struct_add_interface(* this, interface); }
	forceinline StrBuilder to_strbuilder()                         { return struct_to_strbuilder(* this); }
	forceinline void       to_strbuilder_fwd( StrBuilder& result ) { return struct_to_strbuilder_fwd(* this, & result); }
	forceinline void       to_strbuilder_def( StrBuilder& result ) { return struct_to_strbuilder_def(* this, & result); }
#endif
	Using_CodeOps( CodeStruct );
	forceinline operator Code() { return * rcast( Code*, this ); }
	forceinline AST_Struct* operator->() {
		GEN_ASSERT(ast);
		return ast;
	}
	AST_Struct* ast;
};

#undef Define_CodeType
#undef Using_Code
#undef Using_CodeOps

#undef Verify_POD

struct InvalidCode_ImplictCaster
{
	// operator CodeBaseClass() const;
    operator Code              () const { return Code_Invalid; }
    operator CodeBody          () const { return cast(CodeBody,           Code_Invalid); }
    operator CodeAttributes    () const { return cast(CodeAttributes,     Code_Invalid); }
    operator CodeComment       () const { return cast(CodeComment,        Code_Invalid); }
    operator CodeClass         () const { return cast(CodeClass,          Code_Invalid); }
    operator CodeConstructor   () const { return cast(CodeConstructor,    Code_Invalid); }
    operator CodeDefine        () const { return cast(CodeDefine,         Code_Invalid); }
    operator CodeDestructor    () const { return cast(CodeDestructor,     Code_Invalid); }
    operator CodeExec          () const { return cast(CodeExec,           Code_Invalid); }
    operator CodeEnum          () const { return cast(CodeEnum,           Code_Invalid); }
    operator CodeExtern        () const { return cast(CodeExtern,         Code_Invalid); }
    operator CodeInclude       () const { return cast(CodeInclude,        Code_Invalid); }
    operator CodeFriend        () const { return cast(CodeFriend,         Code_Invalid); }
    operator CodeFn            () const { return cast(CodeFn,             Code_Invalid); }
    operator CodeModule        () const { return cast(CodeModule,         Code_Invalid); }
    operator CodeNS            () const { return cast(CodeNS,             Code_Invalid); }
    operator CodeOperator      () const { return cast(CodeOperator,       Code_Invalid); }
    operator CodeOpCast        () const { return cast(CodeOpCast,         Code_Invalid); }
    operator CodeParams        () const { return cast(CodeParams,         Code_Invalid); }
    operator CodePragma        () const { return cast(CodePragma,         Code_Invalid); }
    operator CodePreprocessCond() const { return cast(CodePreprocessCond, Code_Invalid); }
    operator CodeSpecifiers    () const { return cast(CodeSpecifiers,     Code_Invalid); }
    operator CodeStruct        () const { return cast(CodeStruct,         Code_Invalid); }
    operator CodeTemplate      () const { return cast(CodeTemplate,       Code_Invalid); }
    operator CodeTypename      () const { return cast(CodeTypename,       Code_Invalid); }
    operator CodeTypedef       () const { return cast(CodeTypedef,        Code_Invalid); }
    operator CodeUnion         () const { return cast(CodeUnion,          Code_Invalid); }
    operator CodeUsing         () const { return cast(CodeUsing,          Code_Invalid); }
    operator CodeVar           () const { return cast(CodeVar,            Code_Invalid); }
};

struct NullCode_ImplicitCaster
{
    operator Code              () const { return {nullptr}; }
    operator CodeBody          () const { return {(AST_Body*)      nullptr}; }
    operator CodeAttributes    () const { return {(AST_Attributes*)nullptr}; }
    operator CodeComment       () const { return {nullptr}; }
    operator CodeClass         () const { return {nullptr}; }
    operator CodeConstructor   () const { return {nullptr}; }
    operator CodeDefine        () const { return {nullptr}; }
    operator CodeDestructor    () const { return {nullptr}; }
    operator CodeExec          () const { return {nullptr}; }
    operator CodeEnum          () const { return {nullptr}; }
    operator CodeExtern        () const { return {nullptr}; }
    operator CodeInclude       () const { return {nullptr}; }
    operator CodeFriend        () const { return {nullptr}; }
    operator CodeFn            () const { return {nullptr}; }
    operator CodeModule        () const { return {nullptr}; }
    operator CodeNS            () const { return {nullptr}; }
    operator CodeOperator      () const { return {nullptr}; }
    operator CodeOpCast        () const { return {nullptr}; }
    operator CodeParams        () const { return {nullptr}; }
    operator CodePragma        () const { return {nullptr}; }
    operator CodePreprocessCond() const { return {nullptr}; }
    operator CodeSpecifiers    () const { return {nullptr}; }
    operator CodeStruct        () const { return {nullptr}; }
    operator CodeTemplate      () const { return {nullptr}; }
    operator CodeTypename      () const { return CodeTypename{(AST_Typename*)nullptr}; }
    operator CodeTypedef       () const { return {nullptr}; }
    operator CodeUnion         () const { return {nullptr}; }
    operator CodeUsing         () const { return {nullptr}; }
    operator CodeVar           () const { return {nullptr}; }
};

forceinline Code begin( CodeBody body)                   { return begin_CodeBody(body); }
forceinline Code end  ( CodeBody body )                  { return end_CodeBody(body); }
forceinline Code next ( CodeBody body, Code entry_iter ) { return next_CodeBody(body, entry_iter); }

forceinline CodeParams begin(CodeParams params)                        { return begin_CodeParams(params); }
forceinline CodeParams end  (CodeParams params)                        { return end_CodeParams(params); }
forceinline CodeParams next (CodeParams params, CodeParams entry_iter) { return next_CodeParams(params, entry_iter); }

forceinline Specifier* begin(CodeSpecifiers specifiers)                       { return begin_CodeSpecifiers(specifiers); }
forceinline Specifier* end  (CodeSpecifiers specifiers)                       { return end_CodeSpecifiers(specifiers); }
forceinline Specifier* next (CodeSpecifiers specifiers, Specifier& spec_iter) { return next_CodeSpecifiers(specifiers, & spec_iter); }

#if ! GEN_C_LIKE_CPP
GEN_OPTIMIZE_MAPPINGS_BEGIN

forceinline void       append              ( CodeBody body, Code     other )     { return body_append(body, other); }
forceinline void       append              ( CodeBody body, CodeBody other )     { return body_append_body(body, other); }
forceinline StrBuilder to_strbuilder       ( CodeBody body )                     { return body_to_strbuilder(body); }
forceinline void       to_strbuilder       ( CodeBody body, StrBuilder& result ) { return body_to_strbuilder_ref(body, & result); }
forceinline void       to_strbuilder_export( CodeBody body, StrBuilder& result ) { return body_to_strbuilder_export(body, & result); }

forceinline void       add_interface    ( CodeClass self, CodeTypename interface ) { return class_add_interface(self, interface); }
forceinline StrBuilder to_strbuilder    ( CodeClass self )                         { return class_to_strbuilder(self); }
forceinline void       to_strbuilder_def( CodeClass self, StrBuilder& result )     { return class_to_strbuilder_def(self, & result); }
forceinline void       to_strbuilder_fwd( CodeClass self, StrBuilder& result )     { return class_to_strbuilder_fwd(self, & result); }

forceinline void       append       (CodeParams params, CodeParams param )   { return params_append(params, param); }
forceinline CodeParams get          (CodeParams params, s32 idx)             { return params_get(params, idx); }
forceinline bool       has_entries  (CodeParams params )                     { return params_has_entries(params); }
forceinline StrBuilder to_strbuilder(CodeParams params )                     { return params_to_strbuilder(params); }
forceinline void       to_strbuilder(CodeParams params, StrBuilder& result ) { return params_to_strbuilder_ref(params, & result); }
  
forceinline bool       append       (CodeSpecifiers specifiers, Specifier spec)       { return specifiers_append(specifiers, spec); }
forceinline s32        has          (CodeSpecifiers specifiers, Specifier spec)       { return specifiers_has(specifiers, spec); }
forceinline s32        remove       (CodeSpecifiers specifiers, Specifier to_remove ) { return specifiers_remove(specifiers, to_remove); }
forceinline StrBuilder to_strbuilder(CodeSpecifiers specifiers)                       { return specifiers_to_strbuilder(specifiers); }
forceinline void       to_strbuilder(CodeSpecifiers specifiers, StrBuilder& result)       { return specifiers_to_strbuilder_ref(specifiers, & result);  }

forceinline void       add_interface    (CodeStruct self, CodeTypename interface) { return struct_add_interface(self, interface); }
forceinline StrBuilder to_strbuilder    (CodeStruct self)                         { return struct_to_strbuilder(self); }
forceinline void       to_strbuilder_fwd(CodeStruct self, StrBuilder& result)     { return struct_to_strbuilder_fwd(self, & result); }
forceinline void       to_strbuilder_def(CodeStruct self, StrBuilder& result)     { return struct_to_strbuilder_def(self, & result); }

forceinline StrBuilder to_strbuilder(CodeAttributes attributes)                     { return attributes_to_strbuilder(attributes); }
forceinline void       to_strbuilder(CodeAttributes attributes, StrBuilder& result) { return attributes_to_strbuilder_ref(attributes, & result); }

forceinline StrBuilder to_strbuilder(CodeComment comment )                      { return comment_to_strbuilder(comment); }
forceinline void       to_strbuilder(CodeComment comment, StrBuilder& result )  { return comment_to_strbuilder_ref(comment, & result); }

forceinline StrBuilder to_strbuilder    (CodeConstructor constructor)                      { return constructor_to_strbuilder(constructor); }
forceinline void       to_strbuilder_def(CodeConstructor constructor, StrBuilder& result ) { return constructor_to_strbuilder_def(constructor, & result); }
forceinline void       to_strbuilder_fwd(CodeConstructor constructor, StrBuilder& result ) { return constructor_to_strbuilder_fwd(constructor, & result); }

forceinline StrBuilder to_strbuilder(CodeDefine self)                     { return define_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeDefine self, StrBuilder& result) { return define_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder    (CodeDestructor destructor)                      { return destructor_to_strbuilder(destructor); }
forceinline void       to_strbuilder_def(CodeDestructor destructor, StrBuilder& result ) { return destructor_to_strbuilder_def(destructor, & result); }
forceinline void       to_strbuilder_fwd(CodeDestructor destructor, StrBuilder& result ) { return destructor_to_strbuilder_fwd(destructor, & result); }

forceinline StrBuilder to_strbuilder          (CodeEnum self)                      { return enum_to_strbuilder(self); }
forceinline void       to_strbuilder_def      (CodeEnum self, StrBuilder& result ) { return enum_to_strbuilder_def(self, & result); }
forceinline void       to_strbuilder_fwd      (CodeEnum self, StrBuilder& result ) { return enum_to_strbuilder_fwd(self, & result); }
forceinline void       to_strbuilder_class_def(CodeEnum self, StrBuilder& result ) { return enum_to_strbuilder_class_def(self, & result); }
forceinline void       to_strbuilder_class_fwd(CodeEnum self, StrBuilder& result ) { return enum_to_strbuilder_class_fwd(self, & result); }

forceinline StrBuilder to_strbuilder(CodeExec exec)                     { return exec_to_strbuilder(exec); }
forceinline void       to_strbuilder(CodeExec exec, StrBuilder& result) { return exec_to_strbuilder_ref(exec, & result); }

forceinline void to_strbuilder(CodeExtern self, StrBuilder& result) { return extern_to_strbuilder(self, & result); }

forceinline StrBuilder to_strbuilder(CodeInclude self)                     { return include_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeInclude self, StrBuilder& result) { return include_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder(CodeFriend self)                     { return friend_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeFriend self, StrBuilder& result) { return friend_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder    (CodeFn self)                     { return fn_to_strbuilder(self); }
forceinline void       to_strbuilder_def(CodeFn self, StrBuilder& result) { return fn_to_strbuilder_def(self, & result); }
forceinline void       to_strbuilder_fwd(CodeFn self, StrBuilder& result) { return fn_to_strbuilder_fwd(self, & result); }

forceinline StrBuilder to_strbuilder(CodeModule self)                     { return module_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeModule self, StrBuilder& result) { return module_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder(CodeNS self)                     { return namespace_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeNS self, StrBuilder& result) { return namespace_to_strbuilder_ref(self,  & result); }

forceinline StrBuilder to_strbuilder    (CodeOperator self)                      { return code_op_to_strbuilder(self); }
forceinline void       to_strbuilder_fwd(CodeOperator self, StrBuilder& result ) { return code_op_to_strbuilder_fwd(self, & result); }
forceinline void       to_strbuilder_def(CodeOperator self, StrBuilder& result ) { return code_op_to_strbuilder_def(self, & result); }

forceinline StrBuilder to_strbuilder    (CodeOpCast op_cast )                     { return opcast_to_strbuilder(op_cast); }
forceinline void       to_strbuilder_def(CodeOpCast op_cast, StrBuilder& result ) { return opcast_to_strbuilder_def(op_cast, & result); }
forceinline void       to_strbuilder_fwd(CodeOpCast op_cast, StrBuilder& result ) { return opcast_to_strbuilder_fwd(op_cast, & result); }

forceinline StrBuilder to_strbuilder(CodePragma self)                     { return pragma_to_strbuilder(self); }
forceinline void       to_strbuilder(CodePragma self, StrBuilder& result) { return pragma_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder       (CodePreprocessCond cond)                      { return preprocess_to_strbuilder(cond); }
forceinline void       to_strbuilder_if    (CodePreprocessCond cond, StrBuilder& result ) { return preprocess_to_strbuilder_if(cond, & result); }
forceinline void       to_strbuilder_ifdef (CodePreprocessCond cond, StrBuilder& result ) { return preprocess_to_strbuilder_ifdef(cond, & result); }
forceinline void       to_strbuilder_ifndef(CodePreprocessCond cond, StrBuilder& result ) { return preprocess_to_strbuilder_ifndef(cond, & result); }
forceinline void       to_strbuilder_elif  (CodePreprocessCond cond, StrBuilder& result ) { return preprocess_to_strbuilder_elif(cond, & result); }
forceinline void       to_strbuilder_else  (CodePreprocessCond cond, StrBuilder& result ) { return preprocess_to_strbuilder_else(cond, & result); }
forceinline void       to_strbuilder_endif (CodePreprocessCond cond, StrBuilder& result ) { return preprocess_to_strbuilder_endif(cond, & result); }

forceinline StrBuilder to_strbuilder(CodeTemplate self)                     { return template_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeTemplate self, StrBuilder& result) { return template_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder(CodeTypename self)                     { return typename_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeTypename self, StrBuilder& result) { return typename_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder(CodeTypedef self)                      { return typedef_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeTypedef self, StrBuilder& result ) { return typedef_to_strbuilder_ref(self, & result); }

forceinline StrBuilder to_strbuilder    (CodeUnion self)                     { return union_to_strbuilder(self); }
forceinline void       to_strbuilder_def(CodeUnion self, StrBuilder& result) { return union_to_strbuilder_def(self, & result); }
forceinline void       to_strbuilder_fwd(CodeUnion self, StrBuilder& result) { return union_to_strbuilder_fwd(self, & result); }

forceinline StrBuilder to_strbuilder   (CodeUsing op_cast )                     { return using_to_strbuilder(op_cast); }
forceinline void       to_strbuilder   (CodeUsing op_cast, StrBuilder& result ) { return using_to_strbuilder_ref(op_cast, & result); }
forceinline void       to_strbuilder_ns(CodeUsing op_cast, StrBuilder& result ) { return using_to_strbuilder_ns(op_cast, & result); }

forceinline StrBuilder to_strbuilder(CodeVar self)                     { return var_to_strbuilder(self); }
forceinline void       to_strbuilder(CodeVar self, StrBuilder& result) { return var_to_strbuilder_ref(self, & result); }

GEN_OPITMIZE_MAPPINGS_END
#endif //if GEN_C_LIKE_CPP

#pragma endregion Code Types C++
#endif //if GEN_COMPILER_CPP

#pragma region AST Types

/*
  ______   ______  ________      ________
 /      \ /      \|        \    |        \
|  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\\▓▓▓▓▓▓▓▓     \▓▓▓▓▓▓▓▓__    __  ______   ______   _______
| ▓▓__| ▓▓ ▓▓___\▓▓  | ▓▓          | ▓▓  |  \  |  \/      \ /      \ /       \
| ▓▓    ▓▓\▓▓    \   | ▓▓          | ▓▓  | ▓▓  | ▓▓  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓
| ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\  | ▓▓          | ▓▓  | ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓\▓▓    \
| ▓▓  | ▓▓  \__| ▓▓  | ▓▓          | ▓▓  | ▓▓__/ ▓▓ ▓▓__/ ▓▓ ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\
| ▓▓  | ▓▓\▓▓    ▓▓  | ▓▓          | ▓▓   \▓▓    ▓▓ ▓▓    ▓▓\▓▓     \       ▓▓
 \▓▓   \▓▓ \▓▓▓▓▓▓    \▓▓           \▓▓   _\▓▓▓▓▓▓▓ ▓▓▓▓▓▓▓  \▓▓▓▓▓▓▓\▓▓▓▓▓▓▓
                                         |  \__| ▓▓ ▓▓
                                          \▓▓    ▓▓ ▓▓
                                           \▓▓▓▓▓▓ \▓▓
*/

/*
	Show only relevant members of the AST for its type.
	AST* fields are replaced with Code types.
		- Guards assignemnts to AST* fields to ensure the AST is duplicated if assigned to another parent.
*/

struct AST_Body
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached      Name;
	Code              Front;
	Code              Back;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char              _PAD_UNUSED_[ sizeof(ModuleFlag) ];
	s32 			  NumEntries;
};
static_assert( sizeof(AST_Body) == sizeof(AST), "ERROR: AST_Body is not the same size as AST");

struct AST_Attributes
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		StrCached  Content;
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char              _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Attributes) == sizeof(AST), "ERROR: AST_Attributes is not the same size as AST");

#if 0
struct AST_BaseClass
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char              _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_BaseClass) == sizeof(AST), "ERROR: AST_BaseClass is not the same size as AST");
#endif

struct AST_Comment
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		StrCached  Content;
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char              _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Comment) == sizeof(AST), "ERROR: AST_Comment is not the same size as AST");

struct AST_Class
{
	union {
		char                _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment     InlineCmt; // Only supported by forward declarations
			CodeAttributes  Attributes;
			char 	        _PAD_SPECS_ [ sizeof(AST*) ];
			CodeTypename    ParentType;
			char 	        _PAD_PARAMS_[ sizeof(AST*) ];
			CodeBody        Body;
			char 	        _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached            Name;
	CodeTypename            Prev;
	CodeTypename            Next;
	Token*                  Tok;
	Code                    Parent;
	CodeType                Type;
	ModuleFlag              ModuleFlags;
	AccessSpec              ParentAccess;
};
static_assert( sizeof(AST_Class) == sizeof(AST), "ERROR: AST_Class is not the same size as AST");

struct AST_Constructor
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment    InlineCmt; // Only supported by forward declarations
			char           _PAD_PROPERTIES_ [ sizeof(AST*) * 1 ];
			CodeSpecifiers Specs;
			Code           InitializerList;
			CodeParams     Params;
			Code           Body;
			char 		   _PAD_PROPERTIES_2_ [ sizeof(AST*) * 2 ];
		};
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char              _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Constructor) == sizeof(AST), "ERROR: AST_Constructor is not the same size as AST");

struct AST_Define
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		StrCached  Content;
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Define) == sizeof(AST), "ERROR: AST_Define is not the same size as AST");

struct AST_Destructor
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment    InlineCmt;
			char           _PAD_PROPERTIES_ [ sizeof(AST*) * 1 ];
			CodeSpecifiers Specs;
			char           _PAD_PROPERTIES_2_ [ sizeof(AST*) * 2 ];
			Code           Body;
			char 		   _PAD_PROPERTIES_3_ [ sizeof(AST*) ];
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	char                   _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Destructor) == sizeof(AST), "ERROR: AST_Destructor is not the same size as AST");

struct AST_Enum
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment    InlineCmt;
			CodeAttributes Attributes;
			char           _PAD_SPEC_  [ sizeof(AST*) ];
			CodeTypename   UnderlyingType;
			Code           UnderlyingTypeMacro;
			CodeBody       Body;
			char 	       _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	ModuleFlag             ModuleFlags;
	char                   _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Enum) == sizeof(AST), "ERROR: AST_Enum is not the same size as AST");

struct AST_Exec
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		StrCached  Content;
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char              _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Exec) == sizeof(AST), "ERROR: AST_Exec is not the same size as AST");

#if GEN_EXECUTION_EXPRESSION_SUPPORT
struct AST_Expr
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr) == sizeof(AST), "ERROR: AST_Expr is not the same size as AST");

struct AST_Expr_Assign
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Assign) == sizeof(AST), "ERROR: AST_Expr_Assign is not the same size as AST");

struct AST_Expr_Alignof
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Alignof) == sizeof(AST), "ERROR: AST_Expr_Alignof is not the same size as AST");

struct AST_Expr_Binary
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Binary) == sizeof(AST), "ERROR: AST_Expr_Binary is not the same size as AST");

struct AST_Expr_CStyleCast
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_CStyleCast) == sizeof(AST), "ERROR: AST_Expr_CStyleCast is not the same size as AST");

struct AST_Expr_FunctionalCast
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_FunctionalCast) == sizeof(AST), "ERROR: AST_Expr_FunctionalCast is not the same size as AST");

struct AST_Expr_CppCast
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_CppCast) == sizeof(AST), "ERROR: AST_Expr_CppCast is not the same size as AST");

struct AST_Expr_ProcCall
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_ProcCall) == sizeof(AST), "ERROR: AST_Expr_Identifier is not the same size as AST");

struct AST_Expr_Decltype
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Decltype) == sizeof(AST), "ERROR: AST_Expr_Decltype is not the same size as AST");

struct AST_Expr_Comma
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Comma) == sizeof(AST), "ERROR: AST_Expr_Comma is not the same size as AST");

struct AST_Expr_AMS
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_AMS) == sizeof(AST), "ERROR: AST_Expr_AMS is not the same size as AST");

struct AST_Expr_Sizeof
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Sizeof) == sizeof(AST), "ERROR: AST_Expr_Sizeof is not the same size as AST");

struct AST_Expr_Subscript
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Subscript) == sizeof(AST), "ERROR: AST_Expr_Subscript is not the same size as AST");

struct AST_Expr_Ternary
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Ternary) == sizeof(AST), "ERROR: AST_Expr_Ternary is not the same size as AST");

struct AST_Expr_UnaryPrefix
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_UnaryPrefix) == sizeof(AST), "ERROR: AST_Expr_UnaryPrefix is not the same size as AST");

struct AST_Expr_UnaryPostfix
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_UnaryPostfix) == sizeof(AST), "ERROR: AST_Expr_UnaryPostfix is not the same size as AST");

struct AST_Expr_Element
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Expr_Element) == sizeof(AST), "ERROR: AST_Expr_Element is not the same size as AST");
#endif

struct AST_Extern
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			char      _PAD_PROPERTIES_[ sizeof(AST*) * 5 ];
			CodeBody  Body;
			char      _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Extern) == sizeof(AST), "ERROR: AST_Extern is not the same size as AST");

struct AST_Include
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		StrCached  Content;
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Include) == sizeof(AST), "ERROR: AST_Include is not the same size as AST");

struct AST_Friend
{
	union {
		char            _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment InlineCmt;
			char        _PAD_PROPERTIES_[ sizeof(AST*) * 4 ];
			Code        Declaration;
			char 	    _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Friend) == sizeof(AST), "ERROR: AST_Friend is not the same size as AST");

struct AST_Fn
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment     InlineCmt;
			CodeAttributes  Attributes;
			CodeSpecifiers  Specs;
			CodeTypename    ReturnType;
			CodeParams 	    Params;
			CodeBody        Body;
			char 	        _PAD_PROPERTIES_ [ sizeof(AST*) ];
		};
	};
	StrCached            Name;
	Code                    Prev;
	Code                    Next;
	Token*                  Tok;
	Code                    Parent;
	CodeType                Type;
	ModuleFlag              ModuleFlags;
	char 			        _PAD_UNUSED_[ sizeof(u32) ];
};
static_assert( sizeof(AST_Fn) == sizeof(AST), "ERROR: AST_Fn is not the same size as AST");

struct AST_Module
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	ModuleFlag        ModuleFlags;
	char 			  _PAD_UNUSED_[ sizeof(u32) ];
};
static_assert( sizeof(AST_Module) == sizeof(AST), "ERROR: AST_Module is not the same size as AST");

struct AST_NS
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct {
			char 	  _PAD_PROPERTIES_[ sizeof(AST*) * 5 ];
			CodeBody  Body;
			char 	  _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	ModuleFlag        ModuleFlags;
	char 			  _PAD_UNUSED_[ sizeof(u32) ];
};
static_assert( sizeof(AST_NS) == sizeof(AST), "ERROR: AST_NS is not the same size as AST");

struct AST_Operator
{
	union {
		char                _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment     InlineCmt;
			CodeAttributes  Attributes;
			CodeSpecifiers  Specs;
			CodeTypename    ReturnType;
			CodeParams 	    Params;
			CodeBody        Body;
			char 	        _PAD_PROPERTIES_ [ sizeof(AST*) ];
		};
	};
	StrCached   Name;
	Code           Prev;
	Code           Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	ModuleFlag     ModuleFlags;
	Operator       Op;
};
static_assert( sizeof(AST_Operator) == sizeof(AST), "ERROR: AST_Operator is not the same size as AST");

struct AST_OpCast
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment     InlineCmt;
			char 	        _PAD_PROPERTIES_[ sizeof(AST*)  ];
			CodeSpecifiers  Specs;
			CodeTypename    ValueType;
			char 	        _PAD_PROPERTIES_2_[ sizeof(AST*) ];
			CodeBody        Body;
			char 	        _PAD_PROPERTIES_3_[ sizeof(AST*) ];
		};
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_OpCast) == sizeof(AST), "ERROR: AST_OpCast is not the same size as AST");

struct AST_Params
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			char 	     _PAD_PROPERTIES_2_[ sizeof(AST*) * 3 ];
			CodeTypename ValueType;
			Code         Macro;
			Code         Value;
			Code         PostNameMacro; // Thanks Unreal
			// char     _PAD_PROPERTIES_3_[sizeof( AST* )];
		};
	};
	StrCached      Name;
	CodeParams        Last;
	CodeParams        Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) ];
	s32               NumEntries;
};
static_assert( sizeof(AST_Params) == sizeof(AST), "ERROR: AST_Params is not the same size as AST");

struct AST_Pragma
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		StrCached  Content;
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Pragma) == sizeof(AST), "ERROR: AST_Pragma is not the same size as AST");

struct AST_PreprocessCond
{
	union {
		char          _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		StrCached  Content;
	};
	StrCached      Name;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_PreprocessCond) == sizeof(AST), "ERROR: AST_PreprocessCond is not the same size as AST");

struct AST_Specifiers
{
	Specifier         ArrSpecs[ AST_ArrSpecs_Cap ];
	StrCached      Name;
	CodeSpecifiers    NextSpecs;
	Code              Prev;
	Code              Next;
	Token*            Tok;
	Code              Parent;
	CodeType          Type;
	char 			  _PAD_UNUSED_[ sizeof(ModuleFlag) ];
	s32               NumEntries;
};
static_assert( sizeof(AST_Specifiers) == sizeof(AST), "ERROR: AST_Specifier is not the same size as AST");

#if GEN_EXECUTION_EXPRESSION_SUPPORT
struct AST_Stmt
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt) == sizeof(AST), "ERROR: AST_Stmt is not the same size as AST");

struct AST_Stmt_Break
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Break) == sizeof(AST), "ERROR: AST_Stmt_Break is not the same size as AST");

struct AST_Stmt_Case
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Case) == sizeof(AST), "ERROR: AST_Stmt_Case is not the same size as AST");

struct AST_Stmt_Continue
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Continue) == sizeof(AST), "ERROR: AST_Stmt_Continue is not the same size as AST");

struct AST_Stmt_Decl
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Decl) == sizeof(AST), "ERROR: AST_Stmt_Decl is not the same size as AST");

struct AST_Stmt_Do
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Do) == sizeof(AST), "ERROR: AST_Stmt_Do is not the same size as AST");

struct AST_Stmt_Expr
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Expr) == sizeof(AST), "ERROR: AST_Stmt_Expr is not the same size as AST");

struct AST_Stmt_Else
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Else) == sizeof(AST), "ERROR: AST_Stmt_Else is not the same size as AST");

struct AST_Stmt_If
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_If) == sizeof(AST), "ERROR: AST_Stmt_If is not the same size as AST");

struct AST_Stmt_For
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_For) == sizeof(AST), "ERROR: AST_Stmt_For is not the same size as AST");

struct AST_Stmt_Goto
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Goto) == sizeof(AST), "ERROR: AST_Stmt_Goto is not the same size as AST");

struct AST_Stmt_Label
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Label) == sizeof(AST), "ERROR: AST_Stmt_Label is not the same size as AST");

struct AST_Stmt_Switch
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_Switch) == sizeof(AST), "ERROR: AST_Stmt_Switch is not the same size as AST");

struct AST_Stmt_While
{
	union {
		char _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
	};
	StrCached   Name;
	CodeExpr       Prev;
	CodeExpr       Next;
	Token*         Tok;
	Code           Parent;
	CodeType       Type;
	char           _PAD_UNUSED_[ sizeof(ModuleFlag) + sizeof(u32) ];
};
static_assert( sizeof(AST_Stmt_While) == sizeof(AST), "ERROR: AST_Stmt_While is not the same size as AST");
#endif

struct AST_Struct
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment    InlineCmt;
			CodeAttributes Attributes;
			char 	       _PAD_SPECS_ [ sizeof(AST*) ];
			CodeTypename   ParentType;
			char 	       _PAD_PARAMS_[ sizeof(AST*) ];
			CodeBody       Body;
			char 	       _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached           Name;
	CodeTypename           Prev;
	CodeTypename           Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	ModuleFlag             ModuleFlags;
	AccessSpec             ParentAccess;
};
static_assert( sizeof(AST_Struct) == sizeof(AST), "ERROR: AST_Struct is not the same size as AST");

struct AST_Template
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			char 	       _PAD_PROPERTIES_[ sizeof(AST*) * 4 ];
			CodeParams 	   Params;
			Code           Declaration;
			char 	       _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	ModuleFlag             ModuleFlags;
	char 			       _PAD_UNUSED_[ sizeof(u32) ];
};
static_assert( sizeof(AST_Template) == sizeof(AST), "ERROR: AST_Template is not the same size as AST");

#if 0
// WIP... The type ast is going to become more advanced and lead to a major change to AST design.
struct AST_Type
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			char           _PAD_INLINE_CMT_[ sizeof(AST*) ];
			CodeAttributes  Attributes;
			CodeSpecifiers  Specs;
			Code            QualifierID;
			// CodeTypename ReturnType;      // Only used for function signatures
			// CodeParams    Params;          // Only used for function signatures
			Code            ArrExpr;
			// CodeSpecifiers SpecsFuncSuffix; // Only used for function signatures
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                  Tok;
	Code                   Parent;
	CodeType               Type;
	char 			       _PAD_UNUSED_[ sizeof(ModuleFlag) ];
	b32                    IsParamPack;
};
static_assert( sizeof(AST_Type) == sizeof(AST), "ERROR: AST_Type is not the same size as AST");
#endif

struct AST_Typename
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			char           _PAD_INLINE_CMT_[ sizeof(AST*) ];
			CodeAttributes Attributes;
			CodeSpecifiers Specs;
			CodeTypename   ReturnType;      // Only used for function signatures
			CodeParams     Params;          // Only used for function signatures
			Code           ArrExpr;
			CodeSpecifiers SpecsFuncSuffix; // Only used for function signatures
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	char 			       _PAD_UNUSED_[ sizeof(ModuleFlag) ];
	struct {
		b16                IsParamPack;   // Used by typename to know if type should be considered a parameter pack.
		ETypenameTag       TypeTag;       // Used by typename to keep track of explicitly declared tags for the identifier (enum, struct, union)
	};
};
static_assert( sizeof(AST_Typename) == sizeof(AST), "ERROR: AST_Type is not the same size as AST");

struct AST_Typedef
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment    InlineCmt;
			char 	       _PAD_PROPERTIES_[ sizeof(AST*) * 2 ];
			Code           UnderlyingType;
			char 	       _PAD_PROPERTIES_2_[ sizeof(AST*) * 3 ];
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	ModuleFlag             ModuleFlags;
	b32                    IsFunction;
};
static_assert( sizeof(AST_Typedef) == sizeof(AST), "ERROR: AST_Typedef is not the same size as AST");

struct AST_Union
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			char           _PAD_INLINE_CMT_[ sizeof(AST*) ];
			CodeAttributes Attributes;
			char 	       _PAD_PROPERTIES_[ sizeof(AST*) * 3 ];
			CodeBody       Body;
			char 	       _PAD_PROPERTIES_2_[ sizeof(AST*) ];
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	ModuleFlag             ModuleFlags;
	char 			       _PAD_UNUSED_[ sizeof(u32) ];
};
static_assert( sizeof(AST_Union) == sizeof(AST), "ERROR: AST_Union is not the same size as AST");

struct AST_Using
{
	union {
		char                _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment     InlineCmt;
			CodeAttributes  Attributes;
			char 	        _PAD_SPECS_     [ sizeof(AST*) ];
			CodeTypename    UnderlyingType;
			char 	        _PAD_PROPERTIES_[ sizeof(AST*) * 3 ];
		};
	};
	StrCached            Name;
	Code                    Prev;
	Code                    Next;
	Token*                  Tok;
	Code                    Parent;
	CodeType                Type;
	ModuleFlag              ModuleFlags;
	char 			        _PAD_UNUSED_[ sizeof(u32) ];
};
static_assert( sizeof(AST_Using) == sizeof(AST), "ERROR: AST_Using is not the same size as AST");

struct AST_Var
{
	union {
		char               _PAD_[ sizeof(Specifier) * AST_ArrSpecs_Cap + sizeof(AST*) ];
		struct
		{
			CodeComment    InlineCmt;
			CodeAttributes Attributes;
			CodeSpecifiers Specs;
			CodeTypename   ValueType;
			Code           BitfieldSize;
			Code           Value;
			CodeVar		   NextVar;
		};
	};
	StrCached           Name;
	Code                   Prev;
	Code                   Next;
	Token*                 Tok;
	Code                   Parent;
	CodeType               Type;
	ModuleFlag             ModuleFlags;
	s32                    VarParenthesizedInit;
};
static_assert( sizeof(AST_Var) == sizeof(AST), "ERROR: AST_Var is not the same size as AST");

#pragma endregion AST Types

#pragma endregion AST

#pragma region Gen Interface
/*
 /      \                       |      \          |  \                      /      \
|  ▓▓▓▓▓▓\ ______  _______       \▓▓▓▓▓▓_______  _| ▓▓_    ______   ______ |  ▓▓▓▓▓▓\ ______   _______  ______
| ▓▓ __\▓▓/      \|       \       | ▓▓ |       \|   ▓▓ \  /      \ /      \| ▓▓_  \▓▓|      \ /       \/      \
| ▓▓|    \  ▓▓▓▓▓▓\ ▓▓▓▓▓▓▓\      | ▓▓ | ▓▓▓▓▓▓▓\\▓▓▓▓▓▓ |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\ ▓▓ \     \▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\
| ▓▓ \▓▓▓▓ ▓▓    ▓▓ ▓▓  | ▓▓      | ▓▓ | ▓▓  | ▓▓ | ▓▓ __| ▓▓    ▓▓ ▓▓   \▓▓ ▓▓▓▓    /      ▓▓ ▓▓     | ▓▓    ▓▓
| ▓▓__| ▓▓ ▓▓▓▓▓▓▓▓ ▓▓  | ▓▓     _| ▓▓_| ▓▓  | ▓▓ | ▓▓|  \ ▓▓▓▓▓▓▓▓ ▓▓     | ▓▓     |  ▓▓▓▓▓▓▓ ▓▓_____| ▓▓▓▓▓▓▓▓
 \▓▓    ▓▓\▓▓     \ ▓▓  | ▓▓    |   ▓▓ \ ▓▓  | ▓▓  \▓▓  ▓▓\▓▓     \ ▓▓     | ▓▓      \▓▓    ▓▓\▓▓     \\▓▓     \
  \▓▓▓▓▓▓  \▓▓▓▓▓▓▓\▓▓   \▓▓     \▓▓▓▓▓▓\▓▓   \▓▓   \▓▓▓▓  \▓▓▓▓▓▓▓\▓▓      \▓▓       \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓
*/

#if 0
enum LogLevel : u32
{
	Info,
	Warning,
	Panic,
};

struct LogEntry
{
	Str   msg;
	u32   line_num;
	void* data;
};

typedef void LoggerCallback(LogEntry entry);
#endif

// Note(Ed): This is subject to heavily change 
// with upcoming changes to the library's fallback (default) allocations strategy;
// and major changes to lexer/parser context usage.
struct Context
{
// User Configuration

// Persistent Data Allocation
	AllocatorInfo Allocator_DyanmicContainers; // By default will use a genral slab allocator (TODO(Ed): Currently does not)
	AllocatorInfo Allocator_Pool;              // By default will use the growing vmem reserve (TODO(Ed): Currently does not)
	AllocatorInfo Allocator_StrCache;          // By default will use a dedicated slab allocator (TODO(Ed): Currently does not)

// Temporary Allocation
	AllocatorInfo Allocator_Temp;

	// LoggerCallaback* log_callback; // TODO(Ed): Impl user logger callback as an option.

	u32 Max_CommentLineLength; // Used by def_comment
	u32 Max_StrCacheLength;    // Any cached string longer than this is always allocated again.

	u32 InitSize_BuilderBuffer;
	u32 InitSize_CodePoolsArray;
	u32 InitSize_StringArenasArray;

	u32 CodePool_NumBlocks;

	// TODO(Ed): Review these... (No longer needed if using the proper allocation strategy)
	u32 InitSize_LexArena;
	u32 SizePer_StringArena;

// TODO(Ed): Symbol Table
	// Keep track of all resolved symbols (naemspaced identifiers)

// Parser

	// Used by the lexer to persistently treat all these identifiers as preprocessor defines.
	// Populate with strings via gen::cache_str.
	// Functional defines must have format: id( ;at minimum to indicate that the define is only valid with arguments.
	Array(StrCached) PreprocessorDefines;

// Backend

	// The fallback allocator is utilized if any fo the three above allocators is not specified by the user.
	u32 InitSize_Fallback_Allocator_Bucket_Size;
	Array(Arena) Fallback_AllocatorBuckets;

	// Array(Token) LexerTokens;

	Array(Pool)  CodePools;
	Array(Arena) StringArenas;

	StringTable StrCache;

	// TODO(Ed): This needs to be just handled by a parser context

	Arena LexArena;
	StringTable  Lexer_defines;
	Array(Token) Lexer_Tokens;

	// TODO(Ed): Active parse context vs a parse result need to be separated conceptually
	ParseContext parser;
};

// Initialize the library. There first ctx initialized must exist for lifetime of other contextes that come after as its the one that
GEN_API void init(Context* ctx);

// Currently manually free's the arenas, code for checking for leaks.
// However on Windows at least, it doesn't need to occur as the OS will clean up after the process.
GEN_API void deinit(Context* ctx);

// Clears the allocations, but doesn't free the memoery, then calls init() again.
// Ease of use.
GEN_API void reset(Context* ctx);

GEN_API void set_context(Context* ctx);

// Alternative way to add a preprocess define entry for the lexer & parser to utilize 
// if the user doesn't want to use def_define
GEN_API void set_preprocess_define( Str id, b32 is_functional );

// Used internally to retrive or make string allocations.
// Strings are stored in a series of string arenas of fixed size (SizePer_StringArena)
GEN_API StrCached cache_str( Str str );

/*
	This provides a fresh Code AST.
	The gen interface use this as their method from getting a new AST object from the CodePool.
	Use this if you want to make your own API for formatting the supported Code Types.
*/
GEN_API Code make_code();

// Set these before calling gen's init() procedure.

#pragma region Upfront

GEN_API CodeAttributes def_attributes( Str content );
GEN_API CodeComment    def_comment   ( Str content );

struct Opts_def_struct {
	CodeBody       body;
	CodeTypename   parent;
	AccessSpec     parent_access;
	CodeAttributes attributes;
	CodeTypename*  interfaces;
	s32            num_interfaces;
	ModuleFlag     mflags;
};
GEN_API CodeClass def_class( Str name, Opts_def_struct opts GEN_PARAM_DEFAULT );

struct Opts_def_constructor {
	CodeParams params;
	Code      initializer_list;
	Code      body;
};
GEN_API CodeConstructor def_constructor( Opts_def_constructor opts GEN_PARAM_DEFAULT );

struct Opts_def_define {
	b32 dont_append_preprocess_defines;
};
GEN_API CodeDefine def_define( Str name, Str content, Opts_def_define opts GEN_PARAM_DEFAULT );

struct Opts_def_destructor {
	Code           body;
	CodeSpecifiers specifiers;
};
GEN_API CodeDestructor def_destructor( Opts_def_destructor opts GEN_PARAM_DEFAULT );

struct Opts_def_enum {
	CodeBody       body;
	CodeTypename   type;
	EnumT          specifier;
	CodeAttributes attributes;
	ModuleFlag     mflags;
	Code           type_macro;
};
GEN_API CodeEnum def_enum( Str name, Opts_def_enum opts GEN_PARAM_DEFAULT );

GEN_API CodeExec   def_execution  ( Str content );
GEN_API CodeExtern def_extern_link( Str name, CodeBody body );
GEN_API CodeFriend def_friend     ( Code symbol );

struct Opts_def_function {
	CodeParams      params;
	CodeTypename    ret_type;
	CodeBody        body;
	CodeSpecifiers  specs;
	CodeAttributes  attrs;
	ModuleFlag      mflags;
};
GEN_API CodeFn def_function( Str name, Opts_def_function opts GEN_PARAM_DEFAULT );

struct Opts_def_include   { b32        foreign; };
struct Opts_def_module    { ModuleFlag mflags;  };
struct Opts_def_namespace { ModuleFlag mflags;  };
GEN_API CodeInclude def_include  ( Str content,             Opts_def_include   opts GEN_PARAM_DEFAULT );
GEN_API CodeModule  def_module   ( Str name,                Opts_def_module    opts GEN_PARAM_DEFAULT );
GEN_API CodeNS      def_namespace( Str name, CodeBody body, Opts_def_namespace opts GEN_PARAM_DEFAULT );

struct Opts_def_operator {
	CodeParams      params;
	CodeTypename    ret_type;
	CodeBody        body;
	CodeSpecifiers  specifiers;
	CodeAttributes  attributes;
	ModuleFlag      mflags;
};
GEN_API CodeOperator def_operator( Operator op, Str nspace, Opts_def_operator opts GEN_PARAM_DEFAULT );

struct Opts_def_operator_cast {
	CodeBody       body;
	CodeSpecifiers specs;
};
GEN_API CodeOpCast def_operator_cast( CodeTypename type, Opts_def_operator_cast opts GEN_PARAM_DEFAULT );

struct Opts_def_param { Code value; };
GEN_API CodeParams  def_param ( CodeTypename type, Str name, Opts_def_param opts GEN_PARAM_DEFAULT );
GEN_API CodePragma def_pragma( Str directive );

GEN_API CodePreprocessCond def_preprocess_cond( EPreprocessCond type, Str content );

GEN_API CodeSpecifiers def_specifier( Specifier specifier );

GEN_API CodeStruct def_struct( Str name, Opts_def_struct opts GEN_PARAM_DEFAULT );

struct Opts_def_template { ModuleFlag mflags; };
GEN_API CodeTemplate def_template( CodeParams params, Code definition, Opts_def_template opts GEN_PARAM_DEFAULT );

struct Opts_def_type {
	ETypenameTag   type_tag;
	Code           arrayexpr;
	CodeSpecifiers specifiers;
	CodeAttributes attributes;
};
GEN_API CodeTypename def_type( Str name, Opts_def_type opts GEN_PARAM_DEFAULT );

struct Opts_def_typedef {
	CodeAttributes attributes;
	ModuleFlag     mflags;
};
GEN_API CodeTypedef def_typedef( Str name, Code type, Opts_def_typedef opts GEN_PARAM_DEFAULT );

struct Opts_def_union {
	CodeAttributes attributes;
	ModuleFlag     mflags;
};
GEN_API CodeUnion def_union( Str name, CodeBody body, Opts_def_union opts GEN_PARAM_DEFAULT );

struct Opts_def_using {
	CodeAttributes attributes;
	ModuleFlag     mflags;
};
GEN_API CodeUsing def_using( Str name, CodeTypename type, Opts_def_using opts GEN_PARAM_DEFAULT );

GEN_API CodeUsing def_using_namespace( Str name );

struct Opts_def_variable
{
	Code           value;
	CodeSpecifiers specifiers;
	CodeAttributes attributes;
	ModuleFlag     mflags;
};
GEN_API CodeVar def_variable( CodeTypename type, Str name, Opts_def_variable opts GEN_PARAM_DEFAULT );

// Constructs an empty body. Use AST::validate_body() to check if the body is was has valid entries.
GEN_API CodeBody def_body( CodeType type );

// There are two options for defining a struct body, either varadically provided with the args macro to auto-deduce the arg num,
/// or provide as an array of Code objects.

GEN_API CodeBody       def_class_body      ( s32 num, ... );
GEN_API CodeBody       def_class_body      ( s32 num, Code* codes );
GEN_API CodeBody       def_enum_body       ( s32 num, ... );
GEN_API CodeBody       def_enum_body       ( s32 num, Code* codes );
GEN_API CodeBody       def_export_body     ( s32 num, ... );
GEN_API CodeBody       def_export_body     ( s32 num, Code* codes);
GEN_API CodeBody       def_extern_link_body( s32 num, ... );
GEN_API CodeBody       def_extern_link_body( s32 num, Code* codes );
GEN_API CodeBody       def_function_body   ( s32 num, ... );
GEN_API CodeBody       def_function_body   ( s32 num, Code* codes );
GEN_API CodeBody       def_global_body     ( s32 num, ... );
GEN_API CodeBody       def_global_body     ( s32 num, Code* codes );
GEN_API CodeBody       def_namespace_body  ( s32 num, ... );
GEN_API CodeBody       def_namespace_body  ( s32 num, Code* codes );
GEN_API CodeParams     def_params          ( s32 num, ... );
GEN_API CodeParams     def_params          ( s32 num, CodeParams* params );
GEN_API CodeSpecifiers def_specifiers      ( s32 num, ... );
GEN_API CodeSpecifiers def_specifiers      ( s32 num, Specifier* specs );
GEN_API CodeBody       def_struct_body     ( s32 num, ... );
GEN_API CodeBody       def_struct_body     ( s32 num, Code* codes );
GEN_API CodeBody       def_union_body      ( s32 num, ... );
GEN_API CodeBody       def_union_body      ( s32 num, Code* codes );

#pragma endregion Upfront

#pragma region Parsing

// TODO(Ed) : Implmeent the new parser API design.

#if 0
struct StackNode
{
	StackNode* Prev;

	Token Start;
	Token Name;       // The name of the AST node (if parsed)
	Str  FailedProc; // The name of the procedure that failed
};
// Stack nodes are allocated the error's allocator

struct Error
{
	StrBuilder     message;
	StackNode* context_stack;
};

struct ParseInfo
{
	Arena FileMem;
	Arena TokMem;
	Arena CodeMem;

	FileContents FileContent;
	Array<Token> Tokens;
	Array<Error> Errors;
	// Errors are allocated to a dedicated general arena.
};

CodeBody parse_file( Str path );
#endif

GEN_API CodeClass       parse_class        ( Str class_def       );
GEN_API CodeConstructor parse_constructor  ( Str constructor_def );
GEN_API CodeDestructor  parse_destructor   ( Str destructor_def  );
GEN_API CodeEnum        parse_enum         ( Str enum_def        );
GEN_API CodeBody        parse_export_body  ( Str export_def      );
GEN_API CodeExtern      parse_extern_link  ( Str exten_link_def  );
GEN_API CodeFriend      parse_friend       ( Str friend_def      );
GEN_API CodeFn          parse_function     ( Str fn_def          );
GEN_API CodeBody        parse_global_body  ( Str body_def        );
GEN_API CodeNS          parse_namespace    ( Str namespace_def   );
GEN_API CodeOperator    parse_operator     ( Str operator_def    );
GEN_API CodeOpCast      parse_operator_cast( Str operator_def    );
GEN_API CodeStruct      parse_struct       ( Str struct_def      );
GEN_API CodeTemplate    parse_template     ( Str template_def    );
GEN_API CodeTypename    parse_type         ( Str type_def        );
GEN_API CodeTypedef     parse_typedef      ( Str typedef_def     );
GEN_API CodeUnion       parse_union        ( Str union_def       );
GEN_API CodeUsing       parse_using        ( Str using_def       );
GEN_API CodeVar         parse_variable     ( Str var_def         );

#pragma endregion Parsing

#pragma region Untyped text

GEN_API ssize token_fmt_va( char* buf, usize buf_size, s32 num_tokens, va_list va );
//! Do not use directly. Use the token_fmt macro instead.
GEN_API Str token_fmt_impl( ssize, ... );

GEN_API Code untyped_str( Str content);
GEN_API Code untyped_fmt      ( char const* fmt, ... );
GEN_API Code untyped_token_fmt( s32 num_tokens, char const* fmt, ... );

#pragma endregion Untyped text

#pragma region Macros

#ifndef gen_main
#define gen_main main
#endif

#ifndef name
//	Convienence for defining any name used with the gen api.
//  Lets you provide the length and string literal to the functions without the need for the DSL.
#define name( Id_ )   { stringize(Id_), sizeof(stringize( Id_ )) - 1 }
#endif

#ifndef code
//  Same as name just used to indicate intention of literal for code instead of names.
#define code( ... ) { stringize( __VA_ARGS__ ), sizeof(stringize(__VA_ARGS__)) - 1 }
#endif

#ifndef args
// Provides the number of arguments while passing args inplace.
#define args( ... ) num_args( __VA_ARGS__ ), __VA_ARGS__
#endif

#ifndef code_str
// Just wrappers over common untyped code definition constructions.
#define code_str( ... ) GEN_NS untyped_str( code( __VA_ARGS__ ) )
#endif

#ifndef code_fmt
#define code_fmt( ... ) GEN_NS untyped_str( token_fmt( __VA_ARGS__ ) )
#endif

#ifndef parse_fmt
#define parse_fmt( type, ... ) GEN_NS parse_##type( token_fmt( __VA_ARGS__ ) )
#endif

#ifndef token_fmt
/*
Takes a format string (char const*) and a list of tokens (Str) and returns a Str of the formatted string.
Tokens are provided in '<'identifier'>' format where '<' '>' are just angle brackets (you can change it in token_fmt_va)
---------------------------------------------------------
	Example - A string with:
		typedef <type> <name> <name>;
	Will have a token_fmt arguments populated with:
		"type", str_for_type,
		"name", str_for_name,
	and:
		stringize( typedef <type> <name> <name>; )
-----------------------------------------------------------
So the full call for this example would be:
	token_fmt(
		"type", str_for_type
	,	"name", str_for_name
	,	stringize(
		typedef <type> <name> <name>
	));
!----------------------------------------------------------
! Note: token_fmt_va is whitespace sensitive for the tokens.
! This can be alleviated by skipping whitespace between brackets but it was choosen to not have that implementation by default.
*/
#define token_fmt( ... ) GEN_NS token_fmt_impl( (num_args( __VA_ARGS__ ) + 1) / 2, __VA_ARGS__ )
#endif

#pragma endregion Macros

#pragma endregion Gen Interface

#pragma region Inlines

#pragma region Code
inline
void code_append( Code self, Code other )
{
	GEN_ASSERT(self);
	GEN_ASSERT(other);
	GEN_ASSERT_MSG(self != other, "Attempted to recursively append Code AST to itself.");

	if ( other->Parent != nullptr )
		other = code_duplicate(other);

	other->Parent = self;

	if ( self->Front == nullptr )
	{
		self->Front = other;
		self->Back  = other;

		self->NumEntries++;
		return;
	}

	Code
	Current       = self->Back;
	Current->Next = other;
	other->Prev   = Current;
	self->Back    = other;
	self->NumEntries++;
}
inline
bool code_is_body(Code self)
{
	GEN_ASSERT(self);
	switch (self->Type)
	{
		case CT_Enum_Body:
		case CT_Class_Body:
		case CT_Union_Body:
		case CT_Export_Body:
		case CT_Global_Body:
		case CT_Struct_Body:
		case CT_Function_Body:
		case CT_Namespace_Body:
		case CT_Extern_Linkage_Body:
			return true;
	}
	return false;
}
inline
Code* code_entry( Code self, u32 idx )
{
	GEN_ASSERT(self != nullptr);
	Code* current = & self->Front;
	while ( idx >= 0 && current != nullptr )
	{
		if ( idx == 0 )
			return rcast( Code*, current);

		current = & ( * current )->Next;
		idx--;
	}

	return rcast( Code*, current);
}
forceinline
bool code_is_valid(Code self)
{
	GEN_ASSERT(self);
	return self != nullptr && self->Type != CT_Invalid;
}
forceinline
bool code_has_entries(AST* self)
{
	GEN_ASSERT(self);
	return self->NumEntries > 0;
}
forceinline
void code_set_global(Code self)
{
	if ( self == nullptr )
	{
		log_failure("Code::set_global: Cannot set code as global, AST is null!");
		return;
	}

	self->Parent = Code_Global;
}
#if GEN_COMPILER_CPP
forceinline
Code& Code::operator ++()
{
	if ( ast )
		ast = ast->Next.ast;

	return * this;
}
#endif
forceinline
Str code_type_str(Code self)
{
	GEN_ASSERT(self != nullptr);
	return codetype_to_str( self->Type );
}
#pragma endregion Code

#pragma region CodeBody
inline
void body_append( CodeBody self, Code other )
{
	GEN_ASSERT(self);
	GEN_ASSERT(other);

	if (code_is_body(other)) {
		body_append_body( self, cast(CodeBody, other) );
		return;
	}

	code_append( cast(Code, self), other );
}
inline
void body_append_body( CodeBody self, CodeBody body )
{
	GEN_ASSERT(self);
	GEN_ASSERT(body);
	GEN_ASSERT_MSG(self != body, "Attempted to append body to itself.");

	for ( Code entry = begin_CodeBody(body); entry != end_CodeBody(body); entry = next_CodeBody(body, entry) ) {
		body_append( self, entry );
	}
}
inline
Code begin_CodeBody( CodeBody body) {
	GEN_ASSERT(body);
	if ( body != nullptr )
		return body->Front;

	return NullCode;
}
forceinline
Code end_CodeBody(CodeBody body ){
	GEN_ASSERT(body);
	return body->Back->Next;
}
inline
Code next_CodeBody(CodeBody body, Code entry) {
	GEN_ASSERT(body);
	GEN_ASSERT(entry);
	return entry->Next;
}
#pragma endregion CodeBody

#pragma region CodeClass
inline
void class_add_interface( CodeClass self, CodeTypename type )
{
	GEN_ASSERT(self);
	GEN_ASSERT(type);
	CodeTypename possible_slot = self->ParentType;
	if ( possible_slot != nullptr )
	{
		// Were adding an interface to parent type, so we need to make sure the parent type is public.
		self->ParentAccess = AccessSpec_Public;
		// If your planning on adding a proper parent,
		// then you'll need to move this over to ParentType->next and update ParentAccess accordingly.
	}

	while ( possible_slot != nullptr )
	{
		possible_slot = cast(CodeTypename, possible_slot->Next);
	}

	possible_slot = type;
}
#pragma endregion CodeClass

#pragma region CodeParams
inline
void params_append( CodeParams appendee, CodeParams other )
{
	GEN_ASSERT(appendee);
	GEN_ASSERT(other);
	GEN_ASSERT_MSG(appendee != other, "Attempted to append parameter to itself.");
	Code self  = cast(Code, appendee);
	Code entry = cast(Code, other);

	if ( entry->Parent != nullptr )
		entry = code_duplicate( entry );

	entry->Parent = self;

	if ( self->Last == nullptr )
	{
		self->Last = entry;
		self->Next = entry;
		self->NumEntries++;
		return;
	}

	self->Last->Next = entry;
	self->Last       = entry;
	self->NumEntries++;
}
inline
CodeParams params_get(CodeParams self, s32 idx )
{
	GEN_ASSERT(self);
	CodeParams param = self;
	do
	{
		if ( ++ param != nullptr )
			return NullCode;

		param = cast(CodeParams, cast(Code, param)->Next);
	}
	while ( --idx );

	return param;
}
forceinline
bool params_has_entries(CodeParams self)
{
	GEN_ASSERT(self);
	return self->NumEntries > 0;
}
#if GEN_COMPILER_CPP
forceinline
CodeParams& CodeParams::operator ++()
{
	* this = ast->Next;
	return * this;
}
#endif
forceinline
CodeParams begin_CodeParams(CodeParams params)
{
	if ( params != nullptr )
		return params;

	return NullCode;
}
forceinline
CodeParams end_CodeParams(CodeParams params)
{
	// return { (AST_Params*) rcast( AST*, ast)->Last };
	return NullCode;
}
forceinline
CodeParams next_CodeParams(CodeParams params, CodeParams param_iter)
{
	GEN_ASSERT(param_iter);
	return param_iter->Next;
}
#pragma endregion CodeParams

#pragma region CodeSpecifiers
inline
bool specifiers_append(CodeSpecifiers self, Specifier spec )
{
	if ( self == nullptr )
	{
		log_failure("CodeSpecifiers: Attempted to append to a null specifiers AST!");
		return false;
	}
	if ( self->NumEntries == AST_ArrSpecs_Cap )
	{
		log_failure("CodeSpecifiers: Attempted to append over %d specifiers to a specifiers AST!", AST_ArrSpecs_Cap );
		return false;
	}

	self->ArrSpecs[ self->NumEntries ] = spec;
	self->NumEntries++;
	return true;
}
inline
s32 specifiers_has(CodeSpecifiers self, Specifier spec)
{
	GEN_ASSERT(self != nullptr);
	for ( s32 idx = 0; idx < self->NumEntries; idx++ ) {
		if ( self->ArrSpecs[ idx ] == spec )
			return idx;
	}
	return -1;
}
inline
s32 specifiers_remove( CodeSpecifiers self, Specifier to_remove )
{
	if ( self == nullptr )
	{
		log_failure("CodeSpecifiers: Attempted to append to a null specifiers AST!");
		return -1;
	}
	if ( self->NumEntries == AST_ArrSpecs_Cap )
	{
		log_failure("CodeSpecifiers: Attempted to append over %d specifiers to a specifiers AST!", AST_ArrSpecs_Cap );
		return -1;
	}

	s32 result = -1;

	s32 curr = 0;
	s32 next = 0;
	for(; next < self->NumEntries; ++ curr, ++ next)
	{
		Specifier spec = self->ArrSpecs[next];
		if (spec == to_remove)
		{
			result = next;

			next ++;
			if (next >= self->NumEntries)
				break;

			spec = self->ArrSpecs[next];
		}

		self->ArrSpecs[ curr ] = spec;
	}

	if (result > -1) {
		self->NumEntries --;
	}
	return result;
}
forceinline
Specifier* begin_CodeSpecifiers(CodeSpecifiers self)
{
	if ( self != nullptr )
		return & self->ArrSpecs[0];

	return nullptr;
}
forceinline
Specifier* end_CodeSpecifiers(CodeSpecifiers self)
{
	return self->ArrSpecs + self->NumEntries;
}
forceinline
Specifier* next_CodeSpecifiers(CodeSpecifiers self, Specifier* spec_iter)
{
	return spec_iter + 1;
}
#pragma endregion CodeSpecifiers

#pragma region CodeStruct
inline
void struct_add_interface(CodeStruct self, CodeTypename type )
{
	CodeTypename possible_slot = self->ParentType;
	if ( possible_slot != nullptr )
	{
		// Were adding an interface to parent type, so we need to make sure the parent type is public.
		self->ParentAccess = AccessSpec_Public;
		// If your planning on adding a proper parent,
		// then you'll need to move this over to ParentType->next and update ParentAccess accordingly.
	}

	while ( possible_slot != nullptr )
	{
		possible_slot = cast(CodeTypename, possible_slot->Next);
	}

	possible_slot = type;
}
#pragma endregion Code

#pragma region Interface
inline
CodeBody def_body( CodeType type )
{
	switch ( type )
	{
		case CT_Class_Body:
		case CT_Enum_Body:
		case CT_Export_Body:
		case CT_Extern_Linkage:
		case CT_Function_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
			break;

		default:
			log_failure( "def_body: Invalid type %s", codetype_to_str(type).Ptr );
			return (CodeBody)Code_Invalid;
	}

	Code
	result       = make_code();
	result->Type = type;
	return (CodeBody)result;
}

inline
Str token_fmt_impl( ssize num, ... )
{
	local_persist thread_local
	char buf[GEN_PRINTF_MAXLEN] = { 0 };
	mem_set( buf, 0, GEN_PRINTF_MAXLEN );

	va_list va;
	va_start(va, num );
	ssize result = token_fmt_va(buf, GEN_PRINTF_MAXLEN, num, va);
	va_end(va);

	Str str = { buf, result };
	return str;
}
#pragma endregion Interface
#pragma region generated code inline implementation

inline Code& Code::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline Code::operator bool()
{
	return ast != nullptr;
}

inline CodeBody& CodeBody::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeBody::operator bool()
{
	return ast != nullptr;
}

inline CodeAttributes& CodeAttributes::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeAttributes::operator bool()
{
	return ast != nullptr;
}

inline CodeAttributes::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Attributes* CodeAttributes::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeComment& CodeComment::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeComment::operator bool()
{
	return ast != nullptr;
}

inline CodeComment::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Comment* CodeComment::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeConstructor& CodeConstructor::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeConstructor::operator bool()
{
	return ast != nullptr;
}

inline CodeConstructor::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Constructor* CodeConstructor::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeClass& CodeClass::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeClass::operator bool()
{
	return ast != nullptr;
}

inline CodeDefine& CodeDefine::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeDefine::operator bool()
{
	return ast != nullptr;
}

inline CodeDefine::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Define* CodeDefine::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeDestructor& CodeDestructor::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeDestructor::operator bool()
{
	return ast != nullptr;
}

inline CodeDestructor::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Destructor* CodeDestructor::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeEnum& CodeEnum::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeEnum::operator bool()
{
	return ast != nullptr;
}

inline CodeEnum::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Enum* CodeEnum::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeExec& CodeExec::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeExec::operator bool()
{
	return ast != nullptr;
}

inline CodeExec::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Exec* CodeExec::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeExtern& CodeExtern::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeExtern::operator bool()
{
	return ast != nullptr;
}

inline CodeExtern::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Extern* CodeExtern::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeFriend& CodeFriend::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeFriend::operator bool()
{
	return ast != nullptr;
}

inline CodeFriend::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Friend* CodeFriend::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeFn& CodeFn::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeFn::operator bool()
{
	return ast != nullptr;
}

inline CodeFn::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Fn* CodeFn::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeInclude& CodeInclude::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeInclude::operator bool()
{
	return ast != nullptr;
}

inline CodeInclude::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Include* CodeInclude::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeModule& CodeModule::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeModule::operator bool()
{
	return ast != nullptr;
}

inline CodeModule::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Module* CodeModule::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeNS& CodeNS::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeNS::operator bool()
{
	return ast != nullptr;
}

inline CodeNS::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_NS* CodeNS::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeOperator& CodeOperator::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeOperator::operator bool()
{
	return ast != nullptr;
}

inline CodeOperator::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Operator* CodeOperator::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeOpCast& CodeOpCast::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeOpCast::operator bool()
{
	return ast != nullptr;
}

inline CodeOpCast::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_OpCast* CodeOpCast::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeParams& CodeParams::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeParams::operator bool()
{
	return ast != nullptr;
}

inline CodePragma& CodePragma::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodePragma::operator bool()
{
	return ast != nullptr;
}

inline CodePragma::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Pragma* CodePragma::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodePreprocessCond& CodePreprocessCond::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodePreprocessCond::operator bool()
{
	return ast != nullptr;
}

inline CodePreprocessCond::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_PreprocessCond* CodePreprocessCond::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeSpecifiers& CodeSpecifiers::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeSpecifiers::operator bool()
{
	return ast != nullptr;
}

inline CodeStruct& CodeStruct::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeStruct::operator bool()
{
	return ast != nullptr;
}

inline CodeTemplate& CodeTemplate::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeTemplate::operator bool()
{
	return ast != nullptr;
}

inline CodeTemplate::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Template* CodeTemplate::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeTypename& CodeTypename::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeTypename::operator bool()
{
	return ast != nullptr;
}

inline CodeTypename::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Typename* CodeTypename::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeTypedef& CodeTypedef::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeTypedef::operator bool()
{
	return ast != nullptr;
}

inline CodeTypedef::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Typedef* CodeTypedef::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeUnion& CodeUnion::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeUnion::operator bool()
{
	return ast != nullptr;
}

inline CodeUnion::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Union* CodeUnion::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeUsing& CodeUsing::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeUsing::operator bool()
{
	return ast != nullptr;
}

inline CodeUsing::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Using* CodeUsing::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

inline CodeVar& CodeVar::operator=( Code other )
{
	if ( other.ast != nullptr && other->Parent != nullptr )
	{
		ast         = rcast( decltype( ast ), code_duplicate( other ).ast );
		ast->Parent = { nullptr };
	}
	ast = rcast( decltype( ast ), other.ast );
	return *this;
}

inline CodeVar::operator bool()
{
	return ast != nullptr;
}

inline CodeVar::operator Code()
{
	return *rcast( Code*, this );
}

inline AST_Var* CodeVar::operator->()
{
	if ( ast == nullptr )
	{
		log_failure( "Attempt to dereference a nullptr!\n" );
		return nullptr;
	}
	return ast;
}

#pragma endregion generated code inline implementation

#pragma region generated AST/Code cast implementation
GEN_OPTIMIZE_MAPPINGS_BEGIN

forceinline Code::operator CodeBody() const
{
	return { (AST_Body*)ast };
}

forceinline Code::operator CodeAttributes() const
{
	return { (AST_Attributes*)ast };
}

forceinline Code::operator CodeComment() const
{
	return { (AST_Comment*)ast };
}

forceinline Code::operator CodeConstructor() const
{
	return { (AST_Constructor*)ast };
}

forceinline Code::operator CodeClass() const
{
	return { (AST_Class*)ast };
}

forceinline Code::operator CodeDefine() const
{
	return { (AST_Define*)ast };
}

forceinline Code::operator CodeDestructor() const
{
	return { (AST_Destructor*)ast };
}

forceinline Code::operator CodeEnum() const
{
	return { (AST_Enum*)ast };
}

forceinline Code::operator CodeExec() const
{
	return { (AST_Exec*)ast };
}

forceinline Code::operator CodeExtern() const
{
	return { (AST_Extern*)ast };
}

forceinline Code::operator CodeFriend() const
{
	return { (AST_Friend*)ast };
}

forceinline Code::operator CodeFn() const
{
	return { (AST_Fn*)ast };
}

forceinline Code::operator CodeInclude() const
{
	return { (AST_Include*)ast };
}

forceinline Code::operator CodeModule() const
{
	return { (AST_Module*)ast };
}

forceinline Code::operator CodeNS() const
{
	return { (AST_NS*)ast };
}

forceinline Code::operator CodeOperator() const
{
	return { (AST_Operator*)ast };
}

forceinline Code::operator CodeOpCast() const
{
	return { (AST_OpCast*)ast };
}

forceinline Code::operator CodeParams() const
{
	return { (AST_Params*)ast };
}

forceinline Code::operator CodePragma() const
{
	return { (AST_Pragma*)ast };
}

forceinline Code::operator CodePreprocessCond() const
{
	return { (AST_PreprocessCond*)ast };
}

forceinline Code::operator CodeSpecifiers() const
{
	return { (AST_Specifiers*)ast };
}

forceinline Code::operator CodeStruct() const
{
	return { (AST_Struct*)ast };
}

forceinline Code::operator CodeTemplate() const
{
	return { (AST_Template*)ast };
}

forceinline Code::operator CodeTypename() const
{
	return { (AST_Typename*)ast };
}

forceinline Code::operator CodeTypedef() const
{
	return { (AST_Typedef*)ast };
}

forceinline Code::operator CodeUnion() const
{
	return { (AST_Union*)ast };
}

forceinline Code::operator CodeUsing() const
{
	return { (AST_Using*)ast };
}

forceinline Code::operator CodeVar() const
{
	return { (AST_Var*)ast };
}

GEN_OPITMIZE_MAPPINGS_END
#pragma endregion generated AST / Code cast implementation

#pragma endregion Inlines

#pragma region Constants

extern Str enum_underlying_sig;

extern Code access_public;
extern Code access_protected;
extern Code access_private;

extern CodeAttributes attrib_api_export;
extern CodeAttributes attrib_api_import;

extern Code module_global_fragment;
extern Code module_private_fragment;

extern Code fmt_newline;

extern CodePragma pragma_once;

extern CodeParams param_varadic;

extern CodePreprocessCond preprocess_else;
extern CodePreprocessCond preprocess_endif;

extern CodeSpecifiers spec_const;
extern CodeSpecifiers spec_consteval;
extern CodeSpecifiers spec_constexpr;
extern CodeSpecifiers spec_constinit;
extern CodeSpecifiers spec_extern_linkage;
extern CodeSpecifiers spec_final;
extern CodeSpecifiers spec_forceinline;
extern CodeSpecifiers spec_global;
extern CodeSpecifiers spec_inline;
extern CodeSpecifiers spec_internal_linkage;
extern CodeSpecifiers spec_local_persist;
extern CodeSpecifiers spec_mutable;
extern CodeSpecifiers spec_neverinline;
extern CodeSpecifiers spec_noexcept;
extern CodeSpecifiers spec_override;
extern CodeSpecifiers spec_ptr;
extern CodeSpecifiers spec_pure;
extern CodeSpecifiers spec_ref;
extern CodeSpecifiers spec_register;
extern CodeSpecifiers spec_rvalue;
extern CodeSpecifiers spec_static_member;
extern CodeSpecifiers spec_thread_local;
extern CodeSpecifiers spec_virtual;
extern CodeSpecifiers spec_volatile;

extern CodeTypename t_empty; // Used with varaidc parameters. (Exposing just in case its useful for another circumstance)
extern CodeTypename t_auto;
extern CodeTypename t_void;
extern CodeTypename t_int;
extern CodeTypename t_bool;
extern CodeTypename t_char;
extern CodeTypename t_wchar_t;
extern CodeTypename t_class;
extern CodeTypename t_typename;

#ifdef GEN_DEFINE_LIBRARY_CODE_CONSTANTS
	// Predefined typename codes. Are set to readonly and are setup during gen::init()
	extern Context* _ctx;

	extern CodeTypename t_b32;

	extern CodeTypename t_s8;
	extern CodeTypename t_s16;
	extern CodeTypename t_s32;
	extern CodeTypename t_s64;

	extern CodeTypename t_u8;
	extern CodeTypename t_u16;
	extern CodeTypename t_u32;
	extern CodeTypename t_u64;

	extern CodeTypename t_ssize;
	extern CodeTypename t_usize;

	extern CodeTypename t_f32;
	extern CodeTypename t_f64;
#endif

#pragma endregion Constants

#pragma region Builder

struct Builder;
typedef struct Builder Builder;

Builder builder_open        ( char const* path );
void    builder_pad_lines   ( Builder* builder, s32 num );
void    builder_print       ( Builder* builder, Code code );
void    builder_print_fmt_va( Builder* builder, char const* fmt, va_list va );
void    builder_print_fmt   ( Builder* builder, char const* fmt, ... ) {
	va_list va;
	va_start( va, fmt );
	builder_print_fmt_va( builder, fmt, va );
	va_end( va );
}
void builder_write( Builder* builder );

struct Builder
{
	FileInfo File;
	StrBuilder   Buffer;

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
	forceinline static Builder open( char const* path ) { return builder_open(path); }

	forceinline void pad_lines( s32 num ) { return builder_pad_lines(this, num); }

	forceinline void print( Code code )                { return builder_print(this, code); }
	forceinline void print_fmt( char const* fmt, ... ) {
		va_list va;
		va_start( va, fmt );
		builder_print_fmt_va( this, fmt, va );
		va_end( va );
	}

	forceinline void write() { return builder_write(this); }
#endif
};

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
void    builder_pad_lines( Builder& builder, s32 num )    { return builder_pad_lines(& builder, num); }
void    builder_print    ( Builder& builder, Code code )  { return builder_print(& builder, code); }
void    builder_write    ( Builder& builder )             { return builder_write(& builder ); }
void    builder_print_fmt( Builder& builder, char const* fmt, ...) {
	va_list va;
	va_start( va, fmt );
	builder_print_fmt_va( & builder, fmt, va );
	va_end( va );
}
#endif

#pragma endregion Builder

#pragma region Scanner

// This is a simple file reader that reads the entire file into memory.
// It has an extra option to skip the first few lines for undesired includes.
// This is done so that includes can be kept in dependency and component files so that intellisense works.
Code scan_file( char const* path );

CodeBody parse_file( const char* path );

// The follow is basic support for light csv parsing (use it as an example)
// Make something robust if its more serious.

typedef struct CSV_Column CSV_Column;
struct CSV_Column {
	CSV_Object      ADT;
	Array(ADT_Node) Content;
};

typedef struct CSV_Columns2 CSV_Columns2;
struct CSV_Columns2 {
	CSV_Object      ADT;
	Array(ADT_Node) Col_1;
	Array(ADT_Node) Col_2;
};

CSV_Column parse_csv_one_column(AllocatorInfo allocator, char const* path);
CSV_Columns2 parse_csv_two_columns(AllocatorInfo allocator, char const* path);

#pragma endregion Scanner

GEN_NS_END

#pragma region GENCPP IMPLEMENTATION GUARD
#if defined(GEN_IMPLEMENTATION) && ! defined(GEN_IMPLEMENTED)
#	define GEN_IMPLEMENTED


//! If its desired to roll your own dependencies, define GEN_ROLL_OWN_DEPENDENCIES before including this file.
// Dependencies are derived from the c-zpl library: https://github.com/zpl-c/zpl
#ifndef GEN_ROLL_OWN_DEPENDENCIES

#pragma region Macros and Includes

#	include <stdio.h>
// NOTE: Ensure we use standard methods for these calls if we use GEN_PICO
#	if ! defined( GEN_PICO_CUSTOM_ROUTINES )
#		if ! defined( GEN_MODULE_CORE )
#			define _strlen                   strlen
#			define _printf_err( fmt, ... )   fprintf( stderr, fmt, __VA_ARGS__ )
#			define _printf_err_va( fmt, va ) vfprintf( stderr, fmt, va )
#		else
#			define _strlen                   c_str_len
#			define _printf_err( fmt, ... )   c_str_fmt_out_err( fmt, __VA_ARGS__ )
#			define _printf_err_va( fmt, va ) c_str_fmt_out_err_va( fmt, va )
#		endif
#	endif
#
#	include <errno.h>
#
#	if defined( GEN_SYSTEM_UNIX ) || defined( GEN_SYSTEM_MACOS )
#		include <unistd.h>
#	elif defined( GEN_SYSTEM_WINDOWS )
#		if ! defined( GEN_NO_WINDOWS_H )
#			ifndef WIN32_LEAN_AND_MEAN
#				ifndef NOMINMAX
#					define NOMINMAX
#				endif
#
#				define WIN32_LEAN_AND_MEAN
#				define WIN32_MEAN_AND_LEAN
#				define VC_EXTRALEAN
#			endif
#			include <windows.h>
#			undef NOMINMAX
#			undef WIN32_LEAN_AND_MEAN
#			undef WIN32_MEAN_AND_LEAN
#			undef VC_EXTRALEAN
#		endif
#	endif

#include <sys/stat.h>

#ifdef GEN_SYSTEM_MACOS
#	include <copyfile.h>
#endif

#ifdef GEN_SYSTEM_CYGWIN
#	include <windows.h>
#endif

#if defined( GEN_SYSTEM_WINDOWS ) && ! defined( GEN_COMPILER_GCC )
#	include <io.h>
#endif

#if defined( GEN_SYSTEM_LINUX )
#	include <sys/types.h>
#endif

#ifdef GEN_BENCHMARK
// Timing includes
#if defined( GEN_SYSTEM_MACOS ) || GEN_SYSTEM_UNIX
#	include <time.h>
#	include <sys/time.h>
#endif

#if defined( GEN_SYSTEM_MACOS )
#	include <mach/mach.h>
#	include <mach/mach_time.h>
#	include <mach/clock.h>
#endif

#if defined( GEN_SYSTEM_EMSCRIPTEN )
#	include <emscripten.h>
#endif

#if defined( GEN_SYSTEM_WINDOWS )
#	include <timezoneapi.h>
#endif
#endif

#pragma endregion Macros and Includes

GEN_NS_BEGIN

#pragma region Debug

void assert_handler( char const* condition, char const* file, char const* function, s32 line, char const* msg, ... )
{
	_printf_err( "%s - %s:(%d): Assert Failure: ", file, function, line );

	if ( condition )
		_printf_err( "`%s` \n", condition );

	if ( msg )
	{
		va_list va;
		va_start( va, msg );
		_printf_err_va( msg, va );
		va_end( va );
	}

	_printf_err( "%s", "\n" );
}

s32 assert_crash( char const* condition )
{
	GEN_PANIC( condition );
	return 0;
}

#if defined( GEN_SYSTEM_WINDOWS )
	void process_exit( u32 code )
	{
		ExitProcess( code );
	}
#else
#	include <stdlib.h>

	void process_exit( u32 code )
	{
		exit( code );
	}
#endif

#pragma endregion Debug

#pragma region String Ops

internal
ssize _scan_zpl_i64( const char* text, s32 base, s64* value )
{
	const char* text_begin = text;
	s64         result     = 0;
	b32         negative   = false;

	if ( *text == '-' )
	{
		negative = true;
		text++;
	}

	if ( base == 16 && c_str_compare_len( text, "0x", 2 ) == 0 )
		text += 2;

	for ( ;; )
	{
		s64 v;
		if ( char_is_digit( *text ) )
			v = *text - '0';
		else if ( base == 16 && char_is_hex_digit( *text ) )
			v = hex_digit_to_int( *text );
		else
			break;

		result *= base;
		result += v;
		text++;
	}

	if ( value )
	{
		if ( negative )
			result = -result;
		*value = result;
	}

	return ( text - text_begin );
}

// TODO : Are these good enough for characters?
global const char _num_to_char_table[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"@$";

s64 c_str_to_i64( const char* str, char** end_ptr, s32 base )
{
	ssize  len;
	s64 value;

	if ( ! base )
	{
		if ( ( c_str_len( str ) > 2 ) && ( c_str_compare_len( str, "0x", 2 ) == 0 ) )
			base = 16;
		else
			base = 10;
	}

	len = _scan_zpl_i64( str, base, &value );
	if ( end_ptr )
		*end_ptr = ( char* )str + len;
	return value;
}

void i64_to_str( s64 value, char* string, s32 base )
{
	char* buf      = string;
	b32   negative = false;
	u64   v;

	if ( value < 0 )
	{
		negative = true;
		value    = -value;
	}

	v = scast( u64, value);
	if ( v != 0 )
	{
		while ( v > 0 )
		{
			*buf++  = _num_to_char_table[ v % base ];
			v      /= base;
		}
	}
	else
	{
		*buf++ = '0';
	}
	if ( negative )
		*buf++ = '-';
	*buf = '\0';
	c_str_reverse( string );
}

void u64_to_str( u64 value, char* string, s32 base )
{
	char* buf = string;

	if ( value )
	{
		while ( value > 0 )
		{
			*buf++  = _num_to_char_table[ value % base ];
			value  /= base;
		}
	}
	else
	{
		*buf++ = '0';
	}
	*buf = '\0';

	c_str_reverse( string );
}

f64 c_str_to_f64( const char* str, char** end_ptr )
{
	f64 result, value, sign, scale;
	s32 frac;

	while ( char_is_space( *str ) )
	{
		str++;
	}

	sign = 1.0;
	if ( *str == '-' )
	{
		sign = -1.0;
		str++;
	}
	else if ( *str == '+' )
	{
		str++;
	}

	for ( value = 0.0; char_is_digit( *str ); str++ )
	{
		value = value * 10.0 + ( *str - '0' );
	}

	if ( *str == '.' )
	{
		f64 pow10 = 10.0;
		str++;
		while ( char_is_digit( *str ) )
		{
			value += ( *str - '0' ) / pow10;
			pow10 *= 10.0;
			str++;
		}
	}

	frac  = 0;
	scale = 1.0;
	if ( ( *str == 'e' ) || ( *str == 'E' ) )
	{
		u32 exp;

		str++;
		if ( *str == '-' )
		{
			frac = 1;
			str++;
		}
		else if ( *str == '+' )
		{
			str++;
		}

		for ( exp = 0; char_is_digit( *str ); str++ )
		{
			exp = exp * 10 + ( *str - '0' );
		}
		if ( exp > 308 )
			exp = 308;

		while ( exp >= 50 )
		{
			scale *= 1e50;
			exp   -= 50;
		}
		while ( exp >= 8 )
		{
			scale *= 1e8;
			exp   -= 8;
		}
		while ( exp > 0 )
		{
			scale *= 10.0;
			exp   -= 1;
		}
	}

	result = sign * ( frac ? ( value / scale ) : ( value * scale ) );

	if ( end_ptr )
		* end_ptr = rcast( char*, ccast(char*, str) );

	return result;
}

#pragma endregion String Ops

#pragma region Printing

enum
{
	GEN_FMT_MINUS = bit( 0 ),
	GEN_FMT_PLUS  = bit( 1 ),
	GEN_FMT_ALT   = bit( 2 ),
	GEN_FMT_SPACE = bit( 3 ),
	GEN_FMT_ZERO  = bit( 4 ),

	GEN_FMT_CHAR   = bit( 5 ),
	GEN_FMT_SHORT  = bit( 6 ),
	GEN_FMT_INT    = bit( 7 ),
	GEN_FMT_LONG   = bit( 8 ),
	GEN_FMT_LLONG  = bit( 9 ),
	GEN_FMT_SIZE   = bit( 10 ),
	GEN_FMT_INTPTR = bit( 11 ),

	GEN_FMT_UNSIGNED = bit( 12 ),
	GEN_FMT_LOWER    = bit( 13 ),
	GEN_FMT_UPPER    = bit( 14 ),
	GEN_FMT_WIDTH    = bit( 15 ),

	GEN_FMT_DONE = bit( 30 ),

	GEN_FMT_INTS = GEN_FMT_CHAR | GEN_FMT_SHORT | GEN_FMT_INT | GEN_FMT_LONG | GEN_FMT_LLONG | GEN_FMT_SIZE | GEN_FMT_INTPTR
};

typedef struct _format_info _format_info;
struct _format_info
{
	s32 base;
	s32 flags;
	s32 width;
	s32 precision;
};

internal ssize _print_string( char* text, ssize max_len, _format_info* info, char const* str )
{
	ssize res = 0, len = 0;
	ssize remaining = max_len;
	char* begin     = text;

	if ( str == NULL && max_len >= 6 )
	{
		res += c_str_copy_nulpad( text, "(null)", 6 );
		return res;
	}

	if ( info && info->precision >= 0 )
		// Made the design decision for this library that precision is the length of the string.
		len = info->precision;
	else
		len = c_str_len( str );

	if ( info && ( info->width == 0 && info->flags & GEN_FMT_WIDTH ) )
	{
		return res;
	}

	if ( info && ( info->width == 0 || info->flags & GEN_FMT_MINUS ) )
	{
		if ( info->precision > 0 )
			len = info->precision < len ? info->precision : len;
		if ( res + len > max_len )
			return res;
		res  += c_str_copy_nulpad( text, str, len );
		text += res;

		if ( info->width > res )
		{
			ssize padding = info->width - len;

			char pad = ( info->flags & GEN_FMT_ZERO ) ? '0' : ' ';
			while ( padding-- > 0 && remaining-- > 0 )
				*text++ = pad, res++;
		}
	}
	else
	{
		if ( info && ( info->width > res ) )
		{
			ssize   padding = info->width - len;
			char pad     = ( info->flags & GEN_FMT_ZERO ) ? '0' : ' ';
			while ( padding-- > 0 && remaining-- > 0 )
				*text++ = pad, res++;
		}

		if ( res + len > max_len )
			return res;
		res += c_str_copy_nulpad( text, str, len );
	}

	if ( info )
	{
		if ( info->flags & GEN_FMT_UPPER )
			c_str_to_upper( begin );
		else if ( info->flags & GEN_FMT_LOWER )
			c_str_to_lower( begin );
	}

	return res;
}

internal ssize _print_char( char* text, ssize max_len, _format_info* info, char arg )
{
	char str[ 2 ] = "";
	str[ 0 ]      = arg;
	return _print_string( text, max_len, info, str );
}

internal ssize _print_repeated_char( char* text, ssize max_len, _format_info* info, char arg )
{
	ssize  res = 0;
	s32 rem = ( info ) ? ( info->width > 0 ) ? info->width : 1 : 1;
	res     = rem;
	while ( rem-- > 0 )
		*text++ = arg;

	return res;
}

internal ssize _print_i64( char* text, ssize max_len, _format_info* info, s64 value )
{
	char num[ 130 ];
	i64_to_str( value, num, info ? info->base : 10 );
	return _print_string( text, max_len, info, num );
}

internal ssize _print_u64( char* text, ssize max_len, _format_info* info, u64 value )
{
	char num[ 130 ];
	u64_to_str( value, num, info ? info->base : 10 );
	return _print_string( text, max_len, info, num );
}

internal ssize _print_f64( char* text, ssize max_len, _format_info* info, b32 is_hexadecimal, f64 arg )
{
	// TODO: Handle exponent notation
	ssize    width, len, remaining = max_len;
	char* text_begin = text;

	if ( arg )
	{
		u64 value;
		if ( arg < 0 )
		{
			if ( remaining > 1 )
				*text = '-', remaining--;
			text++;
			arg = -arg;
		}
		else if ( info->flags & GEN_FMT_MINUS )
		{
			if ( remaining > 1 )
				*text = '+', remaining--;
			text++;
		}

		value  = scast( u64, arg);
		len    = _print_u64( text, remaining, NULL, value );
		text  += len;

		if ( len >= remaining )
			remaining = min( remaining, 1 );
		else
			remaining -= len;
		arg -= value;

		if ( info->precision < 0 )
			info->precision = 6;

		if ( ( info->flags & GEN_FMT_ALT ) || info->precision > 0 )
		{
			s64 mult = 10;
			if ( remaining > 1 )
				*text = '.', remaining--;
			text++;
			while ( info->precision-- > 0 )
			{
				value  = scast( u64, arg * mult );
				len    = _print_u64( text, remaining, NULL, value );
				text  += len;
				if ( len >= remaining )
					remaining = min( remaining, 1 );
				else
					remaining -= len;
				arg  -= scast( f64, value / mult);
				mult *= 10;
			}
		}
	}
	else
	{
		if ( remaining > 1 )
			*text = '0', remaining--;
		text++;
		if ( info->flags & GEN_FMT_ALT )
		{
			if ( remaining > 1 )
				*text = '.', remaining--;
			text++;
		}
	}

	width = info->width - ( text - text_begin );
	if ( width > 0 )
	{
		char  fill = ( info->flags & GEN_FMT_ZERO ) ? '0' : ' ';
		char* end  = text + remaining - 1;
		len        = ( text - text_begin );

		for ( len = ( text - text_begin ); len--; )
		{
			if ( ( text_begin + len + width ) < end )
				*( text_begin + len + width ) = *( text_begin + len );
		}

		len   = width;
		text += len;
		if ( len >= remaining )
			remaining = min( remaining, 1 );
		else
			remaining -= len;

		while ( len-- )
		{
			if ( text_begin + len < end )
				text_begin[ len ] = fill;
		}
	}

	return ( text - text_begin );
}

neverinline ssize c_str_fmt_va( char* text, ssize max_len, char const* fmt, va_list va )
{
	char const* text_begin = text;
	ssize          remaining  = max_len, res;

	while ( *fmt )
	{
		_format_info info = { 0 };
		ssize        len  = 0;
		info.precision    = -1;

		while ( *fmt && *fmt != '%' && remaining )
			*text++ = *fmt++;

		if ( *fmt == '%' )
		{
			do
			{
				switch ( *++fmt )
				{
					case '-' :
						{
							info.flags |= GEN_FMT_MINUS;
							break;
						}
					case '+' :
						{
							info.flags |= GEN_FMT_PLUS;
							break;
						}
					case '#' :
						{
							info.flags |= GEN_FMT_ALT;
							break;
						}
					case ' ' :
						{
							info.flags |= GEN_FMT_SPACE;
							break;
						}
					case '0' :
						{
							info.flags |= ( GEN_FMT_ZERO | GEN_FMT_WIDTH );
							break;
						}
					default :
						{
							info.flags |= GEN_FMT_DONE;
							break;
						}
				}
			} while ( ! ( info.flags & GEN_FMT_DONE ) );
		}

		// NOTE: Optional Width
		if ( *fmt == '*' )
		{
			int width = va_arg( va, int );
			if ( width < 0 )
			{
				info.flags |= GEN_FMT_MINUS;
				info.width  = -width;
			}
			else
			{
				info.width = width;
			}
			info.flags |= GEN_FMT_WIDTH;
			fmt++;
		}
		else
		{
			info.width = scast( s32, c_str_to_i64( fmt, ccast( char**, & fmt), 10 ));
			if ( info.width != 0 )
			{
				info.flags |= GEN_FMT_WIDTH;
			}
		}

		// NOTE: Optional Precision
		if ( *fmt == '.' )
		{
			fmt++;
			if ( *fmt == '*' )
			{
				info.precision = va_arg( va, int );
				fmt++;
			}
			else
			{
				info.precision = scast( s32, c_str_to_i64( fmt, ccast( char**, & fmt), 10 ));
			}
			info.flags &= ~GEN_FMT_ZERO;
		}

		switch ( *fmt++ )
		{
			case 'h' :
				if ( *fmt == 'h' )
				{    // hh => char
					info.flags |= GEN_FMT_CHAR;
					fmt++;
				}
				else
				{    // h => short
					info.flags |= GEN_FMT_SHORT;
				}
				break;

			case 'l' :
				if ( *fmt == 'l' )
				{    // ll => long long
					info.flags |= GEN_FMT_LLONG;
					fmt++;
				}
				else
				{    // l => long
					info.flags |= GEN_FMT_LONG;
				}
				break;

				break;

			case 'z' :    // NOTE: zpl_usize
				info.flags |= GEN_FMT_UNSIGNED;
				// fallthrough
			case 't' :    // NOTE: zpl_isize
				info.flags |= GEN_FMT_SIZE;
				break;

			default :
				fmt--;
				break;
		}

		switch ( *fmt )
		{
			case 'u' :
				info.flags |= GEN_FMT_UNSIGNED;
				// fallthrough
			case 'd' :
			case 'i' :
				info.base = 10;
				break;

			case 'o' :
				info.base = 8;
				break;

			case 'x' :
				info.base   = 16;
				info.flags |= ( GEN_FMT_UNSIGNED | GEN_FMT_LOWER );
				break;

			case 'X' :
				info.base   = 16;
				info.flags |= ( GEN_FMT_UNSIGNED | GEN_FMT_UPPER );
				break;

			case 'f' :
			case 'F' :
			case 'g' :
			case 'G' :
				len = _print_f64( text, remaining, &info, 0, va_arg( va, f64 ) );
				break;

			case 'a' :
			case 'A' :
				len = _print_f64( text, remaining, &info, 1, va_arg( va, f64 ) );
				break;

			case 'c' :
				len = _print_char( text, remaining, &info, scast( char, va_arg( va, int ) ));
				break;

			case 's' :
				len = _print_string( text, remaining, &info, va_arg( va, char* ) );
				break;

			case 'S':
			{
				if ( *(fmt + 1) == 'B' )
				{

					++ fmt;
					StrBuilder gen_str = { va_arg( va, char*) };

					info.precision = strbuilder_length(gen_str);
					len            = _print_string( text, remaining, &info, gen_str );
					break;
				}

				Str gen_str    = va_arg( va, Str);
				info.precision = gen_str.Len;
				len            = _print_string( text, remaining, &info, gen_str.Ptr );
			}
			break;

			case 'r' :
				len = _print_repeated_char( text, remaining, &info, va_arg( va, int ) );
				break;

			case 'p' :
				info.base   = 16;
				info.flags |= ( GEN_FMT_LOWER | GEN_FMT_UNSIGNED | GEN_FMT_ALT | GEN_FMT_INTPTR );
				break;

			case '%' :
				len = _print_char( text, remaining, &info, '%' );
				break;

			default :
				fmt--;
				break;
		}

		fmt++;

		if ( info.base != 0 )
		{
			if ( info.flags & GEN_FMT_UNSIGNED )
			{
				u64 value = 0;
				switch ( info.flags & GEN_FMT_INTS )
				{
					case GEN_FMT_CHAR :
						value = scast( u64, scast( u8, va_arg( va, int )));
						break;
					case GEN_FMT_SHORT :
						value = scast( u64, scast( u16, va_arg( va, int )));
						break;
					case GEN_FMT_LONG:
						value = scast( u64, va_arg( va, unsigned long ));
						break;
					case GEN_FMT_LLONG :
						value = scast( u64, va_arg( va, unsigned long long ));
						break;
					case GEN_FMT_SIZE :
						value = scast( u64, va_arg( va, usize ));
						break;
					case GEN_FMT_INTPTR :
						value = scast( u64, va_arg( va, uptr ));
						break;
					default :
						value = scast( u64, va_arg( va, unsigned int ));
						break;
				}

				len = _print_u64( text, remaining, &info, value );
			}
			else
			{
				s64 value = 0;
				switch ( info.flags & GEN_FMT_INTS )
				{
					case GEN_FMT_CHAR :
						value = scast( s64, scast( s8, va_arg( va, int )));
						break;
					case GEN_FMT_SHORT :
						value = scast( s64, scast( s16, va_arg( va, int )));
						break;
					case GEN_FMT_LONG :
						value = scast( s64, va_arg( va, long ));
						break;
					case GEN_FMT_LLONG :
						value = scast( s64, va_arg( va, long long ));
						break;
					case GEN_FMT_SIZE :
						value = scast( s64, va_arg( va, usize ));
						break;
					case GEN_FMT_INTPTR :
						value = scast( s64, va_arg( va, uptr ));
						break;
					default :
						value = scast( s64, va_arg( va, int ));
						break;
				}

				len = _print_i64( text, remaining, &info, value );
			}
		}

		text += len;
		if ( len >= remaining )
			remaining = min( remaining, 1 );
		else
			remaining -= len;
	}

	*text++ = '\0';
	res     = ( text - text_begin );
	return ( res >= max_len || res < 0 ) ? -1 : res;
}

char* c_str_fmt_buf_va( char const* fmt, va_list va )
{
	local_persist thread_local char buffer[ GEN_PRINTF_MAXLEN ];
	c_str_fmt_va( buffer, size_of( buffer ), fmt, va );
	return buffer;
}

char* c_str_fmt_buf( char const* fmt, ... )
{
	va_list va;
	char*   str;
	va_start( va, fmt );
	str = c_str_fmt_buf_va( fmt, va );
	va_end( va );
	return str;
}

ssize c_str_fmt_file_va( FileInfo* f, char const* fmt, va_list va )
{
	local_persist thread_local char buf[ GEN_PRINTF_MAXLEN ];
	ssize                              len = c_str_fmt_va( buf, size_of( buf ), fmt, va );
	b32                             res = file_write( f, buf, len - 1 );    // NOTE: prevent extra whitespace
	return res ? len : -1;
}

ssize c_str_fmt_file( FileInfo* f, char const* fmt, ... )
{
	ssize      res;
	va_list va;
	va_start( va, fmt );
	res = c_str_fmt_file_va( f, fmt, va );
	va_end( va );
	return res;
}

ssize c_str_fmt( char* str, ssize n, char const* fmt, ... )
{
	ssize      res;
	va_list va;
	va_start( va, fmt );
	res = c_str_fmt_va( str, n, fmt, va );
	va_end( va );
	return res;
}

ssize c_str_fmt_out_va( char const* fmt, va_list va )
{
	return c_str_fmt_file_va( file_get_standard( EFileStandard_OUTPUT ), fmt, va );
}

ssize c_str_fmt_out_err_va( char const* fmt, va_list va )
{
	return c_str_fmt_file_va( file_get_standard( EFileStandard_ERROR ), fmt, va );
}

ssize c_str_fmt_out_err( char const* fmt, ... )
{
	ssize      res;
	va_list va;
	va_start( va, fmt );
	res = c_str_fmt_out_err_va( fmt, va );
	va_end( va );
	return res;
}

#pragma endregion Printing

#pragma region Memory

void* mem_copy( void* dest, void const* source, ssize n )
{
	if ( dest == nullptr )
	{
		return nullptr;
	}

	return memcpy( dest, source, n );
}

void const* mem_find( void const* data, u8 c, ssize n )
{
	u8 const* s = rcast( u8 const*, data);
	while ( ( rcast( uptr, s) & ( sizeof( usize ) - 1 ) ) && n && *s != c )
	{
		s++;
		n--;
	}
	if ( n && *s != c )
	{
		ssize const* w;
		ssize        k = GEN__ONES * c;
		w           = rcast( ssize const*, s);
		while ( n >= size_of( ssize ) && ! GEN__HAS_ZERO( *w ^ k ) )
		{
			w++;
			n -= size_of( ssize );
		}
		s = rcast( u8 const*, w);
		while ( n && *s != c )
		{
			s++;
			n--;
		}
	}

	return n ? rcast( void const*, s ) : NULL;
}

#define GEN_HEAP_STATS_MAGIC 0xDEADC0DE

typedef struct _heap_stats _heap_stats;
struct _heap_stats
{
	u32 magic;
	ssize  used_memory;
	ssize  alloc_count;
};

global _heap_stats _heap_stats_info;

void heap_stats_init( void )
{
	zero_item( &_heap_stats_info );
	_heap_stats_info.magic = GEN_HEAP_STATS_MAGIC;
}

ssize heap_stats_used_memory( void )
{
	GEN_ASSERT_MSG( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	return _heap_stats_info.used_memory;
}

ssize heap_stats_alloc_count( void )
{
	GEN_ASSERT_MSG( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	return _heap_stats_info.alloc_count;
}

void heap_stats_check( void )
{
	GEN_ASSERT_MSG( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	GEN_ASSERT( _heap_stats_info.used_memory == 0 );
	GEN_ASSERT( _heap_stats_info.alloc_count == 0 );
}

typedef struct _heap_alloc_info _heap_alloc_info;
struct _heap_alloc_info
{
	ssize    size;
	void* physical_start;
};

void* heap_allocator_proc( void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags )
{
	void* ptr = NULL;
	// unused( allocator_data );
	// unused( old_size );
	if ( ! alignment )
		alignment = GEN_DEFAULT_MEMORY_ALIGNMENT;

#ifdef GEN_HEAP_ANALYSIS
	ssize alloc_info_size      = size_of( _heap_alloc_info );
	ssize alloc_info_remainder = ( alloc_info_size % alignment );
	ssize track_size           = max( alloc_info_size, alignment ) + alloc_info_remainder;
	switch ( type )
	{
		case EAllocation_FREE :
			{
				if ( ! old_memory )
					break;
				_heap_alloc_info* alloc_info  = rcast( _heap_alloc_info*, old_memory) - 1;
				_heap_stats_info.used_memory -= alloc_info->size;
				_heap_stats_info.alloc_count--;
				old_memory = alloc_info->physical_start;
			}
			break;
		case EAllocation_ALLOC :
			{
				size += track_size;
			}
			break;
		default :
			break;
	}
#endif

	switch ( type )
	{
#if defined( GEN_COMPILER_MSVC ) || ( defined( GEN_COMPILER_GCC ) && defined( GEN_SYSTEM_WINDOWS ) ) || ( defined( GEN_COMPILER_TINYC ) && defined( GEN_SYSTEM_WINDOWS ) )
		case EAllocation_ALLOC :
			ptr = _aligned_malloc( size, alignment );
			if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				zero_size( ptr, size );
			break;
		case EAllocation_FREE :
			_aligned_free( old_memory );
			break;
		case EAllocation_RESIZE :
			{
				AllocatorInfo a = heap();
				ptr             = default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;

#elif defined( GEN_SYSTEM_LINUX ) && ! defined( GEN_CPU_ARM ) && ! defined( GEN_COMPILER_TINYC )
		case EAllocation_ALLOC :
			{
				ptr = aligned_alloc( alignment, ( size + alignment - 1 ) & ~( alignment - 1 ) );

				if ( flags & GEN_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				{
					zero_size( ptr, size );
				}
			}
			break;

		case EAllocation_FREE :
			{
				free( old_memory );
			}
			break;

		case EAllocation_RESIZE :
			{
				AllocatorInfo a = heap();
				ptr             = default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;
#else
		case EAllocation_ALLOC :
			{
				posix_memalign( &ptr, alignment, size );

				if ( flags & GEN_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				{
					zero_size( ptr, size );
				}
			}
			break;

		case EAllocation_FREE :
			{
				free( old_memory );
			}
			break;

		case EAllocation_RESIZE :
			{
				AllocatorInfo a = heap();
				ptr             = default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;
#endif

		case EAllocation_FREE_ALL :
			break;
	}

#ifdef GEN_HEAP_ANALYSIS
	if ( type == EAllocation_ALLOC )
	{
		_heap_alloc_info* alloc_info = rcast( _heap_alloc_info*, rcast( char*, ptr) + alloc_info_remainder );
		zero_item( alloc_info );
		alloc_info->size              = size - track_size;
		alloc_info->physical_start    = ptr;
		ptr                           = rcast( void*, alloc_info + 1 );
		_heap_stats_info.used_memory += alloc_info->size;
		_heap_stats_info.alloc_count++;
	}
#endif

	return ptr;
}

#pragma region VirtualMemory
VirtualMemory vm_from_memory( void* data, ssize size )
{
	VirtualMemory vm;
	vm.data = data;
	vm.size = size;
	return vm;
}

#if defined( GEN_SYSTEM_WINDOWS )
VirtualMemory vm_alloc( void* addr, ssize size )
{
	VirtualMemory vm;
	GEN_ASSERT( size > 0 );
	vm.data = VirtualAlloc( addr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	vm.size = size;
	return vm;
}

b32 vm_free( VirtualMemory vm )
{
	MEMORY_BASIC_INFORMATION info;
	while ( vm.size > 0 )
	{
		if ( VirtualQuery( vm.data, &info, size_of( info ) ) == 0 )
			return false;
		if ( info.BaseAddress != vm.data || info.AllocationBase != vm.data || info.State != MEM_COMMIT || info.RegionSize > scast( usize, vm.size) )
		{
			return false;
		}
		if ( VirtualFree( vm.data, 0, MEM_RELEASE ) == 0 )
			return false;
		vm.data  = pointer_add( vm.data, info.RegionSize );
		vm.size -= info.RegionSize;
	}
	return true;
}

VirtualMemory vm_trim( VirtualMemory vm, ssize lead_size, ssize size )
{
	VirtualMemory new_vm = { 0 };
	void*             ptr;
	GEN_ASSERT( vm.size >= lead_size + size );

	ptr = pointer_add( vm.data, lead_size );

	vm_free( vm );
	new_vm = vm_alloc( ptr, size );
	if ( new_vm.data == ptr )
		return new_vm;
	if ( new_vm.data )
		vm_free( new_vm );
	return new_vm;
}

b32 vm_purge( VirtualMemory vm )
{
	VirtualAlloc( vm.data, vm.size, MEM_RESET, PAGE_READWRITE );
	// NOTE: Can this really fail?
	return true;
}

ssize virtual_memory_page_size( ssize* alignment_out )
{
	SYSTEM_INFO info;
	GetSystemInfo( &info );
	if ( alignment_out )
		*alignment_out = info.dwAllocationGranularity;
	return info.dwPageSize;
}

#else
#	include <sys/mman.h>

#	ifndef MAP_ANONYMOUS
#		define MAP_ANONYMOUS MAP_ANON
#	endif
VirtualMemory vm_alloc( void* addr, ssize size )
{
	VirtualMemory vm;
	GEN_ASSERT( size > 0 );
	vm.data = mmap( addr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0 );
	vm.size = size;
	return vm;
}

b32 vm_free( VirtualMemory vm )
{
	munmap( vm.data, vm.size );
	return true;
}

VirtualMemory vm_trim( VirtualMemory vm, ssize lead_size, ssize size )
{
	void*  ptr;
	ssize trail_size;
	GEN_ASSERT( vm.size >= lead_size + size );

	ptr        = pointer_add( vm.data, lead_size );
	trail_size = vm.size - lead_size - size;

	if ( lead_size != 0 )
		vm_free( vm_from_memory(( vm.data, lead_size ) );
	if ( trail_size != 0 )
		vm_free( vm_from_memory( ptr, trail_size ) );
	return vm_from_memory( ptr, size );
}

b32 vm_purge( VirtualMemory vm )
{
	int err = madvise( vm.data, vm.size, MADV_DONTNEED );
	return err != 0;
}

ssize virtual_memory_page_size( ssize* alignment_out )
{
	// TODO: Is this always true?
	ssize result = scast( ssize, sysconf( _SC_PAGE_SIZE ));
	if ( alignment_out )
		*alignment_out = result;
	return result;
}
#endif

#pragma endregion VirtualMemory

void* arena_allocator_proc( void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags )
{
	Arena* arena = rcast(Arena*, allocator_data);
	void*      ptr   = NULL;

	// unused( old_size );

	switch ( type )
	{
		case EAllocation_ALLOC :
			{
				void* end        = pointer_add( arena->PhysicalStart, arena->TotalUsed );
				ssize total_size = align_forward_s64( size, alignment );

				// NOTE: Out of memory
				if ( arena->TotalUsed + total_size > (ssize) arena->TotalSize )
				{
					// zpl__printf_err("%s", "Arena out of memory\n");
					GEN_FATAL("Arena out of memory! (Possibly could not fit for the largest size Arena!!)");
					return nullptr;
				}

				ptr              = align_forward( end, alignment );
				arena->TotalUsed += total_size;

				if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
					zero_size( ptr, size );
			}
			break;

		case EAllocation_FREE :
			// NOTE: Free all at once
			// Use Temp_Arena_Memory if you want to free a block
			break;

		case EAllocation_FREE_ALL :
			arena->TotalUsed = 0;
			break;

		case EAllocation_RESIZE :
			{
				// TODO : Check if ptr is on top of stack and just extend
				AllocatorInfo a = arena->Backing;
				ptr             = default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;
	}
	return ptr;
}

void* pool_allocator_proc( void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags )
{
	Pool* pool = rcast( Pool*, allocator_data);
	void* ptr  = NULL;

	// unused( old_size );

	switch ( type )
	{
		case EAllocation_ALLOC :
			{
				uptr next_free;

				GEN_ASSERT( size == pool->BlockSize );
				GEN_ASSERT( alignment == pool->BlockAlign );
				GEN_ASSERT( pool->FreeList != NULL );

				next_free        = * rcast( uptr*, pool->FreeList);
				ptr              = pool->FreeList;
				pool->FreeList   = rcast( void*, next_free);
				pool->TotalSize += pool->BlockSize;

				if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
					zero_size( ptr, size );
			}
			break;

		case EAllocation_FREE :
			{
				uptr* next;
				if ( old_memory == NULL )
					return NULL;

				next             = rcast( uptr*, old_memory);
				*next            = rcast( uptr, pool->FreeList);
				pool->FreeList   = old_memory;
				pool->TotalSize -= pool->BlockSize;
			}
			break;

		case EAllocation_FREE_ALL :
			{
				ssize    actual_block_size, block_index;
				void* curr;
				uptr* end;

				actual_block_size = pool->BlockSize + pool->BlockAlign;
				pool->TotalSize   = 0;

				// NOTE: Init intrusive freelist
				curr = pool->PhysicalStart;
				for ( block_index = 0; block_index < pool->NumBlocks - 1; block_index++ )
				{
					uptr* next = rcast( uptr*, curr);
					* next     = rcast( uptr, curr) + actual_block_size;
					curr       = pointer_add( curr, actual_block_size );
				}

				end            = rcast( uptr*, curr);
				* end          = scast( uptr, NULL);
				pool->FreeList = pool->PhysicalStart;
			}
			break;

		case EAllocation_RESIZE :
			// NOTE: Cannot resize
			GEN_PANIC( "You cannot resize something allocated by with a pool." );
			break;
	}

	return ptr;
}

Pool pool_init_align( AllocatorInfo backing, ssize num_blocks, ssize block_size, ssize block_align )
{
	Pool pool = {};

	ssize    actual_block_size, pool_size, block_index;
	void *data, *curr;
	uptr* end;

	zero_item( &pool );

	pool.Backing     = backing;
	pool.BlockSize   = block_size;
	pool.BlockAlign  = block_align;
	pool.NumBlocks   = num_blocks;

	actual_block_size = block_size + block_align;
	pool_size         = num_blocks * actual_block_size;

	data = alloc_align( backing, pool_size, block_align );

	// NOTE: Init intrusive freelist
	curr = data;
	for ( block_index = 0; block_index < num_blocks - 1; block_index++ )
	{
		uptr* next = ( uptr* ) curr;
		*next      = ( uptr  ) curr + actual_block_size;
		curr       = pointer_add( curr, actual_block_size );
	}

	end  =  ( uptr* ) curr;
	*end =  ( uptr )  NULL;

	pool.PhysicalStart = data;
	pool.FreeList      = data;

	return pool;
}

void pool_clear(Pool* pool)
{
	ssize actual_block_size, block_index;
	void* curr;
	uptr* end;

	actual_block_size = pool->BlockSize + pool->BlockAlign;

	curr = pool->PhysicalStart;
	for ( block_index = 0; block_index < pool->NumBlocks - 1; block_index++ )
	{
		uptr* next = ( uptr* ) curr;
		*next      = ( uptr  ) curr + actual_block_size;
		curr       = pointer_add( curr, actual_block_size );
	}

	end  =  ( uptr* ) curr;
	*end =  ( uptr )  NULL;

	pool->FreeList = pool->PhysicalStart;
}

#pragma endregion Memory

#pragma region Hashing

global u32 const _crc32_table[ 256 ] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd,
	0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
	0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f,
	0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce,
	0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0,
	0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703,
	0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
	0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5,
	0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

u32 crc32( void const* data, ssize len )
{
	ssize        remaining;
	u32       result = ~( scast( u32, 0) );
	u8 const* c      = rcast( u8 const*, data);
	for ( remaining = len; remaining--; c++ )
		result = ( result >> 8 ) ^ ( _crc32_table[ ( result ^ *c ) & 0xff ] );
	return ~result;
}

global u64 const _crc64_table[ 256 ] = {
	0x0000000000000000ull, 0x7ad870c830358979ull, 0xf5b0e190606b12f2ull, 0x8f689158505e9b8bull, 0xc038e5739841b68full, 0xbae095bba8743ff6ull, 0x358804e3f82aa47dull,
	0x4f50742bc81f2d04ull, 0xab28ecb46814fe75ull, 0xd1f09c7c5821770cull, 0x5e980d24087fec87ull, 0x24407dec384a65feull, 0x6b1009c7f05548faull, 0x11c8790fc060c183ull,
	0x9ea0e857903e5a08ull, 0xe478989fa00bd371ull, 0x7d08ff3b88be6f81ull, 0x07d08ff3b88be6f8ull, 0x88b81eabe8d57d73ull, 0xf2606e63d8e0f40aull, 0xbd301a4810ffd90eull,
	0xc7e86a8020ca5077ull, 0x4880fbd87094cbfcull, 0x32588b1040a14285ull, 0xd620138fe0aa91f4ull, 0xacf86347d09f188dull, 0x2390f21f80c18306ull, 0x594882d7b0f40a7full,
	0x1618f6fc78eb277bull, 0x6cc0863448deae02ull, 0xe3a8176c18803589ull, 0x997067a428b5bcf0ull, 0xfa11fe77117cdf02ull, 0x80c98ebf2149567bull, 0x0fa11fe77117cdf0ull,
	0x75796f2f41224489ull, 0x3a291b04893d698dull, 0x40f16bccb908e0f4ull, 0xcf99fa94e9567b7full, 0xb5418a5cd963f206ull, 0x513912c379682177ull, 0x2be1620b495da80eull,
	0xa489f35319033385ull, 0xde51839b2936bafcull, 0x9101f7b0e12997f8ull, 0xebd98778d11c1e81ull, 0x64b116208142850aull, 0x1e6966e8b1770c73ull, 0x8719014c99c2b083ull,
	0xfdc17184a9f739faull, 0x72a9e0dcf9a9a271ull, 0x08719014c99c2b08ull, 0x4721e43f0183060cull, 0x3df994f731b68f75ull, 0xb29105af61e814feull, 0xc849756751dd9d87ull,
	0x2c31edf8f1d64ef6ull, 0x56e99d30c1e3c78full, 0xd9810c6891bd5c04ull, 0xa3597ca0a188d57dull, 0xec09088b6997f879ull, 0x96d1784359a27100ull, 0x19b9e91b09fcea8bull,
	0x636199d339c963f2ull, 0xdf7adabd7a6e2d6full, 0xa5a2aa754a5ba416ull, 0x2aca3b2d1a053f9dull, 0x50124be52a30b6e4ull, 0x1f423fcee22f9be0ull, 0x659a4f06d21a1299ull,
	0xeaf2de5e82448912ull, 0x902aae96b271006bull, 0x74523609127ad31aull, 0x0e8a46c1224f5a63ull, 0x81e2d7997211c1e8ull, 0xfb3aa75142244891ull, 0xb46ad37a8a3b6595ull,
	0xceb2a3b2ba0eececull, 0x41da32eaea507767ull, 0x3b024222da65fe1eull, 0xa2722586f2d042eeull, 0xd8aa554ec2e5cb97ull, 0x57c2c41692bb501cull, 0x2d1ab4dea28ed965ull,
	0x624ac0f56a91f461ull, 0x1892b03d5aa47d18ull, 0x97fa21650afae693ull, 0xed2251ad3acf6feaull, 0x095ac9329ac4bc9bull, 0x7382b9faaaf135e2ull, 0xfcea28a2faafae69ull,
	0x8632586aca9a2710ull, 0xc9622c4102850a14ull, 0xb3ba5c8932b0836dull, 0x3cd2cdd162ee18e6ull, 0x460abd1952db919full, 0x256b24ca6b12f26dull, 0x5fb354025b277b14ull,
	0xd0dbc55a0b79e09full, 0xaa03b5923b4c69e6ull, 0xe553c1b9f35344e2ull, 0x9f8bb171c366cd9bull, 0x10e3202993385610ull, 0x6a3b50e1a30ddf69ull, 0x8e43c87e03060c18ull,
	0xf49bb8b633338561ull, 0x7bf329ee636d1eeaull, 0x012b592653589793ull, 0x4e7b2d0d9b47ba97ull, 0x34a35dc5ab7233eeull, 0xbbcbcc9dfb2ca865ull, 0xc113bc55cb19211cull,
	0x5863dbf1e3ac9decull, 0x22bbab39d3991495ull, 0xadd33a6183c78f1eull, 0xd70b4aa9b3f20667ull, 0x985b3e827bed2b63ull, 0xe2834e4a4bd8a21aull, 0x6debdf121b863991ull,
	0x1733afda2bb3b0e8ull, 0xf34b37458bb86399ull, 0x8993478dbb8deae0ull, 0x06fbd6d5ebd3716bull, 0x7c23a61ddbe6f812ull, 0x3373d23613f9d516ull, 0x49aba2fe23cc5c6full,
	0xc6c333a67392c7e4ull, 0xbc1b436e43a74e9dull, 0x95ac9329ac4bc9b5ull, 0xef74e3e19c7e40ccull, 0x601c72b9cc20db47ull, 0x1ac40271fc15523eull, 0x5594765a340a7f3aull,
	0x2f4c0692043ff643ull, 0xa02497ca54616dc8ull, 0xdafce7026454e4b1ull, 0x3e847f9dc45f37c0ull, 0x445c0f55f46abeb9ull, 0xcb349e0da4342532ull, 0xb1eceec59401ac4bull,
	0xfebc9aee5c1e814full, 0x8464ea266c2b0836ull, 0x0b0c7b7e3c7593bdull, 0x71d40bb60c401ac4ull, 0xe8a46c1224f5a634ull, 0x927c1cda14c02f4dull, 0x1d148d82449eb4c6ull,
	0x67ccfd4a74ab3dbfull, 0x289c8961bcb410bbull, 0x5244f9a98c8199c2ull, 0xdd2c68f1dcdf0249ull, 0xa7f41839ecea8b30ull, 0x438c80a64ce15841ull, 0x3954f06e7cd4d138ull,
	0xb63c61362c8a4ab3ull, 0xcce411fe1cbfc3caull, 0x83b465d5d4a0eeceull, 0xf96c151de49567b7ull, 0x76048445b4cbfc3cull, 0x0cdcf48d84fe7545ull, 0x6fbd6d5ebd3716b7ull,
	0x15651d968d029fceull, 0x9a0d8ccedd5c0445ull, 0xe0d5fc06ed698d3cull, 0xaf85882d2576a038ull, 0xd55df8e515432941ull, 0x5a3569bd451db2caull, 0x20ed197575283bb3ull,
	0xc49581ead523e8c2ull, 0xbe4df122e51661bbull, 0x3125607ab548fa30ull, 0x4bfd10b2857d7349ull, 0x04ad64994d625e4dull, 0x7e7514517d57d734ull, 0xf11d85092d094cbfull,
	0x8bc5f5c11d3cc5c6ull, 0x12b5926535897936ull, 0x686de2ad05bcf04full, 0xe70573f555e26bc4ull, 0x9ddd033d65d7e2bdull, 0xd28d7716adc8cfb9ull, 0xa85507de9dfd46c0ull,
	0x273d9686cda3dd4bull, 0x5de5e64efd965432ull, 0xb99d7ed15d9d8743ull, 0xc3450e196da80e3aull, 0x4c2d9f413df695b1ull, 0x36f5ef890dc31cc8ull, 0x79a59ba2c5dc31ccull,
	0x037deb6af5e9b8b5ull, 0x8c157a32a5b7233eull, 0xf6cd0afa9582aa47ull, 0x4ad64994d625e4daull, 0x300e395ce6106da3ull, 0xbf66a804b64ef628ull, 0xc5bed8cc867b7f51ull,
	0x8aeeace74e645255ull, 0xf036dc2f7e51db2cull, 0x7f5e4d772e0f40a7ull, 0x05863dbf1e3ac9deull, 0xe1fea520be311aafull, 0x9b26d5e88e0493d6ull, 0x144e44b0de5a085dull,
	0x6e963478ee6f8124ull, 0x21c640532670ac20ull, 0x5b1e309b16452559ull, 0xd476a1c3461bbed2ull, 0xaeaed10b762e37abull, 0x37deb6af5e9b8b5bull, 0x4d06c6676eae0222ull,
	0xc26e573f3ef099a9ull, 0xb8b627f70ec510d0ull, 0xf7e653dcc6da3dd4ull, 0x8d3e2314f6efb4adull, 0x0256b24ca6b12f26ull, 0x788ec2849684a65full, 0x9cf65a1b368f752eull,
	0xe62e2ad306bafc57ull, 0x6946bb8b56e467dcull, 0x139ecb4366d1eea5ull, 0x5ccebf68aecec3a1ull, 0x2616cfa09efb4ad8ull, 0xa97e5ef8cea5d153ull, 0xd3a62e30fe90582aull,
	0xb0c7b7e3c7593bd8ull, 0xca1fc72bf76cb2a1ull, 0x45775673a732292aull, 0x3faf26bb9707a053ull, 0x70ff52905f188d57ull, 0x0a2722586f2d042eull, 0x854fb3003f739fa5ull,
	0xff97c3c80f4616dcull, 0x1bef5b57af4dc5adull, 0x61372b9f9f784cd4ull, 0xee5fbac7cf26d75full, 0x9487ca0fff135e26ull, 0xdbd7be24370c7322ull, 0xa10fceec0739fa5bull,
	0x2e675fb4576761d0ull, 0x54bf2f7c6752e8a9ull, 0xcdcf48d84fe75459ull, 0xb71738107fd2dd20ull, 0x387fa9482f8c46abull, 0x42a7d9801fb9cfd2ull, 0x0df7adabd7a6e2d6ull,
	0x772fdd63e7936bafull, 0xf8474c3bb7cdf024ull, 0x829f3cf387f8795dull, 0x66e7a46c27f3aa2cull, 0x1c3fd4a417c62355ull, 0x935745fc4798b8deull, 0xe98f353477ad31a7ull,
	0xa6df411fbfb21ca3ull, 0xdc0731d78f8795daull, 0x536fa08fdfd90e51ull, 0x29b7d047efec8728ull,
};

u64 crc64( void const* data, ssize len )
{
	ssize        remaining;
	u64       result = ( scast( u64, 0) );
	u8 const* c      = rcast( u8 const*, data);
	for ( remaining = len; remaining--; c++ )
		result = ( result >> 8 ) ^ ( _crc64_table[ ( result ^ *c ) & 0xff ] );
	return result;
}

#pragma endregion Hashing

#pragma region StrBuilder

StrBuilder strbuilder_make_length( AllocatorInfo allocator, char const* str, ssize length )
{
	ssize const header_size = sizeof( StrBuilderHeader );

	s32   alloc_size = header_size + length + 1;
	void* allocation = alloc( allocator, alloc_size );

	if ( allocation == nullptr ) {
		StrBuilder null_string = {nullptr};
		return null_string;
	}

	StrBuilderHeader*
	header = rcast(StrBuilderHeader*, allocation);
	header->Allocator = allocator;
	header->Capacity  = length;
	header->Length    = length;

	StrBuilder  result = { rcast( char*, allocation) + header_size };

	if ( length && str )
		mem_copy( result, str, length );
	else
		mem_set( result, 0, alloc_size - header_size );

	result[ length ] = '\0';

	return result;
}

StrBuilder strbuilder_make_reserve( AllocatorInfo allocator, ssize capacity )
{
	ssize const header_size = sizeof( StrBuilderHeader );

	s32   alloc_size = header_size + capacity + 1;
	void* allocation = alloc( allocator, alloc_size );

	if ( allocation == nullptr ) {
		StrBuilder null_string = {nullptr};
		return null_string;
	}
	mem_set( allocation, 0, alloc_size );

	StrBuilderHeader*
	header            = rcast(StrBuilderHeader*, allocation);
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Length    = 0;

	StrBuilder result = { rcast(char*, allocation) + header_size };
	return result;
}

#pragma endregion StrBuilder

#pragma region File Handling

#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )

internal
wchar_t* _alloc_utf8_to_ucs2( AllocatorInfo a, char const* text, ssize* w_len_ )
{
	wchar_t* w_text = NULL;
	ssize       len = 0, w_len = 0, w_len1 = 0;
	if ( text == NULL )
	{
		if ( w_len_ )
			*w_len_ = w_len;
		return NULL;
	}
	len = c_str_len( text );
	if ( len == 0 )
	{
		if ( w_len_ )
			*w_len_ = w_len;
		return NULL;
	}
	w_len = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, text, scast( int, len), NULL, 0 );
	if ( w_len == 0 )
	{
		if ( w_len_ )
			*w_len_ = w_len;
		return NULL;
	}
	w_text = alloc_array( a, wchar_t, w_len + 1 );
	w_len1 = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, text, scast( int, len), w_text, scast( int, w_len) );
	if ( w_len1 == 0 )
	{
		allocator_free( a, w_text );
		if ( w_len_ )
			*w_len_ = 0;
		return NULL;
	}
	w_text[ w_len ] = 0;
	if ( w_len_ )
		*w_len_ = w_len;
	return w_text;
}

internal
GEN_FILE_SEEK_PROC( _win32_file_seek )
{
	LARGE_INTEGER li_offset;
	li_offset.QuadPart = offset;
	if ( ! SetFilePointerEx( fd.p, li_offset, &li_offset, whence ) )
	{
		return false;
	}

	if ( new_offset )
		*new_offset = li_offset.QuadPart;
	return true;
}

internal
GEN_FILE_READ_AT_PROC( _win32_file_read )
{
	// unused( stop_at_newline );
	b32 result = false;
	_win32_file_seek( fd, offset, ESeekWhence_BEGIN, NULL );
	DWORD size_ = scast( DWORD, ( size > GEN_I32_MAX ? GEN_I32_MAX : size ));
	DWORD bytes_read_;
	if ( ReadFile( fd.p, buffer, size_, &bytes_read_, NULL ) )
	{
		if ( bytes_read )
			*bytes_read = bytes_read_;
		result = true;
	}

	return result;
}

internal
GEN_FILE_WRITE_AT_PROC( _win32_file_write )
{
	DWORD size_ = scast( DWORD, ( size > GEN_I32_MAX ? GEN_I32_MAX : size ));
	DWORD bytes_written_;
	_win32_file_seek( fd, offset, ESeekWhence_BEGIN, NULL );
	if ( WriteFile( fd.p, buffer, size_, &bytes_written_, NULL ) )
	{
		if ( bytes_written )
			*bytes_written = bytes_written_;
		return true;
	}
	return false;
}

internal
GEN_FILE_CLOSE_PROC( _win32_file_close )
{
	CloseHandle( fd.p );
}

FileOperations const default_file_operations = { _win32_file_read, _win32_file_write, _win32_file_seek, _win32_file_close };

neverinline
GEN_FILE_OPEN_PROC( _win32_file_open )
{
	DWORD    desired_access;
	DWORD    creation_disposition;
	void*    handle;
	wchar_t* w_text;

	switch ( mode & GEN_FILE_MODES )
	{
		case EFileMode_READ :
			desired_access       = GENERIC_READ;
			creation_disposition = OPEN_EXISTING;
			break;
		case EFileMode_WRITE :
			desired_access       = GENERIC_WRITE;
			creation_disposition = CREATE_ALWAYS;
			break;
		case EFileMode_APPEND :
			desired_access       = GENERIC_WRITE;
			creation_disposition = OPEN_ALWAYS;
			break;
		case EFileMode_READ | EFileMode_RW :
			desired_access       = GENERIC_READ | GENERIC_WRITE;
			creation_disposition = OPEN_EXISTING;
			break;
		case EFileMode_WRITE | EFileMode_RW :
			desired_access       = GENERIC_READ | GENERIC_WRITE;
			creation_disposition = CREATE_ALWAYS;
			break;
		case EFileMode_APPEND | EFileMode_RW :
			desired_access       = GENERIC_READ | GENERIC_WRITE;
			creation_disposition = OPEN_ALWAYS;
			break;
		default :
			GEN_PANIC( "Invalid file mode" );
			return EFileError_INVALID;
	}

	w_text = _alloc_utf8_to_ucs2( heap(), filename, NULL );
	handle = CreateFileW( w_text, desired_access, FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, creation_disposition, FILE_ATTRIBUTE_NORMAL, NULL );

	allocator_free( heap(), w_text );

	if ( handle == INVALID_HANDLE_VALUE )
	{
		DWORD err = GetLastError();
		switch ( err )
		{
			case ERROR_FILE_NOT_FOUND :
				return EFileError_NOT_EXISTS;
			case ERROR_FILE_EXISTS :
				return EFileError_EXISTS;
			case ERROR_ALREADY_EXISTS :
				return EFileError_EXISTS;
			case ERROR_ACCESS_DENIED :
				return EFileError_PERMISSION;
		}
		return EFileError_INVALID;
	}

	if ( mode & EFileMode_APPEND )
	{
		LARGE_INTEGER offset = { { 0 } };
		if ( ! SetFilePointerEx( handle, offset, NULL, ESeekWhence_END ) )
		{
			CloseHandle( handle );
			return EFileError_INVALID;
		}
	}

	fd->p = handle;
	*ops  = default_file_operations;
	return EFileError_NONE;
}

#else    // POSIX
#	include <fcntl.h>

internal
GEN_FILE_SEEK_PROC( _posix_file_seek )
{
#	if defined( GEN_SYSTEM_OSX )
	s64 res = lseek( fd.i, offset, whence );
#	else    // TODO(ZaKlaus): @fixme lseek64
	s64 res = lseek( fd.i, offset, whence );
#	endif
	if ( res < 0 )
		return false;
	if ( new_offset )
		*new_offset = res;
	return true;
}

internal
GEN_FILE_READ_AT_PROC( _posix_file_read )
{
	unused( stop_at_newline );
	ssize res = pread( fd.i, buffer, size, offset );
	if ( res < 0 )
		return false;
	if ( bytes_read )
		*bytes_read = res;
	return true;
}

internal
GEN_FILE_WRITE_AT_PROC( _posix_file_write )
{
	ssize  res;
	s64 curr_offset = 0;
	_posix_file_seek( fd, 0, ESeekWhence_CURRENT, &curr_offset );
	if ( curr_offset == offset )
	{
		// NOTE: Writing to stdout et al. doesn't like pwrite for numerous reasons
		res = write( scast( int, fd.i), buffer, size );
	}
	else
	{
		res = pwrite( scast( int, fd.i), buffer, size, offset );
	}
	if ( res < 0 )
		return false;
	if ( bytes_written )
		*bytes_written = res;
	return true;
}

internal
GEN_FILE_CLOSE_PROC( _posix_file_close )
{
	close( fd.i );
}

FileOperations const default_file_operations = { _posix_file_read, _posix_file_write, _posix_file_seek, _posix_file_close };

neverinline
GEN_FILE_OPEN_PROC( _posix_file_open )
{
	s32 os_mode;
	switch ( mode & GEN_FILE_MODES )
	{
		case EFileMode_READ :
			os_mode = O_RDONLY;
			break;
		case EFileMode_WRITE :
			os_mode = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case EFileMode_APPEND :
			os_mode = O_WRONLY | O_APPEND | O_CREAT;
			break;
		case EFileMode_READ | EFileMode_RW :
			os_mode = O_RDWR;
			break;
		case EFileMode_WRITE | EFileMode_RW :
			os_mode = O_RDWR | O_CREAT | O_TRUNC;
			break;
		case EFileMode_APPEND | EFileMode_RW :
			os_mode = O_RDWR | O_APPEND | O_CREAT;
			break;
		default :
			GEN_PANIC( "Invalid file mode" );
			return EFileError_INVALID;
	}

	fd->i = open( filename, os_mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
	if ( fd->i < 0 )
	{
		// TODO : More file errors
		return EFileError_INVALID;
	}

	*ops = default_file_operations;
	return EFileError_NONE;
}

// POSIX
#endif

internal void _dirinfo_free_entry( DirEntry* entry );

// TODO(zpl) : Is this a bad idea?
global b32      _std_file_set                     = false;
global FileInfo _std_files[ EFileStandard_COUNT ] = {
{
	{ nullptr, nullptr, nullptr, nullptr },
	{ nullptr },
	0,
	nullptr,
	0,
	nullptr
} };

#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )

FileInfo* file_get_standard( FileStandardType std )
{
	if ( ! _std_file_set )
	{
#	define GEN__SET_STD_FILE( type, v ) \
		_std_files[ type ].fd.p = v;    \
		_std_files[ type ].ops  = default_file_operations
		GEN__SET_STD_FILE( EFileStandard_INPUT, GetStdHandle( STD_INPUT_HANDLE ) );
		GEN__SET_STD_FILE( EFileStandard_OUTPUT, GetStdHandle( STD_OUTPUT_HANDLE ) );
		GEN__SET_STD_FILE( EFileStandard_ERROR, GetStdHandle( STD_ERROR_HANDLE ) );
#	undef GEN__SET_STD_FILE
		_std_file_set = true;
	}
	return &_std_files[ std ];
}

#else    // POSIX

FileInfo* file_get_standard( FileStandardType std )
{
	if ( ! _std_file_set )
	{
#	define GEN__SET_STD_FILE( type, v ) \
		_std_files[ type ].fd.i = v;     \
		_std_files[ type ].ops  = default_file_operations
		GEN__SET_STD_FILE( EFileStandard_INPUT, 0 );
		GEN__SET_STD_FILE( EFileStandard_OUTPUT, 1 );
		GEN__SET_STD_FILE( EFileStandard_ERROR, 2 );
#	undef GEN__SET_STD_FILE
		_std_file_set = true;
	}
	return &_std_files[ std ];
}

#endif

FileError file_close( FileInfo* f )
{
	if ( ! f )
		return EFileError_INVALID;

	if ( f->filename )
		allocator_free( heap(), ccast( char*, f->filename ));

#if defined( GEN_SYSTEM_WINDOWS )
	if ( f->fd.p == INVALID_HANDLE_VALUE )
		return EFileError_INVALID;
#else
	if ( f->fd.i < 0 )
		return EFileError_INVALID;
#endif

	if ( f->is_temp )
	{
		f->ops.close( f->fd );
		return EFileError_NONE;
	}

	if ( ! f->ops.read_at )
		f->ops = default_file_operations;
	f->ops.close( f->fd );

#if 0
	if ( f->Dir )
	{
		_dirinfo_free_entry( f->Dir );
		mfree( f->Dir );
		f->Dir = NULL;
	}
#endif

	return EFileError_NONE;
}

FileError file_new( FileInfo* f, FileDescriptor fd, FileOperations ops, char const* filename )
{
	FileError err = EFileError_NONE;
	ssize        len = c_str_len( filename );

	f->ops             = ops;
	f->fd              = fd;
	f->dir             = nullptr;
	f->last_write_time = 0;
	f->filename        = alloc_array( heap(), char, len + 1 );
	mem_copy( ccast( char*, f->filename), ccast( char*, filename), len + 1 );

	return err;
}

FileError file_open( FileInfo* f, char const* filename )
{
	return file_open_mode( f, EFileMode_READ, filename );
}

FileError file_open_mode( FileInfo* f, FileMode mode, char const* filename )
{
	FileInfo file_ =
	{
		{ nullptr, nullptr, nullptr, nullptr },
		{ nullptr },
		0,
		nullptr,
		0,
		nullptr
		};

	*f = file_;
	FileError err;

#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )
	err = _win32_file_open( &f->fd, &f->ops, mode, filename );
#else
	err = _posix_file_open( &f->fd, &f->ops, mode, filename );
#endif

	if ( err == EFileError_NONE )
		return file_new( f, f->fd, f->ops, filename );

	return err;
}

s64 file_size( FileInfo* f )
{
	s64 size        = 0;
	s64 prev_offset = file_tell( f );

	file_seek_to_end( f );
	size = file_tell( f );

	file_seek( f, prev_offset );

	return size;
}

FileContents file_read_contents( AllocatorInfo a, b32 zero_terminate, char const* filepath )
{
	FileContents result;
	FileInfo     file  ;

	result.allocator = a;

	if ( file_open( &file, filepath ) == EFileError_NONE )
	{
		ssize fsize = scast( ssize , file_size( &file ));
		if ( fsize > 0 )
		{
			result.data = alloc( a, zero_terminate ? fsize + 1 : fsize );
			result.size = fsize;
			file_read_at( &file, result.data, result.size, 0 );
			if ( zero_terminate )
			{
				u8* str      = rcast( u8*, result.data);
				str[ fsize ] = '\0';
			}
		}
		file_close( &file );
	}

	return result;
}

typedef struct _memory_fd _memory_fd;
struct _memory_fd
{
	u8            magic;
	u8*           buf;    //< zpl_array OR plain buffer if we can't write
	ssize         cursor;
	AllocatorInfo allocator;

	FileStreamFlags flags;
	ssize           cap;
};

#define GEN__FILE_STREAM_FD_MAGIC 37

FileDescriptor _file_stream_fd_make( _memory_fd* d );
_memory_fd*    _file_stream_from_fd( FileDescriptor fd );

inline
FileDescriptor _file_stream_fd_make( _memory_fd* d )
{
	FileDescriptor fd = { 0 };
	fd.p              = ( void* )d;
	return fd;
}

inline
_memory_fd* _file_stream_from_fd( FileDescriptor fd )
{
	_memory_fd* d = ( _memory_fd* )fd.p;
	GEN_ASSERT( d->magic == GEN__FILE_STREAM_FD_MAGIC );
	return d;
}

b8 file_stream_new( FileInfo* file, AllocatorInfo allocator )
{
	GEN_ASSERT_NOT_NULL( file );

	_memory_fd* d = ( _memory_fd* )alloc( allocator, size_of( _memory_fd ) );

	if ( ! d )
		return false;

	zero_item( file );
	d->magic     = GEN__FILE_STREAM_FD_MAGIC;
	d->allocator = allocator;
	d->flags     = EFileStream_CLONE_WRITABLE;
	d->cap       = 0;
	d->buf       = array_init( u8, allocator );

	if ( ! d->buf )
		return false;

	file->ops             = memory_file_operations;
	file->fd              = _file_stream_fd_make( d );
	file->dir             = NULL;
	file->last_write_time = 0;
	file->filename        = NULL;
	file->is_temp         = true;
	return true;
}

b8 file_stream_open( FileInfo* file, AllocatorInfo allocator, u8* buffer, ssize size, FileStreamFlags flags )
{
	GEN_ASSERT_NOT_NULL( file );
	_memory_fd* d = ( _memory_fd* )alloc( allocator, size_of( _memory_fd ) );
	if ( ! d )
		return false;
	zero_item( file );
	d->magic     = GEN__FILE_STREAM_FD_MAGIC;
	d->allocator = allocator;
	d->flags     = flags;
	if ( d->flags & EFileStream_CLONE_WRITABLE )
	{
		Array(u8) arr = array_init_reserve(u8, allocator, size );
		d->buf = arr;

		if ( ! d->buf )
			return false;

		mem_copy( d->buf, buffer, size );
		d->cap = size;

		array_get_header(arr)->Num = size;
	}
	else
	{
		d->buf = buffer;
		d->cap = size;
	}
	file->ops             = memory_file_operations;
	file->fd              = _file_stream_fd_make( d );
	file->dir             = NULL;
	file->last_write_time = 0;
	file->filename        = NULL;
	file->is_temp         = true;
	return true;
}

u8* file_stream_buf( FileInfo* file, ssize* size )
{
	GEN_ASSERT_NOT_NULL( file );
	_memory_fd* d = _file_stream_from_fd( file->fd );
	if ( size )
		*size = d->cap;
	return d->buf;
}

internal
GEN_FILE_SEEK_PROC( _memory_file_seek )
{
	_memory_fd* d      = _file_stream_from_fd( fd );
	ssize          buflen = d->cap;

	if ( whence == ESeekWhence_BEGIN )
		d->cursor = 0;
	else if ( whence == ESeekWhence_END )
		d->cursor = buflen;

	d->cursor = max( 0, clamp( d->cursor + offset, 0, buflen ) );
	if ( new_offset )
		*new_offset = d->cursor;
	return true;
}

internal
GEN_FILE_READ_AT_PROC( _memory_file_read )
{
	// unused( stop_at_newline );
	_memory_fd* d = _file_stream_from_fd( fd );
	mem_copy( buffer, d->buf + offset, size );
	if ( bytes_read )
		*bytes_read = size;
	return true;
}

internal
GEN_FILE_WRITE_AT_PROC( _memory_file_write )
{
	_memory_fd* d = _file_stream_from_fd( fd );

	if ( ! ( d->flags & ( EFileStream_CLONE_WRITABLE | EFileStream_WRITABLE ) ) )
		return false;

	ssize buflen   = d->cap;
	ssize extralen = max( 0, size - ( buflen - offset ) );
	ssize rwlen    = size - extralen;
	ssize new_cap  = buflen + extralen;

	if ( d->flags & EFileStream_CLONE_WRITABLE )
	{
		Array(u8) arr = { d->buf };

		if ( array_get_header(arr)->Capacity < scast(usize, new_cap) )
		{
			if ( ! array_grow( & arr, ( s64 )( new_cap ) ) )
				return false;
			d->buf = arr;
		}
	}

	mem_copy( d->buf + offset, buffer, rwlen );

	if ( ( d->flags & EFileStream_CLONE_WRITABLE ) && extralen > 0 )
	{
		Array(u8) arr = { d->buf };

		mem_copy( d->buf + offset + rwlen, pointer_add_const( buffer, rwlen ), extralen );
		d->cap = new_cap;
		array_get_header(arr)->Capacity = new_cap;
	}
	else
	{
		extralen = 0;
	}

	if ( bytes_written )
		*bytes_written = ( rwlen + extralen );
	return true;
}

internal
GEN_FILE_CLOSE_PROC( _memory_file_close )
{
	_memory_fd*   d         = _file_stream_from_fd( fd );
	AllocatorInfo allocator = d->allocator;

	if ( d->flags & EFileStream_CLONE_WRITABLE )
	{
		Array(u8) arr = { d->buf };
		array_free(arr);
	}

	allocator_free( allocator, d );
}

FileOperations const memory_file_operations = { _memory_file_read, _memory_file_write, _memory_file_seek, _memory_file_close };

#pragma endregion File Handling

#pragma region Timing

#ifdef GEN_BENCHMARK
	#if defined( GEN_COMPILER_MSVC ) && ! defined( __clang__ )
	u64 read_cpu_time_stamp_counter( void )
	{
		return __rdtsc();
	}
	#elif defined( __i386__ )
	u64 read_cpu_time_stamp_counter( void )
	{
		u64 x;
		__asm__ volatile( ".byte 0x0f, 0x31" : "=A"( x ) );
		return x;
	}
	#elif defined( __x86_64__ )
	u64 read_cpu_time_stamp_counter( void )
	{
		u32 hi, lo;
		__asm__ __volatile__( "rdtsc" : "=a"( lo ), "=d"( hi ) );
		return scast( u64, lo ) | ( scast( u64, hi ) << 32 );
	}
	#elif defined( __powerpc__ )
	u64 read_cpu_time_stamp_counter( void )
	{
		u64 result = 0;
		u32 upper, lower, tmp;
		__asm__ volatile(
			"0:                   \n"
			"\tmftbu   %0         \n"
			"\tmftb    %1         \n"
			"\tmftbu   %2         \n"
			"\tcmpw    %2,%0      \n"
			"\tbne     0b         \n"
			: "=r"( upper ), "=r"( lower ), "=r"( tmp )
		);
		result = upper;
		result = result << 32;
		result = result | lower;

		return result;
	}
	#elif defined( GEN_SYSTEM_EMSCRIPTEN )
	u64 read_cpu_time_stamp_counter( void )
	{
		return ( u64 )( emscripten_get_now() * 1e+6 );
	}
	#elif defined( GEN_CPU_ARM ) && ! defined( GEN_COMPILER_TINYC )
	u64 read_cpu_time_stamp_counter( void )
	{
	#	if defined( __aarch64__ )
		int64_t r = 0;
		asm volatile( "mrs %0, cntvct_el0" : "=r"( r ) );
	#	elif ( __ARM_ARCH >= 6 )
		uint32_t r = 0;
		uint32_t pmccntr;
		uint32_t pmuseren;
		uint32_t pmcntenset;

		// Read the user mode perf monitor counter access permissions.
		asm volatile( "mrc p15, 0, %0, c9, c14, 0" : "=r"( pmuseren ) );
		if ( pmuseren & 1 )
		{    // Allows reading perfmon counters for user mode code.
			asm volatile( "mrc p15, 0, %0, c9, c12, 1" : "=r"( pmcntenset ) );
			if ( pmcntenset & 0x80000000ul )
			{    // Is it counting?
				asm volatile( "mrc p15, 0, %0, c9, c13, 0" : "=r"( pmccntr ) );
				// The counter is set up to count every 64th cycle
				return ( ( int64_t )pmccntr ) * 64;    // Should optimize to << 6
			}
		}
	#	else
	#		error "No suitable method for read_cpu_time_stamp_counter for this cpu type"
	#	endif

		return r;
	}
	#else
	u64 read_cpu_time_stamp_counter( void )
	{
		GEN_PANIC( "read_cpu_time_stamp_counter is not supported on this particular setup" );
		return -0;
	}
	#endif

	#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )

	u64 time_rel_ms( void )
	{
		local_persist LARGE_INTEGER win32_perf_count_freq = {};
		u64                         result;
		LARGE_INTEGER               counter;
		local_persist LARGE_INTEGER win32_perf_counter = {};
		if ( ! win32_perf_count_freq.QuadPart )
		{
			QueryPerformanceFrequency( &win32_perf_count_freq );
			GEN_ASSERT( win32_perf_count_freq.QuadPart != 0 );
			QueryPerformanceCounter( &win32_perf_counter );
		}

		QueryPerformanceCounter( &counter );

		result = ( counter.QuadPart - win32_perf_counter.QuadPart ) * 1000 / ( win32_perf_count_freq.QuadPart );
		return result;
	}

	#else

	#	if defined( GEN_SYSTEM_LINUX ) || defined( GEN_SYSTEM_FREEBSD ) || defined( GEN_SYSTEM_OPENBSD ) || defined( GEN_SYSTEM_EMSCRIPTEN )
	u64 _unix_gettime( void )
	{
		struct timespec t;
		u64             result;

		clock_gettime( 1 /*CLOCK_MONOTONIC*/, &t );
		result = 1000 * t.tv_sec + 1.0e-6 * t.tv_nsec;
		return result;
	}
	#	endif

	u64 time_rel_ms( void )
	{
	#	if defined( GEN_SYSTEM_OSX )
		u64 result;

		local_persist u64 timebase  = 0;
		local_persist u64 timestart = 0;

		if ( ! timestart )
		{
			mach_timebase_info_data_t tb = { 0 };
			mach_timebase_info( &tb );
			timebase   = tb.numer;
			timebase  /= tb.denom;
			timestart  = mach_absolute_time();
		}

		// NOTE: mach_absolute_time() returns things in nanoseconds
		result = 1.0e-6 * ( mach_absolute_time() - timestart ) * timebase;
		return result;
	#	else
		local_persist u64 unix_timestart = 0.0;

		if ( ! unix_timestart )
		{
			unix_timestart = _unix_gettime();
		}

		u64 now = _unix_gettime();

		return ( now - unix_timestart );
	#	endif
	}
	#endif

	f64 time_rel( void )
	{
		return ( f64 )( time_rel_ms() * 1e-3 );
	}
#endif

#pragma endregion Timing

#pragma region Parsing

#pragma region ADT

#define _adt_fprintf( s_, fmt_, ... )                      \
	do                                                     \
	{                                                      \
		if ( c_str_fmt_file( s_, fmt_, ##__VA_ARGS__ ) < 0 ) \
			return EADT_ERROR_OUT_OF_MEMORY;               \
	} while ( 0 )

u8 adt_make_branch( ADT_Node* node, AllocatorInfo backing, char const* name, b32 is_array )
{
	ADT_Type type = EADT_TYPE_OBJECT;
	if ( is_array )
		type = EADT_TYPE_ARRAY;

	ADT_Node* parent = node->parent;
	zero_item( node );

	node->type   = type;
	node->name   = name;
	node->parent = parent;
	node->nodes  = array_init(ADT_Node, backing );

	if ( ! node->nodes )
		return EADT_ERROR_OUT_OF_MEMORY;

	return 0;
}

u8 adt_destroy_branch( ADT_Node* node )
{
	GEN_ASSERT_NOT_NULL( node );
	if ( ( node->type == EADT_TYPE_OBJECT || node->type == EADT_TYPE_ARRAY ) && node->nodes )
	{
		for ( ssize i = 0; i < scast(ssize, array_num(node->nodes)); ++i )
		{
			adt_destroy_branch( node->nodes + i );
		}

		array_free(node->nodes);
	}
	return 0;
}

u8 adt_make_leaf( ADT_Node* node, char const* name, ADT_Type type )
{
	GEN_ASSERT( type != EADT_TYPE_OBJECT && type != EADT_TYPE_ARRAY );

	ADT_Node* parent = node->parent;
	zero_item( node );

	node->type   = type;
	node->name   = name;
	node->parent = parent;
	return 0;
}

ADT_Node* adt_find( ADT_Node* node, char const* name, b32 deep_search )
{
	if ( node->type != EADT_TYPE_OBJECT )
	{
		return NULL;
	}

	for ( ssize i = 0; i < scast(ssize, array_num(node->nodes)); i++ )
	{
		if ( ! c_str_compare( node->nodes[ i ].name, name ) )
		{
			return ( node->nodes + i );
		}
	}

	if ( deep_search )
	{
		for ( ssize i = 0; i < scast(ssize, array_num(node->nodes)); i++ )
		{
			ADT_Node* res = adt_find( node->nodes + i, name, deep_search );

			if ( res != NULL )
				return res;
		}
	}

	return NULL;
}

internal ADT_Node* _adt_get_value( ADT_Node* node, char const* value )
{
	switch ( node->type )
	{
		case EADT_TYPE_MULTISTRING :
		case EADT_TYPE_STRING :
			{
				if ( node->string && ! c_str_compare( node->string, value ) )
				{
					return node;
				}
			}
			break;
		case EADT_TYPE_INTEGER :
		case EADT_TYPE_REAL :
			{
				char     back[ 4096 ] = { 0 };
				FileInfo tmp;

				/* allocate a file descriptor for a memory-mapped number to string conversion, input source buffer is not cloned, however. */
				file_stream_open( &tmp, heap(), ( u8* )back, size_of( back ), EFileStream_WRITABLE );
				adt_print_number( &tmp, node );

				ssize  fsize = 0;
				u8* buf   = file_stream_buf( &tmp, &fsize );

				if ( ! c_str_compare( ( char const* )buf, value ) )
				{
					file_close( &tmp );
					return node;
				}

				file_close( &tmp );
			}
			break;
		default :
			break; /* node doesn't support value based lookup */
	}

	return NULL;
}

internal ADT_Node* _adt_get_field( ADT_Node* node, char* name, char* value )
{
	for ( ssize i = 0; i < scast(ssize, array_num(node->nodes)); i++ )
	{
		if ( ! c_str_compare( node->nodes[ i ].name, name ) )
		{
			ADT_Node* child = &node->nodes[ i ];
			if ( _adt_get_value( child, value ) )
			{
				return node; /* this object does contain a field of a specified value! */
			}
		}
	}

	return NULL;
}

ADT_Node* adt_query( ADT_Node* node, char const* uri )
{
	GEN_ASSERT_NOT_NULL( uri );

	if ( *uri == '/' )
	{
		uri++;
	}

	if ( *uri == 0 )
	{
		return node;
	}

	if ( ! node || ( node->type != EADT_TYPE_OBJECT && node->type != EADT_TYPE_ARRAY ) )
	{
		return NULL;
	}

#if defined EADT_URI_DEBUG || 0
	c_str_fmt_out( "uri: %s\n", uri );
#endif

	char *    p = ( char* )uri, *b = p, *e = p;
	ADT_Node* found_node = NULL;

	b = p;
	p = e     = ( char* )c_str_skip( p, '/' );
	char* buf = c_str_fmt_buf( "%.*s", ( int )( e - b ), b );

	/* handle field value lookup */
	if ( *b == '[' )
	{
		char *l_p = buf + 1, *l_b = l_p, *l_e = l_p, *l_b2 = l_p, *l_e2 = l_p;
		l_e  = ( char* )c_str_skip( l_p, '=' );
		l_e2 = ( char* )c_str_skip( l_p, ']' );

		if ( ( ! *l_e && node->type != EADT_TYPE_ARRAY ) || ! *l_e2 )
		{
			GEN_ASSERT_MSG( 0, "Invalid field value lookup" );
			return NULL;
		}

		*l_e2 = 0;

		/* [field=value] */
		if ( *l_e )
		{
			*l_e = 0;
			l_b2 = l_e + 1;

			/* run a value comparison against our own fields */
			if ( node->type == EADT_TYPE_OBJECT )
			{
				found_node = _adt_get_field( node, l_b, l_b2 );
			}

			/* run a value comparison against any child that is an object node */
			else if ( node->type == EADT_TYPE_ARRAY )
			{
				for ( ssize i = 0; i < scast(ssize, array_num(node->nodes)); i++ )
				{
					ADT_Node* child = &node->nodes[ i ];
					if ( child->type != EADT_TYPE_OBJECT )
					{
						continue;
					}

					found_node = _adt_get_field( child, l_b, l_b2 );

					if ( found_node )
						break;
				}
			}
		}
		/* [value] */
		else
		{
			for ( ssize i = 0; i < scast(ssize, array_num(node->nodes)); i++ )
			{
				ADT_Node* child = &node->nodes[ i ];
				if ( _adt_get_value( child, l_b2 ) )
				{
					found_node = child;
					break; /* we found a matching value in array, ignore the rest of it */
				}
			}
		}

		/* go deeper if uri continues */
		if ( *e )
		{
			return adt_query( found_node, e + 1 );
		}
	}
	/* handle field name lookup */
	else if ( node->type == EADT_TYPE_OBJECT )
	{
		found_node = adt_find( node, buf, false );

		/* go deeper if uri continues */
		if ( *e )
		{
			return adt_query( found_node, e + 1 );
		}
	}
	/* handle array index lookup */
	else
	{
		ssize idx = ( ssize )c_str_to_i64( buf, NULL, 10 );
		if ( idx >= 0 && idx < scast(ssize, array_num(node->nodes)) )
		{
			found_node = &node->nodes[ idx ];

			/* go deeper if uri continues */
			if ( *e )
			{
				return adt_query( found_node, e + 1 );
			}
		}
	}

	return found_node;
}

ADT_Node* adt_alloc_at( ADT_Node* parent, ssize index )
{
	if ( ! parent || ( parent->type != EADT_TYPE_OBJECT && parent->type != EADT_TYPE_ARRAY ) )
	{
		return NULL;
	}

	if ( ! parent->nodes )
		return NULL;

	if ( index < 0 || index > scast(ssize, array_num(parent->nodes)) )
		return NULL;

	ADT_Node o = { 0 };
	o.parent   = parent;
	if ( ! array_append_at( parent->nodes, o, index ) )
		return NULL;

	ADT_Node* node = & parent->nodes[index];
	return node;
}

ADT_Node* adt_alloc( ADT_Node* parent )
{
	if ( ! parent || ( parent->type != EADT_TYPE_OBJECT && parent->type != EADT_TYPE_ARRAY ) )
	{
		return NULL;
	}

	if ( ! parent->nodes )
		return NULL;

	return adt_alloc_at( parent, array_num(parent->nodes) );
}

b8 adt_set_obj( ADT_Node* obj, char const* name, AllocatorInfo backing )
{
	return adt_make_branch( obj, backing, name, 0 );
}

b8 adt_set_arr( ADT_Node* obj, char const* name, AllocatorInfo backing )
{
	return adt_make_branch( obj, backing, name, 1 );
}

b8 adt_set_str( ADT_Node* obj, char const* name, char const* value )
{
	adt_make_leaf( obj, name, EADT_TYPE_STRING );
	obj->string = value;
	return true;
}

b8 adt_set_flt( ADT_Node* obj, char const* name, f64 value )
{
	adt_make_leaf( obj, name, EADT_TYPE_REAL );
	obj->real = value;
	return true;
}

b8 adt_set_int( ADT_Node* obj, char const* name, s64 value )
{
	adt_make_leaf( obj, name, EADT_TYPE_INTEGER );
	obj->integer = value;
	return true;
}

ADT_Node* adt_move_node_at( ADT_Node* node, ADT_Node* new_parent, ssize index )
{
	GEN_ASSERT_NOT_NULL( node );
	GEN_ASSERT_NOT_NULL( new_parent );
	ADT_Node* old_parent = node->parent;
	ADT_Node* new_node   = adt_alloc_at( new_parent, index );
	*new_node            = *node;
	new_node->parent     = new_parent;
	if ( old_parent )
	{
		adt_remove_node( node );
	}
	return new_node;
}

ADT_Node* adt_move_node( ADT_Node* node, ADT_Node* new_parent )
{
	GEN_ASSERT_NOT_NULL( node );
	GEN_ASSERT_NOT_NULL( new_parent );
	GEN_ASSERT( new_parent->type == EADT_TYPE_ARRAY || new_parent->type == EADT_TYPE_OBJECT );
	return adt_move_node_at( node, new_parent, array_num(new_parent->nodes) );
}

void adt_swap_nodes( ADT_Node* node, ADT_Node* other_node )
{
	GEN_ASSERT_NOT_NULL( node );
	GEN_ASSERT_NOT_NULL( other_node );
	ADT_Node* parent                     = node->parent;
	ADT_Node* other_parent               = other_node->parent;
	ssize        index                      = ( pointer_diff( parent->nodes, node ) / size_of( ADT_Node ) );
	ssize        index2                     = ( pointer_diff( other_parent->nodes, other_node ) / size_of( ADT_Node ) );
	ADT_Node  temp                       = parent->nodes[ index ];
	temp.parent                          = other_parent;
	other_parent->nodes[ index2 ].parent = parent;
	parent->nodes[ index ]               = other_parent->nodes[ index2 ];
	other_parent->nodes[ index2 ]        = temp;
}

void adt_remove_node( ADT_Node* node )
{
	GEN_ASSERT_NOT_NULL( node );
	GEN_ASSERT_NOT_NULL( node->parent );
	ADT_Node* parent = node->parent;
	ssize        index  = ( pointer_diff( parent->nodes, node ) / size_of( ADT_Node ) );
	array_remove_at( parent->nodes, index );
}

ADT_Node* adt_append_obj( ADT_Node* parent, char const* name )
{
	ADT_Node* o = adt_alloc( parent );
	if ( ! o )
		return NULL;
	if ( adt_set_obj( o, name, array_get_header(parent->nodes)->Allocator ) )
	{
		adt_remove_node( o );
		return NULL;
	}
	return o;
}

ADT_Node* adt_append_arr( ADT_Node* parent, char const* name )
{
	ADT_Node* o = adt_alloc( parent );
	if ( ! o )
		return NULL;

	ArrayHeader* node_header = array_get_header(parent->nodes);
	if ( adt_set_arr( o, name, node_header->Allocator ) )
	{
		adt_remove_node( o );
		return NULL;
	}
	return o;
}

ADT_Node* adt_append_str( ADT_Node* parent, char const* name, char const* value )
{
	ADT_Node* o = adt_alloc( parent );
	if ( ! o )
		return NULL;
	adt_set_str( o, name, value );
	return o;
}

ADT_Node* adt_append_flt( ADT_Node* parent, char const* name, f64 value )
{
	ADT_Node* o = adt_alloc( parent );
	if ( ! o )
		return NULL;
	adt_set_flt( o, name, value );
	return o;
}

ADT_Node* adt_append_int( ADT_Node* parent, char const* name, s64 value )
{
	ADT_Node* o = adt_alloc( parent );
	if ( ! o )
		return NULL;
	adt_set_int( o, name, value );
	return o;
}

/* parser helpers */
char* adt_parse_number_strict( ADT_Node* node, char* base_str )
{
	GEN_ASSERT_NOT_NULL( node );
	GEN_ASSERT_NOT_NULL( base_str );
	char *p = base_str, *e = p;

	while ( *e )
		++e;

	while ( *p && ( char_first_occurence( "eE.+-", *p ) || char_is_hex_digit( *p ) ) )
	{
		++p;
	}

	if ( p >= e )
	{
		return adt_parse_number( node, base_str );
	}

	return base_str;
}

char* adt_parse_number( ADT_Node* node, char* base_str )
{
	GEN_ASSERT_NOT_NULL( node );
	GEN_ASSERT_NOT_NULL( base_str );
	char *p = base_str, *e = p;

	s32       base         = 0;
	s32       base2        = 0;
	u8        base2_offset = 0;
	s8        exp = 0, orig_exp = 0;
	u8        neg_zero   = 0;
	u8        lead_digit = 0;
	ADT_Type  node_type  = EADT_TYPE_UNINITIALISED;
	u8        node_props = 0;

	/* skip false positives and special cases */
	if ( ! ! char_first_occurence( "eE", *p ) || ( ! ! char_first_occurence( ".+-", *p ) && ! char_is_hex_digit( *( p + 1 ) ) && *( p + 1 ) != '.' ) )
	{
		return ++base_str;
	}

	node_type = EADT_TYPE_INTEGER;
	neg_zero  = false;

	ssize   ib        = 0;
	char buf[ 48 ] = { 0 };

	if ( *e == '+' )
		++e;
	else if ( *e == '-' )
	{
		buf[ ib++ ] = *e++;
	}

	if ( *e == '.' )
	{
		node_type   = EADT_TYPE_REAL;
		node_props  = EADT_PROPS_IS_PARSED_REAL;
		lead_digit  = false;
		buf[ ib++ ] = '0';
		do
		{
			buf[ ib++ ] = *e;
		} while ( char_is_digit( *++e ) );
	}
	else
	{
		if ( ! c_str_compare_len( e, "0x", 2 ) || ! c_str_compare_len( e, "0X", 2 ) )
		{
			node_props = EADT_PROPS_IS_HEX;
		}

		/* bail if ZPL_ADT_PROPS_IS_HEX is unset but we get 'x' on input */
		if ( char_to_lower( *e ) == 'x' && ( node_props != EADT_PROPS_IS_HEX ) )
		{
			return ++base_str;
		}

		while ( char_is_hex_digit( *e ) || char_to_lower( *e ) == 'x' )
		{
			buf[ ib++ ] = *e++;
		}

		if ( *e == '.' )
		{
			node_type  = EADT_TYPE_REAL;
			lead_digit = true;
			u32 step   = 0;

			do
			{
				buf[ ib++ ] = *e;
				++step;
			} while ( char_is_digit( *++e ) );

			if ( step < 2 )
			{
				buf[ ib++ ] = '0';
			}
		}
	}

	/* check if we have a dot here, this is a false positive (IP address, ...) */
	if ( *e == '.' )
	{
		return ++base_str;
	}

	f32  eb          = 10;
	char expbuf[ 6 ] = { 0 };
	ssize   expi        = 0;

	if ( *e && ! ! char_first_occurence( "eE", *e ) )
	{
		++e;
		if ( *e == '+' || *e == '-' || char_is_digit( *e ) )
		{
			if ( *e == '-' )
			{
				eb = 0.1f;
			}
			if ( ! char_is_digit( *e ) )
			{
				++e;
			}
			while ( char_is_digit( *e ) )
			{
				expbuf[ expi++ ] = *e++;
			}
		}

		orig_exp = exp = ( u8 )c_str_to_i64( expbuf, NULL, 10 );
	}

	if ( node_type == EADT_TYPE_INTEGER )
	{
		node->integer = c_str_to_i64( buf, 0, 0 );
#ifndef GEN_PARSER_DISABLE_ANALYSIS
		/* special case: negative zero */
		if ( node->integer == 0 && buf[ 0 ] == '-' )
		{
			neg_zero = true;
		}
#endif
		while ( orig_exp-- > 0 )
		{
			node->integer *= ( s64 )eb;
		}
	}
	else
	{
		node->real = c_str_to_f64( buf, 0 );

#ifndef GEN_PARSER_DISABLE_ANALYSIS
		char *q = buf, *base_string = q, *base_string2 = q;
		base_string           = ccast( char*, c_str_skip( base_string, '.' ));
		*base_string          = '\0';
		base_string2          = base_string + 1;
		char* base_strbuilder_off = base_string2;
		while ( *base_strbuilder_off++ == '0' )
			base2_offset++;

		base  = ( s32 )c_str_to_i64( q, 0, 0 );
		base2 = ( s32 )c_str_to_i64( base_string2, 0, 0 );
		if ( exp )
		{
			exp        = exp * ( ! ( eb == 10.0f ) ? -1 : 1 );
			node_props = EADT_PROPS_IS_EXP;
		}

		/* special case: negative zero */
		if ( base == 0 && buf[ 0 ] == '-' )
		{
			neg_zero = true;
		}
#endif
		while ( orig_exp-- > 0 )
		{
			node->real *= eb;
		}
	}

	node->type  = node_type;
	node->props = node_props;

#ifndef GEN_PARSER_DISABLE_ANALYSIS
	node->base         = base;
	node->base2        = base2;
	node->base2_offset = base2_offset;
	node->exp          = exp;
	node->neg_zero     = neg_zero;
	node->lead_digit   = lead_digit;
#else
	unused( base );
	unused( base2 );
	unused( base2_offset );
	unused( exp );
	unused( neg_zero );
	unused( lead_digit );
#endif
	return e;
}

ADT_Error adt_print_number( FileInfo* file, ADT_Node* node )
{
	GEN_ASSERT_NOT_NULL( file );
	GEN_ASSERT_NOT_NULL( node );
	if ( node->type != EADT_TYPE_INTEGER && node->type != EADT_TYPE_REAL )
	{
		return EADT_ERROR_INVALID_TYPE;
	}

#ifndef GEN_PARSER_DISABLE_ANALYSIS
	if ( node->neg_zero )
	{
		_adt_fprintf( file, "-" );
	}
#endif

	switch ( node->type )
	{
		case EADT_TYPE_INTEGER :
			{
				if ( node->props == EADT_PROPS_IS_HEX )
				{
					_adt_fprintf( file, "0x%llx", ( long long )node->integer );
				}
				else
				{
					_adt_fprintf( file, "%lld", ( long long )node->integer );
				}
			}
			break;

		case EADT_TYPE_REAL :
			{
				if ( node->props == EADT_PROPS_NAN )
				{
					_adt_fprintf( file, "NaN" );
				}
				else if ( node->props == EADT_PROPS_NAN_NEG )
				{
					_adt_fprintf( file, "-NaN" );
				}
				else if ( node->props == EADT_PROPS_INFINITY )
				{
					_adt_fprintf( file, "Infinity" );
				}
				else if ( node->props == EADT_PROPS_INFINITY_NEG )
				{
					_adt_fprintf( file, "-Infinity" );
				}
				else if ( node->props == EADT_PROPS_TRUE )
				{
					_adt_fprintf( file, "true" );
				}
				else if ( node->props == EADT_PROPS_FALSE )
				{
					_adt_fprintf( file, "false" );
				}
				else if ( node->props == EADT_PROPS_NULL )
				{
					_adt_fprintf( file, "null" );
#ifndef GEN_PARSER_DISABLE_ANALYSIS
				}
				else if ( node->props == EADT_PROPS_IS_EXP )
				{
					_adt_fprintf( file, "%lld.%0*d%llde%lld", ( long long )node->base, node->base2_offset, 0, ( long long )node->base2, ( long long )node->exp );
				}
				else if ( node->props == EADT_PROPS_IS_PARSED_REAL )
				{
					if ( ! node->lead_digit )
						_adt_fprintf( file, ".%0*d%lld", node->base2_offset, 0, ( long long )node->base2 );
					else
						_adt_fprintf( file, "%lld.%0*d%lld", ( long long int )node->base2_offset, 0, ( int )node->base, ( long long )node->base2 );
#endif
				}
				else
				{
					_adt_fprintf( file, "%f", node->real );
				}
			}
			break;
	}

	return EADT_ERROR_NONE;
}

ADT_Error adt_print_string( FileInfo* file, ADT_Node* node, char const* escaped_chars, char const* escape_symbol )
{
	GEN_ASSERT_NOT_NULL( file );
	GEN_ASSERT_NOT_NULL( node );
	GEN_ASSERT_NOT_NULL( escaped_chars );
	if ( node->type != EADT_TYPE_STRING && node->type != EADT_TYPE_MULTISTRING )
	{
		return EADT_ERROR_INVALID_TYPE;
	}

	/* escape string */
	char const *p = node->string, *b = p;

	if ( ! p )
		return EADT_ERROR_NONE;

	do
	{
		p = c_str_skip_any( p, escaped_chars );
		_adt_fprintf( file, "%.*s", pointer_diff( b, p ), b );
		if ( *p && ! ! char_first_occurence( escaped_chars, *p ) )
		{
			_adt_fprintf( file, "%s%c", escape_symbol, *p );
			p++;
		}
		b = p;
	} while ( *p );

	return EADT_ERROR_NONE;
}

ADT_Error adt_c_str_to_number( ADT_Node* node )
{
	GEN_ASSERT( node );

	if ( node->type == EADT_TYPE_REAL || node->type == EADT_TYPE_INTEGER )
		return EADT_ERROR_ALREADY_CONVERTED; /* this is already converted/parsed */
	if ( node->type != EADT_TYPE_STRING && node->type != EADT_TYPE_MULTISTRING )
	{
		return EADT_ERROR_INVALID_TYPE;
	}

	adt_parse_number( node, ( char* )node->string );

	return EADT_ERROR_NONE;
}

ADT_Error adt_c_str_to_number_strict( ADT_Node* node )
{
	GEN_ASSERT( node );

	if ( node->type == EADT_TYPE_REAL || node->type == EADT_TYPE_INTEGER )
		return EADT_ERROR_ALREADY_CONVERTED; /* this is already converted/parsed */
	if ( node->type != EADT_TYPE_STRING && node->type != EADT_TYPE_MULTISTRING )
	{
		return EADT_ERROR_INVALID_TYPE;
	}

	adt_parse_number_strict( node, ( char* )node->string );

	return EADT_ERROR_NONE;
}

#undef _adt_fprintf

#pragma endregion ADT

#pragma region CSV

#ifdef GEN_CSV_DEBUG
#	define GEN_CSV_ASSERT( msg ) GEN_PANIC( msg )
#else
#	define GEN_CSV_ASSERT( msg )
#endif

u8 csv_parse_delimiter( CSV_Object* root, char* text, AllocatorInfo allocator, b32 has_header, char delim )
{
	CSV_Error error = ECSV_Error__NONE;
	GEN_ASSERT_NOT_NULL( root );
	GEN_ASSERT_NOT_NULL( text );
	zero_item( root );

	adt_make_branch( root, allocator, NULL, has_header ? false : true );

	char* currentChar = text;
	char* beginChar;
	char* endChar;

	ssize columnIndex       = 0;
	ssize totalColumnIndex = 0;

	do
	{
		char delimiter = 0;
		currentChar = ccast( char*, c_str_trim( currentChar, false ));

		if ( *currentChar == 0 )
			break;

		ADT_Node rowItem = { 0 };
		rowItem.type     = EADT_TYPE_STRING;

	#ifndef GEN_PARSER_DISABLE_ANALYSIS
		rowItem.name_style = EADT_NAME_STYLE_NO_QUOTES;
	#endif

		/* handle string literals */
		if ( *currentChar == '"' )
		{
			currentChar   += 1;
			beginChar      = currentChar;
			endChar        = currentChar;
			rowItem.string = beginChar;
		#ifndef GEN_PARSER_DISABLE_ANALYSIS
			rowItem.name_style = EADT_NAME_STYLE_DOUBLE_QUOTE;
		#endif
			do
			{
				endChar = ccast( char*, c_str_skip( endChar, '"' ));

				if ( *endChar && *( endChar + 1 ) == '"' )
				{
					endChar += 2;
				}
				else
					break;
			}
			while ( *endChar );

			if ( *endChar == 0 )
			{
				GEN_CSV_ASSERT( "unmatched quoted string" );
				error = ECSV_Error__UNEXPECTED_END_OF_INPUT;
				return error;
			}

			*endChar    = 0;
			currentChar = ccast( char*, c_str_trim( endChar + 1, true ));
			delimiter   = * currentChar;

			/* unescape escaped quotes (so that unescaped text escapes :) */
			{
				char* escapedChar = beginChar;
				do
				{
					if ( *escapedChar == '"' && *( escapedChar + 1 ) == '"' )
					{
						mem_move( escapedChar, escapedChar + 1, c_str_len( escapedChar ) );
					}
					escapedChar++;
				}
				while ( *escapedChar );
			}
		}
		else if ( *currentChar == delim )
		{
			delimiter      = * currentChar;
			rowItem.string = "";
		}
		else if ( *currentChar )
		{
			/* regular data */
			beginChar      = currentChar;
			endChar        = currentChar;
			rowItem.string = beginChar;

			do
			{
				endChar++;
			}
			while ( * endChar && * endChar != delim && * endChar != '\n' );

			if ( * endChar )
			{
				currentChar = ccast( char*, c_str_trim( endChar, true ));

				while ( char_is_space( *( endChar - 1 ) ) )
				{
					endChar--;
				}

				delimiter = * currentChar;
				* endChar = 0;
			}
			else
			{
				delimiter   = 0;
				currentChar = endChar;
			}

			/* check if number and process if so */
			b32   skip_number = false;
			char* num_p       = beginChar;

			// We only consider hexadecimal values if they start with 0x
			if ( c_str_len(num_p) > 2 && num_p[0] == '0' && (num_p[1] == 'x' || num_p[1] == 'X') )
			{
				num_p += 2; // skip '0x' prefix
				do
				{
					if (!char_is_hex_digit(*num_p))
					{
						skip_number = true;
						break;
					}
				} while (*num_p++);
			}
			else
			{
				skip_number = true;
			}

			if (!skip_number)
			{
				adt_c_str_to_number(&rowItem);
			}
		}

		if ( columnIndex >= scast(ssize, array_num(root->nodes)) )
		{
			adt_append_arr( root, NULL );
		}

		array_append( root->nodes[ columnIndex ].nodes, rowItem );

		if ( delimiter == delim )
		{
			columnIndex++;
			currentChar++;
		}
		else if ( delimiter == '\n' || delimiter == 0 )
		{
			/* check if number of rows is not mismatched */
			if ( totalColumnIndex < columnIndex )
				totalColumnIndex = columnIndex;

			else if ( totalColumnIndex != columnIndex )
			{
				GEN_CSV_ASSERT( "mismatched rows" );
				error = ECSV_Error__MISMATCHED_ROWS;
				return error;
			}

			columnIndex = 0;

			if ( delimiter != 0 )
				currentChar++;
		}
	}
	while ( *currentChar );

	if (array_num( root->nodes) == 0 )
	{
		GEN_CSV_ASSERT( "unexpected end of input. stream is empty." );
		error = ECSV_Error__UNEXPECTED_END_OF_INPUT;
		return error;
	}

	/* consider first row as a header. */
	if ( has_header )
	{
		for ( ssize i = 0; i < scast(ssize, array_num(root->nodes)); i++ )
		{
			CSV_Object* col = root->nodes + i;
			CSV_Object* hdr = col->nodes;
			col->name       = hdr->string;
			array_remove_at(col->nodes, 0 );
		}
	}

	return error;
}

void csv_free( CSV_Object* obj )
{
	adt_destroy_branch( obj );
}

void _csv_write_record( FileInfo* file, CSV_Object* node )
{
	switch ( node->type )
	{
		case EADT_TYPE_STRING :
			{
#ifndef GEN_PARSER_DISABLE_ANALYSIS
				switch ( node->name_style )
				{
					case EADT_NAME_STYLE_DOUBLE_QUOTE :
						{
							c_str_fmt_file( file, "\"" );
							adt_print_string( file, node, "\"", "\"" );
							c_str_fmt_file( file, "\"" );
						}
						break;

					case EADT_NAME_STYLE_NO_QUOTES :
						{
#endif
							c_str_fmt_file( file, "%s", node->string );
#ifndef GEN_PARSER_DISABLE_ANALYSIS
						}
						break;
				}
#endif
			}
			break;

		case EADT_TYPE_REAL :
		case EADT_TYPE_INTEGER :
			{
				adt_print_number( file, node );
			}
			break;
	}
}

void _csv_write_header( FileInfo* file, CSV_Object* header )
{
	CSV_Object temp = *header;
	temp.string     = temp.name;
	temp.type       = EADT_TYPE_STRING;
	_csv_write_record( file, &temp );
}

void csv_write_delimiter( FileInfo* file, CSV_Object* obj, char delimiter )
{
	GEN_ASSERT_NOT_NULL( file );
	GEN_ASSERT_NOT_NULL( obj );
	GEN_ASSERT( obj->nodes );
	ssize cols = array_num(obj->nodes);
	if ( cols == 0 )
		return;

	ssize rows = array_num(obj->nodes[ 0 ].nodes);
	if ( rows == 0 )
		return;

	b32 has_headers = obj->nodes[ 0 ].name != NULL;

	if ( has_headers )
	{
		for ( ssize i = 0; i < cols; i++ )
		{
			_csv_write_header( file, &obj->nodes[ i ] );
			if ( i + 1 != cols )
			{
				c_str_fmt_file( file, "%c", delimiter );
			}
		}
		c_str_fmt_file( file, "\n" );
	}

	for ( ssize r = 0; r < rows; r++ )
	{
		for ( ssize i = 0; i < cols; i++ )
		{
			_csv_write_record( file, &obj->nodes[ i ].nodes[ r ] );
			if ( i + 1 != cols )
			{
				c_str_fmt_file( file, "%c", delimiter );
			}
		}
		c_str_fmt_file( file, "\n" );
	}
}

StrBuilder csv_write_strbuilder_delimiter( AllocatorInfo a, CSV_Object* obj, char delimiter )
{
	FileInfo tmp;
	file_stream_new( &tmp, a );
	csv_write_delimiter( &tmp, obj, delimiter );

	ssize  fsize;
	u8*    buf    = file_stream_buf( &tmp, &fsize );
	StrBuilder output = strbuilder_make_length( a, ( char* )buf, fsize );
	file_close( &tmp );
	return output;
}

#undef _adt_fprintf

#pragma endregion CSV
#pragma endregion Parsing

GEN_NS_END

// GEN_ROLL_OWN_DEPENDENCIES
#endif

GEN_NS_BEGIN

#pragma region StaticData
global Context* _ctx;

#pragma region Constants
global u32 context_counter;

global Str enum_underlying_sig;

global Code Code_Global;
global Code Code_Invalid;

global Code access_public;
global Code access_protected;
global Code access_private;

global CodeAttributes attrib_api_export;
global CodeAttributes attrib_api_import;

global Code module_global_fragment;
global Code module_private_fragment;

global Code fmt_newline;

global CodeParams param_varadic;

global CodePragma pragma_once;

global CodePreprocessCond preprocess_else;
global CodePreprocessCond preprocess_endif;

global CodeSpecifiers spec_const;
global CodeSpecifiers spec_consteval;
global CodeSpecifiers spec_constexpr;
global CodeSpecifiers spec_constinit;
global CodeSpecifiers spec_extern_linkage;
global CodeSpecifiers spec_final;
global CodeSpecifiers spec_forceinline;
global CodeSpecifiers spec_global;
global CodeSpecifiers spec_inline;
global CodeSpecifiers spec_internal_linkage;
global CodeSpecifiers spec_local_persist;
global CodeSpecifiers spec_mutable;
global CodeSpecifiers spec_noexcept;
global CodeSpecifiers spec_neverinline;
global CodeSpecifiers spec_override;
global CodeSpecifiers spec_ptr;
global CodeSpecifiers spec_pure;
global CodeSpecifiers spec_ref;
global CodeSpecifiers spec_register;
global CodeSpecifiers spec_rvalue;
global CodeSpecifiers spec_static_member;
global CodeSpecifiers spec_thread_local;
global CodeSpecifiers spec_virtual;
global CodeSpecifiers spec_volatile;

global CodeTypename t_empty;
global CodeTypename t_auto;
global CodeTypename t_void;
global CodeTypename t_int;
global CodeTypename t_bool;
global CodeTypename t_char;
global CodeTypename t_wchar_t;
global CodeTypename t_class;
global CodeTypename t_typename;

#ifdef GEN_DEFINE_LIBRARY_CODE_CONSTANTS
global CodeTypename t_b32;

global CodeTypename t_s8;
global CodeTypename t_s16;
global CodeTypename t_s32;
global CodeTypename t_s64;

global CodeTypename t_u8;
global CodeTypename t_u16;
global CodeTypename t_u32;
global CodeTypename t_u64;

global CodeTypename t_ssize;
global CodeTypename t_usize;

global CodeTypename t_f32;
global CodeTypename t_f64;
#endif

#pragma endregion Constants

#pragma endregion StaticData

#pragma region AST

// These macros are used in the swtich cases are used within ast.cpp, inteface.upfront.cpp, parser.cpp

#	define GEN_AST_BODY_CLASS_UNALLOWED_TYPES    \
	case CT_PlatformAttributes:                  \
	case CT_Class_Body:                          \
	case CT_Enum_Body:                           \
	case CT_Extern_Linkage:                      \
	case CT_Function_Body:                       \
	case CT_Function_Fwd:                        \
	case CT_Global_Body:                         \
	case CT_Namespace:                           \
	case CT_Namespace_Body:                      \
	case CT_Operator:                            \
	case CT_Operator_Fwd:                        \
	case CT_Parameters:                          \
	case CT_Specifiers:                          \
	case CT_Struct_Body:                         \
	case CT_Typename
#	define GEN_AST_BODY_STRUCT_UNALLOWED_TYPES GEN_AST_BODY_CLASS_UNALLOWED_TYPES

#	define GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES \
	case CT_Access_Public:                          \
	case CT_Access_Protected:                       \
	case CT_Access_Private:                         \
	case CT_PlatformAttributes:                     \
	case CT_Class_Body:                             \
	case CT_Enum_Body:                              \
	case CT_Extern_Linkage:                         \
	case CT_Friend:                                 \
	case CT_Function_Body:                          \
	case CT_Function_Fwd:                           \
	case CT_Global_Body:                            \
	case CT_Namespace:                              \
	case CT_Namespace_Body:                         \
	case CT_Operator:                               \
	case CT_Operator_Fwd:                           \
	case CT_Operator_Member:                        \
	case CT_Operator_Member_Fwd:                    \
	case CT_Parameters:                             \
	case CT_Specifiers:                             \
	case CT_Struct_Body:                            \
	case CT_Typename

#	define GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES     \
	case CT_Access_Public:                         \
	case CT_Access_Protected:                      \
	case CT_Access_Private:                        \
	case CT_PlatformAttributes:                    \
	case CT_Class_Body:                            \
	case CT_Enum_Body:                             \
	case CT_Execution:                             \
	case CT_Friend:                                \
	case CT_Function_Body:                         \
	case CT_Namespace_Body:                        \
	case CT_Operator_Member:                       \
	case CT_Operator_Member_Fwd:                   \
	case CT_Parameters:                            \
	case CT_Specifiers:                            \
	case CT_Struct_Body:                           \
	case CT_Typename
#	define GEN_AST_BODY_EXPORT_UNALLOWED_TYPES         GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES
#	define GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES

#	define GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES \
	case CT_Access_Public:                        \
	case CT_Access_Protected:                     \
	case CT_Access_Private:                       \
	case CT_PlatformAttributes:                   \
	case CT_Class_Body:                           \
	case CT_Enum_Body:                            \
	case CT_Execution:                            \
	case CT_Friend:                               \
	case CT_Function_Body:                        \
	case CT_Namespace_Body:                       \
	case CT_Operator_Member:                      \
	case CT_Operator_Member_Fwd:                  \
	case CT_Parameters:                           \
	case CT_Specifiers:                           \
	case CT_Struct_Body:                          \
	case CT_Typename

// This serializes all the data-members in a "debug" format, where each member is printed with its associated value.
Str code_debug_str(Code self)
{
	GEN_ASSERT(self != nullptr);
	StrBuilder  result_stack = strbuilder_make_reserve( _ctx->Allocator_Temp, kilobytes(1) );
	StrBuilder* result       = & result_stack;

	if ( self->Parent )
		strbuilder_append_fmt( result, "\n\tParent       : %S %S", code_type_str(self->Parent), self->Name.Len ? self->Name : txt("Null") );
	else
		strbuilder_append_fmt( result, "\n\tParent       : %S", txt("Null") );

	strbuilder_append_fmt( result, "\n\tName         : %S", self->Name.Len ? self->Name : txt("Null") );
	strbuilder_append_fmt( result, "\n\tType         : %S", code_type_str(self) );
	strbuilder_append_fmt( result, "\n\tModule Flags : %S", module_flag_to_str( self->ModuleFlags ) );

	switch ( self->Type )
	{
		case CT_Invalid:
		case CT_NewLine:
		case CT_Access_Private:
		case CT_Access_Protected:
		case CT_Access_Public:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
		break;

		case CT_Untyped:
		case CT_Execution:
		case CT_Comment:
		case CT_PlatformAttributes:
		case CT_Preprocess_Define:
		case CT_Preprocess_Include:
		case CT_Preprocess_Pragma:
		case CT_Preprocess_If:
		case CT_Preprocess_ElIf:
		case CT_Preprocess_Else:
		case CT_Preprocess_IfDef:
		case CT_Preprocess_IfNotDef:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tContent: %S", self->Content );
		break;

		case CT_Class:
		case CT_Struct:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt   : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes  : %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParentAccess: %S", self->ParentType ? access_spec_to_str( self->ParentAccess )                  : txt("No Parent") );
			strbuilder_append_fmt( result, "\n\tParentType  : %S", self->ParentType ? code_type_str(self->ParentType)                           : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody        : %S", self->Body       ? code_debug_str(self->Body)                                : txt("Null") );
		break;

		case CT_Class_Fwd:
		case CT_Struct_Fwd:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt   : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes  : %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParentAccess: %S", self->ParentType ? access_spec_to_str( self->ParentAccess )                  : txt("No Parent") );
			strbuilder_append_fmt( result, "\n\tParentType  : %S", self->ParentType ? code_type_str(self->ParentType)                           : txt("Null") );
		break;

		case CT_Constructor:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt      : %S", self->InlineCmt       ? self->InlineCmt->Content                                       : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs           ? strbuilder_to_str( code_to_strbuilder(self->Specs) )           : txt("Null") );
			strbuilder_append_fmt( result, "\n\tInitializerList: %S", self->InitializerList ? strbuilder_to_str( code_to_strbuilder(self->InitializerList) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParams         : %S", self->Params          ? strbuilder_to_str( code_to_strbuilder(self->Params) )          : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody           : %S", self->Body            ? code_debug_str(self->Body)                                     : txt("Null") );
		break;

		case CT_Constructor_Fwd:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt      : %S", self->InlineCmt       ? self->InlineCmt->Content                                       : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs           ? strbuilder_to_str( code_to_strbuilder(self->Specs) )           : txt("Null") );
			strbuilder_append_fmt( result, "\n\tInitializerList: %S", self->InitializerList ? strbuilder_to_str( code_to_strbuilder(self->InitializerList) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParams         : %S", self->Params          ? strbuilder_to_str( code_to_strbuilder(self->Params) )          : txt("Null") );
		break;

		case CT_Destructor:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt      : %S", self->InlineCmt       ? self->InlineCmt->Content                             : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs           ? strbuilder_to_str( code_to_strbuilder(self->Specs) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody           : %S", self->Body            ? code_debug_str(self->Body)                           : txt("Null") );
		break;

		case CT_Destructor_Fwd:
		break;

		case CT_Enum:
		case CT_Enum_Class:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt       : %S", self->InlineCmt      ? self->InlineCmt->Content                                     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes      : %S", self->Attributes     ? strbuilder_to_str( code_to_strbuilder(self->Attributes) )    : txt("Null") );
			strbuilder_append_fmt( result, "\n\tUnderlying Type : %S", self->UnderlyingType ? strbuilder_to_str( code_to_strbuilder(self->UnderlyingType)) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody            : %S", self->Body           ? code_debug_str(self->Body)                                   : txt("Null") );
		break;

		case CT_Enum_Fwd:
		case CT_Enum_Class_Fwd:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt       : %S", self->InlineCmt      ? self->InlineCmt->Content                                     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes      : %S", self->Attributes     ? strbuilder_to_str( code_to_strbuilder(self->Attributes) )    : txt("Null") );
			strbuilder_append_fmt( result, "\n\tUnderlying Type : %S", self->UnderlyingType ? strbuilder_to_str( code_to_strbuilder(self->UnderlyingType)) : txt("Null") );
		break;

		case CT_Extern_Linkage:
		case CT_Namespace:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tBody: %S", self->Body ? code_debug_str(self->Body) : txt("Null") );
		break;

		case CT_Friend:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt  : %S", self->InlineCmt   ? self->InlineCmt->Content                                  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tDeclaration: %S", self->Declaration ? strbuilder_to_str( code_to_strbuilder(self->Declaration)) : txt("Null") );
		break;

		case CT_Function:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? strbuilder_to_str( code_to_strbuilder(self->Specs))       : txt("Null") );
			strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? strbuilder_to_str( code_to_strbuilder(self->ReturnType))  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? strbuilder_to_str( code_to_strbuilder(self->Params))      : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? code_debug_str(self->Body)                                : txt("Null") );
		break;

		case CT_Function_Fwd:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? strbuilder_to_str( code_to_strbuilder(self->Specs))       : txt("Null") );
			strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? strbuilder_to_str( code_to_strbuilder(self->ReturnType))  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? strbuilder_to_str( code_to_strbuilder(self->Params))      : txt("Null") );
		break;

		case CT_Module:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
		break;

		case CT_Operator:
		case CT_Operator_Member:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? strbuilder_to_str( code_to_strbuilder(self->Specs))       : txt("Null") );
			strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? strbuilder_to_str( code_to_strbuilder(self->ReturnType))  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? strbuilder_to_str( code_to_strbuilder(self->Params))      : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? code_debug_str(self->Body)                                : txt("Null") );
			strbuilder_append_fmt( result, "\n\tOp        : %S", operator_to_str( self->Op ) );
		break;

		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? strbuilder_to_str( code_to_strbuilder(self->Specs) )      : txt("Null") );
			strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? strbuilder_to_str( code_to_strbuilder(self->ReturnType) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? strbuilder_to_str( code_to_strbuilder(self->Params) )     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tOp        : %S", operator_to_str( self->Op ) );
		break;

		case CT_Operator_Cast:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? strbuilder_to_str( code_to_strbuilder(self->Specs))     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tValueType : %S", self->ValueType  ? strbuilder_to_str( code_to_strbuilder(self->ValueType)) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? code_debug_str(self->Body)                              : txt("Null") );
		break;

		case CT_Operator_Cast_Fwd:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? strbuilder_to_str( code_to_strbuilder(self->Specs))     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tValueType : %S", self->ValueType  ? strbuilder_to_str( code_to_strbuilder(self->ValueType)) : txt("Null") );
		break;

		case CT_Parameters:
			strbuilder_append_fmt( result, "\n\tNumEntries: %d", self->NumEntries );
			strbuilder_append_fmt( result, "\n\tLast      : %S", self->Last->Name );
			strbuilder_append_fmt( result, "\n\tNext      : %S", self->Next->Name );
			strbuilder_append_fmt( result, "\n\tValueType : %S", self->ValueType ? strbuilder_to_str( code_to_strbuilder(self->ValueType)) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tValue     : %S", self->Value     ? strbuilder_to_str( code_to_strbuilder(self->Value))     : txt("Null") );
		break;

		case CT_Specifiers:
		{
			strbuilder_append_fmt( result, "\n\tNumEntries: %d", self->NumEntries );
			strbuilder_append_str( result, txt("\n\tArrSpecs: ") );

			s32 idx  = 0;
			s32 left = self->NumEntries;
			while ( left-- )
			{
				Str spec = spec_to_str( self->ArrSpecs[idx] );
				strbuilder_append_fmt( result, "%.*s, ", spec.Len, spec.Ptr );
				idx++;
			}
			strbuilder_append_fmt( result, "\n\tNextSpecs: %S", self->NextSpecs ? code_debug_str(self->NextSpecs) : txt("Null") );
		}
		break;

		case CT_Template:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tParams     : %S", self->Params      ? strbuilder_to_str( code_to_strbuilder(self->Params))      : txt("Null") );
			strbuilder_append_fmt( result, "\n\tDeclaration: %S", self->Declaration ? strbuilder_to_str( code_to_strbuilder(self->Declaration)) : txt("Null") );
		break;

		case CT_Typedef:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt     : %S", self->InlineCmt      ? self->InlineCmt->Content                                     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tUnderlyingType: %S", self->UnderlyingType ? strbuilder_to_str( code_to_strbuilder(self->UnderlyingType)) : txt("Null") );
		break;

		case CT_Typename:
			strbuilder_append_fmt( result, "\n\tAttributes     : %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs      ? strbuilder_to_str( code_to_strbuilder(self->Specs))       : txt("Null") );
			strbuilder_append_fmt( result, "\n\tReturnType     : %S", self->ReturnType ? strbuilder_to_str( code_to_strbuilder(self->ReturnType))  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tParams         : %S", self->Params     ? strbuilder_to_str( code_to_strbuilder(self->Params))      : txt("Null") );
			strbuilder_append_fmt( result, "\n\tArrExpr        : %S", self->ArrExpr    ? strbuilder_to_str( code_to_strbuilder(self->ArrExpr))     : txt("Null") );
		break;

		case CT_Union:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? strbuilder_to_str( code_to_strbuilder(self->Attributes) ) : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? code_debug_str(self->Body)                                : txt("Null") );
		break;

		case CT_Using:
			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt     : %S", self->InlineCmt      ? self->InlineCmt->Content                                      : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes    : %S", self->Attributes     ? strbuilder_to_str( code_to_strbuilder(self->Attributes) )     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tUnderlyingType: %S", self->UnderlyingType ? strbuilder_to_str( code_to_strbuilder(self->UnderlyingType))  : txt("Null") );
		break;

		case CT_Variable:

			if ( self->Parent && self->Parent->Type == CT_Variable )
			{
				// Its a NextVar
				strbuilder_append_fmt( result, "\n\tSpecs       : %S", self->Specs        ? strbuilder_to_str( code_to_strbuilder(self->Specs))        : txt("Null") );
				strbuilder_append_fmt( result, "\n\tValue       : %S", self->Value        ? strbuilder_to_str( code_to_strbuilder(self->Value))        : txt("Null") );
				strbuilder_append_fmt( result, "\n\tBitfieldSize: %S", self->BitfieldSize ? strbuilder_to_str( code_to_strbuilder(self->BitfieldSize)) : txt("Null") );
				strbuilder_append_fmt( result, "\n\tNextVar     : %S", self->NextVar      ? code_debug_str(self->NextVar)                              : txt("Null") );
				break;
			}

			if ( self->Prev )
				strbuilder_append_fmt( result, "\n\tPrev: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );
			if ( self->Next )
				strbuilder_append_fmt( result, "\n\tNext: %S %S", code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : txt("Null") );

			strbuilder_append_fmt( result, "\n\tInlineCmt   : %S", self->InlineCmt    ? self->InlineCmt->Content                                    : txt("Null") );
			strbuilder_append_fmt( result, "\n\tAttributes  : %S", self->Attributes   ? strbuilder_to_str( code_to_strbuilder(self->Attributes) )   : txt("Null") );
			strbuilder_append_fmt( result, "\n\tSpecs       : %S", self->Specs        ? strbuilder_to_str( code_to_strbuilder(self->Specs))         : txt("Null") );
			strbuilder_append_fmt( result, "\n\tValueType   : %S", self->ValueType    ? strbuilder_to_str( code_to_strbuilder(self->ValueType))     : txt("Null") );
			strbuilder_append_fmt( result, "\n\tBitfieldSize: %S", self->BitfieldSize ? strbuilder_to_str( code_to_strbuilder(self->BitfieldSize))  : txt("Null") );
			strbuilder_append_fmt( result, "\n\tValue       : %S", self->Value        ? strbuilder_to_str( code_to_strbuilder(self->Value))         : txt("Null") );
			strbuilder_append_fmt( result, "\n\tNextVar     : %S", self->NextVar      ? code_debug_str(self->NextVar)                               : txt("Null") );
		break;
	}

	return strbuilder_to_str( * result );
}

Code code_duplicate(Code self)
{
	Code result = make_code();

	void* mem_result = rcast(void*, cast(AST*, result));
	void* mem_self   = rcast(void*, cast(AST*, self));
	mem_copy( mem_result, mem_self, sizeof( AST ) );

	result->Parent = NullCode;
	return result;
}

StrBuilder code_to_strbuilder(Code self)
{
	StrBuilder result = strbuilder_make_str( _ctx->Allocator_Temp, txt("") );
	code_to_strbuilder_ptr( self, & result );
	return result;
}

void code_to_strbuilder_ptr( Code self, StrBuilder* result )
{
	GEN_ASSERT(self != nullptr);
	local_persist thread_local
	char SerializationLevel = 0;

	switch ( self->Type )
	{
		case CT_Invalid:
		#ifdef GEN_DONT_ALLOW_INVALID_CODE
			log_failure("Attempted to serialize invalid code! - %S", Parent ? Parent->code_debug_str() : Name );
		#else
			strbuilder_append_fmt( result, "Invalid Code!" );
		#endif
		break;

		case CT_NewLine:
			strbuilder_append_str( result, txt("\n"));
		break;

		case CT_Untyped:
		case CT_Execution:
		case CT_Comment:
		case CT_PlatformAttributes:
			strbuilder_append_str( result, self->Content );
		break;

		case CT_Access_Private:
		case CT_Access_Protected:
		case CT_Access_Public:
			strbuilder_append_str( result, self->Name );
		break;

		case CT_Class:
			class_to_strbuilder_def(cast(CodeClass, self), result );
		break;

		case CT_Class_Fwd:
			class_to_strbuilder_fwd(cast(CodeClass, self), result );
		break;

		case CT_Constructor:
			constructor_to_strbuilder_def(cast(CodeConstructor, self), result );
		break;

		case CT_Constructor_Fwd:
			constructor_to_strbuilder_fwd(cast(CodeConstructor, self), result );
		break;

		case CT_Destructor:
			destructor_to_strbuilder_def(cast(CodeDestructor, self), result );
		break;

		case CT_Destructor_Fwd:
			destructor_to_strbuilder_fwd(cast(CodeDestructor, self), result );
		break;

		case CT_Enum:
			enum_to_strbuilder_def(cast(CodeEnum, self), result );
		break;

		case CT_Enum_Fwd:
			enum_to_strbuilder_fwd(cast(CodeEnum, self), result );
		break;

		case CT_Enum_Class:
			enum_to_strbuilder_class_def(cast(CodeEnum, self), result );
		break;

		case CT_Enum_Class_Fwd:
			enum_to_strbuilder_class_fwd(cast(CodeEnum, self), result );
		break;

		case CT_Export_Body:
			body_to_strbuilder_export(cast(CodeBody, self), result );
		break;

		case CT_Extern_Linkage:
			extern_to_strbuilder(cast(CodeExtern, self), result );
		break;

		case CT_Friend:
			friend_to_strbuilder_ref(cast(CodeFriend, self), result );
		break;

		case CT_Function:
			fn_to_strbuilder_def(cast(CodeFn, self), result );
		break;

		case CT_Function_Fwd:
			fn_to_strbuilder_fwd(cast(CodeFn, self), result );
		break;

		case CT_Module:
			module_to_strbuilder_ref(cast(CodeModule, self), result );
		break;

		case CT_Namespace:
			namespace_to_strbuilder_ref(cast(CodeNS, self), result );
		break;

		case CT_Operator:
		case CT_Operator_Member:
			code_op_to_strbuilder_def(cast(CodeOperator, self), result );
		break;

		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
			code_op_to_strbuilder_fwd(cast(CodeOperator, self), result );
		break;

		case CT_Operator_Cast:
			opcast_to_strbuilder_def(cast(CodeOpCast, self), result );
		break;

		case CT_Operator_Cast_Fwd:
			opcast_to_strbuilder_fwd(cast(CodeOpCast, self), result );
		break;

		case CT_Parameters:
			params_to_strbuilder_ref(cast(CodeParams, self), result );
		break;

		case CT_Preprocess_Define:
			define_to_strbuilder_ref(cast(CodeDefine, self), result );
		break;

		case CT_Preprocess_If:
			preprocess_to_strbuilder_if(cast(CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_IfDef:
			preprocess_to_strbuilder_ifdef(cast(CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_IfNotDef:
			preprocess_to_strbuilder_ifndef(cast(CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_Include:
			include_to_strbuilder_ref(cast(CodeInclude, self), result );
		break;

		case CT_Preprocess_ElIf:
			preprocess_to_strbuilder_elif(cast(CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_Else:
			preprocess_to_strbuilder_else(cast(CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_EndIf:
			preprocess_to_strbuilder_endif(cast(CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_Pragma:
			pragma_to_strbuilder_ref(cast(CodePragma, self), result );
		break;

		case CT_Specifiers:
			specifiers_to_strbuilder_ref(cast(CodeSpecifiers, self), result );
		break;

		case CT_Struct:
			struct_to_strbuilder_def(cast(CodeStruct, self), result );
		break;

		case CT_Struct_Fwd:
			struct_to_strbuilder_fwd(cast(CodeStruct, self), result );
		break;

		case CT_Template:
			template_to_strbuilder_ref(cast(CodeTemplate, self), result );
		break;

		case CT_Typedef:
			typedef_to_strbuilder_ref(cast(CodeTypedef, self), result );
		break;

		case CT_Typename:
			typename_to_strbuilder_ref(cast(CodeTypename, self), result );
		break;

		case CT_Union:
			union_to_strbuilder_def( cast(CodeUnion, self), result );
		break;

		case CT_Union_Fwd:
			union_to_strbuilder_fwd( cast(CodeUnion, self), result );
		break;

		case CT_Using:
			using_to_strbuilder_ref(cast(CodeUsing, self), result );
		break;

		case CT_Using_Namespace:
			using_to_strbuilder_ns(cast(CodeUsing, self), result );
		break;

		case CT_Variable:
			var_to_strbuilder_ref(cast(CodeVar, self), result );
		break;

		case CT_Enum_Body:
		case CT_Class_Body:
		case CT_Extern_Linkage_Body:
		case CT_Function_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
			body_to_strbuilder_ref( cast(CodeBody, self), result );
		break;
	}
}

bool code_is_equal( Code self, Code other )
{
/*
	AST values are either some u32 value, a cached string, or a pointer to another AST.

	u32 values are compared by value.
	Cached strings are compared by pointer.
	AST nodes are compared with AST::is_equal.
*/
	if ( other == nullptr )
	{
		log_fmt( "AST::is_equal: other is null\nAST: %S", code_debug_str(self) );
		return false;
	}

	if ( self->Type != other->Type )
	{
		log_fmt("AST::is_equal: Type check failure with other\nAST: %S\nOther: %S"
			, code_debug_str(self)
			, code_debug_str(other)
		);

		return false;
	}

	switch ( self->Type )
	{
	#define check_member_val( val )                           \
	if ( self->val != other->val )                            \
	{                                                         \
		log_fmt("\nAST::is_equal: Member - " #val " failed\n" \
		        "AST  : %S\n"                                 \
		        "Other: %S\n"                                 \
		    , code_debug_str(self)                            \
		    ,code_debug_str(other)                            \
		);                                                    \
                                                              \
		return false;                                         \
	}

	#define check_member_str( str )                                 \
	if ( ! str_are_equal( self->str, other->str ) )                 \
	{                                                               \
		log_fmt("\nAST::is_equal: Member string - "#str " failed\n" \
				"AST  : %S\n"                                       \
				"Other: %S\n"                                       \
			, code_debug_str(self)                                  \
			,code_debug_str(other)                                  \
		);                                                          \
	                                                                \
		return false;                                               \
	}

	#define check_member_content( content )                                  \
	if ( ! str_are_equal( self->content, other->content ))                   \
	{                                                                        \
		log_fmt("\nAST::is_equal: Member content - "#content " failed\n"     \
				"AST  : %S\n"                                                \
				"Other: %S\n"                                                \
			, code_debug_str(self)                                           \
			, code_debug_str(other)                                          \
		);                                                                   \
                                                                             \
		log_fmt("Content cannot be trusted to be unique with this check "    \
			"so it must be verified by eye for now\n"                        \
			"AST   Content:\n%S\n"                                           \
			"Other Content:\n%S\n"                                           \
			, str_visualize_whitespace(self->content, _ctx->Allocator_Temp)  \
			, str_visualize_whitespace(other->content, _ctx->Allocator_Temp) \
		);                                                                   \
	}

	#define check_member_ast( ast )                                                                \
	if ( self->ast )                                                                               \
	{                                                                                              \
		if ( other->ast == nullptr )                                                               \
		{                                                                                          \
			log_fmt("\nAST::is_equal: Failed for member " #ast " other equivalent param is null\n" \
					"AST  : %S\n"                                                                  \
					"Other: %S\n"                                                                  \
					"For ast member: %S\n"                                                         \
				, code_debug_str(self)                                                             \
				, code_debug_str(other)                                                            \
				, code_debug_str(self->ast)                                                        \
			);                                                                                     \
                                                                                                   \
			return false;                                                                          \
		}                                                                                          \
                                                                                                   \
		if ( ! code_is_equal(self->ast, other->ast ) )                                             \
		{                                                                                          \
			log_fmt( "\nAST::is_equal: Failed for " #ast"\n"                                       \
					"AST  : %S\n"                                                                  \
					"Other: %S\n"                                                                  \
					"For     ast member: %S\n"                                                     \
					"other's ast member: %S\n"                                                     \
				, code_debug_str(self)                                                             \
				, code_debug_str(other)                                                            \
				, code_debug_str(self->ast)                                                        \
				, code_debug_str(other->ast)                                                       \
			);                                                                                     \
		                                                                                           \
			return false;                                                                          \
		}                                                                                          \
	}

		case CT_NewLine:
		case CT_Access_Public:
		case CT_Access_Protected:
		case CT_Access_Private:
		case CT_Preprocess_Else:
		case CT_Preprocess_EndIf:
			return true;


		// Comments are not validated.
		case CT_Comment:
			return true;

		case CT_Execution:
		case CT_PlatformAttributes:
		case CT_Untyped:
		{
			check_member_content( Content );
			return true;
		}

		case CT_Class_Fwd:
		case CT_Struct_Fwd:
		{
			check_member_str( Name );
			check_member_ast( ParentType );
			check_member_val( ParentAccess );
			check_member_ast( Attributes );

			return true;
		}

		case CT_Class:
		case CT_Struct:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ParentType );
			check_member_val( ParentAccess );
			check_member_ast( Attributes );
			check_member_ast( Body );

			return true;
		}

		case CT_Constructor:
		{
			check_member_ast( InitializerList );
			check_member_ast( Params );
			check_member_ast( Body );

			return true;
		}

		case CT_Constructor_Fwd:
		{
			check_member_ast( InitializerList );
			check_member_ast( Params );

			return true;
		}

		case CT_Destructor:
		{
			check_member_ast( Specs );
			check_member_ast( Body );

			return true;
		}

		case CT_Destructor_Fwd:
		{
			check_member_ast( Specs );

			return true;
		}

		case CT_Enum:
		case CT_Enum_Class:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
			check_member_ast( UnderlyingType );
			check_member_ast( Body );
			check_member_ast( UnderlyingTypeMacro );

			return true;
		}

		case CT_Enum_Fwd:
		case CT_Enum_Class_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
			check_member_ast( UnderlyingType );
			check_member_ast( UnderlyingTypeMacro );

			return true;
		}

		case CT_Extern_Linkage:
		{
			check_member_str( Name );
			check_member_ast( Body );

			return true;
		}

		case CT_Friend:
		{
			check_member_str( Name );
			check_member_ast( Declaration );

			return true;
		}

		case CT_Function:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );
			check_member_ast( Body );

			return true;
		}

		case CT_Function_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );

			return true;
		}

		case CT_Module:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );

			return true;
		}

		case CT_Namespace:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Body );

			return true;
		}

		case CT_Operator:
		case CT_Operator_Member:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );
			check_member_ast( Body );

			return true;
		}

		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );

			return true;
		}

		case CT_Operator_Cast:
		{
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( ValueType );
			check_member_ast( Body );

			return true;
		}

		case CT_Operator_Cast_Fwd:
		{
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( ValueType );

			return true;
		}

		case CT_Parameters:
		{
			if ( self->NumEntries > 1 )
			{
				Code curr       = self;
				Code curr_other = other;
				while ( curr != nullptr  )
				{
					if ( curr )
					{
						if ( curr_other == nullptr )
						{
							log_fmt("\nAST::is_equal: Failed for parameter, other equivalent param is null\n"
									"AST  : %S\n"
									"Other: %S\n"
									"For ast member: %S\n"
								, code_debug_str(curr)
							);

							return false;
						}

						if ( str_are_equal(curr->Name, curr_other->Name) )
						{
							log_fmt( "\nAST::is_equal: Failed for parameter name check\n"
									"AST  : %S\n"
									"Other: %S\n"
									"For     ast member: %S\n"
									"other's ast member: %S\n"
								, code_debug_str(self)
								, code_debug_str(other)
								, code_debug_str(curr)
								, code_debug_str(curr_other)
							);
							return false;
						}

						if ( curr->ValueType && ! code_is_equal(curr->ValueType, curr_other->ValueType) )
						{
							log_fmt( "\nAST::is_equal: Failed for parameter value type check\n"
									"AST  : %S\n"
									"Other: %S\n"
									"For     ast member: %S\n"
									"other's ast member: %S\n"
								, code_debug_str(self)
								, code_debug_str(other)
								, code_debug_str(curr)
								, code_debug_str(curr_other)
							);
							return false;
						}

						if ( curr->Value && ! code_is_equal(curr->Value, curr_other->Value) )
						{
							log_fmt( "\nAST::is_equal: Failed for parameter value check\n"
									"AST  : %S\n"
									"Other: %S\n"
									"For     ast member: %S\n"
									"other's ast member: %S\n"
								, code_debug_str(self)
								, code_debug_str(other)
								, code_debug_str(curr)
								, code_debug_str(curr_other)
							);
							return false;
						}
					}

					curr       = curr->Next;
					curr_other = curr_other->Next;
				}

				check_member_val( NumEntries );

				return true;
			}

			check_member_str( Name );
			check_member_ast( ValueType );
			check_member_ast( Value );
			check_member_ast( ArrExpr );

			return true;
		}

		case CT_Preprocess_Define:
		{
			check_member_str( Name );
			check_member_content( Content );

			return true;
		}

		case CT_Preprocess_If:
		case CT_Preprocess_IfDef:
		case CT_Preprocess_IfNotDef:
		case CT_Preprocess_ElIf:
		{
			check_member_content( Content );

			return true;
		}

		case CT_Preprocess_Include:
		case CT_Preprocess_Pragma:
		{
			check_member_content( Content );

			return true;
		}

		case CT_Specifiers:
		{
			check_member_val( NumEntries );
			check_member_str( Name );
			for ( s32 idx = 0; idx < self->NumEntries; ++idx )
			{
				check_member_val( ArrSpecs[ idx ] );
			}
			return true;
		}

		case CT_Template:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Params );
			check_member_ast( Declaration );

			return true;
		}

		case CT_Typedef:
		{
			check_member_val( IsFunction );
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( UnderlyingType );

			return true;
		}
		case CT_Typename:
		{
			check_member_val( IsParamPack );
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( ArrExpr );

			return true;
		}

		case CT_Union:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
			check_member_ast( Body );

			return true;
		}

		case CT_Union_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
		}

		case CT_Using:
		case CT_Using_Namespace:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( UnderlyingType );
			check_member_ast( Attributes );

			return true;
		}

		case CT_Variable:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ValueType );
			check_member_ast( BitfieldSize );
			check_member_ast( Value );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( NextVar );

			return true;
		}

		case CT_Class_Body:
		case CT_Enum_Body:
		case CT_Export_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
		{
			check_member_ast( Front );
			check_member_ast( Back );

			Code curr       = self->Front;
			Code curr_other = other->Front;
			while ( curr != nullptr )
			{
				if ( curr_other == nullptr )
				{
					log_fmt("\nAST::is_equal: Failed for body, other equivalent param is null\n"
							"AST  : %S\n"
							"Other: %S\n"
						, code_debug_str(curr)
						, code_debug_str(other)
					);

					return false;
				}

				if ( ! code_is_equal( curr, curr_other ) )
				{
					log_fmt( "\nAST::is_equal: Failed for body\n"
							"AST  : %S\n"
							"Other: %S\n"
							"For     ast member: %S\n"
							"other's ast member: %S\n"
						, code_debug_str(self)
						, code_debug_str(other)
						, code_debug_str(curr)
						, code_debug_str(curr_other)
					);

					return false;
				}

				curr       = curr->Next;
				curr_other = curr_other->Next;
			}

			check_member_val( NumEntries );

			return true;
		}

	#undef check_member_val
	#undef check_member_str
	#undef check_member_ast
	}

	return true;
}

bool code_validate_body(Code self)
{
	switch ( self->Type )
	{
		case CT_Class_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for (Code code_entry = begin_CodeBody(body); code_entry != end_CodeBody(body); next_CodeBody(body, code_entry)) switch (code_entry->Type)
			{
				GEN_AST_BODY_CLASS_UNALLOWED_TYPES:
					log_failure("AST::validate_body: Invalid entry in body %S", code_debug_str(code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Enum_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for ( Code entry = begin_CodeBody(body); entry != end_CodeBody(body); next_CodeBody(body, entry) )
			{
				if ( entry->Type != CT_Untyped )
				{
					log_failure( "AST::validate_body: Invalid entry in enum body (needs to be untyped or comment) %S", code_debug_str(entry) );
					return false;
				}
			}
		}
		break;
		case CT_Export_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for (Code code_entry = begin_CodeBody(body); code_entry != end_CodeBody(body); next_CodeBody(body, code_entry)) switch (code_entry->Type)
			{
				GEN_AST_BODY_EXPORT_UNALLOWED_TYPES:
					log_failure("AST::validate_body: Invalid entry in body %S", code_debug_str(code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Extern_Linkage:
		{
			CodeBody body = cast(CodeBody, self);
			for (Code code_entry = begin_CodeBody(body); code_entry != end_CodeBody(body); next_CodeBody(body, code_entry)) switch (code_entry->Type)
			{
				GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES:
					log_failure("AST::validate_body: Invalid entry in body %S", code_debug_str(code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Function_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for (Code code_entry = begin_CodeBody(body); code_entry != end_CodeBody(body); next_CodeBody(body, code_entry)) switch (code_entry->Type)
			{
				GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES:
					log_failure("AST::validate_body: Invalid entry in body %S", code_debug_str(code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Global_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for ( Code entry = begin_CodeBody(body); entry != end_CodeBody(body); next_CodeBody(body, entry) )switch (entry->Type)
			{
				GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES:
					log_failure("AST::validate_body: Invalid entry in body %S", code_debug_str(entry));
				return false;
			}
		}
		break;
		case CT_Namespace_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for ( Code entry = begin_CodeBody(body); entry != end_CodeBody(body); next_CodeBody(body, entry) ) switch (entry->Type)
			{
				GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES:
					log_failure("AST::validate_body: Invalid entry in body %S", code_debug_str(entry));
				return false;
			}
		}
		break;
		case CT_Struct_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for ( Code entry = begin_CodeBody(body); entry != end_CodeBody(body); next_CodeBody(body, entry) ) switch (entry->Type)
			{
				GEN_AST_BODY_STRUCT_UNALLOWED_TYPES:
					log_failure("AST::validate_body: Invalid entry in body %S", code_debug_str(entry));
				return false;
			}
		}
		break;
		case CT_Union_Body:
		{
			CodeBody body = cast(CodeBody, self);
			for ( Code entry = begin_CodeBody(body); entry != end_CodeBody(body); next_CodeBody(body, entry) )
			{
				if ( entry->Type != CT_Untyped )
				{
					log_failure( "AST::validate_body: Invalid entry in union body (needs to be untyped or comment) %S", code_debug_str(entry) );
					return false;
				}
			}
		}
		break;

		default:
			log_failure( "AST::validate_body: Invalid this AST does not have a body %S", code_debug_str(self) );
			return false;
	}
	return false;
}

inline
StrBuilder attributes_to_strbuilder(CodeAttributes attributes) {
	GEN_ASSERT(attributes);
	char* raw = ccast(char*, str_duplicate( attributes->Content, _ctx->Allocator_Temp ).Ptr);
	StrBuilder result = { raw };
	return result;
}

inline
void attributes_to_strbuilder_ref(CodeAttributes attributes, StrBuilder* result) {
	GEN_ASSERT(attributes);
	GEN_ASSERT(result);
	strbuilder_append_str(result, attributes->Content);
}

StrBuilder body_to_strbuilder(CodeBody body)
{
	GEN_ASSERT(body);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 128 );
	switch ( body->Type )
	{
		case CT_Untyped:
		case CT_Execution:
			strbuilder_append_str( & result, cast(Code, body)->Content );
		break;

		case CT_Enum_Body:
		case CT_Class_Body:
		case CT_Extern_Linkage_Body:
		case CT_Function_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
			body_to_strbuilder_ref( body, & result );
		break;

		case CT_Export_Body:
			body_to_strbuilder_export( body, & result );
		break;
	}
	return result;
}

void body_to_strbuilder_ref( CodeBody body, StrBuilder* result )
{
	GEN_ASSERT(body   != nullptr);
	GEN_ASSERT(result != nullptr);
	Code curr = body->Front;
	s32  left = body->NumEntries;
	while ( left -- )
	{
		code_to_strbuilder_ptr(curr, result);
		// strbuilder_append_fmt( result, "%SB", code_to_strbuilder(curr) );
		++curr;
	}
}

void body_to_strbuilder_export( CodeBody body, StrBuilder* result )
{
	GEN_ASSERT(body   != nullptr);
	GEN_ASSERT(result != nullptr);
	strbuilder_append_fmt( result, "export\n{\n" );

	Code curr = cast(Code, body);
	s32  left = body->NumEntries;
	while ( left-- )
	{
		code_to_strbuilder_ptr(curr, result);
		// strbuilder_append_fmt( result, "%SB", code_to_strbuilder(curr) );
		++curr;
	}

	strbuilder_append_fmt( result, "};\n" );
}

inline
StrBuilder comment_to_strbuilder(CodeComment comment) {
	GEN_ASSERT(comment);
	char* raw = ccast(char*, str_duplicate( comment->Content, _ctx->Allocator_Temp ).Ptr);
	StrBuilder result = { raw };
	return result;
}

inline
void comment_to_strbuilder_ref(CodeComment comment, StrBuilder* result) {
	GEN_ASSERT(comment);
	GEN_ASSERT(result);
	strbuilder_append_str(result, comment->Content);
}

StrBuilder constructor_to_strbuilder(CodeConstructor self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 128 );
	switch (self->Type)
	{
		case CT_Constructor:
			constructor_to_strbuilder_def( self, & result );
		break;
		case CT_Constructor_Fwd:
			constructor_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void constructor_to_strbuilder_def(CodeConstructor self, StrBuilder* result )
{
	Code ClassStructParent = self->Parent->Parent;
	if (ClassStructParent) {
		strbuilder_append_str( result, ClassStructParent->Name );
	}
	else {
		strbuilder_append_str( result, self->Name );
	}

	if ( self->Params )
		strbuilder_append_fmt( result, "( %SB )", params_to_strbuilder(self->Params) );
	else
		strbuilder_append_str( result, txt("()") );

	if ( self->InitializerList )
		strbuilder_append_fmt( result, " : %SB", code_to_strbuilder(self->InitializerList) );

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, " // %S", self->InlineCmt->Content );

	strbuilder_append_fmt( result, "\n{\n%SB\n}\n", code_to_strbuilder(self->Body) );
}

void constructor_to_strbuilder_fwd(CodeConstructor self, StrBuilder* result )
{
	Code ClassStructParent = self->Parent->Parent;
	if (ClassStructParent) {
		strbuilder_append_str( result, ClassStructParent->Name );
	}
	else {
		strbuilder_append_str( result, self->Name );
	}

	if ( self->Params )
		strbuilder_append_fmt( result, "( %SB )", params_to_strbuilder(self->Params) );
	else
		strbuilder_append_fmt( result, "()");

	if (self->Body)
		strbuilder_append_fmt( result, " = %SB", code_to_strbuilder(self->Body) );

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "; // %S\n", self->InlineCmt->Content );
	else
		strbuilder_append_str( result, txt(";\n") );
}

StrBuilder class_to_strbuilder( CodeClass self )
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Class:
			class_to_strbuilder_def(self, & result );
		break;
		case CT_Class_Fwd:
			class_to_strbuilder_fwd(self, & result );
		break;
	}
	return result;
}

void class_to_strbuilder_def( CodeClass self, StrBuilder* result )
{
	GEN_ASSERT(self);

	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	strbuilder_append_str( result, txt("class ") );

	if ( self->Attributes )
	{
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );
	}

	if ( self->ParentType )
	{
		Str access_level = access_spec_to_str( self->ParentAccess );
		strbuilder_append_fmt( result, "%S : %S %SB", self->Name, access_level, typename_to_strbuilder(self->ParentType) );

		CodeTypename interface = cast(CodeTypename, self->ParentType->Next);
		if ( interface )
			strbuilder_append_str( result, txt("\n") );

		while ( interface )
		{
			strbuilder_append_fmt( result, ", public %SB", typename_to_strbuilder(interface) );
			interface = interface->Next ? cast(CodeTypename, interface->Next) : NullCode;
		}
	}
	else if ( self->Name.Len )
	{
		strbuilder_append_str( result, self->Name );
	}

	if ( self->InlineCmt )
	{
		strbuilder_append_fmt( result, " // %S", self->InlineCmt->Content );
	}

	strbuilder_append_fmt( result, "\n{\n%SB\n}", body_to_strbuilder(self->Body) );

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		strbuilder_append_str( result, txt(";\n") );
}

void class_to_strbuilder_fwd( CodeClass self, StrBuilder* result )
{
	GEN_ASSERT(self);

	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "class %SB %S", attributes_to_strbuilder(self->Attributes), self->Name );

	else strbuilder_append_fmt( result, "class %S", self->Name );

	// Check if it can have an end-statement
	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			strbuilder_append_fmt( result, "; // %S\n", self->InlineCmt->Content );
		else
			strbuilder_append_str( result, txt(";\n") );
	}
}

StrBuilder define_to_strbuilder(CodeDefine define)
{
	return strbuilder_fmt_buf( _ctx->Allocator_Temp, "#define %S %S", define->Name, define->Content );
}

void define_to_strbuilder_ref(CodeDefine define, StrBuilder* result )
{
	strbuilder_append_fmt( result, "#define %S %S", define->Name, define->Content );
}

StrBuilder destructor_to_strbuilder(CodeDestructor self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 128 );
	switch ( self->Type )
	{
		case CT_Destructor:
			destructor_to_strbuilder_def( self, & result );
		break;
		case CT_Destructor_Fwd:
			destructor_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void destructor_to_strbuilder_def(CodeDestructor self, StrBuilder* result )
{
	if ( self->Name.Len )
	{
		strbuilder_append_fmt( result, "%S()", self->Name );
	}
	else if ( self->Specs )
	{
		if ( specifiers_has(self->Specs, Spec_Virtual ) )
			strbuilder_append_fmt( result, "virtual ~%S()", self->Parent->Name );
		else
			strbuilder_append_fmt( result, "~%S()", self->Parent->Name );
	}
	else
		strbuilder_append_fmt( result, "~%S()", self->Parent->Name );

	strbuilder_append_fmt( result, "\n{\n%SB\n}\n", code_to_strbuilder(self->Body) );
}

void destructor_to_strbuilder_fwd(CodeDestructor self, StrBuilder* result )
{
	if ( self->Specs )
	{
		if ( specifiers_has(self->Specs, Spec_Virtual ) )
			strbuilder_append_fmt( result, "virtual ~%S();\n", self->Parent->Name );
		else
			strbuilder_append_fmt( result, "~%S()", self->Parent->Name );

		if ( specifiers_has(self->Specs, Spec_Pure ) )
			strbuilder_append_str( result, txt(" = 0;") );
		else if (self->Body)
			strbuilder_append_fmt( result, " = %SB;", code_to_strbuilder(self->Body) );
	}
	else
		strbuilder_append_fmt( result, "~%S();", self->Parent->Name );

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "  %S", self->InlineCmt->Content );
	else
		strbuilder_append_str( result, txt("\n"));
}

StrBuilder enum_to_strbuilder(CodeEnum self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Enum:
			enum_to_strbuilder_def(self, & result );
		break;
		case CT_Enum_Fwd:
			enum_to_strbuilder_fwd(self, & result );
		break;
		case CT_Enum_Class:
			enum_to_strbuilder_class_def(self, & result );
		break;
		case CT_Enum_Class_Fwd:
			enum_to_strbuilder_class_fwd(self, & result );
		break;
	}
	return result;
}

void enum_to_strbuilder_def(CodeEnum self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes || self->UnderlyingType || self->UnderlyingTypeMacro )
	{
		strbuilder_append_str( result, txt("enum ") );

		if ( self->Attributes )
			strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

		if ( self->UnderlyingType )
			strbuilder_append_fmt( result, "%S : %SB\n{\n%SB\n}"
				, self->Name
				, typename_to_strbuilder(self->UnderlyingType)
				, body_to_strbuilder(self->Body)
			);
		else if ( self->UnderlyingTypeMacro )
			strbuilder_append_fmt( result, "%S %SB\n{\n%SB\n}"
				, self->Name
				, code_to_strbuilder(self->UnderlyingTypeMacro)
				, body_to_strbuilder(self->Body)
			);

		else strbuilder_append_fmt( result, "%S\n{\n%SB\n}", self->Name, body_to_strbuilder(self->Body) );
	}
	else strbuilder_append_fmt( result, "enum %S\n{\n%SB\n}", self->Name, body_to_strbuilder(self->Body) );

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		strbuilder_append_str( result, txt(";\n"));
}

void enum_to_strbuilder_fwd(CodeEnum self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	if ( self->UnderlyingType )
		strbuilder_append_fmt( result, "enum %S : %SB", self->Name, typename_to_strbuilder(self->UnderlyingType) );
	else if (self->UnderlyingTypeMacro)
	{
		log_fmt("IDENTIFIED A UNDERLYING ENUM MACRO");
		strbuilder_append_fmt( result, "enum %S %SB", self->Name, code_to_strbuilder(self->UnderlyingTypeMacro) );
	}
	else
		strbuilder_append_fmt( result, "enum %S", self->Name );

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			strbuilder_append_str( result, txt(";\n"));
	}
}

void enum_to_strbuilder_class_def(CodeEnum self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes || self->UnderlyingType )
	{
		strbuilder_append_str( result, txt("enum class ") );

		if ( self->Attributes )
		{
			strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );
		}

		if ( self->UnderlyingType )
		{
			strbuilder_append_fmt( result, "%S : %SB\n{\n%SB\n}", self->Name, typename_to_strbuilder(self->UnderlyingType), body_to_strbuilder(self->Body) );
		}
		else
		{
			strbuilder_append_fmt( result, "%S\n{\n%SB\n}", self->Name, body_to_strbuilder(self->Body) );
		}
	}
	else
	{
		strbuilder_append_fmt( result, "enum %S\n{\n%SB\n}", self->Name, body_to_strbuilder(self->Body) );
	}

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		strbuilder_append_str( result, txt(";\n"));
}

void enum_to_strbuilder_class_fwd(CodeEnum self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	strbuilder_append_str( result, txt("enum class ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	strbuilder_append_fmt( result, "%S : %SB", self->Name, typename_to_strbuilder(self->UnderlyingType) );

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			strbuilder_append_str( result, txt(";\n"));
	}
}

StrBuilder exec_to_strbuilder(CodeExec exec)
{
	GEN_ASSERT(exec);
	char* raw = ccast(char*, str_duplicate( exec->Content, _ctx->Allocator_Temp ).Ptr);
	StrBuilder result = { raw };
	return result;
}

void extern_to_strbuilder(CodeExtern self, StrBuilder* result )
{
	if ( self->Body )
		strbuilder_append_fmt( result, "extern \"%S\"\n{\n%SB\n}\n", self->Name, body_to_strbuilder(self->Body) );
	else
		strbuilder_append_fmt( result, "extern \"%S\"\n{}\n", self->Name );
}

StrBuilder include_to_strbuilder(CodeInclude include)
{
	return strbuilder_fmt_buf( _ctx->Allocator_Temp, "#include %S\n", include->Content );
}

void include_to_strbuilder_ref( CodeInclude include, StrBuilder* result )
{
	strbuilder_append_fmt( result, "#include %S\n", include->Content );
}

StrBuilder friend_to_strbuilder(CodeFriend self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 256 );
	friend_to_strbuilder_ref( self, & result );
	return result;
}

void friend_to_strbuilder_ref(CodeFriend self, StrBuilder* result )
{
	strbuilder_append_fmt( result, "friend %SB", code_to_strbuilder(self->Declaration) );

	if ( self->Declaration->Type != CT_Function && self->Declaration->Type != CT_Operator && (* result)[ strbuilder_length(* result) - 1 ] != ';' )
	{
		strbuilder_append_str( result, txt(";") );
	}

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "  %S", self->InlineCmt->Content );
	else
		strbuilder_append_str( result, txt("\n"));
}

StrBuilder fn_to_strbuilder(CodeFn self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Function:
			fn_to_strbuilder_def(self, & result );
		break;
		case CT_Function_Fwd:
			fn_to_strbuilder_fwd(self, & result );
		break;
	}
	return result;
}

void fn_to_strbuilder_def(CodeFn self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, " %SB ", attributes_to_strbuilder(self->Attributes) );

	bool prefix_specs = false;
	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );

				prefix_specs = true;
			}
		}
	}

	if ( self->Attributes || prefix_specs )
		strbuilder_append_str( result, txt("\n") );

	if ( self->ReturnType )
		strbuilder_append_fmt( result, "%SB %S(", typename_to_strbuilder(self->ReturnType), self->Name );

	else
		strbuilder_append_fmt( result, "%S(", self->Name );

	if ( self->Params )
		strbuilder_append_fmt( result, "%SB)", params_to_strbuilder(self->Params) );

	else
		strbuilder_append_str( result, txt(")") );

	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );
			}
		}
	}

	strbuilder_append_fmt( result, "\n{\n%SB\n}\n", body_to_strbuilder(self->Body) );
}

void fn_to_strbuilder_fwd(CodeFn self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	b32 prefix_specs = false;
	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! spec_is_trailing( * spec ) || ! ( * spec != Spec_Pure) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );

				prefix_specs = true;
			}
		}
	}

	if ( self->Attributes || prefix_specs )
	{
		strbuilder_append_str( result, txt("\n") );
	}

	if ( self->ReturnType )
		strbuilder_append_fmt( result, "%SB %S(", typename_to_strbuilder(self->ReturnType), self->Name );

	else
		strbuilder_append_fmt( result, "%S(", self->Name );

	if ( self->Params )
		strbuilder_append_fmt( result, "%SB)", params_to_strbuilder(self->Params) );

	else
		strbuilder_append_str( result, txt(")") );

	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );
			}
		}
	}

	if ( self->Specs && specifiers_has(self->Specs, Spec_Pure ) >= 0 )
		strbuilder_append_str( result, txt(" = 0;") );
	else if (self->Body)
		strbuilder_append_fmt( result, " = %SB;", body_to_strbuilder(self->Body) );

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
	else
		strbuilder_append_str( result, txt(";\n") );
}

StrBuilder module_to_strbuilder(CodeModule self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 64 );
	module_to_strbuilder_ref( self, & result );
	return result;
}

void module_to_strbuilder_ref(CodeModule self, StrBuilder* result )
{
	if (((scast(u32, ModuleFlag_Export) & scast(u32, self->ModuleFlags)) == scast(u32, ModuleFlag_Export)))
		strbuilder_append_str( result, txt("export "));

	if (((scast(u32, ModuleFlag_Import) & scast(u32, self->ModuleFlags)) == scast(u32, ModuleFlag_Import)))
		strbuilder_append_str( result, txt("import "));

	strbuilder_append_fmt( result, "%S;\n", self->Name );
}

StrBuilder namespace_to_strbuilder(CodeNS self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 512 );
	namespace_to_strbuilder_ref( self, & result );
	return result;
}

void namespace_to_strbuilder_ref(CodeNS self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	strbuilder_append_fmt( result, "namespace %S\n{\n%SB\n}\n", self->Name, body_to_strbuilder(self->Body) );
}

StrBuilder code_op_to_strbuilder(CodeOperator self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Operator:
		case CT_Operator_Member:
			code_op_to_strbuilder_def( self, & result );
		break;
		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
			code_op_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void code_op_to_strbuilder_def(CodeOperator self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );
			}
		}
	}

	if ( self->Attributes || self->Specs )
	{
		strbuilder_append_str( result, txt("\n") );
	}

	if ( self->ReturnType )
		strbuilder_append_fmt( result, "%SB %S (", typename_to_strbuilder(self->ReturnType), self->Name );

	if ( self->Params )
		strbuilder_append_fmt( result, "%SB)", params_to_strbuilder(self->Params) );

	else
		strbuilder_append_str( result, txt(")") );

	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );
			}
		}
	}

	strbuilder_append_fmt( result, "\n{\n%SB\n}\n"
		, body_to_strbuilder(self->Body)
	);
}

void code_op_to_strbuilder_fwd(CodeOperator self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB\n", attributes_to_strbuilder(self->Attributes) );

	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );
			}
		}
	}

	if ( self->Attributes || self->Specs )
	{
		strbuilder_append_str( result, txt("\n") );
	}

	strbuilder_append_fmt( result, "%SB %S (", typename_to_strbuilder(self->ReturnType), self->Name );

	if ( self->Params )
		strbuilder_append_fmt( result, "%SB)", params_to_strbuilder(self->Params) );

	else
		strbuilder_append_fmt( result, ")" );

	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );
			}
		}
	}

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
	else
		strbuilder_append_str( result, txt(";\n") );
}

StrBuilder opcast_to_strbuilder(CodeOpCast self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 128 );
	switch ( self->Type )
	{
		case CT_Operator_Cast:
			opcast_to_strbuilder_def(self, & result );
		break;
		case CT_Operator_Cast_Fwd:
			opcast_to_strbuilder_fwd(self, & result );
		break;
	}
	return result;
}

void opcast_to_strbuilder_def(CodeOpCast self, StrBuilder* result )
{
	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, "%*s ", spec_str.Len, spec_str.Ptr );
			}
		}

		if ( self->Name.Ptr && self->Name.Len )
			strbuilder_append_fmt( result, "%S operator %SB()", self->Name, typename_to_strbuilder(self->ValueType) );
		else
			strbuilder_append_fmt( result, "operator %SB()", typename_to_strbuilder(self->ValueType) );

		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %.*s", spec_str.Len, spec_str.Ptr );
			}
		}

		strbuilder_append_fmt( result, "\n{\n%SB\n}\n", body_to_strbuilder(self->Body) );
		return;
	}

	if ( self->Name.Ptr && self->Name.Len )
		strbuilder_append_fmt( result, "%S operator %SB()\n{\n%SB\n}\n", self->Name, typename_to_strbuilder(self->ValueType), body_to_strbuilder(self->Body) );
	else
		strbuilder_append_fmt( result, "operator %SB()\n{\n%SB\n}\n", typename_to_strbuilder(self->ValueType), body_to_strbuilder(self->Body) );
}

void opcast_to_strbuilder_fwd(CodeOpCast self, StrBuilder* result )
{
	if ( self->Specs )
	{
		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, "%*s ", spec_str.Len, spec_str.Ptr );
			}
		}

		strbuilder_append_fmt( result, "operator %SB()", typename_to_strbuilder(self->ValueType) );

		for ( Specifier* spec = begin_CodeSpecifiers(self->Specs); spec != end_CodeSpecifiers(self->Specs); spec = next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( spec_is_trailing( * spec ) )
			{
				Str spec_str = spec_to_str( * spec );
				strbuilder_append_fmt( result, " %*s", spec_str.Len, spec_str.Ptr );
			}
		}

		if ( self->InlineCmt )
			strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			strbuilder_append_str( result, txt(";\n") );
		return;
	}

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "operator %SB();  %SB", typename_to_strbuilder(self->ValueType) );
	else
		strbuilder_append_fmt( result, "operator %SB();\n", typename_to_strbuilder(self->ValueType) );
}

StrBuilder params_to_strbuilder(CodeParams self)
{
	GEN_ASSERT(self);
	GEN_ASSERT(self);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 128 );
	params_to_strbuilder_ref( self, & result );
	return result;
}

void params_to_strbuilder_ref( CodeParams self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Macro )
	{
		// Related to parsing: ( <macro>, ... )
		strbuilder_append_str( result, self->Macro->Content );
		// Could also be: ( <macro> <type <name>, ... )
	}

	if ( self->Name.Ptr && self->Name.Len )
	{
		if ( self->ValueType == nullptr )
			strbuilder_append_fmt( result, " %S", self->Name );
		else
			strbuilder_append_fmt( result, " %SB %S", typename_to_strbuilder(self->ValueType), self->Name );

	}
	else if ( self->ValueType )
		strbuilder_append_fmt( result, " %SB", typename_to_strbuilder(self->ValueType) );

	if ( self->PostNameMacro )
	{
		strbuilder_append_fmt( result, " %SB", code_to_strbuilder(self->PostNameMacro) );
	}

	if ( self->Value )
		strbuilder_append_fmt( result, " = %SB", code_to_strbuilder(self->Value) );

	if ( self->NumEntries - 1 > 0 )
	{
		for ( CodeParams param = begin_CodeParams(self->Next); param != end_CodeParams(self->Next); param = next_CodeParams(self->Next, param) )
		{
			strbuilder_append_fmt( result, ", %SB", params_to_strbuilder(param) );
		}
	}
}

StrBuilder preprocess_to_strbuilder(CodePreprocessCond self)
{
	GEN_ASSERT(self);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 256 );
	switch ( self->Type )
	{
		case CT_Preprocess_If:
			preprocess_to_strbuilder_if( self, & result );
		break;
		case CT_Preprocess_IfDef:
			preprocess_to_strbuilder_ifdef( self, & result );
		break;
		case CT_Preprocess_IfNotDef:
			preprocess_to_strbuilder_ifndef( self, & result );
		break;
		case CT_Preprocess_ElIf:
			preprocess_to_strbuilder_elif( self, & result );
		break;
		case CT_Preprocess_Else:
			preprocess_to_strbuilder_else( self, & result );
		break;
		case CT_Preprocess_EndIf:
			preprocess_to_strbuilder_endif( self, & result );
		break;
	}
	return result;
}

void preprocess_to_strbuilder_if(CodePreprocessCond cond, StrBuilder* result )
{
	GEN_ASSERT(cond);
	strbuilder_append_fmt( result, "#if %S", cond->Content );
}

void preprocess_to_strbuilder_ifdef(CodePreprocessCond cond, StrBuilder* result )
{
	GEN_ASSERT(cond);
	strbuilder_append_fmt( result, "#ifdef %S\n", cond->Content );
}

void preprocess_to_strbuilder_ifndef(CodePreprocessCond cond, StrBuilder* result )
{
	GEN_ASSERT(cond);
	strbuilder_append_fmt( result, "#ifndef %S", cond->Content );
}

void preprocess_to_strbuilder_elif(CodePreprocessCond cond, StrBuilder* result )
{
	GEN_ASSERT(cond);
	strbuilder_append_fmt( result, "#elif %S\n", cond->Content );
}

void preprocess_to_strbuilder_else(CodePreprocessCond cond, StrBuilder* result )
{
	GEN_ASSERT(cond);
	strbuilder_append_str( result, txt("#else\n") );
}

void preprocess_to_strbuilder_endif(CodePreprocessCond cond, StrBuilder* result )
{
	GEN_ASSERT(cond);
	strbuilder_append_str( result, txt("#endif\n") );
}

StrBuilder pragma_to_strbuilder(CodePragma self)
{
	GEN_ASSERT(self);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 256 );
	pragma_to_strbuilder_ref( self, & result );
	return result;
}

void pragma_to_strbuilder_ref(CodePragma self, StrBuilder* result )
{
	strbuilder_append_fmt( result, "#pragma %S\n", self->Content );
}

StrBuilder specifiers_to_strbuilder(CodeSpecifiers self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 64 );
	specifiers_to_strbuilder_ref( self, & result );
	return result;
}

void specifiers_to_strbuilder_ref( CodeSpecifiers self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	s32 idx  = 0;
	s32 left = self->NumEntries;
	while ( left-- )
	{
		Str spec = spec_to_str( self->ArrSpecs[idx] );
		strbuilder_append_fmt( result, "%.*s ", spec.Len, spec.Ptr );
		idx++;
	}
}

StrBuilder struct_to_strbuilder(CodeStruct self)
{
	GEN_ASSERT(self);
	GEN_ASSERT(self);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Struct:
			struct_to_strbuilder_def( self, & result );
		break;
		case CT_Struct_Fwd:
			struct_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void struct_to_strbuilder_def( CodeStruct self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	strbuilder_append_str( result, txt("struct ") );

	if ( self->Attributes )
	{
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );
	}

	if ( self->ParentType )
	{
		Str access_level = access_spec_to_str( self->ParentAccess );

		strbuilder_append_fmt( result, "%S : %S %SB", self->Name, access_level, typename_to_strbuilder(self->ParentType) );

		CodeTypename interface = cast(CodeTypename, self->ParentType->Next);
		if ( interface )
			strbuilder_append_str( result, txt("\n") );

		while ( interface )
		{
			strbuilder_append_fmt( result, ", %SB", typename_to_strbuilder(interface) );
			interface = interface->Next ? cast( CodeTypename, interface->Next) : NullCode;
		}
	}
	else if ( self->Name.Len )
	{
		strbuilder_append_str( result, self->Name );
	}

	if ( self->InlineCmt )
	{
		strbuilder_append_fmt( result, " // %S", self->InlineCmt->Content );
	}

	strbuilder_append_fmt( result, "\n{\n%SB\n}", body_to_strbuilder(self->Body) );

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		strbuilder_append_str( result, txt(";\n"));
}

void struct_to_strbuilder_fwd( CodeStruct self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "struct %SB %S", attributes_to_strbuilder(self->Attributes), self->Name );

	else strbuilder_append_fmt( result, "struct %S", self->Name );

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			strbuilder_append_str( result, txt( ";\n") );
	}
}

StrBuilder template_to_strbuilder(CodeTemplate self)
{
	GEN_ASSERT(self);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 1024 );
	template_to_strbuilder_ref( self, & result );
	return result;
}

void template_to_strbuilder_ref(CodeTemplate self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Params )
		strbuilder_append_fmt( result, "template< %SB >\n%SB", params_to_strbuilder(self->Params), code_to_strbuilder(self->Declaration) );
	else
		strbuilder_append_fmt( result, "template<>\n%SB", code_to_strbuilder(self->Declaration) );
}

StrBuilder typedef_to_strbuilder(CodeTypedef self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 128 );
	typedef_to_strbuilder_ref( self, & result );
	return result;
}

void typedef_to_strbuilder_ref(CodeTypedef self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	strbuilder_append_str( result, txt("typedef "));

	// Determines if the typedef is a function typename
	if ( self->UnderlyingType->ReturnType )
		strbuilder_append_string( result, code_to_strbuilder(self->UnderlyingType) );
	else
		strbuilder_append_fmt( result, "%SB %S", code_to_strbuilder(self->UnderlyingType), self->Name );

	if ( self->UnderlyingType->Type == CT_Typename && self->UnderlyingType->ArrExpr )
	{
		strbuilder_append_fmt( result, "[ %SB ];", code_to_strbuilder(self->UnderlyingType->ArrExpr) );

		Code next_arr_expr = self->UnderlyingType->ArrExpr->Next;
		while ( next_arr_expr )
		{
			strbuilder_append_fmt( result, "[ %SB ];", code_to_strbuilder(next_arr_expr) );
			next_arr_expr = next_arr_expr->Next;
		}
	}
	else
	{
		strbuilder_append_str( result, txt(";") );
	}

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "  %S", self->InlineCmt->Content);
	else
		strbuilder_append_str( result, txt("\n"));
}

StrBuilder typename_to_strbuilder(CodeTypename self)
{
	StrBuilder result = strbuilder_make_str( _ctx->Allocator_Temp, txt("") );
	typename_to_strbuilder_ref( self, & result );
	return result;
}

void typename_to_strbuilder_ref(CodeTypename self, StrBuilder* result )
{
	#if defined(GEN_USE_NEW_TYPENAME_PARSING)
		if ( self->ReturnType && self->Params )
		{
			if ( self->Attributes )
				strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );
			else
			{
				if ( self->Specs )
					strbuilder_append_fmt( result, "%SB ( %S ) ( %SB ) %SB", typename_to_strbuilder(self->ReturnType), self->Name, params_to_strbuilder(self->Params), specifiers_to_strbuilder(self->Specs) );
				else
					strbuilder_append_fmt( result, "%SB ( %S ) ( %SB )", typename_to_strbuilder(self->ReturnType), self->Name, params_to_strbuilder(self->Params) );
			}

			break;
		}
	#else
		if ( self->ReturnType && self->Params )
		{
			if ( self->Attributes )
				strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );
			else
			{
				if ( self->Specs )
					strbuilder_append_fmt( result, "%SB %S ( %SB ) %SB", typename_to_strbuilder(self->ReturnType), self->Name, params_to_strbuilder(self->Params), specifiers_to_strbuilder(self->Specs) );
				else
					strbuilder_append_fmt( result, "%SB %S ( %SB )", typename_to_strbuilder(self->ReturnType), self->Name, params_to_strbuilder(self->Params) );
			}

			return;
		}
	#endif

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	switch ( self->TypeTag )
	{
		case Tag_Class  : strbuilder_append_str( result, txt("class "));  break;
		case Tag_Enum   : strbuilder_append_str( result, txt("enum "));   break;
		case Tag_Struct : strbuilder_append_str( result, txt("struct ")); break;
		case Tag_Union  : strbuilder_append_str( result, txt("union "));  break;
		default:
			break;
	}

	if ( self->Specs )
		strbuilder_append_fmt( result, "%S %SB", self->Name, specifiers_to_strbuilder(self->Specs) );
	else
		strbuilder_append_fmt( result, "%S", self->Name );

	if ( self->IsParamPack )
		strbuilder_append_str( result, txt("..."));
}

StrBuilder union_to_strbuilder(CodeUnion self)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Union:
			union_to_strbuilder_def( self, & result );
		break;
		case CT_Union_Fwd:
			union_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void union_to_strbuilder_def(CodeUnion self, StrBuilder* result )
{
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	strbuilder_append_str( result, txt("union ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	if ( self->Name.Len )
	{
		strbuilder_append_fmt( result, "%S\n{\n%SB\n}"
			, self->Name
			, body_to_strbuilder(self->Body)
		);
	}
	else
	{
		// Anonymous union
		strbuilder_append_fmt( result, "\n{\n%SB\n}"
			, body_to_strbuilder(self->Body)
		);
	}

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		strbuilder_append_str( result, txt(";\n"));
}

void union_to_strbuilder_fwd(CodeUnion self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	strbuilder_append_str( result, txt("union ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	if ( self->Name.Len )
	{
		strbuilder_append_fmt( result, "%S", self->Name);
	}

	if ( self->Parent == nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		strbuilder_append_str( result, txt(";\n"));
}

StrBuilder using_to_strbuilder(CodeUsing self)
{
	GEN_ASSERT(self);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 128 );
	switch ( self->Type )
	{
		case CT_Using:
			using_to_strbuilder_ref( self, & result );
		break;
		case CT_Using_Namespace:
			using_to_strbuilder_ns( self, & result );
		break;
	}
	return result;
}

void using_to_strbuilder_ref(CodeUsing self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes )
		strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

	if ( self->UnderlyingType )
	{
		strbuilder_append_fmt( result, "using %S = %SB", self->Name, typename_to_strbuilder(self->UnderlyingType) );

		if ( self->UnderlyingType->ArrExpr )
		{
			strbuilder_append_fmt( result, "[ %SB ]", code_to_strbuilder(self->UnderlyingType->ArrExpr) );

			Code next_arr_expr = self->UnderlyingType->ArrExpr->Next;
			while ( next_arr_expr )
			{
				strbuilder_append_fmt( result, "[ %SB ]", code_to_strbuilder(next_arr_expr) );
				next_arr_expr = next_arr_expr->Next;
			}
		}

		strbuilder_append_str( result, txt(";") );
	}
	else
		strbuilder_append_fmt( result, "using %S;", self->Name );

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "  %S\n", self->InlineCmt->Content );
	else
		strbuilder_append_str( result, txt("\n"));
}

inline
void using_to_strbuilder_ns(CodeUsing self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "using namespace $SC;  %S", self->Name, self->InlineCmt->Content );
	else
		strbuilder_append_fmt( result, "using namespace %S;\n", self->Name );
}

inline
StrBuilder var_to_strbuilder(CodeVar self)
{
	GEN_ASSERT(self);
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, 256 );
	var_to_strbuilder_ref( self, & result );
	return result;
}

neverinline
void var_to_strbuilder_ref(CodeVar self, StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Parent && self->Parent->Type == CT_Variable )
	{
		// Its a comma-separated variable ( a NextVar )

		if ( self->Specs )
			strbuilder_append_fmt( result, "%SB ", specifiers_to_strbuilder(self->Specs) );

		strbuilder_append_str( result, self->Name );

		if ( self->ValueType && self->ValueType->ArrExpr )
		{
			strbuilder_append_fmt( result, "[ %SB ]", code_to_strbuilder(self->ValueType->ArrExpr) );

			Code next_arr_expr = self->ValueType->ArrExpr->Next;
			while ( next_arr_expr )
			{
				strbuilder_append_fmt( result, "[ %SB ]", code_to_strbuilder(next_arr_expr) );
				next_arr_expr = next_arr_expr->Next;
			}
		}

		if ( self->Value )
		{
			if ( self->VarParenthesizedInit )
				strbuilder_append_fmt( result, "( %SB ", code_to_strbuilder(self->Value) );
			else
				strbuilder_append_fmt( result, " = %SB", code_to_strbuilder(self->Value) );
		}

		// Keep the chain going...
		if ( self->NextVar )
			strbuilder_append_fmt( result, ", %SB", var_to_strbuilder(self->NextVar) );

		if ( self->VarParenthesizedInit )
			strbuilder_append_str( result, txt(" )"));

		return;
	}

	if ( bitfield_is_equal( u32, self->ModuleFlags, ModuleFlag_Export ))
		strbuilder_append_str( result, txt("export ") );

	if ( self->Attributes || self->Specs )
	{
		if ( self->Attributes )
			strbuilder_append_fmt( result, "%SB ", attributes_to_strbuilder(self->Attributes) );

		if ( self->Specs )
			strbuilder_append_fmt( result, "%SB\n", specifiers_to_strbuilder(self->Specs) );

		strbuilder_append_fmt( result, "%SB %S", typename_to_strbuilder(self->ValueType), self->Name );

		if ( self->ValueType && self->ValueType->ArrExpr )
		{
			strbuilder_append_fmt( result, "[ %SB ]", code_to_strbuilder(self->ValueType->ArrExpr) );

			Code next_arr_expr = self->ValueType->ArrExpr->Next;
			while ( next_arr_expr )
			{
				strbuilder_append_fmt( result, "[ %SB ]", code_to_strbuilder(next_arr_expr) );
				next_arr_expr = next_arr_expr->Next;
			}
		}

		if ( self->BitfieldSize )
			strbuilder_append_fmt( result, " : %SB", code_to_strbuilder(self->BitfieldSize) );

		if ( self->Value )
		{
			if ( self->VarParenthesizedInit )
				strbuilder_append_fmt( result, "( %SB ", code_to_strbuilder(self->Value) );
			else
				strbuilder_append_fmt( result, " = %SB", code_to_strbuilder(self->Value) );
		}

		if ( self->NextVar )
			strbuilder_append_fmt( result, ", %SB", var_to_strbuilder(self->NextVar) );

		if ( self->VarParenthesizedInit )
			strbuilder_append_str( result, txt(" )"));

		if ( self->InlineCmt )
			strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content);
		else
			strbuilder_append_str( result, txt(";\n") );

		return;
	}

	if ( self->BitfieldSize )
		strbuilder_append_fmt( result, "%SB %S : %SB", typename_to_strbuilder(self->ValueType), self->Name, code_to_strbuilder(self->BitfieldSize) );

	else if ( self->ValueType && self->ValueType->ArrExpr )
	{
		strbuilder_append_fmt( result, "%SB %S[ %SB ]", typename_to_strbuilder(self->ValueType), self->Name, code_to_strbuilder(self->ValueType->ArrExpr) );

		Code next_arr_expr = self->ValueType->ArrExpr->Next;
		while ( next_arr_expr )
		{
			strbuilder_append_fmt( result, "[ %SB ]", code_to_strbuilder(next_arr_expr) );
			next_arr_expr = next_arr_expr->Next;
		}
	}

	else
		strbuilder_append_fmt( result, "%SB %S", typename_to_strbuilder(self->ValueType), self->Name );

	if ( self->Value )
	{
		if ( self->VarParenthesizedInit )
			strbuilder_append_fmt( result, "( %SB ", code_to_strbuilder(self->Value) );
		else
			strbuilder_append_fmt( result, " = %SB", code_to_strbuilder(self->Value) );
	}

	if ( self->NextVar )
		strbuilder_append_fmt( result, ", %SB", var_to_strbuilder( self->NextVar) );

	if ( self->VarParenthesizedInit )
		strbuilder_append_str( result, txt(" )"));

	strbuilder_append_str( result, txt(";") );

	if ( self->InlineCmt )
		strbuilder_append_fmt( result, "  %S", self->InlineCmt->Content);
	else
		strbuilder_append_str( result, txt("\n"));
}
#pragma endregion AST

#pragma region Interface

internal void parser_init();
internal void parser_deinit();

internal
void* fallback_allocator_proc( void* allocator_data, AllocType type, ssize size, ssize alignment, void* old_memory, ssize old_size, u64 flags )
{
	GEN_ASSERT(_ctx);
	GEN_ASSERT(_ctx->Fallback_AllocatorBuckets);
	Arena* last = array_back(_ctx->Fallback_AllocatorBuckets);

	switch ( type )
	{
		case EAllocation_ALLOC:
		{
			if ( ( last->TotalUsed + size ) > last->TotalSize )
			{
				Arena bucket = arena_init_from_allocator( heap(), _ctx->InitSize_Fallback_Allocator_Bucket_Size );

				if ( bucket.PhysicalStart == nullptr )
					GEN_FATAL( "Failed to create bucket for Fallback_AllocatorBuckets");

				if ( ! array_append( _ctx->Fallback_AllocatorBuckets, bucket ) )
					GEN_FATAL( "Failed to append bucket to Fallback_AllocatorBuckets");

				last = array_back(_ctx->Fallback_AllocatorBuckets);
			}

			return alloc_align( arena_allocator_info(last), size, alignment );
		}
		case EAllocation_FREE:
		{
			// Doesn't recycle.
		}
		break;
		case EAllocation_FREE_ALL:
		{
			// Memory::cleanup instead.
		}
		break;
		case EAllocation_RESIZE:
		{
			if ( last->TotalUsed + size > last->TotalSize )
			{
				Arena bucket = arena_init_from_allocator( heap(), _ctx->InitSize_Fallback_Allocator_Bucket_Size );

				if ( bucket.PhysicalStart == nullptr )
					GEN_FATAL( "Failed to create bucket for Fallback_AllocatorBuckets");

				if ( ! array_append( _ctx->Fallback_AllocatorBuckets, bucket ) )
					GEN_FATAL( "Failed to append bucket to Fallback_AllocatorBuckets");

				last = array_back( _ctx->Fallback_AllocatorBuckets);
			}

			void* result = alloc_align( last->Backing, size, alignment );

			if ( result != nullptr && old_memory != nullptr )
			{
				mem_copy( result, old_memory, old_size );
			}

			return result;
		}
	}

	return nullptr;
}

internal
void define_constants()
{
	// We only initalize these if there is no base context.
	if ( context_counter > 0 )
		return;

	Code_Global          = make_code();
	Code_Global->Name    = cache_str( txt("Global Code") );
	Code_Global->Content = Code_Global->Name;

	Code_Invalid = make_code();
	code_set_global(Code_Invalid);

	t_empty       = (CodeTypename) make_code();
	t_empty->Type = CT_Typename;
	t_empty->Name = cache_str( txt("") );
	code_set_global(cast(Code, t_empty));

	access_private       = make_code();
	access_private->Type = CT_Access_Private;
	access_private->Name = cache_str( txt("private:\n") );
	code_set_global(cast(Code, access_private));

	access_protected       = make_code();
	access_protected->Type = CT_Access_Protected;
	access_protected->Name = cache_str( txt("protected:\n") );
	code_set_global(access_protected);

	access_public       = make_code();
	access_public->Type = CT_Access_Public;
	access_public->Name = cache_str( txt("public:\n") );
	code_set_global(access_public);

	Str api_export_str = code(GEN_API_Export_Code);
	attrib_api_export = def_attributes( api_export_str );
	code_set_global(cast(Code, attrib_api_export));

	Str api_import_str = code(GEN_API_Import_Code);
	attrib_api_import = def_attributes( api_import_str );
	code_set_global(cast(Code, attrib_api_import));

	module_global_fragment          = make_code();
	module_global_fragment->Type    = CT_Untyped;
	module_global_fragment->Name    = cache_str( txt("module;") );
	module_global_fragment->Content = module_global_fragment->Name;
	code_set_global(cast(Code, module_global_fragment));

	module_private_fragment          = make_code();
	module_private_fragment->Type    = CT_Untyped;
	module_private_fragment->Name    = cache_str( txt("module : private;") );
	module_private_fragment->Content = module_private_fragment->Name;
	code_set_global(cast(Code, module_private_fragment));

	fmt_newline = make_code();
	fmt_newline->Type = CT_NewLine;
	code_set_global((Code)fmt_newline);

	pragma_once          = (CodePragma) make_code();
	pragma_once->Type    = CT_Preprocess_Pragma;
	pragma_once->Name    = cache_str( txt("once") );
	pragma_once->Content = pragma_once->Name;
	code_set_global((Code)pragma_once);

	param_varadic            = (CodeParams) make_code();
	param_varadic->Type      = CT_Parameters;
	param_varadic->Name      = cache_str( txt("...") );
	param_varadic->ValueType = t_empty;
	code_set_global((Code)param_varadic);

	preprocess_else = (CodePreprocessCond) make_code();
	preprocess_else->Type = CT_Preprocess_Else;
	code_set_global((Code)preprocess_else);

	preprocess_endif = (CodePreprocessCond) make_code();
	preprocess_endif->Type = CT_Preprocess_EndIf;
	code_set_global((Code)preprocess_endif);

	Str auto_str     = txt("auto");     t_auto     = def_type( auto_str );     code_set_global( t_auto );
	Str void_str     = txt("void");     t_void     = def_type( void_str );     code_set_global( t_void );
	Str int_str      = txt("int");      t_int      = def_type( int_str );      code_set_global( t_int  );
	Str bool_str     = txt("bool");     t_bool     = def_type( bool_str );     code_set_global( t_bool );
	Str char_str     = txt("char");     t_char     = def_type( char_str );     code_set_global( t_char );
	Str wchar_str    = txt("wchar_t");  t_wchar_t  = def_type( wchar_str );    code_set_global( t_wchar_t );
	Str class_str    = txt("class");    t_class    = def_type( class_str );    code_set_global( t_class );
	Str typename_str = txt("typename"); t_typename = def_type( typename_str ); code_set_global( t_typename );

#ifdef GEN_DEFINE_LIBRARY_CODE_CONSTANTS
	t_b32 = def_type( name(b32) ); code_set_global( t_b32 );

	Str s8_str  = txt("s8");  t_s8  = def_type( s8_str ); code_set_global( t_s8 );
	Str s16_str = txt("s16"); t_s16 = def_type( s16_str ); code_set_global( t_s16 );
	Str s32_str = txt("s32"); t_s32 = def_type( s32_str ); code_set_global( t_s32 );
	Str s64_str = txt("s64"); t_s64 = def_type( s64_str ); code_set_global( t_s64 );

	Str u8_str  = txt("u8");  t_u8  = def_type( u8_str );  code_set_global( t_u8 );
	Str u16_str = txt("u16"); t_u16 = def_type( u16_str ); code_set_global( t_u16 );
	Str u32_str = txt("u32"); t_u32 = def_type( u32_str ); code_set_global( t_u32 );
	Str u64_str = txt("u64"); t_u64 = def_type( u64_str ); code_set_global( t_u64 );

	Str ssize_str = txt("ssize"); t_ssize = def_type( ssize_str ); code_set_global( t_ssize );
	Str usize_str = txt("usize"); t_usize = def_type( usize_str ); code_set_global( t_usize );

	Str f32_str = txt("f32"); t_f32 = def_type( f32_str ); code_set_global( t_f32 );
	Str f64_str = txt("f64"); t_f64 = def_type( f64_str ); code_set_global( t_f64 );
#endif

	spec_const            = def_specifier( Spec_Const);            code_set_global( cast(Code, spec_const ));
	spec_consteval        = def_specifier( Spec_Consteval);        code_set_global( cast(Code, spec_consteval ));;
	spec_constexpr        = def_specifier( Spec_Constexpr);        code_set_global( cast(Code, spec_constexpr ));;
	spec_constinit        = def_specifier( Spec_Constinit);        code_set_global( cast(Code, spec_constinit ));;
	spec_extern_linkage   = def_specifier( Spec_External_Linkage); code_set_global( cast(Code, spec_extern_linkage ));;
	spec_final            = def_specifier( Spec_Final);            code_set_global( cast(Code, spec_final ));;
	spec_forceinline      = def_specifier( Spec_ForceInline);      code_set_global( cast(Code, spec_forceinline ));;
	spec_global           = def_specifier( Spec_Global);           code_set_global( cast(Code, spec_global ));;
	spec_inline           = def_specifier( Spec_Inline);           code_set_global( cast(Code, spec_inline ));;
	spec_internal_linkage = def_specifier( Spec_Internal_Linkage); code_set_global( cast(Code, spec_internal_linkage ));;
	spec_local_persist    = def_specifier( Spec_Local_Persist);    code_set_global( cast(Code, spec_local_persist ));;
	spec_mutable          = def_specifier( Spec_Mutable);          code_set_global( cast(Code, spec_mutable ));;
	spec_neverinline      = def_specifier( Spec_NeverInline);      code_set_global( cast(Code, spec_neverinline ));;
	spec_noexcept         = def_specifier( Spec_NoExceptions);     code_set_global( cast(Code, spec_noexcept ));;
	spec_override         = def_specifier( Spec_Override);         code_set_global( cast(Code, spec_override ));;
	spec_ptr              = def_specifier( Spec_Ptr);              code_set_global( cast(Code, spec_ptr ));;
	spec_pure             = def_specifier( Spec_Pure);             code_set_global( cast(Code, spec_pure ));
	spec_ref              = def_specifier( Spec_Ref);              code_set_global( cast(Code, spec_ref ));;
	spec_register         = def_specifier( Spec_Register);         code_set_global( cast(Code, spec_register ));;
	spec_rvalue           = def_specifier( Spec_RValue);           code_set_global( cast(Code, spec_rvalue ));;
	spec_static_member    = def_specifier( Spec_Static);           code_set_global( cast(Code, spec_static_member ));;
	spec_thread_local     = def_specifier( Spec_Thread_Local);     code_set_global( cast(Code, spec_thread_local ));;
	spec_virtual          = def_specifier( Spec_Virtual);          code_set_global( cast(Code, spec_virtual ));;
	spec_volatile         = def_specifier( Spec_Volatile);         code_set_global( cast(Code, spec_volatile ));

	spec_local_persist = def_specifiers( 1, Spec_Local_Persist );
	code_set_global(cast(Code, spec_local_persist));

	if (enum_underlying_sig.Len == 0) {
		enum_underlying_sig = txt("enum_underlying(");
	}
	array_append( _ctx->PreprocessorDefines, enum_underlying_sig);
}

void init(Context* ctx)
{
	do_once() {
		context_counter = 0;
	}
	AllocatorInfo fallback_allocator = { & fallback_allocator_proc, nullptr };
	
	b32 using_fallback_allocator = false;
	if (ctx->Allocator_DyanmicContainers.Proc == nullptr) {
		ctx->Allocator_DyanmicContainers = fallback_allocator;
		using_fallback_allocator = true;
	}
	if (ctx->Allocator_Pool.Proc == nullptr ) {
		ctx->Allocator_Pool = fallback_allocator;
		using_fallback_allocator = true;
	}
	if (ctx->Allocator_StrCache.Proc == nullptr) {
		ctx->Allocator_StrCache = fallback_allocator;
		using_fallback_allocator = true;
	}
	if (ctx->Allocator_Temp.Proc == nullptr) {
		ctx->Allocator_Temp = fallback_allocator;
		using_fallback_allocator = true;
	}
	// Setup fallback allocator
	if (using_fallback_allocator)
	{
		ctx->Fallback_AllocatorBuckets = array_init_reserve(Arena, heap(), 128 );
		if ( ctx->Fallback_AllocatorBuckets == nullptr )
			GEN_FATAL( "Failed to reserve memory for Fallback_AllocatorBuckets");

		Arena bucket = arena_init_from_allocator( heap(), ctx->InitSize_Fallback_Allocator_Bucket_Size );
		if ( bucket.PhysicalStart == nullptr )
			GEN_FATAL( "Failed to create first bucket for Fallback_AllocatorBuckets");

		array_append( ctx->Fallback_AllocatorBuckets, bucket );
	}

	if (ctx->Max_CommentLineLength == 0) {
		ctx->Max_CommentLineLength = 1024;
	}
	if (ctx->Max_StrCacheLength == 0) {
		ctx->Max_StrCacheLength = kilobytes(512);
	}

	if (ctx->InitSize_BuilderBuffer == 0) {
		ctx->InitSize_BuilderBuffer = megabytes(2);
	}
	if (ctx->InitSize_CodePoolsArray == 0) { 
		ctx->InitSize_CodePoolsArray = 16; 
	}
	if (ctx->InitSize_StringArenasArray == 0) {
		ctx->InitSize_StringArenasArray = 16; 
	}
	if (ctx->CodePool_NumBlocks == 0) {
		ctx->CodePool_NumBlocks = kilobytes(16);
	}

	if (ctx->InitSize_LexArena == 0 ) {
		ctx->InitSize_LexArena = megabytes(4);
	}
	if (ctx->SizePer_StringArena == 0) {
		ctx->SizePer_StringArena = megabytes(1);
	}

	if (ctx->InitSize_Fallback_Allocator_Bucket_Size == 0) { 
		ctx->InitSize_Fallback_Allocator_Bucket_Size = megabytes(8);
	}

	// Override the current context (user has to put it back if unwanted).
	_ctx = ctx;

	// Setup the arrays
	{
		ctx->CodePools = array_init_reserve(Pool, ctx->Allocator_DyanmicContainers, ctx->InitSize_CodePoolsArray );
		if ( ctx->CodePools == nullptr )
			GEN_FATAL( "gen::init: Failed to initialize the CodePools array" );

		ctx->StringArenas = array_init_reserve(Arena, ctx->Allocator_DyanmicContainers, ctx->InitSize_StringArenasArray );
		if ( ctx->StringArenas == nullptr )
			GEN_FATAL( "gen::init: Failed to initialize the StringArenas array" );
	}
	// Setup the code pool and code entries arena.
	{
		Pool code_pool = pool_init( ctx->Allocator_Pool, ctx->CodePool_NumBlocks, sizeof(AST) );
		if ( code_pool.PhysicalStart == nullptr )
			GEN_FATAL( "gen::init: Failed to initialize the code pool" );
		array_append( ctx->CodePools, code_pool );

		// TODO(Ed): This is going to be phased out most likely.
		ctx->LexArena = arena_init_from_allocator( ctx->Allocator_DyanmicContainers, ctx->InitSize_LexArena );

		// TODO(Ed): Eventually the string arenas needs to be phased out for a dedicated string slab allocator
		Arena strbuilder_arena = arena_init_from_allocator( ctx->Allocator_StrCache, ctx->SizePer_StringArena );
		if ( strbuilder_arena.PhysicalStart == nullptr )
			GEN_FATAL( "gen::init: Failed to initialize the string arena" );
		array_append( ctx->StringArenas, strbuilder_arena );
	}
	// Setup the hash tables
	{
		ctx->StrCache = hashtable_init(StrCached, ctx->Allocator_DyanmicContainers);
		if ( ctx->StrCache.Entries == nullptr )
			GEN_FATAL( "gen::init: Failed to initialize the StringCache");
	}
	// Preprocessor Defines
	ctx->PreprocessorDefines = array_init_reserve(StrCached, ctx->Allocator_DyanmicContainers, kilobytes(1) );

	define_constants();
	parser_init();

	++ context_counter;
}

void deinit(Context* ctx)
{
	GEN_ASSERT(context_counter);
	GEN_ASSERT_MSG(context_counter > 0, "Attempted to deinit a context that for some reason wan't accounted for!");
	usize index = 0;
	usize left  = array_num(ctx->CodePools);
	do
	{
		Pool* code_pool = & ctx->CodePools[index];
		pool_free(code_pool);
		index++;
	}
	while ( left--, left );

	index = 0;
	left  = array_num(ctx->StringArenas);
	do
	{
		Arena* strbuilder_arena = & ctx->StringArenas[index];
		arena_free(strbuilder_arena);
		index++;
	}
	while ( left--, left );

	hashtable_destroy(ctx->StrCache);

	array_free( ctx->CodePools);
	array_free( ctx->StringArenas);

	arena_free(& ctx->LexArena);

	array_free(ctx->PreprocessorDefines);

	left  = array_num( ctx->Fallback_AllocatorBuckets);
	if (left)
	{
		index = 0;
		do
		{
			Arena* bucket = & ctx->Fallback_AllocatorBuckets[ index ];
			arena_free(bucket);
			index++;
		}
		while ( left--, left );
		array_free( ctx->Fallback_AllocatorBuckets);
	}
	parser_deinit();

	if (_ctx == ctx) 
		_ctx = nullptr;
	-- context_counter;
}

void reset(Context* ctx)
{
	s32 index = 0;
	s32 left  = array_num(ctx->CodePools);
	do
	{
		Pool* code_pool = & ctx->CodePools[index];
		pool_clear(code_pool);
		index++;
	}
	while ( left--, left );

	index = 0;
	left  = array_num(ctx->StringArenas);
	do
	{
		Arena* strbuilder_arena = & ctx->StringArenas[index];
		strbuilder_arena->TotalUsed = 0;;
		index++;
	}
	while ( left--, left );

	hashtable_clear(ctx->StrCache);
	define_constants();
}

void set_context(Context* new_ctx) {
	GEN_ASSERT(new_ctx);
	_ctx = new_ctx;
}

AllocatorInfo get_cached_str_allocator( s32 str_length )
{
	Arena* last     = array_back(_ctx->StringArenas);
	usize  size_req = str_length + sizeof(StrBuilderHeader) + sizeof(char*);
	if ( last->TotalUsed + scast(ssize, size_req) > last->TotalSize )
	{
		Arena new_arena = arena_init_from_allocator( _ctx->Allocator_StrCache, _ctx->SizePer_StringArena );
		if ( ! array_append( _ctx->StringArenas, new_arena ) )
			GEN_FATAL( "gen::get_cached_str_allocator: Failed to allocate a new string arena" );

		last = array_back( _ctx->StringArenas);
	}
	return arena_allocator_info(last);
}

// Will either make or retrive a code string.
StrCached cache_str( Str str )
{
	if (str.Len > _ctx->Max_StrCacheLength) {
		// Do not cache the string, just shove into the arena and and return it.
		Str result = strbuilder_to_str( strbuilder_make_str( get_cached_str_allocator( str.Len ), str ));
		return result;
	}
	u64 key = crc32( str.Ptr, str.Len ); {
		StrCached* result = hashtable_get( _ctx->StrCache, key );
		if ( result )
			return * result;
	}
	Str result = strbuilder_to_str( strbuilder_make_str( get_cached_str_allocator( str.Len ), str ));
	hashtable_set( _ctx->StrCache, key, result );
	return result;
}

// Used internally to retireve a Code object form the CodePool.
Code make_code()
{
	Pool* allocator = array_back( _ctx->CodePools);
	if ( allocator->FreeList == nullptr )
	{
		Pool code_pool = pool_init( _ctx->Allocator_Pool, _ctx->CodePool_NumBlocks, sizeof(AST) );

		if ( code_pool.PhysicalStart == nullptr )
			GEN_FATAL( "gen::make_code: Failed to allocate a new code pool - CodePool allcoator returned nullptr." );

		if ( ! array_append( _ctx->CodePools, code_pool ) )
			GEN_FATAL( "gen::make_code: Failed to allocate a new code pool - CodePools failed to append new pool." );

		allocator = array_back( _ctx->CodePools);
	}
	Code result = { rcast( AST*, alloc( pool_allocator_info(allocator), sizeof(AST) )) };
	mem_set( rcast(void*, cast(AST*, result)), 0, sizeof(AST) );
	return result;
}

void set_preprocess_define( Str id, b32 is_functional ) {
	StrBuilder builder = strbuilder_make_str( _ctx->Allocator_Temp, id );
	if (is_functional) {
		strbuilder_append_char( & builder, '(' );
	}
	array_append( _ctx->PreprocessorDefines, cache_str( strbuilder_to_str(builder)) ); 
}

#pragma region Upfront

enum OpValidateResult : u32
{
	OpValResult_Fail,
	OpValResult_Global,
	OpValResult_Member
};

internal neverinline
OpValidateResult operator__validate( Operator op, CodeParams params_code, CodeTypename ret_type, CodeSpecifiers specifier )
{
	if ( op == Op_Invalid )
	{
		log_failure("gen::def_operator: op cannot be invalid");
		return OpValResult_Fail;
	}

#pragma region Helper Macros
#	define check_params()                                                                                                   \
	if ( ! params_code )                                                                                                    \
	{                                                                                                                       \
		log_failure("gen::def_operator: params is null and operator%S requires it", operator_to_str(op));                   \
		return OpValResult_Fail;                                                                                            \
	}                                                                                                                       \
	if ( params_code->Type != CT_Parameters )                                                                               \
	{                                                                                                                       \
		log_failure("gen::def_operator: params is not of Parameters type - %S", code_debug_str( cast(Code, params_code)));  \
		return OpValResult_Fail;                                                                                            \
	}

#	define check_param_eq_ret()                                                                            \
	if ( ! is_member_symbol && ! code_is_equal(cast(Code, params_code->ValueType), cast(Code, ret_type)) ) \
	{                                                                                                      \
		log_failure("gen::def_operator: operator%S requires first parameter to equal return type\n"        \
			"param types: %S\n"                                                                            \
			"return type: %S",                                                                             \
			operator_to_str(op).Ptr,                                                                       \
			code_debug_str(cast(Code, params_code)),                                                       \
			code_debug_str(cast(Code, ret_type))                                                           \
		);                                                                                                 \
		return OpValResult_Fail;                                                                           \
	}
#pragma endregion Helper Macros

	if ( ! ret_type )
	{
		log_failure("gen::def_operator: ret_type is null but is required by operator%S", operator_to_str(op));
	}

	if ( ret_type->Type != CT_Typename )
	{
		log_failure("gen::def_operator: ret_type is not of typename type - %S", code_debug_str(cast(Code, ret_type)));
		return OpValResult_Fail;
	}

	bool is_member_symbol = false;

	switch ( op )
	{
#	define specs( ... ) num_args( __VA_ARGS__ ), __VA_ARGS__
		case Op_Assign:
			check_params();

			if ( params_code->NumEntries > 1 )
			{
				log_failure("gen::def_operator: "
					"operator%S does not support non-member definition (more than one parameter provided) - %S",
					operator_to_str(op),
					code_debug_str(cast(Code, params_code))
				);
				return OpValResult_Fail;
			}

			is_member_symbol = true;
			break;

		case Op_Assign_Add:
		case Op_Assign_Subtract:
		case Op_Assign_Multiply:
		case Op_Assign_Divide:
		case Op_Assign_Modulo:
		case Op_Assign_BAnd:
		case Op_Assign_BOr:
		case Op_Assign_BXOr:
		case Op_Assign_LShift:
		case Op_Assign_RShift:
			check_params();

			if ( params_code->NumEntries == 1 )
				is_member_symbol = true;

			else
				check_param_eq_ret();

			if (params_code->NumEntries > 2 )
			{
				log_failure("gen::def_operator: operator%S may not be defined with more than two parametes - param count; %d\n%S"
					, operator_to_str(op)
					, params_code->NumEntries
					, code_debug_str(cast(Code, params_code))
				);
				return OpValResult_Fail;
			}
			break;

		case Op_Increment:
		case Op_Decrement:
			// If its not set, it just means its a prefix member op.
			if ( params_code )
			{
				if ( params_code->Type != CT_Parameters )
				{
					log_failure("gen::def_operator: operator%S params code provided is not of Parameters type - %S"
						, operator_to_str(op)
						, code_debug_str(cast(Code, params_code))
					);
					return OpValResult_Fail;
				}

				switch ( params_code->NumEntries )
				{
					case 1:
						if ( code_is_equal((Code)params_code->ValueType, (Code)t_int ) )
							is_member_symbol = true;

						else
							check_param_eq_ret();
						break;

					case 2:
						check_param_eq_ret();

						if ( ! code_is_equal((Code)params_get(params_code, 1), (Code)t_int ) )
						{
							log_failure("gen::def_operator: "
								"operator%S requires second parameter of non-member definition to be int for post-decrement",
								operator_to_str(op)
							);
							return OpValResult_Fail;
						}
						break;

					default:
						log_failure("gen::def_operator: operator%S recieved unexpected number of parameters recived %d instead of 0-2"
							, operator_to_str(op)
							, params_code->NumEntries
						);
						return OpValResult_Fail;
				}
			}
			break;

		case Op_Unary_Plus:
		case Op_Unary_Minus:
			if ( ! params_code )
				is_member_symbol = true;

			else
			{
				if ( params_code->Type != CT_Parameters )
				{
					log_failure("gen::def_operator: params is not of Parameters type - %S", code_debug_str((Code)params_code));
					return OpValResult_Fail;
				}

				if ( code_is_equal((Code)params_code->ValueType, (Code)ret_type ) )
				{
					log_failure("gen::def_operator: "
						"operator%S is non-member symbol yet first paramter does not equal return type\n"
						"param type: %S\n"
						"return type: %S\n"
						, operator_to_str(op)
						, code_debug_str((Code)params_code)
						, code_debug_str((Code)ret_type)
					);
					return OpValResult_Fail;
				}

				if ( params_code->NumEntries > 1 )
				{
					log_failure("gen::def_operator: operator%S may not have more than one parameter - param count: %d"
						, operator_to_str(op)
						, params_code->NumEntries
					);
					return OpValResult_Fail;
				}
			}
			break;

		case Op_BNot:
		{
			// Some compilers let you do this...
		#if 0
			if ( ! ret_type.is_equal( t_bool) )
			{
				log_failure( "gen::def_operator: return type is not a boolean - %S", code_debug_str(params_code) );
				return OpValidateResult::Fail;
			}
		#endif

			if ( ! params_code )
				is_member_symbol = true;

			else
			{
				if ( params_code->Type != CT_Parameters )
				{
					log_failure( "gen::def_operator: params is not of Parameters type - %S", code_debug_str((Code)params_code) );
					return OpValResult_Fail;
				}

				if ( params_code->NumEntries > 1 )
				{
					log_failure(
						"gen::def_operator: operator%S may not have more than one parameter - param count: %d",
						operator_to_str( op ),
						params_code->NumEntries
					);
					return OpValResult_Fail;
				}
			}
			break;
		}

		case Op_Add:
		case Op_Subtract:
		case Op_Multiply:
		case Op_Divide:
		case Op_Modulo:
		case Op_BAnd:
		case Op_BOr:
		case Op_BXOr:
		case Op_LShift:
		case Op_RShift:
			check_params();

			switch ( params_code->NumEntries )
			{
				case 1:
					is_member_symbol = true;
					break;

				case 2:
					// This is allowed.
					// if ( ! code_is_equal((Code)params_code->ValueType, (Code)ret_type ) )
					// {
					// 	log_failure("gen::def_operator: "
					// 		"operator%S is non-member symbol yet first paramter does not equal return type\n"
					// 		"param type: %S\n"
					// 		"return type: %S\n"
					// 		, operator_to_str(op)
					// 		, code_debug_str((Code)params_code)
					// 		, code_debug_str((Code)ret_type)
					// 	);
					// 	return OpValResult_Fail;
					// }
					break;

				default:
					log_failure("gen::def_operator: operator%S recieved unexpected number of paramters recived %d instead of 0-2"
						, operator_to_str(op)
						, params_code->NumEntries
					);
					return OpValResult_Fail;
			}
			break;

		case Op_UnaryNot:
			if ( ! params_code )
				is_member_symbol = true;

			else
			{
				if ( params_code->Type != CT_Parameters )
				{
					log_failure("gen::def_operator: params is not of Parameters type - %S", code_debug_str((Code)params_code));
					return OpValResult_Fail;
				}

				if ( params_code->NumEntries != 1 )
				{
					log_failure("gen::def_operator: operator%S recieved unexpected number of paramters recived %d instead of 0-1"
						, operator_to_str(op)
						, params_code->NumEntries
					);
					return OpValResult_Fail;
				}
			}

			if ( ! code_is_equal((Code)ret_type, (Code)t_bool ))
			{
				log_failure("gen::def_operator: operator%S return type must be of type bool - %S"
					, operator_to_str(op)
					, code_debug_str((Code)ret_type)
				);
				return OpValResult_Fail;
			}
			break;

		case Op_LAnd:
		case Op_LOr:
		case Op_LEqual:
		case Op_LNot:
		case Op_Lesser:
		case Op_Greater:
		case Op_LesserEqual:
		case Op_GreaterEqual:
			check_params();

			switch ( params_code->NumEntries )
			{
				case 1:
					is_member_symbol = true;
					break;

				case 2:
					break;

				default:
					log_failure("gen::def_operator: operator%S recieved unexpected number of paramters recived %d instead of 1-2"
						, operator_to_str(op)
						, params_code->NumEntries
					);
					return OpValResult_Fail;
			}
			break;

		case Op_Indirection:
		case Op_AddressOf:
		case Op_MemberOfPointer:
			if ( params_code && params_code->NumEntries > 1)
			{
				log_failure("gen::def_operator: operator%S recieved unexpected number of paramters recived %d instead of 0-1"
					, operator_to_str(op)
					, params_code->NumEntries
				);
				return OpValResult_Fail;
			}
			else
			{
				is_member_symbol = true;
			}
			break;

		case Op_PtrToMemOfPtr:
			if ( params_code )
			{
				log_failure("gen::def_operator: operator%S expects no paramters - %S", operator_to_str(op), code_debug_str((Code)params_code));
				return OpValResult_Fail;
			}
			break;

		case Op_Subscript:
		case Op_FunctionCall:
		case Op_Comma:
			check_params();
			break;

		case Op_New:
		case Op_Delete:
			// This library doesn't support validating new and delete yet.
			break;
#	undef specs
	}

	return is_member_symbol ? OpValResult_Member : OpValResult_Global;
#	undef check_params
#	undef check_ret_type
#	undef check_param_eq_ret
}

forceinline
bool name__check( char const* context, Str name )
{
	if ( name.Len <= 0 ) {
		log_failure( "gen::%s: Invalid name length provided - %d",  name.Len );
		return false;
	}
	if ( name.Ptr == nullptr ) {
		log_failure( "gen::%s: name is null" );
		return false;
	}
	return true;
}
#define name_check( context, name ) name__check( #context, name )

forceinline
bool null__check( char const* context, char const* code_id, Code code ) {
	if ( code == nullptr ) {
		log_failure( "gen::%s: %s provided is null", context, code_id );
		return false;
	}
	return true;
}
#define null_check( context, code ) null__check( #context, #code, cast(Code, code) )

/*
The implementaiton of the upfront constructors involves doing three things:
* Validate the arguments given to construct the intended type of AST is valid.
* Construct said AST type.
* Lock the AST (set to readonly) and return the valid object.

If any of the validation fails, it triggers a call to log_failure with as much info the give the user so that they can hopefully
identify the issue without having to debug too much (at least they can debug though...)

The largest of the functions is related to operator overload definitions.
The library validates a good protion of their form and thus the argument processing for is quite a bit.
*/
CodeAttributes def_attributes( Str content )
{
	if ( content.Len <= 0 || content.Ptr == nullptr ) {
		log_failure( "gen::def_attributes: Invalid attributes provided" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	Code
	result          = make_code();
	result->Type    = CT_PlatformAttributes;
	result->Name    = cache_str( content );
	result->Content = result->Name;
	return (CodeAttributes) result;
}

CodeComment def_comment( Str content )
{
	if ( content.Len <= 0 || content.Ptr == nullptr )
	{
		log_failure( "gen::def_comment: Invalid comment provided:" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	StrBuilder  cmt_formatted = strbuilder_make_reserve( _ctx->Allocator_Temp, kilobytes(1) );
	char const* end           = content.Ptr + content.Len;
	char const* scanner       = content.Ptr;
	s32         curr          = 0;
	do
	{
		char const* next   = scanner;
		s32         length = 0;
		while ( next != end && scanner[ length ] != '\n' )
		{
			next = scanner + length;
			length++;
		}
		length++;

		strbuilder_append_fmt(& cmt_formatted, "//%.*s", length, scanner );
		scanner += length;
	}
	while ( scanner <= end );

	if ( * strbuilder_back(cmt_formatted) != '\n' )
		strbuilder_append_str( & cmt_formatted, txt("\n") );

	Str name = strbuilder_to_str(cmt_formatted);

	Code
	result          = make_code();
	result->Type    = CT_Comment;
	result->Name    = cache_str( name );
	result->Content = result->Name;

	strbuilder_free(& cmt_formatted);

	return (CodeComment) result;
}

CodeConstructor def_constructor( Opts_def_constructor p )
{
	if ( p.params && p.params->Type != CT_Parameters ) {
		log_failure("gen::def_constructor: params must be of Parameters type - %s", code_debug_str((Code)p.params));
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeConstructor result = (CodeConstructor) make_code();
	if ( p.params ) {
		result->Params = p.params;
	}
	if ( p.initializer_list ) {
		result->InitializerList = p.initializer_list;
	}
	if ( p.body )
	{
		switch ( p.body->Type ) {
			case CT_Function_Body:
			case CT_Untyped:
			break;

			default:
				log_failure("gen::def_constructor: body must be either of Function_Body or Untyped type - %s", code_debug_str(p.body));
				return InvalidCode;
		}

		result->Type = CT_Constructor;
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Constructor_Fwd;
	}
	return result;
}

CodeClass def_class( Str name, Opts_def_struct p )
{
	if ( ! name_check( def_class, name ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_class: attributes was not a 'PlatformAttributes' type: %s", code_debug_str(p.attributes) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.parent && ( p.parent->Type != CT_Class && p.parent->Type != CT_Struct && p.parent->Type != CT_Typename && p.parent->Type != CT_Untyped ) ) {
		log_failure( "gen::def_class: parent provided is not type 'Class', 'Struct', 'Typeanme', or 'Untyped': %s", code_debug_str(p.parent) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeClass
	result              = (CodeClass) make_code();
	result->Name        = cache_str( name );
	result->ModuleFlags = p.mflags;
	if ( p.body )
	{
		switch ( p.body->Type )
		{
			case CT_Class_Body:
			case CT_Untyped:
			break;

			default:
				log_failure("gen::def_class: body must be either of Class_Body or Untyped type - %s", code_debug_str(p.body));
				return InvalidCode;
		}

		result->Type         = CT_Class;
		result->Body         = p.body;
		result->Body->Parent = cast(Code, result);
	}
	else {
		result->Type = CT_Class_Fwd;
	}

	result->Attributes   = p.attributes;
	result->ParentAccess = p.parent_access;
	result->ParentType   = p.parent;

	for (s32 idx = 0; idx < p.num_interfaces; idx++ ) {
		class_add_interface(result, p.interfaces[idx] );
	}
	return result;
}

CodeDefine def_define( Str name, Str content, Opts_def_define p )
{
	if ( ! name_check( def_define, name ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeDefine
	result          = (CodeDefine) make_code();
	result->Type    = CT_Preprocess_Define;
	result->Name    = cache_str( name );

	if ( content.Len <= 0 || content.Ptr == nullptr )
		result->Content = cache_str( txt("") );
	else
		result->Content = cache_str( strbuilder_to_str(strbuilder_fmt_buf(_ctx->Allocator_Temp, "%S\n", content)) );

	b32  append_preprocess_defines = ! p.dont_append_preprocess_defines;
	if ( append_preprocess_defines ) {
		// Add the define to PreprocessorDefines for usage in parsing
		s32 lex_id_len = 0;
		for (; lex_id_len < result->Name.Len; ++ lex_id_len ) {
			if ( result->Name.Ptr[lex_id_len] == '(' )
				break;
		}
		Str lex_id = { result->Name.Ptr,  lex_id_len };
		array_append(_ctx->PreprocessorDefines, cache_str(lex_id) );
	}
	return result;
}

CodeDestructor def_destructor( Opts_def_destructor p )
{
	if ( p.specifiers && p.specifiers->Type != CT_Specifiers ) {
		log_failure( "gen::def_destructor: specifiers was not a 'Specifiers' type: %s", code_debug_str(p.specifiers) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeDestructor
	result        = (CodeDestructor) make_code();
	result->Specs = p.specifiers;
	if ( p.body )
	{
		switch ( p.body->Type )
		{
			case CT_Function_Body:
			case CT_Untyped:
			break;

			default:
				log_failure("gen::def_destructor: body must be either of Function_Body or Untyped type - %s", code_debug_str(p.body));
				return InvalidCode;
		}

		result->Type = CT_Destructor;
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Destructor_Fwd;
	}
	return result;
}

CodeEnum def_enum( Str name, Opts_def_enum p )
{
	if ( ! name_check( def_enum, name ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.type && p.type->Type != CT_Typename ) {
		log_failure( "gen::def_enum: enum underlying type provided was not of type Typename: %s", code_debug_str(p.type) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_enum: attributes was not a 'PlatformAttributes' type: %s", code_debug_str(p.attributes) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeEnum
	result              = (CodeEnum) make_code();
	result->Name        = cache_str( name );
	result->ModuleFlags = p.mflags;
	if ( p.body )
	{
		switch ( p.body->Type )
		{
			case CT_Enum_Body:
			case CT_Untyped:
			break;

			default:
				log_failure( "gen::def_enum: body must be of Enum_Body or Untyped type %s", code_debug_str(p.body));
				return InvalidCode;
		}

		result->Type = p.specifier == EnumDecl_Class ?
			CT_Enum_Class : CT_Enum;

		result->Body = p.body;
	}
	else
	{
		result->Type = p.specifier == EnumDecl_Class ?
			CT_Enum_Class_Fwd : CT_Enum_Fwd;
	}
	result->Attributes = p.attributes;

	if ( p.type ) {
		result->UnderlyingType = p.type;
	}
	else if ( p.type_macro ) {
		result->UnderlyingTypeMacro = p.type_macro;
	}
	else if ( result->Type != CT_Enum_Class_Fwd && result->Type != CT_Enum_Fwd )
	{
		log_failure( "gen::def_enum: enum forward declaration must have an underlying type" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	return result;
}

CodeExec def_execution( Str content )
{
	if ( content.Len <= 0 || content.Ptr == nullptr ) {
		log_failure( "gen::def_execution: Invalid execution provided" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeExec
	result          = (CodeExec) make_code();
	result->Type    = CT_Execution;
	result->Content = cache_str( content );
	return result;
}

CodeExtern def_extern_link( Str name, CodeBody body )
{
	if ( ! name_check(def_extern_link, name) || ! null_check(def_extern_link, body) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( body->Type != CT_Extern_Linkage_Body && body->Type != CT_Untyped ) {
		log_failure("gen::def_extern_linkage: body is not of extern_linkage or untyped type %s", code_debug_str(body));
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeExtern
	result        = (CodeExtern)make_code();
	result->Type  = CT_Extern_Linkage;
	result->Name  = cache_str( name );
	result->Body  = body;
	return result;
}

CodeFriend def_friend( Code declaration )
{
	if ( ! null_check( def_friend, declaration ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	switch ( declaration->Type )
	{
		case CT_Class_Fwd:
		case CT_Function_Fwd:
		case CT_Operator_Fwd:
		case CT_Struct_Fwd:
		case CT_Class:
		case CT_Function:
		case CT_Operator:
		case CT_Struct:
		break;

		default:
			log_failure("gen::def_friend: requires declartion to have class, function, operator, or struct - %s", code_debug_str(declaration));
			return InvalidCode;
	}
	CodeFriend
	result              = (CodeFriend) make_code();
	result->Type        = CT_Friend;
	result->Declaration = declaration;
	return result;
}

CodeFn def_function( Str name, Opts_def_function p )
{
	if ( ! name_check( def_function, name )) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.params && p.params->Type != CT_Parameters ) {
		log_failure( "gen::def_function: params was not a `Parameters` type: %s", code_debug_str(p.params) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.ret_type && p.ret_type->Type != CT_Typename ) {
		log_failure( "gen::def_function: ret_type was not a Typename: %s", code_debug_str(p.ret_type) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.specs && p.specs-> Type != CT_Specifiers ) {
		log_failure( "gen::def_function: specifiers was not a `Specifiers` type: %s", code_debug_str(p.specs) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.attrs && p.attrs->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_function: attributes was not a `PlatformAttributes` type: %s", code_debug_str(p.attrs) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeFn
	result              = (CodeFn) make_code();
	result->Name        = cache_str( name );
	result->ModuleFlags = p.mflags;
	if ( p.body )
	{
		switch ( p.body->Type )
		{
			case CT_Function_Body:
			case CT_Execution:
			case CT_Untyped:
				break;

			default:
			{
				log_failure("gen::def_function: body must be either of Function_Body, Execution, or Untyped type. %s", code_debug_str(p.body));
				return InvalidCode;
			}
		}
		result->Type = CT_Function;
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Function_Fwd;
	}
	result->Attributes = p.attrs;
	result->Specs      = p.specs;
	result->Params     = p.params;
	result->ReturnType = p.ret_type ? p.ret_type : t_void;
	return result;
}

CodeInclude def_include( Str path, Opts_def_include p )
{
	if ( path.Len <= 0 || path.Ptr == nullptr ) {
		log_failure( "gen::def_include: Invalid path provided - %d" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	StrBuilder content = p.foreign ?
			strbuilder_fmt_buf( _ctx->Allocator_Temp, "<%.*s>",   path.Len, path.Ptr )
		:	strbuilder_fmt_buf( _ctx->Allocator_Temp, "\"%.*s\"", path.Len, path.Ptr );

	CodeInclude
	result          = (CodeInclude) make_code();
	result->Type    = CT_Preprocess_Include;
	result->Name    = cache_str( strbuilder_to_str(content) );
	result->Content = result->Name;
	return result;
}

CodeModule def_module( Str name, Opts_def_module p )
{
	if ( ! name_check( def_module, name )) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeModule
	result              = (CodeModule) make_code();
	result->Type        = CT_Module;
	result->Name        = cache_str( name );
	result->ModuleFlags = p.mflags;
	return result;
}

CodeNS def_namespace( Str name, CodeBody body, Opts_def_namespace p )
{
	if ( ! name_check( def_namespace, name )) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( ! null_check( def_namespace, body)) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( body && body->Type != CT_Namespace_Body && body->Type != CT_Untyped ) {
		log_failure("gen::def_namespace: body is not of namespace or untyped type %s", code_debug_str(body));
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeNS
	result              = (CodeNS) make_code();
	result->Type        = CT_Namespace;
	result->Name        = cache_str( name );
	result->ModuleFlags = p.mflags;
	result->Body        = body;
	return result;
}

CodeOperator def_operator( Operator op, Str nspace, Opts_def_operator p )
{
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_operator: PlatformAttributes was provided but its not of attributes type: %s", code_debug_str(p.attributes) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.specifiers && p.specifiers->Type != CT_Specifiers ) {
		log_failure( "gen::def_operator: Specifiers was provided but its not of specifiers type: %s", code_debug_str(p.specifiers) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	OpValidateResult check_result = operator__validate( op, p.params, p.ret_type, p.specifiers );
	if ( check_result == OpValResult_Fail ) {
		return InvalidCode;
	}

	char const* name = nullptr;

	Str op_str = operator_to_str( op );
	if ( nspace.Len > 0 )
		name = c_str_fmt_buf( "%.*soperator %.*s", nspace.Len, nspace.Ptr, op_str.Len, op_str.Ptr );
	else
		name = c_str_fmt_buf( "operator %.*s", op_str.Len, op_str.Ptr );

	Str name_resolved = { name, c_str_len(name) };

	CodeOperator
	result              = (CodeOperator) make_code();
	result->Name        = cache_str( name_resolved );
	result->ModuleFlags = p.mflags;
	result->Op          = op;
	if ( p.body )
	{
		switch ( p.body->Type )
		{
			case CT_Function_Body:
			case CT_Execution:
			case CT_Untyped:
				break;

			default:
			{
				log_failure("gen::def_operator: body must be either of Function_Body, Execution, or Untyped type. %s", code_debug_str(p.body));
				GEN_DEBUG_TRAP();
				return InvalidCode;
			}
		}

		result->Type = check_result == OpValResult_Global ?
			CT_Operator : CT_Operator_Member;

		result->Body = p.body;
	}
	else
	{
		result->Type = check_result == OpValResult_Global ?
			CT_Operator_Fwd : CT_Operator_Member_Fwd;
	}
	result->Attributes = p.attributes;
	result->Specs      = p.specifiers;
	result->ReturnType = p.ret_type;
	result->Params     = p.params;
	return result;
}

CodeOpCast def_operator_cast( CodeTypename type, Opts_def_operator_cast p )
{
	if ( ! null_check( def_operator_cast, type )) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( type->Type != CT_Typename ) {
		log_failure( "gen::def_operator_cast: type is not a typename - %s", code_debug_str(type) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeOpCast result = (CodeOpCast) make_code();
	if (p.body)
	{
		result->Type = CT_Operator_Cast;

		if ( p.body->Type != CT_Function_Body && p.body->Type != CT_Execution ) {
			log_failure( "gen::def_operator_cast: body is not of function body or execution type - %s", code_debug_str(p.body) );
			GEN_DEBUG_TRAP();
			return InvalidCode;
		}
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Operator_Cast_Fwd;
	}
	result->Specs     = p.specs;
	result->ValueType = type;
	return result;
}

CodeParams def_param( CodeTypename type, Str name, Opts_def_param p )
{
	if ( ! name_check( def_param, name ) || ! null_check( def_param, type ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( type->Type != CT_Typename ) {
		log_failure( "gen::def_param: type is not a typename - %s", code_debug_str(type) );
		return InvalidCode;
	}
	if ( p.value && p.value->Type != CT_Untyped ) {
		log_failure( "gen::def_param: value is not untyped - %s", code_debug_str(p.value) );
		return InvalidCode;
	}
	CodeParams
	result            = (CodeParams) make_code();
	result->Type      = CT_Parameters;
	result->Name      = cache_str( name );
	result->ValueType = type;
	result->Value     = p.value;
	result->NumEntries++;
	return result;
}

CodePragma def_pragma( Str directive )
{
	if ( directive.Len <= 0 || directive.Ptr == nullptr ) {
		log_failure( "gen::def_comment: Invalid comment provided:" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodePragma
	result          = (CodePragma) make_code();
	result->Type    = CT_Preprocess_Pragma;
	result->Content = cache_str( directive );
	return result;
}

CodePreprocessCond def_preprocess_cond( EPreprocessCond type, Str expr )
{
	if ( expr.Len <= 0 || expr.Ptr == nullptr ) {
		log_failure( "gen::def_comment: Invalid comment provided:" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodePreprocessCond
	result          = (CodePreprocessCond) make_code();
	result->Content = cache_str( expr );
	switch (type)
	{
		case PreprocessCond_If:
			result->Type = CT_Preprocess_If;
		break;
		case PreprocessCond_IfDef:
			result->Type = CT_Preprocess_IfDef;
		break;
		case PreprocessCond_IfNotDef:
			result->Type = CT_Preprocess_IfNotDef;
		break;
		case PreprocessCond_ElIf:
			result->Type = CT_Preprocess_ElIf;
		break;
	}
	return result;
}

CodeSpecifiers def_specifier( Specifier spec )
{
	CodeSpecifiers
	result       = (CodeSpecifiers) make_code();
	result->Type = CT_Specifiers;
	specifiers_append(result, spec );
	return result;
}

CodeStruct def_struct( Str name, Opts_def_struct p )
{
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_struct: attributes was not a `PlatformAttributes` type - %s", code_debug_str(cast(Code, p.attributes)) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.parent && p.parent->Type != CT_Typename ) {
		log_failure( "gen::def_struct: parent was not a `Struct` type - %s", code_debug_str(p.parent) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.body && p.body->Type != CT_Struct_Body ) {
		log_failure( "gen::def_struct: body was not a Struct_Body type - %s", code_debug_str(p.body) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeStruct
	result              = (CodeStruct) make_code();
	result->ModuleFlags = p.mflags;
	if ( name.Len )
		result->Name = cache_str( name );

	if ( p.body ) {
		result->Type = CT_Struct;
		result->Body = p.body;
	}
	else {
		result->Type = CT_Struct_Fwd;
	}
	result->Attributes   = p.attributes;
	result->ParentAccess = p.parent_access;
	result->ParentType   = p.parent;

	for (s32 idx = 0; idx < p.num_interfaces; idx++ ) {
		struct_add_interface(result, p.interfaces[idx] );
	}
	return result;
}

CodeTemplate def_template( CodeParams params, Code declaration, Opts_def_template p )
{
	if ( ! null_check( def_template, declaration ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( params && params->Type != CT_Parameters ){
		log_failure( "gen::def_template: params is not of parameters type - %s", code_debug_str(params) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	switch (declaration->Type )
	{
		case CT_Class:
		case CT_Function:
		case CT_Struct:
		case CT_Variable:
		case CT_Using:
		break;

		default:
			log_failure( "gen::def_template: declaration is not of class, function, struct, variable, or using type - %s", code_debug_str(declaration) );
	}
	CodeTemplate
	result              = (CodeTemplate) make_code();
	result->Type        = CT_Template;
	result->ModuleFlags = p.mflags;
	result->Params      = params;
	result->Declaration = declaration;
	return result;
}

CodeTypename def_type( Str name, Opts_def_type p )
{
	if ( ! name_check( def_type, name )) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	Code           arrayexpr  = p.arrayexpr;
	CodeSpecifiers specifiers = p.specifiers;
	CodeAttributes attributes = p.attributes;
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_type: attributes is not of attributes type - %s", code_debug_str((Code)p.attributes) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.specifiers && p.specifiers->Type != CT_Specifiers ) {
		log_failure( "gen::def_type: specifiers is not of specifiers type - %s", code_debug_str((Code)p.specifiers) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.arrayexpr && p.arrayexpr->Type != CT_Untyped ) {
		log_failure( "gen::def_type: arrayexpr is not of untyped type - %s", code_debug_str((Code)p.arrayexpr) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeTypename
	result             = (CodeTypename) make_code();
	result->Name       = cache_str( name );
	result->Type       = CT_Typename;
	result->Attributes = p.attributes;
	result->Specs      = p.specifiers;
	result->ArrExpr    = p.arrayexpr;
	result->TypeTag    = p.type_tag;
	return result;
}

CodeTypedef def_typedef( Str name, Code type, Opts_def_typedef p )
{
	if ( ! null_check( def_typedef, type ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	switch ( type->Type )
	{
		case CT_Class:
		case CT_Class_Fwd:
		case CT_Enum:
		case CT_Enum_Fwd:
		case CT_Enum_Class:
		case CT_Enum_Class_Fwd:
		case CT_Function_Fwd:
		case CT_Struct:
		case CT_Struct_Fwd:
		case CT_Union:
		case CT_Typename:
			break;
		default:
			log_failure( "gen::def_typedef: type was not a Class, Enum, Function Forward, Struct, Typename, or Union - %s", code_debug_str((Code)type) );
			GEN_DEBUG_TRAP();
			return InvalidCode;
	}
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_typedef: attributes was not a PlatformAttributes - %s", code_debug_str((Code)p.attributes) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	// Registering the type.
	CodeTypename registered_type = def_type( name );
	if ( ! registered_type ) {
		log_failure( "gen::def_typedef: failed to register type" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeTypedef
	result                 = (CodeTypedef) make_code();
	result->Type           = CT_Typedef;
	result->ModuleFlags    = p.mflags;
	result->UnderlyingType = type;

	if ( name.Len <= 0  )
	{
		if (type->Type != CT_Untyped) {
			log_failure( "gen::def_typedef: name was empty and type was not untyped (indicating its a function typedef) - %s", code_debug_str(type) );
			GEN_DEBUG_TRAP();
			return InvalidCode;
		}
		result->Name       = cache_str( type->Name );
		result->IsFunction = true;
	}
	else
	{
		result->Name       = cache_str( name );
		result->IsFunction = false;
	}
	return result;
}

CodeUnion def_union( Str name, CodeBody body, Opts_def_union p )
{
	if ( ! null_check( def_union, body ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( body->Type != CT_Union_Body ) {
		log_failure( "gen::def_union: body was not a Union_Body type - %s", code_debug_str(body) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_union: attributes was not a PlatformAttributes type - %s", code_debug_str(p.attributes) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeUnion
	result              = (CodeUnion) make_code();
	result->ModuleFlags = p.mflags;
	result->Type        = CT_Union;
	result->Body        = body;
	result->Attributes  = p.attributes;
	if ( name.Ptr )
		result->Name = cache_str( name );
	return result;
}

CodeUsing def_using( Str name, CodeTypename type, Opts_def_using p )
{
	if ( ! name_check( def_using, name ) || null_check( def_using, type ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}

	CodeTypename register_type = def_type( name );
	if ( ! register_type ) {
		log_failure( "gen::def_using: failed to register type" );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes ) {
		log_failure( "gen::def_using: attributes was not a PlatformAttributes type - %s", code_debug_str(p.attributes) );
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeUsing
	result                 = (CodeUsing) make_code();
	result->Name           = cache_str( name );
	result->ModuleFlags    = p.mflags;
	result->Type           = CT_Using;
	result->UnderlyingType = type;
	result->Attributes     = p.attributes;
	return result;
}

CodeUsing def_using_namespace( Str name )
{
	if ( ! name_check( def_using_namespace, name ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	CodeUsing
	result          = (CodeUsing) make_code();
	result->Name    = cache_str( name );
	result->Type    = CT_Using_Namespace;
	return result;
}

CodeVar def_variable( CodeTypename type, Str name, Opts_def_variable p )
{
	if ( ! name_check( def_variable, name ) || null_check( def_variable, type ) ) {
		GEN_DEBUG_TRAP();
		return InvalidCode;
	}
	if ( p.attributes && p.attributes->Type != CT_PlatformAttributes )
	{
		log_failure( "gen::def_variable: attributes was not a `PlatformAttributes` type - %s", code_debug_str(p.attributes) );
		return InvalidCode;
	}
	if ( p.specifiers && p.specifiers->Type != CT_Specifiers )
	{
		log_failure( "gen::def_variable: specifiers was not a `Specifiers` type - %s", code_debug_str(p.specifiers) );
		return InvalidCode;
	}
	if ( type->Type != CT_Typename )
	{
		log_failure( "gen::def_variable: type was not a Typename - %s", code_debug_str(type) );
		return InvalidCode;
	}
	if ( p.value && p.value->Type != CT_Untyped )
	{
		log_failure( "gen::def_variable: value was not a `Untyped` type - %s", code_debug_str(p.value) );
		return InvalidCode;
	}
	CodeVar
	result              = (CodeVar) make_code();
	result->Name        = cache_str( name );
	result->Type        = CT_Variable;
	result->ModuleFlags = p.mflags;
	result->ValueType   = type;
	result->Attributes  = p.attributes;
	result->Specs       = p.specifiers;
	result->Value       = p.value;
	return result;
}

#pragma region Helper Macros for def_**_body functions
#define def_body_start( Name_ )                                               \
if ( num <= 0 )                                                               \
{                                                                             \
	log_failure("gen::" stringize(Name_) ": num cannot be zero or negative"); \
	return InvalidCode;                                                       \
}

#define def_body_code_array_start( Name_ )                                     \
if ( num <= 0 )                                                                \
{                                                                              \
	log_failure("gen::" stringize(Name_) ": num cannot be zero or negative");  \
	return InvalidCode;                                                        \
}                                                                              \
																			   \
if ( codes == nullptr )                                                        \
{                                                                              \
	log_failure("gen::" stringize(Name_)" : Provided a null array of codes");  \
	return InvalidCode;                                                        \
}

#pragma endregion Helper Macros for def_**_body functions

CodeBody def_class_body( s32 num, ... )
{
	def_body_start( def_class_body );

	CodeBody result = ( CodeBody )make_code();
	result->Type    = CT_Class_Body;

	va_list va;
	va_start( va, num );
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry) {
			log_failure("gen::"
						"def_class_body"
						": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_CLASS_UNALLOWED_TYPES:
				log_failure("gen::" "def_class_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);
	va_end(va);

	return result;
}

CodeBody def_class_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_class_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Function_Body;
	do
	{
		Code entry = *codes;
		codes++;
		if ( ! entry) {
			log_failure("gen::" "def_class_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_CLASS_UNALLOWED_TYPES:
				log_failure("gen::" "def_class_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);

	return result;
}

CodeBody def_enum_body( s32 num, ... )
{
	def_body_start( def_enum_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Enum_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry ) {
			log_failure("gen::def_enum_body: Provided a null entry");
			return InvalidCode;
		}
		if ( entry->Type != CT_Untyped && entry->Type != CT_Comment ) {
			log_failure("gen::def_enum_body: Entry type is not allowed - %s. Must be of untyped or comment type.", code_debug_str(entry) );
			return InvalidCode;
		}
		body_append(result, entry );
	}
	while ( num--, num > 0 );
	va_end(va);

	return (CodeBody) result;
}

CodeBody def_enum_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_enum_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Enum_Body;
	do
	{
		Code entry = *codes;
		if ( ! entry ) {
			log_failure("gen::def_enum_body: Provided a null entry");
			return InvalidCode;
		}
		if ( entry->Type != CT_Untyped && entry->Type != CT_Comment ) {
			log_failure("gen::def_enum_body: Entry type is not allowed: %s", code_debug_str(entry) );
			return InvalidCode;
		}
		body_append(result, entry );
	}
	while ( codes++, num--, num > 0 );

	return result;
}

CodeBody def_export_body( s32 num, ... )
{
	def_body_start( def_export_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Export_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry)
		{
			log_failure("gen::" "def_export_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_EXPORT_UNALLOWED_TYPES:
				log_failure("gen::" "def_export_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);
	va_end(va);

	return result;
}

CodeBody def_export_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_export_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Export_Body;
	do
	{
		Code entry = *codes;
		codes++;
		if ( ! entry) {
			log_failure("gen::" "def_export_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_EXPORT_UNALLOWED_TYPES:
				log_failure("gen::" "def_export_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);

	return result;
}

CodeBody def_extern_link_body( s32 num, ... )
{
	def_body_start( def_extern_linkage_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Extern_Linkage_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry) {
			log_failure("gen::" "def_extern_linkage_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES:
				log_failure("gen::" "def_extern_linkage_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);
	va_end(va);

	return result;
}

CodeBody def_extern_link_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_extern_linkage_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Extern_Linkage_Body;
	do
	{
		Code entry = *codes;
		codes++;
		if (!entry)
		{
			log_failure("gen::" "def_extern_linkage_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES:
				log_failure("gen::" "def_extern_linkage_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);

	return result;
}

CodeBody def_function_body( s32 num, ... )
{
	def_body_start( def_function_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Function_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry) {
			log_failure("gen::" stringize(def_function_body) ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES:
				log_failure("gen::" stringize(def_function_body) ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);
	va_end(va);

	return result;
}

CodeBody def_function_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_function_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Function_Body;
	do
	{
		Code entry = *codes;
		codes++;
		if (!entry) {
			log_failure("gen::" "def_function_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES:
				log_failure("gen::" "def_function_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);

	return result;
}

CodeBody def_global_body( s32 num, ... )
{
	def_body_start( def_global_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Global_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry) {
			log_failure("gen::" "def_global_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			case CT_Global_Body:
				// result.body_append( entry.code_cast<CodeBody>() ) ;
				body_append_body( result, cast(CodeBody, entry) );
				continue;

			GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES:
				log_failure("gen::" "def_global_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);
	va_end(va);

	return result;
}

CodeBody def_global_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_global_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Global_Body;
	do
	{
		Code entry = *codes;
		codes++;
		if ( ! entry) {
			log_failure("gen::" "def_global_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			case CT_Global_Body:
				body_append_body(result, cast(CodeBody, entry) );
				continue;

			GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES:
				log_failure("gen::" "def_global_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}

		body_append(result, entry);
	}
	while (num--, num > 0);

	return result;
}

CodeBody def_namespace_body( s32 num, ... )
{
	def_body_start( def_namespace_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Namespace_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry) {
			log_failure("gen::" "def_namespace_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES:
				log_failure("gen::" "def_namespace_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);
	va_end(va);

	return result;
}

CodeBody def_namespace_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_namespace_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Global_Body;
	do
	{
		Code entry = *codes;
		codes++;
		if ( ! entry) {
			log_failure("gen::" "def_namespace_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES:
				log_failure("gen::" "def_namespace_body" ": Entry type is not allowed: %s", code_debug_str(entry) );
				return InvalidCode;

			default: break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);

	return result;
}

CodeParams def_params( s32 num, ... )
{
	def_body_start( def_params );

	va_list va;
	va_start(va, num);

	Code_POD  pod   = va_arg(va, Code_POD);
	CodeParams param = pcast( CodeParams, pod );

	null_check( def_params, param );
	if ( param->Type != CT_Parameters ) {
		log_failure( "gen::def_params: param %d is not a Parameters", num - num + 1 );
		return InvalidCode;
	}

	CodeParams result = (CodeParams) code_duplicate(param);
	while ( -- num )
	{
		pod   = va_arg(va, Code_POD);
		param = pcast( CodeParams, pod );
		if ( param->Type != CT_Parameters ) {
			log_failure( "gen::def_params: param %d is not a Parameters", num - num + 1 );
			return InvalidCode;
		}
		params_append(result, param );
	}
	va_end(va);

	return result;
}

CodeParams def_params( s32 num, CodeParams* codes )
{
	def_body_code_array_start( def_params );

#	define check_current(current)                                                                                      \
	if ( current == nullptr ) {                                                                                        \
		log_failure("gen::def_params: Provide a null code in codes array");                                            \
		return InvalidCode;                                                                                            \
	}                                                                                                                  \
	if (current->Type != CT_Parameters ) {                                                                             \
		log_failure("gen::def_params: Code in coes array is not of paramter type - %s", code_debug_str(current) );     \
		return InvalidCode;                                                                                            \
	}
	CodeParams current = (CodeParams)code_duplicate(* codes);
	check_current(current);

	CodeParams
	result            = (CodeParams) make_code();
	result->Name      = current->Name;
	result->Type      = current->Type;
	result->ValueType = current->ValueType;
	while( codes++, current = * codes, num--, num > 0 ) {
		check_current(current);
		params_append(result, current );
	}
#	undef check_current

	return result;
}

CodeSpecifiers def_specifiers( s32 num, ... )
{
	if ( num <= 0 ) {
		log_failure("gen::def_specifiers: num cannot be zero or less");
		return InvalidCode;
	}
	if ( num > AST_ArrSpecs_Cap ) {
		log_failure("gen::def_specifiers: num of speciifers to define AST larger than AST specicifier capacity - %d", num);
		return InvalidCode;
	}
	CodeSpecifiers
	result       = (CodeSpecifiers) make_code();
	result->Type = CT_Specifiers;

	va_list va;
	va_start(va, num);
	do {
		Specifier type = (Specifier)va_arg(va, int);
		specifiers_append(result, type );
	}
	while ( --num, num );
	va_end(va);

	return result;
}

CodeSpecifiers def_specifiers( s32 num, Specifier* specs )
{
	if ( num <= 0 ) {
		log_failure("gen::def_specifiers: num cannot be zero or less");
		return InvalidCode;
	}
	if ( num > AST_ArrSpecs_Cap ) {
		log_failure("gen::def_specifiers: num of speciifers to define AST larger than AST specicifier capacity - %d", num);
		return InvalidCode;
	}
	CodeSpecifiers
	result       = (CodeSpecifiers) make_code();
	result->Type = CT_Specifiers;

	s32 idx = 0;
	do {
		specifiers_append(result, specs[idx] );
		idx++;
	}
	while ( --num, num );

	return result;
}

CodeBody def_struct_body( s32 num, ... )
{
	def_body_start( def_struct_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Struct_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast(Code, pod);
		if ( ! entry) {
			log_failure("gen::" "def_struct_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_STRUCT_UNALLOWED_TYPES:
				log_failure("gen::" "def_struct_body" ": Entry type is not allowed: %s", code_debug_str(entry));
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);
	va_end(va);

	return result;
}

CodeBody def_struct_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_struct_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Struct_Body;
	do
	{
		Code entry = *codes;
		codes++;
		if ( ! entry) {
			log_failure("gen::" "def_struct_body" ": Provided an null entry");
			return InvalidCode;
		}
		switch (entry->Type)
		{
			GEN_AST_BODY_STRUCT_UNALLOWED_TYPES:
				log_failure("gen::" "def_struct_body" ": Entry type is not allowed: %s", code_debug_str(entry) );
				return InvalidCode;

			default:
			break;
		}
		body_append(result, entry);
	}
	while (num--, num > 0);

	return result;
}

CodeBody def_union_body( s32 num, ... )
{
	def_body_start( def_union_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Union_Body;

	va_list va;
	va_start(va, num);
	do
	{
		Code_POD pod   = va_arg(va, Code_POD);
		Code     entry = pcast( Code, pod );
		if ( ! entry ) {
			log_failure("gen::def_union_body: Provided a null entry");
			return InvalidCode;
		}
		if ( entry->Type != CT_Untyped && entry->Type != CT_Comment ) {
			log_failure("gen::def_union_body: Entry type is not allowed - %s. Must be of untyped or comment type.", code_debug_str(entry) );
			return InvalidCode;
		}
		body_append(result, entry );
	}
	while ( num--, num > 0 );
	va_end(va);

	return result;
}

CodeBody def_union_body( s32 num, Code* codes )
{
	def_body_code_array_start( def_union_body );

	CodeBody
	result       = (CodeBody) make_code();
	result->Type = CT_Union_Body;
	do
	{
		Code entry = *codes;
		if ( ! entry ) {
			log_failure("gen::def_union_body: Provided a null entry");
			return InvalidCode;
		}
		if ( entry->Type != CT_Untyped && entry->Type != CT_Comment ) {
			log_failure("gen::def_union_body: Entry type is not allowed: %s", code_debug_str(entry) );
			return InvalidCode;
		}
		body_append(result, entry );
	}
	while ( codes++, num--, num > 0 );

	return (CodeBody) result;
}

#	undef name_check
#	undef null_check
#	undef def_body_start
#	undef def_body_code_array_start

#pragma endregion Upfront

#pragma region Parsing


StrBuilder tok_to_strbuilder(Token tok)
{
	StrBuilder result   = strbuilder_make_reserve( _ctx->Allocator_Temp, kilobytes(4) );
	Str        type_str = toktype_to_str( tok.Type );

	strbuilder_append_fmt( & result, "Line: %d Column: %d, Type: %.*s Content: %.*s"
		, tok.Line, tok.Column
		, type_str.Len, type_str.Ptr
		, tok.Text.Len, tok.Text.Ptr
	);
	return result;
}

bool lex__eat( TokArray* self, TokType type );

Token* lex_current(TokArray* self, bool skip_formatting )
{
	if ( skip_formatting )
	{
		while ( self->Arr[self->Idx].Type == Tok_NewLine || self->Arr[self->Idx].Type == Tok_Comment  )
			self->Idx++;
	}
	return & self->Arr[self->Idx];
}

Token* lex_peek(TokArray self, bool skip_formatting)
{
	s32 idx = self.Idx;
	if ( skip_formatting )
	{
		while ( self.Arr[idx].Type == Tok_NewLine )
			idx++;

		return & self.Arr[idx];
	}
	return & self.Arr[idx];
}

Token* lex_previous(TokArray self, bool skip_formatting)
{
	s32 idx = self.Idx;
	if ( skip_formatting )
	{
		while ( self.Arr[idx].Type == Tok_NewLine )
			idx --;

		return & self.Arr[idx];
	}
	return & self.Arr[idx - 1];
}

Token* lex_next(TokArray self, bool skip_formatting)
{
	s32 idx = self.Idx;
	if ( skip_formatting )
	{
		while ( self.Arr[idx].Type == Tok_NewLine )
			idx++;

		return & self.Arr[idx + 1];
	}
	return & self.Arr[idx + 1];
}

enum
{
	Lex_Continue,
	Lex_ReturnNull,
};

forceinline
void lexer_move_forward( LexContext* ctx )
{
	if ( * ctx->scanner == '\n' ) {
		ctx->line   += 1;
		ctx->column  = 1;
	}
	else {
		++ ctx->column;
	}
	-- ctx->left;
	++ ctx->scanner;
}
#define move_forward() lexer_move_forward(ctx)

forceinline
void lexer_skip_whitespace( LexContext* ctx )
{
	while ( ctx->left && char_is_space( * ctx->scanner ) )
		move_forward();
}
#define skip_whitespace() lexer_skip_whitespace(ctx)

forceinline
void lexer_end_line( LexContext* ctx )
{
	while ( ctx->left && (* ctx->scanner) == ' ' )
		move_forward();

	if ( ctx->left && (* ctx->scanner) == '\r' ) {
		move_forward();
		move_forward();
	}
	else if ( ctx->left && (* ctx->scanner) == '\n' )
		move_forward();
}
#define end_line() lexer_end_line(ctx)

forceinline
s32 lex_preprocessor_directive( LexContext* ctx )
{
	char const* hash = ctx->scanner;
	Token hash_tok = { { hash, 1 }, Tok_Preprocess_Hash, ctx->line, ctx->column, TF_Preprocess };
	array_append( _ctx->Lexer_Tokens, hash_tok  );

	move_forward();
	skip_whitespace();

	ctx->token.Text.Ptr = ctx->scanner;
	while (ctx->left && ! char_is_space((* ctx->scanner)) )
	{
		move_forward();
		ctx->token.Text.Len++;
	}

	ctx->token.Type = str_to_toktype( tok_to_str(ctx->token) );

	bool   is_preprocessor = ctx->token.Type >= Tok_Preprocess_Define && ctx->token.Type <= Tok_Preprocess_Pragma;
	if ( ! is_preprocessor )
	{
		ctx->token.Type  = Tok_Preprocess_Unsupported;

		// Its an unsupported directive, skip it
		s32 within_string = false;
		s32 within_char   = false;
		while ( ctx->left )
		{
			if ( * ctx->scanner == '"' && ! within_char )
				within_string ^= true;

			if ( * ctx->scanner == '\'' && ! within_string )
				within_char ^= true;

			if ( * ctx->scanner == '\\' && ! within_string && ! within_char )
			{
				move_forward();
				ctx->token.Text.Len++;

				if ( (* ctx->scanner) == '\r' )
				{
					move_forward();
					ctx->token.Text.Len++;
				}

				if ( (* ctx->scanner) == '\n' )
				{
					move_forward();
					ctx->token.Text.Len++;
					continue;
				}
				else
				{
					log_failure( "gen::Parser::lex: Invalid escape sequence '\\%c' (%d, %d)"
								" in preprocessor directive (%d, %d)\n%.100s"
						, (* ctx->scanner), ctx->line, ctx->column
						, ctx->token.Line, ctx->token.Column, ctx->token.Text );
					break;
				}
			}

			if ( (* ctx->scanner) == '\r' )
			{
				move_forward();
				ctx->token.Text.Len++;
			}

			if ( (* ctx->scanner) == '\n' )
			{
				move_forward();
				ctx->token.Text.Len++;
				break;
			}

			move_forward();
			ctx->token.Text.Len++;
		}

		ctx->token.Text.Len = ctx->token.Text.Len + ctx->token.Text.Ptr - hash;
		ctx->token.Text.Ptr = hash;
		array_append( _ctx->Lexer_Tokens, ctx->token );
		return Lex_Continue; // Skip found token, its all handled here.
	}

	if ( ctx->token.Type == Tok_Preprocess_Else || ctx->token.Type == Tok_Preprocess_EndIf )
	{
		ctx->token.Flags |= TF_Preprocess_Cond;
		array_append( _ctx->Lexer_Tokens, ctx->token );
		end_line();
		return Lex_Continue;
	}
	else if ( ctx->token.Type >= Tok_Preprocess_If && ctx->token.Type <= Tok_Preprocess_ElIf  )
	{
		ctx->token.Flags |= TF_Preprocess_Cond;
	}

	array_append( _ctx->Lexer_Tokens, ctx->token );

	skip_whitespace();

	if ( ctx->token.Type == Tok_Preprocess_Define )
	{
		Token name = { { ctx->scanner, 0 }, Tok_Identifier, ctx->line, ctx->column, TF_Preprocess };

		name.Text.Ptr = ctx->scanner;
		name.Text.Len = 1;
		move_forward();

		while ( ctx->left && ( char_is_alphanumeric((* ctx->scanner)) || (* ctx->scanner) == '_' ) )
		{
			move_forward();
			name.Text.Len++;
		}

		if ( ctx->left && (* ctx->scanner) == '(' )
		{
			move_forward();
			name.Text.Len++;
		}

		array_append( _ctx->Lexer_Tokens, name );

		u64 key = crc32( name.Text.Ptr, name.Text.Len );
		hashtable_set(ctx->defines, key, tok_to_str(name) );
	}

	Token preprocess_content = { { ctx->scanner, 0 }, Tok_Preprocess_Content, ctx->line, ctx->column, TF_Preprocess };

	if ( ctx->token.Type == Tok_Preprocess_Include )
	{
		preprocess_content.Type = Tok_String;

		if ( (* ctx->scanner) != '"' && (* ctx->scanner) != '<' )
		{
			StrBuilder directive_str = strbuilder_fmt_buf( _ctx->Allocator_Temp, "%.*s", min( 80, ctx->left + preprocess_content.Text.Len ), ctx->token.Text.Ptr );

			log_failure( "gen::Parser::lex: Expected '\"' or '<' after #include, not '%c' (%d, %d)\n%s"
				, (* ctx->scanner)
				, preprocess_content.Line
				, preprocess_content.Column
				, (char*) directive_str
			);
			return Lex_ReturnNull;
		}
		move_forward();
		preprocess_content.Text.Len++;

		while ( ctx->left && (* ctx->scanner) != '"' && (* ctx->scanner) != '>' )
		{
			move_forward();
			preprocess_content.Text.Len++;
		}

		move_forward();
		preprocess_content.Text.Len++;

		if ( (* ctx->scanner) == '\r' && ctx->scanner[1] == '\n' )
		{
			move_forward();
			move_forward();
		}
		else if ( (* ctx->scanner) == '\n' )
		{
			move_forward();
		}

		array_append( _ctx->Lexer_Tokens, preprocess_content );
		return Lex_Continue; // Skip found token, its all handled here.
	}

	s32 within_string = false;
	s32 within_char   = false;

	// SkipWhitespace();
	while ( ctx->left )
	{
		if ( (* ctx->scanner) == '"' && ! within_char )
			within_string ^= true;

		if ( (* ctx->scanner) == '\'' && ! within_string )
			within_char ^= true;

		if ( (* ctx->scanner) == '\\' && ! within_string && ! within_char )
		{
			move_forward();
			preprocess_content.Text.Len++;

			if ( (* ctx->scanner) == '\r' )
			{
				move_forward();
				preprocess_content.Text.Len++;
			}

			if ( (* ctx->scanner) == '\n' )
			{
				move_forward();
				preprocess_content.Text.Len++;
				continue;
			}
			else
			{
				StrBuilder directive_str = strbuilder_make_length( _ctx->Allocator_Temp, ctx->token.Text.Ptr, ctx->token.Text.Len );
				StrBuilder content_str   = strbuilder_fmt_buf( _ctx->Allocator_Temp, "%.*s", min( 400, ctx->left + preprocess_content.Text.Len ), preprocess_content.Text.Ptr );

				log_failure( "gen::Parser::lex: Invalid escape sequence '\\%c' (%d, %d)"
							" in preprocessor directive '%s' (%d, %d)\n%s"
					, (* ctx->scanner), ctx->line, ctx->column
					, directive_str, preprocess_content.Line, preprocess_content.Column
					, content_str );
				break;
			}
		}

		if ( (* ctx->scanner) == '\r' )
		{
			break;
			//move_forward();
		}

		if ( (* ctx->scanner) == '\n' )
		{
			//move_forward();
			break;
		}

		move_forward();
		preprocess_content.Text.Len++;
	}

	array_append( _ctx->Lexer_Tokens, preprocess_content );
	return Lex_Continue; // Skip found token, its all handled here.
}

forceinline
void lex_found_token( LexContext* ctx )
{
	if ( ctx->token.Type != Tok_Invalid )
	{
		array_append( _ctx->Lexer_Tokens, ctx->token );
		return;
	}

	TokType type = str_to_toktype( tok_to_str(ctx->token) );

	if (type <= Tok_Access_Public && type >= Tok_Access_Private )
	{
		ctx->token.Flags |= TF_AccessSpecifier;
	}

	if ( type > Tok___Attributes_Start )
	{
		ctx->token.Flags |= TF_Attribute;
	}

	if ( type == Tok_Decl_Extern_Linkage )
	{
		skip_whitespace();

		if ( (* ctx->scanner) != '"' )
		{
			type         = Tok_Spec_Extern;
			ctx->token.Flags |= TF_Specifier;
		}

		ctx->token.Type = type;
		array_append( _ctx->Lexer_Tokens, ctx->token );
		return;
	}

	if ( ( type <= Tok_Star && type >= Tok_Spec_Alignas)
			|| type == Tok_Ampersand
			|| type == Tok_Ampersand_DBL )
	{
		ctx->token.Type   = type;
		ctx->token.Flags |= TF_Specifier;
		array_append( _ctx->Lexer_Tokens, ctx->token );
		return;
	}


	if ( type != Tok_Invalid )
	{
		ctx->token.Type = type;
		array_append( _ctx->Lexer_Tokens, ctx->token );
		return;
	}

	u64 key = 0;
	if ( (* ctx->scanner) == '(')
		key = crc32( ctx->token.Text.Ptr, ctx->token.Text.Len + 1 );
	else
		key = crc32( ctx->token.Text.Ptr, ctx->token.Text.Len );

	Str* define = hashtable_get(ctx->defines, key );
	if ( define )
	{
		ctx->token.Type = Tok_Preprocess_Macro;

		// Want to ignore any arguments the define may have as they can be execution expressions.
		if ( ctx->left && (* ctx->scanner) == '(' )
		{
			move_forward();
			ctx->token.Text.Len++;

			s32 level = 0;
			while ( ctx->left && ((* ctx->scanner) != ')' || level > 0) )
			{
				if ( (* ctx->scanner) == '(' )
					level++;

				else if ( (* ctx->scanner) == ')' && level > 0 )
					level--;

				move_forward();
				ctx->token.Text.Len++;
			}

			move_forward();
			ctx->token.Text.Len++;
		}

		//if ( (* ctx->scanner) == '\r' && ctx->scanner[1] == '\n' )
		//{
		//	move_forward();
		//	ctx->token..Text.Length++;
		//}
		//else if ( (* ctx->scanner) == '\n' )
		//{
		//	move_forward();
		//	ctx->token..Text.Length++;
		//}
	}
	else
	{
		ctx->token.Type = Tok_Identifier;
	}

	array_append( _ctx->Lexer_Tokens, ctx->token );
}

neverinline
// TokArray lex( Array<Token> tokens, Str content )
TokArray lex( Str content )
{
	LexContext c; LexContext* ctx = & c;
	c.content = content;
	c.left    = content.Len;
	c.scanner = content.Ptr;
	c.defines = _ctx->Lexer_defines;

	char const* word        = c.scanner;
	s32         word_length = 0;

	c.line   = 1;
	c.column = 1;

	skip_whitespace();
	if ( c.left <= 0 )
	{
		log_failure( "gen::lex: no tokens found (only whitespace provided)" );
		TokArray null_array = {};
		return null_array;
	}

	for ( StrCached* entry = array_begin(_ctx->PreprocessorDefines); entry != array_end(_ctx->PreprocessorDefines); entry = array_next(_ctx->PreprocessorDefines, entry))
	{
		s32         length  = 0;
		char const* entry_scanner = (*entry).Ptr;
		while ( entry->Len > length && (char_is_alphanumeric( *entry_scanner ) || *entry_scanner == '_') )
		{
			entry_scanner++;
			length ++;
		}
		if ( entry_scanner[0] == '(' )
		{
			length++;
		}

		u64 key = crc32( entry->Ptr, length );
		hashtable_set(c.defines, key, * entry );
	}

	array_clear(_ctx->Lexer_Tokens);

	while (c.left )
	{
		#if 0
		if (Tokens.num())
		{
			log_fmt("\nLastTok: %SB", Tokens.back().to_strbuilder());
		}
		#endif

		{
			Token thanks_c = { { c.scanner, 0 }, Tok_Invalid, c.line, c.column, TF_Null };
			c.token = thanks_c;
		}

		bool is_define = false;

		if ( c.column == 1 )
		{
			if ( (* ctx->scanner) == '\r')
			{
				move_forward();
				c.token.Text.Len = 1;
			}

			if ( (* ctx->scanner) == '\n' )
			{
				move_forward();

				c.token.Type = Tok_NewLine;
				c.token.Text.Len++;

				array_append( _ctx->Lexer_Tokens, c.token );
				continue;
			}
		}

		c.token.Text.Len = 0;

		skip_whitespace();
		if ( c.left <= 0 )
			break;

		switch ( (* ctx->scanner) )
		{
			case '#':
			{
				s32 result = lex_preprocessor_directive( ctx );
				switch ( result )
				{
					case Lex_Continue:
					{
						//TokType last_type = Tokens[array_get_header(Tokens)->Num - 2].Type;
						//if ( last_type == Tok_Preprocess_Pragma )
						{
							{
								Token thanks_c = { { c.scanner, 0 }, Tok_Invalid, c.line, c.column, TF_Null };
								c.token = thanks_c;
							}
							if ( (* ctx->scanner) == '\r')
							{
								move_forward();
								c.token.Text.Len = 1;
							}

							if ( (* ctx->scanner) == '\n' )
							{
								c.token.Type = Tok_NewLine;
								c.token.Text.Len++;
								move_forward();

								array_append( _ctx->Lexer_Tokens, c.token );
							}
						}
						continue;
					}

					case Lex_ReturnNull:
					{
						TokArray tok_array =  {};
						return tok_array;
					}
				}
			}
			case '.':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Access_MemberSymbol;
				c.token.Flags  = TF_AccessOperator;

				if (c.left) {
					move_forward();
				}

				if ( (* ctx->scanner) == '.' )
				{
					move_forward();
					if( (* ctx->scanner) == '.' )
					{
						c.token.Text.Len = 3;
						c.token.Type     = Tok_Varadic_Argument;
						c.token.Flags    = TF_Null;
						move_forward();
					}
					else
					{
						StrBuilder context_str = strbuilder_fmt_buf( _ctx->Allocator_Temp, "%s", c.scanner, min( 100, c.left ) );

						log_failure( "gen::lex: invalid varadic argument, expected '...' got '..%c' (%d, %d)\n%s", (* ctx->scanner), c.line, c.column, context_str );
					}
				}

				goto FoundToken;
			}
			case '&' :
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Ampersand;
				c.token.Flags |= TF_Operator;
				c.token.Flags |= TF_Specifier;

				if (c.left)
					move_forward();

				if ( (* ctx->scanner) == '&' )	// &&
				{
					c.token.Text.Len = 2;
					c.token.Type     = Tok_Ampersand_DBL;

					if (c.left)
						move_forward();
				}

				goto FoundToken;
			}
			case ':':
			{
				Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Assign_Classifer;
				// Can be either a classifier (ParentType, Bitfield width), or ternary else
				// token.Type   = Tok_Colon;

				if (c.left)
					move_forward();

				if ( (* ctx->scanner) == ':' )
				{
					move_forward();
					c.token.Type = Tok_Access_StaticSymbol;
					c.token.Text.Len++;
				}
				goto FoundToken;
			}
			case '{':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_BraceCurly_Open;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '}':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_BraceCurly_Close;
				c.token.Flags  = TF_EndDefinition;

				if (c.left)
					move_forward();

				end_line();
				goto FoundToken;
			}
			case '[':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_BraceSquare_Open;
				if ( c.left )
				{
					move_forward();

					if ( (* ctx->scanner) == ']' )
					{
						c.token.Text.Len = 2;
						c.token.Type     = Tok_Operator;
						move_forward();
					}
				}
				goto FoundToken;
			}
			case ']':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_BraceSquare_Close;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '(':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Capture_Start;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case ')':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Capture_End;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '\'':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Char;
				c.token.Flags  = TF_Literal;

				move_forward();

				if ( c.left && (* ctx->scanner) == '\\' )
				{
					move_forward();
					c.token.Text.Len++;

					if ( (* ctx->scanner) == '\'' )
					{
						move_forward();
						c.token.Text.Len++;
					}
				}

				while ( c.left && (* ctx->scanner) != '\'' )
				{
					move_forward();
					c.token.Text.Len++;
				}

				if ( c.left )
				{
					move_forward();
					c.token.Text.Len++;
				}
				goto FoundToken;
			}
			case ',':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Comma;
				c.token.Flags  = TF_Operator;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '*':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Star;
				c.token.Flags |= TF_Specifier;
				c.token.Flags |= TF_Operator;

				if (c.left)
					move_forward();

				if ( (* ctx->scanner) == '=' )
				{
					c.token.Text.Len++;
					c.token.Flags |= TF_Assign;
					// c.token.Type = Tok_Assign_Multiply;

					if ( c.left )
						move_forward();
				}

				goto FoundToken;
			}
			case ';':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Statement_End;
				c.token.Flags  = TF_EndDefinition;

				if (c.left)
					move_forward();

				end_line();
				goto FoundToken;
			}
			case '"':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_String;
				c.token.Flags |= TF_Literal;

				move_forward();
				while ( c.left )
				{
					if ( (* ctx->scanner) == '"' )
					{
						move_forward();
						break;
					}

					if ( (* ctx->scanner) == '\\' )
					{
						move_forward();
						c.token.Text.Len++;

						if ( c.left )
						{
							move_forward();
							c.token.Text.Len++;
						}
						continue;
					}

					move_forward();
					c.token.Text.Len++;
				}
				goto FoundToken;
			}
			case '?':
			{
				Str text = { c.scanner, 1 };
				c.token.Text     = text;
				c.token.Type     = Tok_Operator;
				// c.token.Type     = Tok_Ternary;
				c.token.Flags    = TF_Operator;

				if (c.left)
					move_forward();

				goto FoundToken;
			}
			case '=':
			{
				Str text = { c.scanner, 1 };
				c.token.Text     = text;
				c.token.Type     = Tok_Operator;
				// c.token.Type     = Tok_Assign;
				c.token.Flags    = TF_Operator;
				c.token.Flags   |= TF_Assign;

				if (c.left)
					move_forward();

				if ( (* ctx->scanner) == '=' )
				{
					c.token.Text.Len++;
					c.token.Flags = TF_Operator;

					if (c.left)
						move_forward();
				}

				goto FoundToken;
			}
			case '+':
			{
				// c.token.Type = Tok_Add

			}
			case '%':
			{
				// c.token.Type = Tok_Modulo;

			}
			case '^':
			{
				// c.token.Type = Tok_B_XOr;
			}
			case '~':
			{
				// c.token.Type = Tok_Unary_Not;

			}
			case '!':
			{
				// c.token.Type = Tok_L_Not;
			}
			case '<':
			{
				// c.token.Type = Tok_Lesser;

			}
			case '>':
			{
				// c.token.Type = Tok_Greater;

			}
			case '|':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Operator;
				c.token.Flags  = TF_Operator;
				// token.Type   = Tok_L_Or;

				if (c.left)
					move_forward();

				if ( (* ctx->scanner) == '=' )
				{
					c.token.Text.Len++;
					c.token.Flags |= TF_Assign;
					// token.Flags |= TokFlags::Assignment;
					// token.Type = Tok_Assign_L_Or;

					if (c.left)
						move_forward();
				}
				else while ( c.left && (* ctx->scanner) == *(c.scanner - 1) && c.token.Text.Len < 3 )
				{
					c.token.Text.Len++;

					if (c.left)
						move_forward();
				}
				goto FoundToken;
			}

			// Dash is unfortunatlly a bit more complicated...
			case '-':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Operator;
				// token.Type = Tok_Subtract;
				c.token.Flags  = TF_Operator;
				if ( c.left )
				{
					move_forward();

					if ( (* ctx->scanner) == '>'  )
					{
						c.token.Text.Len++;
//						token.Type = Tok_Access_PointerToMemberSymbol;
						c.token.Flags |= TF_AccessOperator;
						move_forward();

						if ( (* ctx->scanner) == '*' )
						{
//							token.Type = Tok_Access_PointerToMemberOfPointerSymbol;
							c.token.Text.Len++;
							move_forward();
						}
					}
					else if ( (* ctx->scanner) == '=' )
					{
						c.token.Text.Len++;
						// token.Type = Tok_Assign_Subtract;
						c.token.Flags |= TF_Assign;

						if (c.left)
							move_forward();
					}
					else while ( c.left && (* ctx->scanner) == *(c.scanner - 1) && c.token.Text.Len < 3 )
					{
						c.token.Text.Len++;

						if (c.left)
							move_forward();
					}
				}
				goto FoundToken;
			}
			case '/':
			{
				Str text = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Operator;
				// token.Type   = Tok_Divide;
				c.token.Flags  = TF_Operator;
				move_forward();

				if ( c.left )
				{
					if ( (* ctx->scanner) == '=' )
					{
						// token.Type = TokeType::Assign_Divide;
						move_forward();
						c.token.Text.Len++;
						c.token.Flags = TF_Assign;
					}
					else if ( (* ctx->scanner) == '/' )
					{
						c.token.Type     = Tok_Comment;
						c.token.Text.Len = 2;
						c.token.Flags    = TF_Null;
						move_forward();

						while ( c.left && (* ctx->scanner) != '\n' && (* ctx->scanner) != '\r' )
						{
							move_forward();
							c.token.Text.Len++;
						}

						if ( (* ctx->scanner) == '\r' )
						{
							move_forward();
							c.token.Text.Len++;
						}
						if ( (* ctx->scanner) == '\n' )
						{
							move_forward();
							c.token.Text.Len++;
						}
						array_append( _ctx->Lexer_Tokens, c.token );
						continue;
					}
					else if ( (* ctx->scanner) == '*' )
					{
						c.token.Type     = Tok_Comment;
						c.token.Text.Len = 2;
						c.token.Flags    = TF_Null;
						move_forward();

						bool star   = (* ctx->scanner)    == '*';
						bool slash  = c.scanner[1] == '/';
						bool at_end = star && slash;
						while ( c.left && ! at_end  )
						{
							move_forward();
							c.token.Text.Len++;

							star   = (* ctx->scanner)    == '*';
							slash  = c.scanner[1] == '/';
							at_end = star && slash;
						}
						c.token.Text.Len += 2;
						move_forward();
						move_forward();

						if ( (* ctx->scanner) == '\r' )
						{
							move_forward();
							c.token.Text.Len++;
						}
						if ( (* ctx->scanner) == '\n' )
						{
							move_forward();
							c.token.Text.Len++;
						}
						array_append( _ctx->Lexer_Tokens, c.token );
						// end_line();
						continue;
					}
				}
				goto FoundToken;
			}
		}

		if ( char_is_alpha( (* ctx->scanner) ) || (* ctx->scanner) == '_' )
		{
			Str text = { c.scanner, 1 };
			c.token.Text = text;
			move_forward();

			while ( c.left && ( char_is_alphanumeric((* ctx->scanner)) || (* ctx->scanner) == '_' ) )
			{
				move_forward();
				c.token.Text.Len++;
			}

			goto FoundToken;
		}
		else if ( char_is_digit((* ctx->scanner)) )
		{
			// This is a very brute force lex, no checks are done for validity of literal.

			Str text = { c.scanner, 1 };
			c.token.Text   = text;
			c.token.Type   = Tok_Number;
			c.token.Flags  = TF_Literal;
			move_forward();

			if (c.left
			&& (	(* ctx->scanner) == 'x' || (* ctx->scanner) == 'X'
				||	(* ctx->scanner) == 'b' || (* ctx->scanner) == 'B'
				||  (* ctx->scanner) == 'o' || (* ctx->scanner) == 'O' )
			)
			{
				move_forward();
				c.token.Text.Len++;

				while ( c.left && char_is_hex_digit((* ctx->scanner)) )
				{
					move_forward();
					c.token.Text.Len++;
				}

				goto FoundToken;
			}

			while ( c.left && char_is_digit((* ctx->scanner)) )
			{
				move_forward();
				c.token.Text.Len++;
			}

			if ( c.left && (* ctx->scanner) == '.' )
			{
				move_forward();
				c.token.Text.Len++;

				while ( c.left && char_is_digit((* ctx->scanner)) )
				{
					move_forward();
					c.token.Text.Len++;
				}

				// Handle number literal suffixes in a botched way
				if (c.left && (
					(* ctx->scanner) == 'l' || (* ctx->scanner) == 'L' ||  // long/long long
					(* ctx->scanner) == 'u' || (* ctx->scanner) == 'U' ||  // unsigned
					(* ctx->scanner) == 'f' || (* ctx->scanner) == 'F' ||  // float
					(* ctx->scanner) == 'i' || (* ctx->scanner) == 'I' ||  // imaginary
					(* ctx->scanner) == 'z' || (* ctx->scanner) == 'Z'))   // complex
				{
					char prev = (* ctx->scanner);
					move_forward();
					c.token.Text.Len++;

					// Handle 'll'/'LL' as a special case when we just processed an 'l'/'L'
					if (c.left && (prev == 'l' || prev == 'L') && ((* ctx->scanner) == 'l' || (* ctx->scanner) == 'L'))
					{
						move_forward();
						c.token.Text.Len++;
					}
				}
			}

			goto FoundToken;
		}
		else
		{
			s32 start = max( 0, array_num(_ctx->Lexer_Tokens) - 100 );
			log_fmt("\n%d\n", start);
			for ( s32 idx = start; idx < array_num(_ctx->Lexer_Tokens); idx++ )
			{
				log_fmt( "Token %d Type: %s : %.*s\n"
					, idx
					, toktype_to_str( _ctx->Lexer_Tokens[ idx ].Type ).Ptr
					, _ctx->Lexer_Tokens[ idx ].Text.Len, _ctx->Lexer_Tokens[ idx ].Text.Ptr
				);
			}

			StrBuilder context_str = strbuilder_fmt_buf( _ctx->Allocator_Temp, "%.*s", min( 100, c.left ), c.scanner );
			log_failure( "Failed to lex token '%c' (%d, %d)\n%s", (* ctx->scanner), c.line, c.column, context_str );

			// Skip to next whitespace since we can't know if anything else is valid until then.
			while ( c.left && ! char_is_space( (* ctx->scanner) ) )
			{
				move_forward();
			}
		}

		FoundToken:
		{
			lex_found_token( ctx );
			TokType last_type = array_back(_ctx->Lexer_Tokens)->Type;
			if ( last_type == Tok_Preprocess_Macro )
			{
				Token thanks_c = { { c.scanner, 0 }, Tok_Invalid, c.line, c.column, TF_Null };
				c.token = thanks_c;
				if ( (* ctx->scanner) == '\r')
				{
					move_forward();
					c.token.Text.Len = 1;
				}

				if ( (* ctx->scanner) == '\n' )
				{
					c.token.Type = Tok_NewLine;
					c.token.Text.Len++;
					move_forward();

					array_append( _ctx->Lexer_Tokens, c.token );
					continue;
				}
			}
		}
	}

	if ( array_num(_ctx->Lexer_Tokens) == 0 )
	{
		log_failure( "Failed to lex any tokens" );
		{
			TokArray tok_array =  {};
			return tok_array;
		}
	}

	hashtable_clear(_ctx->Lexer_defines);
	// defines_map_arena.free();
	TokArray result = { _ctx->Lexer_Tokens, 0 };
	return result;
}
#undef move_forward
#undef skip_whitespace
#undef end_line

// TODO(Ed) : Rename ETok_Capture_Start, ETok_Capture_End to Open_Parenthesis adn Close_Parenthesis

constexpr bool lex_dont_skip_formatting = false;
constexpr bool      lex_skip_formatting = true;

void parser_push( ParseContext* ctx, StackNode* node )
{
	node->Prev = ctx->Scope;
	ctx->Scope = node;

#if 0 && GEN_BUILD_DEBUG
	log_fmt("\tEntering _ctx->parser: %.*s\n", Scope->ProcName.Len, Scope->ProcName.Ptr );
#endif
}

void parser_pop(ParseContext* ctx)
{
#if 0 && GEN_BUILD_DEBUG
	log_fmt("\tPopping  _ctx->parser: %.*s\n", Scope->ProcName.Len, Scope->ProcName.Ptr );
#endif
	ctx->Scope = ctx->Scope->Prev;
}

StrBuilder parser_to_strbuilder(ParseContext ctx)
{
	StrBuilder result = strbuilder_make_reserve( _ctx->Allocator_Temp, kilobytes(4) );

	Token scope_start = * ctx.Scope->Start;
	Token last_valid  = ctx.Tokens.Idx >= array_num(ctx.Tokens.Arr) ? ctx.Tokens.Arr[array_num(ctx.Tokens.Arr) -1] : (* lex_current(& ctx.Tokens, true));

	sptr        length  = scope_start.Text.Len;
	char const* current = scope_start.Text.Ptr + length;
	while ( current <= array_back( ctx.Tokens.Arr)->Text.Ptr && (* current) != '\n' && length < 74 )
	{
		current++;
		length++;
	}

	Str scope_str = { scope_start.Text.Ptr, length };
	StrBuilder line = strbuilder_make_str( _ctx->Allocator_Temp, scope_str );
	strbuilder_append_fmt( & result, "\tScope    : %s\n", line );
	strbuilder_free(& line);

	sptr   dist            = (sptr)last_valid.Text.Ptr - (sptr)scope_start.Text.Ptr + 2;
	sptr   length_from_err = dist;

	Str err_str        = { last_valid.Text.Ptr, length_from_err };
	StrBuilder line_from_err = strbuilder_make_str( _ctx->Allocator_Temp, err_str );

	if ( length_from_err < 100 )
		strbuilder_append_fmt(& result, "\t(%d, %d):%*c\n", last_valid.Line, last_valid.Column, length_from_err, '^' );
	else
		strbuilder_append_fmt(& result, "\t(%d, %d)\n", last_valid.Line, last_valid.Column );

	StackNode* curr_scope = ctx.Scope;
	s32 level = 0;
	do
	{
		if ( curr_scope->Name.Ptr )
		{
			strbuilder_append_fmt(& result, "\t%d: %s, AST Name: %.*s\n", level, curr_scope->ProcName.Ptr, curr_scope->Name.Len, curr_scope->Name.Ptr );
		}
		else
		{
			strbuilder_append_fmt(& result, "\t%d: %s\n", level, curr_scope->ProcName.Ptr );
		}

		curr_scope = curr_scope->Prev;
		level++;
	}
	while ( curr_scope );
	return result;
}

bool lex__eat(TokArray* self, TokType type )
{
	if ( array_num(self->Arr) - self->Idx <= 0 )
	{
		log_failure( "No tokens left.\n%s", parser_to_strbuilder(_ctx->parser) );
		return false;
	}

	Token at_idx = self->Arr[ self->Idx ];

	if ( ( at_idx.Type == Tok_NewLine && type != Tok_NewLine )
	||   ( at_idx.Type == Tok_Comment && type != Tok_Comment ) )
	{
		self->Idx ++;
	}

	if ( at_idx.Type != type )
	{
		Token tok = * lex_current( self, lex_skip_formatting );
		log_failure( "Parse Error, TokArray::eat, Expected: ' %s ' not ' %.*s ' (%d, %d)`\n%s"
			, toktype_to_str(type).Ptr
			, at_idx.Text.Len, at_idx.Text.Ptr
			, tok.Line
			, tok.Column
			, parser_to_strbuilder(_ctx->parser)
		);
		GEN_DEBUG_TRAP();
		return false;
	}

#if 0 && GEN_BUILD_DEBUG
	log_fmt("Ate: %SB\n", self->Arr[Idx].to_strbuilder() );
#endif

	self->Idx ++;
	return true;
}

internal
void parser_init()
{
	_ctx->Lexer_Tokens = array_init_reserve(Token, arena_allocator_info( & _ctx->LexArena)
		, ( _ctx->InitSize_LexArena - sizeof( ArrayHeader ) ) / sizeof(Token)
	);

	_ctx->Lexer_defines = hashtable_init_reserve(Str, _ctx->Allocator_DyanmicContainers, 256 );
}

internal
void parser_deinit()
{
	Array(Token) null_array = { nullptr };
	_ctx->Lexer_Tokens = null_array;
}

#pragma region Helper Macros

#define check_parse_args( def ) _check_parse_args(def, stringize(_func_) )
bool _check_parse_args( Str def, char const* func_name )
{
	if ( def.Len <= 0 )
	{
		log_failure( c_str_fmt_buf("gen::%s: length must greater than 0", func_name) );
		parser_pop(& _ctx->parser);
		return false;
	}
	if ( def.Ptr == nullptr )
	{
		log_failure( c_str_fmt_buf("gen::%s: def was null", func_name) );
		parser_pop(& _ctx->parser);
		return false;
	}
	return true;
}

#	define currtok_noskip (* lex_current( &  _ctx->parser.Tokens, lex_dont_skip_formatting ))
#	define currtok        (* lex_current( & _ctx->parser.Tokens, lex_skip_formatting ))
#	define peektok        (* lex_peek(_ctx->parser.Tokens, lex_skip_formatting))
#	define prevtok        (* lex_previous( _ctx->parser.Tokens, lex_dont_skip_formatting))
#	define nexttok		  (* lex_next( _ctx->parser.Tokens, lex_skip_formatting ))
#	define nexttok_noskip (* lex_next( _ctx->parser.Tokens, lex_dont_skip_formatting))
#	define eat( Type_ )   lex__eat( & _ctx->parser.Tokens, Type_ )
#	define left           ( array_num(_ctx->parser.Tokens.Arr) - _ctx->parser.Tokens.Idx )

#if GEN_COMPILER_CPP
#	define def_assign( ... ) { __VA_ARGS__ }
#else
#	define def_assign( ... ) __VA_ARGS__
#endif


#ifdef check
#define CHECK_WAS_DEFINED
#pragma push_macro("check")
#undef check
#endif

#	define check_noskip( Type_ ) ( left && currtok_noskip.Type == Type_ )
#	define check( Type_ )        ( left && currtok.Type        == Type_ )

#	define push_scope()                                                                                                         \
	Str null_name = {};                                                                                                         \
	StackNode scope = { nullptr, lex_current( &  _ctx->parser.Tokens, lex_dont_skip_formatting ), null_name, txt( __func__ ) }; \
	parser_push( & _ctx->parser, & scope )

#pragma endregion Helper Macros

// Procedure Forwards ( Entire parser internal parser interface )

internal Code               parse_array_decl                   ();
internal CodeAttributes     parse_attributes                   ();
internal CodeComment        parse_comment                      ();
internal Code               parse_complicated_definition       ( TokType which );
internal CodeBody           parse_class_struct_body            ( TokType which, Token name );
internal Code               parse_class_struct                 ( TokType which, bool inplace_def );
internal CodeDefine         parse_define                       ();
internal Code               parse_expression                   ();
internal Code               parse_forward_or_definition        ( TokType which, bool is_inplace );
internal CodeFn             parse_function_after_name          ( ModuleFlag mflags, CodeAttributes attributes, CodeSpecifiers specifiers, CodeTypename ret_type, Token name );
internal Code               parse_function_body                ();
internal CodeBody           parse_global_nspace                ( CodeType which );
internal Code               parse_global_nspace_constructor_destructor( CodeSpecifiers specifiers );
internal Token              parse_identifier                   ( bool* possible_member_function );
internal CodeInclude        parse_include                      ();
internal CodeOperator       parse_operator_after_ret_type      ( ModuleFlag mflags, CodeAttributes attributes, CodeSpecifiers specifiers, CodeTypename ret_type );
internal Code               parse_operator_function_or_variable( bool expects_function, CodeAttributes attributes, CodeSpecifiers specifiers );
internal CodePragma         parse_pragma                       ();
internal CodeParams         parse_params                       ( bool use_template_capture );
internal CodePreprocessCond parse_preprocess_cond              ();
internal Code               parse_simple_preprocess            ( TokType which, bool dont_consume_braces );
internal Code               parse_static_assert                ();
internal void               parse_template_args                ( Token* token );
internal CodeVar            parse_variable_after_name          ( ModuleFlag mflags, CodeAttributes attributes, CodeSpecifiers specifiers, CodeTypename type, Str name );
internal CodeVar            parse_variable_declaration_list    ();

internal CodeClass       parser_parse_class    ( bool inplace_def );
internal CodeConstructor parser_parse_constructor     ( CodeSpecifiers specifiers );
internal CodeDestructor  parser_parse_destructor      ( CodeSpecifiers specifiers );
internal CodeEnum        parser_parse_enum            ( bool inplace_def );
internal CodeBody        parser_parse_export_body     ();
internal CodeBody        parser_parse_extern_link_body();
internal CodeExtern      parser_parse_extern_link     ();
internal CodeFriend      parser_parse_friend          ();
internal CodeFn          parser_parse_function        ();
internal CodeNS          parser_parse_namespace       ();
internal CodeOpCast      parser_parse_operator_cast   ( CodeSpecifiers specifiers );
internal CodeStruct      parser_parse_struct          ( bool inplace_def );
internal CodeVar         parser_parse_variable        ();
internal CodeTemplate    parser_parse_template        ();
internal CodeTypename    parser_parse_type            ( bool from_template, bool* is_function );
internal CodeTypedef     parser_parse_typedef         ();
internal CodeUnion       parser_parse_union           ( bool inplace_def );
internal CodeUsing       parser_parse_using           ();

constexpr bool parser_inplace_def         = true;
constexpr bool parser_not_inplace_def     = false;
constexpr bool parser_dont_consume_braces = true;
constexpr bool parser_consume_braces      = false;
constexpr bool parser_not_from_template   = false;

constexpr bool parser_use_parenthesis = false;

// Internal parsing functions

constexpr bool parser_strip_formatting_dont_preserve_newlines = false;
/*
	This function was an attempt at stripping formatting from any c++ code.
	It has edge case failures that prevent it from being used in function bodies.
*/
internal
StrBuilder parser_strip_formatting( Str raw_text, bool preserve_newlines )
{
	StrBuilder content = strbuilder_make_reserve( _ctx->Allocator_Temp, raw_text.Len );

	if ( raw_text.Len == 0 )
		return content;

#define cut_length ( scanner - raw_text.Ptr - last_cut )
#define cut_ptr    ( raw_text.Ptr   + last_cut )
#define pos        ( rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr ) )
#define move_fwd() do { scanner++; tokleft--; } while(0)

	s32         tokleft  = raw_text.Len;
	sptr        last_cut = 0;
	char const* scanner  = raw_text.Ptr;

	if ( scanner[0] == ' ' )
	{
		move_fwd();
		last_cut = 1;
	}

	bool within_string     = false;
	bool within_char       = false;
	bool must_keep_newline = false;
	while ( tokleft )
	{
		// Skip over the content of string literals
		if ( scanner[0] == '"' )
		{
			move_fwd();

			while ( tokleft && ( scanner[0] != '"' || *( scanner - 1 ) == '\\' ) )
			{
				if ( scanner[0] == '\\' && tokleft > 1 )
				{
					scanner += 2;
					tokleft -= 2;
				}
				else
				{
					move_fwd();
				}
			}

			// Skip the closing "
			if ( tokleft )
				move_fwd();

			strbuilder_append_c_str_len( & content, cut_ptr, cut_length );
			last_cut = rcast(sptr, scanner ) - rcast( sptr, raw_text.Ptr );
			continue;
		}

		// Skip over the content of character literals
		if ( scanner[0] == '\'' )
		{
			move_fwd();

			while ( tokleft
			&& ( scanner[0] != '\''
				|| ( *(scanner -1 ) == '\\' )
			) )
			{
				move_fwd();
			}

			// Skip the closing '
			if ( tokleft )
				move_fwd();

			strbuilder_append_c_str_len( & content, cut_ptr, cut_length );
			last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );
			continue;
		}

		// Block comments
		if ( tokleft > 1 && scanner[0] == '/' && scanner[1] == '*' )
		{
			while ( tokleft > 1 && !(scanner[0] == '*' && scanner[1] == '/') )
				move_fwd();

			scanner += 2;
			tokleft -= 2;

			strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );
			last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );
			continue;
		}

		// Line comments
		if ( tokleft > 1 && scanner[0] == '/' && scanner[1] == '/' )
		{
			must_keep_newline = true;

			scanner += 2;
			tokleft -= 2;

			while ( tokleft && scanner[ 0 ] != '\n' )
				move_fwd();

			if (tokleft)
				move_fwd();

			strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );
			last_cut = rcast( sptr,  scanner ) - rcast( sptr, raw_text.Ptr );
			continue;
		}

		// Tabs
		if (scanner[0] == '\t')
		{
			if (pos > last_cut)
				strbuilder_append_c_str_len( & content, cut_ptr, cut_length);

			if ( * strbuilder_back( content ) != ' ' )
				strbuilder_append_char( & content, ' ' );

			move_fwd();
			last_cut = rcast( sptr, scanner) - rcast( sptr, raw_text.Ptr);
			continue;
		}

		if ( tokleft > 1 && scanner[0] == '\r' && scanner[1] == '\n' )
		{
			if ( must_keep_newline || preserve_newlines )
			{
				must_keep_newline = false;

				scanner += 2;
				tokleft -= 2;

				strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );
				last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );
				continue;
			}

			if ( pos > last_cut )
				strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );

			// Replace with a space
			if ( * strbuilder_back( content ) != ' ' )
				strbuilder_append_char( & content,  ' ' );

			scanner += 2;
			tokleft -= 2;

			last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );
			continue;
		}

		if ( scanner[0] == '\n' )
		{
			if ( must_keep_newline || preserve_newlines )
			{
				must_keep_newline = false;

				move_fwd();

				strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );
				last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );
				continue;
			}

			if ( pos > last_cut )
				strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );

			// Replace with a space
			if ( * strbuilder_back( content ) != ' ' )
				strbuilder_append_char( & content,  ' ' );

			move_fwd();

			last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );
			continue;
		}

		// Escaped newlines
		if ( scanner[0] == '\\' )
		{
			strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );

			s32 amount_to_skip = 1;
			if ( tokleft > 1 && scanner[1] == '\n' )
			{
				amount_to_skip = 2;
			}
			else if ( tokleft > 2 && scanner[1] == '\r' && scanner[2] == '\n' )
			{
				amount_to_skip = 3;
			}

			if ( amount_to_skip > 1 && pos == last_cut )
			{
				scanner += amount_to_skip;
				tokleft -= amount_to_skip;
			}
			else
				move_fwd();

			last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );
			continue;
		}

		// Consectuive spaces
		if ( tokleft > 1 && char_is_space( scanner[0] ) && char_is_space( scanner[ 1 ] ) )
		{
			strbuilder_append_c_str_len( & content,  cut_ptr, cut_length );
			do
			{
				move_fwd();
			}
			while ( tokleft && char_is_space( scanner[0] ) );

			last_cut = rcast( sptr, scanner ) - rcast( sptr, raw_text.Ptr );

			// Preserve only 1 space of formattting
			char* last = strbuilder_back(content);
			if ( last == nullptr || * last != ' ' )
				strbuilder_append_char( & content, ' ' );

			continue;
		}

		move_fwd();
	}

	if ( last_cut < raw_text.Len )
	{
		strbuilder_append_c_str_len( & content,  cut_ptr, raw_text.Len - last_cut );
	}

#undef cut_ptr
#undef cut_length
#undef pos
#undef move_fwd

	return content;
}

internal
Code parse_array_decl()
{
	push_scope();

	if ( check( Tok_Operator ) && currtok.Text.Ptr[0] == '[' && currtok.Text.Ptr[1] == ']' )
	{
		Code array_expr = untyped_str( txt(" ") );
		eat( Tok_Operator );
		// []

		parser_pop(& _ctx->parser);
		return array_expr;
	}

	if ( check( Tok_BraceSquare_Open ) )
	{
		eat( Tok_BraceSquare_Open );
		// [

		if ( left == 0 )
		{
			log_failure( "Error, unexpected end of array declaration ( '[]' scope started )\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		if ( currtok.Type == Tok_BraceSquare_Close )
		{
			log_failure( "Error, empty array expression in definition\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		Token untyped_tok = currtok;

		while ( left && currtok.Type != Tok_BraceSquare_Close )
		{
			eat( currtok.Type );
		}

		untyped_tok.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)untyped_tok.Text.Ptr;

		Code array_expr = untyped_str( tok_to_str(untyped_tok) );
		// [ <Content>

		if ( left == 0 )
		{
			log_failure( "Error, unexpected end of array declaration, expected ]\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		if ( currtok.Type != Tok_BraceSquare_Close )
		{
			log_failure( "%s: Error, expected ] in array declaration, not %s\n%s", toktype_to_str( currtok.Type ), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		eat( Tok_BraceSquare_Close );
		// [ <Content> ]

		// Its a multi-dimensional array
		if ( check( Tok_BraceSquare_Open ))
		{
			Code adjacent_arr_expr = parse_array_decl();
			// [ <Content> ][ <Content> ]...

			array_expr->Next = adjacent_arr_expr;
		}

		parser_pop(& _ctx->parser);
		return array_expr;
	}

	parser_pop(& _ctx->parser);
	return NullCode;
}

internal inline
CodeAttributes parse_attributes()
{
	push_scope();

	Token start = currtok;
	s32   len   = 0;

	// There can be more than one attribute. If there is flatten them to a single string.
	// TODO(Ed): Support keeping an linked list of attributes similar to parameters
	while ( left && tok_is_attribute(currtok) )
	{
		if ( check( Tok_Attribute_Open ) )
		{
			eat( Tok_Attribute_Open );
			// [[

			while ( left && currtok.Type != Tok_Attribute_Close )
			{
				eat( currtok.Type );
			}
			// [[ <Content>

			eat( Tok_Attribute_Close );
			// [[ <Content> ]]

			len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )start.Text.Ptr;
		}
		else if ( check( Tok_Decl_GNU_Attribute ) )
		{
			eat( Tok_Decl_GNU_Attribute );
			eat( Tok_Capture_Start );
			eat( Tok_Capture_Start );
			// __attribute__((

			while ( left && currtok.Type != Tok_Capture_End )
			{
				eat( currtok.Type );
			}
			// __attribute__(( <Content>

			eat( Tok_Capture_End );
			eat( Tok_Capture_End );
			// __attribute__(( <Content> ))

			len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )start.Text.Ptr;
		}
		else if ( check( Tok_Decl_MSVC_Attribute ) )
		{
			eat( Tok_Decl_MSVC_Attribute );
			eat( Tok_Capture_Start );
			// __declspec(

			while ( left && currtok.Type != Tok_Capture_End )
			{
				eat( currtok.Type );
			}
			// __declspec( <Content>

			eat( Tok_Capture_End );
			// __declspec( <Content> )

			len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )start.Text.Ptr;
		}
		else if ( tok_is_attribute(currtok) )
		{
			eat( currtok.Type );
			// <Attribute>

			// If its a macro based attribute, this could be a functional macro such as Unreal's UE_DEPRECATED(...)
			if ( check( Tok_Capture_Start))
			{
				eat( Tok_Capture_Start );

				s32 level = 0;
				while (left && currtok.Type != Tok_Capture_End && level == 0)
				{
					if (currtok.Type == Tok_Capture_Start)
						++ level;
					if (currtok.Type == Tok_Capture_End)
						--level;
					eat(currtok.Type);
				}
				eat(Tok_Capture_End);
			}

			len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )start.Text.Ptr;
			// <Attribute> ( ... )
		}
	}

	if ( len > 0 )
	{
		Str attribute_txt = { start.Text.Ptr, len };
		parser_pop(& _ctx->parser);

		StrBuilder name_stripped = parser_strip_formatting( attribute_txt, parser_strip_formatting_dont_preserve_newlines );

		Code result     = make_code();
		result->Type    = CT_PlatformAttributes;
		result->Name    = cache_str( strbuilder_to_str(name_stripped) );
		result->Content = result->Name;
		// result->Token   =

		return ( CodeAttributes )result;
	}

	parser_pop(& _ctx->parser);
	return NullCode;
}

internal
Code parse_class_struct( TokType which, bool inplace_def )
{
	if ( which != Tok_Decl_Class && which != Tok_Decl_Struct )
	{
		log_failure( "Error, expected class or struct, not %s\n%s", toktype_to_str( which ), parser_to_strbuilder(_ctx->parser) );
		return InvalidCode;
	}

	Token name = NullToken;

	AccessSpec     access     = AccessSpec_Default;
	CodeTypename   parent     = { nullptr };
	CodeBody       body       = { nullptr };
	CodeAttributes attributes = { nullptr };
	ModuleFlag     mflags     = ModuleFlag_None;

	Code result = InvalidCode;

	if ( check(Tok_Module_Export) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <ModuleFlags>

	eat( which );
	// <ModuleFlags> <class/struct>

	attributes = parse_attributes();
	// <ModuleFlags> <class/struct> <Attributes>

	if ( check( Tok_Identifier ) )
	{
		name = parse_identifier(nullptr);
		_ctx->parser.Scope->Name = name.Text;
	}
	// <ModuleFlags> <class/struct> <Attributes> <Name>

	local_persist
	char interface_arr_mem[ kilobytes(4) ] = {0};
	Array(CodeTypename) interfaces; {
		Arena arena = arena_init_from_memory( interface_arr_mem, kilobytes(4) );
		interfaces  = array_init_reserve(CodeTypename, arena_allocator_info(& arena), 4 );
	}

	// TODO(Ed) : Make an AST_DerivedType, we'll store any arbitary derived type into there as a linear linked list of them.
	if ( check( Tok_Assign_Classifer ) )
	{
		eat( Tok_Assign_Classifer );
		// <ModuleFlags> <class/struct> <Attributes> <Name> :

		if ( tok_is_access_specifier(currtok) )
		{
			access = tok_to_access_specifier(currtok);
			// <ModuleFlags> <class/struct> <Attributes> <Name> : <Access Specifier>
			eat( currtok.Type );
		}

		Token parent_tok = parse_identifier(nullptr);
		parent = def_type( tok_to_str(parent_tok) );
		// <ModuleFlags> <class/struct> <Attributes> <Name> : <Access Specifier> <Parent/Interface Name>

		while ( check(Tok_Comma) )
		{
			eat( Tok_Comma );
			// <ModuleFlags> <class/struct> <Attributes> <Name> : <Access Specifier> <Name>,

			if ( tok_is_access_specifier(currtok) )
			{
				eat(currtok.Type);
			}
			Token interface_tok = parse_identifier(nullptr);

			array_append( interfaces, def_type( tok_to_str(interface_tok) ) );
			// <ModuleFlags> <class/struct> <Attributes> <Name> : <Access Specifier> <Name>, ...
		}
	}

	if ( check( Tok_BraceCurly_Open ) )
	{
		body = parse_class_struct_body( which, name );
	}
	// <ModuleFlags> <class/struct> <Attributes> <Name> : <Access Specifier> <Name>, ... { <Body> }

	CodeComment inline_cmt = NullCode;
	if ( ! inplace_def )
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <ModuleFlags> <class/struct> <Attributes> <Name> : <Access Specifier> <Name>, ... { <Body> };

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
		// <ModuleFlags> <class/struct> <Attributes> <Name> : <Access Specifier> <Name>, ... { <Body> }; <InlineCmt>
	}

	if ( which == Tok_Decl_Class )
		result = cast(Code, def_class( tok_to_str(name), def_assign( body, parent, access, attributes, interfaces, scast(s32, array_num(interfaces)), mflags ) ));

	else
		result = cast(Code, def_struct( tok_to_str(name), def_assign( body, (CodeTypename)parent, access, attributes, interfaces, scast(s32, array_num(interfaces)), mflags ) ));

	if ( inline_cmt )
		result->InlineCmt = cast(Code, inline_cmt);

	array_free(interfaces);
	return result;
}

internal neverinline
CodeBody parse_class_struct_body( TokType which, Token name )
{
	push_scope();

	eat( Tok_BraceCurly_Open );
	// {

	CodeBody
	result = (CodeBody) make_code();

	if ( which == Tok_Decl_Class )
		result->Type = CT_Class_Body;

	else
		result->Type = CT_Struct_Body;

	while ( left && currtok_noskip.Type != Tok_BraceCurly_Close )
	{
		Code           member     = Code_Invalid;
		CodeAttributes attributes = { nullptr };
		CodeSpecifiers specifiers = { nullptr };

		bool expects_function = false;

		// _ctx->parser.Scope->Start = currtok_noskip;

		if ( currtok_noskip.Type == Tok_Preprocess_Hash )
			eat( Tok_Preprocess_Hash );

		b32 macro_found = true;

		switch ( currtok_noskip.Type )
		{
			case Tok_Statement_End:
			{
				// TODO(Ed): Convert this to a general warning procedure
				log_fmt("Dangling end statement found %SB\n", tok_to_strbuilder(currtok_noskip));
				eat( Tok_Statement_End );
				continue;
			}
			case Tok_NewLine:
				member = fmt_newline;
				eat( Tok_NewLine );
			break;

			case Tok_Comment:
				member = cast(Code, parse_comment());
			break;

			case Tok_Access_Public:
				member = access_public;
				eat( Tok_Access_Public );
				eat( Tok_Assign_Classifer );
				// public:
			break;

			case Tok_Access_Protected:
				member = access_protected;
				eat( Tok_Access_Protected );
				eat( Tok_Assign_Classifer );
				// protected:
			break;

			case Tok_Access_Private:
				member = access_private;
				eat( Tok_Access_Private );
				eat( Tok_Assign_Classifer );
				// private:
			break;

			case Tok_Decl_Class:
				member = parse_complicated_definition( Tok_Decl_Class );
				// class
			break;

			case Tok_Decl_Enum:
				member = parse_complicated_definition( Tok_Decl_Enum );
				// enum
			break;

			case Tok_Decl_Friend:
				member = cast(Code, parser_parse_friend());
				// friend
			break;

			case Tok_Decl_Operator:
				member = cast(Code, parser_parse_operator_cast(NullCode));
				// operator <Type>()
			break;

			case Tok_Decl_Struct:
				member = parse_complicated_definition( Tok_Decl_Struct );
				// struct
			break;

			case Tok_Decl_Template:
				member = cast(Code, parser_parse_template());
				// template< ... >
			break;

			case Tok_Decl_Typedef:
				member = cast(Code, parser_parse_typedef());
				// typedef
			break;

			case Tok_Decl_Union:
				member = parse_complicated_definition( Tok_Decl_Union );
				// union
			break;

			case Tok_Decl_Using:
				member = cast(Code, parser_parse_using());
				// using
			break;

			case Tok_Operator:
				//if ( currtok.Text[0] != '~' )
				//{
				//	log_failure( "Operator token found in global body but not destructor unary negation\n%s", to_strbuilder(_ctx->parser) );
				//	return InvalidCode;
				//}

				member = cast(Code, parser_parse_destructor(NullCode));
				// ~<Name>()
			break;

			case Tok_Preprocess_Define:
				member = cast(Code, parse_define());
				// #define
			break;

			case Tok_Preprocess_Include:
				member = cast(Code, parse_include());
				// #include
			break;

			case Tok_Preprocess_If:
			case Tok_Preprocess_IfDef:
			case Tok_Preprocess_IfNotDef:
			case Tok_Preprocess_ElIf:
				member = cast(Code, parse_preprocess_cond());
				// #<Condition>
			break;

			case Tok_Preprocess_Else:
				member = cast(Code, preprocess_else);
				eat( Tok_Preprocess_Else );
				// #else
			break;

			case Tok_Preprocess_EndIf:
				member = cast(Code, preprocess_endif);
				eat( Tok_Preprocess_EndIf );
				// #endif
			break;

			case Tok_Preprocess_Macro:
				// <Macro>
				macro_found = true;
				goto Preprocess_Macro_Bare_In_Body;
			break;

			case Tok_Preprocess_Pragma:
				member = cast(Code, parse_pragma());
				// #pragma
			break;

			case Tok_Preprocess_Unsupported:
				member = cast(Code, parse_simple_preprocess( Tok_Preprocess_Unsupported, parser_consume_braces ));
				// #<UNKNOWN>
			break;

			case Tok_StaticAssert:
				member = parse_static_assert();
				// static_assert
			break;

			case Tok_Attribute_Open:
			case Tok_Decl_GNU_Attribute:
			case Tok_Decl_MSVC_Attribute:
		#define Entry( attribute, str ) case attribute:
			GEN_DEFINE_ATTRIBUTE_TOKENS
		#undef Entry
			{
				attributes = parse_attributes();
				// <Attributes>
			}
			//! Fallthrough intended
			case Tok_Spec_Consteval:
			case Tok_Spec_Constexpr:
			case Tok_Spec_Constinit:
			case Tok_Spec_Explicit:
			case Tok_Spec_ForceInline:
			case Tok_Spec_Inline:
			case Tok_Spec_Mutable:
			case Tok_Spec_NeverInline:
			case Tok_Spec_Static:
			case Tok_Spec_Volatile:
			case Tok_Spec_Virtual:
			{
				Specifier specs_found[16] = { Spec_NumSpecifiers };
				s32        NumSpecifiers = 0;

				while ( left && tok_is_specifier(currtok) )
				{
					Specifier spec = str_to_specifier( tok_to_str(currtok) );

					b32 ignore_spec = false;

					switch ( spec )
					{
						case Spec_Constexpr:
						case Spec_Constinit:
						case Spec_Explicit:
						case Spec_Inline:
						case Spec_ForceInline:
						case Spec_Mutable:
						case Spec_NeverInline:
						case Spec_Static:
						case Spec_Volatile:
						case Spec_Virtual:
						break;

						case Spec_Consteval:
							expects_function = true;
						break;

						case Spec_Const :
							ignore_spec = true;
						break;

						default:
							log_failure( "Invalid specifier %S for variable\n%S", spec_to_str(spec), strbuilder_to_str( parser_to_strbuilder(_ctx->parser)) );
							parser_pop(& _ctx->parser);
							return InvalidCode;
					}

					// Every specifier after would be considered part of the type type signature
					if (ignore_spec)
						break;

					specs_found[NumSpecifiers] = spec;
					NumSpecifiers++;
					eat( currtok.Type );
				}

				if ( NumSpecifiers )
				{
					specifiers = def_specifiers( NumSpecifiers, specs_found );
				}
				// <Attributes> <Specifiers>

				if ( tok_is_attribute(currtok) )
				{
					// Unfortuantely Unreal has code where there is attirbutes before specifiers
					CodeAttributes more_attributes = parse_attributes();

					if ( attributes )
					{
						StrBuilder fused = strbuilder_make_reserve( _ctx->Allocator_Temp, attributes->Content.Len + more_attributes->Content.Len );
						strbuilder_append_fmt( & fused, "%SB %SB", attributes->Content, more_attributes->Content );

						Str attrib_name     = strbuilder_to_str(fused);
						attributes->Name    = cache_str( attrib_name );
						attributes->Content = attributes->Name;
						// <Attributes> <Specifiers> <Attributes>
					}

					attributes = more_attributes;
				}

				if ( currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '~' )
				{
					member = cast(Code, parser_parse_destructor( specifiers ));
					// <Attribute> <Specifiers> ~<Name>()
					break;
				}

				if ( currtok.Type == Tok_Decl_Operator )
				{
					member = cast(Code, parser_parse_operator_cast( specifiers ));
					// <Attributes> <Specifiers> operator <Type>()
					break;
				}
			}
			//! Fallthrough intentional
			case Tok_Identifier:
			case Tok_Spec_Const:
			case Tok_Type_Unsigned:
			case Tok_Type_Signed:
			case Tok_Type_Short:
			case Tok_Type_Long:
			case Tok_Type_bool:
			case Tok_Type_char:
			case Tok_Type_int:
			case Tok_Type_double:
			{
				if ( nexttok.Type == Tok_Capture_Start && name.Text.Len && currtok.Type == Tok_Identifier )
				{
					if ( c_str_compare_len( name.Text.Ptr, currtok.Text.Ptr, name.Text.Len ) == 0 )
					{
						member = cast(Code, parser_parse_constructor( specifiers ));
						// <Attributes> <Specifiers> <Name>()
						break;
					}
				}

				if (macro_found)
				{
				Preprocess_Macro_Bare_In_Body:
					b32 lone_macro = nexttok.Type == Tok_Statement_End || nexttok_noskip.Type == Tok_NewLine;
					if (lone_macro)
					{
						member = cast(Code, parse_simple_preprocess( Tok_Preprocess_Macro, parser_consume_braces ));
						// <Macro>;

						if ( member == Code_Invalid )
						{
							log_failure( "Failed to parse member\n%s", parser_to_strbuilder(_ctx->parser) );
							parser_pop(& _ctx->parser);
							return InvalidCode;
						}
						continue;
					}

					// We have a macro but its most likely behaving as a typename
					// <Macro ...
				}

				member = parse_operator_function_or_variable( expects_function, attributes, specifiers );
				// <Attributes> <Specifiers> operator <Op> ...
				// or
				// <Attributes> <Specifiers> <Name> ...
			}
			break;

			default:
				Token untyped_tok = currtok;

				while ( left && currtok.Type != Tok_BraceCurly_Close )
				{
					untyped_tok.Text.Len = ( (sptr)currtok.Text.Ptr + currtok.Text.Len ) - (sptr)untyped_tok.Text.Ptr;
					eat( currtok.Type );
				}

				member = untyped_str( tok_to_str(untyped_tok) );
				// Something unknown
			break;
		}

		if ( member == Code_Invalid )
		{
			log_failure( "Failed to parse member\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		body_append(result, member );
	}

	eat( Tok_BraceCurly_Close );
	// { <Members> }
	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeComment parse_comment()
{
	push_scope();

	CodeComment
	result          = (CodeComment) make_code();
	result->Type    = CT_Comment;
	result->Content = cache_str( tok_to_str(currtok_noskip) );
	// result->Token   = currtok_noskip;
	eat( Tok_Comment );

	parser_pop(& _ctx->parser);
	return result;
}

internal
Code parse_complicated_definition( TokType which )
{
	push_scope();

	bool is_inplace = false;

	TokArray tokens = _ctx->parser.Tokens;

	s32 idx         = tokens.Idx;
	s32 level       = 0;
	for ( ; idx < array_num(tokens.Arr); idx++ )
	{
		if ( tokens.Arr[ idx ].Type == Tok_BraceCurly_Open )
			level++;

		if ( tokens.Arr[ idx ].Type == Tok_BraceCurly_Close )
			level--;

		if ( level == 0 && tokens.Arr[ idx ].Type == Tok_Statement_End )
			break;
	}

	if ( ( idx - 2 ) == tokens.Idx )
	{
		// Its a forward declaration only
		Code result = parse_forward_or_definition( which, is_inplace );
		// <class, enum, struct, or union> <Name>;
		parser_pop(& _ctx->parser);
		return result;
	}

	Token tok = tokens.Arr[ idx - 1 ];
	if ( tok_is_specifier(tok) && spec_is_trailing( str_to_specifier( tok_to_str(tok))) )
	{
		// <which> <type_identifier>(...) <specifier> ...;

		s32   spec_idx = idx - 1;
		Token spec     = tokens.Arr[spec_idx];
		while ( tok_is_specifier(spec) && spec_is_trailing( str_to_specifier( tok_to_str(spec))) )
		{
			-- spec_idx;
			spec = tokens.Arr[spec_idx];
		}

		if ( tokens.Arr[spec_idx].Type == Tok_Capture_End )
		{
			// Forward declaration with trailing specifiers for a procedure
			tok = tokens.Arr[spec_idx];

			Code result = parse_operator_function_or_variable( false, NullCode, NullCode );
			// <Attributes> <Specifiers> <ReturnType/ValueType> <operator <Op>, or Name> ...
			parser_pop(& _ctx->parser);
			return result;
		}

		log_failure( "Unsupported or bad member definition after %s declaration\n%s", toktype_to_str(which), parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}
	if ( tok.Type == Tok_Identifier )
	{
		tok = tokens.Arr[ idx - 2 ];
		bool is_indirection = tok.Type == Tok_Ampersand || tok.Type == Tok_Star;
		bool ok_to_parse    = false;

		if ( tok.Type == Tok_BraceCurly_Close )
		{
			// Its an inplace definition
			// <which> <type_identifier> { ... } <identifier>;
			ok_to_parse = true;
			is_inplace  = true;
		}
		else if ( tok.Type == Tok_Identifier && tokens.Arr[ idx - 3 ].Type == which )
		{
			// Its a variable with type ID using <which> namespace.
			// <which> <type_identifier> <identifier>;
			ok_to_parse = true;
		}
		else if ( tok.Type == Tok_Assign_Classifer
		&& (	( tokens.Arr[idx - 5].Type == which && tokens.Arr[idx - 4].Type == Tok_Decl_Class )
			||	( tokens.Arr[idx - 4].Type == which))
		)
		{
			// Its a forward declaration of an enum
			// <enum>         <type_identifier> : <identifier>;
			// <enum> <class> <type_identifier> : <identifier>;
			ok_to_parse = true;
			Code result = cast(Code, parser_parse_enum( ! parser_inplace_def));
			parser_pop(& _ctx->parser);
			return result;
		}
		else if ( is_indirection )
		{
			// Its a indirection type with type ID using struct namespace.
			// <which> <type_identifier>* <identifier>;
			ok_to_parse = true;
		}

		if ( ! ok_to_parse )
		{
			log_failure( "Unsupported or bad member definition after %s declaration\n%s", toktype_to_str(which), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		Code result = parse_operator_function_or_variable( false, NullCode, NullCode );
		// <Attributes> <Specifiers> <ReturnType/ValueType> <operator <Op>, or Name> ...
		parser_pop(& _ctx->parser);
		return result;
	}
	else if ( tok.Type >= Tok_Type_Unsigned && tok.Type <= Tok_Type_MS_W64 )
	{
		tok = tokens.Arr[ idx - 2 ];

		if ( tok.Type != Tok_Assign_Classifer
		|| (	( tokens.Arr[idx - 5].Type != which && tokens.Arr[idx - 4].Type != Tok_Decl_Class )
			&&	( tokens.Arr[idx - 4].Type != which))
		)
		{
			log_failure( "Unsupported or bad member definition after %s declaration\n%s", toktype_to_str(which), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		// Its a forward declaration of an enum class
		// <enum>         <type_identifier> : <identifier>;
		// <enum> <class> <type_identifier> : <identifier>;
		Code result = cast(Code, parser_parse_enum( ! parser_inplace_def));
		parser_pop(& _ctx->parser);
		return result;
	}
	else if ( tok.Type == Tok_BraceCurly_Close )
	{
		// Its a definition
		Code result = parse_forward_or_definition( which, is_inplace );
		// <which> { ... };
		parser_pop(& _ctx->parser);
		return result;
	}
	else if ( tok.Type == Tok_BraceSquare_Close )
	{
		// Its an array definition
		Code result = parse_operator_function_or_variable( false, NullCode, NullCode );
		// <which> <type_identifier> <identifier> [ ... ];
		parser_pop(& _ctx->parser);
		return result;
	}
	else
	{
		log_failure( "Unsupported or bad member definition after %s declaration\n%SB", toktype_to_str(which).Ptr, parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}
}

internal inline
CodeDefine parse_define()
{
	push_scope();
	eat( Tok_Preprocess_Define );
	// #define

	CodeDefine
	define = (CodeDefine) make_code();
	define->Type = CT_Preprocess_Define;

	if ( ! check( Tok_Identifier ) )
	{
		log_failure( "Error, expected identifier after #define\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	_ctx->parser.Scope->Name = currtok.Text;
	define->Name = cache_str( tok_to_str(currtok) );
	eat( Tok_Identifier );
	// #define <Name>

	if ( ! check( Tok_Preprocess_Content ))
	{
		log_failure( "Error, expected content after #define %s\n%s", define->Name, parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	if ( currtok.Text.Len == 0 )
	{
		define->Content = cache_str( tok_to_str(currtok) );
		eat( Tok_Preprocess_Content );
		// #define <Name> <Content>

		parser_pop(& _ctx->parser);
		return define;
	}

	define->Content = cache_str( strbuilder_to_str( parser_strip_formatting( tok_to_str(currtok), parser_strip_formatting_dont_preserve_newlines )) );
	eat( Tok_Preprocess_Content );
	// #define <Name> <Content>

	parser_pop(& _ctx->parser);
	return define;
}

internal inline
Code parse_assignment_expression()
{
	Code expr = { nullptr };

	eat( Tok_Operator );
	// <Attributes> <Specifiers> <ValueType> <Name> =

	Token expr_tok = currtok;

	if ( currtok.Type == Tok_Statement_End && currtok.Type != Tok_Comma )
	{
		log_failure( "Expected expression after assignment operator\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	s32 level = 0;
	while ( left && currtok.Type != Tok_Statement_End && (currtok.Type != Tok_Comma || level > 0) )
	{
		if (currtok.Type == Tok_BraceCurly_Open )
			level++;
		if (currtok.Type == Tok_BraceCurly_Close )
			level--;
		if (currtok.Type == Tok_Capture_Start)
			level++;
		else if (currtok.Type == Tok_Capture_End)
			level--;

		eat( currtok.Type );
	}

	expr_tok.Text.Len = ( ( sptr )currtok.Text.Ptr + currtok.Text.Len ) - ( sptr )expr_tok.Text.Ptr - 1;
	expr              = untyped_str( tok_to_str(expr_tok) );
	// = <Expression>
	return expr;
}

internal inline
Code parse_forward_or_definition( TokType which, bool is_inplace )
{
	Code result = InvalidCode;

	switch ( which )
	{
		case Tok_Decl_Class:
			result = cast(Code, parser_parse_class( is_inplace ));
			return result;

		case Tok_Decl_Enum:
			result = cast(Code, parser_parse_enum( is_inplace ));
			return result;

		case Tok_Decl_Struct:
			result = cast(Code, parser_parse_struct( is_inplace ));
			return result;

		case Tok_Decl_Union:
			result = cast(Code, parser_parse_union( is_inplace ));
			return result;

		default:
			log_failure( "Error, wrong token type given to parse_complicated_definition "
				"(only supports class, enum, struct, union) \n%s"
				, parser_to_strbuilder(_ctx->parser) );

			return InvalidCode;
	}
}

// Function parsing is handled in multiple places because its initial signature is shared with variable parsing
internal inline
CodeFn parse_function_after_name(
	  ModuleFlag     mflags
	, CodeAttributes attributes
	, CodeSpecifiers specifiers
	, CodeTypename   ret_type
	, Token          name
)
{
	push_scope();
	CodeParams params = parse_params(parser_use_parenthesis);
	// <Attributes> <Specifiers> <ReturnType> <Name> ( <Parameters> )

	// TODO(Ed), Review old comment : These have to be kept separate from the return type's specifiers.
	while ( left && tok_is_specifier(currtok) )
	{
		if ( specifiers == nullptr )
		{
			specifiers = def_specifier( str_to_specifier( tok_to_str(currtok)) );
			eat( currtok.Type );
			continue;
		}

		specifiers_append(specifiers, str_to_specifier( tok_to_str(currtok)) );
		eat( currtok.Type );
	}
	// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers>

	CodeBody    body       = NullCode;
	CodeComment inline_cmt = NullCode;
	if ( check( Tok_BraceCurly_Open ) )
	{
		body = cast(CodeBody, parse_function_body());
		if ( cast(Code, body) == Code_Invalid )
		{
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> { <Body> }
	}
	else if ( check(Tok_Operator) && currtok.Text.Ptr[0] == '=' )
	{
		eat(Tok_Operator);
		specifiers_append(specifiers, Spec_Pure );

		eat( Tok_Number);
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> = 0;

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers>; <InlineCmt>
	}
	else
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers>;

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
			// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers>; <InlineCmt>
	}

	StrBuilder
	name_stripped = strbuilder_make_str( _ctx->Allocator_Temp, tok_to_str(name) );
	strbuilder_strip_space(name_stripped);

	CodeFn
	result              = (CodeFn) make_code();
	result->Name        = cache_str( strbuilder_to_str(name_stripped) );
	result->ModuleFlags = mflags;

	if ( body )
	{
		switch ( body->Type )
		{
			case CT_Function_Body:
			case CT_Untyped:
				break;

			default:
			{
				log_failure("Body must be either of Function_Body or Untyped type, %s\n%s", code_debug_str(body), parser_to_strbuilder(_ctx->parser));
				parser_pop(& _ctx->parser);
				return InvalidCode;
			}
		}

		result->Type = CT_Function;
		result->Body = body;
	}
	else
	{
		result->Type = CT_Function_Fwd;
	}

	if ( attributes )
		result->Attributes = attributes;

	if ( specifiers )
		result->Specs = specifiers;

	result->ReturnType = ret_type;

	if ( params )
		result->Params = params;

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;

	parser_pop(& _ctx->parser);
	return result;
}

internal
Code parse_function_body()
{
	push_scope();

	eat( Tok_BraceCurly_Open );

	CodeBody
	result = (CodeBody) make_code();
	result->Type = CT_Function_Body;

	// TODO : Support actual parsing of function body
	Token start = currtok_noskip;

	s32 level = 0;
	while ( left && ( currtok_noskip.Type != Tok_BraceCurly_Close || level > 0 ) )
	{
		if ( currtok_noskip.Type == Tok_BraceCurly_Open )
			level++;

		else if ( currtok_noskip.Type == Tok_BraceCurly_Close && level > 0 )
			level--;

		eat( currtok_noskip.Type );
	}

	Token past = prevtok;

	s32 len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)start.Text.Ptr;

	if ( len > 0 )
	{
		Str str = { start.Text.Ptr, len };
		body_append( result, cast(Code, def_execution( str )) );
	}

	eat( Tok_BraceCurly_Close );

	parser_pop(& _ctx->parser);
	return cast(Code, result);
}

internal neverinline
CodeBody parse_global_nspace( CodeType which )
{
	push_scope();

	if ( which != CT_Namespace_Body && which != CT_Global_Body && which != CT_Export_Body && which != CT_Extern_Linkage_Body )
		return InvalidCode;

	if ( which != CT_Global_Body )
		eat( Tok_BraceCurly_Open );
		// {

	CodeBody
	result = (CodeBody) make_code();
	result->Type = which;

	while ( left && currtok_noskip.Type != Tok_BraceCurly_Close )
	{
		Code           member     = Code_Invalid;
		CodeAttributes attributes = { nullptr };
		CodeSpecifiers specifiers = { nullptr };

		bool expects_function = false;

		// _ctx->parser.Scope->Start = currtok_noskip;

		if ( currtok_noskip.Type == Tok_Preprocess_Hash )
			eat( Tok_Preprocess_Hash );

		b32 macro_found = false;

		switch ( currtok_noskip.Type )
		{
			case Tok_Comma:
			{
				log_failure("Dangling comma found: %SB\nContext:\n%SB", tok_to_strbuilder(currtok), parser_to_strbuilder(_ctx->parser));
				parser_pop( & _ctx->parser);
				return InvalidCode;
			}
			break;
			case Tok_Statement_End:
			{
				// TODO(Ed): Convert this to a general warning procedure
				log_fmt("Dangling end statement found %SB\n", tok_to_strbuilder(currtok_noskip));
				eat( Tok_Statement_End );
				continue;
			}
			case Tok_NewLine:
				// Empty lines are auto skipped by Tokens.current()
				member = fmt_newline;
				eat( Tok_NewLine );
			break;

			case Tok_Comment:
				member = cast(Code, parse_comment());
			break;

			case Tok_Decl_Class:
				member = parse_complicated_definition( Tok_Decl_Class );
				// class
			break;

			case Tok_Decl_Enum:
				member = parse_complicated_definition( Tok_Decl_Enum );
				// enum
			break;

			case Tok_Decl_Extern_Linkage:
				if ( which == CT_Extern_Linkage_Body )
					log_failure( "Nested extern linkage\n%s", parser_to_strbuilder(_ctx->parser) );

				member = cast(Code, parser_parse_extern_link());
				// extern "..." { ... }
			break;

			case Tok_Decl_Namespace:
				member = cast(Code, parser_parse_namespace());
				// namespace <Name> { ... }
			break;

			case Tok_Decl_Struct:
				member = parse_complicated_definition( Tok_Decl_Struct );
				// struct ...
			break;

			case Tok_Decl_Template:
				member = cast(Code, parser_parse_template());
				// template<...> ...
			break;

			case Tok_Decl_Typedef:
				member = cast(Code, parser_parse_typedef());
				// typedef ...
			break;

			case Tok_Decl_Union:
				member = parse_complicated_definition( Tok_Decl_Union );
				// union ...
			break;

			case Tok_Decl_Using:
				member = cast(Code, parser_parse_using());
				// using ...
			break;

			case Tok_Preprocess_Define:
				member = cast(Code, parse_define());
				// #define ...
			break;

			case Tok_Preprocess_Include:
				member = cast(Code, parse_include());
				// #include ...
			break;

			case Tok_Preprocess_If:
			case Tok_Preprocess_IfDef:
			case Tok_Preprocess_IfNotDef:
			case Tok_Preprocess_ElIf:
				member = cast(Code, parse_preprocess_cond());
				// #<Conditional> ...
			break;

			case Tok_Preprocess_Else:
				member = cast(Code, preprocess_else);
				eat( Tok_Preprocess_Else );
				// #else
			break;

			case Tok_Preprocess_EndIf:
				member = cast(Code, preprocess_endif);
				eat( Tok_Preprocess_EndIf );
				// #endif
			break;

			case Tok_Preprocess_Macro: {
				// <Macro>
				macro_found = true;
				goto Preprocess_Macro_Bare_In_Body;
			}
			break;

			case Tok_Preprocess_Pragma: {
				member = cast(Code, parse_pragma());
				// #pragma ...
			}
			break;

			case Tok_Preprocess_Unsupported: {
				member = cast(Code, parse_simple_preprocess( Tok_Preprocess_Unsupported, parser_consume_braces ));
				// #<UNSUPPORTED> ...
			}
			break;

			case Tok_StaticAssert: {
				member = cast(Code, parse_static_assert());
				// static_assert( <Conditional Expression>, ... );
			}
			break;

			case Tok_Module_Export: {
				if ( which == CT_Export_Body )
					log_failure( "Nested export declaration\n%s", parser_to_strbuilder(_ctx->parser) );

				member = cast(Code, parser_parse_export_body());
				// export { ... }
			}
			break;

			case Tok_Module_Import: {
				// import ...
				log_failure( "gen::%s: This function is not implemented" );
				return InvalidCode;
			}
			//! Fallthrough intentional
			case Tok_Attribute_Open:
			case Tok_Decl_GNU_Attribute:
			case Tok_Decl_MSVC_Attribute:
		#define Entry( attribute, str ) case attribute:
			GEN_DEFINE_ATTRIBUTE_TOKENS
		#undef Entry
			{
				attributes = parse_attributes();
				// <Attributes>
			}
			//! Fallthrough intentional
			case Tok_Spec_Consteval:
			case Tok_Spec_Constexpr:
			case Tok_Spec_Constinit:
			case Tok_Spec_Extern:
			case Tok_Spec_ForceInline:
			case Tok_Spec_Global:
			case Tok_Spec_Inline:
			case Tok_Spec_Internal_Linkage:
			case Tok_Spec_NeverInline:
			case Tok_Spec_Static:
			{
				Specifier specs_found[16] = { Spec_NumSpecifiers };
				s32        NumSpecifiers = 0;

				while ( left && tok_is_specifier(currtok) )
				{
					Specifier spec = str_to_specifier( tok_to_str(currtok) );

					bool ignore_spec = false;

					switch ( spec )
					{
						case Spec_Constexpr:
						case Spec_Constinit:
						case Spec_ForceInline:
						case Spec_Global:
						case Spec_External_Linkage:
						case Spec_Internal_Linkage:
						case Spec_Inline:
						case Spec_Mutable:
						case Spec_NeverInline:
						case Spec_Static:
						case Spec_Volatile:
						break;

						case Spec_Consteval:
							expects_function = true;
						break;

						case Spec_Const:
							ignore_spec = true;
						break;

						default:
							Str spec_str = spec_to_str(spec);

							log_failure( "Invalid specifier %S for variable\n%S", spec_str, strbuilder_to_str( parser_to_strbuilder(_ctx->parser)) );
							parser_pop(& _ctx->parser);
							return InvalidCode;
					}

					if (ignore_spec)
						break;

					specs_found[NumSpecifiers] = spec;
					NumSpecifiers++;
					eat( currtok.Type );
				}

				if ( NumSpecifiers )
				{
					specifiers = def_specifiers( NumSpecifiers, specs_found );
				}
				// <Attributes> <Specifiers>
			}
			//! Fallthrough intentional
			case Tok_Identifier:
			case Tok_Spec_Const:
			case Tok_Type_Long:
			case Tok_Type_Short:
			case Tok_Type_Signed:
			case Tok_Type_Unsigned:
			case Tok_Type_bool:
			case Tok_Type_char:
			case Tok_Type_double:
			case Tok_Type_int:
			{
				// This s only in a scope so that Preprocess_Macro_Bare_In_Body works without microsoft extension warnings
				{
					Code constructor_destructor = parse_global_nspace_constructor_destructor( specifiers );
					// Possible constructor implemented at global file scope.
					if ( constructor_destructor )
					{
						member = constructor_destructor;
						break;
					}

					bool found_operator_cast_outside_class_implmentation = false;
					s32  idx = _ctx->parser.Tokens.Idx;

					for ( ; idx < array_num(_ctx->parser.Tokens.Arr); idx++ )
					{
						Token tok = _ctx->parser.Tokens.Arr[ idx ];

						if ( tok.Type == Tok_Identifier )
						{
							idx++;
							tok = _ctx->parser.Tokens.Arr[ idx ];
							if ( tok.Type == Tok_Access_StaticSymbol )
								continue;

							break;
						}

						if ( tok.Type == Tok_Decl_Operator )
							found_operator_cast_outside_class_implmentation = true;

						break;
					}

					if ( found_operator_cast_outside_class_implmentation )
					{
						member = cast(Code, parser_parse_operator_cast( specifiers ));
						// <Attributes> <Specifiers> <Name>::operator <Type>() { ... }
						break;
					}
				}

				if (macro_found)
				{
				Preprocess_Macro_Bare_In_Body:
					b32 lone_macro = nexttok.Type == Tok_Statement_End || nexttok_noskip.Type == Tok_NewLine;
					if (lone_macro)
					{
						member = parse_simple_preprocess( Tok_Preprocess_Macro, parser_consume_braces );
						// <Macro>;

						if ( member == Code_Invalid )
						{
							log_failure( "Failed to parse member\n%s", parser_to_strbuilder(_ctx->parser) );
							parser_pop(& _ctx->parser);
							return InvalidCode;
						}
						goto Member_Resolved_To_Lone_Macro;
					}

					// We have a macro but its most likely behaving as a typename
					// <Macro ...
				}

				member = parse_operator_function_or_variable( expects_function, attributes, specifiers );
				// <Attributes> <Specifiers> ...
			}
		}

	Member_Resolved_To_Lone_Macro:
		if ( member == Code_Invalid )
		{
			log_failure( "Failed to parse member\nToken: %SB\nContext:\n%SB", tok_to_strbuilder(currtok_noskip), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		// log_fmt("Global Body Member: %s", member->debug_str());
		body_append(result, member );
	}

	if ( which != CT_Global_Body )
		eat( Tok_BraceCurly_Close );
		// { <Body> }

	parser_pop(& _ctx->parser);
	return result;
}

internal inline
Code parse_global_nspace_constructor_destructor( CodeSpecifiers specifiers )
{
	Code result = { nullptr };

	/*
		To check if a definition is for a constructor we can go straight to the opening parenthesis for its parameters
		From There we work backwards to see if we come across two identifiers with the same name between an member access
		:: operator, there can be template parameters on the left of the :: so we ignore those.
		Whats important is that	its back to back.

		This has multiple possible faults. What we parse using this method may not filter out if something has a "return type"
		This is bad since technically you could have a namespace nested into another namespace with the same name.
		If this awful pattern is done the only way to distiguish with this coarse parse is to know there is no return type defined.

		TODO(Ed): We could fix this by attempting to parse a type, but we would have to have a way to have it soft fail and rollback.
	*/
	TokArray tokens = _ctx->parser.Tokens;

	s32   idx = tokens.Idx;
	Token nav = tokens.Arr[ idx ];
	for ( ; idx < array_num(tokens.Arr); idx++, nav = tokens.Arr[ idx ] )
	{
		if ( nav.Text.Ptr[0] == '<' )
		{
			// Skip templated expressions as they mey have expressions with the () operators
			s32 capture_level  = 0;
			s32 template_level = 0;
			for ( ; idx < array_num(tokens.Arr); idx++, nav = tokens.Arr[idx] )
			{
				if (nav.Text.Ptr[ 0 ] == '<')
					++ template_level;

				if (nav.Text.Ptr[ 0 ] == '>')
					-- template_level;
				if (nav.Type == Tok_Operator && nav.Text.Ptr[1] == '>')
					-- template_level;

				if ( nav.Type == Tok_Capture_Start)
				{
					if (template_level != 0 )
						++ capture_level;
					else
						break;
				}

				if ( template_level != 0 && nav.Type == Tok_Capture_End)
					-- capture_level;
			}
		}

		if ( nav.Type == Tok_Capture_Start )
			break;
	}

	-- idx;
	Token tok_right = tokens.Arr[idx];
	Token tok_left  = NullToken;

	if (tok_right.Type != Tok_Identifier)
	{
		// We're not dealing with a constructor if there is no identifier right before the opening of a parameter's scope.
		return result;
	}

	-- idx;
	tok_left = tokens.Arr[idx];
	// <Attributes> <Specifiers> ... <Identifier>

	bool possible_destructor = false;
	if ( tok_left.Type == Tok_Operator && tok_left.Text.Ptr[0] == '~')
	{
		possible_destructor = true;
		-- idx;
		tok_left = tokens.Arr[idx];
	}

	if ( tok_left.Type != Tok_Access_StaticSymbol )
		return result;

	-- idx;
	tok_left = tokens.Arr[idx];
	// <Attributes> <Specifiers> ... :: <Identifier>

	// We search toward the left until we find the next valid identifier
	s32 capture_level  = 0;
	s32 template_level = 0;
	while ( idx != tokens.Idx )
	{
		if (tok_left.Text.Ptr[ 0 ] == '<')
			++ template_level;

		if (tok_left.Text.Ptr[ 0 ] == '>')
			-- template_level;
		if (tok_left.Type == Tok_Operator && tok_left.Text.Ptr[1] == '>')
			-- template_level;

		if ( template_level != 0 && tok_left.Type == Tok_Capture_Start)
			++ capture_level;

		if ( template_level != 0 && tok_left.Type == Tok_Capture_End)
			-- capture_level;

		if ( capture_level == 0 && template_level == 0  && tok_left.Type == Tok_Identifier )
			break;

		-- idx;
		tok_left = tokens.Arr[idx];
	}

	bool is_same = c_str_compare_len( tok_right.Text.Ptr, tok_left.Text.Ptr, tok_right.Text.Len ) == 0;
	if (tok_left.Type == Tok_Identifier && is_same)
	{
		// We have found the pattern we desired
		if (possible_destructor)
		{
			// <Name> :: ~<Name> (
			result = cast(Code, parser_parse_destructor( specifiers ));
		}
		else {
			// <Name> :: <Name> (
			result = cast(Code, parser_parse_constructor( specifiers ));
		}
	}

	return result;
}

// TODO(Ed): I want to eventually change the identifier to its own AST type.
// This would allow distinction of the qualifier for a symbol <qualifier>::<nested symboL>
// This would also allow
internal
Token parse_identifier( bool* possible_member_function )
{
	push_scope();

	Token name = currtok;
	_ctx->parser.Scope->Name = name.Text;
	eat( Tok_Identifier );
	// <Name>

	parse_template_args( & name );
	// <Name><Template Args>

	while ( check( Tok_Access_StaticSymbol ) )
	{
		eat( Tok_Access_StaticSymbol );
		// <Qualifier Name> <Template Args> ::

		Token invalid = NullToken;
		if ( left == 0 )
		{
			log_failure( "Error, unexpected end of static symbol identifier\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return invalid;
		}

		if ( currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '~' )
		{
			bool is_destructor = str_are_equal( _ctx->parser.Scope->Prev->ProcName, txt("parser_parse_destructor"));
			if (is_destructor)
			{
				name.Text.Len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )name.Text.Ptr;
				parser_pop(& _ctx->parser);
				return name;
			}

			log_failure( "Error, had a ~ operator after %SB but not a destructor\n%s", toktype_to_str( prevtok.Type ), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return invalid;
		}

		if ( currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '*' && currtok.Text.Len == 1 )
		{
			if ( possible_member_function )
				*possible_member_function = true;

			else
			{
				log_failure( "Found a member function pointer identifier but the parsing context did not expect it\n%s", parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return invalid;
			}
		}

		if ( currtok.Type != Tok_Identifier )
		{
			log_failure( "Error, expected static symbol identifier, not %s\n%s", toktype_to_str( currtok.Type ), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return invalid;
		}

		name.Text.Len = ( (sptr)currtok.Text.Ptr + currtok.Text.Len ) - (sptr)name.Text.Ptr;
		eat( Tok_Identifier );
		// <Qualifier Name> <Template Args> :: <Name>

		parse_template_args( & name );
		// <Qualifier Name> <Template Args> :: <Name> <Template Args>
	}
	// <Qualifier Name> <Template Args> :: <Name> <Template Args> ...

	parser_pop(& _ctx->parser);
	return name;
}

internal
CodeInclude parse_include()
{
	push_scope();

	CodeInclude
	include       = (CodeInclude) make_code();
	include->Type = CT_Preprocess_Include;
	eat( Tok_Preprocess_Include );
	// #include

	if ( ! check( Tok_String ))
	{
		log_failure( "Error, expected include string after #include\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	_ctx->parser.Scope->Name = currtok.Text;
	include->Content = cache_str( tok_to_str(currtok) );
	eat( Tok_String );
	// #include <Path> or "Path"

	parser_pop(& _ctx->parser);
	return include;
}

internal
CodeOperator parse_operator_after_ret_type(
	  ModuleFlag     mflags
	, CodeAttributes attributes
	, CodeSpecifiers specifiers
	, CodeTypename   ret_type
)
{
	push_scope();

	Token nspace = NullToken;
	if ( check( Tok_Identifier ) )
	{
		nspace = currtok;
		while ( left && currtok.Type == Tok_Identifier )
		{
			eat( Tok_Identifier );

			if ( currtok.Type == Tok_Access_StaticSymbol )
				eat( Tok_Access_StaticSymbol );
		}

		nspace.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)nspace.Text.Ptr;
	}
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...>

	eat( Tok_Decl_Operator );
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator

	if ( ! left && currtok.Type != Tok_Operator
		&& currtok.Type != Tok_Star
		&& currtok.Type != Tok_Ampersand
		&& currtok.Type != Tok_Ampersand_DBL )
	{
		log_failure( "Expected operator after 'operator' keyword\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	_ctx->parser.Scope->Name = currtok.Text;

	bool was_new_or_delete = false;

	Operator op = Op_Invalid;
	switch ( currtok.Text.Ptr[0] )
	{
		case '+':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_Add;

			else if ( currtok.Text.Ptr[1] == '+' )
				op = Op_Increment;

			else
				op = Op_Add;
		}
		break;
		case '-':
		{
			if ( currtok.Text.Ptr[1] == '>' )
			{
				if ( currtok.Text.Ptr[2] == '*' )
					op = Op_MemberOfPointer;

				else
					op = Op_MemberOfPointer;

				break;
			}

			else if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_Subtract;

			else
				op = Op_Subtract;
		}
		break;
		case '*':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_Multiply;

			else
			{
				Token finder = prevtok;
				while ( finder.Type != Tok_Decl_Operator )
				{
					if ( finder.Type == Tok_Identifier)
					{
						op = Op_Indirection;
						break;
					}
				}

				if ( op == Op_Invalid)
					op = Op_Multiply;
			}
		}
		break;
		case '/':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_Divide;

			else
				op = Op_Divide;
		}
		break;
		case '%':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_Modulo;

			else
				op = Op_Modulo;
		}
		break;
		case '&':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_BAnd;

			else if ( currtok.Text.Ptr[1] == '&' )
				op = Op_LAnd;

			else
			{


				if ( op == Op_Invalid )
				op = Op_BAnd;
			}
		}
		break;
		case '|':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_BOr;

			else if ( currtok.Text.Ptr[1] == '|' )
				op = Op_LOr;

			else
				op = Op_BOr;
		}
		break;
		case '^':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_Assign_BXOr;

			else
				op = Op_BXOr;
		}
		break;
		case '~':
		{
			op = Op_BNot;
		}
		break;
		case '!':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_LNot;

			else
				op = Op_UnaryNot;
		}
		break;
		case '=':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_LEqual;

			else
				op = Op_Assign;
		}
		break;
		case '<':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_LesserEqual;

			else if ( currtok.Text.Ptr[1] == '<' )
			{
				if ( currtok.Text.Ptr[2] == '=' )
					op = Op_Assign_LShift;

				else
					op = Op_LShift;
			}
			else
				op = Op_Lesser;
		}
		break;
		case '>':
		{
			if ( currtok.Text.Ptr[1] == '=' )
				op = Op_GreaterEqual;

			else if ( currtok.Text.Ptr[1] == '>' )
			{
				if ( currtok.Text.Ptr[2] == '=' )
					op = Op_Assign_RShift;

				else
					op = Op_RShift;
			}
			else
				op = Op_Greater;
		}
		break;
		case '(':
		{
			if ( currtok.Text.Ptr[1] == ')' )
				op = Op_FunctionCall;

			else
				op = Op_Invalid;
		}
		break;
		case '[':
		{
			if ( currtok.Text.Ptr[1] == ']' )
				op = Op_Subscript;

			else
				op = Op_Invalid;
		}
		break;
		default:
		{
			Str c_str_new    = operator_to_str(Op_New);
			Str c_str_delete = operator_to_str(Op_Delete);
			if ( c_str_compare_len( currtok.Text.Ptr, c_str_new.Ptr, max(c_str_new.Len - 1, currtok.Text.Len)) == 0)
			{
				op = Op_New;
				eat( Tok_Identifier );
				was_new_or_delete = true;

				s32 idx = _ctx->parser.Tokens.Idx + 1;
				{
					while ( _ctx->parser.Tokens.Arr[ idx ].Type == Tok_NewLine )
						idx++;
				}
				Token next = _ctx->parser.Tokens.Arr[idx];
				if ( currtok.Type == Tok_Operator && c_str_compare_len(currtok.Text.Ptr, "[]", 2) == 0)
				{
					eat(Tok_Operator);
					op = Op_NewArray;
				}
				else if ( currtok.Type == Tok_BraceSquare_Open && next.Type == Tok_BraceSquare_Close)
				{
					eat(Tok_BraceSquare_Open);
					eat(Tok_BraceSquare_Close);
					op = Op_NewArray;
				}
			}
			else if ( c_str_compare_len( currtok.Text.Ptr, c_str_delete.Ptr, max(c_str_delete.Len - 1, currtok.Text.Len )) == 0)
			{
				op = Op_Delete;
				eat(Tok_Identifier);
				was_new_or_delete = true;

				s32 idx = _ctx->parser.Tokens.Idx + 1;
				{
					while ( _ctx->parser.Tokens.Arr[ idx ].Type == Tok_NewLine )
						idx++;
				}
				Token next = _ctx->parser.Tokens.Arr[idx];
				if ( currtok.Type == Tok_Operator && c_str_compare_len(currtok.Text.Ptr, "[]", 2) == 0)
				{
					eat(Tok_Operator);
					op = Op_DeleteArray;
				}
				else if ( currtok.Type == Tok_BraceSquare_Open && next.Type == Tok_BraceSquare_Close)
				{
					eat(Tok_BraceSquare_Open);
					eat(Tok_BraceSquare_Close);
					op = Op_DeleteArray;
				}
			}
			else
			{
				if ( op == Op_Invalid )
				{
					log_failure( "Invalid operator '%s'\n%s", prevtok.Text, parser_to_strbuilder(_ctx->parser) );
					parser_pop(& _ctx->parser);
					return InvalidCode;
				}
			}
		}
	}

	if ( op == Op_Invalid )
	{
		log_failure( "Invalid operator '%s'\n%s", currtok.Text, parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	if ( ! was_new_or_delete)
		eat( currtok.Type );
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op>

	// Parse Params
	CodeParams params = parse_params(parser_use_parenthesis);
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> )

	if ( params == nullptr && op == Op_Multiply )
		op = Op_MemberOfPointer;

	while ( left && tok_is_specifier(currtok) )
	{
		if ( specifiers == nullptr )
		{
			specifiers = def_specifier( str_to_specifier( tok_to_str(currtok)) );
			eat( currtok.Type );
			continue;
		}

		specifiers_append(specifiers, str_to_specifier( tok_to_str(currtok)) );
		eat( currtok.Type );
	}
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers>

	// Parse Body
	CodeBody    body       = { nullptr };
	CodeComment inline_cmt = NullCode;
	if ( check( Tok_BraceCurly_Open ) )
	{
		body = cast(CodeBody, parse_function_body());
		if ( cast(Code, body) == Code_Invalid )
		{
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}
		// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers> { ... }
	}
	else
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers>;

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
			// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers>; <InlineCmt>
	}

	// OpValidateResult check_result = operator__validate( op, params, ret_type, specifiers );
	CodeOperator result = def_operator( op, tok_to_str(nspace), def_assign( params, ret_type, body, specifiers, attributes, mflags ) );

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;

	parser_pop(& _ctx->parser);
	return result;
}

internal
Code parse_operator_function_or_variable( bool expects_function, CodeAttributes attributes, CodeSpecifiers specifiers )
{
	push_scope();

	Code result = InvalidCode;

#ifndef GEN_PARSER_DISABLE_MACRO_FUNCTION_SIGNATURES
	b32 lone_macro = false;

	if ( currtok.Type == Tok_Preprocess_Macro && nexttok.Type == Tok_Statement_End )
	{
		// Were dealing with a lone macro after attributes/specifiers, there was a end statement ';' after.
		result = parse_simple_preprocess( Tok_Preprocess_Macro, parser_consume_braces );
		parser_pop(& _ctx->parser);
		return result;
		// <Attributes> <Specifiers> <Macro>
	}
#endif

	CodeTypename type = parser_parse_type( parser_not_from_template, nullptr );
	// <Attributes> <Specifiers> <ReturnType/ValueType>

	if ( type == InvalidCode )
	{
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	bool found_operator = false;
	s32  idx            = _ctx->parser.Tokens.Idx;

	for ( ; idx < array_num(_ctx->parser.Tokens.Arr); idx++ )
	{
		Token tok = _ctx->parser.Tokens.Arr[ idx ];

		if ( tok.Type == Tok_Identifier )
		{
			idx++;
			tok = _ctx->parser.Tokens.Arr[ idx ];
			if ( tok.Type == Tok_Access_StaticSymbol )
				continue;

			break;
		}

		if ( tok.Type == Tok_Decl_Operator )
			found_operator = true;

		break;
	}

	if ( found_operator )
	{
		// Dealing with an operator overload
		result = cast(Code, parse_operator_after_ret_type( ModuleFlag_None, attributes, specifiers, type ));
		// <Attributes> <Specifiers> <ReturnType> operator ...
	}
	else
	{
		Token name = parse_identifier(nullptr);
		_ctx->parser.Scope->Name = name.Text;

		bool detected_capture = check( Tok_Capture_Start );

		// Check three tokens ahead to make sure that were not dealing with a constructor initialization...
		//                  (         350.0f    ,         <---  Could be the scenario
		// Example : <Capture_Start> <Value> <Comma>
		//                 idx         +1      +2
		bool detected_comma = _ctx->parser.Tokens.Arr[ _ctx->parser.Tokens.Idx + 2 ].Type == Tok_Comma;
		if ( detected_capture && ! detected_comma )
		{
			// Dealing with a function
			result = cast(Code, parse_function_after_name( ModuleFlag_None, attributes, specifiers, type, name ));
			// <Attributes> <Specifiers> <ReturnType> <Name> ( ...
		}
		else
		{
			if ( expects_function )
			{
				log_failure( "Expected function declaration (consteval was used)\n%s", parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
			}

			// Dealing with a variable
			result = cast(Code, parse_variable_after_name( ModuleFlag_None, attributes, specifiers, type, tok_to_str(name) ));
			// <Attributes> <Specifiers> <ValueType> <Name> ...
		}
	}

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodePragma parse_pragma()
{
	push_scope();

	CodePragma
	pragma       = (CodePragma) make_code();
	pragma->Type = CT_Preprocess_Pragma;
	eat( Tok_Preprocess_Pragma );
	// #pragma

	if ( ! check( Tok_Preprocess_Content ))
	{
		log_failure( "Error, expected content after #pragma\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	_ctx->parser.Scope->Name = currtok.Text;

	pragma->Content = cache_str( tok_to_str(currtok) );
	eat( Tok_Preprocess_Content );
	// #pragma <Content>

	parser_pop(& _ctx->parser);
	return pragma;
}

internal inline
CodeParams parse_params( bool use_template_capture )
{
	push_scope();

	if ( ! use_template_capture )
		eat( Tok_Capture_Start );
	// (

	else
	{
		if ( check( Tok_Operator ) && currtok.Text.Ptr[ 0 ] == '<' )
			eat( Tok_Operator );
		// <
	}

	if ( ! use_template_capture && check( Tok_Capture_End ) )
	{
		eat( Tok_Capture_End );
		// )
		parser_pop(& _ctx->parser);
		return NullCode;
	}
	else if ( check( Tok_Operator ) && currtok.Text.Ptr[ 0 ] == '>' )
	{
		eat( Tok_Operator );
		// >
		parser_pop(& _ctx->parser);
		return NullCode;
	}

	Code         macro           = { nullptr };
	CodeTypename type            = { nullptr };
	Code         value           = { nullptr };
	Token        name            = NullToken;
	Code         post_name_macro = { nullptr };

	if ( check( Tok_Varadic_Argument ) )
	{
		eat( Tok_Varadic_Argument );
		// ( or <  ...

		parser_pop(& _ctx->parser);
		return param_varadic;
		// ( ... )
		// or < ... >
	}

	#define CheckEndParams() \
		(use_template_capture ? (currtok.Text.Ptr[ 0 ] != '>') : (currtok.Type != Tok_Capture_End))

	// Ex: Unreal has this type of macro:                 vvvvvvvvv
	// COREUOBJECT_API void CallFunction( FFrame& Stack, RESULT_DECL, UFunction* Function );
	// and:                 vvvv
	// AddComponentByClass(UPARAM(meta = (AllowAbstract = "false")) TSubclassOf<UActorComponent> Class, bool bManualAttachment, ...
	if ( check(Tok_Preprocess_Macro))
	{
		macro = parse_simple_preprocess(Tok_Preprocess_Macro, parser_consume_braces);
		// ( <Macro>
	}
	if ( currtok.Type != Tok_Comma )
	{
		type = parser_parse_type( use_template_capture, nullptr );
		if ( cast(Code, type) == Code_Invalid )
		{
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}
		// ( <Macro> <ValueType>

		if ( check( Tok_Identifier ) )
		{
			name = currtok;
			eat( Tok_Identifier );
			// ( <Macro> <ValueType> <Name>
		}

		// Unreal has yet another type of macro:
		// template<class T UE_REQUIRES(TPointerIsConvertibleFromTo<T, UInterface>::Value)>
		// class T ... and then ^this^ UE_REQUIRES shows up
		// So we need to consume that.
		if ( check( Tok_Preprocess_Macro ))
		{
			post_name_macro = parse_simple_preprocess( Tok_Preprocess_Macro, parser_consume_braces );
		}

		// In template captures you can have a typename have direct assignment without a name
		// typename = typename ...
		// Which would result in a static value type from a struct expansion (traditionally)
		if ( ( name.Text.Ptr || use_template_capture ) && bitfield_is_equal( u32, currtok.Flags, TF_Assign ) )
		{
			eat( Tok_Operator );
			// ( <Macro> <ValueType> <Name> =

			Token value_tok = currtok;

			if ( currtok.Type == Tok_Comma )
			{
				log_failure( "Expected value after assignment operator\n%s.", parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
			}

			s32 capture_level  = 0;
			s32 template_level = 0;
			while ( (left && ( currtok.Type != Tok_Comma ) && template_level >= 0 && CheckEndParams()) || (capture_level > 0 || template_level > 0) )
			{
				if (currtok.Text.Ptr[ 0 ] == '<')
					++ template_level;

				if (currtok.Text.Ptr[ 0 ] == '>')
					-- template_level;
				if (currtok.Type == Tok_Operator && currtok.Text.Ptr[1] == '>')
					-- template_level;

				if ( currtok.Type == Tok_Capture_Start)
					++ capture_level;

				if ( currtok.Type == Tok_Capture_End)
					-- capture_level;

				value_tok.Text.Len = ( ( sptr )currtok.Text.Ptr + currtok.Text.Len ) - ( sptr )value_tok.Text.Ptr;
				eat( currtok.Type );
			}

			value = untyped_str( strbuilder_to_str(parser_strip_formatting( tok_to_str(value_tok), parser_strip_formatting_dont_preserve_newlines )) );
			// ( <Macro> <ValueType> <Name> = <Expression>
		}
	}

	CodeParams result = ( CodeParams )make_code();
	result->Type     = CT_Parameters;

	result->Macro = macro;

	if ( name.Text.Len > 0 )
		result->Name = cache_str( tok_to_str(name) );

	result->ValueType = type;

	if ( value )
		result->Value = value;

	result->NumEntries++;

	while ( check(Tok_Comma) )
	{
		eat( Tok_Comma );
		// ( <Macro> <ValueType> <Name> = <Expression>,

		Code type  = { nullptr };
		Code value = { nullptr };

		if ( check( Tok_Varadic_Argument ) )
		{
			eat( Tok_Varadic_Argument );
			params_append(result, param_varadic );
			continue;
			// ( <Macro> <ValueType> <Name> = <Expression>, ...
		}

		// Ex: Unreal has this type of macro:                 vvvvvvvvv
		// COREUOBJECT_API void CallFunction( FFrame& Stack, RESULT_DECL, UFunction* Function );
		// and:                 vvvv
		// AddComponentByClass(UPARAM(meta = (AllowAbstract = "false")) TSubclassOf<UActorComponent> Class, bool bManualAttachment, ...
		if ( check(Tok_Preprocess_Macro))
		{
			macro = parse_simple_preprocess(Tok_Preprocess_Macro, parser_dont_consume_braces);
			// ( <Macro>
		}
		if ( currtok.Type != Tok_Comma )
		{
			type = cast(Code, parser_parse_type( use_template_capture, nullptr ));
			if ( type == Code_Invalid )
			{
				parser_pop(& _ctx->parser);
				return InvalidCode;
			}
			// ( <Macro> <ValueType> <Name> = <Expression>, <Macro> <ValueType>

			name = NullToken;

			if ( check( Tok_Identifier ) )
			{
				name = currtok;
				eat( Tok_Identifier );
				// ( <Macro> <ValueType> <Name> = <Expression>, <Macro> <ValueType> <Name>
			}

			// Unreal has yet another type of macro:
			// template<class T UE_REQUIRES(TPointerIsConvertibleFromTo<T, UInterface>::Value)>
			// class T ... and then ^this^ UE_REQUIRES shows up
			// So we need to consume that.
			if ( check( Tok_Preprocess_Macro ))
			{
				post_name_macro = parse_simple_preprocess( Tok_Preprocess_Macro, parser_dont_consume_braces );
			}

			// In template captures you can have a typename have direct assignment without a name
			// typename = typename ...
			// Which would result in a static value type from a struct expansion (traditionally)
			if ( ( name.Text.Ptr || use_template_capture ) && bitfield_is_equal( u32, currtok.Flags, TF_Assign ) )
			{
				eat( Tok_Operator );
				// ( <Macro> <ValueType> <Name> = <Expression>, <Macro> <ValueType> <Name> =

				Token value_tok = currtok;

				if ( currtok.Type == Tok_Comma )
				{
					log_failure( "Expected value after assignment operator\n%s", parser_to_strbuilder(_ctx->parser) );
					parser_pop(& _ctx->parser);
					return InvalidCode;
				}

				s32 capture_level  = 0;
				s32 template_level = 0;
				while ( (left
				&& currtok.Type != Tok_Comma
				&& template_level >= 0
				&& CheckEndParams()) || (capture_level > 0 || template_level > 0) )
				{
					if (currtok.Text.Ptr[ 0 ] == '<')
						++ template_level;

					if (currtok.Text.Ptr[ 0 ] == '>')
						-- template_level;
					if (currtok.Type == Tok_Operator && currtok.Text.Ptr[1] == '>')
						-- template_level;

					if ( currtok.Type == Tok_Capture_Start)
						++ capture_level;

					if ( currtok.Type == Tok_Capture_End)
						-- capture_level;

					value_tok.Text.Len = ( ( sptr )currtok.Text.Ptr + currtok.Text.Len ) - ( sptr )value_tok.Text.Ptr;
					eat( currtok.Type );
				}

				value = untyped_str( strbuilder_to_str(parser_strip_formatting( tok_to_str(value_tok), parser_strip_formatting_dont_preserve_newlines )) );
				// ( <Macro> <ValueType> <Name> = <Expression>, <Macro> <ValueType> <Name> = <Expression>
			}
			// ( <Macro> <ValueType> <Name> = <Expression>, <Macro> <ValueType> <Name> = <Expression>, ..
		}

		CodeParams param = ( CodeParams )make_code();
		param->Type     = CT_Parameters;

		param->Macro = macro;

		if ( name.Text.Len > 0 )
			param->Name = cache_str( tok_to_str(name) );

		param->PostNameMacro = post_name_macro;
		param->ValueType     = cast(CodeTypename, type);

		if ( value )
			param->Value = value;

		params_append(result, param );
	}

	if ( ! use_template_capture )
		eat( Tok_Capture_End );
	// ( <Macro> <ValueType> <Name> = <Expression>, <Macro> <ValueType> <Name> = <Expression>, .. )

	else
	{
		if ( ! check( Tok_Operator ) || currtok.Text.Ptr[ 0 ] != '>' )
		{
			log_failure( "Expected '<' after 'template' keyword\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}
		eat( Tok_Operator );
		// < <Macro> <ValueType> <Name> = <Expression>, <Macro> <ValueType> <Name> = <Expression>, .. >
	}

	parser_pop(& _ctx->parser);
	return result;
#undef context
}

internal
CodePreprocessCond parse_preprocess_cond()
{
	push_scope();

	if ( ! tok_is_preprocess_cond(currtok) )
	{
		log_failure( "Error, expected preprocess conditional\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	CodePreprocessCond
	cond       = (CodePreprocessCond) make_code();
	cond->Type = scast(CodeType, currtok.Type - ( Tok_Preprocess_If - CT_Preprocess_If ) );
	eat( currtok.Type );
	// #<Conditional>

	if ( ! check( Tok_Preprocess_Content ))
	{
		log_failure( "Error, expected content after #define\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	_ctx->parser.Scope->Name = currtok.Text;
	cond->Content = cache_str( tok_to_str(currtok) );
	eat( Tok_Preprocess_Content );
	// #<Conditiona> <Content>

	parser_pop(& _ctx->parser);
	return cond;
}

internal
Code parse_simple_preprocess( TokType which, bool dont_consume_braces )
{
	// TODO(Ed): We can handle a macro a bit better than this. It's AST can be made more robust..
	// Make an AST_Macro, it should have an Name be the macro itself, with the function body being an optional function body node.
	// If we want it to terminate or have an inline comment we can possbily use its parent typedef for that info...
	push_scope();

	Token tok = currtok;
	eat( which );
	// <Macro>

	if ( ! dont_consume_braces && peektok.Type == Tok_BraceCurly_Open )
	{
		// Eat the block scope right after the macro. Were assuming the macro defines a function definition's signature
		eat( Tok_BraceCurly_Open );
		// <Macro> {

		// TODO(Ed) : Parse this properly later (expression and statement support)
		s32 level = 0;
		while ( left && ( currtok.Type != Tok_BraceCurly_Close || level > 0 ) )
		{
			if ( currtok.Type == Tok_BraceCurly_Open )
				level++;

			else if ( currtok.Type == Tok_BraceCurly_Close && level > 0 )
				level--;

			eat( currtok.Type );
		}
		eat( Tok_BraceCurly_Close );
		// <Macro> { <Body> }

		Str prev_proc = _ctx->parser.Scope->Prev->ProcName;
		if ( c_str_compare_len( prev_proc.Ptr, "parser_parse_typedef", prev_proc.Len ) != 0 )
		{
			if ( check( Tok_Statement_End ))
			{
				Token stmt_end = currtok;
				eat( Tok_Statement_End );
				// <Macro> { <Content> };

				if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
					eat( Tok_Comment );
					// <Macro> { <Content> }; <InlineCmt>
			}
		}

		tok.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)tok.Text.Ptr;
	}
	else
	{
		// If the macro is just a macro in the body of an AST it may have a semi-colon for the user to close on purpsoe
		// (especially for functional macros)
		Str calling_proc = _ctx->parser.Scope->Prev->ProcName;

		if (str_contains(_ctx->parser.Scope->Prev->ProcName, txt("parser_parse_enum")))
		{
			// Do nothing
			goto Leave_Scope_Early;
		}
		else if (str_contains(_ctx->parser.Scope->Prev->ProcName, txt("parser_parse_typedef")))
		{
			// TODO(Ed): Reveiw the context for this?
			if ( peektok.Type == Tok_Statement_End )
			{
				Token stmt_end = currtok;
				eat( Tok_Statement_End );
				// <Macro>;

				tok.Text.Len += prevtok.Text.Len;

				// TODO(Ed): Reveiw the context for this? (ESPECIALLY THIS)
				if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
				{
					eat( Tok_Comment );
					// <Macro>; <InlineCmt>

					tok.Text.Len += prevtok.Text.Len;
				}
			}
		}
		else if (
				str_contains(calling_proc, txt("parse_global_nspace"))
			||	str_contains(calling_proc, txt("parse_class_struct_body"))
		)
		{
			if (peektok.Type == Tok_Statement_End)
			{
				Token stmt_end = currtok;
				eat( Tok_Statement_End );
				// <Macro>;
				tok.Text.Len += prevtok.Text.Len;
			}

		}
		// tok.Length = ( (sptr)prevtok.Text + prevtok.Length ) - (sptr)tok.Text;
	}

Leave_Scope_Early:

	char const* content = c_str_fmt_buf( "%.*s ", tok.Text.Len, tok.Text.Ptr );

	Code result = untyped_str( to_str_from_c_str(content) );
	_ctx->parser.Scope->Name = tok.Text;

	parser_pop(& _ctx->parser);
	return result;
}

internal
Code parse_static_assert()
{
	push_scope();

	Code
	assert       = make_code();
	assert->Type = CT_Untyped;

	Token content = currtok;

	_ctx->parser.Scope->Name = content.Text;

	eat( Tok_StaticAssert );
	eat( Tok_Capture_Start );
	// static_assert(

	// TODO(Ed) : Parse this properly.
	s32 level = 0;
	while ( left && ( currtok.Type != Tok_Capture_End || level > 0 ) )
	{
		if ( currtok.Type == Tok_Capture_Start )
			level++;
		else if ( currtok.Type == Tok_Capture_End )
			level--;

		eat( currtok.Type );
	}
	eat( Tok_Capture_End );
	eat( Tok_Statement_End );
	// static_assert( <Content> );

	content.Text.Len  = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)content.Text.Ptr;

	char const* str  = c_str_fmt_buf( "%.*s\n", content.Text.Len, content.Text.Ptr );
	Str content_str = { str, content.Text.Len + 1 };
	assert->Content = cache_str( content_str );
	assert->Name	= assert->Content;

	parser_pop(& _ctx->parser);
	return assert;
}

/*
	This a brute-froce make all the arguments part of the token provided.
	Can have in-place function signatures, regular identifiers, in-place typenames, compile-time expressions, parameter-pack expansion, etc.
	This means that validation can only go so far, and so if there is any different in formatting
	passed the basic stripping supported it report a soft failure.
*/
internal inline
void parse_template_args( Token* token )
{
	if ( currtok.Type == Tok_Operator && currtok.Text.Ptr[ 0 ] == '<' && currtok.Text.Len == 1 )
	{
		eat( Tok_Operator );
		// <

		s32 level = 0;
		while ( left && level >= 0 && ( currtok.Text.Ptr[ 0 ] != '>' || level > 0 ) )
		{
			if ( currtok.Text.Ptr[ 0 ] == '<' )
				level++;

			if ( currtok.Text.Ptr[ 0 ] == '>' )
				level--;
			if ( currtok.Type == Tok_Operator && currtok.Text.Ptr[1] == '>')
				level--;

			eat( currtok.Type );
		}
		// < <Content>

		// Due to the >> token, this could have been eaten early...
		if (level == 0)
			eat( Tok_Operator );
		// < <Content> >

		// Extend length of name to last token
		token->Text.Len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )token->Text.Ptr;
	}
}

// Variable parsing is handled in multiple places because its initial signature is shared with function parsing
internal
CodeVar parse_variable_after_name(
	  ModuleFlag        mflags
	, CodeAttributes    attributes
	, CodeSpecifiers    specifiers
	, CodeTypename      type
	, Str              name
)
{
	push_scope();

	Code array_expr    = parse_array_decl();
	Code expr          = { nullptr };
	Code bitfield_expr = { nullptr };

	b32 using_constructor_initializer = false;

	if ( bitfield_is_equal( u32, currtok.Flags, TF_Assign ) )
	{
		// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>
		expr = parse_assignment_expression();
	}

	if ( currtok.Type == Tok_BraceCurly_Open )
	{
		Token expr_tok = currtok;

		eat( Tok_BraceCurly_Open );
		// <Attributes> <Specifiers> <ValueType> <Name> {

		s32 level = 0;
		while ( left && ( currtok.Type != Tok_BraceCurly_Close || level > 0 ) )
		{
			if ( currtok.Type == Tok_BraceCurly_Open )
				level++;

			else if ( currtok.Type == Tok_BraceCurly_Close && level > 0 )
				level--;

			eat( currtok.Type );
		}
		eat( Tok_BraceCurly_Close );

		expr_tok.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)expr_tok.Text.Ptr;
		expr              = untyped_str( tok_to_str(expr_tok) );
		// <Attributes> <Specifiers> <ValueType> <Name> = { <Expression> }
	}

	if ( currtok.Type == Tok_Capture_Start )
	{
		eat( Tok_Capture_Start);
		// <Attributes> <Specifiers> <ValueType> <Name> (

		Token expr_token = currtok;

		using_constructor_initializer = true;

		s32 level = 0;
		while ( left && ( currtok.Type != Tok_Capture_End || level > 0 ) )
		{
			if ( currtok.Type == Tok_Capture_Start )
				level++;

			else if ( currtok.Type == Tok_Capture_End && level > 0 )
				level--;

			eat( currtok.Type );
		}

		expr_token.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)expr_token.Text.Ptr;
		expr                = untyped_str( tok_to_str(expr_token) );
		eat( Tok_Capture_End );
		// <Attributes> <Specifiers> <ValueType> <Name> ( <Expression> )
	}

	if ( currtok.Type == Tok_Assign_Classifer )
	{
		eat( Tok_Assign_Classifer );
		// <Attributes> <Specifiers> <ValueType> <Name> :

		Token expr_tok = currtok;

		if ( currtok.Type == Tok_Statement_End )
		{
			log_failure( "Expected expression after bitfield \n%SB", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		while ( left && currtok.Type != Tok_Statement_End )
		{
			eat( currtok.Type );
		}

		expr_tok.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)expr_tok.Text.Ptr;
		bitfield_expr     = untyped_str( tok_to_str(expr_tok) );
		// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>
	}

	CodeVar     next_var   = NullCode;
	Token       stmt_end   = NullToken;
	CodeComment inline_cmt = NullCode;
	if ( type )
	{
		if ( currtok.Type == Tok_Comma )
		{
			// Were dealing with a statement with more than one declaration
			// This is only handled this way if its the first declaration
			// Otherwise its looped through in parse_variable_declaration_list
			next_var = parse_variable_declaration_list();
			// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>, ...
			// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>, ...
			// <Attributes> <Specifiers> <ValueType> <Name> { <Expression> }, ...
		}

		// If we're dealing with a "comma-procedding then we cannot expect a statement end or inline comment
		// Any comma procedding variable will not have a type provided so it can act as a indicator to skip this
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>, ...;
		// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>, ...;
		// <Attributes> <Specifiers> <ValueType> <Name> { <Expression> }, ...;

		// Check for inline comment : <type> <identifier> = <expression>; // <inline comment>
		if ( left && ( currtok_noskip.Type == Tok_Comment ) && currtok_noskip.Line == stmt_end.Line )
		{
			inline_cmt = parse_comment();
			// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>, ...; <InlineCmt>
			// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>, ...; <InlineCmt>
			// <Attributes> <Specifiers> <ValueType> <Name> { <Expression> }, ...; <InlineCmt>
		}
	}

	CodeVar
	result              = (CodeVar) make_code();
	result->Type        = CT_Variable;
	result->Name        = cache_str( name );
	result->ModuleFlags = mflags;

	// Type can be null if we're dealing with a declaration from a variable declaration-list
	if ( type )
		result->ValueType = type;

	if (array_expr )
		type->ArrExpr = array_expr;

	if ( bitfield_expr )
		result->BitfieldSize = bitfield_expr;

	if ( attributes )
		result->Attributes = attributes;

	if ( specifiers )
		result->Specs = specifiers;

	if ( expr )
		result->Value = expr;

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;

	if ( next_var )
	{
		result->NextVar         = next_var;
		result->NextVar->Parent = cast(Code, result);
	}

	result->VarParenthesizedInit = using_constructor_initializer;

	parser_pop(& _ctx->parser);
	return result;
}

/*
	Note(Ed): This does not support the following:
	* Function Pointers
*/
internal
CodeVar parse_variable_declaration_list()
{
	push_scope();

	CodeVar result   = NullCode;
	CodeVar last_var = NullCode;
	while ( check( Tok_Comma ) )
	{
		eat( Tok_Comma );
		// ,

		CodeSpecifiers specifiers = NullCode;

		while ( left && tok_is_specifier(currtok) )
		{
			Specifier spec = str_to_specifier( tok_to_str(currtok) );

			switch ( spec )
			{
				case Spec_Const:
					if ( specifiers->NumEntries && specifiers->ArrSpecs[ specifiers->NumEntries - 1 ] != Spec_Ptr )
					{
						log_failure( "Error, const specifier must come after pointer specifier for variable declaration proceeding comma\n"
						"(Parser will add and continue to specifiers, but will most likely fail to compile)\n%SB"
						, parser_to_strbuilder(_ctx->parser) );

						specifiers_append(specifiers, spec );
					}
				break;

				case Spec_Ptr:
				case Spec_Ref:
				case Spec_RValue:
				break;

				default:
				{
					log_failure( "Error, invalid specifier '%S' proceeding comma\n"
					"(Parser will add and continue to specifiers, but will most likely fail to compile)\n%S"
					, tok_to_str(currtok), strbuilder_to_str( parser_to_strbuilder(_ctx->parser)) );
					continue;
				}
				break;
			}

			if ( specifiers )
				specifiers_append(specifiers, spec );
			else
				specifiers = def_specifier( spec );

			eat(currtok.Type);
		}
		// , <Specifiers>

		Str name = tok_to_str(currtok);
		eat( Tok_Identifier );
		// , <Specifiers> <Name>

		CodeVar var = parse_variable_after_name( ModuleFlag_None, NullCode, specifiers, NullCode, name );
		// , <Specifiers> <Name> ...

		if ( ! result )
		{
			result   = var;
			last_var = var;
		}
		else
		{
			last_var->NextVar         = var;
			last_var->NextVar->Parent = cast(Code, var);
			last_var                  = var;
		}
	}

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeClass parser_parse_class( bool inplace_def )
{
	push_scope();
	CodeClass result = (CodeClass) parse_class_struct( Tok_Decl_Class, inplace_def );
	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeConstructor parser_parse_constructor( CodeSpecifiers specifiers )
{
	push_scope();

	Token     identifier = parse_identifier(nullptr);
	CodeParams params     = parse_params(parser_not_from_template);
	// <Name> ( <Parameters> )

	Code        initializer_list = NullCode;
	CodeBody    body             = NullCode;
	CodeComment inline_cmt       = NullCode;

	// TODO(Ed) : Need to support postfix specifiers

	if ( check( Tok_Assign_Classifer ) )
	{
		eat( Tok_Assign_Classifer );
		// <Name> ( <Parameters> ) :

		Token initializer_list_tok = currtok;

		s32 level                  = 0;
		while ( left && ( currtok.Type != Tok_BraceCurly_Open || level > 0 ) )
		{
			if (currtok.Type == Tok_Capture_Start)
				level++;
			else if ( currtok.Type == Tok_Capture_End )
				level--;

			eat( currtok.Type );
		}

		initializer_list_tok.Text.Len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )initializer_list_tok.Text.Ptr;
		// <Name> ( <Parameters> ) : <InitializerList>

		initializer_list = untyped_str( tok_to_str(initializer_list_tok) );

		// TODO(Ed): Constructors can have post-fix specifiers

		body = cast(CodeBody, parse_function_body());
		// <Name> ( <Parameters> ) : <InitializerList> { <Body> }
	}
	else if ( check( Tok_BraceCurly_Open ) )
	{
		body = cast(CodeBody, parse_function_body());
		// <Name> ( <Parameters> ) { <Body> }
	}
	else if ( check( Tok_Operator) && currtok.Text.Ptr[ 0 ] == '=' )
	{
		body = cast(CodeBody, parse_assignment_expression());
	}
	else
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <Name> ( <Parameters> );

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
		// <Name> ( <Parameters> ); <InlineCmt>
	}

	CodeConstructor result = ( CodeConstructor )make_code();

	result->Name = cache_str( tok_to_str(identifier));

	result->Specs = specifiers;

	if ( params )
		result->Params = params;

	if ( initializer_list )
		result->InitializerList = initializer_list;

	if ( body && body->Type == CT_Function_Body )
	{
		result->Body = cast(Code, body);
		result->Type = CT_Constructor;
	}
	else
		result->Type = CT_Constructor_Fwd;

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeDestructor parser_parse_destructor( CodeSpecifiers specifiers )
{
	push_scope();

	bool has_context         = _ctx->parser.Scope && _ctx->parser.Scope->Prev;
	bool is_in_global_nspace = has_context && str_are_equal( _ctx->parser.Scope->Prev->ProcName, txt("parse_global_nspace") );

	if ( check( Tok_Spec_Virtual ) )
	{
		if ( specifiers )
			specifiers_append(specifiers, Spec_Virtual );
		else
			specifiers = def_specifier( Spec_Virtual );
		eat( Tok_Spec_Virtual );
	}
	// <Virtual Specifier>

	Token prefix_identifier = NullToken;
	if (is_in_global_nspace)
		prefix_identifier = parse_identifier(nullptr);

	if ( left && currtok.Text.Ptr[ 0 ] == '~' )
		eat( Tok_Operator );
	else
	{
		log_failure( "Expected destructor '~' token\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop( & _ctx->parser);
		return InvalidCode;
	}
	// <Virtual Specifier> ~

	Token       identifier = parse_identifier(nullptr);
	CodeBody    body       = { nullptr };
	CodeComment inline_cmt = NullCode;
	// <Virtual Specifier> ~<Name>

	eat( Tok_Capture_Start );
	eat( Tok_Capture_End );
	// <Virtual Specifier> ~<Name>()

	bool pure_virtual = false;

	if ( check( Tok_Operator ) && currtok.Text.Ptr[ 0 ] == '=' )
	{
		// <Virtual Specifier> ~<Name>() =

		bool skip_formatting = true;
		Token upcoming = nexttok;
		if ( left && upcoming.Text.Ptr[ 0 ] == '0' )
		{
			eat( Tok_Operator );
			eat( Tok_Number );
			// <Virtual Specifier> ~<Name>() = 0

			specifiers_append(specifiers, Spec_Pure );
		}
		else if ( left && c_str_compare_len( upcoming.Text.Ptr, "default", sizeof("default") - 1 ) == 0)
		{
			body = cast(CodeBody, parse_assignment_expression());
			// <Virtual Specifier> ~<
		}
		else
		{
			log_failure( "Pure or default specifier expected due to '=' token\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop( & _ctx->parser);
			return InvalidCode;
		}

		pure_virtual = true;
	}

	if ( ! pure_virtual && check( Tok_BraceCurly_Open ) )
		body = cast(CodeBody, parse_function_body());
	// <Virtual Specifier> ~<Name>() { ... }
	else
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <Virtual Specifier> ~<Name>() <Pure Specifier>;

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
		// <Virtual Specifier> ~<Name>() <Pure Specifier>; <InlineCmt>
	}

	CodeDestructor result = ( CodeDestructor )make_code();

	if ( tok_is_valid(prefix_identifier) )
	{
		prefix_identifier.Text.Len += 1 + identifier.Text.Len;
		result->Name = cache_str( tok_to_str(prefix_identifier) );
	}

	if ( specifiers )
		result->Specs = specifiers;

	if ( body && body->Type == CT_Function_Body )
	{
		result->Body = cast(Code, body);
		result->Type = CT_Destructor;
	}
	else
		result->Type = CT_Destructor_Fwd;

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeEnum parser_parse_enum( bool inplace_def )
{
	push_scope();

	Specifier specs_found[16] = { Spec_NumSpecifiers };
	s32       NumSpecifiers = 0;

	CodeAttributes attributes = { nullptr };

	Token        name       = NullToken;
	Code         array_expr = { nullptr };
	CodeTypename type       = { nullptr };

	char  entries_code[ kilobytes(128) ] = { 0 };
	s32   entries_length = 0;

	bool is_enum_class = false;

	eat( Tok_Decl_Enum );
	// enum

	if ( currtok.Type == Tok_Decl_Class )
	{
		eat( Tok_Decl_Class);
		is_enum_class = true;
		// enum class
	}

	attributes = parse_attributes();
	// enum <class> <Attributes>

	if ( check( Tok_Identifier ) )
	{
		name = currtok;
		_ctx->parser.Scope->Name = currtok.Text;
		eat( Tok_Identifier );
	}
	// enum <class> <Attributes> <Name>

	b32  use_macro_underlying = false;
	Code underlying_macro     = { nullptr };
	if ( currtok.Type == Tok_Assign_Classifer )
	{
		eat( Tok_Assign_Classifer );
		// enum <class> <Attributes> <Name> :

		type = parser_parse_type(parser_not_from_template, nullptr);
		if ( cast(Code, type) == Code_Invalid )
		{
			log_failure( "Failed to parse enum classifier\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}
		// enum <class> <Attributes> <Name> : <UnderlyingType>
	}
	else if ( currtok.Type == Tok_Preprocess_Macro )
	{
		// We'll support the enum_underlying macro
		if ( str_contains( tok_to_str(currtok), enum_underlying_sig) )
		{
			use_macro_underlying = true;
			underlying_macro     = parse_simple_preprocess( Tok_Preprocess_Macro, parser_dont_consume_braces );
		}
	}

	CodeBody body = { nullptr };

	if ( currtok.Type == Tok_BraceCurly_Open )
	{
		body       = (CodeBody) make_code();
		body->Type = CT_Enum_Body;

		eat( Tok_BraceCurly_Open );
		// enum <class> <Attributes> <Name> : <UnderlyingType> {

		Code member = InvalidCode;

		bool expects_entry = true;
		while ( left && currtok_noskip.Type != Tok_BraceCurly_Close )
		{
			if ( ! expects_entry )
			{
				log_failure( "Did not expect an entry after last member of enum body.\n%s", parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				break;
			}

			if ( currtok_noskip.Type == Tok_Preprocess_Hash )
				eat( Tok_Preprocess_Hash );

			switch ( currtok_noskip.Type )
			{
				case Tok_NewLine:
					member = untyped_str( tok_to_str(currtok_noskip) );
					eat( Tok_NewLine );
				break;

				case Tok_Comment:
					member = cast(Code, parse_comment());
				break;

				case Tok_Preprocess_Define:
					member = cast(Code, parse_define());
					// #define
				break;

				case Tok_Preprocess_If:
				case Tok_Preprocess_IfDef:
				case Tok_Preprocess_IfNotDef:
				case Tok_Preprocess_ElIf:
					member = cast(Code, parse_preprocess_cond());
					// #<if, ifdef, ifndef, elif> ...
				break;

				case Tok_Preprocess_Else:
					member = cast(Code, preprocess_else);
					eat( Tok_Preprocess_Else );
				break;

				case Tok_Preprocess_EndIf:
					member = cast(Code, preprocess_endif);
					eat( Tok_Preprocess_EndIf );
				break;

				case Tok_Preprocess_Macro:
					member = cast(Code, parse_simple_preprocess( Tok_Preprocess_Macro, parser_consume_braces));
					// <Macro>
				break;

				case Tok_Preprocess_Pragma:
					member = cast(Code, parse_pragma());
					// #pragma
				break;

				case Tok_Preprocess_Unsupported:
					member = cast(Code, parse_simple_preprocess( Tok_Preprocess_Unsupported, parser_consume_braces ));
					// #<UNSUPPORTED>
				break;

				default:
					Token entry = currtok;

					eat( Tok_Identifier);
					// <Name>

					if ( currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '=' )
					{
						eat( Tok_Operator );
						// <Name> =

						while ( currtok.Type != Tok_Comma && currtok.Type != Tok_BraceCurly_Close )
						{
							eat( currtok.Type );
						}
					}
					// <Name> = <Expression>

					// Unreal UMETA macro support
					if ( currtok.Type == Tok_Preprocess_Macro )
					{
						eat( Tok_Preprocess_Macro );
						// <Name> = <Expression> <Macro>
					}

					if ( currtok.Type == Tok_Comma )
					{
						//Token prev = * previous(_ctx->parser.Tokens, dont_skip_formatting);
						//entry.Length = ( (sptr)prev.Text + prev.Length ) - (sptr)entry.Text;

						eat( Tok_Comma );
						// <Name> = <Expression> <Macro>,
					}

					// Consume inline comments
					// if ( currtok.Type == Tok_Comment && prevtok.Line == currtok.Line )
					// {
						// eat( Tok_Comment );
						// <Name> = <Expression> <Macro>, // <Inline Comment>
					// }

					Token prev     = * lex_previous(_ctx->parser.Tokens, lex_dont_skip_formatting);
					entry.Text.Len = ( (sptr)prev.Text.Ptr + prev.Text.Len ) - (sptr)entry.Text.Ptr;

					member = untyped_str( tok_to_str(entry) );
				break;
			}

			if ( member == Code_Invalid )
			{
				log_failure( "Failed to parse member\n%s", parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
			}

			body_append(body, member );
		}

		eat( Tok_BraceCurly_Close );
		// enum <class> <Attributes> <Name> : <UnderlyingType> { <Body> }
	}

	CodeComment inline_cmt = NullCode;

	if ( ! inplace_def )
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// enum <class> <Attributes> <Name> : <UnderlyingType> { <Body> };

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
			// enum <class> <Attributes> <Name> : <UnderlyingType> { <Body> }; <InlineCmt>
	}

	CodeEnum
	result = (CodeEnum) make_code();

	if ( body )
	{
		result->Type = is_enum_class ? CT_Enum_Class : CT_Enum;
		result->Body = body;
	}
	else
	{
		result->Type = is_enum_class ? CT_Enum_Class_Fwd : CT_Enum_Fwd;
	}

	result->Name = cache_str( tok_to_str(name) );

	if ( attributes )
		result->Attributes = attributes;

	result->UnderlyingTypeMacro = underlying_macro;
	result->UnderlyingType      = type;

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;


	parser_pop(& _ctx->parser);
	return result;
}

internal inline
CodeBody parser_parse_export_body()
{
	push_scope();
	CodeBody result = parse_global_nspace( CT_Export_Body );
	parser_pop(& _ctx->parser);
	return result;
}

internal inline
CodeBody parser_parse_extern_link_body()
{
	push_scope();
	CodeBody result = parse_global_nspace( CT_Extern_Linkage_Body );
	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeExtern parser_parse_extern_link()
{
	push_scope();

	eat( Tok_Decl_Extern_Linkage );
	// extern

	Token name = currtok;
	eat( Tok_String );
	// extern "<Name>"

	name.Text.Ptr += 1;
	name.Text.Len -= 1;

	CodeExtern
	result       = (CodeExtern) make_code();
	result->Type = CT_Extern_Linkage;
	result->Name = cache_str( tok_to_str(name) );

	CodeBody entry = parser_parse_extern_link_body();
	if ( cast(Code, entry) == Code_Invalid )
	{
		log_failure( "Failed to parse body\n%s", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return result;
	}
	// extern "<Name>" { <Body> }

	result->Body = entry;

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeFriend parser_parse_friend()
{
	push_scope();

	eat( Tok_Decl_Friend );
	// friend

	CodeFn         function   = { nullptr };
	CodeOperator   op         = { nullptr };
	CodeSpecifiers specifiers = { nullptr };

	// Specifiers Parsing
	{
		Specifier specs_found[ 16 ] = { Spec_NumSpecifiers };
		s32       NumSpecifiers = 0;

		while ( left && tok_is_specifier(currtok) )
		{
			Specifier spec = str_to_specifier( tok_to_str(currtok) );

			switch ( spec )
			{
				case Spec_Const :
				case Spec_Inline :
				case Spec_ForceInline :
					break;

				default :
					log_failure( "Invalid specifier %S for friend definition\n%S", spec_to_str( spec ), strbuilder_to_str( parser_to_strbuilder(_ctx->parser)) );
					parser_pop(& _ctx->parser);
					return InvalidCode;
			}

			// Ignore const it will be handled by the type
			if ( spec == Spec_Const )
				break;

			specs_found[ NumSpecifiers ] = spec;
			NumSpecifiers++;
			eat( currtok.Type );
		}

		if ( NumSpecifiers )
		{
			specifiers = def_specifiers( NumSpecifiers, specs_found );
		}
		// <friend> <specifiers>
	}

	// Type declaration or return type
	CodeTypename type = parser_parse_type(parser_not_from_template, nullptr);
	if ( cast(Code, type) == Code_Invalid )
	{
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}
	// friend <Type>

	// Funciton declaration
	if ( currtok.Type == Tok_Identifier )
	{
		// Name
		Token name          = parse_identifier(nullptr);
		_ctx->parser.Scope->Name = name.Text;
		// friend <ReturnType> <Name>

		function = parse_function_after_name( ModuleFlag_None, NullCode, specifiers, type, name );

		// Parameter list
		// CodeParams params = parse_params();
		// friend <ReturnType> <Name> ( <Parameters> )

		// function             = make_code();
		// function->Type       = Function_Fwd;
		// function->Name       = cache_str( name );
		// function->ReturnType = type;

		// if ( params )
			// function->Params = params;
	}

	// Operator declaration or definition
	if ( currtok.Type == Tok_Decl_Operator )
	{
		op = parse_operator_after_ret_type( ModuleFlag_None, NullCode, specifiers, type );
	}

	CodeComment inline_cmt = NullCode;
	if ( function && function->Type == CT_Function_Fwd )
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// friend <Type>;
		// friend <ReturnType> <Name> ( <Parameters> );

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
		// friend <Type>; <InlineCmt>
		// friend <ReturnType> <Name> ( <Parameters> ); <InlineCmt>
	}

	CodeFriend result = ( CodeFriend )make_code();
	result->Type      = CT_Friend;

	if ( function )
		result->Declaration = cast(Code, function);
	else if ( op )
		result->Declaration = cast(Code, op);
	else
		result->Declaration = cast(Code, type);

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeFn parser_parse_function()
{
	push_scope();

	Specifier specs_found[16] = { Spec_NumSpecifiers };
	s32        NumSpecifiers = 0;

	CodeAttributes attributes = { nullptr };
	CodeSpecifiers specifiers = { nullptr };
	ModuleFlag     mflags     = ModuleFlag_None;

	if ( check(Tok_Module_Export) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <export>

	attributes = parse_attributes();
	// <export> <Attributes>

	while ( left && tok_is_specifier(currtok) )
	{
		Specifier spec = str_to_specifier( tok_to_str(currtok) );

		switch ( spec )
		{
			case Spec_Const:
			case Spec_Consteval:
			case Spec_Constexpr:
			case Spec_External_Linkage:
			case Spec_ForceInline:
			case Spec_Inline:
			case Spec_NeverInline:
			case Spec_Static:
			break;

			default:
				log_failure( "Invalid specifier %S for functon\n%SB", spec_to_str(spec), parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
		}

		if ( spec == Spec_Const )
			continue;

		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat( currtok.Type );
	}

	if ( NumSpecifiers )
	{
		specifiers = def_specifiers( NumSpecifiers, specs_found );
	}
	// <export> <Attributes> <Specifiers>

	CodeTypename ret_type = parser_parse_type(parser_not_from_template, nullptr);
	if ( cast(Code, ret_type) == Code_Invalid )
	{
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}
	// <export> <Attributes> <Specifiers> <ReturnType>

	Token name = parse_identifier(nullptr);
	_ctx->parser.Scope->Name = name.Text;
	if ( ! tok_is_valid(name) )
	{
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}
	// <export> <Attributes> <Specifiers> <ReturnType> <Name>

	CodeFn result = parse_function_after_name( mflags, attributes, specifiers, ret_type, name );
	// <export> <Attributes> <Specifiers> <ReturnType> <Name> ...

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeNS parser_parse_namespace()
{
	push_scope();

	eat( Tok_Decl_Namespace );
	// namespace

	Token name = parse_identifier(nullptr);
	_ctx->parser.Scope->Name = name.Text;
	// namespace <Name>

	CodeBody body = parse_global_nspace( CT_Namespace_Body );
	if ( cast(Code, body) == Code_Invalid )
	{
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}
	// namespace <Name> { <Body> }

	CodeNS
	result       = (CodeNS) make_code();
	result->Type = CT_Namespace;
	result->Name = cache_str( tok_to_str(name) );

	result->Body = body;

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeOperator parser_parse_operator()
{
	push_scope();

	CodeAttributes attributes = { nullptr };
	CodeSpecifiers specifiers = { nullptr };
	ModuleFlag     mflags     = ModuleFlag_None;

	Specifier specs_found[16] = { Spec_NumSpecifiers };
	s32        NumSpecifiers = 0;

	if ( check(Tok_Module_Export) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <export>

	attributes = parse_attributes();
	// <export> <Attributes>

	while ( left && tok_is_specifier(currtok) )
	{
		Specifier spec = str_to_specifier( tok_to_str(currtok) );

		switch ( spec )
		{
			case Spec_Const:
			case Spec_Constexpr:
			case Spec_ForceInline:
			case Spec_Inline:
			case Spec_NeverInline:
			case Spec_Static:
			break;

			default:
				log_failure( "Invalid specifier " "%S" " for operator\n%SB", spec_to_str(spec), parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
		}

		if ( spec == Spec_Const )
			continue;

		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat( currtok.Type );
	}

	if ( NumSpecifiers )
	{
		specifiers = def_specifiers( NumSpecifiers, specs_found );
	}
	// <export> <Attributes> <Specifiers>

	// Parse Return Type
	CodeTypename ret_type = parser_parse_type(parser_not_from_template, nullptr);
	// <export> <Attributes> <Specifiers> <ReturnType>

	CodeOperator result = parse_operator_after_ret_type( mflags, attributes, specifiers, ret_type );
	// <export> <Attributes> <Specifiers> <ReturnType> ...

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeOpCast parser_parse_operator_cast( CodeSpecifiers specifiers )
{
	push_scope();

	// Operator's namespace if not within same class.
	Token name = NullToken;
	if ( check( Tok_Identifier ) )
	{
		name = currtok;
		while ( left && currtok.Type == Tok_Identifier )
		{
			eat( Tok_Identifier );
			// <Specifiers> <Qualifier>

			if ( currtok.Type == Tok_Access_StaticSymbol )
				eat( Tok_Access_StaticSymbol );
				// <Specifiers> <Qualifier> ::
		}
		// <Specifiers> <Qualifier> :: ...

		name.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)name.Text.Ptr;
	}

	eat( Tok_Decl_Operator );
	// <Specifiers> <Qualifier> :: ... operator

	CodeTypename type = parser_parse_type(parser_not_from_template, nullptr);
	// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>

	Str scope_name         = { type->Name.Ptr, type->Name.Len };
	Token scope_name_tok   = { scope_name, Tok_Identifier, 0, 0, TF_Null };
	_ctx->parser.Scope->Name = scope_name_tok.Text;

	eat( Tok_Capture_Start );
	eat( Tok_Capture_End );
	// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>()

	// TODO(Ed) : operator cast can have const, volatile, l-value, r-value noexecept qualifying specifiers.
	if ( check(Tok_Spec_Const))
	{
		if ( specifiers == nullptr )
			specifiers = def_specifier( Spec_Const );

		else
			specifiers_append(specifiers, Spec_Const );

		eat( Tok_Spec_Const );
	}
	// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const>

	Code        body       = NullCode;
	CodeComment inline_cmt = NullCode;

	if ( check( Tok_BraceCurly_Open) )
	{
		eat( Tok_BraceCurly_Open );
		// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const> {

		Token body_str = currtok;

		s32 level = 0;
		while ( left && ( currtok.Type != Tok_BraceCurly_Close || level > 0 ) )
		{
			if ( currtok.Type == Tok_BraceCurly_Open )
				level++;

			else if ( currtok.Type == Tok_BraceCurly_Close )
				level--;

			eat( currtok.Type );
		}
		body_str.Text.Len = ( (sptr)prevtok.Text.Ptr + prevtok.Text.Len ) - (sptr)body_str.Text.Ptr;

		eat( Tok_BraceCurly_Close );
		// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const> { <Body> }

		body = untyped_str( tok_to_str(body_str) );
	}
	else
	{
		Token stmt_end = currtok;
		eat( Tok_Statement_End );
		// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const>;

		if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
			inline_cmt = parse_comment();
			// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const>; <InlineCmt>
	}

	CodeOpCast result = (CodeOpCast) make_code();

	if ( tok_is_valid(name) )
		result->Name = cache_str( tok_to_str(name) );

	if (body)
	{
		result->Type = CT_Operator_Cast;
		result->Body = cast(CodeBody, body);
	}
	else
	{
		result->Type = CT_Operator_Cast_Fwd;
	}

	if ( specifiers )
		result->Specs = specifiers;

	result->ValueType = cast(CodeTypename, type);

	parser_pop(& _ctx->parser);
	return result;
}

internal inline
CodeStruct parser_parse_struct( bool inplace_def )
{
	push_scope();
	CodeStruct result = (CodeStruct) parse_class_struct( Tok_Decl_Struct, inplace_def );
	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeTemplate parser_parse_template()
{
#define UseTemplateCapture true

	push_scope();

	ModuleFlag mflags = ModuleFlag_None;

	if ( check( Tok_Module_Export ) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <export> template

	eat( Tok_Decl_Template );
	// <export> template

	CodeParams params = parse_params( UseTemplateCapture );
	if ( cast(Code, params) == Code_Invalid )
	{
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}
	// <export> template< <Parameters> >

	Code definition = { nullptr };

	while ( left )
	{
		if ( check( Tok_Decl_Class ) )
		{
			definition = cast(Code, parser_parse_class( parser_not_inplace_def));
			// <export> template< <Parameters> > class ...
			break;
		}

		if ( check( Tok_Decl_Struct ) )
		{
			definition = cast(Code, parser_parse_struct( parser_not_inplace_def));
			// <export> template< <Parameters> > struct ...
			break;
		}

		if ( check( Tok_Decl_Union ) )
		{
			definition = cast(Code, parser_parse_union( parser_not_inplace_def));
			// <export> template< <Parameters> > union ...
			break;
		}

		if ( check( Tok_Decl_Using ) )
		{
			definition = cast(Code, parser_parse_using());
			// <export> template< <Parameters> > using ...
			break;
		}

		// Its either a function or a variable
		Token name                = NullToken;

		CodeAttributes attributes = { nullptr };
		CodeSpecifiers specifiers = { nullptr };

		bool expects_function     = false;

		Specifier specs_found[ 16 ] = { Spec_NumSpecifiers };
		s32        NumSpecifiers = 0;

		attributes = parse_attributes();
		// <export> template< <Parameters> > <Attributes>

		// Specifiers Parsing
		{
			while ( left && tok_is_specifier(currtok) )
			{
				Specifier spec = str_to_specifier( tok_to_str(currtok) );

				switch ( spec )
				{
					case Spec_Const :
					case Spec_Constexpr :
					case Spec_Constinit :
					case Spec_External_Linkage :
					case Spec_Global :
					case Spec_Inline :
					case Spec_ForceInline :
					case Spec_Local_Persist :
					case Spec_Mutable :
					case Spec_Static :
					case Spec_Thread_Local :
					case Spec_Volatile :
						break;

					case Spec_Consteval :
						expects_function = true;
						break;

					default :
						log_failure( "Invalid specifier %S for variable or function\n%SB", spec_to_str( spec ), parser_to_strbuilder(_ctx->parser) );
						parser_pop(& _ctx->parser);
						return InvalidCode;
				}

				// Ignore const it will be handled by the type
				if ( spec == Spec_Const )
					break;

				specs_found[ NumSpecifiers ] = spec;
				NumSpecifiers++;
				eat( currtok.Type );
			}

			if ( NumSpecifiers )
			{
				specifiers = def_specifiers( NumSpecifiers, specs_found );
			}
			// <export> template< <Parameters> > <Attributes> <Specifiers>
		}


		bool has_context         = _ctx->parser.Scope && _ctx->parser.Scope->Prev;
		bool is_in_global_nspace = has_context && str_are_equal( _ctx->parser.Scope->Prev->ProcName, txt("parse_global_nspace") );
		// Possible constructor implemented at global file scope.
		if (is_in_global_nspace)
		{
			Code constructor_destructor = parse_global_nspace_constructor_destructor( specifiers );
			if ( constructor_destructor )
			{
				definition = constructor_destructor;
				// <Attributes> <Specifiers> <Name> :: <Name> <Type> () { ... }
				break;
			}
		}

		// Possible user Defined operator casts
		if (is_in_global_nspace)
		{
			bool found_operator_cast_outside_class_implmentation = false;
			s32  idx = _ctx->parser.Tokens.Idx;

			for ( ; idx < array_num(_ctx->parser.Tokens.Arr); idx++ )
			{
				Token tok = _ctx->parser.Tokens.Arr[ idx ];

				if ( tok.Type == Tok_Identifier )
				{
					idx++;
					tok = _ctx->parser.Tokens.Arr[ idx ];
					if ( tok.Type == Tok_Access_StaticSymbol )
						continue;

					break;
				}

				if ( tok.Type == Tok_Decl_Operator )
					found_operator_cast_outside_class_implmentation = true;

				break;
			}

			if ( found_operator_cast_outside_class_implmentation )
			{
				definition = cast(Code, parser_parse_operator_cast( specifiers ));
				// <Attributes> <Specifiers> <Name> :: operator <Type> () { ... }
				break;
			}
		}

		definition = parse_operator_function_or_variable( expects_function, attributes, specifiers );
		// <export> template< <Parameters> > <Attributes> <Specifiers> ...
		break;
	}

	CodeTemplate result = ( CodeTemplate )make_code();
	result->Type        = CT_Template;
	result->Params      = params;
	result->Declaration = definition;
	result->ModuleFlags = mflags;
	// result->Name        = definition->Name;

	parser_pop(& _ctx->parser);
	return result;
#undef UseTemplateCapture
}

/*
	This is a mess, but it works
	Parsing typename is arguably one of the worst aspects of C/C++.
	This is an effort to parse it without a full blown or half-blown compliant parser.

	Recursive function typenames are not supported, if they are used expect it to serailize just fine, but validation with AST::is_equal
	will not be possible if two ASTs share the same definiton but the formatting is slightly different:
	AST_1->Name: (* A    ( int   (*)    (short a,unsigned b,long c) ) )
	AST_2->Name: (* A       ( int(*)(short a, unsigned b, long c) ) )

	The excess whitespace cannot be stripped however, because there is no semantic awareness within the first capture group.
*/
internal
CodeTypename parser_parse_type( bool from_template, bool* typedef_is_function )
{
	push_scope();

	Token context_tok = prevtok;

	Specifier specs_found[ 16 ] = { Spec_NumSpecifiers };
	s32       NumSpecifiers = 0;

	Token name= NullToken;

	ETypenameTag tag = Tag_None;

	// Attributes are assumed to be before the type signature
	CodeAttributes attributes = parse_attributes();
	// <Attributes>

	// Prefix specifiers
	while ( left && tok_is_specifier(currtok) )
	{
		Specifier spec = str_to_specifier( tok_to_str(currtok) );

		if ( spec != Spec_Const )
		{
			log_failure( "Error, invalid specifier used in type definition: %S\n%SB", tok_to_str(currtok), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		specs_found[ NumSpecifiers ] = spec;
		NumSpecifiers++;
		eat( currtok.Type );
	}
	// <Attributes> <Specifiers>

	if ( left == 0 )
	{
		log_failure( "Error, unexpected end of type definition\n%SB", parser_to_strbuilder(_ctx->parser) );
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	if ( from_template && currtok.Type == Tok_Decl_Class )
	{
		// If a value's type is being parsed from a template, class can be used instead of typename.
		name = currtok;
		eat(Tok_Decl_Class);
		// <class>
	}

	// All kinds of nonsense can makeup a type signature, first we check for a in-place definition of a class, enum, struct, or union
	else if ( currtok.Type == Tok_Decl_Class || currtok.Type == Tok_Decl_Enum || currtok.Type == Tok_Decl_Struct
			|| currtok.Type == Tok_Decl_Union )
	{
		switch (currtok.Type) {
			case Tok_Decl_Class  : tag = Tag_Class;  break;
			case Tok_Decl_Enum   : tag = Tag_Enum;   break;
			case Tok_Decl_Struct : tag = Tag_Struct; break;
			case Tok_Decl_Union  : tag = Tag_Union;  break;
			default:
				break;
		}
		eat( currtok.Type );
		// <Attributes> <Specifiers> <class, enum, struct, union>

		name = parse_identifier(nullptr);

		// name.Length = ( ( sptr )currtok.Text + currtok.Length ) - ( sptr )name.Text;
		// eat( Tok_Identifier );
		_ctx->parser.Scope->Name = name.Text;
		// <Attributes> <Specifiers> <class, enum, struct, union> <Name>
	}

// Decltype draft implementaiton
#if 0
else if ( currtok.Type == Tok_DeclType )
{
	// Will have a capture and its own parsing rules, were going to just shove everything in a string (for now).
	name = currtok;
	eat( Tok_DeclType );
	// <Attributes> <Specifiers> decltype

	eat( Tok_Capture_Start );
	while ( left && currtok.Type != Tok_Capture_End )
	{
		if ( currtok.Type == Tok_Capture_Start )
			level++;

		if ( currtok.Type == Tok_Capture_End )
			level--;

		eat( currtok.Type );
	}
	eat( Tok_Capture_End );

	name.Length = ( (sptr)currtok.Text + currtok.Length ) - (sptr)name.Text;
	_ctx->parser.Scope->Name = name;
	// <Attributes> <Specifiers> decltype( <Expression > )
}
#endif

	// Check if native type keywords are used, eat them for the signature.
	// <attributes> <specifiers> <native types ...> ...
	else if ( currtok.Type >= Tok_Type_Unsigned && currtok.Type <= Tok_Type_MS_W64 )
	{
		// TODO(Ed) : Review this... Its necessary for parsing however the algo's path to this is lost...
		name = currtok;
		eat( currtok.Type );

		while ( left && currtok.Type >= Tok_Type_Unsigned && currtok.Type <= Tok_Type_MS_W64 )
		{
			eat( currtok.Type );
		}

		name.Text.Len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )name.Text.Ptr;
		// <Attributes> <Specifiers> <Compound type expression>
	}
	else if ( currtok.Type == Tok_Type_Typename )
	{
		name = currtok;
		eat(Tok_Type_Typename);
		// <typename>

		if ( ! from_template )
		{
			name                = parse_identifier(nullptr);
			_ctx->parser.Scope->Name = name.Text;
			if ( ! tok_is_valid(name) )
			{
				log_failure( "Error, failed to type signature\n%s", parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
			}
		}
	}
	else if ( currtok.Type == Tok_Preprocess_Macro ) {
		// Typename is a macro
		name = currtok;
		eat(Tok_Preprocess_Macro);
	}

	// The usual Identifier type signature that may have namespace qualifiers
	else
	{
		name                = parse_identifier(nullptr);
		_ctx->parser.Scope->Name = name.Text;
		if ( ! tok_is_valid(name) )
		{
			log_failure( "Error, failed to type signature\n%s", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}
		// <Attributes> <Specifiers> <Qualifier ::> <Identifier>
		// <Attributes> <Specifiers> <Identifier>
	}

	// Suffix specifiers for typename.
	while ( left && tok_is_specifier(currtok) )
	{
		Specifier spec = str_to_specifier( tok_to_str(currtok) );

		if ( spec != Spec_Const && spec != Spec_Ptr && spec != Spec_Ref && spec != Spec_RValue )
		{
			log_failure( "Error, invalid specifier used in type definition: %S\n%SB", tok_to_str(currtok), parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		specs_found[ NumSpecifiers ] = spec;
		NumSpecifiers++;
		eat( currtok.Type );
	}

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	if ( NumSpecifiers )
	{
		specifiers    = def_specifiers( NumSpecifiers, specs_found );
		NumSpecifiers = 0;
	}
#endif
	// <Attributes> <Specifiers> <Identifier> <Specifiers>

	// For function type signatures
	CodeTypename return_type = NullCode;
	CodeParams    params      = NullCode;

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	CodeParams params_nested = NullCode;
#endif

	bool   is_function_typename = false;
	Token* last_capture         = nullptr;
	{
		Token* scanner = _ctx->parser.Tokens.Arr + _ctx->parser.Tokens.Idx;

		// An identifier being within a typename's signature only occurs if were parsing a typename for a typedef.
		if ( typedef_is_function && scanner->Type == Tok_Identifier )
		{
			is_function_typename = true;
			++scanner;
		}
		is_function_typename = scanner->Type == Tok_Capture_Start;

		Token* first_capture = scanner;
		if ( is_function_typename )
		{
			// Go to the end of the signature
			while ( scanner->Type != Tok_Statement_End && scanner->Type != Tok_BraceCurly_Open )
				++scanner;

			// Go back to the first capture start found
			while ( scanner->Type != Tok_Capture_Start )
				--scanner;

			last_capture = scanner;
		}

		bool has_context   = _ctx->parser.Scope && _ctx->parser.Scope->Prev;
		bool is_for_opcast = has_context && str_are_equal( _ctx->parser.Scope->Prev->ProcName, txt("parser_parse_operator_cast") );
		if ( is_for_opcast && is_function_typename && last_capture )
		{
			// If we're parsing for an operator cast, having one capture start is not enough
			// we need to make sure that the capture is not for the cast definition.
			is_function_typename = false;

			if ( last_capture == first_capture )
			{
				// The capture start in question is the first capture start, this is not a function typename.
				is_function_typename = false;
			}
		}
	}

	if ( is_function_typename )
	{
		// We're dealing with a function typename.
		// By this point, decltype should have been taken care of for return type, along with any all its specifiers

		// The previous information with exception to attributes will be considered the return type.
		return_type       = ( CodeTypename )make_code();
		return_type->Type = CT_Typename;

		// StrBuilder
		// name_stripped = StrBuilder::make( FallbackAllocator, name );
		// name_stripped.strip_space();
		return_type->Name = cache_str( tok_to_str(name) );

#ifdef GEN_USE_NEW_TYPENAME_PARSING
		if ( specifiers )
		{
			return_type->Specs = specifiers;
			specifiers         = nullptr;
		}

#else
		if ( NumSpecifiers )
			return_type->Specs = def_specifiers( NumSpecifiers, ( Specifier* )specs_found );

		// Reset specifiers, the function itself will have its own suffix specifiers possibly.
		NumSpecifiers = 0;
#endif
		// <Attributes> <ReturnType>
		name = NullToken;

		// The next token can either be a capture for the identifier or it could be the identifier exposed.
		if ( ! check( Tok_Capture_Start ) )
		{
			// Started with an identifier immeidately, which means its of the format: <ReturnType> <identifier> <capture>;
			name = parse_identifier(nullptr);
		}
		// <Attributes> <ReturnType> <Identifier>

		// If the next token is a capture start and is not the last capture, then we're dealing with function typename whoose identifier is within the
		// capture.
		else if ( ( _ctx->parser.Tokens.Arr + _ctx->parser.Tokens.Idx ) != last_capture )
		{
			// WIP : Possible alternative without much pain...
			// If this were to be parsed properly...
			// Eat Capture Start
			// Deal with possible binding specifiers (*, &, &&) and modifiers on those bindings (const, volatile)
			// Parse specifiers for the typename with an optional identifier,
			// we can shove these specific specifiers into a specs, and then leave the suffix ones for a separate member of the AST.
			// Parse immeidate capture which would be with parse_params()
			// Eat Capture End
#ifdef GEN_USE_NEW_TYPENAME_PARSING
			eat( Tok_Capture_Start );
			// <Attributes> <ReturnType> (

			// Binding specifiers
			while ( left && currtok.is_specifier() )
			{
				Specifier spec = to_type( currtok );

				if ( spec != Spec_Ptr && spec != Spec_Ref && spec != Spec_RValue )
				{
					log_failure( "Error, invalid specifier used in type definition: %S\n%SB", toktype_to_str(currtok), to_strbuilder(_ctx->parser) );
					pop(& _ctx->parser);
					return InvalidCode;
				}

				specs_found[ NumSpecifiers ] = spec;
				NumSpecifiers++;
				eat( currtok.Type );
			}

			if ( NumSpecifiers )
			{
				specifiers = def_specifiers( NumSpecifiers, specs_found );
			}
			NumSpecifiers = 0;
			// <Attributes> <ReturnType> ( <Specifiers>

			if ( check( Tok_Identifier ) )
				name = parse_identifier();
			// <Attributes> <ReturnType> ( <Specifiers> <Identifier>

			// Immeidate parameters

			if ( check( Tok_Capture_Start ) )
				params_nested = parse_params();
			// <Attributes> <ReturnType> ( <Specifiers> <Identifier> ( <Parameters> )

			eat( Tok_Capture_End );
			// <Attributes> <ReturnType> ( <Specifiers> <Identifier> ( <Parameters> ) )

#else
			// Starting immediatley with a capture, most likely declaring a typename for a member function pointer.
			// Everything within this capture will just be shoved into the name field including the capture tokens themselves.
			name = currtok;

			eat( Tok_Capture_Start );
			// <Attributes> <ReturnType> (

			s32 level = 0;
			while ( left && ( currtok.Type != Tok_Capture_End || level > 0 ) )
			{
				if ( currtok.Type == Tok_Capture_Start )
					level++;

				if ( currtok.Type == Tok_Capture_End )
					level--;

				eat( currtok.Type );
			}
			eat( Tok_Capture_End );
			// <Attributes> <ReturnType> ( <Expression> )

			name.Text.Len = ( ( sptr )prevtok.Text.Ptr + prevtok.Text.Len ) - ( sptr )name.Text.Ptr;
#endif
		}

		// Were now dealing with the parameters of the function
		params = parse_params(parser_use_parenthesis);
		// <Attributes> <ReturnType> <All Kinds of nonsense> ( <Parameters> )

		// Look for suffix specifiers for the function
		while ( left && tok_is_specifier(currtok) )
		{
			Specifier spec = str_to_specifier( tok_to_str(currtok) );

			if ( spec != Spec_Const
					// TODO : Add support for NoExcept, l-value, volatile, l-value, etc
					// && spec != Spec_NoExcept
					&& spec != Spec_RValue )
			{
				log_failure( "Error, invalid specifier used in type definition: %S\n%S", tok_to_str(currtok), strbuilder_to_str( parser_to_strbuilder(_ctx->parser)) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
			}

			specs_found[ NumSpecifiers ] = spec;
			NumSpecifiers++;
			eat( currtok.Type );
		}

#ifdef GEN_USE_NEW_TYPENAME_PARSING
		if ( NumSpecifiers )
		{
			func_suffix_specs = def_specifiers( NumSpecifiers, specs_found );
			NumSpecifiers     = 0;
		}
#endif
		// <Attributes> <ReturnType> <All Kinds of nonsense> ( <Parameters> ) <Specifiers>
	}
	// <Attributes> <All Kinds of nonsense>

	bool is_param_pack = false;
	if ( check( Tok_Varadic_Argument ) )
	{
		is_param_pack = true;
		eat( Tok_Varadic_Argument );
		// <Attributes> <All kinds of nonsense> ...
	}

	CodeTypename result = ( CodeTypename )make_code();
	result->Type        = CT_Typename;
	// result->Token = _ctx->parser.Scope->Start;

	// Need to wait until were using the new parsing method to do this.
	StrBuilder name_stripped = parser_strip_formatting( tok_to_str(name), parser_strip_formatting_dont_preserve_newlines );

	// name_stripped.strip_space();

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	if ( params_nested )
	{
		name_stripped.append( params_nested->to_strbuilder() );
	}
#endif

	result->Name = cache_str( strbuilder_to_str(name_stripped) );

	if ( attributes )
		result->Attributes = attributes;

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	if ( specifiers )
	{
		result->Specs = specifiers;
	}

	if ( func_suffix_specs )
	{
		result->FuncSuffixSpecs = func_suffix_specs;
	}
#else
	if ( NumSpecifiers )
	{
		CodeSpecifiers specifiers = def_specifiers( NumSpecifiers, ( Specifier* )specs_found );
		result->Specs   = specifiers;
	}
#endif

	if ( is_param_pack )
		result->IsParamPack = true;

	// These following are only populated if its a function typename
	if ( return_type )
	{
		result->ReturnType = return_type;

		if ( typedef_is_function )
			*typedef_is_function = true;
	}

	if ( params )
		result->Params = params;

	result->TypeTag = tag;

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeTypedef parser_parse_typedef()
{
	push_scope();

	bool  is_function = false;
	Token name        = NullToken;
	Code  array_expr  = { nullptr };
	Code  type        = { nullptr };

	ModuleFlag mflags = ModuleFlag_None;

	if ( check(Tok_Module_Export) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <ModuleFlags>

	eat( Tok_Decl_Typedef );
	// <ModuleFlags> typedef

	const bool from_typedef = true;

#if GEN_PARSER_DISABLE_MACRO_TYPEDEF
	if ( false )
#else
	if ( check( Tok_Preprocess_Macro ))
#endif
	{
		type = cast(Code, t_empty);
		name = currtok;
		_ctx->parser.Scope->Name = name.Text;
		eat( Tok_Preprocess_Macro );
		// <ModuleFalgs> typedef <Preprocessed_Macro>

		if ( currtok.Type == Tok_Identifier )
		{
			type = untyped_str(name.Text);
			name = currtok;
			eat(Tok_Identifier);
		}
		// <ModuleFalgs> typedef <Preprocessed_Macro> <Identifier>
	}
	else
	{
		bool is_complicated =
				currtok.Type == Tok_Decl_Enum
			||	currtok.Type == Tok_Decl_Class
			||	currtok.Type == Tok_Decl_Struct
			||	currtok.Type == Tok_Decl_Union;


		// This code is highly correlated with parse_complicated_definition
		if ( is_complicated )
		{
			TokArray tokens = _ctx->parser.Tokens;
			TokType  which  = currtok.Type;

			s32 idx = tokens.Idx;
			s32 level = 0;
			for ( ; idx < array_num(tokens.Arr); idx ++ )
			{
				if ( tokens.Arr[idx].Type == Tok_BraceCurly_Open )
					level++;

				if ( tokens.Arr[idx].Type == Tok_BraceCurly_Close )
					level--;

				if ( level == 0 && tokens.Arr[idx].Type == Tok_Statement_End )
					break;
			}

			Token pre_foward_tok = currtok;
			if ( (idx - 3 ) == tokens.Idx )
			{
				// Its a forward declaration only
				type = parse_forward_or_definition( which, from_typedef );
				// <ModuleFalgs> typedef <UnderlyingType: Forward Decl>
			}
			else
			{
				Token tok = tokens.Arr[ idx - 1 ];
				if ( tok.Type == Tok_Identifier )
				{
					log_fmt("Found id\n");
					tok = tokens.Arr[ idx - 2 ];

					bool is_indirection = tok.Type == Tok_Ampersand
					||                    tok.Type == Tok_Star;

					bool ok_to_parse = false;

					Token temp_3 = tokens.Arr[ idx - 3 ];

					if ( tok.Type == Tok_BraceCurly_Close )
					{
						// Its an inplace definition
						// typedef <which> <type_identifier> { ... } <identifier>;
						ok_to_parse = true;
					}
					else if ( tok.Type == Tok_Identifier && tokens.Arr[ idx - 3 ].Type == which )
					{
						// Its a variable with type ID using which namespace.
						// typedef <which> <type_identifier> <identifier>;
						ok_to_parse = true;
					}
					else if ( is_indirection )
					{
						// Its a indirection type with type ID using struct namespace.
						// typedef <which> <type_identifier>* <identifier>;
						ok_to_parse = true;
					}

					if ( ! ok_to_parse )
					{
						log_failure( "Unsupported or bad member definition after struct declaration\n%SB", parser_to_strbuilder(_ctx->parser) );
						parser_pop(& _ctx->parser);
						return InvalidCode;
					}

					// TODO(Ed) : I'm not sure if I have to use parser_parse_type here, I'd rather not as that would complicate parser_parse_type.
					// type = parser_parse_type();
					type = parse_forward_or_definition( which, from_typedef );
					// <ModuleFalgs> typedef <UnderlyingType>
				}
				else if ( tok.Type == Tok_BraceCurly_Close )
				{
					// Its a definition
					// <which> { ... };
					type = parse_forward_or_definition( currtok.Type, from_typedef );
					// <ModuleFalgs> typedef <UnderlyingType>
				}
				else if ( tok.Type == Tok_BraceSquare_Close)
				{
					// Its an array definition
					// <which> <type_identifier> <identifier> [ ... ];
					type = cast(Code, parser_parse_type(parser_not_from_template, nullptr));
					// <ModuleFalgs> typedef <UnderlyingType>
				}
				else
				{
					log_failure( "Unsupported or bad member definition after struct declaration\n%SB", parser_to_strbuilder(_ctx->parser) );
					parser_pop(& _ctx->parser);
					return InvalidCode;
				}
			}
		}
		else
		{
			bool from_template = false;
			type = cast(Code, parser_parse_type( from_template, &is_function ));
			// <ModuleFalgs> typedef <UnderlyingType>
		}

		if ( check( Tok_Identifier ) )
		{
			name = currtok;
			eat( Tok_Identifier );
			// <ModuleFalgs> typedef <UnderlyingType> <Name>
		}
		else if ( ! is_function )
		{
			log_failure( "Error, expected identifier for typedef\n%SB", parser_to_strbuilder(_ctx->parser) );
			parser_pop(& _ctx->parser);
			return InvalidCode;
		}

		array_expr = parse_array_decl();
		// <UnderlyingType> + <ArrayExpr>
	}

	Token stmt_end = currtok;
	eat( Tok_Statement_End );
	// <ModuleFalgs> typedef <UnderlyingType> <Name>;

	CodeComment inline_cmt = NullCode;
	if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
		inline_cmt = parse_comment();
		// <ModuleFalgs> typedef <UnderlyingType> <Name> <ArrayExpr>; <InlineCmt>

	CodeTypedef
	result              = (CodeTypedef) make_code();
	result->Type        = CT_Typedef;
	result->ModuleFlags = mflags;

	if ( is_function )
	{
		result->Name       = type->Name;
		result->IsFunction = true;
	}
	else
	{
		result->Name       = cache_str( tok_to_str(name) );
		result->IsFunction = false;
	}

	if ( type )
	{
		result->UnderlyingType         = type;
		result->UnderlyingType->Parent = cast(Code, result);
	}
	// Type needs to be aware of its parent so that it can be serialized properly.

	if ( type->Type == CT_Typename && array_expr && array_expr->Type != CT_Invalid )
		cast(CodeTypename, type)->ArrExpr = array_expr;

	if ( inline_cmt )
		result->InlineCmt = inline_cmt;

	parser_pop(& _ctx->parser);
	return result;
}

internal neverinline
CodeUnion parser_parse_union( bool inplace_def )
{
	push_scope();

	ModuleFlag mflags = ModuleFlag_None;

	if ( check(Tok_Module_Export) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <ModuleFlags>

	eat( Tok_Decl_Union );
	// <ModuleFlags> union

	CodeAttributes attributes = parse_attributes();
	// <ModuleFlags> union <Attributes>

	Str name = { nullptr, 0 };
	if ( check( Tok_Identifier ) )
{
		name = tok_to_str(currtok);
		_ctx->parser.Scope->Name = currtok.Text;
		eat( Tok_Identifier );
	}
	// <ModuleFlags> union <Attributes> <Name>

	CodeBody body = { nullptr };

	if ( ! inplace_def || ! check(Tok_Identifier) )
	{
		eat( Tok_BraceCurly_Open );
		// <ModuleFlags> union <Attributes> <Name> {

		body = cast(CodeBody, make_code());
		body->Type = CT_Union_Body;

		while ( ! check_noskip( Tok_BraceCurly_Close ) )
		{
			if ( currtok_noskip.Type == Tok_Preprocess_Hash )
				eat( Tok_Preprocess_Hash );

			Code member = { nullptr };
			switch ( currtok_noskip.Type )
			{
				case Tok_NewLine:
					member = fmt_newline;
					eat( Tok_NewLine );
				break;

				case Tok_Comment:
					member = cast(Code, parse_comment());
				break;

				// TODO(Ed) : Unions can have constructors and destructors

				case Tok_Decl_Class:
					member = parse_complicated_definition( Tok_Decl_Class );
				break;

				case Tok_Decl_Enum:
					member = parse_complicated_definition( Tok_Decl_Enum );
				break;

				case Tok_Decl_Struct:
					member = parse_complicated_definition( Tok_Decl_Struct );
				break;

				case Tok_Decl_Union:
					member = parse_complicated_definition( Tok_Decl_Union );
				break;

				case Tok_Preprocess_Define:
					member = cast(Code, parse_define());
				break;

				case Tok_Preprocess_If:
				case Tok_Preprocess_IfDef:
				case Tok_Preprocess_IfNotDef:
				case Tok_Preprocess_ElIf:
					member = cast(Code, parse_preprocess_cond());
				break;

				case Tok_Preprocess_Else:
					member = cast(Code, preprocess_else);
					eat( Tok_Preprocess_Else );
				break;

				case Tok_Preprocess_EndIf:
					member = cast(Code, preprocess_endif);
					eat( Tok_Preprocess_EndIf );
				break;

				case Tok_Preprocess_Macro:
					if ( nexttok.Type == Tok_Identifier ) {
						// Its a variable with a macro typename
						member = cast(Code, parser_parse_variable());
						break;
					}

					member = parse_simple_preprocess( Tok_Preprocess_Macro, parser_consume_braces );
				break;

				case Tok_Preprocess_Pragma:
					member = cast(Code, parse_pragma());
				break;

				case Tok_Preprocess_Unsupported:
					member = parse_simple_preprocess( Tok_Preprocess_Unsupported, parser_consume_braces );
				break;

				default:
					member = cast(Code, parser_parse_variable());
				break;
			}

			if ( member )
				body_append(body, member );
		}
		// <ModuleFlags> union <Attributes> <Name> { <Body>

		eat( Tok_BraceCurly_Close );
		// <ModuleFlags> union <Attributes> <Name> { <Body> }
	}

	if ( ! inplace_def )
		eat( Tok_Statement_End );
		// <ModuleFlags> union <Attributes> <Name> { <Body> };

	CodeUnion
	result = (CodeUnion) make_code();
	result->Type        = body ? CT_Union : CT_Union_Fwd;
	result->ModuleFlags = mflags;

	if ( name.Len )
		result->Name = cache_str( name );

	result->Body       = body;
	result->Attributes = attributes;

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeUsing parser_parse_using()
{
	push_scope();

	Specifier specs_found[16] = { Spec_Invalid };
	s32        NumSpecifiers = 0;

	Token        name       = NullToken;
	Code         array_expr = { nullptr };
	CodeTypename type       = { nullptr };

	bool is_namespace = false;

	ModuleFlag     mflags     = ModuleFlag_None;
	CodeAttributes attributes = { nullptr };

	if ( check(Tok_Module_Export) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <ModuleFlags>

	eat( Tok_Decl_Using );
	// <ModuleFlags> using

	if ( currtok.Type == Tok_Decl_Namespace )
	{
		is_namespace = true;
		eat( Tok_Decl_Namespace );
		// <ModuleFlags> using namespace
	}

	name = currtok;
	_ctx->parser.Scope->Name = name.Text;
	eat( Tok_Identifier );
	// <ModuleFlags> using <namespace> <Name>

	if ( ! is_namespace )
	{
		if ( bitfield_is_equal( u32, currtok.Flags, TF_Assign ) )
		{
			attributes = parse_attributes();
			// <ModuleFlags> using <Name> <Attributes>

			eat( Tok_Operator );
			// <ModuleFlags> using <Name> <Attributes> =

			type = parser_parse_type(parser_not_from_template, nullptr);
			// <ModuleFlags> using <Name> <Attributes> = <UnderlyingType>

			array_expr = parse_array_decl();
			// <UnderlyingType> + <ArrExpr>
		}
	}

	Token stmt_end = currtok;
	eat( Tok_Statement_End );
	// <ModuleFlags> using <namespace> <Attributes> <Name> = <UnderlyingType>;

	CodeComment inline_cmt = NullCode;
	if ( currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line )
	{
		inline_cmt = parse_comment();
	}
	// <ModuleFlags> using <namespace> <Attributes> <Name> = <UnderlyingType>; <InlineCmt>

	CodeUsing
	result              = (CodeUsing) make_code();
	result->Name        = cache_str( tok_to_str(name) );
	result->ModuleFlags = mflags;

	if ( is_namespace)
	{
		result->Type = CT_Using_Namespace;
	}
	else
	{
		result->Type = CT_Using;

		if ( type )
			result->UnderlyingType = type;

		if ( array_expr )
			type->ArrExpr = array_expr;

		if ( attributes )
			result->Attributes = attributes;

		if ( inline_cmt )
			result->InlineCmt = inline_cmt;
	}

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeVar parser_parse_variable()
{
	push_scope();

	Specifier specs_found[16] = { Spec_NumSpecifiers };
	s32       NumSpecifiers = 0;

	ModuleFlag	   mflags     = ModuleFlag_None;
	CodeAttributes attributes = { nullptr };
	CodeSpecifiers specifiers = { nullptr };

	if ( check(Tok_Module_Export) )
	{
		mflags = ModuleFlag_Export;
		eat( Tok_Module_Export );
	}
	// <ModuleFlags>

	attributes = parse_attributes();
	// <ModuleFlags> <Attributes>

	while ( left && tok_is_specifier(currtok) )
	{
		Specifier spec = str_to_specifier( tok_to_str(currtok) );
		switch  ( spec )
		{
			case Spec_Const:
			case Spec_Constexpr:
			case Spec_Constinit:
			case Spec_External_Linkage:
			case Spec_Global:
			case Spec_Inline:
			case Spec_Local_Persist:
			case Spec_Mutable:
			case Spec_Static:
			case Spec_Thread_Local:
			case Spec_Volatile:
			break;

			default:
				log_failure( "Invalid specifier %S for variable\n%S", spec_to_str( spec ), strbuilder_to_str( parser_to_strbuilder(_ctx->parser)) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
		}

		// Ignore const specifiers, they're handled by the type
		if ( spec == Spec_Const )
			break;

		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat( currtok.Type );
	}

	if ( NumSpecifiers )
	{
		specifiers = def_specifiers( NumSpecifiers, specs_found );
	}
	// <ModuleFlags> <Attributes> <Specifiers>

	CodeTypename type = parser_parse_type(parser_not_from_template, nullptr);
	// <ModuleFlags> <Attributes> <Specifiers> <ValueType>

	if ( cast(Code, type) == Code_Invalid )
		return InvalidCode;

	_ctx->parser.Scope->Name = parse_identifier(nullptr).Text;
	// <ModuleFlags> <Attributes> <Specifiers> <ValueType> <Name>

	CodeVar result = parse_variable_after_name( mflags, attributes, specifiers, type, _ctx->parser.Scope->Name );
	// Regular  : <ModuleFlags> <Attributes> <Specifiers> <ValueType> <Name>                  = <Value>; <InlineCmt>
	// Bitfield : <ModuleFlags> <Attributes> <Specifiers> <ValueType> <Name> : <BitfieldSize> = <Value>; <InlineCmt>

	parser_pop(& _ctx->parser);
	return result;
}

internal
CodeTypename parser_parse_type_alt( bool from_template, bool* typedef_is_functon )
{
	return InvalidCode;
}

#ifdef CHECK_WAS_DEFINED
#pragma pop_macro("check")
#endif

// Publically Exposed Interface

CodeClass parse_class( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	push_scope();
	CodeClass result = (CodeClass) parse_class_struct( Tok_Decl_Class, parser_not_inplace_def );
	parser_pop(& _ctx->parser);
	return result;
}

CodeConstructor parse_constructor( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	// TODO(Ed): Constructors can have prefix attributes

	CodeSpecifiers specifiers = NullCode;
	Specifier      specs_found[ 16 ] = { Spec_NumSpecifiers };
	s32            NumSpecifiers = 0;

	while ( left && tok_is_specifier(currtok) )
	{
		Specifier spec = str_to_specifier( tok_to_str(currtok) );

		b32 ignore_spec = false;

		switch ( spec )
		{
			case Spec_Constexpr :
			case Spec_Explicit:
			case Spec_Inline :
			case Spec_ForceInline :
			case Spec_NeverInline :
				break;

			case Spec_Const :
				ignore_spec = true;
				break;

			default :
				log_failure( "Invalid specifier %s for variable\n%S", spec_to_str( spec ), parser_to_strbuilder(_ctx->parser) );
				parser_pop(& _ctx->parser);
				return InvalidCode;
		}

		// Every specifier after would be considered part of the type type signature
		if (ignore_spec)
			break;

		specs_found[ NumSpecifiers ] = spec;
		NumSpecifiers++;
		eat( currtok.Type );
	}

	if ( NumSpecifiers )
	{
		specifiers = def_specifiers( NumSpecifiers, specs_found );
		// <specifiers> ...
	}

	_ctx->parser.Tokens         = toks;
	CodeConstructor result = parser_parse_constructor( specifiers );
	return result;
}

CodeDestructor parse_destructor( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	// TODO(Ed): Destructors can have prefix attributes
	// TODO(Ed): Destructors can have virtual

	_ctx->parser.Tokens        = toks;
	CodeDestructor result = parser_parse_destructor(NullCode);
	return result;
}

CodeEnum parse_enum( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
	{
		parser_pop(& _ctx->parser);
		return InvalidCode;
	}

	_ctx->parser.Tokens = toks;
	return parser_parse_enum( parser_not_inplace_def);
}

CodeBody parse_export_body( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_export_body();
}

CodeExtern parse_extern_link( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_extern_link();
}

CodeFriend parse_friend( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_friend();
}

CodeFn parse_function( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return (CodeFn) parser_parse_function();
}

CodeBody parse_global_body( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	push_scope();
	CodeBody result = parse_global_nspace( CT_Global_Body );
	parser_pop(& _ctx->parser);
	return result;
}

CodeNS parse_namespace( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_namespace();
}

CodeOperator parse_operator( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return (CodeOperator) parser_parse_operator();
}

CodeOpCast parse_operator_cast( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_operator_cast(NullCode);
}

CodeStruct parse_struct( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	push_scope();
	CodeStruct result = (CodeStruct) parse_class_struct( Tok_Decl_Struct, parser_not_inplace_def );
	parser_pop(& _ctx->parser);
	return result;
}

CodeTemplate parse_template( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_template();
}

CodeTypename parse_type( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_type( parser_not_from_template, nullptr);
}

CodeTypedef parse_typedef( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_typedef();
}

CodeUnion parse_union( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_union( parser_not_inplace_def);
}

CodeUsing parse_using( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_using();
}

CodeVar parse_variable( Str def )
{
	check_parse_args( def );

	TokArray toks = lex( def );
	if ( toks.Arr == nullptr )
		return InvalidCode;

	_ctx->parser.Tokens = toks;
	return parser_parse_variable();
}

// Undef helper macros
#undef check_parse_args
#undef currtok_noskip
#undef currtok
#undef peektok
#undef prevtok
#undef nexttok
#undef nexttok_noskip
#undef eat
#undef left
#undef check
#undef push_scope
#undef def_assign

// Here for C Variant
#undef lex_dont_skip_formatting
#undef lex_skip_formatting

#undef parser_inplace_def
#undef parser_not_inplace_def
#undef parser_dont_consume_braces
#undef parser_consume_braces
#undef parser_not_from_template
#undef parser_use_parenthesis
#undef parser_strip_formatting_dont_preserve_newlines

#pragma endregion Parsing

#pragma region Untyped

ssize token_fmt_va( char* buf, usize buf_size, s32 num_tokens, va_list va )
{
	char const* buf_begin = buf;
	ssize       remaining = buf_size;

	local_persist StringTable tok_map;
	do_once() {
		tok_map = hashtable_init(Str, _ctx->Allocator_DyanmicContainers );
	}
	// Populate token pairs
	{
		s32 left = num_tokens - 1;

		while ( left-- )
		{
			char const* token = va_arg( va, char const* );
			Str         value = va_arg( va, Str );

			u32 key = crc32( token, c_str_len(token) );
			hashtable_set( tok_map, key, value );
		}
	}

	char const* fmt     = va_arg( va, char const* );
	char        current = *fmt;

	while ( current )
	{
		ssize len = 0;

		while ( current && current != '<' && remaining )
		{
			* buf = * fmt;
			buf++;
			fmt++;
			remaining--;

			current = * fmt;
		}

		if ( current == '<' )
		{
			char const* scanner = fmt + 1;

			s32 tok_len = 0;

			while ( *scanner != '>' )
			{
				tok_len++;
				scanner++;
			}

			char const* token = fmt + 1;

			u32       key   = crc32( token, tok_len );
			Str*     value = hashtable_get(tok_map, key );

			if ( value )
			{
				ssize          left = value->Len;
				char const* str  = value->Ptr;

				while ( left-- )
				{
					* buf = * str;
					buf++;
					str++;
					remaining--;
				}

				scanner++;
				fmt     = scanner;
				current = * fmt;
				continue;
			}

			* buf = * fmt;
			buf++;
			fmt++;
			remaining--;

			current = * fmt;
		}
	}
	hashtable_clear(tok_map);
	ssize result = buf_size - remaining;
	return result;
}

Code untyped_str( Str content )
{
	if ( content.Len == 0 )
	{
		log_failure( "untyped_str: empty string" );
		return InvalidCode;
	}

	Code
	result          = make_code();
	result->Name    = cache_str( content );
	result->Type    = CT_Untyped;
	result->Content = result->Name;

	if ( result->Name.Len == 0 )
	{
		log_failure( "untyped_str: could not cache string" );
		return InvalidCode;
	}

	return result;
}

Code untyped_fmt( char const* fmt, ...)
{
	if ( fmt == nullptr )
	{
		log_failure( "untyped_fmt: null format string" );
		return InvalidCode;
	}

	local_persist thread_local
	char buf[GEN_PRINTF_MAXLEN] = { 0 };

	va_list va;
	va_start(va, fmt);
	ssize length = c_str_fmt_va(buf, GEN_PRINTF_MAXLEN, fmt, va);
	va_end(va);
    Str content = { buf, length };

	Code
	result          = make_code();
	result->Type    = CT_Untyped;
	result->Content = cache_str( content );

	if ( result->Name.Len == 0 )
	{
		log_failure( "untyped_fmt: could not cache string" );
		return InvalidCode;
	}

	return result;
}

Code untyped_token_fmt( s32 num_tokens, char const* fmt, ... )
{
	if ( num_tokens == 0 )
	{
		log_failure( "untyped_token_fmt: zero tokens" );
		return InvalidCode;
	}

	local_persist thread_local
	char buf[GEN_PRINTF_MAXLEN] = { 0 };

	va_list va;
	va_start(va, fmt);
	ssize length = token_fmt_va(buf, GEN_PRINTF_MAXLEN, num_tokens, va);
	va_end(va);

	Str buf_str = { buf, length };

	Code
	result          = make_code();
	result->Type    = CT_Untyped;
	result->Content = cache_str( buf_str );

	if ( result->Name.Len == 0 )
	{
		log_failure( "untyped_fmt: could not cache string" );
		return InvalidCode;
	}

	return result;
}

#pragma endregion Untyped

#pragma endregion Interface

#pragma region Builder

Builder builder_open( char const* path )
{
	Builder result;

	FileError error = file_open_mode( & result.File, EFileMode_WRITE, path );
	if ( error != EFileError_NONE )
	{
		log_failure( "gen::File::open - Could not open file: %s", path);
		return result;
	}

	result.Buffer = strbuilder_make_reserve( _ctx->Allocator_Temp, _ctx->InitSize_BuilderBuffer );

	// log_fmt("$Builder - Opened file: %s\n", result.File.filename );
	return result;
}

void builder_pad_lines( Builder* builder, s32 num )
{
	strbuilder_append_str( & builder->Buffer, txt("\n") );
}

void builder_print( Builder* builder, Code code )
{
	StrBuilder   str = code_to_strbuilder(code);
	// const ssize len = str.length();
	// log_fmt( "%s - print: %.*s\n", File.filename, len > 80 ? 80 : len, str.Data );
	strbuilder_append_string( & builder->Buffer, str );
}

void builder_print_fmt_va( Builder* builder, char const* fmt, va_list va )
{
	ssize   res;
	char buf[ GEN_PRINTF_MAXLEN ] = { 0 };

	res = c_str_fmt_va( buf, count_of( buf ) - 1, fmt, va ) - 1;

	strbuilder_append_c_str_len( (StrBuilder*) & (builder->Buffer), (char const*)buf, res);
}

void builder_write(Builder* builder)
{
	b32 result = file_write( & builder->File, builder->Buffer, strbuilder_length(builder->Buffer) );

	if ( result == false )
		log_failure("gen::File::write - Failed to write to file: %s\n", file_name( & builder->File ) );

	log_fmt( "Generated: %s\n", builder->File.filename );
	file_close( & builder->File );
	strbuilder_free(& builder->Buffer);
}

#pragma endregion Builder

#pragma region Scanner

Code scan_file( char const* path )
{
	FileInfo file;

	FileError error = file_open_mode( & file, EFileMode_READ, path );
	if ( error != EFileError_NONE )
	{
		GEN_FATAL( "scan_file: Could not open: %s", path );
	}

	ssize fsize = file_size( & file );
	if ( fsize <= 0 )
	{
		GEN_FATAL("scan_file: %s is empty", path );
	}

	StrBuilder str = strbuilder_make_reserve( _ctx->Allocator_Temp, fsize );
		file_read( & file, str, fsize );
		strbuilder_get_header(str)->Length = fsize;

	// Skip GEN_INTELLISENSE_DIRECTIVES preprocessor blocks
	// Its designed so that the directive should be the first thing in the file.
	// Anything that comes before it will also be omitted.
	{
	#define current (*scanner)
	#define matched    0
	#define move_fwd() do { ++ scanner; -- left; } while (0)
		const Str directive_start = txt( "ifdef" );
		const Str directive_end   = txt( "endif" );
		const Str def_intellisense = txt("GEN_INTELLISENSE_DIRECTIVES" );

		bool        found_directive = false;
		char const* scanner         = (char const*)str;
		s32         left            = fsize;
		while ( left )
		{
			// Processing directive.
			if ( current == '#' )
			{
				move_fwd();
				while ( left && char_is_space( current ) )
					move_fwd();

				if ( ! found_directive )
				{
					if ( left && c_str_compare_len( scanner, directive_start.Ptr, directive_start.Len ) == matched )
					{
						scanner += directive_start.Len;
						left    -= directive_start.Len;

						while ( left && char_is_space( current ) )
							move_fwd();

						if ( left && c_str_compare_len( scanner, def_intellisense.Ptr, def_intellisense.Len ) == matched )
						{
							scanner += def_intellisense.Len;
							left    -= def_intellisense.Len;

							found_directive = true;
						}
					}

					// Skip to end of line
					while ( left && current != '\r' && current != '\n' )
						move_fwd();
					move_fwd();

					if ( left && current == '\n' )
						move_fwd();

					continue;
				}

				if ( left && c_str_compare_len( scanner, directive_end.Ptr, directive_end.Len ) == matched )
				{
					scanner += directive_end.Len;
					left    -= directive_end.Len;

					// Skip to end of line
					while ( left && current != '\r' && current != '\n' )
						move_fwd();
					move_fwd();

					if ( left && current == '\n' )
						move_fwd();

					// sptr skip_size = fsize - left;
					if ( (scanner + 2) >= ( (char const*) str + fsize ) )
					{
						mem_move( str, scanner, left );
						strbuilder_get_header(str)->Length = left;
						break;
					}

					mem_move( str, scanner, left );
					strbuilder_get_header(str)->Length = left;

					break;
				}
			}

			move_fwd();
		}
	#undef move_fwd
	#undef matched
	#undef current
	}

	file_close( & file );
	return untyped_str( strbuilder_to_str(str) );
}

CodeBody parse_file( const char* path ) {
	FileContents file    = file_read_contents( _ctx->Allocator_Temp, true, path );
	Str          content = { (char const*)file.data, file.size };
	CodeBody     code    = parse_global_body( content );
	log_fmt("\nParsed: %s\n", path);
	return code;
}

CSV_Column parse_csv_one_column(AllocatorInfo allocator, char const* path) {
	FileContents content   = file_read_contents( allocator, file_zero_terminate, path );
	Arena        csv_arena = arena_init_from_memory(content.data, content.size);

	CSV_Column result;
	csv_parse( & result.ADT, rcast(char*, content.data), allocator, false );
	result.Content = result.ADT.nodes[0].nodes;
	return result;
}

CSV_Columns2 parse_csv_two_columns(AllocatorInfo allocator, char const* path) {
	FileContents content   = file_read_contents( allocator, file_zero_terminate, path );
	Arena        csv_arena = arena_init_from_memory(content.data, content.size);

	CSV_Columns2 result;
	csv_parse( & result.ADT, rcast(char*, content.data), allocator, false );
	result.Col_1 = result.ADT.nodes[0].nodes;
	result.Col_2 = result.ADT.nodes[1].nodes;
	return result;
}

#pragma endregion Scanner

GEN_NS_END

#endif
#pragma endregion GENCPP IMPLEMENTATION GUARD

#ifdef __clang__
#	pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#	pragma GCC diagnostic pop
#endif
