clear-host
$path_root = git rev-parse --show-toplevel

$path_project      = join-path $path_root    "project"
$path_build 	   = join-path $path_root    "build"
$path_dependencies = join-path $path_project "dependencies"

if ( Test-Path $path_project ) {
	Remove-Item $path_build -Recurse
}
