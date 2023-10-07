#if INTELLISENSE_DIRECTIVES
#include "engine.hpp"
#endif

NS_ENGINE_BEGIN

using GetSoundSampleValueFn = s16( EngineState* state, AudioBuffer* sound_buffer );

internal s16
square_wave_sample_value( EngineState* state, AudioBuffer* sound_buffer )
{
	s32 wave_period = sound_buffer->samples_per_second / state->wave_tone_hz;

	s32 sample_value = (sound_buffer->running_sample_index /  (wave_period / 2) ) % 2 ?
		state->tone_volume : - state->tone_volume;

	return scast(s16, sample_value);
}

internal s16
sine_wave_sample_value( EngineState* state, AudioBuffer* sound_buffer )
{
	f32& time = state->sample_wave_sine_time;

	s32 wave_period = sound_buffer->samples_per_second / state->wave_tone_hz;

	// time =  TAU * (f32)sound_buffer->RunningSampleIndex / (f32)SoundTest_WavePeriod;
	f32 sine_value   = sinf( time );
	s16 sample_value = scast(s16, sine_value * scast(f32, state->tone_volume));

	time += TAU * 1.0f / scast(f32, wave_period );
	if ( time > TAU )
	{
		time -= TAU;
	}
	return sample_value;
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

	u8* row   = rcast( u8*, buffer->memory);
	local_persist float wildcard = 0;
	for ( u32 y = 0; y < buffer->height; ++ y )
	{
		// u8* pixel = rcast(u8*, row);
		// Pixel* pixel = rcast( Pixel*, row );
		u32* pixel = rcast(u32*, row);
		for ( u32 x = 0; x < buffer->width; ++ x )
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
			u8 red   = scast(u8, y + y_offset);
			u8 green = scast(u8, x + x_offset);
			u8 blue  = scast(u8, x + y_offset) - scast(u8, y + y_offset);
			//    blue *= 2;
		#endif

			*pixel++ = u32(red/2 << 16) | u32(green/6 << 0) | blue/2 << 0;
		}
		wildcard += 0.5375f;
		row += buffer->pitch;
	}
}

internal
void render_player( OffscreenBuffer* buffer, s32 pos_x, s32 pos_y )
{
	u8* end_of_buffer = rcast(u8*, buffer->memory)
		- buffer->bytes_per_pixel * buffer->width
		+ buffer->pitch * buffer->height;

	s32 top    = pos_y;
	s32 bottom = pos_y + 10;

	u32 color = 0xFFFFFFFF;

	for ( s32 coord_x = pos_x; coord_x < (pos_x+ 10); ++ coord_x )
	{
		u8*
		pixel_byte  = rcast(u8*, buffer->memory);
		pixel_byte += coord_x * buffer->bytes_per_pixel;
		pixel_byte += top     * buffer->pitch;

		for ( s32 coord_y = top; coord_y < bottom; ++ coord_y )
		{
			if ( pixel_byte < buffer->memory || pixel_byte >= end_of_buffer )
				continue;

			s32* pixel = rcast(s32*, pixel_byte);
			*pixel = color;



			pixel_byte += buffer->pitch;
		}
	}
}

NS_ENGINE_END
