#if INTELLISENSE_DIRECTIVES
#include "platform.hpp"
#include "engine/engine.hpp"
#include "engine/input.hpp"
#include "jsl.hpp"
#include "win32.hpp"
#endif

NS_PLATFORM_BEGIN
using namespace win32;

using JSL_DeviceHandle      = int;
using EngineXInputPadStates = engine::XInputPadState   [ engine::Max_Controllers ];
using EngineDSPadStates     = engine::DualsensePadState[ engine::Max_Controllers ];

internal void
input_process_digital_btn( engine::DigitalBtn* old_state, engine::DigitalBtn* new_state, u32 raw_btns, u32 btn_flag )
{
#define had_transition() ( old_state->ended_down != new_state->ended_down )
	new_state->ended_down        = (raw_btns & btn_flag) > 0;
	if ( had_transition() )
		new_state->half_transitions += 1;
	else
		new_state->half_transitions = 0;
#undef had_transition
}

internal f32
jsl_input_process_axis_value( f32 value, f32 deadzone_threshold )
{
	f32 result = 0;
	if ( value < -deadzone_threshold  )
	{
		result = (value + deadzone_threshold ) / (1.0f - deadzone_threshold );

		if (result < -1.0f)
			result = -1.0f; // Clamp to ensure it doesn't go below -1
	}
	else if ( value > deadzone_threshold )
	{
		result = (value - deadzone_threshold ) / (1.0f - deadzone_threshold );

		if (result > 1.0f)
			result = 1.0f; // Clamp to ensure it doesn't exceed 1
	}
	return result;
}

internal f32
xinput_process_axis_value( s16 value, s16 deadzone_threshold )
{
	f32 result = 0;
	if ( value < -deadzone_threshold )
	{
		result = scast(f32, value + deadzone_threshold) / (32768.0f - scast(f32, deadzone_threshold));
	}
	else if ( value > deadzone_threshold )
	{
		result = scast(f32, value + deadzone_threshold) / (32767.0f - scast(f32, deadzone_threshold));
	}
	return result;
}

