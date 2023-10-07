#pragma once

struct Context
{
	Context* parent;
	// AllocatorInfo allocator;
	// Logger logger;
};


Context* make_context();


#include "gen/context.gen.hpp"
