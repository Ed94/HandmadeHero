#if GEN_TIME
#define GEN_DEFINE_LIBRARY_CODE_CONSTANTS
#define GEN_IMPLEMENTATION
#define GEN_BENCHMARK
#define GEN_ENFORCE_STRONG_CODE_TYPES
#include "dependencies/gen.hpp"
using namespace gen;

int gen_main()
{
	gen::init();
	log_fmt("Generating code for Handmade Hero: Platform Module\n");
	log_fmt("Generaton finished for Handmade Hero: Platform Module\n");
	// gen::deinit();
	return 0;
}
#endif
