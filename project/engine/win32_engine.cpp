#include "platform/win32.hpp"
#include "engine.hpp"

b32 WINAPI DllMain(
	HINSTANCE instance,
	DWORD reason_for_call,
	LPVOID reserved
)
{
	return true;
}
