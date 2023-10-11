#if INTELLISENSE_DIRECTIVES
#include "engine.hpp"
#include "input.hpp"
#include "engine_to_platform_api.hpp"

// TODO(Ed) : This needs to be moved out eventually
#include "handmade.hpp"

#include "tile_map.cpp"
#endif

NS_ENGINE_BEGIN

#define pressed( btn ) (btn.ended_down && btn.half_transitions > 0)

// Used to determine if analog input is at move threshold
constexpr f32 analog_move_threshold = 0.5f;

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
	hh::Memory game_memory;
	
	MemoryArena world_arena;
	World* world;
	
	f32 auto_snapshot_interval;
	f32 auto_snapshot_timer;

	s32 wave_tone_hz;
	s32 tone_volume;
	s32 x_offset;
	s32 y_offset;

	b32 renderer_paused;

	f32 sample_wave_sine_time;
	b32 sample_wave_switch;
};

NS_ENGINE_END

// TODO(Ed) : does this need to be here or can it be moved to handmade_engine.cpp?
#include "test_samples.cpp"

#if Build_Development
// TODO(Ed) : Do a proper header/src pair for this
#include "state_and_replay.cpp"
// Build_Development
#endif

NS_ENGINE_BEGIN

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

		actions->sprint |= pressed( pad->circle );
		actions->jump   |= pressed( pad->cross );

		actions->player_x_move_analog += pad->stick.left.X.end;
		actions->player_y_move_analog += pad->stick.left.Y.end;
	}
	if ( controller->xpad )
	{
		XInputPadState* pad = controller->xpad;

		actions->sprint |= pressed( pad->B );
		actions->jump   |= pressed( pad->A );

		actions->player_x_move_analog += pad->stick.left.X.end;
		actions->player_y_move_analog += pad->stick.left.Y.end;
	}

	if ( controller->keyboard )
	{
		KeyboardState* keyboard = controller->keyboard;
		actions->jump   |= pressed( keyboard->space );
		actions->sprint |= keyboard->left_shift.ended_down;

		actions->player_x_move_digital += keyboard->D.ended_down - keyboard->A.ended_down;
		actions->player_y_move_digital += keyboard->W.ended_down - keyboard->S.ended_down;
	}

	if ( controller->mouse )
	{
		MousesState* mouse = controller->mouse;
	}
}

internal 
void output_sound( EngineState* state, AudioBuffer* sound_buffer, GetSoundSampleValueFn* get_sample_value )
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

