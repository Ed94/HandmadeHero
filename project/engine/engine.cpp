//#include "win32.h"
#include "engine.hpp"
#include "engine_to_platform_api.hpp"
#include "handmade.hpp"

NS_ENGINE_BEGIN

#define pressed( btn ) (btn.ended_down && btn.half_transitions > 0)

// Used to determine if analog input is at move threshold
constexpr f32 analog__move_threshold = 0.5f;

struct EngineActions
{
	b32 move_up;
	b32 move_down;
	b32 move_left;
	b32 move_right;

	b32 loop_mode_engine;
	b32 loop_mode_game;

	b32 raise_volume;
	b32 lower_volume;
	b32 raise_tone_hz;
	b32 lower_tone_hz;

	b32 toggle_wave_tone;

#if Build_Development
	b32 pause_renderer;

	b32 set_snapshot_slot_1;
	b32 set_snapshot_slot_2;
	b32 set_snapshot_slot_3;
	b32 set_snapshot_slot_4;
#endif
};

struct EngineState
{
	s32 wave_tone_hz;
	s32 tone_volume;
	s32 x_offset;
	s32 y_offset;

	b32 renderer_paused;

	f32 sample_wave_sine_time;
	b32 sample_wave_switch;

	hh::Memory game_memory;
};

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
output_sound( EngineState* state, AudioBuffer* sound_buffer, GetSoundSampleValueFn* get_sample_value )
{
	s16* sample_out = sound_buffer->samples;
	for ( s32 sample_index = 0; sample_index < sound_buffer->num_samples; ++ sample_index )
	{
		s16 sample_value = get_sample_value( state, sound_buffer );
		sound_buffer->running_sample_index++;

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

internal void
render_player( OffscreenBuffer* buffer, s32 pos_x, s32 pos_y )
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

#if Build_Development
using SnapshotFn = void ( Memory* memory, platform::ModuleAPI* platform_api );

internal
void load_engine_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	platform_api->memory_copy( memory->persistent, memory->total_size()
		, memory->snapshots[ memory->active_snapshot_slot ].memory );
}

internal
void load_game_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	s32          slot  = memory->active_snapshot_slot;
	EngineState* state = rcast( EngineState*, memory->persistent );

	void* persistent_slot = memory->snapshots[ slot ].memory;
	void* transient_slot  = rcast( Byte*, memory->snapshots[ slot ].memory ) + state->game_memory.persistent_size;

	platform_api->memory_copy( state->game_memory.persistent, state->game_memory.persistent_size, persistent_slot );
	platform_api->memory_copy( state->game_memory.transient,  state->game_memory.transient_size,  transient_slot );
}

internal
void take_engine_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	platform_api->memory_copy( memory->snapshots[ memory->active_snapshot_slot ].memory, memory->total_size(), memory->persistent );
}

internal
void take_game_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	s32          slot  = memory->active_snapshot_slot;
	EngineState* state = rcast( EngineState*, memory->persistent );

	void* persistent_slot = memory->snapshots[ slot ].memory;
	void* transient_slot  = rcast( Byte*, memory->snapshots[ slot ].memory ) + state->game_memory.persistent_size;

	platform_api->memory_copy( persistent_slot, state->game_memory.persistent_size, state->game_memory.persistent );
	platform_api->memory_copy( transient_slot,  state->game_memory.transient_size,  state->game_memory.transient );
}

internal
void begin_recording_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	Str file_name = str_ascii("test_input_");
	StrPath file_path = {};
	file_path.concat( platform_api->path_scratch, file_name );
	snprintf( file_path.ptr, file_path.len, "%s%d.hm_replay", file_name.ptr, memory->active_snapshot_slot );

	platform_api->file_delete( memory->active_input_replay_file.path );
	memory->active_input_replay_file.path = file_path;
	memory->replay_mode                   = ReplayMode_Record;
}

internal
void end_recording_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	memory->replay_mode = ReplayMode_Off;
	platform_api->file_close( & memory->active_input_replay_file );
}

internal
void begin_playback_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	Str     file_name = str_ascii("test_input_");
	StrPath file_path = {};
	file_path.concat( platform_api->path_scratch, file_name );
	snprintf( file_path.ptr, file_path.len, "%s%d.hm_replay", file_name.ptr, memory->active_snapshot_slot );

	// TODO(Ed - From Casey): Recording system still seems to take too long
	// on record start - find out what Windows is doing and if
	// we can speed up / defer some of that processing.

	if ( platform_api->file_check_exists( file_path ) )
	{
		memory->active_input_replay_file.path = file_path;
		memory->replay_mode = ReplayMode_Playback;
	}
	else
	{
		// TODO(Ed) : Logging
	}
}

