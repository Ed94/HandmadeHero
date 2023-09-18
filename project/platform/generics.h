#pragma once

template< class Type >
void swap( Type& a, Type& b )
{
	Type
	temp = a;
	a    = b;
	b    = temp;
}
