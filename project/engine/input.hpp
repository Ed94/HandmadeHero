#pragma once

#if INTELLISENSE_DIRECTIVES
#include "platform/platform.hpp"
#include "engine_module.hpp"
#endif

NS_ENGINE_BEGIN

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

NS_ENGINE_END
