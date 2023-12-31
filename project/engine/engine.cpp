#if INTELLISENSE_DIRECTIVES
#include "engine.hpp"
#include "input.hpp"
#include "engine_to_platform_api.hpp"

#include "tile_map.cpp"
#include "random.cpp"

// TODO(Ed) : This needs to be moved out eventually
#include "handmade.hpp"
#endif

NS_ENGINE_BEGIN
struct EngineState
{
	hh::Memory game_memory;

	MemoryArena world_arena;

#if Build_Development
	f32 auto_snapshot_interval;
	f32 auto_snapshot_timer;
#endif

	s32 wave_tone_hz;
	s32 tone_volume;
	s32 x_offset;
	s32 y_offset;

	b32 renderer_paused;

	f32 sample_wave_sine_time;
	b32 sample_wave_switch;

	EngineContext context;
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

global EngineContext* Engine_Context = nullptr;

EngineContext* get_context()
{
	return Engine_Context;
}

#define pressed( btn ) (btn.ended_down && btn.half_transitions > 0)

inline
Vec2 get_screen_center( OffscreenBuffer* back_buffer )
{
	Vec2 result {
		scast(f32, back_buffer->width)  * 0.5f,
		scast(f32, back_buffer->height) * 0.5f
	};
	return result;
}

internal
void input_poll_engine_actions( InputState* input, EngineActions* actions )
{
	KeyboardState* keyboard = input->keyboard;

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

	MousesState* mouse = input->mouse;

	actions->move_right = (mouse->horizontal_wheel.end > 0.f) * 20;
	actions->move_left  = (mouse->horizontal_wheel.end < 0.f) * 20;

	actions->move_up    = (mouse->vertical_wheel.end > 0.f) * 10;
	actions->move_down  = (mouse->vertical_wheel.end < 0.f) * 10;

#if Build_Development
	actions->load_auto_snapshot |= pressed( keyboard->L ) && keyboard->right_alt.ended_down;
#endif
}

// TODO(Ed) : Move to handmade module
internal
void input_poll_player_actions( hh::Player* player, b32 is_player_2 )
{
	hh::ControllerState* controller = & player->controller;
	hh::PlayerActions*   actions    = & player->actions;
	* actions = {};

	if ( controller->ds_pad )
	{
		DualsensePadState* pad = controller->ds_pad;

		actions->sprint |= pad->circle.ended_down;
		actions->jump   |= pressed( pad->cross );

		actions->player_x_move_analog += pad->stick.left.X.end;
		actions->player_y_move_analog += pad->stick.left.Y.end;
		
		actions->join |= pressed( pad->options );
	}
	if ( controller->xpad )
	{
		XInputPadState* pad = controller->xpad;

		actions->sprint |= pad->B.ended_down;
		actions->jump   |= pressed( pad->A );

		actions->player_x_move_analog += pad->stick.left.X.end;
		actions->player_y_move_analog += pad->stick.left.Y.end;
		
		actions->join |= pressed( pad->start );
	}

	if ( controller->keyboard )
	{
		KeyboardState* keyboard = controller->keyboard;
		actions->jump   |= pressed( keyboard->space );
		actions->sprint |= keyboard->left_shift.ended_down;

		actions->player_x_move_digital += keyboard->D.ended_down - keyboard->A.ended_down;
		actions->player_y_move_digital += keyboard->W.ended_down - keyboard->S.ended_down;
		
		actions->join |= pressed( keyboard->enter );
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
	, Vec2 min, Vec2 max
	, f32 red, f32 green, f32 blue  )
{
	Vec2i min_rounded { round(min.x), round(min.y) };
	Vec2i max_rounded { round(max.x), round(max.y) };

	s32 width  = buffer->width;
	s32 height = buffer->height;

	if ( min_rounded.x < 0 )
		min_rounded.x = 0;
	if ( min_rounded.y < 0 )
		min_rounded.y = 0;
	if ( max_rounded.x > width )
		max_rounded.x = width;
	if ( max_rounded.y > height )
		max_rounded.y = height;

	s32 red_32   = round( 255.f * red );
	s32 green_32 = round( 255.f * green );
	s32 blue_32  = round( 255.f * blue );

	s32 color =
			(scast(s32, red_32)   << 16)
		|	(scast(s32, green_32) << 8)
		|	(scast(s32, blue_32)  << 0);

	// Start with the pixel on the top left corner of the rectangle
	u8* row = rcast(u8*, buffer->memory )
		+ min_rounded.x * buffer->bytes_per_pixel
		+ min_rounded.y * buffer->pitch;

	for ( s32 y = min_rounded.y; y < max_rounded.y; ++ y )
	{
		s32* pixel_32 = rcast(s32*, row);

		for ( s32 x = min_rounded.x; x < max_rounded.x; ++ x )
		{
			*pixel_32 = color;
			pixel_32++;
		}
		row += buffer->pitch;
	}
}

internal
void draw_bitmap( OffscreenBuffer* buffer, Vec2 pos, Bitmap* bitmap )
{
	s32 half_width  = bitmap->width / 2;
	s32 half_height = bitmap->height / 2;

	Vec2i pos_rounded   { round(pos.x), round(pos.y) };
	Vec2i bmp_half_size { bitmap->width / 2, bitmap->height / 2 };
	Vec2i min = pos_rounded - bmp_half_size;
	Vec2i max = pos_rounded + bmp_half_size;

	s32 max_x = round( pos.x ) + half_width;
	s32 max_y = round( pos.y ) + half_height;

	s32 bmp_offset_x = min.x < 0 ? min.x * -1 : 0;
	u32 bmp_offset_y = min.y < 0 ? bitmap->height + min.y - 1 : bitmap->height - 1;

	s32 width  = buffer->width;
	s32 height = buffer->height;

	if ( min.x < 0 )
		min.x = 0;
	if ( min.y < 0 )
		min.y = 0;
	if ( max.x > width )
		max.x = width;
	if ( max.y > height )
		max.y = height;

	// Start with the pixel on the top left corner of the rectangle
	u8*  dst_row = rcast(u8*, buffer->memory )
	          + min.x * buffer->bytes_per_pixel
	          + min.y * buffer->pitch;
	u32* src_row = bitmap->pixels + bitmap->width * bmp_offset_y;

	for ( s32 y = min.y; y < max.y; ++ y )
	{
		u32* dst = rcast(u32*, dst_row);
		u32* src = src_row + bmp_offset_x;

		for ( s32 x = min.x; x < max.x; ++ x )
		{
		#define extract( pixel, shift ) (( *pixel >> shift ) & 0xFF)
			f32 alpha = scast(f32, extract(src, 24)) / 255.f;

			f32 src_R = scast(f32, extract(src, 16));
			f32 src_G = scast(f32, extract(src, 8));
			f32 src_B = scast(f32, extract(src, 0));

			f32 dst_R = scast(f32, extract(dst, 16));
			f32 dst_G = scast(f32, extract(dst, 8));
			f32 dst_B = scast(f32, extract(dst, 0));
		#undef extract

			f32 red   = (1 - alpha) * dst_R + alpha * src_R;
			f32 green = (1 - alpha) * dst_G + alpha * src_G;
			f32 blue  = (1 - alpha) * dst_B + alpha * src_B;

			*dst = u32(red + 0.5f) << 16 | u32(green + 0.5f) << 8 | u32(blue + 0.5f) << 0;

			++ dst;
			++ src;
		}

		dst_row += buffer->pitch;
		src_row -= bitmap->width;
	}
}

inline
void draw_debug_point(OffscreenBuffer* back_buffer, World* world, TileMapPos pos, f32 red, f32 green, f32 blue)
{
	TileMap* tile_map = world->tile_map;

	Vec2 min {
		pos.rel_pos.x * world->tile_meters_to_pixels + world->tile_lower_left_x + scast(f32, pos.tile_x * world->tile_size_in_pixels),
		pos.rel_pos.y * world->tile_meters_to_pixels + world->tile_lower_left_y + scast(f32, pos.tile_y * world->tile_size_in_pixels)
	};
	Vec2 max {
		(pos.rel_pos.x + 0.1f) * world->tile_meters_to_pixels + world->tile_lower_left_x + scast(f32, pos.tile_x * world->tile_size_in_pixels),
		(pos.rel_pos.y + 0.1f) * world->tile_meters_to_pixels + world->tile_lower_left_y + scast(f32, pos.tile_y * world->tile_size_in_pixels)
	};

	draw_rectangle(back_buffer
		, min, max
		, red, green, blue);
}

internal
Bitmap load_bmp( platform::ModuleAPI* platform_api, Str file_path  )
{
	Bitmap result {};

	platform::File file {
		file_path
	};
	if ( ! platform_api->file_read_content( & file ) )
	{

		return result;
	}

	BitmapHeaderPacked* header = pcast(BitmapHeaderPacked*, file.data);
	assert( header->compression == 3 );
	
	u32 alpha_mask = ~(header->red_mask | header->green_mask | header->blue_mask);

	// TODO(Ed) : Do not directly assign this, allocate the pixels to somewhere in game or engine persistent.
	result.pixels         = rcast(u32*, rcast(Byte*, file.data) + header->bitmap_offset);
	result.width          = header->width;
	result.height         = header->height;
	result.bits_per_pixel = header->bits_per_pixel;

	u32 red_scan   = 0;
	u32 green_scan = 0;
	u32 blue_scan  = 0;
	u32 alpha_scan = 0;

	b32 red_found    = bitscan_forward( & red_scan,   header->red_mask );
	b32 green_found  = bitscan_forward( & green_scan, header->green_mask );
	b32 blue_found   = bitscan_forward( & blue_scan,  header->blue_mask );
	b32 alpha_found  = bitscan_forward( & alpha_scan, alpha_mask );
	
	assert( red_found );
	assert( green_found );
	assert( blue_found );
	assert( alpha_found );
	
#if 1
	s32 blue_shift  =  0 - scast( s32, blue_scan  );
	s32 green_shift =  8 - scast( s32, green_scan );
	s32 red_shift   = 16 - scast( s32, red_scan   );
	s32 alpha_shift = 24 - scast( s32, alpha_scan );
#endif

	u32* src = result.pixels;
	for ( s32 y = 0; y < header->width; ++ y )
	{
		for ( s32 x = 0; x < header->height; ++ x )
		{
			struct Pixel
			{
				u8 Alpha;
				u8 Blue;
				u8 Green;
				u8 Red;
			};

			Pixel* px = rcast(Pixel*, src);

		#if 0
			u32 alpha = (( *src >> alpha_scan ) & 0xFF) << 24;
			u32 red   = (( *src >> red_scan   ) & 0xFF) << 16;
			u32 green = (( *src >> green_scan ) & 0xFF) << 8;
			u32 blue  = (( *src >> blue_scan  ) & 0xFF) << 0;
		#else
			u32 alpha = rotate_left( *src & alpha_mask,         alpha_shift );
			u32 red   = rotate_left( *src & header->red_mask,   red_shift   );
			u32 green = rotate_left( *src & header->green_mask, green_shift );
			u32 blue  = rotate_left( *src & header->blue_mask,  blue_shift  );
		#endif

			 *src = alpha | red | green | blue;
			++ src;
		}
	}

	//platform_api->file_close( & file );
	return result;
}

inline
hh::Entity* get_entity( hh::GameState* gs, s32 id )
{
	hh::Entity* entity = nullptr;
	
	if ( id > 0 && id < array_count(gs->entities) )
	{
		entity = & gs->entities[ id - 1 ];
	}
	
	return entity;
}

inline
s32 get_entity( hh::GameState* gs )
{
	for ( s32 id = 0; id < array_count(gs->entities); ++ id )
	{
		hh::Entity* entity = & gs->entities[ id ];
		if ( ! entity->exists )
		{
			* entity = {};
			entity->exists = true;
			return id + 1;
		}
	}
	
	return 0;
}

// TODO(Ed): Move to handmade module
inline
void player_init( hh::Player* player, hh::GameState* gs )
{
	hh::PlayerState* state     = & player->state;
	s32              entity_id = get_entity( gs );
	assert( entity_id );
	
	hh::Entity* entity = get_entity( gs, entity_id );
	
	entity->kind = hh::EntityKind_Hero;
	
	entity->position = gs->spawn_pos;

	entity->velocity = {};

	state->mid_jump   = false;
	state->jump_time  = 0.f;

	entity->height = 1.4f;
	entity->width  = entity->height * 0.7f;
	
	entity->facing_direction = hh::FacingDirection_Front;
	
	player->entity_id = entity_id; 
}

// TODO(Ed) : Make a menu down the line to allow the player to assign an input device to a player.
void update_player_controllers( hh::GameState* gs, InputState* input )
{	
	gs->player_1.controller.keyboard = input->keyboard;
	gs->player_1.controller.mouse    = input->mouse;

	for ( s32 id = 1; id <= Max_Controllers; ++ id )
	{
		XInputPadState*    xpad   = input->xpads  [ id - 1 ];
		DualsensePadState* ds_pad = input->ds_pads[ id - 1 ];
		if ( gs->player_1.controller.xpad_id == id )
		{
			gs->player_1.controller.xpad = xpad;
			xpad = nullptr;
		}
		if ( gs->player_1.controller.ds_pad_id == id )
		{
			gs->player_1.controller.ds_pad = ds_pad;
			ds_pad = nullptr;
		}
		if ( gs->player_2.controller.xpad_id == id )
		{
			gs->player_2.controller.xpad = xpad;
			xpad = nullptr;
		}
		if ( gs->player_2.controller.ds_pad_id == id )
		{
			gs->player_2.controller.ds_pad = ds_pad;
			ds_pad = nullptr;
		}
		
	#define can_assign( player ) ( player.controller.xpad == nullptr && player.controller.ds_pad == nullptr )
		if ( xpad && pressed( xpad->start ) )
		{
			if ( can_assign( gs->player_1 ) )
			{
				gs->player_1.controller.xpad_id = id;
				gs->player_1.controller.xpad    = xpad;
				continue;
			}
				
			if ( can_assign( gs->player_2 ) )
			{
				gs->player_2.controller.xpad_id = id;
				gs->player_2.controller.xpad    = xpad;
			}
		}
		
		if ( ds_pad && pressed( ds_pad->options ) )
		{
			if ( can_assign( gs->player_1 ) )
			{
				gs->player_1.controller.ds_pad_id = id;
				gs->player_1.controller.ds_pad    = ds_pad;
				continue;
			}
				
			if ( can_assign( gs->player_2 ) )
			{
				gs->player_2.controller.ds_pad_id = id;
				gs->player_2.controller.ds_pad    = ds_pad;
			}
		}
	#undef can_assign
	}
}

internal
void test_wall(f32 wall_x, Vec2 origin, f32 min_y, f32 max_y, Vec2 ray, f32* result_distance )
{
	f32 tolerance = 0.01f;

	if ( ray.x != 0.f )
	{
		f32 intersect_distance = (wall_x - origin.x) * 1 / ray.x;
		f32 result_y           = origin.y + intersect_distance * ray.y;
		
		if ( intersect_distance > 0.f && (* result_distance) > intersect_distance)
		{
			if ( result_y >= min_y && result_y <= max_y )
			{
//				(* result_distance) = intersect_distance;
				(* result_distance) = max( 0.f, intersect_distance - tolerance );
			}
		}
	}
}

// TODO(Ed) : Move to handmade module?
internal
void update_player( hh::Player* player, f32 delta_time, World* world, hh::GameState* gs, OffscreenBuffer* back_buffer )
{
	hh::PlayerActions* actions = & player->actions;
	if ( actions->join && ! player->entity_id ) { 
		player_init( player, gs );
		gs->camera_assigned_entity_id = player->entity_id;
	}
	
	if ( ! player->entity_id )
		return;
	
	hh::PlayerState* state  = & player->state;
	hh::Entity*      entity = get_entity( gs, player->entity_id );

	TileMap* tile_map = world->tile_map;

	f32 tile_size_in_pixels   = scast(f32, world->tile_size_in_pixels);
	f32 player_half_width     = entity->width  / 2.f;
	f32 player_quarter_height = entity->height / 4.f;

//	f32 move_accel = 36.f;
	f32 move_accel = 200.f;
	
	if ( actions->sprint )
	{
		move_accel = 94.f;
	}

	TileMapPos old_pos        = entity->position;
	Pos2_f32   new_player_pos = { old_pos.rel_pos.x, old_pos.rel_pos.y };

	Vec2 player_move_vec = {};
	if ( actions->player_x_move_analog || actions->player_y_move_analog )
	{
		player_move_vec.x = scast(f32, actions->player_x_move_analog );
		player_move_vec.y = scast(f32, actions->player_y_move_analog );
	}
	else
	{
		player_move_vec.x = scast(f32, actions->player_x_move_digital );
		player_move_vec.y = scast(f32, actions->player_y_move_digital );
	}
	
	// vec_x_adjusted = desired_vec_len / curr_vec_len * curr_vec_x
	
	// Allows for more ganular movement with the analog
	if (0)
	{
		f32 accel_scale = magnitude( player_move_vec );
		if ( accel_scale > 1.f )
			accel_scale = 1.f;
		move_accel *= accel_scale;
	}
	
	Dir2   player_direction  = cast( Dir2, player_move_vec );
	Accel2 player_move_accel = scast( Accel2, player_direction ) * move_accel;

	// TODO(Ed) : ODE!
	Accel2 friction    = pcast( Accel2, entity->velocity) * 9.f;
	player_move_accel -= friction;

	entity->velocity += player_move_accel * 0.5f;
	new_player_pos   += entity->velocity;

	// Collision
	if (0)
	{
		b32 collision_nw = false;
		b32 collision_ne = false;
		b32 collision_sw = false;
		b32 collision_se = false;
		do
		{
			// Base position
			//TileMapPos test_pos = {
				//new_player_pos.x, new_player_pos.y,
				//old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
			//};
			//test_pos = recannonicalize_position( tile_map, test_pos );

			// TODO(Ed) : Need a delta-function that auto-reconnonicalizes.

			TileMapPos test_pos_nw {
				new_player_pos.x - player_half_width, new_player_pos.y + player_quarter_height,
				old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
			};
			test_pos_nw  = recannonicalize_position( tile_map, test_pos_nw );
			collision_nw = ! TileMap_is_point_empty( tile_map, test_pos_nw );

			TileMapPos test_pos_ne {
				new_player_pos.x + player_half_width, new_player_pos.y + player_quarter_height,
				old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
			};
			test_pos_ne  = recannonicalize_position( tile_map, test_pos_ne );
			collision_ne = ! TileMap_is_point_empty( tile_map, test_pos_ne );

			TileMapPos test_pos_sw {
				new_player_pos.x - player_half_width, new_player_pos.y,
				old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
			};
			test_pos_sw  = recannonicalize_position( tile_map, test_pos_sw );
			collision_sw = ! TileMap_is_point_empty( tile_map, test_pos_sw );

			TileMapPos test_pos_se {
				new_player_pos.x + player_half_width, new_player_pos.y,
				old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
			};
			test_pos_se  = recannonicalize_position( tile_map, test_pos_se );
			collision_se = ! TileMap_is_point_empty( tile_map, test_pos_se );
		} 
		while(0);

		if ( collision_se || collision_sw || collision_ne || collision_nw )
		{
			// Should be colliding with a wall
			
			Vec2 wall_vector = { 0, 0 };
			if ( collision_nw && collision_sw )
			{
				wall_vector = { 1.f, 0.f };
			}
			if ( collision_ne && collision_se )
			{
				wall_vector = { -1.f, 0.f };
			}
			if ( collision_nw && collision_ne )
			{
				wall_vector = { 0.f, 1.f };
			}
			if ( collision_se && collision_sw )
			{
				wall_vector = { 0.f, -1.f };
			}
			
			//if ( collision_nw && !collision_ne && !collision_sw && !collision_se )
			//{
			//	wall_vector = { 1.f, 1.f };
			//}
			
			// The 2x multiplier allows for the the "bounce off" velocity to occur instead of the player just looking like they impacted the wall and stopped
			entity->velocity -= cast( Vel2,  1.f * scalar_product( Vec2( entity->velocity ), wall_vector ) * wall_vector );
			
			new_player_pos = { old_pos.rel_pos.x, old_pos.rel_pos.y };
			new_player_pos += entity->velocity;
		}
		
		TileMapPos new_pos = {
			new_player_pos.x, new_player_pos.y,
			old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
		};
		new_pos = recannonicalize_position( tile_map, new_pos );
		entity->position = new_pos;
	}
	else
	{
		TileMapPos new_pos = recannonicalize_position( tile_map, {
			new_player_pos.x, new_player_pos.y,
			old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
		});
		TileMapPos best_position          = old_pos;
		f32        min_intersection_delta = 1.0f;
			
		if (0) 
		{
			s32 min_tile_x = min( old_pos.tile_x, new_pos.tile_x );
			s32 min_tile_y = min( old_pos.tile_y, new_pos.tile_y );
			
			s32 max_tile_x = max( old_pos.tile_x, new_pos.tile_x );
			s32 max_tile_y = max( old_pos.tile_y, new_pos.tile_y );
			
			for ( s32 tile_y = min_tile_y; tile_y <= max_tile_y; ++ tile_y )
			for ( s32 tile_x = min_tile_x; tile_x <= max_tile_x; ++ tile_x )
			{
				TileMapPos test_tile_pos = centered_tile_point( tile_x, tile_y, old_pos.tile_z );
				s32        tile_value    = TileMap_get_tile_value( tile_map, test_tile_pos );
				if ( ! TileMap_is_tile_value_empty( tile_value ) )
				{
					Vec2 tile_xy_in_meters = Vec2 { tile_map->tile_side_in_meters, tile_map->tile_side_in_meters };
					
					Vec2 min_corner = -0.5f * tile_xy_in_meters;
					Vec2 max_corner =  0.5f * tile_xy_in_meters;
					
					Vec2 rel_old_pos     = subtract( old_pos, test_tile_pos ).rel_pos;
					Vec2 rel_old_pos_inv = { rel_old_pos.y,      rel_old_pos.x      };
					Vec2 vel_inv         = { entity->velocity.y, entity->velocity.x };
					
					test_wall( min_corner.x, rel_old_pos,     min_corner.y, max_corner.y, entity->velocity, & min_intersection_delta );
					test_wall( max_corner.x, rel_old_pos,     min_corner.y, max_corner.y, entity->velocity, & min_intersection_delta );
					test_wall( min_corner.y, rel_old_pos_inv, min_corner.x, max_corner.x, vel_inv,          & min_intersection_delta );
					test_wall( max_corner.y, rel_old_pos_inv, min_corner.x, max_corner.x, vel_inv,          & min_intersection_delta );
				}
			}
		}
		else
		{
			s32 tile_delta_x = sign( new_pos.tile_x - old_pos.tile_x );
			s32 tile_delta_y = sign( new_pos.tile_y - old_pos.tile_y );
						
			for ( s32 tile_y = old_pos.tile_y; ; tile_y += tile_delta_y )
			{
				for ( s32 tile_x = old_pos.tile_x; ; tile_x += tile_delta_x )
				{
					TileMapPos test_tile_pos = centered_tile_point( tile_x, tile_y, old_pos.tile_z );
					s32        tile_value    = TileMap_get_tile_value( tile_map, test_tile_pos );
					
					local_persist TileChunkPosition last_chunk = {};
					TileChunkPosition curr_chunk = get_tile_chunk_position_for(tile_map, old_pos.tile_x, old_pos.tile_y, old_pos.tile_z );
						
					if ( ! TileMap_is_tile_value_empty( tile_value ) )
					{
						Vec2 tile_xy_in_meters = Vec2 { tile_map->tile_side_in_meters, tile_map->tile_side_in_meters };
						
						Vec2 min_corner = -0.5f * tile_xy_in_meters;
						Vec2 max_corner =  0.5f * tile_xy_in_meters;
						
						Vec2 rel_old_pos     = subtract( old_pos, test_tile_pos ).rel_pos;
						Vec2 rel_old_pos_inv = { rel_old_pos.y,      rel_old_pos.x      };
						Vec2 vel_inv         = { entity->velocity.y, entity->velocity.x };
						
						test_wall( min_corner.x, rel_old_pos,     min_corner.y, max_corner.y, entity->velocity, & min_intersection_delta );
						test_wall( max_corner.x, rel_old_pos,     min_corner.y, max_corner.y, entity->velocity, & min_intersection_delta );
						test_wall( min_corner.y, rel_old_pos_inv, min_corner.x, max_corner.x, vel_inv,          & min_intersection_delta );
						test_wall( max_corner.y, rel_old_pos_inv, min_corner.x, max_corner.x, vel_inv,          & min_intersection_delta );
					}
					
					if ( tile_x == new_pos.tile_x )
						break;
				}
				
				if ( tile_y == new_pos.tile_y )
					break;
			}
		}
		
		new_player_pos  = { old_pos.rel_pos.x, old_pos.rel_pos.y };
		new_player_pos += entity->velocity * min_intersection_delta;
		new_pos = recannonicalize_position( tile_map, {
			new_player_pos.x, new_player_pos.y,
			old_pos.tile_x, old_pos.tile_y, old_pos.tile_z
		});
		entity->position = new_pos;
	}
	
	b32    on_new_tile = TileMap_are_on_same_tile( & entity->position, & old_pos );
	if ( ! on_new_tile )
	{
		u32 new_tile_value = TileMap_get_tile_value( tile_map, entity->position );

		if ( new_tile_value == 3 )
		{
			++ entity->position.tile_z;
		}
		else if ( new_tile_value == 4 )
		{
			-- entity->position.tile_z;
		}
	}
	
	using hh::EFacingDirection;
	using hh::FacingDirection_Front;
	using hh::FacingDirection_Back;
	using hh::FacingDirection_Left;
	using hh::FacingDirection_Right;
	
	if ( is_nearly_zero( entity->velocity.x ) && is_nearly_zero( entity->velocity.y ) )
	{
		// Note(Ed): Leave facing directiona alone
	}
	else if ( abs( entity->velocity.x ) > abs( entity->velocity.y ) )
	{
		if ( entity->velocity.x > 0 )
		{
			entity->facing_direction = FacingDirection_Right;
		}
		else
		{
			entity->facing_direction = FacingDirection_Left;
		}
	}
	else if ( abs( entity->velocity.x ) < abs( entity->velocity.y ) )
	{
		if ( entity->velocity.y >= 0 )
		{
			entity->facing_direction = FacingDirection_Back;
		}
		else
		{
			entity->facing_direction = FacingDirection_Front;
		}
	}

	if ( state->jump_time > 0.f )
	{
		state->jump_time -= delta_time;
	}
	else
	{
		state->jump_time = 0.f;
		state->mid_jump  = false;
	}

	if ( ! state->mid_jump && actions->jump )
	{
		state->jump_time = 1.f;
		state->mid_jump  = true;
	}
}

void render_player( hh::Entity* entity, World* world, hh::GameState* gs, OffscreenBuffer* back_buffer )
{
	Vec2 screen_center = get_screen_center( back_buffer );
	
	f32 player_red   = 0.7f;
	f32 player_green = 0.7f;
	f32 player_blue  = 0.3f;
	
	f32 player_half_width = entity->width  / 2.f;
	
	if ( entity->position.tile_z != gs->camera_pos.tile_z )
		return;

	TileMapPos player_to_camera = subtract( entity->position, gs->camera_pos );

	Vec2 player_to_screenspace {
			  player_to_camera.rel_pos.x + scast(f32, player_to_camera.tile_x) * world->tile_map->tile_side_in_meters,
		-1 * (player_to_camera.rel_pos.y + scast(f32, player_to_camera.tile_y) * world->tile_map->tile_side_in_meters)
	};
	Pos2 player_ground_pos = cast( Pos2, screen_center + player_to_screenspace * world->tile_meters_to_pixels );

	hh::HeroBitmaps* hero_bitmaps = & gs->hero_bitmaps[entity->facing_direction];

#if 1
	Vec2 player_collision_min {
		player_ground_pos.x - player_half_width * world->tile_meters_to_pixels,
		player_ground_pos.y - entity->height    * world->tile_meters_to_pixels,
	};
	Vec2 player_collision_max {
		player_ground_pos.x + player_half_width * world->tile_meters_to_pixels,
		player_ground_pos.y
	};

	draw_rectangle( back_buffer
		, player_collision_min, player_collision_max
		, player_red, player_green, player_blue );
#endif

	draw_bitmap( back_buffer
		, { player_ground_pos.x, player_ground_pos.y - scast(f32, hero_bitmaps->align_y) }
		, & hero_bitmaps->torso );
	draw_bitmap( back_buffer
		, { player_ground_pos.x, player_ground_pos.y - scast(f32, hero_bitmaps->align_y) }
		, & hero_bitmaps->cape );

	if ( entity == get_entity( gs, gs->player_1.entity_id ) )
	{
		draw_bitmap( back_buffer
			, { player_ground_pos.x, player_ground_pos.y - 125.f }
			, & gs->mojito_head );
	}
	else
	{
		draw_bitmap( back_buffer
			, { player_ground_pos.x, player_ground_pos.y - scast(f32, hero_bitmaps->align_y) }
			, & hero_bitmaps->head );
	}
}

Engine_API
void on_module_reload( Memory* memory, platform::ModuleAPI* platfom_api )
{
	Engine_Context = & rcast(EngineState*, memory->persistent)->context;
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

	Engine_Context = & state->context;

#if Build_Development
	state->auto_snapshot_interval = 60.f;
#endif

	state->tone_volume = 1000;

	state->x_offset = 0;
	state->y_offset = 0;

	state->sample_wave_switch    = false;
	state->wave_tone_hz          = 60;
	state->sample_wave_sine_time = 0.f;

	state->renderer_paused = false;
	state->game_memory.persistent_size = memory->persistent_size / Memory::game_memory_factor;
	state->game_memory.persistent      = rcast(Byte*, memory->persistent) + state->game_memory.persistent_size;
	state->game_memory.transient_size  = memory->transient_size / Memory::game_memory_factor;
	state->game_memory.transient       = rcast(Byte*, memory->transient) + state->game_memory.transient_size;
		
	hh::GameState* gs = rcast( hh::GameState*, state->game_memory.persistent );
	assert( sizeof(hh::GameState) <= state->game_memory.persistent_size );
	
	World* world;

	// World setup
	{
		ssize world_arena_size    = memory->engine_persistent_size() - sizeof(EngineState);
		Byte* world_arena_storage = memory->persistent + sizeof(EngineState);
		MemoryArena::init( & state->world_arena, world_arena_size, world_arena_storage );

		//state->world = MemoryArena_push_struct( & state->world_arena, World);
		state->context.world = state->world_arena.push_struct( World );
		world = state->context.world;

		TileMap* tile_map = state->world_arena.push_struct( TileMap );
		world->tile_map = tile_map;

		tile_map->chunk_shift     = 4;
		tile_map->chunk_mask      = (1 << tile_map->chunk_shift) - 1;
		tile_map->chunk_dimension = (1 << tile_map->chunk_shift);

		tile_map->tile_chunks_num_x = 128;
		tile_map->tile_chunks_num_y = 128;
		tile_map->tile_chunks_num_z = 2;

		ssize num_chunks = tile_map->tile_chunks_num_x * tile_map->tile_chunks_num_y * tile_map->tile_chunks_num_z;
		tile_map->chunks = state->world_arena.push_array( TileChunk, num_chunks );

		//TileChunk temp_chunk;
		//temp_chunk.tiles  = rcast( u32*, & temp_tiles );
		//tile_map->chunks = & temp_chunk;

		tile_map->tile_side_in_meters = 1.4f;
		world->tile_size_in_pixels    = 80;
		world->tile_meters_to_pixels  = scast(f32, world->tile_size_in_pixels) / tile_map->tile_side_in_meters;

		f32 tile_size_in_pixels = scast(f32, world->tile_size_in_pixels);

		world->tile_lower_left_x = -( tile_size_in_pixels * 0.5f);
		world->tile_lower_left_y = +( tile_size_in_pixels * 0.25f) + scast(f32, back_buffer->height);

		world->tiles_per_screen_x = 17;
		world->tiles_per_screen_y = 9;

		s32 screen_x  = -10;
		s32 screen_y  = -10;
		u32 rng_index = 0;
		
		b32 door_left   = false;
		b32 door_right  = false;
		b32 door_top    = false;
		b32 door_bottom = false;
		b32 door_up     = false;
		b32 door_down   = false;

		u32 abs_tile_z = 0;

		for ( u32 screen_index = 0; screen_index <= 100; ++ screen_index )
		{
			// TODO(Ed) : We need a proper RNG.
			assert( rng_index < array_count(RNG_Table) )
			u32 random_choice;
			if ( door_up || door_down )
			{
				random_choice = RNG_Table[ rng_index ] % 2;
			}
			else
			{
				random_choice = RNG_Table[ rng_index ] % 3;
			}
			++ rng_index;

			b32 created_z_door = false;
			if ( random_choice == 2 )
			{
				created_z_door = true;
				if ( abs_tile_z == 0 )
				{
					door_up = true;
				}
				else
				{
					door_down = true;
				}
			}
			else if ( random_choice == 1 )
			{
				door_right = true;
			}
			else
			{
				door_top = true;
			}
			
			local_persist TileChunkPosition last_chunk = {};
			s32 chunk_index = 0;

			for (s32 tile_y = 0; tile_y < world->tiles_per_screen_y; ++ tile_y )
			{
				for ( s32 tile_x = 0; tile_x < world->tiles_per_screen_x; ++ tile_x )
				{
					s32 abs_tile_x = screen_x * world->tiles_per_screen_x + tile_x;
					s32 abs_tile_y = screen_y * world->tiles_per_screen_y + tile_y;
					
					do_once()
					{
						gs->spawn_pos.tile_x = abs_tile_x + 4;
						gs->spawn_pos.tile_y = abs_tile_y + 4;
					}

					s32 tile_value = 1;

					b32 in_middle_x = tile_x == (world->tiles_per_screen_x / 2);
					b32 in_middle_y = tile_y == (world->tiles_per_screen_y / 2);

					b32 on_right = tile_x == (world->tiles_per_screen_x - 1);
					b32 on_left  = tile_x == 0;

					b32 on_bottom = tile_y == 0;
					b32 on_top    = tile_y == (world->tiles_per_screen_y - 1);

					if ( on_left && (! in_middle_y || ! door_left ))
					{
						tile_value = 2;
					}
					if ( on_right && (! in_middle_y || ! door_right ))
					{
						tile_value = 2;
					}

					if ( on_bottom && (! in_middle_x || ! door_bottom ))
					{
						tile_value = 2;
					}
					if ( on_top && (! in_middle_x || ! door_top ))
					{
						tile_value = 2;
					}

					if ( tile_x == 6 && tile_y == 6 )
					{
						if ( door_up )
						{
							tile_value = 3;
						}
						else if ( door_down )
						{
							tile_value = 4;
						}
					}

					last_chunk = get_tile_chunk_position_for( tile_map, abs_tile_x, abs_tile_y, abs_tile_z );
					chunk_index =  
						last_chunk.z * tile_map->tile_chunks_num_y * tile_map->tile_chunks_num_x
		        	+ 	last_chunk.y * tile_map->tile_chunks_num_x 
		        	+ 	last_chunk.x;
					
					// u32 tile_value = tile_x == tile_y && tile_y % 2 ? 1 : 0;
					TileMap_set_tile_value( & state->world_arena, world->tile_map, abs_tile_x, abs_tile_y, abs_tile_z, tile_value );
				}
				
				s32 something = false;
				something++;
				if ( something )
				{
					something--;
				}
			}

			door_left   = door_right;
			door_bottom = door_top;

			if ( created_z_door )
			{
				door_down = ! door_down;
				door_up   = ! door_up;
			}
			else
			{
				door_up   = false;
				door_down = false;
			}

			door_right  = false;
			door_top    = false;

			if ( random_choice == 2 )
			{
				if ( abs_tile_z == 0 )
				{
					abs_tile_z = 1;
				}
				else
				{
					abs_tile_z = 0;
				}
			}
			else if ( random_choice == 1 )
			{
				screen_x += 1;
			}
			else
			{
				screen_y += 1;
			}
		}
	}

	// Personally made assets
	{
		StrPath path_test_bg {};
		path_test_bg.concat( platform_api->path_content, str_ascii("test_background.bmp") );
		gs->test_bg = load_bmp( platform_api, path_test_bg );

		StrPath path_mojito {};
		path_mojito.concat( platform_api->path_content, str_ascii("mojito.bmp") );
		gs->mojito = load_bmp( platform_api, path_mojito );

		StrPath path_mojito_head {};
		path_mojito_head.concat( platform_api->path_content, str_ascii("mojito_head.bmp") );
		gs->mojito_head = load_bmp( platform_api, path_mojito_head );

		StrPath path_debug_bitmap {};
		path_debug_bitmap.concat( platform_api->path_content, str_ascii("debug_bitmap2.bmp") );
		gs->debug_bitmap = load_bmp( platform_api, path_debug_bitmap );
	}

	// Offical assets
	{
		StrPath path_test_bg_hh;
		path_test_bg_hh.concat( platform_api->path_content, str_ascii("offical/test/test_background.bmp"));
		gs->test_bg_hh = load_bmp( platform_api, path_test_bg_hh );

	#define path_test "offical\\test\\"
		constexpr char const subpath_hero_front_head[] = path_test "test_hero_front_head.bmp";
		constexpr char const subpath_hero_back_head [] = path_test "test_hero_back_head.bmp";
		constexpr char const subpath_hero_right_head[] = path_test "test_hero_right_head.bmp";
		constexpr char const subpath_hero_left_head [] = path_test "test_hero_left_head.bmp";

		constexpr char const subpath_hero_front_cape[] = path_test "test_hero_front_cape.bmp";
		constexpr char const subpath_hero_back_cape [] = path_test "test_hero_back_cape.bmp";
		constexpr char const subpath_hero_left_cape [] = path_test "test_hero_left_cape.bmp";
		constexpr char const subpath_hero_right_cape[] = path_test "test_hero_right_cape.bmp";

		constexpr char const subpath_hero_front_torso[] = path_test "test_hero_front_torso.bmp";
		constexpr char const subpath_hero_back_torso [] = path_test "test_hero_back_torso.bmp";
		constexpr char const subpath_hero_left_torso [] = path_test "test_hero_left_torso.bmp";
		constexpr char const subpath_hero_right_torso[] = path_test "test_hero_right_torso.bmp";
	#undef path_test

	#define load_bmp_asset( sub_path, container )                             \
		{                                                                     \
			StrPath path {};                                                  \
			path.concat( platform_api->path_content, str_ascii( sub_path ) ); \
			container = load_bmp( platform_api, path );                       \
		}

		using hh::FacingDirection_Front;
		using hh::FacingDirection_Back;
		using hh::FacingDirection_Left;
		using hh::FacingDirection_Right;

		load_bmp_asset( subpath_hero_front_head, gs->hero_bitmaps[FacingDirection_Front].head );
		load_bmp_asset( subpath_hero_back_head,  gs->hero_bitmaps[FacingDirection_Back ].head );
		load_bmp_asset( subpath_hero_left_head,  gs->hero_bitmaps[FacingDirection_Left ].head );
		load_bmp_asset( subpath_hero_right_head, gs->hero_bitmaps[FacingDirection_Right].head );

		load_bmp_asset( subpath_hero_front_cape, gs->hero_bitmaps[FacingDirection_Front].cape );
		load_bmp_asset( subpath_hero_back_cape,  gs->hero_bitmaps[FacingDirection_Back ].cape );
		load_bmp_asset( subpath_hero_left_cape,  gs->hero_bitmaps[FacingDirection_Left ].cape );
		load_bmp_asset( subpath_hero_right_cape, gs->hero_bitmaps[FacingDirection_Right].cape );

		load_bmp_asset( subpath_hero_front_torso, gs->hero_bitmaps[FacingDirection_Front].torso );
		load_bmp_asset( subpath_hero_back_torso,  gs->hero_bitmaps[FacingDirection_Back ].torso );
		load_bmp_asset( subpath_hero_left_torso,  gs->hero_bitmaps[FacingDirection_Left ].torso );
		load_bmp_asset( subpath_hero_right_torso, gs->hero_bitmaps[FacingDirection_Right].torso );

		s32 align_x = 0;
		s32 align_y = 76;
		gs->hero_bitmaps[FacingDirection_Front].align_x = align_x;
		gs->hero_bitmaps[FacingDirection_Back ].align_x = align_x;
		gs->hero_bitmaps[FacingDirection_Left ].align_x = align_x;
		gs->hero_bitmaps[FacingDirection_Right].align_x = align_x;
		gs->hero_bitmaps[FacingDirection_Front].align_y = align_y;
		gs->hero_bitmaps[FacingDirection_Back ].align_y = align_y;
		gs->hero_bitmaps[FacingDirection_Left ].align_y = align_y;
		gs->hero_bitmaps[FacingDirection_Right].align_y = align_y;

	#undef load_bmp_asset
	}

	gs->camera_pos.tile_x = world->tiles_per_screen_x / 2;
	gs->camera_pos.tile_y = world->tiles_per_screen_y / 2;
	
	using hh::FacingDirection_Front;

	gs->camera_assigned_entity_id = gs->player_1.entity_id;
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
	
	// TODO(Ed): This can get unscoped when the handmade module impl is offloaded
	// Engine Update
	{
		assert( sizeof(EngineState) <= memory->persistent_size );
	
		state->context.delta_time = delta_time;
	
		// Engine auto_snapshot
		#if Build_Development
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
		#endif
	
		EngineActions engine_actions {};
	
		input_poll_engine_actions( input, & engine_actions );
	
	#if Build_Development
		if ( engine_actions.load_auto_snapshot )
		{
			s32 current_slot = memory->active_snapshot_slot;
			memory->active_snapshot_slot = 0;
			load_engine_snapshot( memory, platform_api );
			memory->active_snapshot_slot = current_slot;
		}
	
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
	
		#if Build_Development
			if ( engine_actions.loop_mode_game && ! memory->engine_loop_active )
			{
				process_loop_mode( & take_game_snapshot, & load_game_snapshot, memory, state, input, platform_api );
				memory->game_loop_active = memory->replay_mode > ReplayMode_Off;
			}
	
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
	}

	hh::GameState* gs = rcast( hh::GameState*, state->game_memory.persistent );
	update_player_controllers( gs, input );
	
	input_poll_player_actions( & gs->player_1, 0 );
	input_poll_player_actions( & gs->player_2, 1 );

	World*   world    = state->context.world;
	TileMap* tile_map = world->tile_map;

	update_player( & gs->player_1, delta_time, world, gs, back_buffer );
	update_player( & gs->player_2, delta_time, world, gs, back_buffer );
	
	// TODO(Ed) : Move to handmade module?
	// Camera Update
	if ( gs->camera_assigned_entity_id )
	{
		hh::Entity* entity_followed = get_entity( gs, gs->camera_assigned_entity_id );
		TileMapPos player_to_camera = subtract( entity_followed->position, gs->camera_pos );
		
		gs->camera_pos.tile_z = entity_followed->position.tile_z;
	
		if ( player_to_camera.tile_x > world->tiles_per_screen_x / 2 )
		{
			gs->camera_pos.tile_x += world->tiles_per_screen_x;
		}
		if ( player_to_camera.tile_y > world->tiles_per_screen_y / 2 )
		{
			gs->camera_pos.tile_y += world->tiles_per_screen_y;
		}
		if ( player_to_camera.tile_x < -world->tiles_per_screen_x / 2 )
		{
			gs->camera_pos.tile_x -= world->tiles_per_screen_x;
		}
		if ( player_to_camera.tile_y < -world->tiles_per_screen_y / 2 )
		{
			gs->camera_pos.tile_y -= world->tiles_per_screen_y;
		}
	}
	
	Vec2 screen_center = get_screen_center( back_buffer );
	
	// Background & Tile Render relative to assigned entity
	// void render_bg_and_level( ... )
	{
		hh::Entity* entity_followed = get_entity( gs, gs->camera_assigned_entity_id );
			
		draw_rectangle( back_buffer
			, Zero(Vec2)
			, { scast(f32, back_buffer->width), scast(f32, back_buffer->height) }
			, 1.f, 0.24f, 0.24f );
	
		draw_bitmap( back_buffer, screen_center, & gs->test_bg );
		draw_bitmap( back_buffer, screen_center, & gs->test_bg_hh );
	
		// Screen Camera
		for ( s32 relative_row = -10; relative_row < +10; ++ relative_row )
		{
			for ( s32 relative_col = -20; relative_col < +20; ++ relative_col )
			{
				s32 col = gs->camera_pos.tile_x + relative_col;
				s32 row = gs->camera_pos.tile_y + relative_row;
	
				s32 tile_id  = TileMap_get_tile_value( tile_map, col, row, gs->camera_pos.tile_z );
				f32 color[3] = { 0.15f, 0.15f, 0.15f };
				
				if ( tile_id > 1 || tile_id == 0 || (entity_followed && row == entity_followed->position.tile_y && col == entity_followed->position.tile_x) )
//				if ( tile_id > 1 )
				{
					if ( tile_id == 0 )
					{
						color[0] = 0.88f;
						color[1] = 0.22f;
						color[2] = 0.77f;
					}
				
					if ( tile_id == 2 )
					{
						color[0] = 0.42f;
						color[1] = 0.42f;
						color[2] = 0.52f;
					}
					if ( tile_id == 3 )
					{
						color[0] = 0.02f;
						color[1] = 0.02f;
						color[2] = 0.02f;
					}
					if ( tile_id == 4 )
					{
						color[0] = 0.42f;
						color[1] = 0.62f;
						color[2] = 0.42f;
					}
	
					if ( entity_followed && row == entity_followed->position.tile_y && col == entity_followed->position.tile_x )
					{
						color[0] = 0.44f;
						color[1] = 0.3f;
						color[2] = 0.3f;
					}
									
					f32 tile_size_in_pixels   = scast(f32, world->tile_size_in_pixels);
		
					Vec2 tile_pixel_size = Vec2 { tile_size_in_pixels * 0.5f, tile_size_in_pixels * 0.5f } * 0.9f;
					Pos2 center {
						screen_center.x + scast(f32, relative_col) * tile_size_in_pixels - gs->camera_pos.rel_pos.x * world->tile_meters_to_pixels,
						screen_center.y - scast(f32, relative_row) * tile_size_in_pixels + gs->camera_pos.rel_pos.y * world->tile_meters_to_pixels
					};
					Pos2 min = center - cast( Pos2, tile_pixel_size );
					Pos2 max = center + cast( Pos2, tile_pixel_size );
		
					draw_rectangle( back_buffer
					, min, max
					, color[0], color[1], color[2] );
				}
			}
		}
	}

	// Bad bitmap test
	#if 0
	{
		u32* src = gs->mojito_head.pixels;
		u32* dst = rcast(u32*, back_buffer->memory);
		for ( s32 y = 0; y < gs->mojito_head.height; ++ y )
		{
			for ( s32 x = 0; x < gs->mojito_head.width; ++ x )
			{
				*dst = *src;
				++ dst;
				++ src;
			}
		}
	}
	#endif

	// Render Entities
	{	
		s32         id     = array_count(gs->entities); 
		hh::Entity* entity = gs->entities; 
		for (; id; -- id, ++ entity )
		{
			if ( ! entity->exists )
				continue;
			
			switch ( entity->kind )
			{
				case hh::EntityKind_Hero:
					render_player( entity, world, gs, back_buffer );
			}
		}
	}
	
	// Snapshot Visual Aid
	#if Build_Development
	{
		// Auto-Snapshot percent bar
		if (1)
		{
			f32 snapshot_percent_x = ((state->auto_snapshot_timer / state->auto_snapshot_interval)) * (f32)back_buffer->width / 4.f;
			draw_rectangle( back_buffer
				, Zero(Vec2)
				, { snapshot_percent_x, 10.f }
				, 0.f, 0.15f, 0.35f );
		}
	
		// Replay indicator
		if ( memory->replay_mode == ReplayMode_Record )
		{
			hh::Entity* entity_followed = get_entity( gs, gs->camera_assigned_entity_id );
		
			// TODO(Ed) : We're prob going to need a better indicator for recording...
			draw_rectangle( back_buffer
				, { entity_followed->position.rel_pos.x + 50.f, entity_followed->position.rel_pos.y - 50.f }
				, { entity_followed->position.rel_pos.x + 10.f, entity_followed->position.rel_pos.y + 40.f }
				, 1.f, 1.f, 1.f );
		}
	}
	#endif

	// Mouse Debug
	{
		// Mouse Position
		if ( 0 )
		{
				draw_rectangle( back_buffer
					, { (f32)input->mouse->X.end,        (f32)input->mouse->Y.end }
					, { (f32)input->mouse->X.end + 10.f, (f32)input->mouse->Y.end + 10.f }
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
