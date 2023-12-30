/* TODO(Ed) : Do this properly later
I shoved this in here to depollute the mess Casey is making in engine.cpp.
I'll set this up more properly later when I see a good time to clean things up.

Doing my best to follow his advice to leave cleaning to when things are more "cemented".
*/
#if INTELLISENSE_DIRECTIVES
#include "engine.hpp"
#include "input.hpp"
#endif

NS_ENGINE_BEGIN

using SnapshotFn = void ( Memory* memory, platform::ModuleAPI* platform_api );

internal
void load_engine_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	platform_api->memory_copy( memory->persistent, memory->total_size()
	                          , memory->snapshots[ memory->active_snapshot_slot ].memory );
}

internal
void load_game_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	s32          slot  = memory->active_snapshot_slot;
	EngineState* state = rcast( EngineState*, memory->persistent );

	void* persistent_slot = memory->snapshots[ slot ].memory;
	void* transient_slot  = rcast( Byte*, memory->snapshots[ slot ].memory ) + state->game_memory.persistent_size;

	platform_api->memory_copy( state->game_memory.persistent, state->game_memory.persistent_size, persistent_slot );
	platform_api->memory_copy( state->game_memory.transient,  state->game_memory.transient_size,  transient_slot );
}

internal
void take_engine_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	platform_api->memory_copy( memory->snapshots[ memory->active_snapshot_slot ].memory, memory->total_size(), memory->persistent );
	memory->snapshots[ memory->active_snapshot_slot ].age = platform_api->get_wall_clock();
}

internal
void take_game_snapshot( Memory* memory, platform::ModuleAPI* platform_api )
{
	s32          slot  = memory->active_snapshot_slot;
	EngineState* state = rcast( EngineState*, memory->persistent );

	void* persistent_slot = memory->snapshots[ slot ].memory;
	void* transient_slot  = rcast( Byte*, memory->snapshots[ slot ].memory ) + state->game_memory.persistent_size;

	platform_api->memory_copy( persistent_slot, state->game_memory.persistent_size, state->game_memory.persistent );
	platform_api->memory_copy( transient_slot,  state->game_memory.transient_size,  state->game_memory.transient );
	memory->snapshots[ memory->active_snapshot_slot ].age = platform_api->get_wall_clock();
}

internal
void begin_recording_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	Str file_name = str_ascii("test_input_");
	StrPath file_path = {};
	file_path.concat( platform_api->path_scratch, file_name );
	snprintf( file_path.ptr, file_path.len, "%s%d.hm_replay", file_name.ptr, memory->active_snapshot_slot );

	memory->active_input_replay_file.path = file_path;
	platform_api->file_delete( memory->active_input_replay_file.path );
	memory->replay_mode                   = ReplayMode_Record;
}

internal
void end_recording_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	memory->replay_mode = ReplayMode_Off;
	platform_api->file_close( & memory->active_input_replay_file );
}

internal
void begin_playback_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	Str     file_name = str_ascii("test_input_");
	StrPath file_path = {};
	file_path.concat( platform_api->path_scratch, file_name );
	snprintf( file_path.ptr, file_path.len, "%s%d.hm_replay", file_name.ptr, memory->active_snapshot_slot );

	// TODO(Ed - From Casey): Recording system still seems to take too long
	// on record start - find out what Windows is doing and if
	// we can speed up / defer some of that processing.

	if ( platform_api->file_check_exists( file_path ) )
	{
		memory->active_input_replay_file.path = file_path;
		memory->replay_mode = ReplayMode_Playback;
	}
	else
	{
		// TODO(Ed) : Logging
	}
}

internal
void end_playback_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	memory->replay_mode = ReplayMode_Off;
	platform_api->file_close( & memory->active_input_replay_file );
}

InputStateSnapshot input_state_snapshot( InputState* input )
{
	InputStateSnapshot snapshot = {};
	if ( input->keyboard )
		snapshot.keyboard = * input->keyboard;
	
	if ( input->mouse )
		snapshot.mouse = * input->mouse;
	
	for ( s32 idx = 0; idx < Max_Controllers; ++ idx )
	{
		XInputPadState* xpad = input->xpads[ idx ];
		if ( xpad )
			snapshot.xpads[ idx ] = * xpad;
		
		DualsensePadState* ds_pad = input->ds_pads[ idx ];
		if ( ds_pad )
			snapshot.ds_pads[ idx ] = * ds_pad;
	}
	return snapshot;
}

internal
void record_input( Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	InputStateSnapshot snapshot = input_state_snapshot( input );
	if ( platform_api->file_write_stream( & memory->active_input_replay_file, sizeof(snapshot), &snapshot ) == 0 )
	{
		// TODO(Ed) : Logging
	}
}

internal
void play_input( SnapshotFn* load_snapshot, Memory* memory, InputState* input, platform::ModuleAPI* platform_api )
{
	InputStateSnapshot new_input;
	if ( platform_api->file_read_stream( & memory->active_input_replay_file, sizeof(InputStateSnapshot), & new_input ) == 0 )
	{
		load_snapshot( memory, platform_api );
		platform_api->file_rewind( & memory->active_input_replay_file );
		return;
	}
	
	if ( input->keyboard )
		* input->keyboard = new_input.keyboard;
	
	if ( input->mouse )
		* input->mouse = new_input.mouse;
		
	for ( s32 idx = 0; idx < Max_Controllers; ++ idx )
	{
		XInputPadState* xpad = input->xpads[ idx ];
		if ( xpad )
			* xpad = new_input.xpads[ idx ];
			
		DualsensePadState* ds_pad = input->ds_pads[ idx ];
		if ( ds_pad )
			* ds_pad = new_input.ds_pads[ idx ];
	}
}

void process_loop_mode( SnapshotFn* take_snapshot, SnapshotFn* load_snapshot
                       , Memory* memory, EngineState* state, InputState* input, platform::ModuleAPI* platform_api )
{
	if ( memory->replay_mode == ReplayMode_Off )
	{
		take_snapshot( memory, platform_api );
		begin_recording_input( memory, input, platform_api );
	}
	else if ( memory->replay_mode == ReplayMode_Playback )
	{
		end_playback_input( memory, input, platform_api );
		load_snapshot( memory, platform_api );
	}
	else if ( memory->replay_mode == ReplayMode_Record )
	{
		end_recording_input( memory, input, platform_api );
		load_snapshot( memory, platform_api );
		begin_playback_input( memory, input, platform_api );
	}
}

NS_ENGINE_END
