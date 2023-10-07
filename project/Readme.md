# Project Documentation

Currently the project is split into two modules:

* Platform
* Engine

This project takes the approach of doing all includes for a module in a single translation unit.  
Any includes within the project files outside of the translation unit files used for builds are for intellisense purposes.  
They are wrapped in `INTELLISENSE_DIRECTIVES` preprocessor conditional, and are necessary for most editors as they do not parse the project directories properly.  
(They do a rudimentary parse on a per-file basis on includes ussually)

## Platform

Translation Unit: `handmade_win32.cpp` for Windows

Deals with providing the core library for the project along with dealing with th platform specific grime.  
Only supports Windows at the momment. May add suport for macos or Ubuntu/SteamOS(linux) in the future.

## Engine

Translation Unit: `handmade_engine.cpp`

Currently deals with both *engine* and *game* code until I see a point where I can segment the two into separate modules.  
The the "proto-code" for the game module is within `handmade.hpp` & `handmade.cpp`
