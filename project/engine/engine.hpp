/*
	Services the engine provides to the platform layer
*/

#pragma once

#if INTELLISENSE_DIRECTIVES
#include "platform/platform.hpp"
#include "engine_module.hpp"
#include "tile_map.hpp"
#endif

NS_ENGINE_BEGIN

struct Clocks
{
	// TODO(Ed) : Clock values...
	f32 seconds_elapsed;
};

struct ThreadContext
{
	u32 placeholder;
};

struct MemorySnapshot
{
	StrPath file_path;
	void*   opaque_handle;
	void*   opaque_handle_2;
	void*   memory;
	u64     age;
};


enum ReplayMode : s32
{
	ReplayMode_Off,
	ReplayMode_Record,
	ReplayMode_Playback
};

struct ReplayData
{
	static constexpr
	s32 Num_Snapshot_Slots = 3;
	// Abuse RAM to store snapshots of the Engine or Game state.
	MemorySnapshot snapshots[ Num_Snapshot_Slots ];
	s32 active_snapshot_slot;

	// Recording and playback info is the same for either engine or game.

	ReplayMode replay_mode;
	platform::File active_input_replay_file;

	// Engine-wide recording & playback loop.
	s32 engine_loop_active;
	s32 game_loop_active;
};

struct Memory
{
	// All memory for the engine is required to be zero initialized.

	// Wiped on shutdown
	Byte* persistent;
	ssize persistent_size;

	// Wiped on a per-frame basis
	// void* Frame;
	// u64   FrameSize;

	// Wiped whenever the engine wants to?
	Byte* transient;
	ssize transient_size;

	// TODO(Ed) : Move this to state & replay archive definitions?
	#if Build_Development
		static constexpr
		s32 Num_Snapshot_Slots = 3;
		// Abuse RAM to store snapshots of the Engine or Game state.
		MemorySnapshot snapshots[ Num_Snapshot_Slots ];
		s32 active_snapshot_slot;

		// Recording and playback info is the same for either engine or game.

		ReplayMode replay_mode;
		platform::File active_input_replay_file;

		// Engine-wide recording & playback loop.
		s32 engine_loop_active;
		s32 game_loop_active;

		//ReplayData replay;
	#endif

	// The game will have 1/2 of persistent's memory available ot it.
	static constexpr
	ssize game_memory_factor = 2;

	ssize engine_persistent_size()
	{
		return persistent_size - persistent_size / game_memory_factor;
	}

	ssize total_size()
	{
		return persistent_size + transient_size;
	}
};

// Allocator member-interface macros
#define push_struct( type )     push__struct<type>()
#define push_array( type, num ) push__array<type>( num )

struct MemoryArena
{
	Byte* storage;
	ssize size;
	ssize used;

	static
	void init( MemoryArena* arena, ssize size, Byte* storage )
	{
		arena->storage = storage;
		arena->size    = size;
		arena->used    = 0;
	}

	template< typename Type >
	Type* push__struct()
	{
		ssize type_size = sizeof( Type );
		assert( used + type_size <= size );

		Type* result = rcast(Type*, storage + used);
		used += type_size;

		return result;
	}

	template< typename Type >
	Type* push__array( ssize num )
	{
		ssize mem_amount = sizeof( Type ) * num;
		assert( used + mem_amount <= size );

		Type* result = rcast(Type*, storage + used);
		used += mem_amount;

		return result;
	}
};

struct OffscreenBuffer
{
	void*      memory; // Lets use directly mess with the "pixel's memory buffer"
	u32        width;
	u32        height;
	u32        pitch;
	u32        bytes_per_pixel;
};

// TODO : Will be gutting this once we have other stuff lifted.
struct AudioBuffer
{
	s16* samples;
	u32  running_sample_index;
	s32  samples_per_second;
	s32  num_samples;
};

struct World
{
	f32 tile_lower_left_x;
	f32 tile_lower_left_y;

	f32 tile_meters_to_pixels;
	s32 tile_size_in_pixels;

	TileMap* tile_map;
};

#pragma pack(push, 1)
struct BitmapHeaderPacked
{
	u16 file_type;
	u32 file_size;
	u16 _reserved_1_;
	u16 _reserved_2_;
	u32 bitmap_offset;
	u32 size;
	s32 width;
	s32 height;
	u16 planes;
	u16 bits_per_pixel;
};
#pragma pack(pop)

struct Bitmap
{
	u32* pixels;
	s32  width;
	s32  height;
	u32  bits_per_pixel;
	u32  size;
};

NS_ENGINE_END
