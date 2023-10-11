/*


*/
#pragma once

#if INTELLISENSE_DIRECTIVES
#include "platform/platform.hpp"
#include "engine_module.hpp"
#endif

NS_ENGINE_BEGIN

struct TileChunk
{
	u32* tiles;
};

/*
	This is a "backend" transient datatype for handling lookup of tile data from "chunks" of tiles.
*/
struct TileChunkPosition
{
	u32 tile_chunk_x;
	u32 tile_chunk_y;

	// "Chunk-relative (x, y)

	u32 tile_x;
	u32 tile_y;
};

struct TileMap
{
	// TODO(Ed) : Beginner's sparseness
	s32 tile_chunks_num_x;
	s32 tile_chunks_num_y;

	s32 tile_size_in_pixels;
	f32 tile_size_in_meters;
	f32 tile_meters_to_pixels;

	u32 chunk_shift;
	u32 chunk_mask;
	u32 chunk_dimension;

	TileChunk* chunks;
};

struct TileMapPosition
{
	// TODO(Ed) : Should this be from the center of the tile?

	f32 x;
	f32 y;

	// "World-relative (x, y), AKA: Absolute Position
	// Fixed point tile locations.
	// High bits are the tile-chunk index, and the low bits are the tile index in the chunk.

	u32 tile_x;
	u32 tile_y;
};

NS_ENGINE_END
