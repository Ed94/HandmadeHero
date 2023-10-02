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


#include "engine/engine_to_platform_api.hpp"

constexpr StrC fname_handmade_engine_symbols = txt("handmade_engine.symbols");

void get_symbols_from_module_table( FileContents symbol_table, Array<String> symbols )
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
			symbols.append( String::make_length( GlobalAllocator, token.Ptr, token.Len ) );
		}
	}
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

	Array<String> symbols = Array<String>::init_reserve( GlobalAllocator, kilobytes(1) );
	get_symbols_from_module_table( symbol_table, symbols );

	using ModuleAPI = engine::ModuleAPI;

	builder.print( parse_variable( token_fmt( "symbol", (StrC)symbols[ModuleAPI::Sym_OnModuleReload], stringize(
		constexpr const Str symbol_on_module_load = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (StrC)symbols[ModuleAPI::Sym_Startup], stringize(
		constexpr const Str symbol_startup = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (StrC)symbols[ModuleAPI::Sym_Shutdown], stringize(
		constexpr const Str symbol_shutdown = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (StrC)symbols[ModuleAPI::Sym_UpdateAndRender], stringize(
		constexpr const Str symbol_update_and_render = str_ascii("<symbol>");
	))));
	builder.print( parse_variable( token_fmt( "symbol", (StrC)symbols[ModuleAPI::Sym_UpdateAudio], stringize(
		constexpr const Str symbol_update_audio = str_ascii("<symbol>");
	))));

	builder.print_fmt( "\nNS_ENGINE_END" );
	builder.print( fmt_newline );
	builder.write();
#pragma pop_macro("str_ascii")

	log_fmt("Generaton finished for Handmade Hero: Engine Module\n\n");
	// gen::deinit();
	return 0;
}
#endif
