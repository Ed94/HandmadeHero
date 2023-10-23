#pragma once

#if INTELLISENSE_DIRECTIVES
#include <math.h>
#include "grime.hpp"
#endif

// TODO(Ed) : Convert all of these to platform-efficient versions

inline
f32 abs( f32 value )
{
	return fabsf(value);
}

inline
f32 sqrt( f32 value )
{
	return sqrtf(value);
}

inline
s32 sqrt(s32 value) {
    if (value < 0) {
        return -1;
    }

    if (value == 0 || value == 1) {
        return value;
    }

    // Initial guess for the square root
    s32 current_estimate    = value;
    s32 reciprocal_estimate = 1;
    // Second estimate based on the reciprocal of our current guess

    // Tolerance level for convergence
    const s32 tolerance = 1;

    while (current_estimate - reciprocal_estimate > tolerance) {
        current_estimate    = (current_estimate + reciprocal_estimate) / 2;
        reciprocal_estimate = value / current_estimate;
    }

    return current_estimate;
}

inline
s32 floor( f32 value )
{
	s32 result = scast(s32, floorf( value ));
	return result;
}

inline
s32 round( f32 value )
{
	s32 result = scast(s32, roundf( value ));
	return result;
}

inline
s32 truncate( f32 value )
{
	s32 result = scast(s32, value);
	return result;
}

inline
f32 sine( f32 angle )
{
	f32 result = sinf( angle );
	return result;
}

inline
f32 cosine( f32 angle )
{
	f32 result = cosf( angle );
	return result;
}

inline
f32 arc_tangent( f32 Y, f32 X )
{
	f32 result = atan2f( Y, X );
	return result;
}

inline
b32 bitscan_forward( u32* index, u32 value )
{
	b32 found = false;

	// TODO(Ed) : This file can technically be generated with a metaprogram or via the preprocessor..
	// We can keep The definitions for each of these based on the platform and
	// Choose which one gets added to te project that way (keeps the runtime definition clean)
#if Compiler_MSVC
	found = _BitScanForward( rcast(unsigned long*, index), value );
#elif Compiler_Clang
	*index = __builtin_ctz( value );
	found  = index;
#else
	for ( u32 test = 0; test < 32; ++ test )
	{
		if ( value & (1 << test ))
		{
			*index = test;
			found = true;
			break;
		}
	}
#endif
	return found;
}
