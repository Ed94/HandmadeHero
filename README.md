# HandmadeHero

Any code I do for this [series](https://handmadehero.org) will be here.  

***(Only original hand-written code will be here, no code from the series itself)***

## Scripts

* `build.ps1` - Builds the project use `.\scripts\build msvc debug` or `.\scripts\build clang debug`. 
  * `optimize` for optimized builds.
  * `dev` for development builds. ( Dev memory layout and code paths compiled ).
* `clean.ps1` - Cleans the project
* `update_deps.ps1` - Updates the project dependencies to their latest from their respective repos. (Not done automatically on build)

*Make sure to run `update_deps.ps1` before building for the first time.*

## Notes

Building requires msvc or llvm's clang + lld, and powershell 7

The build is done in two stages:

1. ~~Build and run metaprogram to scan and generate dependent code.~~ (Not needed yet)
2. Build the handmade hero runtime.

## Gallery

![img](https://files.catbox.moe/xyh7u7.png)
![img](https://files.catbox.moe/b7ifa8.png)