internal
void end_playback_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	memory->replay_mode = ReplayMode_Off;
	platform_api->file_close( & memory->active_input_replay_file );
}

InputStateSnapshot input_state_snapshot( InputState* input )
{
	InputStateSnapshot snapshot = {};
	for ( s32 idx = 0; idx < array_count( snapshot.controllers ); ++ idx )
	{
		ControllerState* controller = & input->controllers[idx];
		if ( controller == nullptr )
			continue;

		if ( controller->ds_pad )
			snapshot.controllers[idx].ds_pad = *controller->ds_pad;

		if ( controller->xpad )
			snapshot.controllers[idx].xpad = *controller->xpad;

		if ( controller->keyboard )
		{
			snapshot.controllers[idx].keyboard = *controller->keyboard;
		}

		if ( controller->mouse )
			snapshot.controllers[idx].mouse = *controller->mouse;
	}
	return snapshot;
}

internal
void record_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	InputStateSnapshot snapshot = input_state_snapshot( input );
	if ( platform_api->file_write_stream( & memory->active_input_replay_file, sizeof(snapshot), &snapshot ) == 0 )
	{
		// TODO(Ed) : Logging
	}
}

internal
void play_input( SnapshotFn* load_snapshot, Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	InputStateSnapshot new_input;
	if ( platform_api->file_read_stream( & memory->active_input_replay_file, sizeof(InputStateSnapshot), & new_input ) == 0 )
	{
		load_snapshot( memory, platform_api );
		platform_api->file_rewind( & memory->active_input_replay_file );
		return;
	}

	for ( s32 idx = 0; idx < array_count( new_input.controllers ); ++ idx )
	{
		ControllerState* controller = & input->controllers[idx];
		if ( controller == nullptr )
			continue;

		if ( controller->ds_pad )
			*controller->ds_pad = new_input.controllers[idx].ds_pad;

		if ( controller->xpad )
			*controller->xpad = new_input.controllers[idx].xpad;

		if ( controller->keyboard )
		{
			*controller->keyboard = new_input.controllers[idx].keyboard;
		}

		if ( controller->mouse )
			*controller->mouse = new_input.controllers[idx].mouse;
	}
}

void process_loop_mode( SnapshotFn* take_snapshot, SnapshotFn* load_snapshot
	, Memory* memory, EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	if ( memory->replay_mode == ReplayMode_Off )
	{
		take_snapshot( memory, platform_api );
		begin_recording_input( memory, input, platform_api );
	}
	else if ( memory->replay_mode == ReplayMode_Playback )
	{
		end_playback_input( memory, input, platform_api );
		load_snapshot( memory, platform_api );
	}
	else if ( memory->replay_mode == ReplayMode_Record )
	{
		end_recording_input( memory, input, platform_api );
		load_snapshot( memory, platform_api );
		begin_playback_input( memory, input, platform_api );
	}
}
// Build_Development
#endif

internal
void input_poll_engine_actions( InputState* input, EngineActions* actions )
{
	ControllerState* controller = & input->controllers[0];
	KeyboardState*   keyboard   = controller->keyboard;

	// actions->move_right |= keyboard->D.EndedDown;
	// actions->move_left  |= keyboard->A.EndedDown;
	// actions->move_up    |= keyboard->W.EndedDown;
	// actions->move_down  |= keyboard->S.EndedDown;

	actions->raise_volume |= keyboard->up.ended_down;
	actions->lower_volume |= keyboard->down.ended_down;

	actions->raise_tone_hz |= keyboard->right.ended_down;
	actions->lower_tone_hz |= keyboard->left.ended_down;

#if Build_Development
	actions->pause_renderer |= pressed( keyboard->pause );

	actions->set_snapshot_slot_1 |= pressed( keyboard->_1 ) && keyboard->right_alt.ended_down;
	actions->set_snapshot_slot_2 |= pressed( keyboard->_2 ) && keyboard->right_alt.ended_down;
	actions->set_snapshot_slot_3 |= pressed( keyboard->_3 ) && keyboard->right_alt.ended_down;
	actions->set_snapshot_slot_4 |= pressed( keyboard->_4 ) && keyboard->right_alt.ended_down;
#endif

	actions->toggle_wave_tone |= pressed( keyboard->Q );

	actions->loop_mode_game   |= pressed( keyboard->L ) && ! keyboard->right_shift.ended_down;
	actions->loop_mode_engine |= pressed( keyboard->L ) &&   keyboard->right_shift.ended_down;

	MousesState* mouse = controller->mouse;

	actions->move_right = (mouse->horizontal_wheel.end > 0.f) * 20;
	actions->move_left  = (mouse->horizontal_wheel.end < 0.f) * 20;

	actions->move_up    = (mouse->vertical_wheel.end > 0.f) * 10;
	actions->move_down  = (mouse->vertical_wheel.end < 0.f) * 10;

}

