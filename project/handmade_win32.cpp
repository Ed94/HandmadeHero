/*
Handmade Win32 Platform Translation Unit
*/

#include "compiler_ignores.hpp"

#include <math.h>  // TEMP
#include <stdio.h> // TEMP

#include "grime.hpp"
#include "macros.hpp"
#include "generics.hpp"
#include "math_constants.hpp"
#include "types.hpp"
#include "intrinsics.hpp"
#include "strings.hpp"
#include "context.hpp"
#include "platform.hpp"

// Engine layer headers
#include "engine/engine.hpp"
#include "engine/engine_to_platform_api.hpp"
#include "gen/engine_symbol_table.hpp"

#include "jsl.hpp" // Using this to get dualsense controllers
#include "win32/win32.hpp"
#include "win32/win32_platform.cpp"
