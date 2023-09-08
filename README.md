# HandmadeHero

Any code I do for this series will be here.



## Scripts

* `build.ps1` - Builds the project use `.\scripts\build msvc` or `.\scripts\build clang`, add `release` to build in release mode
* `clean.ps1` - Cleans the project
* `update.ps1` - Updates the project dependencies to their latest from their respective repos. (Not done automatically on build)

## Notes

Building requires msvc or llvm's clang + lld, and powershell 7

The build is done in two stages:

1. Build and run metaprogram to scan and generate dependent code.
2. Build the handmade hero runtime.

