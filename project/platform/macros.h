#pragma once

// Keywords

#define global        static    // Global variables
#define internal      static    // Internal linkage
#define local_persist static    // Local Persisting variables

#define api_c extern "C"

// Casting

#define ccast( Type, Value ) ( * const_cast< Type* >( & (Value) ) )
#define pcast( Type, Value ) ( * reinterpret_cast< Type* >( & ( Value ) ) )
#define rcast( Type, Value ) reinterpret_cast< Type >( Value )
#define scast( Type, Value ) static_cast< Type >( Value )

#define do_once()          \
	do                     \
	{                      \
		local_persist      \
		bool Done = false; \
		if ( Done )        \
			return;        \
		Done = true;       \
	}                      \
	while(0)

#define do_once_start      \
	do                     \
	{                      \
		local_persist      \
		bool Done = false; \
		if ( Done )        \
			break;         \
		Done = true;

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
		if ( !( expression ) )   \
		{                        \
			*( int* )0 = 0;      \
		}
	// platform::assertion_failure( __FILE__, __LINE__, #expression );
#else
#	define assert( expression )
#endif

// TODO(Ed) : Add this sauce later
#if 0
#define congrats( message )
#define ensure( condition, expression )
#define fatal( message )
#endif
