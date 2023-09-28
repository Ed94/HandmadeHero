#pragma once

// Keywords

#define global        static    // Global variables
#define internal      static    // Internal linkage
#define local_persist static    // Local Persisting variables

#define api_c extern "C"

// Casting

#define ccast( Type, Value ) ( const_cast< Type >( (Value) ) )
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

// TODO(Ed) : Add this sauce later
#if 0
#define congrats( message )
#define ensure( condition, expression )
#define fatal( message )
#endif

// Just experimenting with a way to check for global variables being accessed from the wrong place.
// (Could also be done with gencpp technically)
#if 0
enum class EGlobalVarsAllowFuncs
{
	ProcessPendingWindowMessages,
	Num,
	Invalid,
};
EGlobalVarsAllowFuncs to_type( char const* proc_name )
{
	char const* global_vars_allowed_funcs[] {
		"process_pending_window_messages"
	};

	if ( proc_name )
	{
		for ( u32 idx = 0; idx < array_count( global_vars_allowed_funcs ); ++idx )
		{
			if ( strcmp( proc_name, global_vars_allowed_funcs[idx] ) == 0 )
			{
				return scast( EGlobalVarsAllowFuncs, idx );
			}
		}
	}

	return EGlobalVarsAllowFuncs::Invalid;
}

#if Build_Development
#	define checked_global_getter( global_var, procedure ) \
	( ensure( to_type(procedure) != EGlobalVarsAllowFuncs::Invalid), global_var )
#else
#	define checked_global_getter( global_var, procedure ) global_var
#endif

#endif