internal
void draw_rectangle( OffscreenBuffer* buffer
	, f32 min_x, f32 min_y
	, f32 max_x, f32 max_y
	, f32 red, f32 green, f32 blue  )
{
	s32 min_x_32 = round( min_x );
	s32 min_y_32 = round( min_y );
	s32 max_x_32 = round( max_x );
	s32 max_y_32 = round( max_y );

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

	s32 red_32   = round( 255.f * red );
	s32 green_32 = round( 255.f * green );
	s32 blue_32  = round( 255.f * blue );

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

inline
void draw_debug_point(OffscreenBuffer* back_buffer, World* world, TileMapPosition pos, f32 red, f32 green, f32 blue)
{
	TileMap* tile_map = world->tile_map;

	draw_rectangle(back_buffer,
		pos.x          * tile_map->tile_meters_to_pixels + world->tile_lower_left_x + scast(f32, pos.tile_x * tile_map->tile_size_in_pixels),
		pos.y          * tile_map->tile_meters_to_pixels + world->tile_lower_left_y + scast(f32, pos.tile_y * tile_map->tile_size_in_pixels),
		(pos.x + 0.1f) * tile_map->tile_meters_to_pixels + world->tile_lower_left_x + scast(f32, pos.tile_x * tile_map->tile_size_in_pixels),
		(pos.y + 0.1f) * tile_map->tile_meters_to_pixels + world->tile_lower_left_y + scast(f32, pos.tile_y * tile_map->tile_size_in_pixels),
		red, green, blue);
}


internal 
void MemoryArena_init( MemoryArena* arena, ssize size, Byte* storage )
{
	arena->storage = storage;
	arena->size    = size;
	arena->used    = 0;
}

#define MemoryArena_push_struct( arena, type ) MemoryArena__push_struct<type>( arena )
template< typename Type > inline
Type* MemoryArena__push_struct( MemoryArena* arena )
{
	assert( arena != nullptr );
	
	ssize type_size = sizeof( Type );
	assert( arena->used + type_size <= arena->size );
	
	Type* result = rcast(Type*, arena->storage + arena->used);
	arena->used += type_size;
	
	return result;
}

#define MemoryArena_push_array( arena, type, num ) MemoryArena__push_array<type>( arena, num )
template< typename Type> inline
Type* MemoryArena__push_array( MemoryArena* arena, ssize num )
{
	assert( arena != nullptr );
	
	ssize mem_amount = sizeof( Type ) * num;
	assert( arena->used + mem_amount <= arena->size );
	
	Type* result = rcast(Type*, arena->storage + arena->used);
	arena->used += mem_amount;
	
	return result;
}

Engine_API
void on_module_reload( Memory* memory, platform::ModuleAPI* platfom_api )
{

}

Engine_API
void startup( OffscreenBuffer* back_buffer, Memory* memory, platform::ModuleAPI* platform_api )
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

	state->auto_snapshot_interval = 60.f;

	state->tone_volume = 1000;

	state->x_offset = 0;
	state->y_offset = 0;

	state->sample_wave_switch = false;
	state->wave_tone_hz = 60;
	state->sample_wave_sine_time = 0.f;

	state->renderer_paused = false;
	state->game_memory.persistent_size = memory->persistent_size / Memory::game_memory_factor;
	state->game_memory.persistent      = rcast(Byte*, memory->persistent) + state->game_memory.persistent_size;
	state->game_memory.transient_size  = memory->transient_size / Memory::game_memory_factor;
	state->game_memory.transient       = rcast(Byte*, memory->transient) + state->game_memory.transient_size;
		
	// World setup
	{
		ssize world_arena_size    = memory->engine_persistent_size() - sizeof(EngineState);
		Byte* world_arena_storage = memory->persistent + sizeof(EngineState);
		MemoryArena_init( & state->world_arena, world_arena_size, world_arena_storage );
		
		state->world = MemoryArena_push_struct( & state->world_arena, World);
		World* world = state->world;
		
		TileMap* tile_map = MemoryArena_push_struct( & state->world_arena, TileMap);
		world->tile_map = tile_map;

		tile_map->chunk_shift     = 4;
		tile_map->chunk_mask      = (1 << tile_map->chunk_shift) - 1;
		tile_map->chunk_dimension = (1 << tile_map->chunk_shift);

		tile_map->tile_chunks_num_x = 128;
		tile_map->tile_chunks_num_y = 128;
		
		tile_map->chunks = MemoryArena_push_array( & state->world_arena, TileChunk, tile_map->tile_chunks_num_x * tile_map->tile_chunks_num_y );
		
		for ( s32 y = 0; y < tile_map->tile_chunks_num_y; ++ y )
		{
			for ( s32 x = 0; x < tile_map->tile_chunks_num_x; ++ x )
			{
				ssize num_tiles = tile_map->chunk_dimension * tile_map->chunk_dimension;
				tile_map->chunks[ (y * tile_map->tile_chunks_num_x) + x  ].tiles = MemoryArena_push_array( & state->world_arena, u32, num_tiles );
			}
		}

		//TileChunk temp_chunk;
		//temp_chunk.tiles  = rcast( u32*, & temp_tiles );
		//tile_map->chunks = & temp_chunk;

		tile_map->tile_size_in_meters   = 1.4f;
		tile_map->tile_size_in_pixels   = 85;
		tile_map->tile_meters_to_pixels = scast(f32, tile_map->tile_size_in_pixels) / tile_map->tile_size_in_meters;

		f32 tile_size_in_pixels = scast(f32, tile_map->tile_size_in_pixels);

		world->tile_lower_left_x = -( tile_size_in_pixels * 0.5f);
		world->tile_lower_left_y = +( tile_size_in_pixels * 0.25f) + scast(f32, back_buffer->height);

		u32 tiles_per_screen_x = 17;
		u32 tiles_per_screen_y = 9;
		for ( u32 screen_y = 0; screen_y < 32; ++ screen_y )
		{
			for ( u32 screen_x = 0; screen_x < 32; ++ screen_x )
			{
				for (u32 tile_y = 0; tile_y < tiles_per_screen_y; ++ tile_y )
				{
					for ( u32 tile_x = 0; tile_x < tiles_per_screen_x; ++ tile_x )
					{
						u32 abs_tile_x = screen_x * tiles_per_screen_x + tile_x;
						u32 abs_tile_y = screen_y * tiles_per_screen_y + tile_y;
						u32 tile_value = tile_x == tile_y && tile_y % 2 ? 1 : 0;
						TileMap_set_tile_value( & state->world_arena, world->tile_map, abs_tile_x, abs_tile_y, tile_value );
					}
				}
			}
		}
	}
	
	hh::GameState* game_state = rcast( hh::GameState*, state->game_memory.persistent );
	assert( sizeof(hh::GameState) <= state->game_memory.persistent_size );

	hh::PlayerState* player = & game_state->player_state;
	player->position.tile_x     = 4;
	player->position.tile_y     = 4;
	player->position.x          = 0.f;
	player->position.y          = 0.f;

	player->mid_jump   = false;
	player->jump_time  = 0.f;

	player->height = 1.4f;
	player->width  = player->height * 0.7f;
}