internal
void input_poll_player_actions( InputState* input, hh::PlayerActions* actions )
{
	ControllerState* controller = & input->controllers[0];

	if ( controller->ds_pad )
	{
		DualsensePadState* pad = controller->ds_pad;

		actions->jump |= pressed( pad->cross );

		actions->player_x_move_analog += pad->stick.left.X.end;
		actions->player_y_move_analog += pad->stick.left.Y.end;
	}
	if ( controller->xpad )
	{
		XInputPadState* pad = controller->xpad;

		actions->jump |= pressed( pad->A );

		actions->player_x_move_analog += pad->stick.left.X.end;
		actions->player_y_move_analog += pad->stick.left.Y.end;
	}

	if ( controller->keyboard )
	{
		KeyboardState* keyboard = controller->keyboard;
		actions->jump |= pressed( keyboard->space );

		actions->player_x_move_digital += keyboard->D.ended_down - keyboard->A.ended_down;
		actions->player_y_move_digital += keyboard->W.ended_down - keyboard->S.ended_down;
	}

	if ( controller->mouse )
	{
		MousesState* mouse = controller->mouse;
	}
}

Engine_API
void on_module_reload( Memory* memory, platform::ModuleAPI* platfom_api )
{

}

Engine_API
void startup( Memory* memory, platform::ModuleAPI* platform_api )
{
#if Build_Development
	memory->active_snapshot_slot     = 1;
	memory->replay_mode		         = ReplayMode_Off;
	memory->active_input_replay_file = {};
	memory->engine_loop_active       = false;
	memory->game_loop_active         = false;
#endif

	for ( s32 slot = 0; slot < memory->Num_Snapshot_Slots; ++ slot )
	{
		// TODO(Ed) : Specify default file paths for saving slots ?
	}

	EngineState* state = rcast( EngineState*, memory->persistent );
	assert( sizeof(EngineState) <= memory->persistent_size );

	state->tone_volume = 1000;

	state->x_offset = 0;
	state->y_offset = 0;

	state->sample_wave_switch = false;
	state->wave_tone_hz = 60;
	state->sample_wave_sine_time = 0.f;

	state->renderer_paused = false;
	state->game_memory.persistent_size = memory->persistent_size / 2;
	state->game_memory.persistent      = rcast(Byte*, memory->persistent) + state->game_memory.persistent_size;
	state->game_memory.transient_size  = memory->transient_size / 2;
	state->game_memory.transient       = rcast(Byte*, memory->transient) + state->game_memory.transient_size;

	hh::PlayerState* player = rcast( hh::PlayerState*, state->game_memory.persistent );
	assert( sizeof(hh::PlayerState) <= state->game_memory.persistent_size );

	player->pos_x     = 100;
	player->pos_y     = 100;
	player->mid_jump  = false;
	player->jump_time = 0.f;
}

Engine_API
void shutdown( Memory* memory, platform::ModuleAPI* platform_api )
{
}

