#include "platform/platform.hpp"
#include "win32.hpp"

NS_PLATFORM_BEGIN

// TODO : This will def need to be looked over.
struct DirectSoundBuffer
{
	LPDIRECTSOUNDBUFFER secondary_buffer;
	s16*                samples;
	u32                 secondary_buffer_size;
	u32                 samples_per_second;
	u32                 bytes_per_sample;

	u32                 bytes_per_second;
	u32                 guard_sample_bytes;

	DWORD               is_playing;
	u32                 running_sample_index;

	u32                 latency_sample_count;
};

struct AudioTimeMarker
{
	DWORD output_play_cursor;
	DWORD output_write_cursor;
	DWORD output_location;
	DWORD output_byte_count;

	DWORD flip_play_curosr;
	DWORD flip_write_cursor;

	DWORD expected_flip_cursor;
};

using DirectSoundCreateFn = HRESULT WINAPI (LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN  pUnkOuter );
global DirectSoundCreateFn* direct_sound_create;

#if Build_Development
internal void
debug_draw_vertical_line( s32 x_pos, s32 top, s32 bottom, s32 color )
{
	if ( top <= 0 )
	{
		top = 0;
	}

	if ( bottom > Surface_Back_Buffer.height )
	{
		bottom = Surface_Back_Buffer.height;
	}

	if ( x_pos >= 0 && x_pos < Surface_Back_Buffer.width )
	{
		u8*
		pixel_byte  = rcast(u8*, Surface_Back_Buffer.memory );
		pixel_byte += x_pos * Surface_Back_Buffer.bytes_per_pixel;
		pixel_byte += top   * Surface_Back_Buffer.pitch;

		for ( s32 y = top; y < bottom; ++ y )
		{
			s32* pixel = rcast(s32*, pixel_byte);
			*pixel = color;

			pixel_byte += Surface_Back_Buffer.pitch;
		}
	}
}

inline void
debug_draw_sound_buffer_marker( DirectSoundBuffer* sound_buffer, f32 ratio
	, u32 pad_x, u32 pad_y
	, u32 top, u32 bottom
	, DWORD value, u32 color )
{
	// assert( value < sound_buffer->SecondaryBufferSize );
	u32 x = pad_x + scast(u32, ratio * scast(f32, value ));
	debug_draw_vertical_line( x, top, bottom, color );
}

internal void
debug_sync_display( DirectSoundBuffer* sound_buffer
                   , u32 num_markers, AudioTimeMarker* markers
				   , u32 current_marker
                   , f32 ms_per_frame )
{
	u32 pad_x         = 32;
	u32 pad_y         = 16;
	f32 buffers_ratio = scast(f32, Surface_Back_Buffer.width) / (scast(f32, sound_buffer->secondary_buffer_size) * 1);

	u32 line_height = 64;
	for ( u32 marker_index = 0; marker_index < num_markers; ++ marker_index )
	{
		AudioTimeMarker* marker = & markers[marker_index];
		assert( marker->output_play_cursor  < sound_buffer->secondary_buffer_size );
		assert( marker->output_write_cursor < sound_buffer->secondary_buffer_size );
		assert( marker->output_location     < sound_buffer->secondary_buffer_size );
		assert( marker->output_byte_count   < sound_buffer->secondary_buffer_size );
		assert( marker->flip_play_curosr    < sound_buffer->secondary_buffer_size );
		assert( marker->flip_write_cursor   < sound_buffer->secondary_buffer_size );

		DWORD play_color          = 0x88888888;
		DWORD write_color         = 0x88800000;
		DWORD expected_flip_color = 0xFFFFF000;
		DWORD play_window_color   = 0xFFFF00FF;

		u32 top    = pad_y;
		u32 bottom = pad_y + line_height;
		if ( marker_index == current_marker )
		{
			play_color  = 0xFFFFFFFF;
			write_color = 0xFFFF0000;

			top    += pad_y + line_height;
			bottom += pad_y + line_height;

			u32 row_2_top = top;

			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->output_play_cursor,  play_color );
			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->output_write_cursor, write_color );

			play_color  = 0xFFFFFFFF;
			write_color = 0xFFFF0000;

			top    += pad_y + line_height;
			bottom += pad_y + line_height;

			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->output_location, play_color );
			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->output_location + marker->output_byte_count, write_color );

			play_color  = 0xFFFFFFFF;
			write_color = 0xFFFF0000;

			top    += pad_y + line_height;
			bottom += pad_y + line_height;

			debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, row_2_top, bottom, marker->expected_flip_cursor, expected_flip_color );
		}

		DWORD play_window = marker->flip_play_curosr + 480 * sound_buffer->bytes_per_sample;

		debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->flip_play_curosr,  play_color );
		debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, play_window,               play_window_color );
		debug_draw_sound_buffer_marker( sound_buffer, buffers_ratio, pad_x, pad_y, top, bottom, marker->flip_write_cursor, write_color );
	}
}
#endif

