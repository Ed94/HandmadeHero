#if INTELLISENSE_DIRECTIVES
#include "tile_map.hpp"
#endif

NS_ENGINE_BEGIN

inline
void cannonicalize_coord( TileMap* tile_map, u32* tile_coord, f32* pos_coord )
{
	assert( tile_map != nullptr );
	assert( tile_coord != nullptr );

	f32 tile_size = scast(f32, tile_map->tile_size_in_meters);

	// Note(Ed) : World is assumed to be a "torodial topology"
	s32 offset         = round( (* pos_coord) / tile_size );
	u32 new_tile_coord = (* tile_coord) + offset;
	f32 new_pos_coord  = (* pos_coord)  - scast(f32, offset) * tile_size;

	assert( new_pos_coord >= -tile_size * 0.5f );
	assert( new_pos_coord <=  tile_size * 0.5f );

	(* tile_coord) = new_tile_coord;
	(* pos_coord)  = new_pos_coord;
}

inline
TileMapPosition recannonicalize_position( TileMap* tile_map, TileMapPosition pos )
{
	assert( tile_map != nullptr );

	TileMapPosition result = pos;
	cannonicalize_coord( tile_map, & result.tile_x, & result.x );
	cannonicalize_coord( tile_map, & result.tile_y, & result.y );
	return result;
}
                                                                    
inline
u32 TileChunk_get_tile_value( TileChunk* tile_chunk, TileMap* tile_map, u32 x, u32 y )
{
	assert( tile_map      != nullptr );
	assert( tile_chunk != nullptr );
	assert( x < tile_map->chunk_dimension );
	assert( y < tile_map->chunk_dimension );

	u32 value = tile_chunk->tiles[ (y * tile_map->chunk_dimension) + x ];
	return value;
}

inline
void TileChunk_set_tile_value( TileChunk* tile_chunk, TileMap* tile_map, u32 x, u32 y, u32 value)
{
	assert( tile_map   != nullptr );
	assert( tile_chunk != nullptr );
	assert( x < tile_map->chunk_dimension );
	assert( y < tile_map->chunk_dimension );
	
	tile_chunk->tiles[ (y * tile_map->chunk_dimension) + x ] = value;
}

inline
TileChunk* TileMap_get_chunk( TileMap* tile_map, s32 tile_chunk_x, s32 tile_chunk_y, s32 tile_chunk_z )
{
	TileChunk* chunk = nullptr;                     
	
	if (	tile_chunk_x >= 0 && tile_chunk_x < scast(s32, tile_map->tile_chunks_num_x)
	    &&	tile_chunk_y >= 0 && tile_chunk_y < scast(s32, tile_map->tile_chunks_num_y)
	    &&	tile_chunk_z >= 0 && tile_chunk_z < scast(s32, tile_map->tile_chunks_num_z) )
	{
		chunk = & tile_map->chunks[ 
		          tile_chunk_z * tile_map->tile_chunks_num_y * tile_map->tile_chunks_num_x
		        + tile_chunk_y * tile_map->tile_chunks_num_x 
		        + tile_chunk_x ];
	}                         

	return chunk;
}

inline
TileChunkPosition get_tile_chunk_position_for( TileMap* tile_map, u32 abs_tile_x, u32 abs_tile_y, u32 abs_tile_z )
{
	assert( tile_map != nullptr );

	TileChunkPosition chunk_pos {};
	chunk_pos.tile_chunk_x = abs_tile_x >> tile_map->chunk_shift;
	chunk_pos.tile_chunk_y = abs_tile_y >> tile_map->chunk_shift;
	chunk_pos.tile_chunk_z = abs_tile_z;
	chunk_pos.tile_x       = abs_tile_x &  tile_map->chunk_mask;
	chunk_pos.tile_y       = abs_tile_y &  tile_map->chunk_mask;

	return chunk_pos;
}

u32 TileMap_get_tile_value( TileMap* tile_map, u32 tile_x, u32 tile_y, u32 tile_z )
{
	assert( tile_map != nullptr );

	u32 value = 0;

	TileChunkPosition chunk_pos = get_tile_chunk_position_for( tile_map, tile_x, tile_y, tile_z );
	TileChunk*        chunk     = TileMap_get_chunk( tile_map, chunk_pos.tile_chunk_x, chunk_pos.tile_chunk_y, chunk_pos.tile_chunk_z );

	if ( chunk && chunk->tiles )
		value = TileChunk_get_tile_value( chunk, tile_map, chunk_pos.tile_x, chunk_pos.tile_y );
	return value;
}

internal
b32 TileMap_is_point_empty( TileMap* tile_map, TileMapPosition position )
{
	assert( tile_map != nullptr );

	u32 chunk_value = TileMap_get_tile_value( tile_map, position.tile_x, position.tile_y, position.tile_z );
	b32 is_empty    = chunk_value == 1;
	return is_empty;
}

internal
void TileMap_set_tile_value( MemoryArena* arena, TileMap* tile_map, u32 abs_tile_x, u32 abs_tile_y, u32 abs_tile_z, u32 value )
{
	TileChunkPosition chunk_pos = get_tile_chunk_position_for( tile_map, abs_tile_x, abs_tile_y, abs_tile_z );
	TileChunk*        chunk     = TileMap_get_chunk( tile_map, chunk_pos.tile_chunk_x, chunk_pos.tile_chunk_y, chunk_pos.tile_chunk_z );

	assert( chunk != nullptr );
	
	if ( chunk->tiles == nullptr )
	{
		ssize num_tiles = tile_map->chunk_dimension * tile_map->chunk_dimension;
		chunk->tiles = arena->push_array( u32, num_tiles );
				
		for ( ssize tile_index = 0; tile_index < num_tiles; ++ tile_index )
		{
			chunk->tiles[ tile_index ] = 1;
		}
	}
	
	TileChunk_set_tile_value( chunk, tile_map, chunk_pos.tile_x, chunk_pos.tile_y, value );
}

NS_ENGINE_END
