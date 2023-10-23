/*


*/
#pragma once

#if INTELLISENSE_DIRECTIVES
#include "platform/platform.hpp"
#include "engine_module.hpp"
#endif

NS_ENGINE_BEGIN

// TODO(Ed) : I switch the tile coordinates to signed values, I'm clamping rn to force positive

struct TileChunk
{
	s32* tiles;
};

/*
	This is a "backend" transient datatype for handling lookup of tile data from "chunks" of tiles.
*/
struct TileChunkPosition
{
	s32 tile_chunk_x;
	s32 tile_chunk_y;
	s32 tile_chunk_z;

	// "Chunk-relative (x, y)

	s32 tile_x;
	s32 tile_y;
};

struct TileMap
{
	// TODO(Ed) : Beginner's sparseness
	s32 tile_chunks_num_x;
	s32 tile_chunks_num_y;
	s32 tile_chunks_num_z;

	f32 tile_size_in_meters;

	// TODO(Ed) : Real sparseness ? (not use the giant pointer array)
	s32 chunk_shift;
	s32 chunk_mask;
	s32 chunk_dimension;

	TileChunk* chunks;
};

struct TileMapPos
{
	// Note(Ed) : Relative position from tile center.
	Vec2 rel_pos;

	// "World-relative (x, y), AKA: Absolute Position
	// Fixed point tile locations.
	// High bits are the tile-chunk index, and the low bits are the tile index in the chunk.

	s32 tile_x;
	s32 tile_y;
	s32 tile_z;
};

NS_ENGINE_END
