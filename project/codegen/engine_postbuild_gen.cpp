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
#undef min
#undef max
#undef cast

#include <math.h>

#include "platform/platform_module.hpp"
#include "platform/grime.hpp"
#include "platform/macros.hpp"
#include "platform/generics.hpp"
#include "platform/types.hpp"
#include "platform/intrinsics.hpp"
#include "platform/float_ops.hpp"
#include "platform/strings.hpp"
#include "platform/platform.hpp"

#include "engine/engine_module.hpp"
#include "engine/gen/vectors.hpp"
#include "engine/input.hpp"
#include "engine/tile_map.hpp"
#include "engine/engine.hpp"
#include "engine/engine_to_platform_api.hpp"
#include "engine/gen/physics.hpp"

using namespace gen;

using GStr = gen::Str;

constexpr GStr fname_handmade_engine_symbols = txt("handmade_engine.symbols");

void get_symbols_from_module_table( FileContents symbol_table, Array<StrBuilder> symbols )
{
	struct Token
	{
		char const* Ptr;
		u32         Len;
	};

	char const* scanner = rcast( char const*, symbol_table.data );
	u32 left = symbol_table.size;
	while ( left )
	{
		if ( *scanner == '\n' || *scanner == '\r' )
		{
			++ scanner;
			-- left;
		}
		else
		{
			Token token {};
			token.Ptr = scanner;
			while ( left && *scanner != '\r' && *scanner != '\n' )
			{
				-- left;
				++ scanner;
				++ token.Len;
			}
			symbols.append( StrBuilder::make_length( _ctx->Allocator_Temp, token.Ptr, token.Len ) );
		}
	}
}

int gen_main()
{
	gen::Context ctx {};
	gen::init( & ctx);
	log_fmt("Generating code for Handmade Hero: Engine Module (Post-Build)\n");

	FileContents symbol_table = file_read_contents( ctx.Allocator_Temp, true, fname_handmade_engine_symbols );

#pragma push_macro("str_ascii")
#undef str_ascii
	Builder builder = Builder::open( "engine_symbols.gen.hpp" );
	builder.print( pragma_once );
	builder.print( def_include( txt("engine/engine.hpp") ) );
	builder.print( fmt_newline );
	builder.print_fmt( "NS_ENGINE_BEGIN\n\n" );

	Array<StrBuilder> symbols = Array<StrBuilder>::init_reserve( ctx.Allocator_Temp, kilobytes(1) );
	get_symbols_from_module_table( symbol_table, symbols );

	using ModuleAPI = engine::ModuleAPI;

	builder.print( parse_variable( token_fmt( "symbol", (GStr)symbols[ModuleAPI::Sym_OnModuleReload], stringize(
		constexpr const Str symbol_on_module_load = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (GStr)symbols[ModuleAPI::Sym_Startup], stringize(
		constexpr const Str symbol_startup = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (GStr)symbols[ModuleAPI::Sym_Shutdown], stringize(
		constexpr const Str symbol_shutdown = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (GStr)symbols[ModuleAPI::Sym_UpdateAndRender], stringize(
		constexpr const Str symbol_update_and_render = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (GStr)symbols[ModuleAPI::Sym_UpdateAudio], stringize(
		constexpr const Str symbol_update_audio = str_ascii("<symbol>");
	))));

	builder.print_fmt( "\nNS_ENGINE_END" );
	builder.print( fmt_newline );
	builder.write();
#pragma pop_macro("str_ascii")

	log_fmt("Generaton finished for Handmade Hero: Engine Module (Post-Build)\n\n");
	// gen::deinit();
	return 0;
}
#endif
