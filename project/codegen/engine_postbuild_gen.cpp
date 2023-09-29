#if GEN_TIME
#define GEN_DEFINE_LIBRARY_CODE_CONSTANTS
#define GEN_IMPLEMENTATION
#define GEN_BENCHMARK
#define GEN_ENFORCE_STRONG_CODE_TYPES
#include "dependencies/gen.hpp"
using namespace gen;

#include "engine/engine_to_platform_api.hpp"
constexpr StrC fname_handmade_engine_symbols = txt("handmade_engine.symbols");

String get_symbol_from_module_table( FileContents symbol_table, u32 symbol_ID )
{
	struct Token
	{
		char const* Ptr;
		u32         Len;
	};

	Token tokens[256] = {};

	char const* scanner = rcast( char const*, symbol_table.data );
	u32 left = symbol_table.size;
	u32 line = 0;
	while ( left )
	{
		if ( *scanner == '\n' || *scanner == '\r' )
		{
			++ scanner;
			-- left;
		}
		else
		{
			tokens[line].Ptr = scanner;
			while ( left && *scanner != '\r' && *scanner != '\n' )
			{
				-- left;
				++ scanner;
				++ tokens[line].Len;
			}

			if ( line == symbol_ID )
			{
				String result = String::make_length( GlobalAllocator, tokens[line].Ptr, tokens[line].Len );
				return result;
			}
			++ line;
		}
	}
	return {};
}

int gen_main()
{
	gen::init();
	log_fmt("Generating code for Handmade Hero: Engine Module\n");

	FileContents symbol_table = file_read_contents( GlobalAllocator, true, fname_handmade_engine_symbols );

#pragma push_macro("str_ascii")
#undef str_ascii
	Builder builder = Builder::open( "engine_symbol_table.hpp" );
	builder.print( pragma_once );
	builder.print( def_include( txt("engine/engine.hpp") ) );
	builder.print( fmt_newline );
	builder.print_fmt( "NS_ENGINE_BEGIN\n\n" );

	StrC symbol_on_module_load    = get_symbol_from_module_table( symbol_table, engine::ModuleAPI::Sym_OnModuleReload );
	StrC symbol_startup           = get_symbol_from_module_table( symbol_table, engine::ModuleAPI::Sym_Startup );
	StrC symbol_shutdown          = get_symbol_from_module_table( symbol_table, engine::ModuleAPI::Sym_Shutdown );
	StrC symbol_update_and_render = get_symbol_from_module_table( symbol_table, engine::ModuleAPI::Sym_UpdateAndRender );
	StrC symbol_update_audio      = get_symbol_from_module_table( symbol_table, engine::ModuleAPI::Sym_UpdateAudio );


	builder.print( parse_variable( token_fmt( "symbol", symbol_on_module_load, stringize(
		constexpr const Str symbol_on_module_load = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", symbol_startup, stringize(
		constexpr const Str symbol_startup = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", symbol_shutdown, stringize(
		constexpr const Str symbol_shutdown = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", symbol_update_and_render, stringize(
		constexpr const Str symbol_update_and_render = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", symbol_update_audio, stringize(
		constexpr const Str symbol_update_audio = str_ascii("<symbol>");
	))));

	builder.print_fmt( "\nNS_ENGINE_END" );
	builder.print( fmt_newline );
	builder.write();
#pragma pop_macro("str_ascii")

	log_fmt("Generaton finished for Handmade Hero: Engine Module\n");
	// gen::deinit();
	return 0;
}
#endif
