//#include "win32.h"
#include "engine.hpp"
#include "engine_to_platform_api.hpp"
#include "handmade.hpp"

NS_ENGINE_BEGIN

#define pressed( btn ) (btn.EndedDown && btn.HalfTransitions == 1)

// Used to determine if analog input is at move threshold
constexpr f32 analog__move_threshold = 0.5f;

struct EngineActions
{
	b32 move_up    = false;
	b32 move_down  = false;
	b32 move_left  = false;
	b32 move_right = false;

	b32 loop_mode = false;

	b32 raise_volume  = false;
	b32 lower_volume  = false;
	b32 raise_tone_hz = false;
	b32 lower_tone_hz = false;

	b32 toggle_wave_tone = false;

#if Build_Development
	b32 pause_renderer  = false;
#endif
};

struct PlayerActions
{
	s32 player_x_move_digital = 0;
	s32 player_y_move_digital = 0;
	f32 player_x_move_analog  = 0;
	f32 player_y_move_analog  = 0;

	b32 jump = false;
};

struct EngineState
{
	s32 WaveToneHz;
	s32 ToneVolume;
	s32 XOffset;
	s32 YOffset;

	b32 RendererPaused;

	f32 SampleWaveSineTime;
	b32 SampleWaveSwitch;

	s32 InputRecordingIndex;
	s32 InputPlayingIndex;

	platform::File ActiveInputRecordingFile;
	platform::File ActivePlaybackFile;

	hh::Memory game_memory;
};


using GetSoundSampleValueFn = s16( EngineState* state, AudioBuffer* sound_buffer );

internal s16
square_wave_sample_value( EngineState* state, AudioBuffer* sound_buffer )
{
	s32 wave_period = sound_buffer->SamplesPerSecond / state->WaveToneHz;

	s32 sample_value = (sound_buffer->RunningSampleIndex /  (wave_period / 2) ) % 2 ?
		state->ToneVolume : - state->ToneVolume;

	return scast(s16, sample_value);
}

internal s16
sine_wave_sample_value( EngineState* state, AudioBuffer* sound_buffer )
{
	f32& time = state->SampleWaveSineTime;

	s32 wave_period = sound_buffer->SamplesPerSecond / state->WaveToneHz;

	// time =  TAU * (f32)sound_buffer->RunningSampleIndex / (f32)SoundTest_WavePeriod;
	f32 sine_value   = sinf( time );
	s16 sample_value = scast(s16, sine_value * scast(f32, state->ToneVolume));

	time += TAU * 1.0f / scast(f32, wave_period );
	if ( time > TAU )
	{
		time -= TAU;
	}
	return sample_value;
}

internal void
output_sound( EngineState* state, AudioBuffer* sound_buffer, GetSoundSampleValueFn* get_sample_value )
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
			u8 red   = scast(u8, y + x_offset);
			u8 green = scast(u8, x + y_offset);
			u8 blue  = scast(u8, x + y - x_offset - y_offset);
			//    blue *= 2;
		#endif

			*pixel++ = u32(red/2 << 16) | u32(green/6 << 8) | blue/2 << 0;
		}
		wildcard += 0.5375f;
		row += buffer->Pitch;
	}
}

internal void
render_player( OffscreenBuffer* buffer, s32 pos_x, s32 pos_y )
{
	u8* end_of_buffer = rcast(u8*, buffer->Memory)
		- buffer->BytesPerPixel * buffer->Width
		+ buffer->Pitch * buffer->Height;

	s32 top    = pos_y;
	s32 bottom = pos_y + 10;

	u32 color = 0xFFFFFFFF;

	for ( s32 coord_x = pos_x; coord_x < (pos_x+ 10); ++ coord_x )
	{
		u8*
		pixel_byte  = rcast(u8*, buffer->Memory);
		pixel_byte += coord_x * buffer->BytesPerPixel;
		pixel_byte += top     * buffer->Pitch;

		for ( s32 coord_y = top; coord_y < bottom; ++ coord_y )
		{
			if ( pixel_byte < buffer->Memory || pixel_byte >= end_of_buffer )
				continue;

			s32* pixel = rcast(s32*, pixel_byte);
			*pixel = color;

			pixel_byte += buffer->Pitch;
		}
	}
}

