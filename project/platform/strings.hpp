#pragma once

void str_append( u32 dest_len, char* dest, u32 src_len, char const* src );
void str_concat( u32 dest_size, char* dest
	, u32 str_a_len, char const* str_a
	, u32 str_b_len, char const* str_b );
u32 str_length( char const* str );

#define str_ascii( str ) { sizeof( str ) - 1, ccast( char*, str) }

// Length tracked raw strings.
struct Str
{
	u32   len;
	char* ptr;

	void append( u32 src_len, char const* src ) {
		str_append( len, ptr, src_len, src );
	}
	void append( Str const src ) {
		str_append( len, ptr, src.len, src.ptr );
	}

	void concat( u32 dest_size, Str* dest, Str const str_a, Str const str_b )
	{
		str_concat( dest_size, dest->ptr
			, str_a.len, str_a.ptr
			, str_b.len, str_b.ptr );
		dest->len = str_a.len + str_b.len;
	}

	operator char*() const {
		return ptr;
	}
	char& operator []( u32 idx ) {
		return ptr[idx];
	}
	char const& operator []( u32 idx ) const {
		return ptr[idx];
	}
};

// Fixed length raw strings.
template< u32 capacity >
struct StrFixed
{
	constexpr static u32 Capacity = capacity;

	u32  len;
	char ptr[capacity];

	void append( u32 src_len, char const* src ) {
		str_append( len, data, src_len, src );
	}
	void append( Str const src ) {
		str_append( len, data, src.Len, src.data );
	}

	void concat( Str const str_a, Str const str_b )
	{
		str_concat( Capacity, ptr
			, str_a.len, str_a.ptr
			, str_b.len, str_b.ptr );
		len = str_a.len + str_b.len;
	}

	operator char*()           { return ptr;}
	operator char const*()     { return ptr; }
	operator Str()             { return { len, ptr }; }
	operator Str const() const { return { len, ptr }; }
	char& operator []( u32 idx ) {
		assert( idx < Capacity );
		return ptr[idx];
	}
	char const& operator []( u32 idx ) const {
		assert( idx < Capacity );
		return ptr[idx];
	}
};

inline
void str_append( u32 dest_len, char* dest, u32 src_len, char const* src )
{
	assert( dest_len > 0 );
	assert( dest != nullptr );
	assert( src_len > 0 );
	assert( src != nullptr );
	assert( dest_len >= src_len );

	char* dest_end = dest + dest_len;
	assert( *dest_end == '\0' );

	u32 left = src_len;
	while ( left-- )
	{
		*dest = *src;
		++ dest;
		++ src;
	}
}

void str_concat( u32 dest_size, char* dest
	, u32 str_a_len, char const* str_a
	, u32 str_b_len, char const* str_b )
{
	assert( dest_size > 0 );
	assert( dest != nullptr );
	assert( *dest == '\0' );
	assert( str_a_len > 0 );
	assert( str_a != nullptr );
	assert( str_b_len > 0 );
	assert( str_b != nullptr );
	assert( str_a_len + str_b_len < dest_size );

	char* dest_a = dest;
	char* dest_b = dest + str_a_len;
	if ( str_a_len > str_b_len )
	{
		u32 left = str_b_len;
		while ( left-- )
		{
			*dest_a = *str_a;
			*dest_b = *str_b;

			++ dest_a;
			++ dest_b;
			++ str_a;
			++ str_b;
		}

		left = str_a_len - str_b_len;
		while ( left-- )
		{
			*dest_a = *str_a;
			++ dest_a;
			++ str_a;
		}
	}
	else if ( str_a_len < str_b_len )
	{
		u32 left = str_a_len;
		while ( left-- )
		{
			*dest_a = *str_a;
			*dest_b = *str_b;
			++ dest_a;
			++ dest_b;
			++ str_a;
			++ str_b;
		}

		left = str_b_len - str_a_len;
		while ( left-- )
		{
			*dest_b = *str_b;
			++ dest_b;
			++ str_b;
		}
	}
	else
	{
		u32 left = str_a_len;
		while ( left-- )
		{
			*dest_a = *str_a;
			*dest_b = *str_b;
			++ dest_a;
			++ str_a;
			++ dest_b;
			++ str_b;
		}
	}
}

inline
u32 str_length( char const* str )
{
	assert( str != nullptr );

	u32 result = 0;
	while ( *str != '\0' )
	{
		++ result;
		++ str;
	}

	return result;
}

using StrPath = StrFixed< S16_MAX >;
