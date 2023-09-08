clear-host
$path_root = git rev-parse --show-toplevel

$path_project      = join-path $path_root "project"
$path_dependencies = join-path $path_project "dependencies"

Remove-Item $path_dependencies -Recurse
