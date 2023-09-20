#include "engine.h"
#include "win32.h"

NS_ENGINE_BEGIN

struct EngineState
{
	s32 WavePeriod;
	s32 WaveToneHz;
	s32 ToneVolume;
	s32 XOffset;
	s32 YOffset;
};

using GetSoundSampleValueFn = s16( EngineState* state, SoundBuffer* sound_buffer );

internal s16
square_wave_sample_value( EngineState* state, SoundBuffer* sound_buffer )
{
	s16 sample_value = (sound_buffer->RunningSampleIndex /  (state->WavePeriod / 2) ) % 2 ?
		state->ToneVolume : - state->ToneVolume;

	return sample_value;
}

internal s16
sine_wave_sample_value( EngineState* state, SoundBuffer* sound_buffer )
{
	local_persist f32 time = 0.f;

	// time =  TAU * (f32)sound_buffer->RunningSampleIndex / (f32)SoundTest_WavePeriod;
	f32 sine_value   = sinf( time );
	s16 sample_value = scast(u16, sine_value * state->ToneVolume);

	time += TAU * 1.0f / scast(f32, state->WavePeriod );
	return sample_value;
}

internal void
output_sound( EngineState* state, SoundBuffer* sound_buffer, GetSoundSampleValueFn* get_sample_value )
{
	s16* sample_out = sound_buffer->Samples;
	for ( u32 sample_index = 0; sample_index < sound_buffer->NumSamples; ++ sample_index )
	{
		s16 sample_value = get_sample_value( state, sound_buffer );
		sound_buffer->RunningSampleIndex++;

		// char ms_timing_debug[256] {};
		// wsprintfA( ms_timing_debug, "sample_value: %d\n", sample_value );
		// OutputDebugStringA( ms_timing_debug );

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

b32 input_using_analog()
{
	return false;
}

internal void
startup()
{
}

internal void
shutdown()
{
}

// TODO : I rather expose the back_buffer and sound_buffer using getters for access in any function.
internal void
update_and_render( InputState* input, OffscreenBuffer* back_buffer, SoundBuffer* sound_buffer, Memory* memory )
{
	// Graphics & Input Test
	local_persist u32 x_offset = 0;
	local_persist u32 y_offset = 0;

	// Wave Sound Test
	local_persist bool wave_switch = false;

#if 0
	if ( input_using_analog() )
	{
		// TODO(Ed) : Use analog movement tuning
	}
	else
	{
		// TODO(Ed) : Use digital movement tuning
	}
#endif

	EngineState* state = rcast( EngineState*, memory->Persistent );
	do_once_start
		assert( sizeof(EngineState) <= memory->PersistentSize );

		state->ToneVolume = 3000;
		state->WaveToneHz = 262;
		state->WavePeriod = sound_buffer->SamplesPerSecond / state->WaveToneHz;

		state->XOffset = 0;
		state->YOffset = 0;

		#if Build_Debug
		{
			using namespace platform;

			char* file_path = __FILE__;
			Debug_FileContent file_content = debug_file_read_content( file_path );
			if ( file_content.Size )
			{
				debug_file_write_content( "test.out", file_content.Size, file_content.Data );
				debug_file_free_content( & file_content );
			}
		}
		#endif
	do_once_end

	ControllerState* controller = & input->Controllers[0];

	if ( controller->DSPad )
	{
		DualsensePadState* pad = controller->DSPad;

		x_offset += pad->DPad.Right.State;
		x_offset -= pad->DPad.Left.State;
		y_offset += pad->DPad.Down.State;
		y_offset -= pad->DPad.Up.State;

		x_offset += pad->Stick.Left.X.End;
		y_offset += pad->Stick.Left.Y.End;

		if ( pad->Triangle.State )
		{
			state->ToneVolume += 10;
		}
		if ( pad->Circle.State )
		{
			state->ToneVolume -= 10;
		}

		if ( pad->Square.State )
		{
			state->WaveToneHz += 1;
			state->WavePeriod  = sound_buffer->SamplesPerSecond / state->WaveToneHz;
		}
		if ( pad->X.State )
		{
			state->WaveToneHz -= 1;
			state->WavePeriod  = sound_buffer->SamplesPerSecond / state->WaveToneHz;
		}

		if ( pad->Options.State )
		{
			wave_switch ^= true;
		}

		if ( pad->Share.State )
		{
			// TODO(Ed) : Add rumble test
		}
	}
	else if ( controller->XPad )
	{
		XInputPadState* pad = controller->XPad;

		x_offset += pad->DPad.Right.State;
		x_offset -= pad->DPad.Left.State;
		y_offset += pad->DPad.Down.State;
		y_offset -= pad->DPad.Up.State;

		x_offset += pad->Stick.Left.X.End;
		y_offset += pad->Stick.Left.Y.End;

		if ( pad->Y.State )
		{
			state->ToneVolume += 10;
		}
		if ( pad->B.State )
		{
			state->ToneVolume -= 10;
		}

		if ( pad->X.State )
		{
			state->WaveToneHz += 1;
			state->WavePeriod  = sound_buffer->SamplesPerSecond / state->WaveToneHz;
		}
		if ( pad->A.State )
		{
			state->WaveToneHz -= 1;
			state->WavePeriod  = sound_buffer->SamplesPerSecond / state->WaveToneHz;
		}

		if ( pad->Start.State )
		{
			wave_switch ^= true;
		}

		if ( pad->Back.State )
		{
			// TODO(Ed) : Add rumble test
		}
	}

	// TODO(Ed) : Allow sample offsets here for more robust platform options
	if ( ! wave_switch )
		output_sound( state, sound_buffer, sine_wave_sample_value );
	else
		output_sound( state, sound_buffer, square_wave_sample_value );

	render_weird_graident( back_buffer, x_offset, y_offset );
}

NS_ENGINE_END
