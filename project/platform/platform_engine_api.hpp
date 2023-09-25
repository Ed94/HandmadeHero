/*
	This represents the API only accessible to the platform layer to fullfill for the engine layer.
*/
#pragma once
#include "engine.hpp"

NS_ENGINE_BEGIN

void startup();
void shutdown();

// Needs a contextual reference to four things:
// Timing, Input, Bitmap Buffer
void update_and_render( InputState* input, OffscreenBuffer* back_buffer,  Memory* memory );

// Audio timing is complicated, processing samples must be done at a different period from the rest of the engine's usual update.
// IMPORTANT: This has very tight timing, and cannot be more than a millisecond in execution.
// TODO(Ed) : Reduce timing pressure on performance by measuring it or pinging its time.
void update_audio( AudioBuffer* audio_buffer, Memory* memory );

NS_ENGINE_END
