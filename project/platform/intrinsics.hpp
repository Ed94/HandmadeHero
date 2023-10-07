#pragma once

#if INTELLISENSE_DIRECTIVES
#include <math.h>
#endif

// TODO(Ed) : Convert all of these to platform-efficient versions

inline
s32 floor_f32_to_s32( f32 value )
{
	s32 result = scast(s32, floorf( value ));
	return result;
}

inline
s32 round_f32_to_s32( f32 value )
{
	s32 result = scast(s32, value + 0.5f);
	return result;
}

inline
s32 truncate_f32_to_s32( f32 value )
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

f32 cosine( f32 angle )
{
	f32 result = cosf( angle );
	return result;
}

f32 arc_tangent( f32 Y, f32 X )
{
	f32 result = atan2f( Y, X );
	return result;
}
