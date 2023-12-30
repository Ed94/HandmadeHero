#pragma once

// Keywords

#define global        static    // Global variables
#define internal      static    // Internal linkage
#define local_persist static    // Local Persisting variables

#define api_c extern "C"

// Casting

#define ccast( type, value ) ( const_cast< type >( (value) ) )
#define pcast( type, value ) ( * reinterpret_cast< type* >( & ( value ) ) )
#define rcast( type, value ) reinterpret_cast< type >( value )
#define scast( type, value ) static_cast< type >( value )

#define do_once() for ( local_persist b32 once = true; once; once = false )

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

#define array_count( array ) ( sizeof( array ) / sizeof( ( array )[0] ) )

// TODO(Ed) : Move to memory header eventually
#define kilobytes( x ) ( ( x ) * ( s64 )( 1024 ) )
#define megabytes( x ) ( kilobytes( x ) * ( s64 )( 1024 ) )
#define gigabytes( x ) ( megabytes( x ) * ( s64 )( 1024 ) )
#define terabytes( x ) ( gigabytes( x ) * ( s64 )( 1024 ) )

// TODO(Ed) : Move to debug header eventually

#if Build_Development
#	define assert( expression ) \
		if ( !( expression ) )  \
		{                       \
			__debugbreak();     \
		}
	// platform::assertion_failure( __FILE__, __LINE__, #expression );
#else
#	define assert( expression )
#endif

#ifdef COMPILER_CLANG
#	define compiler_decorated_func_name __PRETTY_NAME__
#elif defined(COMPILER_MSVC)
#	define compiler_decorated_func_name __FUNCDNAME__
#endif

#if Build_Development
#	define congrats( message )          platform::impl_congrats( message )
#	define ensure( condition, message ) platform::impl_ensure( condition, message )
#	define fatal( message )             platform::impl_fatal( message )
#else
#	define congrats( message )
#	define ensure( condition, message ) true
#	define fatal( message )
#endif
