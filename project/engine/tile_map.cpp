#if INTELLISENSE_DIRECTIVES
#include "tile_map.hpp"
#endif

NS_ENGINE_BEGIN

inline
TileMapPos subtract( TileMapPos pos_a, TileMapPos pos_b )
{
	TileMapPos result = {
		pos_a.rel_pos - pos_b.rel_pos,

		pos_a.tile_x - pos_b.tile_x,
		pos_a.tile_y - pos_b.tile_y,

		// TODO(Ed) : Think about how to handle z...
		pos_a.tile_z - pos_b.tile_z
	};
	
	return result;
}

// TODO(Ed) : Consider moving (Casey wants to)
inline
void cannonicalize_coord( TileMap* tile_map, s32* tile_coord, f32* pos_coord )
{
	assert( tile_map   != nullptr );
	assert( tile_coord != nullptr );

	f32 tile_size = scast(f32, tile_map->tile_side_in_meters);

	// Note(Ed) : World is assumed to be a "torodial topology"
	// Note(Ed) : Above note is for Casey's setup, mine uses signed coorindates for the tiles... so not sure.
	s32 offset         = round( (* pos_coord) / tile_size );
	s32 new_tile_coord = (* tile_coord) + offset;
	f32 new_pos_coord  = (* pos_coord)  - scast(f32, offset) * tile_size;

	assert( new_pos_coord >= -tile_size * 0.5f );
	assert( new_pos_coord <=  tile_size * 0.5f );

	(* tile_coord) = new_tile_coord;
	(* pos_coord)  = new_pos_coord;
}

// TODO(Ed) : Consider moving (Casey wants to)
inline
TileMapPos recannonicalize_position( TileMap* tile_map, TileMapPos pos )
{
	assert( tile_map != nullptr );

	TileMapPos result = pos;
	cannonicalize_coord( tile_map, & result.tile_x, & result.rel_pos.x );
	cannonicalize_coord( tile_map, & result.tile_y, & result.rel_pos.y );
	return result;
}

inline
void offset( TileMap* tile_map, TileMapPos& map_pos, Vec2 rel_offset )
{
	map_pos.rel_pos += rel_offset;
	map_pos          = recannonicalize_position( tile_map, map_pos);
}
                                                                    
inline
u32 TileChunk_get_tile_value( TileChunk* tile_chunk, TileMap* tile_map, s32 x, s32 y )
{
	assert( tile_map   != nullptr );
	assert( tile_chunk != nullptr );
	assert( x < tile_map->chunk_dimension );
	assert( y < tile_map->chunk_dimension );

	u32 value = tile_chunk->tiles[ (y * tile_map->chunk_dimension) + x ];
	return value;
}

inline
void TileChunk_set_tile_value( TileChunk* tile_chunk, TileMap* tile_map, s32 x, s32 y, s32 value)
{
	assert( tile_map   != nullptr );
	assert( tile_chunk != nullptr );
	assert( x < tile_map->chunk_dimension );
	assert( y < tile_map->chunk_dimension );
	
	tile_chunk->tiles[ (y * tile_map->chunk_dimension) + x ] = value;
}

inline
TileChunk* TileMap_get_chunk( TileMap* tile_map, TileChunkPosition chunk_pos )
{
	TileChunk* chunk = nullptr;                     
	
	if (	chunk_pos.x >= 0 && chunk_pos.x < tile_map->tile_chunks_num_x
	    &&	chunk_pos.y >= 0 && chunk_pos.y < tile_map->tile_chunks_num_y
	    &&	chunk_pos.z >= 0 && chunk_pos.z < tile_map->tile_chunks_num_z )
	{
		chunk = & tile_map->chunks[ 
		          chunk_pos.z * tile_map->tile_chunks_num_y * tile_map->tile_chunks_num_x
		        + chunk_pos.y * tile_map->tile_chunks_num_x 
		        + chunk_pos.x ];
		
		return chunk;
	}                         

	return chunk;
}