internal
void begin_recording_input( EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	Str file_name = str_ascii("test_input.hmi");
	StrPath file_path = {};
	file_path.concat( platform_api->path_scratch, file_name );

	state->ActiveInputRecordingFile.Path = file_path;
	state->InputRecordingIndex = 1;

	// TODO(Ed) : If game persist memory is larger than 4 gb, this will need to be done in chunks...
	platform_api->file_write_content( & state->ActiveInputRecordingFile, scast(u32, state->game_memory.PersistentSize), state->game_memory.Persistent );
}

internal
void end_recording_input( EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	platform_api->file_close( & state->ActiveInputRecordingFile );
	state->InputRecordingIndex = 0;
}

internal
void begin_playback_input( EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	Str file_name = str_ascii("test_input.hmi");
	StrPath file_path = {};
	file_path.concat( platform_api->path_scratch, file_name );
	if ( platform_api->file_check_exists( file_path ) )
	{
		state->ActivePlaybackFile.Path = file_path;
		state->InputPlayingIndex = 1;
	}

	if ( state->ActiveInputRecordingFile.OpaqueHandle == nullptr )
	{
		platform_api->file_read_stream( & state->ActivePlaybackFile, scast(u32, state->game_memory.PersistentSize), state->game_memory.Persistent );
	}
}

internal
void end_playback_input( EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	platform_api->file_rewind( & state->ActivePlaybackFile );
	platform_api->file_read_stream( & state->ActivePlaybackFile, scast(u32, state->game_memory.PersistentSize), state->game_memory.Persistent );
	platform_api->file_close( & state->ActivePlaybackFile );

	state->InputPlayingIndex = 0;
}

InputStateSnapshot input_state_snapshot( InputState* input )
{
	InputStateSnapshot snapshot = {};
	for ( s32 idx = 0; idx < array_count( snapshot.Controllers ); ++ idx )
	{
		ControllerState* controller = & input->Controllers[idx];
		if ( controller == nullptr )
			continue;

		if ( controller->DSPad )
			snapshot.Controllers[idx].DSPad = *controller->DSPad;

		if ( controller->XPad )
			snapshot.Controllers[idx].XPad = *controller->XPad;

		if ( controller->Keyboard )
		{
			snapshot.Controllers[idx].Keyboard = *controller->Keyboard;
		}

		if ( controller->Mouse )
			snapshot.Controllers[idx].Mouse = *controller->Mouse;
	}
	return snapshot;
}

internal
void record_input( EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	InputStateSnapshot snapshot = input_state_snapshot( input );
	if ( platform_api->file_write_stream( & state->ActiveInputRecordingFile, sizeof(snapshot), &snapshot ) == 0 )
	{
		// TODO(Ed) : Logging
	}
}

internal
void play_input( EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	InputStateSnapshot new_input;
	if ( platform_api->file_read_stream( & state->ActivePlaybackFile, sizeof(InputStateSnapshot), & new_input ) == 0 )
	{
		end_playback_input( state, input, platform_api );
		begin_playback_input( state, input, platform_api );
		return;
	}

	for ( s32 idx = 0; idx < array_count( new_input.Controllers ); ++ idx )
	{
		ControllerState* controller = & input->Controllers[idx];
		if ( controller == nullptr )
			continue;

		if ( controller->DSPad )
			*controller->DSPad = new_input.Controllers[idx].DSPad;

		if ( controller->XPad )
			*controller->XPad = new_input.Controllers[idx].XPad;

		if ( controller->Keyboard )
		{
			*controller->Keyboard = new_input.Controllers[idx].Keyboard;
		}

		if ( controller->Mouse )
			*controller->Mouse = new_input.Controllers[idx].Mouse;
	}
}

