#pragma once

template< class Type >
void swap( Type& a, Type& b )
{
	Type
	temp = a;
	a    = b;
	b    = temp;
}

#define Zero( type ) tmpl_zero<type>()
template< class Type >
constexpr Type tmpl_zero();
