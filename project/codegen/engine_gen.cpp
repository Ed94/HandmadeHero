#include "platform/compiler_ignores.hpp"

#if GEN_TIME
#define GEN_DEFINE_LIBRARY_CODE_CONSTANTS
#define GEN_IMPLEMENTATION
#define GEN_BENCHMARK
#define GEN_ENFORCE_STRONG_CODE_TYPES
#include "dependencies/gen.hpp"
#undef ccast
#undef pcast
#undef rcast
#undef scast
#undef do_once
#undef do_once_start
#undef do_once_end
using namespace gen;

#include "platform/platform_module.hpp"
#include "platform/grime.hpp"
#include "platform/macros.hpp"
#include "platform/types.hpp"
#include "platform/strings.hpp"
#include "platform/platform.hpp"

constexpr StrC fname_vec_header = txt("vectors.hpp");

constexpr char const* vec2_ops = stringize(
	template<>
	constexpr <type> tmpl_zero< <type> >() {
		return { 0, 0 };
	}

	inline
	<type> abs( <type> v ) {
		<type> result {
			abs( v.x ),
			abs( v.y )
		};
		return result;
	}

	inline
	<type> operator - ( <type> v ) {
		<type> result {
			- v.x,
			- v.y
		};
		return result;
	}

	inline
	<type> operator + ( <type> a, <type> b ) {
		<type> result {
			a.x + b.x,
			a.y + b.y
		};
		return result;
	}

	inline
	<type> operator - ( <type> a, <type> b ) {
		<type> result {
			a.x - b.x,
			a.y - b.y
		};
		return result;
	}
	
	inline
	<type> operator * ( <type> v, <unit_type> s ) {
		<type> result {
			v.x * s,
			v.y * s
		};
		return result;
	}
		
	inline
	<type> operator / ( <type> v, <unit_type> s ) {
		<type> result {
			v.x / s,
			v.y / s
		};
		return result;
	}

	inline
	<type>& operator += ( <type>& a, <type> b ) {
		a.x += b.x;
		a.y += b.y;
		return a;
	}
	inline
	<type>& operator -= ( <type>& a, <type> b ) {
		a.x -= b.x;
		a.y -= b.y;
		return a;
	}

	inline
	<type>& operator *= ( <type>& v, <unit_type> s ) {
		v.x *= s;
		v.y *= s;
		return v;
	}

	inline
	<type>& operator /= ( <type>& v, <unit_type> s ) {
		v.x /= s;
		v.y /= s;
		return v;
	}
);

#define gen_vec2( vec_name, type ) gen__vec2( txt( stringize(vec_name) ), txt( stringize(type) ) )
CodeBody gen__vec2( StrC vec_name, StrC type )
{
	CodeStruct vec_struct = parse_struct( token_fmt( "type", vec_name, "unit_type", type, stringize(
		struct <type>
		{
			union {
				struct {
					<unit_type> x;
					<unit_type> y;
				};
				<unit_type> Basis[2];
			};
		};
	)));

	CodeBody vec_ops = parse_global_body( token_fmt( "type", vec_name, "unit_type", type, vec2_ops) );
	CodeBody vec_def = def_global_body( args(
		vec_struct,
		fmt_newline,
		vec_ops
	));

	return vec_def;
}

