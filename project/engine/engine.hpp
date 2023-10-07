/*
	Services the engine provides to the platform layer
*/

#pragma once

#if INTELLISENSE_DIRECTIVES
#include "platform/platform.hpp"
#endif

#define NS_ENGINE_BEGIN namespace engine {
#define NS_ENGINE_END }

NS_ENGINE_BEGIN

enum ReplayMode : s32
{
	ReplayMode_Off,
	ReplayMode_Record,
	ReplayMode_Playback
};

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

struct Memory
{
	// All memory for the engine is required to be zero initialized.

	// Wiped on shutdown
	void* persistent;
	u64   persistent_size;

	// Wiped on a per-frame basis
	// void* Frame;
	// u64   FrameSize;

	// Wiped whenever the engine wants to?
	void* transient;
	u64   transient_size;

	// TODO(Ed) : Move this crap to state & replay archive definitions?
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
	#endif

	u64 total_size()
	{
		return persistent_size + transient_size;
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

struct DigitalBtn
{
	s32 half_transitions;
	b32 ended_down;
};

struct AnalogAxis
{
	f32 start;
	f32 end;
	f32 min;
	f32 max;

	// Platform doesn't provide this, we process in the engine layer.
	f32 average;
};

struct AnalogStick
{
	AnalogAxis X;
	AnalogAxis Y;
};

union KeyboardState
{
	DigitalBtn keys[12];
	struct {
		DigitalBtn _1;
		DigitalBtn _2;
		DigitalBtn _3;
		DigitalBtn _4;

		DigitalBtn Q;
		DigitalBtn E;
		DigitalBtn W;
		DigitalBtn A;
		DigitalBtn S;
		DigitalBtn D;
		DigitalBtn K;
		DigitalBtn L;
		DigitalBtn escape;
		DigitalBtn backspace;
		DigitalBtn up;
		DigitalBtn down;
		DigitalBtn left;
		DigitalBtn right;
		DigitalBtn space;
		DigitalBtn pause;
		DigitalBtn left_alt;
		DigitalBtn right_alt;
		DigitalBtn right_shift;
		DigitalBtn left_shift;
	};
};

struct MousesState
{
	DigitalBtn left;
	DigitalBtn middle;
	DigitalBtn right;

	AnalogAxis X;
	AnalogAxis Y;
	AnalogAxis vertical_wheel;
	AnalogAxis horizontal_wheel;
};

struct XInputPadState
{
	struct
	{
		AnalogStick left;
		AnalogStick right;
	} stick;

	AnalogAxis left_trigger;
	AnalogAxis right_trigger;

	union {
		DigitalBtn btns[14];
		struct {
			struct {
				DigitalBtn up;
				DigitalBtn down;
				DigitalBtn left;
				DigitalBtn right;
			} dpad;
			DigitalBtn A;
			DigitalBtn B;
			DigitalBtn X;
			DigitalBtn Y;
			DigitalBtn back;
			DigitalBtn start;
			DigitalBtn left_shoulder;
			DigitalBtn right_shoulder;
		};
	};
};

struct DualsensePadState
{
	struct
	{
		AnalogStick left;
		AnalogStick right;
	} stick;

	AnalogAxis L2;
	AnalogAxis R2;

	union {
		DigitalBtn btns[14];
		struct {
			struct {
				DigitalBtn up;
				DigitalBtn down;
				DigitalBtn left;
				DigitalBtn right;
			} dpad;
			DigitalBtn cross;
			DigitalBtn circle;
			DigitalBtn square;
			DigitalBtn triangle;
			DigitalBtn share;
			DigitalBtn options;
			DigitalBtn L1;
			DigitalBtn R1;
		};
	};
};

struct ControllerState
{
	KeyboardState*     keyboard;
	MousesState*       mouse;
	XInputPadState*    xpad;
	DualsensePadState* ds_pad;
};

struct ControllerStateSnapshot
{
	KeyboardState     keyboard;
	MousesState       mouse;
	XInputPadState    xpad;
	DualsensePadState ds_pad;
};

struct InputState
{
	ControllerState controllers[4];
};

struct InputStateSnapshot
{
	ControllerStateSnapshot controllers[4];
};

using InputBindCallback             = void( void* );
using InputBindCallback_DigitalBtn  = void( engine::DigitalBtn*  button );
using InputBindCallback_AnalogAxis  = void( engine::AnalogAxis*  axis );
using InputBindCallback_AnalogStick = void( engine::AnalogStick* stick );

struct InputMode
{
	InputBindCallback* binds;
	s32                num_binds;
};

void input_mode_pop( InputMode* mode );
void input_mode_pop( InputMode* mode );

#if 0
struct RecordedInput
{
	s32         num;
	InputState* stream;
};
#endif

struct TileMap
{
	u32* tiles;
};

struct World
{
	f32 tile_size_in_meters;
	s32 tile_size_in_pixels;
	
	f32 tile_upper_left_x;
	f32 tile_upper_left_y;

	s32 num_tiles_x; // Number of tiles on the x-axis for a tilemap.
	s32 num_tiles_y; // Number of tiles on the y-axis for a tilemap.

	// TODO(Ed) : Beginner's sparseness
	s32 tilemaps_num_x;
	s32 tilemaps_num_y;

	TileMap* tile_maps;
};

struct CanonPosition
{
	// TODO(Ed): Convert these to resolution-indenpent rep of world units (a proper vector space?))
	// Note: Tile-Relative position
	f32 x;
	f32 y;

	/* TODO(Ed) :
	Take the tile map x & y and the tile x & y
	where there is some low bits for the tile index
	and the high bits are the tile "page".
	*/
	s32 tile_map_x;
	s32 tile_map_y;

	s32 tile_x;
	s32 tile_y;
};

// TODO(Ed) : Is this necessary?
struct RawPosition
{
	// Note: TileMap-Relative position
	f32 x;
	f32 y;

	s32 tile_map_x;
	s32 tile_map_y;
};

NS_ENGINE_END
