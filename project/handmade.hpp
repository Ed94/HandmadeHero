#pragma once

#include "engine/engine.hpp"

#define NS_HANDMADE_BEGIN namespace handmade {
#define NS_HANDMADE_END   }

NS_HANDMADE_BEGIN

struct Memory
{
	// Subscection of engine memory for the game to use.

	void* Persistent;
	u64   PersistentSize;

	// void* Frame;
	// u64   FrameSize;

	void* Transient;
	u64   TransientSize;
};

// We want a 'binding' to have multiple binds to active it (most likely)
struct Actionable
{
	char const*                Name;
	engine::InputBindCallback* Binds;
	s32                        NumBinds;
	char _PAD_[4];
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
	engine::ControllerState* Controller;

	// Possilby some other stuff in the future.
};

NS_HANDMADE_END
