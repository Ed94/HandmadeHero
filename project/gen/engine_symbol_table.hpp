#pragma once
#include "engine/engine.hpp"

NS_ENGINE_BEGIN

constexpr 
Str const  symbol_on_module_load = str_ascii("?on_module_reload@engine@@YAXPEAUMemory@1@PEAUModuleAPI@platform@@@Z");
constexpr 
Str const  symbol_startup = str_ascii("?startup@engine@@YAXPEAUMemory@1@PEAUModuleAPI@platform@@@Z");
constexpr 
Str const  symbol_shutdown = str_ascii("?shutdown@engine@@YAXPEAUMemory@1@PEAUModuleAPI@platform@@@Z");
constexpr 
Str const  symbol_update_and_render = str_ascii("?update_and_render@engine@@YAXPEAUInputState@1@PEAUOffscreenBuffer@1@PEAUMemory@1@PEAUModuleAPI@platform@@PEAUThreadContext@1@@Z");
constexpr 
Str const  symbol_update_audio = str_ascii("?update_audio@engine@@YAXPEAUAudioBuffer@1@PEAUMemory@1@PEAUModuleAPI@platform@@PEAUThreadContext@1@@Z");

NS_ENGINE_END
