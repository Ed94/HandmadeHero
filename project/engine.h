/*
	Services the engine provides to the platform layer
*/

#pragma once

#include "platform.h"

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
struct SoundBuffer
{
	s16* Samples;
	u32  RunningSampleIndex;
	s32  SamplesPerSecond;
	s32  NumSamples;
	char _PAD_[4];
};

struct DigitalBtn
{
	s32 HalfTransitions;
	b32 State;
};
#define DigitalBtn_Up   0
#define DigitalBtn_Down 1

struct AnalogAxis
{
	f32 Start;
	f32 End;
	f32 Min;
	f32 Max;
};

struct AnalogStick
{
	AnalogAxis X;
	AnalogAxis Y;
};

struct KeyboardState
{
	DigitalBtn Q;
	DigitalBtn E;
	DigitalBtn W;
	DigitalBtn A;
	DigitalBtn S;
	DigitalBtn D;
	DigitalBtn Esc;
	DigitalBtn Up;
	DigitalBtn Down;
	DigitalBtn Left;
	DigitalBtn Right;
	DigitalBtn Space;
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

	b32 using_analog()
	{
		return true;
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

	b32 using_analog()
	{
		return true;
	};
};

struct ControllerState
{
	KeyboardState*     Keyboard;
	MousesState*       Mouse;
	XInputPadState*    XPad;
	DualsensePadState* DSPad;
};

struct InputState
{
	ControllerState Controllers[4];
};

b32 input_using_analog();

// Needs a contextual reference to four things:
// Timing, Input, Bitmap Buffer, Sound Buffer
void update_and_render( InputState* input, OffscreenBuffer* back_buffer, SoundBuffer* sound_buffer, Memory* memory );

NS_ENGINE_END
