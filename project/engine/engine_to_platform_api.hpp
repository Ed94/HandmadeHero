/*
	This represents the API only accessible to the platform layer to fullfill for the engine layer.
*/
#pragma once
#include "engine/engine.hpp"

#ifndef Engine_API
#	define Engine_API
#endif

NS_ENGINE_BEGIN

using OnModuleRelaodFn = void( Memory* memory, platform::ModuleAPI* platform_api );
using StartupFn        = void( Memory* memory, platform::ModuleAPI* platform_api );
using ShutdownFn       = void( Memory* memory, platform::ModuleAPI* platform_api );

// Needs a contextual reference to four things:
// Timing, Input, Bitmap Buffer
using UpdateAndRenderFn = void ( InputState* input, OffscreenBuffer* back_buffer, Memory* memory, platform::ModuleAPI* platform_api );

// Audio timing is complicated, processing samples must be done at a different period from the rest of the engine's usual update.
// IMPORTANT: This has very tight timing, and cannot be more than a millisecond in execution.
// TODO(Ed) : Reduce timing pressure on performance by measuring it or pinging its time.
using UpdateAudioFn = void ( AudioBuffer* audio_buffer, Memory* memory, platform::ModuleAPI* platform_api );

struct ModuleAPI
{
	enum : u32
	{
		Sym_OnModuleReload,
		Sym_Startup,
		Sym_Shutdown,
		Sym_UpdateAndRender,
		Sym_UpdateAudio,
	};

	OnModuleRelaodFn* on_module_reload;
	StartupFn*        startup;
	ShutdownFn*       shutdown;

	UpdateAndRenderFn* update_and_render;
	UpdateAudioFn*     update_audio;

	b32 IsValid;
};

NS_ENGINE_END