Engine_API
// TODO : I rather expose the back_buffer and sound_buffer using getters for access in any function.
void update_and_render( InputState* input, OffscreenBuffer* back_buffer, Memory* memory, platform::ModuleAPI* platform_api, ThreadContext* thread )
{
	EngineState* state = rcast( EngineState*, memory->persistent );
	assert( sizeof(EngineState) <= memory->persistent_size );

	ControllerState* controller = & input->controllers[0];

	EngineActions     engine_actions {};
	hh::PlayerActions player_actions {};

	input_poll_engine_actions( input, & engine_actions );

#if Build_Development
	// Ease of use: Allow user to press L key without shift if engine loop recording is active.
	engine_actions.loop_mode_engine |= engine_actions.loop_mode_game && memory->engine_loop_active;

	if ( engine_actions.loop_mode_engine && ! memory->game_loop_active )
	{
		process_loop_mode( & take_engine_snapshot, & load_engine_snapshot, memory, state, input, platform_api );
		memory->engine_loop_active = memory->replay_mode > ReplayMode_Off;
	}

	// Input recording and playback for engine state
	if ( memory->engine_loop_active )
	{
		if ( memory->replay_mode == ReplayMode_Record )
		{
			record_input( memory, input, platform_api );
		}
		if ( memory->replay_mode == ReplayMode_Playback )
		{
			play_input( & load_engine_snapshot, memory, input, platform_api );
		}
	}
#endif

	// Process Engine Actions
	{
		state->x_offset += 3 * engine_actions.move_right;
		state->x_offset -= 3 * engine_actions.move_left;
		state->y_offset += 3 * engine_actions.move_down;
		state->y_offset -= 3 * engine_actions.move_up;

		if ( engine_actions.raise_volume )
		{
			state->tone_volume += 10;
		}
		if ( engine_actions.lower_volume )
		{
			state->tone_volume -= 10;
			if ( state->tone_volume <= 0 )
				state->tone_volume = 0;
		}

		if ( engine_actions.raise_tone_hz )
		{
			state->wave_tone_hz += 1;
		}
		if ( engine_actions.lower_tone_hz )
		{
			state->wave_tone_hz -= 1;
			if ( state->wave_tone_hz <= 0 )
				state->wave_tone_hz = 1;
		}

		if ( engine_actions.toggle_wave_tone )
		{
			state->sample_wave_switch ^= true;
		}

		if ( engine_actions.loop_mode_game && ! memory->engine_loop_active )
		{
			process_loop_mode( & take_game_snapshot, & load_game_snapshot, memory, state, input, platform_api );
			memory->game_loop_active = memory->replay_mode > ReplayMode_Off;
		}

	#if Build_Development
		if ( engine_actions.pause_renderer )
		{
			if ( state->renderer_paused )
			{
				platform_api->debug_set_pause_rendering(false);
				state->renderer_paused = false;
			}
			else
			{
				platform_api->debug_set_pause_rendering(true);
				state->renderer_paused = true;
			}
		}

		if ( ! memory->game_loop_active )
		{
			if ( engine_actions.set_snapshot_slot_1 ) memory->active_snapshot_slot = 0;
			if ( engine_actions.set_snapshot_slot_2 ) memory->active_snapshot_slot = 1;
			if ( engine_actions.set_snapshot_slot_3 ) memory->active_snapshot_slot = 2;
			if ( engine_actions.set_snapshot_slot_4 ) memory->active_snapshot_slot = 3;
		}
	#endif
	}

#if Build_Development
	if ( ! memory->engine_loop_active )
	{
		// Input recording and playback for game state
		if ( memory->replay_mode == ReplayMode_Record )
		{
			record_input( memory, input, platform_api );
		}
		if ( memory->replay_mode == ReplayMode_Playback )
		{
			play_input( & load_game_snapshot, memory, input, platform_api );
		}
	}
#endif

	hh::PlayerState* player = rcast( hh::PlayerState*, state->game_memory.persistent );
	assert( sizeof(hh::PlayerState) <= state->game_memory.persistent_size );

	input_poll_player_actions( input, & player_actions );
	{
		player->pos_x += player_actions.player_x_move_digital * 5;
		player->pos_y -= player_actions.player_y_move_digital * 5;
		player->pos_x += scast(u32, player_actions.player_x_move_analog  * 5);
		player->pos_y -= scast(u32, player_actions.player_y_move_analog  * 5) - scast(u32, sinf( player->jump_time * TAU ) * 10);

		if ( player->jump_time > 0.f )
		{
			player->jump_time -= 0.025f;
		}
		else
		{
			player->jump_time = 0.f;
			player->mid_jump  = false;
		}

		if ( ! player->mid_jump && player_actions.jump )
		{
			player->jump_time = 1.f;
			player->mid_jump  = true;
		}
	}

	render_weird_graident( back_buffer, state->x_offset, state->y_offset );
	render_player( back_buffer, player->pos_x, player->pos_y );

#if Build_Development
	if ( memory->replay_mode == ReplayMode_Record )
		render_player( back_buffer, player->pos_x + 20, player->pos_y - 20 );
#endif

	render_player( back_buffer, (s32)input->controllers[0].mouse->X.end, (s32)input->controllers[0].mouse->Y.end );

	// Mouse buttons test
	{
		if ( input->controllers[0].mouse->left.ended_down == true )
			render_player( back_buffer, 5, 5 );

		if ( input->controllers[0].mouse->middle.ended_down == true )

			render_player( back_buffer, 5, 20 );

		if ( input->controllers[0].mouse->right.ended_down == true )
			render_player( back_buffer, 5, 35 );
	}
}

Engine_API
void update_audio( AudioBuffer* audio_buffer, Memory* memory, platform::ModuleAPI* platform_api, ThreadContext* thread )
{
	EngineState* state = rcast( EngineState*, memory->persistent );
	do_once_start

	do_once_end

	// TODO(Ed) : Allow sample offsets here for more robust platform options
	if ( ! state->sample_wave_switch )
		output_sound( state, audio_buffer, sine_wave_sample_value );
	else
		output_sound( state, audio_buffer, square_wave_sample_value );
}

NS_ENGINE_END

#undef pressed
