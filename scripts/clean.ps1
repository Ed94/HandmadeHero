clear-host
$path_root = git rev-parse --show-toplevel

$path_project      = join-path $path_root    "project"
$path_build 	   = join-path $path_root    "build"
$path_dependencies = join-path $path_project "dependencies"

if ( Test-Path $path_build ) {
	Remove-Item -verbose $path_build -Recurse
}
