/*
This header exists for base shared definitions across engine files.

For now what every file needs is at least the namespace macros.
*/
#pragma once

#define NS_ENGINE_BEGIN namespace engine {
#define NS_ENGINE_END }

#ifndef Engine_API
// The build system is reponsible for defining this API macro for exporting symbols.
#	define Engine_API
#endif