internal
void input_poll_engine_actions( InputState* input, EngineActions* actions )
{
	ControllerState* controller = & input->Controllers[0];
	KeyboardState* keyboard = controller->Keyboard;

	actions->move_right |= keyboard->D.EndedDown;
	actions->move_left  |= keyboard->A.EndedDown;
	actions->move_up    |= keyboard->W.EndedDown;
	actions->move_down  |= keyboard->S.EndedDown;

	actions->raise_volume |= keyboard->Up.EndedDown;
	actions->lower_volume |= keyboard->Down.EndedDown;

	actions->raise_tone_hz |= keyboard->Right.EndedDown;
	actions->lower_tone_hz |= keyboard->Left.EndedDown;

#if Build_Development
	actions->pause_renderer |= pressed( keyboard->Pause );
#endif

	actions->toggle_wave_tone |= pressed( keyboard->Q );

	actions->loop_mode |= pressed( keyboard->L );
}

internal
void input_poll_player_actions( InputState* input, PlayerActions* actions )
{
	ControllerState* controller = & input->Controllers[0];

	if ( controller->DSPad )
	{
		DualsensePadState* pad = controller->DSPad;

		actions->jump |= pressed( pad->X );

		actions->player_x_move_analog += pad->Stick.Left.X.End;
		actions->player_y_move_analog += pad->Stick.Left.Y.End;
	}
	if ( controller->XPad )
	{
		XInputPadState* pad = controller->XPad;

		actions->jump |= pressed( pad->A );

		actions->player_x_move_analog += pad->Stick.Left.X.End;
		actions->player_y_move_analog += pad->Stick.Left.Y.End;
	}

	if ( controller->Keyboard )
	{
		KeyboardState* keyboard = controller->Keyboard;
		actions->jump |= pressed( keyboard->Space );

		actions->player_x_move_digital += keyboard->D.EndedDown - keyboard->A.EndedDown;
		actions->player_y_move_digital += keyboard->W.EndedDown - keyboard->S.EndedDown;
	}
}

Engine_API
void on_module_reload( Memory* memory, platform::ModuleAPI* platfom_api )
{

}

Engine_API
void startup( Memory* memory, platform::ModuleAPI* platform_api )
{
	EngineState* state = rcast( EngineState*, memory->Persistent );
	assert( sizeof(EngineState) <= memory->PersistentSize );

	state->ToneVolume = 1000;

	state->XOffset = 0;
	state->YOffset = 0;

	state->SampleWaveSwitch = false;
	state->WaveToneHz = 60;
	state->SampleWaveSineTime = 0.f;

	state->RendererPaused = false;

	state->InputRecordingIndex = 0;
	state->InputPlayingIndex   = 0;

	state->ActiveInputRecordingFile = {};
	state->ActivePlaybackFile = {};

	state->game_memory.PersistentSize = memory->PersistentSize / 2;
	state->game_memory.Persistent     = rcast(Byte*, memory->Persistent) + state->game_memory.PersistentSize;
	state->game_memory.TransientSize  = memory->TransientSize / 2;
	state->game_memory.Transient      = rcast(Byte*, memory->Transient) + state->game_memory.TransientSize;

	hh::PlayerState* player = rcast( hh::PlayerState*, state->game_memory.Persistent );
	assert( sizeof(hh::PlayerState) <= state->game_memory.PersistentSize );

	player->Pos_X = 100;
	player->Pos_Y = 100;
	player->MidJump = false;
}

Engine_API
void shutdown( Memory* memory, platform::ModuleAPI* platform_api )
{
}

