clear-host
$path_root = git rev-parse --show-toplevel

$path_project      = join-path $path_root    "project"
$path_build 	   = join-path $path_root    "build"
$path_data 	       = join-path $path_root    "data"
$path_dependencies = join-path $path_project "dependencies"
$path_binaries     = join-path $path_data    "binaries"

if ( Test-Path $path_build ) {
	Remove-Item -verbose $path_build -Recurse
}
if ( Test-Path $path_binaries ) {
	Remove-Item -verbose $path_binaries -Recurse
}

