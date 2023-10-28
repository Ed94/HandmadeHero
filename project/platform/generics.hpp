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

// Custom casting trait for when you cannot define an implicit cast ergonomically.
#define cast( type, obj ) tmpl_cast<type, decltype(obj)>( obj )
template< class Type, class OtherType >
constexpr Type tmpl_cast( OtherType obj ) {
	static_assert( false, "No overload templated cast defined for type combination. Define this using an overload of tmpl_cast specialization." );
}
