#include "engine.h"
//#include "win32.h"

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
	s32 sample_value = (sound_buffer->RunningSampleIndex /  (state->WavePeriod / 2) ) % 2 ?
		state->ToneVolume : - state->ToneVolume;

	return scast(s16, sample_value);
}

internal s16
sine_wave_sample_value( EngineState* state, SoundBuffer* sound_buffer )
{
	local_persist f32 time = 0.f;

	// time =  TAU * (f32)sound_buffer->RunningSampleIndex / (f32)SoundTest_WavePeriod;
	f32 sine_value   = sinf( time );
	s16 sample_value = scast(s16, sine_value * scast(f32, state->ToneVolume));

	time += TAU * 1.0f / scast(f32, state->WavePeriod );
	return sample_value;
}

internal void
output_sound( EngineState* state, SoundBuffer* sound_buffer, GetSoundSampleValueFn* get_sample_value )
{
	s16* sample_out = sound_buffer->Samples;
	for ( s32 sample_index = 0; sample_index < sound_buffer->NumSamples; ++ sample_index )
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


			*pixel++ = u32(red << 16) | u32(green << 8) | blue;
		}
		wildcard += 0.5375f;
		row += buffer->Pitch;
	}
}

b32 input_using_analog()
{
	return false;
}

void startup()
{
}

void shutdown()
{
}

// TODO : I rather expose the back_buffer and sound_buffer using getters for access in any function.
void update_and_render( InputState* input, OffscreenBuffer* back_buffer, SoundBuffer* sound_buffer, Memory* memory )
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

		state->ToneVolume = 1000;
		state->WaveToneHz = 120;
		state->WavePeriod = sound_buffer->SamplesPerSecond / state->WaveToneHz;

		state->XOffset = 0;
		state->YOffset = 0;

		#if Build_Debug && 0
		{
			using namespace platform;

			char const* file_path = __FILE__;
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

	// Abstracting the actionables as booleans and processing within this scope
	// for now until proper callbacks for input bindings are setup.
	b32 move_up    = false;
	b32 move_down  = false;
	b32 move_left  = false;
	b32 move_right = false;

	b32 action_up    = false;
	b32 action_down  = false;
	b32 action_left  = false;
	b32 action_right = false;

	b32 raise_volume  = false;
	b32 lower_volume  = false;
	b32 raise_tone_hz = false;
	b32 lower_tone_hz = false;

	b32 toggle_wave_tone = false;

	f32 analog_threshold = 0.5f;

	if ( controller->DSPad )
	{
		DualsensePadState* pad = controller->DSPad;

		move_right |= pad->DPad.Right.EndedDown || pad->Stick.Left.X.End >  analog_threshold;
		move_left  |= pad->DPad.Left.EndedDown  || pad->Stick.Left.X.End < -analog_threshold;
		move_up    |= pad->DPad.Up.EndedDown    || pad->Stick.Left.Y.End >  analog_threshold;
		move_down  |= pad->DPad.Down.EndedDown  || pad->Stick.Left.Y.End < -analog_threshold;

		raise_volume |= pad->Triangle.EndedDown;
		lower_volume |= pad->Circle.EndedDown;

		raise_tone_hz |= pad->Square.EndedDown;
		lower_tone_hz |= pad->X.EndedDown;

		toggle_wave_tone |= pad->Options.EndedDown;
	}
	if ( controller->XPad )
	{
		XInputPadState* pad = controller->XPad;

		move_right |= pad->DPad.Right.EndedDown || pad->Stick.Left.X.End >  analog_threshold;
		move_left  |= pad->DPad.Left.EndedDown  || pad->Stick.Left.X.End < -analog_threshold;
		move_up    |= pad->DPad.Up.EndedDown    || pad->Stick.Left.Y.End >  analog_threshold;
		move_down  |= pad->DPad.Down.EndedDown  || pad->Stick.Left.Y.End < -analog_threshold;

		raise_volume |= pad->Y.EndedDown;
		lower_volume |= pad->B.EndedDown;

		raise_tone_hz |= pad->X.EndedDown;
		lower_tone_hz |= pad->A.EndedDown;

		toggle_wave_tone |= pad->Start.EndedDown;
	}
	if ( controller->Keyboard )
	{
		KeyboardState* keyboard = controller->Keyboard;

		move_right |= keyboard->D.EndedDown;
		move_left  |= keyboard->A.EndedDown;
		move_up    |= keyboard->W.EndedDown;
		move_down  |= keyboard->S.EndedDown;

		raise_volume |= keyboard->Up.EndedDown;
		lower_volume |= keyboard->Down.EndedDown;

		raise_tone_hz |= keyboard->Right.EndedDown;
		lower_tone_hz |= keyboard->Left.EndedDown;

		toggle_wave_tone |= keyboard->Space.EndedDown;
	}

	x_offset += 3 * move_right;
	x_offset -= 3 * move_left;
	y_offset += 3 * move_down;
	y_offset -= 3 * move_up;

	if ( raise_volume )
	{
		state->ToneVolume += 10;
	}
	if ( lower_volume )
	{
		state->ToneVolume -= 10;
	}

	if ( raise_tone_hz )
	{
		state->WaveToneHz += 1;
		state->WavePeriod  = sound_buffer->SamplesPerSecond / state->WaveToneHz;
	}
	if ( lower_tone_hz )
	{
		state->WaveToneHz -= 1;
		state->WavePeriod  = sound_buffer->SamplesPerSecond / state->WaveToneHz;
	}

	if ( toggle_wave_tone )
	{
		wave_switch ^= true;
	}

	// TODO(Ed) : Allow sample offsets here for more robust platform options
	if ( ! wave_switch )
		output_sound( state, sound_buffer, sine_wave_sample_value );
	else
		output_sound( state, sound_buffer, square_wave_sample_value );

	render_weird_graident( back_buffer, x_offset, y_offset );
}

NS_ENGINE_END