internal void
init_sound(HWND window_handle, DirectSoundBuffer* sound_buffer )
{
	// Load library
	HMODULE sound_library = LoadLibraryA( "dsound.dll" );
	if ( ! ensure(sound_library, "Failed to load direct sound library" ) )
	{
		// TOOD : Diagnostic
		return;
	}

	// Get direct sound object
	direct_sound_create = get_procedure_from_library< DirectSoundCreateFn >( sound_library, "DirectSoundCreate" );
	if ( ! ensure( direct_sound_create, "Failed to get direct_sound_create_procedure" ) )
	{
		// TOOD : Diagnostic
		return;
	}

	LPDIRECTSOUND direct_sound;
	if ( ! SUCCEEDED(direct_sound_create( 0, & direct_sound, 0 )) )
	{
		// TODO : Diagnostic
	}
	if ( ! SUCCEEDED( direct_sound->SetCooperativeLevel(window_handle, DSSCL_PRIORITY) ) )
	{
		// TODO : Diagnostic
	}

	WAVEFORMATEX
	wave_format {};
	wave_format.wFormatTag      = WAVE_FORMAT_PCM;  /* format type */
	wave_format.nChannels       = 2;  /* number of channels (i.e. mono, stereo...) */
	wave_format.nSamplesPerSec  = scast(u32, sound_buffer->samples_per_second);  /* sample rate */
	wave_format.wBitsPerSample  = 16;  /* number of bits per sample of mono data */
	wave_format.nBlockAlign     = wave_format.nChannels      * wave_format.wBitsPerSample / 8 ;  /* block size of data */
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;  /* for buffer estimation */
	wave_format.cbSize          = 0;  /* the count in bytes of the size of */

	LPDIRECTSOUNDBUFFER primary_buffer;
	{
		DSBUFFERDESC
		buffer_description { sizeof(buffer_description) };
		buffer_description.dwFlags       = DSBCAPS_PRIMARYBUFFER;
		buffer_description.dwBufferBytes = 0;

		if ( ! SUCCEEDED( direct_sound->CreateSoundBuffer( & buffer_description, & primary_buffer, 0 ) ))
		{
			// TODO : Diagnostic
		}
		if ( ! SUCCEEDED( primary_buffer->SetFormat( & wave_format ) ) )
		{
			// TODO : Diagnostic
		}
	}

	DSBUFFERDESC
	buffer_description { sizeof(buffer_description) };
	buffer_description.dwFlags       = DSBCAPS_GETCURRENTPOSITION2;
	buffer_description.dwBufferBytes = sound_buffer->secondary_buffer_size;
	buffer_description.lpwfxFormat   = & wave_format;

	if ( ! SUCCEEDED( direct_sound->CreateSoundBuffer( & buffer_description, & sound_buffer->secondary_buffer, 0 ) ))
	{
		// TODO : Diagnostic
	}
	if ( ! SUCCEEDED( sound_buffer->secondary_buffer->SetFormat( & wave_format ) ) )
	{
		// TODO : Diagnostic
	}
}

internal void
ds_clear_sound_buffer( DirectSoundBuffer* sound_buffer )
{
	LPVOID region_1;
	DWORD  region_1_size;
	LPVOID region_2;
	DWORD  region_2_size;

	HRESULT ds_lock_result = sound_buffer->secondary_buffer->Lock( 0, sound_buffer->secondary_buffer_size
		, & region_1, & region_1_size
		, & region_2, & region_2_size
		, 0 );
	if ( ! SUCCEEDED( ds_lock_result ) )
	{
		return;
	}

	u8* sample_out = rcast( u8*, region_1 );
	for ( DWORD byte_index = 0; byte_index < region_1_size; ++ byte_index )
	{
		*sample_out = 0;
		++ sample_out;
	}

	sample_out = rcast( u8*, region_2 );
	for ( DWORD byte_index = 0; byte_index < region_2_size; ++ byte_index )
	{
		*sample_out = 0;
		++ sample_out;
	}

	if ( ! SUCCEEDED( sound_buffer->secondary_buffer->Unlock( region_1, region_1_size, region_2, region_2_size ) ))
	{
		return;
	}
}

