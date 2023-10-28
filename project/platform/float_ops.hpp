#pragma once
#if INTELLISENSE_DIRECTIVES
#include "types.hpp"
#include "intrinsics.hpp"
#endif

#define mili_accuracy  0.001
#define micro_accuracy 0.00001
#define nano_accuracy  0.000000001

b32 is_nearly_zero( f32 value, f32 accuracy = mili_accuracy ) {
	return abs(value) < accuracy;	
}

f32 square( f32 value ) {
	return value * value;
}
