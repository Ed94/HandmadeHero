#if GEN_TIME
#define GEN_DEFINE_LIBRARY_CODE_CONSTANTS
#define GEN_IMPLEMENTATION
#define GEN_BENCHMARK
#define GEN_ENFORCE_STRONG_CODE_TYPES
#include "gen.hpp"
using namespace gen;

int gen_main()
{
	gen::init();
	log_fmt("Generating code for Handmade Hero\n");

	log_fmt("Generaton finished for Handmade Hero\n");
	// gen::deinit();
	return 0;
}
#endif