inline
TileChunkPosition get_tile_chunk_position_for( TileMap* tile_map, s32 abs_tile_x, s32 abs_tile_y, s32 abs_tile_z )
{
	assert( tile_map != nullptr );

	TileChunkPosition chunk_pos {};
	chunk_pos.x = abs_tile_x >> tile_map->chunk_shift;
	chunk_pos.y = abs_tile_y >> tile_map->chunk_shift;
	chunk_pos.z = abs_tile_z;
	
	// Correct negative values
	s32 neg_mask = (1 << (sizeof(s32) * 8 - 1));
	s32 is_neg_x = (chunk_pos.x & neg_mask) < 0;
	s32 is_neg_y = (chunk_pos.y & neg_mask) < 0;
	s32 offset_x = is_neg_x * tile_map->tile_chunks_num_x;
	s32 offset_y = is_neg_y * tile_map->tile_chunks_num_y;
	chunk_pos.x = offset_x + chunk_pos.x;
	chunk_pos.y = offset_y + chunk_pos.y;
	
//	chunk_pos.tile_x  = (abs_tile_x * (-1 * is_neg_x)) & tile_map->chunk_mask;
//	chunk_pos.tile_y  = (abs_tile_y * (-1 * is_neg_y)) & tile_map->chunk_mask;

	chunk_pos.tile_x  = abs_tile_x & tile_map->chunk_mask;
	chunk_pos.tile_y  = abs_tile_y & tile_map->chunk_mask;

	return chunk_pos;
}

inline
u32 TileMap_get_tile_value( TileMap* tile_map, s32 tile_x, s32 tile_y, s32 tile_z )
{
	assert( tile_map != nullptr );

	s32 value = 0;

	TileChunkPosition chunk_pos = get_tile_chunk_position_for( tile_map, tile_x, tile_y, tile_z );
	TileChunk*        chunk     = TileMap_get_chunk( tile_map, chunk_pos );

	if ( chunk && chunk->tiles )
		value = TileChunk_get_tile_value( chunk, tile_map, chunk_pos.tile_x, chunk_pos.tile_y );
	return value;
}

inline
u32 TileMap_get_tile_value( TileMap* tile_map, TileMapPos position )
{
	u32 value = TileMap_get_tile_value( tile_map, position.tile_x, position.tile_y, position.tile_z );
	return value;
}

internal
b32 TileMap_is_tile_value_empty(s32 tile_value)
{
	b32 
	is_empty  = tile_value == 1;
	is_empty |= tile_value == 3;
	is_empty |= tile_value == 4;
	return is_empty;
}

internal
b32 TileMap_is_point_empty( TileMap* tile_map, TileMapPos position )
{
	assert( tile_map != nullptr );

	u32 chunk_value = TileMap_get_tile_value( tile_map, position.tile_x, position.tile_y, position.tile_z );
	
	return TileMap_is_tile_value_empty( chunk_value );
}

internal
void TileMap_set_tile_value( MemoryArena* arena, TileMap* tile_map, s32 abs_tile_x, s32 abs_tile_y, s32 abs_tile_z, s32 value )
{
	TileChunkPosition chunk_pos = get_tile_chunk_position_for( tile_map, abs_tile_x, abs_tile_y, abs_tile_z );
	TileChunk*        chunk     = TileMap_get_chunk( tile_map, chunk_pos );

	assert( chunk != nullptr );
	
	if ( chunk->tiles == nullptr )
	{
		ssize num_tiles = tile_map->chunk_dimension * tile_map->chunk_dimension;
		chunk->tiles = arena->push_array( s32, num_tiles );
				
		for ( ssize tile_index = 0; tile_index < num_tiles; ++ tile_index )
		{
			chunk->tiles[ tile_index ] = 1;
		}
	}
	
	TileChunk_set_tile_value( chunk, tile_map, chunk_pos.tile_x, chunk_pos.tile_y, value );
}

internal
b32 TileMap_are_on_same_tile( TileMapPos* pos_a, TileMapPos* pos_b )
{
	b32 result =
		pos_a->tile_x == pos_b->tile_x
	&&  pos_a->tile_y == pos_b->tile_y
	&&  pos_a->tile_z == pos_b->tile_z;
	return result;
}

inline
TileMapPos centered_tile_point( s32 tile_x, s32 tile_y, s32 tile_z )
{
	TileMapPos result {};
	result.tile_x = tile_x;
	result.tile_y = tile_y;
	result.tile_z = tile_z;
	return result;
}

NS_ENGINE_END
