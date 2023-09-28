/*
	Services the engine provides to the platform layer
*/

#pragma once

#include "platform/platform.hpp"

#define NS_ENGINE_BEGIN namespace engine {
#define NS_ENGINE_END }

NS_ENGINE_BEGIN

struct Clocks
{
	// TODO(Ed) : Clock values...
	f32 SecondsElapsed;
};

struct Memory
{
	// All memory for the engine is required to be zero initialized.

	// Wiped on shutdown
	void* Persistent;
	u64   PersistentSize;

	// Wiped on a per-frame basis
	// void* Frame;
	// u64   FrameSize;

	// Wiped whenever the engine wants to?
	void* Transient;
	u64   TransientSize;
};

struct OffscreenBuffer
{
	void*      Memory; // Lets use directly mess with the "pixel's memory buffer"
	u32        Width;
	u32        Height;
	u32        Pitch;
	u32        BytesPerPixel;
};

// TODO : Will be gutting this once we have other stuff lifted.
struct AudioBuffer
{
	s16* Samples;
	u32  RunningSampleIndex;
	s32  SamplesPerSecond;
	s32  NumSamples;
};

struct DigitalBtn
{
	s32 HalfTransitions;
	b32 EndedDown;
};

struct AnalogAxis
{
	f32 Start;
	f32 End;
	f32 Min;
	f32 Max;

	// Platform doesn't provide this, we process in the engine layer.
	f32 Average;
};

struct AnalogStick
{
	AnalogAxis X;
	AnalogAxis Y;
};

union KeyboardState
{
	DigitalBtn Keys[12];
	struct {
		DigitalBtn Row_1;
		DigitalBtn Q;
		DigitalBtn E;
		DigitalBtn W;
		DigitalBtn A;
		DigitalBtn S;
		DigitalBtn D;
		DigitalBtn L;
		DigitalBtn Escape;
		DigitalBtn Backspace;
		DigitalBtn Up;
		DigitalBtn Down;
		DigitalBtn Left;
		DigitalBtn Right;
		DigitalBtn Space;
		DigitalBtn Pause;
	};
};

struct MousesState
{
	DigitalBtn Left;
	DigitalBtn Middle;
	DigitalBtn Right;
};

struct XInputPadState
{
	struct
	{
		AnalogStick Left;
		AnalogStick Right;
	} Stick;

	AnalogAxis LeftTrigger;
	AnalogAxis RightTrigger;

	union {
		DigitalBtn Btns[14];
		struct {
			struct {
				DigitalBtn Up;
				DigitalBtn Down;
				DigitalBtn Left;
				DigitalBtn Right;
			} DPad;
			DigitalBtn A;
			DigitalBtn B;
			DigitalBtn X;
			DigitalBtn Y;
			DigitalBtn Back;
			DigitalBtn Start;
			DigitalBtn LeftShoulder;
			DigitalBtn RightShoulder;
		};
	};
};

struct DualsensePadState
{
	struct
	{
		AnalogStick Left;
		AnalogStick Right;
	} Stick;

	AnalogAxis L2;
	AnalogAxis R2;

	union {
		DigitalBtn Btns[14];
		struct {
			struct {
				DigitalBtn Up;
				DigitalBtn Down;
				DigitalBtn Left;
				DigitalBtn Right;
			} DPad;
			DigitalBtn X;
			DigitalBtn Circle;
			DigitalBtn Square;
			DigitalBtn Triangle;
			DigitalBtn Share;
			DigitalBtn Options;
			DigitalBtn L1;
			DigitalBtn R1;
		};
	};
};

struct ControllerState
{
	KeyboardState*     Keyboard;
	MousesState*       Mouse;
	XInputPadState*    XPad;
	DualsensePadState* DSPad;
};

struct ControllerStateSnapshot
{
	KeyboardState     Keyboard;
	MousesState       Mouse;
	XInputPadState    XPad;
	DualsensePadState DSPad;
};

struct InputState
{
	ControllerState Controllers[4];
};

struct InputStateSnapshot
{
	ControllerStateSnapshot Controllers[4];
};

using InputBindCallback             = void( void* );
using InputBindCallback_DigitalBtn  = void( engine::DigitalBtn*  Button );
using InputBindCallback_AnalogAxis  = void( engine::AnalogAxis*  Axis );
using InputBindCallback_AnalogStick = void( engine::AnalogStick* Stick );

struct InputMode
{
	InputBindCallback* Binds;
	s32                NumBinds;
};

void input_mode_pop( InputMode* mode );
void input_mode_pop( InputMode* mode );

#if 0
struct RecordedInput
{
	s32         Num;
	InputState* Stream;
};
#endif

NS_ENGINE_END