internal void
ds_fill_sound_buffer( DirectSoundBuffer* sound_buffer, DWORD byte_to_lock, DWORD bytes_to_write )
{
	LPVOID region_1;
	DWORD  region_1_size;
	LPVOID region_2;
	DWORD  region_2_size;

	HRESULT ds_lock_result = sound_buffer->secondary_buffer->Lock( byte_to_lock, bytes_to_write
		, & region_1, & region_1_size
		, & region_2, & region_2_size
		, 0 );
	if ( ! SUCCEEDED( ds_lock_result ) )
	{
		return;
	}

	// TODO : Assert that region sizes are valid

	DWORD region_1_sample_count = region_1_size / sound_buffer->bytes_per_sample;
	s16*  sample_out            = rcast( s16*, region_1 );
	s16*  sample_in 		    = sound_buffer->samples;
	for ( DWORD sample_index = 0; sample_index < region_1_sample_count; ++ sample_index )
	{
		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		++ sound_buffer->running_sample_index;
	}

	DWORD region_2_sample_count = region_2_size / sound_buffer->bytes_per_sample;
	      sample_out            = rcast( s16*, region_2 );
	for ( DWORD sample_index = 0; sample_index < region_2_sample_count; ++ sample_index )
	{
		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		*sample_out = *sample_in;
		++ sample_out;
		++ sample_in;

		++ sound_buffer->running_sample_index;
	}

	if ( ! SUCCEEDED( sound_buffer->secondary_buffer->Unlock( region_1, region_1_size, region_2, region_2_size ) ))
	{
		return;
	}
}

