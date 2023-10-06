#pragma once

#include "engine/engine.hpp"

#define NS_HANDMADE_BEGIN namespace hh {
#define NS_HANDMADE_END   }

NS_HANDMADE_BEGIN

struct Memory
{
	// Subscection of engine memory for the game to use.

	void* persistent;
	u64   persistent_size;

	// void* Frame;
	// u64   FrameSize;

	void* transient;
	u64   transient_size;

	u64 total_size()
	{
		return persistent_size + transient_size;
	}
};

// We want a 'binding' to have multiple binds to active it (most likely)
struct Actionable
{
	char const*                name;
	engine::InputBindCallback* binds;
	s32                        num_binds;
};

struct ActionableMode
{

};

/*
	Platform Layer:

	Controller : Keyboard & Mouse, XPad, DSPad

	---VV---

	Engine Layer:

	InputBinding callbacks (per-game-logic frame basis)
	Push/Pop input modes (binding sets)

	---VV---

	Game Layer:

	Actionables : Binding Sets where a raw input, or input interpretation leads to an player action.
	ActionSet   : Actionables.Push/Pop -> Input.Push/Pop ?
	Player : Controller, Actionables, ActionSets
*/

struct Player
{
	// So far just has an assigned controller.
	engine::ControllerState* controller;

	// Possilby some other stuff in the future.
};

struct PlayerState
{
	f32 width;
	f32 height;

	// TODO(Ed) : Should this be canonical position now?
	f32 pos_x;
	f32 pos_y;

	b32 mid_jump;
	f32 jump_time;
};

struct PlayerActions
{
	s32 player_x_move_digital;
	s32 player_y_move_digital;
	f32 player_x_move_analog;
	f32 player_y_move_analog;

	b32 jump = false;
};

struct GameState
{
	s32 tile_map_x;
	s32 tile_map_y;

	PlayerState player_state;
};

NS_HANDMADE_END
