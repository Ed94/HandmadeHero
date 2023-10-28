// This was generated by project/codegen/engine_gen.cpp
#pragma once
#if INTELLISENSE_DIRECTIVES
#include "vectors.hpp"
#include "engine.hpp"
#endif

struct Pos2_f32
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
		};

		f32 Basis[ 2 ];
	};

	operator Vec2_f32()
	{
		return *rcast( Vec2_f32*, this );
	}
};

template<>
inline Pos2_f32 tmpl_cast< Pos2_f32, Vec2_f32 >( Vec2_f32 vec )
{
	return pcast( Pos2_f32, vec );
}

template<>
constexpr Pos2_f32 tmpl_zero< Pos2_f32 >()
{
	return { 0, 0 };
}

inline Pos2_f32 abs( Pos2_f32 v )
{
	Pos2_f32 result { abs( v.x ), abs( v.y ) };
	return result;
}

inline f32 magnitude( Pos2_f32 v )
{
	f32 result = sqrt( v.x * v.x + v.y * v.y );
	return result;
}

inline Pos2_f32 normalize( Pos2_f32 v )
{
	f32 square_size = v.x * v.x + v.y * v.y;
	if ( square_size < scast( f32, 1e-4 ) )
	{
		return Zero( Pos2_f32 );
	}
	f32      mag = sqrt( square_size );
	Pos2_f32 result { v.x / mag, v.y / mag };
	return result;
}

inline Pos2_f32 operator-( Pos2_f32 v )
{
	Pos2_f32 result { -v.x, -v.y };
	return result;
}

inline Pos2_f32 operator+( Pos2_f32 a, Pos2_f32 b )
{
	Pos2_f32 result { a.x + b.x, a.y + b.y };
	return result;
}

inline Pos2_f32 operator-( Pos2_f32 a, Pos2_f32 b )
{
	Pos2_f32 result { a.x - b.x, a.y - b.y };
	return result;
}

inline Pos2_f32 operator*( Pos2_f32 v, f32 s )
{
	Pos2_f32 result { v.x * s, v.y * s };
	return result;
}

inline Pos2_f32 operator*( f32 s, Pos2_f32 v )
{
	Pos2_f32 result { v.x * s, v.y * s };
	return result;
}

inline Pos2_f32 operator/( Pos2_f32 v, f32 s )
{
	Pos2_f32 result { v.x / s, v.y / s };
	return result;
}

inline Pos2_f32& operator+=( Pos2_f32& a, Pos2_f32 b )
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Pos2_f32& operator-=( Pos2_f32& a, Pos2_f32 b )
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

inline Pos2_f32& operator*=( Pos2_f32& v, f32 s )
{
	v.x *= s;
	v.y *= s;
	return v;
}

inline Pos2_f32& operator/=( Pos2_f32& v, f32 s )
{
	v.x /= s;
	v.y /= s;
	return v;
}

using Dist2_f32 = f32;

inline Dist2_f32 distance( Pos2_f32 a, Pos2_f32 b )
{
	f32       x      = b.x - a.x;
	f32       y      = b.y - a.y;
	Dist2_f32 result = sqrt( x * x + y * y );
	return result;
}

struct Vel2_f32
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
		};

		f32 Basis[ 2 ];
	};

	operator Vec2_f32()
	{
		return *rcast( Vec2_f32*, this );
	}
};

template<>
inline Vel2_f32 tmpl_cast< Vel2_f32, Vec2_f32 >( Vec2_f32 vec )
{
	return pcast( Vel2_f32, vec );
}

template<>
constexpr Vel2_f32 tmpl_zero< Vel2_f32 >()
{
	return { 0, 0 };
}

inline Vel2_f32 abs( Vel2_f32 v )
{
	Vel2_f32 result { abs( v.x ), abs( v.y ) };
	return result;
}

inline f32 magnitude( Vel2_f32 v )
{
	f32 result = sqrt( v.x * v.x + v.y * v.y );
	return result;
}

inline Vel2_f32 normalize( Vel2_f32 v )
{
	f32 square_size = v.x * v.x + v.y * v.y;
	if ( square_size < scast( f32, 1e-4 ) )
	{
		return Zero( Vel2_f32 );
	}
	f32      mag = sqrt( square_size );
	Vel2_f32 result { v.x / mag, v.y / mag };
	return result;
}

inline Vel2_f32 operator-( Vel2_f32 v )
{
	Vel2_f32 result { -v.x, -v.y };
	return result;
}

inline Vel2_f32 operator+( Vel2_f32 a, Vel2_f32 b )
{
	Vel2_f32 result { a.x + b.x, a.y + b.y };
	return result;
}

inline Vel2_f32 operator-( Vel2_f32 a, Vel2_f32 b )
{
	Vel2_f32 result { a.x - b.x, a.y - b.y };
	return result;
}

inline Vel2_f32 operator*( Vel2_f32 v, f32 s )
{
	Vel2_f32 result { v.x * s, v.y * s };
	return result;
}

inline Vel2_f32 operator*( f32 s, Vel2_f32 v )
{
	Vel2_f32 result { v.x * s, v.y * s };
	return result;
}

inline Vel2_f32 operator/( Vel2_f32 v, f32 s )
{
	Vel2_f32 result { v.x / s, v.y / s };
	return result;
}

inline Vel2_f32& operator+=( Vel2_f32& a, Vel2_f32 b )
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Vel2_f32& operator-=( Vel2_f32& a, Vel2_f32 b )
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

