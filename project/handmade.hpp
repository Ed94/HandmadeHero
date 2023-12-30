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

struct ControllerState
{
	s32 xpad_id;
	s32 ds_pad_id;

	engine::KeyboardState*     keyboard;
	engine::MousesState*       mouse;
	engine::XInputPadState*    xpad;
	engine::DualsensePadState* ds_pad;
};

enum EHeroBitmapsDirection : u32
{
	HeroBitmaps_Front,
	HeroBitmaps_Back,
	HeroBitmaps_Left,
	HeroBitmaps_Right
};

struct HeroBitmaps
{
	using Bitmap = engine::Bitmap;

	s32 align_x;
	s32 align_y;

	Bitmap head;
	Bitmap cape;
	Bitmap torso;
};

struct Entity
{
	b32 exists;
	
	f32 width;
	f32 height;
	
	engine::TileMapPos position;
	Vel2               move_velocity;
};

struct PlayerState
{
	b32 mid_jump;
	f32 jump_time;
	
	EHeroBitmapsDirection hero_direction;
};

struct PlayerActions
{
	s32 player_x_move_digital;
	s32 player_y_move_digital;
	f32 player_x_move_analog;
	f32 player_y_move_analog;

	b32 sprint;
	b32 jump;
	
	b32 join;
};

struct Player
{
	// So far just has an assigned controller.
	ControllerState controller;
	
	s32             entity_id;
	//Entity          entity;
	PlayerState     state;
	
	// TODO(Ed) : If the player ever gets different actions depending on a context, change it here.
	PlayerActions actions;
};

struct GameState
{
	Entity entities[256];

	Player player_1;
	Player player_2;

//	PlayerState player_state;
//	PlayerState player_state_2;

	using Bitmap = engine::Bitmap;

	Bitmap debug_bitmap;
	Bitmap test_bg;
	Bitmap mojito;
	Bitmap mojito_head;

	Bitmap test_bg_hh;
	
	// TODO(Ed) : Allow splitscreen?
	s32 camera_assigned_entity_id;
	engine::TileMapPos camera_pos;

	HeroBitmaps hero_bitmaps[4];
};

NS_HANDMADE_END