#define gen_phys2( type ) gen__phys2( txt( stringize(type) ) )
Code gen__phys2( StrC type )
{
	String t_vec   = String::fmt_buf( GlobalAllocator, "Vec2_%s",   type.Ptr );
	String t_pos   = String::fmt_buf( GlobalAllocator, "Pos2_%s",   type.Ptr );
	String t_dist  = String::fmt_buf( GlobalAllocator, "Dist2_%s",  type.Ptr );
	String t_vel   = String::fmt_buf( GlobalAllocator, "Vel2_%s",   type.Ptr );
	String t_accel = String::fmt_buf( GlobalAllocator, "Accel2_%s", type.Ptr );

#pragma push_macro("pcast")
#pragma push_macro("rcast")
#undef pcast
#undef rcast
	Code result = parse_global_body( token_fmt(
		"unit_type",  (StrC)type,
		"vec_type",   (StrC)t_vec,
		"pos_type",   (StrC)t_pos,
		"dist_type",  (StrC)t_dist,
		"vel_type",   (StrC)t_vel,
		"accel_type", (StrC)t_accel,
	stringize(
		struct <pos_type> {
			union {
				struct {
					<unit_type> x;
					<unit_type> y;
				};
				<unit_type> Basis[2];
			};
			
			operator <vec_type>() {
				return * rcast(<vec_type>*, this);
			}
		};

		using <dist_type> = <unit_type>;

		inline
		<dist_type> distance( <pos_type> a, <pos_type> b ) {
			<unit_type> x = b.x - a.x;
			<unit_type> y = b.y - a.y;

			<dist_type> result = sqrt( x * x +  y * y );
			return result;
		}

		struct <vel_type> {
			union {
				struct {
					<unit_type> x;
					<unit_type> y;
				};
				<unit_type> Basis[2];
			};

			operator <vec_type>() {
				return * rcast(<vec_type>*, this);
			}
		};

		inline
		<vel_type> velocity( <pos_type> a, <pos_type> b ) {
			<vec_type> result = b - a;
			return pcast(<vel_type>, result);
		}

		inline
		<pos_type>& operator +=(<pos_type>& pos, const <vel_type>& vel) {
			pos.x += vel.x;
			pos.y += vel.y;
			return pos;
		}

		inline
		<vel_type>& operator *= ( <vel_type>& v, <unit_type> s ) {
			v.x *= s;
			v.y *= s;
			return v;
		}

		struct <accel_type> {
			union {
				struct {
					<unit_type> x;
					<unit_type> y;
				};
				<unit_type> Basis[2];
			};

			operator <vec_type>() {
				return * rcast(<vec_type>*, this);
			}
		};

		inline
		<accel_type> acceleration( <vel_type> a, <vel_type> b ) {
			<vec_type> result = b - a;
			return pcast(<accel_type>, result);
		}
	)));
	return result;
#pragma pop_macro("rcast")
#pragma pop_macro("pcast")
}

int gen_main()
{
	gen::init();
	log_fmt("Generating code for Handmade Hero: Engine Module\n");

	CodeComment cmt_gen_notice = def_comment( txt("This was generated by project/codegen/engine_gen.cpp") );

	Builder vec_header = Builder::open(fname_vec_header);
	{
		vec_header.print( cmt_gen_notice );
		vec_header.print( pragma_once );
		vec_header.print_fmt( "#if INTELLISENSE_DIRECTIVES" );
		vec_header.print( fmt_newline );
		vec_header.print( def_include( txt("engine_module.hpp") ));
		vec_header.print( def_include( txt("platform.hpp") ));
		vec_header.print( preprocess_endif );
		vec_header.print( fmt_newline );

		CodeUsing using_vec2  = parse_using( code( using Vec2  = Vec2_f32; ));
		CodeUsing using_vec2i = parse_using( code( using Vec2i = Vec2_s32; ));
		vec_header.print( gen_vec2( Vec2_f32, f32) );
		vec_header.print( gen_vec2( Vec2_s32, s32) );
		vec_header.print( using_vec2 );
		vec_header.print( using_vec2i );

		CodeUsing using_vec3  = parse_using( code( using Vec2  = Vec3_f32; ));
		CodeUsing using_vec3i = parse_using( code( using Vec2i = Vec3_f32; ));

		// vec_header.print( using_vec3 );
		vec_header.write();
	}

	Builder physics_header = Builder::open( txt("physics.hpp") );
	{
		physics_header.print( cmt_gen_notice );
		physics_header.print( pragma_once );
		physics_header.print_fmt( "#if INTELLISENSE_DIRECTIVES" );
		physics_header.print( fmt_newline );
		physics_header.print( def_include( txt("vectors.hpp") ));
		physics_header.print( preprocess_endif );
		physics_header.print( fmt_newline );

		physics_header.print( gen_phys2( f32 ) );
		physics_header.print( gen_phys2( s32 ) );

		physics_header.print( parse_global_body( code(
			using Pos2   = Pos2_f32;
			using Dist2  = Dist2_f32;
			using Vel2   = Vel2_f32;
			using Accel2 = Accel2_f32;
			
			using Pos2i   = Pos2_s32;
			using Dist2i  = Dist2_s32;
			using Vel2i   = Vel2_s32;
			using Accel2i = Accel2_s32;
		)));

		physics_header.write();
	}

	log_fmt("Generaton finished for Handmade Hero: Engine Module\n\n");
	// gen::deinit();
	return 0;
}
#endif
