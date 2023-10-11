#pragma once
#if INTELLISENSE_DIRECTIVES
#include "engine/engine.hpp"
#endif

#define NS_HANDMADE_BEGIN namespace hh {
#define NS_HANDMADE_END   }

NS_HANDMADE_BEGIN

struct Memory
{
	// Subscection of engine memory for the game to use.

	void* persistent;
	ssize persistent_size;

	// void* Frame;
	// u64   FrameSize;

	void* transient;
	ssize transient_size;

	ssize total_size()
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

	engine::TileMapPosition position;

	b32 mid_jump;
	f32 jump_time;
};

struct PlayerActions
{
	s32 player_x_move_digital;
	s32 player_y_move_digital;
	f32 player_x_move_analog;
	f32 player_y_move_analog;

	b32 sprint;
	b32 jump;
};

struct GameState
{
	PlayerState player_state;
};

NS_HANDMADE_END

