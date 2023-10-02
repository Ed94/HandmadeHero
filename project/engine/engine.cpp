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

	b32 load_auto_snapshot;
	b32 set_snapshot_slot_1;
	b32 set_snapshot_slot_2;
	b32 set_snapshot_slot_3;
	b32 set_snapshot_slot_4;

	b32 force_null_access_violation;
#endif
};

struct EngineState
{
	f32 auto_snapshot_interval;
	f32 auto_snapshot_timer;

	s32 wave_tone_hz;
	s32 tone_volume;
	s32 x_offset;
	s32 y_offset;

	b32 renderer_paused;

	f32 sample_wave_sine_time;
	b32 sample_wave_switch;

	hh::Memory game_memory;
};

#include "test_samples.cpp"

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
	memory->snapshots[ memory->active_snapshot_slot ].age = platform_api->get_wall_clock();
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
	memory->snapshots[ memory->active_snapshot_slot ].age = platform_api->get_wall_clock();
}

internal
void begin_recording_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	Str file_name = str_ascii("test_input_");
	StrPath file_path = {};
	file_path.concat( platform_api->path_scratch, file_name );
	snprintf( file_path.ptr, file_path.len, "%s%d.hm_replay", file_name.ptr, memory->active_snapshot_slot );

	memory->active_input_replay_file.path = file_path;
	platform_api->file_delete( memory->active_input_replay_file.path );
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

	actions->loop_mode_game   |= pressed( keyboard->L ) && ! keyboard->right_shift.ended_down && ! keyboard->right_alt.ended_down;
	actions->loop_mode_engine |= pressed( keyboard->L ) &&   keyboard->right_shift.ended_down;

	MousesState* mouse = controller->mouse;

	actions->move_right = (mouse->horizontal_wheel.end > 0.f) * 20;
	actions->move_left  = (mouse->horizontal_wheel.end < 0.f) * 20;

	actions->move_up    = (mouse->vertical_wheel.end > 0.f) * 10;
	actions->move_down  = (mouse->vertical_wheel.end < 0.f) * 10;

	actions->load_auto_snapshot |= pressed( keyboard->L ) && keyboard->right_alt.ended_down;
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
		// d ms_timing_debug );

		*sample_out = sample_value;
		++ sample_out;

		*sample_out = sample_value;
		++ sample_out;
	}
}

s32 round_f32_to_s32( f32 value )
{
	// TODO(Ed) : Casey wants to use an intrinsic
	return scast(s32, value + 0.5f);
}

