#include "platform/compiler_ignores.hpp"

#define GEN_DEFINE_LIBRARY_CODE_CONSTANTS
#define GEN_IMPLEMENTATION
#define GEN_BENCHMARK
#define GEN_ENFORCE_STRONG_CODE_TYPES
#include "dependencies/gen.hpp"
#undef min
#undef max
#undef ccast
#undef pcast
#undef rcast
#undef scast
#undef do_once
#undef do_once_start
#undef do_once_end
#undef cast

#include "platform/platform_module.hpp"
#include "platform/grime.hpp"
#include "platform/macros.hpp"
#include "platform/types.hpp"
#include "platform/strings.hpp"
#include "platform/platform.hpp"

using namespace gen;
using GStr = gen::Str;

constexpr GStr fname_vec_header = txt("vectors.hpp");

#pragma push_macro("scast")
#pragma push_macro("Zero")
#undef scast
constexpr char const* vec2f_ops = stringize(
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
	<unit_type> magnitude( <type> v ) {
		<unit_type> result = sqrt( v.x * v.x + v.y * v.y );
		return result;
	}
	
	inline
	<type> normalize( <type> v ) {
		<unit_type> square_size = v.x * v.x + v.y * v.y;
		if ( square_size < scast(<unit_type>, 1e-4) ) {
			return Zero( <type> );
		}

		<unit_type> mag = sqrt( square_size );
		<type> result {
			v.x / mag,
			v.y / mag
		};
		return result;
	}

	inline
	<unit_type> scalar_product( <type> a, <type> b ) {
		<unit_type> result = a.x * b.x + a.y * b.y;
		return result;
	}
	
	inline
	<unit_type> magnitude_squared( <type> v ) {
		<unit_type> result = scalar_product( v, v );
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
	<type> operator * ( <unit_type> s, <type> v ) {
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

constexpr char const* vec2i_ops = stringize(
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
	<unit_type> magnitude( <type> v ) {
		<unit_type> result = sqrt( v.x * v.x + v.y * v.y );
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
	<type> operator * ( <unit_type> s, <type> v ) {
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
#pragma pop_macro("scast")
#pragma pop_macro("Zero")

#define gen_vec2f( vec_name, type ) gen__vec2f( txt( stringize(vec_name) ), txt( stringize(type) ) )
CodeBody gen__vec2f( GStr vec_name, GStr type )
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

	CodeBody vec_ops = parse_global_body( token_fmt( "type", vec_name, "unit_type", type, vec2f_ops) );
	CodeBody vec_def = def_global_body( args(
		vec_struct,
		fmt_newline,
		vec_ops
	));

	return vec_def;
}

#define gen_vec2i( vec_name, type ) gen__vec2i( txt( stringize(vec_name) ), txt( stringize(type) ) )
CodeBody gen__vec2i( GStr vec_name, GStr type )
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

	CodeBody vec_ops = parse_global_body( token_fmt( "type", vec_name, "unit_type", type, vec2i_ops) );
	CodeBody vec_def = def_global_body( args(
		vec_struct,
		fmt_newline,
		vec_ops
	));

	return vec_def;
}

#define gen_phys2( type ) gen__phys2( txt( stringize(type) ) )
Code gen__phys2( GStr type )
{
	StrBuilder sym_vec   = StrBuilder::fmt_buf( _ctx->Allocator_Temp, "Vec2_%s",   type.Ptr );
	StrBuilder sym_pos   = StrBuilder::fmt_buf( _ctx->Allocator_Temp, "Pos2_%s",   type.Ptr );
	StrBuilder sym_dir   = StrBuilder::fmt_buf( _ctx->Allocator_Temp, "Dir2_%s",   type.Ptr);
	StrBuilder sym_dist  = StrBuilder::fmt_buf( _ctx->Allocator_Temp, "Dist_%s",   type.Ptr );
	StrBuilder sym_vel   = StrBuilder::fmt_buf( _ctx->Allocator_Temp, "Vel2_%s",   type.Ptr );
	StrBuilder sym_accel = StrBuilder::fmt_buf( _ctx->Allocator_Temp, "Accel2_%s", type.Ptr );

#pragma push_macro("pcast")
#pragma push_macro("rcast")
#undef pcast
#undef rcast
	constexpr char const* tmpl_struct = stringize(
		struct <type>
		{
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

		template<>
		inline
		<type> tmpl_cast< <type>, <vec_type> >( <vec_type> vec )
		{
			return pcast( <type>, vec );
		}
	);
	CodeBody pos_struct = parse_global_body( token_fmt( "type", (GStr)sym_pos, "unit_type", type, "vec_type", (GStr)sym_vec, tmpl_struct ));
	CodeBody pos_ops    = parse_global_body( token_fmt( "type", (GStr)sym_pos, "unit_type", type, vec2f_ops ));

	CodeBody dir_struct = parse_global_body( token_fmt(
		"type", (GStr)sym_dir,
		"unit_type", type,
		"vec_type", (GStr)sym_vec,
		"vel_type", (GStr)sym_vel,
		"accel_type", (GStr)sym_accel,
	stringize(
		struct <type>
		{
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
			operator <vel_type>() {
				return * rcast(<vel_type>*, this);
			}
			operator <accel_type>() {
				return * rcast(<accel_type>*, this);
			}
		};

		template<>
		inline
		<type> tmpl_cast< <type>, <vec_type> >( <vec_type> vec )
		{
			<unit_type> abs_sum = abs( vec.x + vec.y );
			if ( is_nearly_zero( abs_sum - 1 ) )
				return pcast( <type>, vec );

			<vec_type> normalized = normalize(vec);
			return pcast( <type>, normalized );
		}
	)));

	CodeBody dist_def = parse_global_body( token_fmt(
		"type",      (GStr)sym_dist,
		"unit_type", type,
		"dist_type", (GStr)sym_dist,
		"pos_type",  (GStr)sym_pos,
	stringize(
		using <dist_type> = <unit_type>;

		inline
		<dist_type> distance( <pos_type> a, <pos_type> b ) {
			<unit_type> x = b.x - a.x;
			<unit_type> y = b.y - a.y;

			<dist_type> result = sqrt( x * x +  y * y );
			return result;
		}
	)));

	CodeBody vel_struct = parse_global_body( token_fmt( "type", (GStr)sym_vel, "unit_type", type, "vec_type", (GStr)sym_vec, tmpl_struct ));
	CodeBody vel_ops    = parse_global_body( token_fmt( "type", (GStr)sym_vel, "unit_type", type, vec2f_ops ));

	CodeBody accel_struct = parse_global_body( token_fmt( "type", (GStr)sym_accel, "unit_type", type, "vec_type", (GStr)sym_vec, tmpl_struct ));
	CodeBody accel_ops    = parse_global_body( token_fmt( "type", (GStr)sym_accel, "unit_type", type, vec2f_ops ));

	// TODO(Ed): Is there a better name for this?
	Code ops = parse_global_body( token_fmt(
		"unit_type",  (GStr)type,
		"vec_type",   (GStr)sym_vec,
		"pos_type",   (GStr)sym_pos,
		"dir_type",   (GStr)sym_dir,
		"vel_type",   (GStr)sym_vel,
		"accel_type", (GStr)sym_accel,
	stringize(
		inline
		<vel_type> velocity( <pos_type> a, <pos_type> b ) {
			<vec_type> result = b - a;
			return pcast(<vel_type>, result);
		}

		inline
		<pos_type>& operator +=(<pos_type>& pos, const <vel_type> vel) {
			pos.x += vel.x * engine::get_context()->delta_time;
			pos.y += vel.y * engine::get_context()->delta_time;
			return pos;
		}

		inline
		<accel_type> acceleration( <vel_type> a, <vel_type> b ) {
			<vec_type> result = b - a;
			return pcast(<accel_type>, result);
		}

		inline
		<vel_type>& operator +=(<vel_type>& vel, const <accel_type> accel) {
			vel.x += accel.x * engine::get_context()->delta_time;
			vel.y += accel.y * engine::get_context()->delta_time;
			return vel;
		}

		inline
		<dir_type> direction( <pos_type> pos_a, <pos_type> pos_b )
		{
			<vec_type>  diff = pos_b - pos_a;
			<unit_type> mag  = magnitude( diff );

			<dir_type> result {
				diff.x / mag,
				diff.y / mag
			};
			return result;
		}

		inline
		<dir_type> direction( <vel_type> vel )
		{
			<unit_type> mag = magnitude( vel );
			<dir_type> result {
				vel.x / mag,
				vel.y / mag
			};
			return result;
		}

		inline
		<dir_type> direction( <accel_type> accel )
		{
			<unit_type> mag = magnitude( accel );
			<dir_type> result {
				accel.x / mag,
				accel.y / mag
			};
			return result;
		}
	)));
#pragma pop_macro("rcast")
#pragma pop_macro("pcast")

	CodeBody result = def_global_body( args(
		pos_struct,
		pos_ops,
		dist_def,
		vel_struct,
		vel_ops,
		accel_struct,
		accel_ops,
		dir_struct,
		ops
	));
	return result;
}

int gen_main()
{
	gen::Context ctx {};
	gen::init( & ctx);
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
//		vec_header.print_fmt( "NS_ENGINE_BEGIN\n" );

		CodeUsing using_vec2  = parse_using( code( using Vec2  = Vec2_f32; ));
		CodeUsing using_vec2i = parse_using( code( using Vec2i = Vec2_s32; ));
		vec_header.print( gen_vec2f( Vec2_f32, f32) );
		vec_header.print( gen_vec2i( Vec2_s32, s32) );
		vec_header.print( using_vec2 );
		vec_header.print( using_vec2i );

//		vec_header.print_fmt( "NS_ENGINE_END\n" );
		vec_header.write();
	}

	Builder physics_header = Builder::open( txt("physics.hpp") );
	{
		physics_header.print( cmt_gen_notice );
		physics_header.print( pragma_once );
		physics_header.print_fmt( "#if INTELLISENSE_DIRECTIVES" );
		physics_header.print( fmt_newline );
		physics_header.print( def_include( txt("vectors.hpp") ));
		physics_header.print( def_include( txt("engine.hpp") ));
		physics_header.print( preprocess_endif );
		physics_header.print( fmt_newline );
//		physics_header.print_fmt( "NS_ENGINE_BEGIN\n" );

		physics_header.print( gen_phys2( f32 ) );

		physics_header.print( parse_global_body( code(
			using Dist   = Dist_f32;
			using Pos2   = Pos2_f32;
			using Dir2   = Dir2_f32;
			using Vel2   = Vel2_f32;
			using Accel2 = Accel2_f32;
		)));

//		physics_header.print_fmt( "NS_ENGINE_END\n" );
		physics_header.write();
	}

	log_fmt("Generaton finished for Handmade Hero: Engine Module\n\n");
	// gen::deinit();
	return 0;
}
