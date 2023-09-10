#pragma once

// Keywords

#define global        static    // Global variables
#define internal      static    // Internal linkage
#define local_persist static    // Local Persisting variables

#define api_c extern "C"

// Casting

#define ccast( Type, Value ) ( * const_cast< Type* >( & (Value) ) )
#define pcast( Type, Value ) ( * reinterpret_cast< Type* >( & ( Value ) ) )
#define rcast( Type, Value ) reinterpret_cast< Type >( Value )
#define scast( Type, Value ) static_cast< Type >( Value )