Engine_API
// TODO : I rather expose the back_buffer and sound_buffer using getters for access in any function.
void update_and_render( InputState* input, OffscreenBuffer* back_buffer, Memory* memory, platform::ModuleAPI* platform_api )
{
	EngineState* state = rcast( EngineState*, memory->Persistent );
	assert( sizeof(EngineState) <= memory->PersistentSize );

	ControllerState* controller = & input->Controllers[0];

	EngineActions engine_actions {};
	PlayerActions player_actions {};

	input_poll_engine_actions( input, & engine_actions );
	{
		state->XOffset += 3 * engine_actions.move_right;
		state->XOffset -= 3 * engine_actions.move_left;
		state->YOffset += 3 * engine_actions.move_down;
		state->YOffset -= 3 * engine_actions.move_up;

		if ( engine_actions.raise_volume )
		{
			state->ToneVolume += 10;
		}
		if ( engine_actions.lower_volume )
		{
			state->ToneVolume -= 10;
			if ( state->ToneVolume <= 0 )
				state->ToneVolume = 0;
		}

		if ( engine_actions.raise_tone_hz )
		{
			state->WaveToneHz += 1;
		}
		if ( engine_actions.lower_tone_hz )
		{
			state->WaveToneHz -= 1;
			if ( state->WaveToneHz <= 0 )
				state->WaveToneHz = 1;
		}

		if ( engine_actions.toggle_wave_tone )
		{
			state->SampleWaveSwitch ^= true;
		}

		if ( engine_actions.loop_mode )
		{
			if ( state->InputRecordingIndex == 0 && state->InputPlayingIndex == 0 )
			{
				begin_recording_input( state, input, platform_api );
			}
			else if ( state->InputPlayingIndex )
			{
				end_playback_input( state, input, platform_api );
			}
			else if ( state->InputRecordingIndex )
			{
				end_recording_input( state, input, platform_api );
				begin_playback_input( state, input, platform_api );
			}
		}

	#if Build_Development
		if ( engine_actions.pause_renderer )
		{
			if ( state->RendererPaused )
			{
				platform_api->debug_set_pause_rendering(false);
				state->RendererPaused = false;
			}
			else
			{
				platform_api->debug_set_pause_rendering(true);
				state->RendererPaused = true;
			}
		}
	#endif
	}

	if ( state->InputRecordingIndex )
	{
		record_input( state, input, platform_api );
	}
	if ( state->InputPlayingIndex )
	{
		play_input( state, input, platform_api );
	}

	hh::PlayerState* player = rcast( hh::PlayerState*, state->game_memory.Persistent );
	assert( sizeof(hh::PlayerState) <= state->game_memory.PersistentSize );

	input_poll_player_actions( input, & player_actions );
	{
		player->Pos_X += player_actions.player_x_move_digital * 5;
		player->Pos_Y -= player_actions.player_y_move_digital * 5;
		player->Pos_X += scast(u32, player_actions.player_x_move_analog  * 5);
		player->Pos_Y -= scast(u32, player_actions.player_y_move_analog  * 5) - scast(u32, sinf( player->JumpTime * TAU ) * 10);

		if ( player->JumpTime > 0.f )
		{
			player->JumpTime -= 0.025f;
		}
		else
		{
			player->JumpTime = 0.f;
			player->MidJump = false;
		}

		if ( ! player->MidJump && player_actions.jump )
		{
			player->JumpTime = 1.f;
			player->MidJump = true;
		}
	}

	render_weird_graident( back_buffer, 0, 0 );
	render_player( back_buffer, player->Pos_X, player->Pos_Y );

	if ( state->InputRecordingIndex )
		render_player( back_buffer, player->Pos_X + 20, player->Pos_Y - 20 );
}

Engine_API
void update_audio( AudioBuffer* audio_buffer, Memory* memory, platform::ModuleAPI* platform_api )
{
	EngineState* state = rcast( EngineState*, memory->Persistent );
	do_once_start

	do_once_end

	// TODO(Ed) : Allow sample offsets here for more robust platform options
	if ( ! state->SampleWaveSwitch )
		output_sound( state, audio_buffer, sine_wave_sample_value );
	else
		output_sound( state, audio_buffer, square_wave_sample_value );
}

NS_ENGINE_END

#undef pressed
