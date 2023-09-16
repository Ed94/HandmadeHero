#include "engine.h"

NS_ENGINE_BEGIN


using GetSoundSampleValueFn = s16( SoundBuffer* sound_buffer );

global s32 SoundWavePeriod = 250;

internal s16
square_wave_sample_value( SoundBuffer* sound_buffer )
{
	s16 sample_value = (sound_buffer->RunningSampleIndex /  (sound_buffer->WavePeriod /2)) % 2 ?
		sound_buffer->ToneVolume : - sound_buffer->ToneVolume;

	return sample_value;
}

internal s16
sine_wave_sample_value( SoundBuffer* sound_buffer )
{
	local_persist f32 time = 0.f;

	f32 sine_value   = sinf( time );
	s16 sample_value = scast(u16, sine_value * sound_buffer->ToneVolume);

	time += TAU * 1.0f / scast(f32, sound_buffer->WavePeriod );
	return sample_value;
}

internal void
output_sound( SoundBuffer* sound_buffer, GetSoundSampleValueFn* get_sample_value )
{
	s16* sample_out = sound_buffer->Samples;
	for ( u32 sample_index = 0; sample_index < sound_buffer->NumSamples; ++ sample_index )
	{
		s16 sample_value = get_sample_value( sound_buffer );
		++ sound_buffer->RunningSampleIndex;

		*sample_out = sample_value;
		++ sample_out;

		*sample_out = sample_value;
		++ sample_out;
	}
}

internal void
render_weird_graident(OffscreenBuffer* buffer, u32 x_offset, u32 y_offset )
{
	// TODO(Ed): See if with optimizer if buffer should be passed by value.

	struct Pixel {
		u8 Blue;
		u8 Green;
		u8 Red;
		u8 Alpha;
	};

	u8* row   = rcast( u8*, buffer->Memory);
	local_persist float wildcard = 0;
	for ( u32 y = 0; y < buffer->Height; ++ y )
	{
		// u8* pixel = rcast(u8*, row);
		// Pixel* pixel = rcast( Pixel*, row );
		u32* pixel = rcast(u32*, row);
		for ( u32 x = 0; x < buffer->Width; ++ x )
		{
			/* Pixel in memory:
			-----------------------------------------------
				Pixel + 0  Pixel + 1  Pixel + 2   Pixel + 3
				RR         GG         GG          XX
			-----------------------------------------------
				x86-64 : Little Endian Arch
				0x XX BB GG RR
			*/
		#if 0
			u8 blue  = scast(u8, x + x_offset * u8(wildcard) % 256);
			u8 green = scast(u8, y + y_offset - u8(wildcard) % 128);
			u8 red   = scast(u8, wildcard) % 256 - x * 0.4f;
		#else
			u8 blue  = scast(u8, x + x_offset);
			u8 green = scast(u8, y + y_offset);
			u8 red   = 0;
		#endif


			*pixel++ = (red << 16) | (green << 8) | blue;
		}
		wildcard += 0.5375f;
		row += buffer->Pitch;
	}
}

internal
void update_and_render( OffscreenBuffer* back_buffer, SoundBuffer* sound_buffer
	// Temp (for feature parity)
	, u32 x_offset, u32 y_offset
)
{
	// TODO(Ed) : Allow sample offsets here for more robust platform options
	output_sound( sound_buffer, square_wave_sample_value );

	render_weird_graident( back_buffer, x_offset, y_offset );
}

NS_ENGINE_END
