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

## Milestone

Day 028: Drawing a Tile Map

## Gallery

![img](docs/imgs/handmade_win32_2023-10-01_20-22-20.gif)
![img](docs/imgs/Code_2023-09-28_15-14-53.gif)
![img](https://files.catbox.moe/ruv97s.gif)
![img](https://files.catbox.moe/9zau4s.png)
![img](https://files.catbox.moe/b7ifa8.png)