internal void
poll_input( HWND window_handle, engine::InputState* input, u32 jsl_num_devices, JSL_DeviceHandle* jsl_device_handles
	, engine::KeyboardState* old_keyboard, engine::KeyboardState* new_keyboard
	, engine::MousesState*   old_mouse,    engine::MousesState*   new_mouse
	, EngineXInputPadStates* old_xpads,    EngineXInputPadStates* new_xpads
	, EngineDSPadStates*     old_ds_pads,  EngineDSPadStates*     new_ds_pads )
{
	// Keyboard Polling
	// Keyboards are unified for now.
	{
		constexpr u32 is_down = 0x80000000;
		input_process_digital_btn( & old_keyboard->_1, 	        & new_keyboard->_1,          GetAsyncKeyState( '1' ),       is_down );
		input_process_digital_btn( & old_keyboard->_2, 	        & new_keyboard->_2,          GetAsyncKeyState( '2' ),       is_down );
		input_process_digital_btn( & old_keyboard->_3, 	        & new_keyboard->_3,          GetAsyncKeyState( '3' ),       is_down );
		input_process_digital_btn( & old_keyboard->_4, 	        & new_keyboard->_4,          GetAsyncKeyState( '4' ),       is_down );
		input_process_digital_btn( & old_keyboard->Q,           & new_keyboard->Q,           GetAsyncKeyState( 'Q' ),       is_down );
		input_process_digital_btn( & old_keyboard->E,           & new_keyboard->E,           GetAsyncKeyState( 'E' ),       is_down );
		input_process_digital_btn( & old_keyboard->W,           & new_keyboard->W,           GetAsyncKeyState( 'W' ),       is_down );
		input_process_digital_btn( & old_keyboard->A,           & new_keyboard->A,           GetAsyncKeyState( 'A' ),       is_down );
		input_process_digital_btn( & old_keyboard->S,           & new_keyboard->S,           GetAsyncKeyState( 'S' ),       is_down );
		input_process_digital_btn( & old_keyboard->D,           & new_keyboard->D,           GetAsyncKeyState( 'D' ),       is_down );
		input_process_digital_btn( & old_keyboard->K,           & new_keyboard->K,           GetAsyncKeyState( 'K' ),       is_down );
		input_process_digital_btn( & old_keyboard->L,           & new_keyboard->L,           GetAsyncKeyState( 'L' ),       is_down );
		input_process_digital_btn( & old_keyboard->escape,      & new_keyboard->escape,      GetAsyncKeyState( VK_ESCAPE ), is_down );
		input_process_digital_btn( & old_keyboard->backspace,   & new_keyboard->backspace,   GetAsyncKeyState( VK_BACK ),   is_down );
		input_process_digital_btn( & old_keyboard->up,          & new_keyboard->up,          GetAsyncKeyState( VK_UP ),     is_down );
		input_process_digital_btn( & old_keyboard->down,        & new_keyboard->down,        GetAsyncKeyState( VK_DOWN ),   is_down );
		input_process_digital_btn( & old_keyboard->left,        & new_keyboard->left,        GetAsyncKeyState( VK_LEFT ),   is_down );
		input_process_digital_btn( & old_keyboard->right,       & new_keyboard->right,       GetAsyncKeyState( VK_RIGHT ),  is_down );
		input_process_digital_btn( & old_keyboard->space,       & new_keyboard->space,       GetAsyncKeyState( VK_SPACE ),  is_down );
		input_process_digital_btn( & old_keyboard->pause,       & new_keyboard->pause,       GetAsyncKeyState( VK_PAUSE ),  is_down );
		input_process_digital_btn( & old_keyboard->left_alt,    & new_keyboard->left_alt,    GetAsyncKeyState( VK_LMENU ),  is_down );
		input_process_digital_btn( & old_keyboard->right_alt,   & new_keyboard->right_alt,   GetAsyncKeyState( VK_RMENU ),  is_down );
		input_process_digital_btn( & old_keyboard->left_shift,  & new_keyboard->left_shift,  GetAsyncKeyState( VK_LSHIFT ), is_down );
		input_process_digital_btn( & old_keyboard->right_shift, & new_keyboard->right_shift, GetAsyncKeyState( VK_RSHIFT ), is_down );

	#if NEW_INPUT_DESIGN
		input->keyboard = new_keyboard;
	#else
		input->controllers[0].keyboard = new_keyboard;
	#endif
	}

	// Mouse polling
	{
		// input->Controllers[0].Mouse = {};

		constexpr u32 is_down = 0x80000000;
		input_process_digital_btn( & old_mouse->left,   & new_mouse->left,   GetAsyncKeyState( VK_LBUTTON ), is_down );
		input_process_digital_btn( & old_mouse->middle, & new_mouse->middle, GetAsyncKeyState( VK_MBUTTON ), is_down );
		input_process_digital_btn( & old_mouse->right,  & new_mouse->right,  GetAsyncKeyState( VK_RBUTTON ), is_down );

		POINT mouse_pos;
		GetCursorPos( & mouse_pos );
		ScreenToClient( window_handle, & mouse_pos );

		new_mouse->vertical_wheel   = {};
		new_mouse->horizontal_wheel = {};

		new_mouse->X.end = (f32)mouse_pos.x;
		new_mouse->Y.end = (f32)mouse_pos.y;
		
	#if NEW_INPUT_DESIGN
		input->mouse = new_mouse;
	#else
		input->controllers[0].mouse = new_mouse;
	#endif
	}

	// XInput Polling
	// TODO(Ed) : Should we poll this more frequently?
	for ( DWORD controller_index = 0; controller_index < engine::Max_Controllers; ++ controller_index )
	{
		XINPUT_STATE controller_state;
		b32 xinput_detected = xinput_get_state( controller_index, & controller_state ) == XI_PluggedIn;
		if ( xinput_detected )
		{
			XINPUT_GAMEPAD*         xpad     = & controller_state.Gamepad;
			engine::XInputPadState* old_xpad = old_xpads[ controller_index ];
			engine::XInputPadState* new_xpad = new_xpads[ controller_index ];
			input_process_digital_btn( & old_xpad->dpad.up,    & new_xpad->dpad.up,    xpad->wButtons, XINPUT_GAMEPAD_DPAD_UP );
			input_process_digital_btn( & old_xpad->dpad.down,  & new_xpad->dpad.down,  xpad->wButtons, XINPUT_GAMEPAD_DPAD_DOWN );
			input_process_digital_btn( & old_xpad->dpad.left,  & new_xpad->dpad.left,  xpad->wButtons, XINPUT_GAMEPAD_DPAD_LEFT );
			input_process_digital_btn( & old_xpad->dpad.right, & new_xpad->dpad.right, xpad->wButtons, XINPUT_GAMEPAD_DPAD_RIGHT );

			input_process_digital_btn( & old_xpad->Y, & new_xpad->Y, xpad->wButtons, XINPUT_GAMEPAD_Y );
			input_process_digital_btn( & old_xpad->A, & new_xpad->A, xpad->wButtons, XINPUT_GAMEPAD_A );
			input_process_digital_btn( & old_xpad->B, & new_xpad->B, xpad->wButtons, XINPUT_GAMEPAD_B );
			input_process_digital_btn( & old_xpad->X, & new_xpad->X, xpad->wButtons, XINPUT_GAMEPAD_X );

			input_process_digital_btn( & old_xpad->back,  & new_xpad->back,  xpad->wButtons, XINPUT_GAMEPAD_BACK );
			input_process_digital_btn( & old_xpad->start, & new_xpad->start, xpad->wButtons, XINPUT_GAMEPAD_START );

			input_process_digital_btn( & old_xpad->left_shoulder,  & new_xpad->left_shoulder,  xpad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER );
			input_process_digital_btn( & old_xpad->right_shoulder, & new_xpad->right_shoulder, xpad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER );

			new_xpad->stick.left.X.start = old_xpad->stick.left.X.end;
			new_xpad->stick.left.Y.start = old_xpad->stick.left.Y.end;

			f32 left_x = xinput_process_axis_value( xpad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
			f32 left_y = xinput_process_axis_value( xpad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );

			// TODO(Ed) : Min/Max macros!!!
			new_xpad->stick.left.X.min = new_xpad->stick.left.X.max = new_xpad->stick.left.X.end = left_x;
			new_xpad->stick.left.Y.min = new_xpad->stick.left.Y.max = new_xpad->stick.left.Y.end = left_y;

			// TODO(Ed): Make this actually an average for later
			new_xpad->stick.left.X.average = left_x;
			new_xpad->stick.left.Y.average = left_y;

		#if NEW_INPUT_DESIGN
			input->xpads[ controller_index ] = new_xpad;
		#else 
			input->controllers[ controller_index ].xpad = new_xpad;
		#endif
		}
		else
		{
		#if NEW_INPUT_DESIGN
			input->xpads[ controller_index ] = nullptr;
		#else 
			input->controllers[ controller_index ].xpad = nullptr;
		#endif
		}
	}

	// JSL Input Polling
	for ( u32 jsl_device_index = 0; jsl_device_index < jsl_num_devices; ++ jsl_device_index )
	{
		if ( ! JslStillConnected( jsl_device_handles[ jsl_device_index ] ) )
		{
			OutputDebugStringA( "Error: JSLStillConnected returned false\n" );
			
		#if NEW_INPUT_DESIGN
			input->ds_pads[ jsl_device_index ] = nullptr;
		#else
			input->controllers[ jsl_device_index ].ds_pad = nullptr;
		#endif
			continue;
		}

		// TODO : Won't support more than 4 for now... (or prob ever)
		if ( jsl_device_index > 4 )
			break;

		JOY_SHOCK_STATE state = JslGetSimpleState( jsl_device_handles[ jsl_device_index ] );

		// For now we're assuming anything that is detected via JSL is a dualsense pad.
		// We'll eventually add support possibly for the nintendo pro controller.
		engine::DualsensePadState* old_ds_pad  = old_ds_pads[ jsl_device_index ];
		engine::DualsensePadState* new_ds_pad  = new_ds_pads[ jsl_device_index ];

		input_process_digital_btn( & old_ds_pad->dpad.up,    & new_ds_pad->dpad.up,    state.buttons, JSMASK_UP );
		input_process_digital_btn( & old_ds_pad->dpad.down,  & new_ds_pad->dpad.down,  state.buttons, JSMASK_DOWN );
		input_process_digital_btn( & old_ds_pad->dpad.left,  & new_ds_pad->dpad.left,  state.buttons, JSMASK_LEFT );
		input_process_digital_btn( & old_ds_pad->dpad.right, & new_ds_pad->dpad.right, state.buttons, JSMASK_RIGHT );

		input_process_digital_btn( & old_ds_pad->triangle, & new_ds_pad->triangle, state.buttons, JSMASK_N );
		input_process_digital_btn( & old_ds_pad->cross,    & new_ds_pad->cross,    state.buttons, JSMASK_S );
		input_process_digital_btn( & old_ds_pad->square,   & new_ds_pad->square,   state.buttons, JSMASK_W );
		input_process_digital_btn( & old_ds_pad->circle,   & new_ds_pad->circle,   state.buttons, JSMASK_E );

		input_process_digital_btn( & old_ds_pad->share,   & new_ds_pad->share,   state.buttons, JSMASK_SHARE );
		input_process_digital_btn( & old_ds_pad->options, & new_ds_pad->options, state.buttons, JSMASK_OPTIONS );

		input_process_digital_btn( & old_ds_pad->L1, & new_ds_pad->L1, state.buttons, JSMASK_L );
		input_process_digital_btn( & old_ds_pad->R1, & new_ds_pad->R1, state.buttons, JSMASK_R );

		new_ds_pad->stick.left.X.start = old_ds_pad->stick.left.X.end;
		new_ds_pad->stick.left.Y.start = old_ds_pad->stick.left.Y.end;

		// Joyshock abstracts the sticks to a float value already for us of -1.f to 1.f.
		// We'll assume a deadzone of 10% for now.
		f32 left_x = jsl_input_process_axis_value( state.stickLX, 0.1f );
		f32 left_y = jsl_input_process_axis_value( state.stickLY, 0.1f );

		new_ds_pad->stick.left.X.min = new_ds_pad->stick.left.X.max = new_ds_pad->stick.left.X.end = left_x;
		new_ds_pad->stick.left.Y.min = new_ds_pad->stick.left.Y.max = new_ds_pad->stick.left.Y.end = left_y;

		// TODO(Ed): Make this actually an average for later
		new_ds_pad->stick.left.X.average = left_x;
		new_ds_pad->stick.left.Y.average = left_y;

	#if NEW_INPUT_DESIGN
		input->ds_pads[ jsl_device_index ] = new_ds_pad;
	#else
		input->controllers[ jsl_device_index ].ds_pad = new_ds_pad;
	#endif
	}
}

NS_PLATFORM_END
