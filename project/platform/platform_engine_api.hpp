/*
	This represents the API only accessible to the platform layer to fullfill for the engine layer.
*/
#pragma once
#include "engine.hpp"

NS_ENGINE_BEGIN

void startup();
void shutdown();

// Needs a contextual reference to four things:
// Timing, Input, Bitmap Buffer, Sound Buffer
void update_and_render( InputState* input, OffscreenBuffer* back_buffer, SoundBuffer* sound_buffer, Memory* memory );

NS_ENGINE_END
