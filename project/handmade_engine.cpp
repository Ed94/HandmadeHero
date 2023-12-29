/*
Handmade Engine Translation Unit
*/

#include "platform/compiler_ignores.hpp"

#include <math.h>  // TEMP
#include <stdio.h> // TEMP

#include "platform/platform_module.hpp"
#include "platform/grime.hpp"
#include "platform/macros.hpp"
#include "platform/generics.hpp"
#include "platform/math_constants.hpp"
#include "platform/types.hpp"
#include "platform/intrinsics.hpp"
#include "platform/float_ops.hpp"
#include "platform/strings.hpp"
#include "platform/context.hpp"
#include "platform/platform.hpp"

#include "engine_module.hpp"
#include "gen/vectors.hpp"
#include "input.hpp"
#include "tile_map.hpp"
#include "engine.hpp"
// Physics Depends on stuff in engine.hpp for now.
#include "gen/physics.hpp"
#include "engine_to_platform_api.hpp"

// Game layer headers
#include "handmade.hpp"

#include "tile_map.cpp"
#include "random.cpp"
#include "engine.cpp"

