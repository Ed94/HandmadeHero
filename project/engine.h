/*
	Services the engine provides to the platform layer
*/

#pragma once

#include "platform.h"

#define NS_ENGINE_BEGIN namespace engine {
#define NS_ENGINE_END }

NS_ENGINE_BEGIN

struct OffscreenBuffer
{
	void*      Memory; // Lets use directly mess with the "pixel's memory buffer"
	u32        Width;
	u32        Height;
	u32        Pitch;
	u32        BytesPerPixel;
};

struct SoundBuffer
{
	s16* Samples;
	u32  RunningSampleIndex;
	s32  SamplesPerSecond;
	s32  NumSamples;
	s32  ToneVolume;
	s32  WaveToneHz;
	s32  WavePeriod;
};

// Needs a contextual reference to four things:
// Timing, Input, Bitmap Buffer, Sound Buffer
void update_and_render( OffscreenBuffer* back_buffer, SoundBuffer* sound_buffer
	// Temp (for feature parity)
	, u32 x_offset, u32 y_offset
);

NS_ENGINE_END