internal
void draw_rectangle( OffscreenBuffer* buffer
	, f32 min_x, f32 min_y
	, f32 max_x, f32 max_y
	, f32 red, f32 green, f32 blue  )
{
	s32 min_x_32 = round_f32_to_s32( min_x );
	s32 min_y_32 = round_f32_to_s32( min_y );
	s32 max_x_32 = round_f32_to_s32( max_x );
	s32 max_y_32 = round_f32_to_s32( max_y );

	s32 buffer_width  = buffer->width;
	s32 buffer_height = buffer->height;

	if ( min_x_32 < 0 )
		min_x_32 = 0;
	if ( min_y_32 < 0 )
		min_y_32 = 0;
	if ( max_x_32 > buffer_width )
		max_x_32 = buffer_width;
	if ( max_y_32 > buffer_height )
		max_y_32 = buffer_height;

	s32 red_32   = round_f32_to_s32( 255.f * red );
	s32 green_32 = round_f32_to_s32( 255.f * green );
	s32 blue_32  = round_f32_to_s32( 255.f * blue );

	s32 color =
			(scast(s32, red_32)   << 16)
		|	(scast(s32, green_32) << 8)
		|	(scast(s32, blue_32)  << 0);

	// Start with the pixel on the top left corner of the rectangle
	u8* row = rcast(u8*, buffer->memory )
		+ min_x_32 * buffer->bytes_per_pixel
		+ min_y_32 * buffer->pitch;

	for ( s32 y = min_y_32; y < max_y_32; ++ y )
	{
		s32* pixel_32 = rcast(s32*, row);

		for ( s32 x = min_x_32; x < max_x_32; ++ x )
		{
			*pixel_32 = color;
			pixel_32++;
		}
		row += buffer->pitch;
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

#if 0
	for ( s32 slot = 0; slot < memory->Num_Snapshot_Slots; ++ slot )
	{
		// TODO(Ed) : Specify default file paths for saving slots ?
	}
#endif

	EngineState* state = rcast( EngineState*, memory->persistent );
	assert( sizeof(EngineState) <= memory->persistent_size );

	state->auto_snapshot_interval = 10.f;

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

	player->pos_x     = 920;
	player->pos_y     = 466;
	player->mid_jump  = false;
	player->jump_time = 0.f;
}

Engine_API
void shutdown( Memory* memory, platform::ModuleAPI* platform_api )
{
}

Engine_API
// TODO : I rather expose the back_buffer and sound_buffer using getters for access in any function.
void update_and_render( f32 delta_time, InputState* input, OffscreenBuffer* back_buffer, Memory* memory, platform::ModuleAPI* platform_api, ThreadContext* thread )
{
	EngineState* state = rcast( EngineState*, memory->persistent );
	assert( sizeof(EngineState) <= memory->persistent_size );

	// Engine auto_snapshot
	{
		state->auto_snapshot_timer += delta_time;
		if ( state->auto_snapshot_timer >= state->auto_snapshot_interval )
		{
			state->auto_snapshot_timer = 0.f;

			s32 current_slot = memory->active_snapshot_slot;
			memory->active_snapshot_slot = 0;

			take_engine_snapshot( memory, platform_api );
			memory->active_snapshot_slot = current_slot;
			state->auto_snapshot_timer = 0.f;
		}
	}

	ControllerState* controller = & input->controllers[0];

	EngineActions     engine_actions {};
	hh::PlayerActions player_actions {};

	input_poll_engine_actions( input, & engine_actions );

	if ( engine_actions.load_auto_snapshot )
	{
		s32 current_slot = memory->active_snapshot_slot;
		memory->active_snapshot_slot = 0;
		load_engine_snapshot( memory, platform_api );
		memory->active_snapshot_slot = current_slot;
	}

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
			if ( engine_actions.set_snapshot_slot_1 ) memory->active_snapshot_slot = 1;
			if ( engine_actions.set_snapshot_slot_2 ) memory->active_snapshot_slot = 2;
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
		f32 move_speed = 200.f;

		player->pos_x += scast(f32, player_actions.player_x_move_digital) * delta_time * move_speed;
		player->pos_y -= scast(f32, player_actions.player_y_move_digital) * delta_time * move_speed;

		player->pos_x += scast(f32, player_actions.player_x_move_analog  * delta_time * move_speed);
		player->pos_y -= scast(f32, player_actions.player_y_move_analog  * delta_time * move_speed);
		player->pos_y += sinf( player->jump_time * TAU ) * 200.f * delta_time;

		if ( player->jump_time > 0.f )
		{
			player->jump_time -= delta_time;
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

	f32 x_offset_f = scast(f32, state->x_offset);
	f32 y_offset_f = scast(f32, state->y_offset);

	draw_rectangle( back_buffer
		, 0.f, 0.f
		, scast(f32, back_buffer->width), scast(f32, back_buffer->height)
		, 1.f, 0.24f, 0.24f );

	// Draw tilemap
	u32 tilemap[9][16] = {
		{ 1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 0, 1,  1, 1, 1, 1 },
		{ 1, 1, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
		{ 1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1,  0, 0, 0, 1 },
		{ 1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1,  0, 0, 0, 1 },
		{ 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0 },
		{ 1, 1, 0, 0,  0, 1, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
		{ 1, 1, 0, 0,  0, 1, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1 },
		{ 1, 0, 0, 0,  0, 0, 0, 0,  0, 1, 0, 0,  0, 0, 0, 1 },
		{ 1, 1, 1, 1,  1, 1, 0, 1,  1, 1, 1, 1,  1, 1, 1, 1 },
	};

	f32 upper_left_x = 0;
	f32 upper_left_y = 0;

	f32 tile_width   = 119;
	f32 tile_height  = 116;

	for ( s32 row = 0; row < 9; ++ row )
	{
		for ( s32 col = 0; col < 16; ++ col )
		{
			u32 tileID = tilemap[row][col];
			f32 grey[3] = { 0.15f, 0.15f, 0.15f };

			if ( tileID == 1 )
			{
				grey[0] = 0.22f;
				grey[1] = 0.22f;
				grey[2] = 0.22f;
			}

			f32 min_x = upper_left_x + scast(f32, col) * tile_width;
			f32 min_y = upper_left_y + scast(f32, row) * tile_height;
			f32 max_x = min_x + tile_width;
			f32 max_y = min_y + tile_height;

			draw_rectangle( back_buffer
				, min_x, min_y
				, max_x, max_y
				, grey[0], grey[1], grey[2] );
		}
	}

	// Player
	f32 player_width  = 50.f;
	f32 player_height = 100.f;

	f32 player_red   = 0.3f;
	f32 player_green = 0.3f;
	f32 player_blue  = 0.3f;

	draw_rectangle( back_buffer
		, player->pos_x, player->pos_y
		, player->pos_x + player_width, player->pos_y + player_height
		, player_red, player_green, player_blue );

	// Auto-Snapshot percent bar
	if (1)
	{
		f32 snapshot_percent_x = ((state->auto_snapshot_timer / state->auto_snapshot_interval)) * (f32)back_buffer->width / 4.f;
		draw_rectangle( back_buffer
			, 0.f, 0.f
			, snapshot_percent_x, 10.f
			, 0x00, 0.15f, 0.35f );
	}

#if Build_Development
	if ( memory->replay_mode == ReplayMode_Record )
	{
		draw_rectangle( back_buffer
			, scast(f32, player->pos_x) + 50.f, scast(f32, player->pos_y) - 50.f
			, scast(f32, player->pos_x) + 10.f, scast(f32, player->pos_y) + 40.f
			, 1.f, 1.f, 1.f );
	}
#endif

	// Change above to use draw rectangle
	draw_rectangle( back_buffer
		, (f32)input->controllers[0].mouse->X.end, (f32)input->controllers[0].mouse->Y.end
		, (f32)input->controllers[0].mouse->X.end + 10.f, (f32)input->controllers[0].mouse->Y.end + 10.f
		, 1.f, 1.f, 0.f );

	// Mouse buttons test
	#if 0
	{
		if ( input->controllers[0].mouse->left.ended_down == true )
			render_player( back_buffer, 5, 5 );

		if ( input->controllers[0].mouse->middle.ended_down == true )

			render_player( back_buffer, 5, 20 );

		if ( input->controllers[0].mouse->right.ended_down == true )
			render_player( back_buffer, 5, 35 );
	}
	#endif
}

Engine_API
void update_audio( f32 delta_time, AudioBuffer* audio_buffer, Memory* memory, platform::ModuleAPI* platform_api, ThreadContext* thread )
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
