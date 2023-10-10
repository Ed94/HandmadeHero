$clean = join-path $PSScriptRoot 'clean.ps1'
$build = join-path $PSScriptRoot 'build.ps1'

& $clean
& $build 'msvc' 'dev'