inline Vel2_f32& operator*=( Vel2_f32& v, f32 s )
{
	v.x *= s;
	v.y *= s;
	return v;
}

inline Vel2_f32& operator/=( Vel2_f32& v, f32 s )
{
	v.x /= s;
	v.y /= s;
	return v;
}

struct Accel2_f32
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
		};

		f32 Basis[ 2 ];
	};

	operator Vec2_f32()
	{
		return *rcast( Vec2_f32*, this );
	}
};

template<>
inline Accel2_f32 tmpl_cast< Accel2_f32, Vec2_f32 >( Vec2_f32 vec )
{
	return pcast( Accel2_f32, vec );
}

template<>
constexpr Accel2_f32 tmpl_zero< Accel2_f32 >()
{
	return { 0, 0 };
}

inline Accel2_f32 abs( Accel2_f32 v )
{
	Accel2_f32 result { abs( v.x ), abs( v.y ) };
	return result;
}

inline f32 magnitude( Accel2_f32 v )
{
	f32 result = sqrt( v.x * v.x + v.y * v.y );
	return result;
}

inline Accel2_f32 normalize( Accel2_f32 v )
{
	f32 square_size = v.x * v.x + v.y * v.y;
	if ( square_size < scast( f32, 1e-4 ) )
	{
		return Zero( Accel2_f32 );
	}
	f32        mag = sqrt( square_size );
	Accel2_f32 result { v.x / mag, v.y / mag };
	return result;
}

inline Accel2_f32 operator-( Accel2_f32 v )
{
	Accel2_f32 result { -v.x, -v.y };
	return result;
}

inline Accel2_f32 operator+( Accel2_f32 a, Accel2_f32 b )
{
	Accel2_f32 result { a.x + b.x, a.y + b.y };
	return result;
}

inline Accel2_f32 operator-( Accel2_f32 a, Accel2_f32 b )
{
	Accel2_f32 result { a.x - b.x, a.y - b.y };
	return result;
}

inline Accel2_f32 operator*( Accel2_f32 v, f32 s )
{
	Accel2_f32 result { v.x * s, v.y * s };
	return result;
}

inline Accel2_f32 operator*( f32 s, Accel2_f32 v )
{
	Accel2_f32 result { v.x * s, v.y * s };
	return result;
}

inline Accel2_f32 operator/( Accel2_f32 v, f32 s )
{
	Accel2_f32 result { v.x / s, v.y / s };
	return result;
}

inline Accel2_f32& operator+=( Accel2_f32& a, Accel2_f32 b )
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Accel2_f32& operator-=( Accel2_f32& a, Accel2_f32 b )
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

inline Accel2_f32& operator*=( Accel2_f32& v, f32 s )
{
	v.x *= s;
	v.y *= s;
	return v;
}

inline Accel2_f32& operator/=( Accel2_f32& v, f32 s )
{
	v.x /= s;
	v.y /= s;
	return v;
}

struct Dir2_f32
{
	union
	{
		struct
		{
			f32 x;
			f32 y;
		};

		f32 Basis[ 2 ];
	};

	operator Vec2_f32()
	{
		return *rcast( Vec2_f32*, this );
	}

	operator Vel2_f32()
	{
		return *rcast( Vel2_f32*, this );
	}

	operator Accel2_f32()
	{
		return *rcast( Accel2_f32*, this );
	}
};

template<>
inline Dir2_f32 tmpl_cast< Dir2_f32, Vec2_f32 >( Vec2_f32 vec )
{
	f32 abs_sum = abs( vec.x + vec.y );
	if ( is_nearly_zero( abs_sum - 1 ) )
		return pcast( Dir2_f32, vec );
	Vec2_f32 normalized = normalize( vec );
	return pcast( Dir2_f32, normalized );
}

inline Vel2_f32 velocity( Pos2_f32 a, Pos2_f32 b )
{
	Vec2_f32 result = b - a;
	return pcast( Vel2_f32, result );
}

inline Pos2_f32& operator+=( Pos2_f32& pos, Vel2_f32 const vel )
{
	pos.x += vel.x * engine::get_context()->delta_time;
	pos.y += vel.y * engine::get_context()->delta_time;
	return pos;
}

inline Accel2_f32 acceleration( Vel2_f32 a, Vel2_f32 b )
{
	Vec2_f32 result = b - a;
	return pcast( Accel2_f32, result );
}

inline Vel2_f32& operator+=( Vel2_f32& vel, Accel2_f32 const accel )
{
	vel.x += accel.x * engine::get_context()->delta_time;
	vel.y += accel.y * engine::get_context()->delta_time;
	return vel;
}

inline Dir2_f32 direction( Pos2_f32 pos_a, Pos2_f32 pos_b )
{
	Vec2_f32 diff = pos_b - pos_a;
	f32      mag  = magnitude( diff );
	Dir2_f32 result { diff.x / mag, diff.y / mag };
	return result;
}

inline Dir2_f32 direction( Vel2_f32 vel )
{
	f32      mag = magnitude( vel );
	Dir2_f32 result { vel.x / mag, vel.y / mag };
	return result;
}

inline Dir2_f32 direction( Accel2_f32 accel )
{
	f32      mag = magnitude( accel );
	Dir2_f32 result { accel.x / mag, accel.y / mag };
	return result;
}

using Pos2   = Pos2_f32;
using Dir2   = Dir2_f32;
using Dist2  = Dist2_f32;
using Vel2   = Vel2_f32;
using Accel2 = Accel2_f32;