internal
void process_audio_frame( DirectSoundBuffer& ds_sound_buffer, DWORD& ds_play_cursor, DWORD& ds_write_cursor, f32& ds_latency_ms
	, b32& sound_is_valid
	, AudioTimeMarker* audio_time_markers, u32 audio_marker_index
	, f32 flip_to_audio_ms, u64 last_frame_clock
	, engine::ModuleAPI& engine_api, engine::Memory* engine_memory, ModuleAPI* platform_api, engine::ThreadContext* thread_context_placeholder)
{
/*
	Audio Processing:
	There is a sync boundary value, that is the number of samples that the engine's frame-time may vary by
	(ex: approx 2ms of variance between frame-times).

	On wakeup : Check play cursor position and forcast ahead where the cursor will be for the next sync boundary.
	Based on that, check the write cursor position, if its (at least) before the synch boundary, the target write position is
	the frame boundary plus one frame. (Low latency)

	If its after (sync boundary), we cannot sync audio.
	Write a frame's worth of audio plus some number of "guard" samples. (High Latency)
*/
	if ( ! SUCCEEDED( ds_sound_buffer.secondary_buffer->GetCurrentPosition( & ds_play_cursor, & ds_write_cursor ) ))
	{
		sound_is_valid = false;
		return;
	}

	if ( ! sound_is_valid )
	{
		ds_sound_buffer.running_sample_index = ds_write_cursor / ds_sound_buffer.bytes_per_sample;
		sound_is_valid = true;
	}

	DWORD byte_to_lock   = 0;
	DWORD target_cursor  = 0;
	DWORD bytes_to_write = 0;

	byte_to_lock = (ds_sound_buffer.running_sample_index * ds_sound_buffer.bytes_per_sample) % ds_sound_buffer.secondary_buffer_size;

	DWORD bytes_per_second = ds_sound_buffer.bytes_per_sample * ds_sound_buffer.samples_per_second;

	DWORD expected_samplebytes_per_frame = bytes_per_second / Engine_Refresh_Hz;

	f32   left_until_flip_ms        = Engine_Frame_Target_MS - flip_to_audio_ms;
	DWORD expected_bytes_until_flip = scast(DWORD, (left_until_flip_ms / Engine_Frame_Target_MS) * scast(f32, expected_samplebytes_per_frame));

	DWORD expected_sync_boundary_byte = ds_play_cursor + expected_bytes_until_flip;

	DWORD sync_write_cursor = ds_write_cursor;
	if ( sync_write_cursor < ds_play_cursor )
	{
		// unwrap the cursor so its ahead of the play curosr linearly.
		sync_write_cursor += ds_sound_buffer.secondary_buffer_size;
	}
	assert( sync_write_cursor >= ds_play_cursor );

	sync_write_cursor += ds_sound_buffer.guard_sample_bytes;

	b32 audio_interface_is_low_latency = sync_write_cursor < expected_sync_boundary_byte;
	if ( audio_interface_is_low_latency )
	{
		target_cursor = ( expected_sync_boundary_byte + expected_samplebytes_per_frame );
	}
	else
	{
		target_cursor = (ds_write_cursor +  expected_samplebytes_per_frame + ds_sound_buffer.guard_sample_bytes);
	}
	target_cursor %= ds_sound_buffer.secondary_buffer_size;

	if ( byte_to_lock > target_cursor)
	{
		// Infront of play cursor |--play--byte_to_write-->--|
		bytes_to_write =  ds_sound_buffer.secondary_buffer_size - byte_to_lock;
		bytes_to_write += target_cursor;
	}
	else
	{
		// Behind play cursor |--byte_to_write-->--play--|
		bytes_to_write = target_cursor - byte_to_lock;
	}

// Engine Sound
	// f32 delta_time = timing_get_seconds_elapsed( last_frame_clock, timing_get_wall_clock() );

	// s16 samples[ 48000 * 2 ];
	engine::AudioBuffer sound_buffer {};
	sound_buffer.num_samples          = bytes_to_write / ds_sound_buffer.bytes_per_sample;
	sound_buffer.running_sample_index = ds_sound_buffer.running_sample_index;
	sound_buffer.samples_per_second   = ds_sound_buffer.samples_per_second;
	sound_buffer.samples              = ds_sound_buffer.samples;
	engine_api.update_audio( 0.f, & sound_buffer, engine_memory, platform_api, thread_context_placeholder );

	AudioTimeMarker* marker = & audio_time_markers[ audio_marker_index ];
	marker->output_play_cursor   = ds_play_cursor;
	marker->output_write_cursor  = ds_write_cursor;
	marker->output_location      = byte_to_lock;
	marker->output_byte_count    = bytes_to_write;
	marker->expected_flip_cursor = expected_sync_boundary_byte;

// Update audio buffer
	if ( ! sound_is_valid )
		return;

#if Build_Development && 0
#if 0
	DWORD play_cursor;
	DWORD write_cursor;
	ds_sound_buffer.SecondaryBuffer->GetCurrentPosition( & play_cursor, & write_cursor );
#endif
	DWORD unwrapped_write_cursor = ds_write_cursor;
	if ( unwrapped_write_cursor < ds_play_cursor )
	{
		unwrapped_write_cursor += ds_sound_buffer.SecondaryBufferSize;
	}
	ds_cursor_byte_delta = unwrapped_write_cursor - ds_play_cursor;

	constexpr f32 to_milliseconds = 1000.f;
	f32 sample_delta  = scast(f32, ds_cursor_byte_delta) / scast(f32, ds_sound_buffer.BytesPerSample);
	f32 ds_latency_s  = sample_delta / scast(f32, ds_sound_buffer.SamplesPerSecond);
		ds_latency_ms = ds_latency_s * to_milliseconds;

	char text_buffer[256];
	sprintf_s( text_buffer, sizeof(text_buffer), "BTL:%u TC:%u BTW:%u - PC:%u WC:%u DELTA:%u bytes %f ms\n"
		, (u32)byte_to_lock, (u32)target_cursor, (u32)bytes_to_write
		, (u32)play_cursor, (u32)write_cursor, (u32)ds_cursor_byte_delta, ds_latency_ms );
	OutputDebugStringA( text_buffer );
#endif
	ds_fill_sound_buffer( & ds_sound_buffer, byte_to_lock, bytes_to_write  );

	DWORD ds_status = 0;
	if ( SUCCEEDED( ds_sound_buffer.secondary_buffer->GetStatus( & ds_status ) ) )
	{
		ds_sound_buffer.is_playing = ds_status & DSBSTATUS_PLAYING;
	}
	if ( ds_sound_buffer.is_playing )
		return;

	ds_sound_buffer.secondary_buffer->Play( 0, 0, DSBPLAY_LOOPING );
}

NS_PLATFORM_END