Engine_API
void shutdown( Memory* memory, platform::ModuleAPI* platform_api )
{
}

Engine_API
void update_and_render( f32 delta_time, InputState* input, OffscreenBuffer* back_buffer
	, Memory* memory, platform::ModuleAPI* platform_api, ThreadContext* thread )
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

	hh::GameState*   game_state = rcast( hh::GameState*, state->game_memory.persistent );
	hh::PlayerState* player     = & game_state->player_state;

	f32 x_offset_f = scast(f32, state->x_offset);
	f32 y_offset_f = scast(f32, state->y_offset);

	World*   world    = state->world;
	TileMap* tile_map = world->tile_map;
	
	f32 tile_size_in_pixels   = scast(f32, tile_map->tile_size_in_pixels);
	f32 player_half_width     = player->width  / 2.f;
	f32 player_quarter_height = player->height / 4.f;

	input_poll_player_actions( input, & player_actions );
	{
		f32 move_speed = 2.f;

		if ( player_actions.sprint )
		{
			move_speed = 6.f;
		}

		f32 new_player_pos_x = player->position.x;
		f32 new_player_pos_y = player->position.y;
		if ( player_actions.player_x_move_analog || player_actions.player_y_move_analog )
		{
			new_player_pos_x += scast(f32, player_actions.player_x_move_analog * delta_time * move_speed);
			new_player_pos_y += scast(f32, player_actions.player_y_move_analog * delta_time * move_speed);
		}
		else
		{
			new_player_pos_x += scast(f32, player_actions.player_x_move_digital) * delta_time * move_speed;
			new_player_pos_y += scast(f32, player_actions.player_y_move_digital) * delta_time * move_speed;
		}
		new_player_pos_y += sinf( player->jump_time * TAU ) * 10.f * delta_time;

		b32 valid_new_pos = true;
		{
			TileMapPosition test_pos = {
				new_player_pos_x, new_player_pos_y,
				player->position.tile_x, player->position.tile_y
			};
			test_pos = recannonicalize_position( tile_map, test_pos );

			// TODO(Ed) : Need a delta-function that auto-reconnonicalizes.

			TileMapPosition test_pos_nw {
				new_player_pos_x - player_half_width, new_player_pos_y + player_quarter_height,
				player->position.tile_x, player->position.tile_y
			};
			test_pos_nw       = recannonicalize_position( tile_map, test_pos_nw );
			valid_new_pos    &= TileMap_is_point_empty( tile_map, test_pos_nw );

			TileMapPosition test_pos_ne {
				new_player_pos_x + player_half_width, new_player_pos_y + player_quarter_height,
				player->position.tile_x, player->position.tile_y
			};
			test_pos_ne    = recannonicalize_position( tile_map, test_pos_ne );
			valid_new_pos &= TileMap_is_point_empty( tile_map, test_pos_ne );

			TileMapPosition test_pos_sw {
				new_player_pos_x - player_half_width, new_player_pos_y,
				player->position.tile_x, player->position.tile_y
			};
			test_pos_sw    = recannonicalize_position( tile_map, test_pos_sw );
			valid_new_pos &= TileMap_is_point_empty( tile_map, test_pos_sw );

			TileMapPosition test_pos_se {
				new_player_pos_x + player_half_width, new_player_pos_y,
				player->position.tile_x, player->position.tile_y
			};
			test_pos_se    = recannonicalize_position( tile_map, test_pos_se );
			valid_new_pos &= TileMap_is_point_empty( tile_map, test_pos_se );
		}

		if ( valid_new_pos )
		{
			TileMapPosition new_pos = {
				new_player_pos_x, new_player_pos_y,
				player->position.tile_x, player->position.tile_y
			};
			player->position = recannonicalize_position( tile_map, new_pos );
		}

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

	draw_rectangle( back_buffer
		, 0.f, 0.f
		, scast(f32, back_buffer->width), scast(f32, back_buffer->height)
		, 1.f, 0.24f, 0.24f );


// Scrolling
	f32 screen_center_x = 0.5f * scast(f32, back_buffer->width);
	f32 screen_center_y = 0.5f * scast(f32, back_buffer->height);

	for ( s32 relative_row = -10; relative_row < +10; ++ relative_row )
	{
		for ( s32 relative_col = -20; relative_col < +20; ++ relative_col )
		{
			u32 col = player->position.tile_x + relative_col;
			u32 row = player->position.tile_y + relative_row;

			u32 tileID   = TileMap_get_tile_value( tile_map, col, row );
			f32 color[3] = { 0.15f, 0.15f, 0.15f };

			if ( tileID == 1 )
			{
				color[0] = 0.22f;
				color[1] = 0.22f;
				color[2] = 0.22f;
			}

			if ( row == player->position.tile_y && col == player->position.tile_x )
			{
				color[0] = 0.44f;
				color[1] = 0.3f;
				color[2] = 0.3f;
			}

			f32 center_x = screen_center_x + scast(f32, relative_col) * tile_size_in_pixels - player->position.x * tile_map->tile_meters_to_pixels;
			f32 center_y = screen_center_y - scast(f32, relative_row) * tile_size_in_pixels + player->position.y * tile_map->tile_meters_to_pixels;

			f32 min_x = center_x - tile_size_in_pixels * 0.5f;
			f32 min_y = center_y - tile_size_in_pixels * 0.5f;
			f32 max_x = center_x + tile_size_in_pixels * 0.5f;
			f32 max_y = center_y + tile_size_in_pixels * 0.5f;

			draw_rectangle( back_buffer
			               , min_x, min_y
			               , max_x, max_y
			               , color[0], color[1], color[2] );
		}
	}

	// Player
	f32 player_red   = 0.7f;
	f32 player_green = 0.7f;
	f32 player_blue  = 0.3f;

	f32 player_tile_x_offset = screen_center_x + scast(f32, player->position.tile_x) * tile_map->tile_meters_to_pixels + player->position.x * tile_map->tile_meters_to_pixels;
	f32 player_tile_y_offset = screen_center_y - scast(f32, player->position.tile_y) * tile_map->tile_meters_to_pixels + player->position.y * tile_map->tile_meters_to_pixels;

	f32 player_screen_pos_x = screen_center_x;
	f32 player_screen_pos_y = screen_center_y;

//	player_min_x = player_tile_x_offset - player_half_width * world;

	draw_rectangle( back_buffer
	               , player_screen_pos_x - player_half_width * tile_map->tile_meters_to_pixels, player_screen_pos_y - player->height * tile_map->tile_meters_to_pixels
	               , player_screen_pos_x + player_half_width * tile_map->tile_meters_to_pixels, player_screen_pos_y
	               , player_red, player_green, player_blue );

	// Auto-Snapshot percent bar
	if (1)
	{
		f32 snapshot_percent_x = ((state->auto_snapshot_timer / state->auto_snapshot_interval)) * (f32)back_buffer->width / 4.f;
		draw_rectangle( back_buffer
			, 0.f, 0.f
			, snapshot_percent_x, 10.f
			, 0.f, 0.15f, 0.35f );
	}

#if Build_Development
	if ( memory->replay_mode == ReplayMode_Record )
	{
		draw_rectangle( back_buffer
			, player->position.x + 50.f, player->position.y - 50.f
			, player->position.x + 10.f, player->position.y + 40.f
			, 1.f, 1.f, 1.f );
	}
#endif

	// Change above to use draw rectangle
	if ( 0 )
	{
		draw_rectangle( back_buffer
			, (f32)input->controllers[0].mouse->X.end, (f32)input->controllers[0].mouse->Y.end
			, (f32)input->controllers[0].mouse->X.end + 10.f, (f32)input->controllers[0].mouse->Y.end + 10.f
			, 1.f, 1.f, 0.f );
	}

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
