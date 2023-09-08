clear-host
$path_root = & git rev-parse --show-toplevel

$path_project      = Join-Path $path_root         "project"
$path_deps         = Join-Path $path_project      "dependencies"
$path_deps_windows = Join-Path $path_deps         "windows"
$path_temp         = Join-Path $path_deps         "temp"
$path_platform     = Join-Path $path_project      "platform"

# Define the URL of the zip file and the destination directory
$url            = "https://github.com/Ed94/gencpp/releases/download/latest/gencpp_singleheader.zip"
$destinationZip = Join-Path $path_temp "gencpp_singleheader.zip"

# Create directories if they don't exist
if (-not (Test-Path $path_deps)) {
	New-Item -ItemType Directory -Path $path_deps
}
if (-not (Test-Path $path_temp)) {
	New-Item -ItemType Directory -Path $path_temp
}

#pragma region gencpp
Invoke-WebRequest -Uri $url -OutFile $destinationZip
Expand-Archive    -Path $destinationZip                  -DestinationPath $path_temp
Move-Item         -Path (Join-Path $path_temp "gen.hpp") -Destination     $path_deps -Force
#pragma endregion gencpp

#pragma region windows modular headers
$repo = 'https://github.com/Leandros/WindowsHModular.git'

Push-Location $path_temp
if ( -not(test-path $path_deps_windows)) {
	New-Item -ItemType Directory -Path $path_deps_windows
}

& git clone $repo
Copy-Item -Recurse .\WindowsHModular\include\win32\* $path_deps_windows
Remove-Item (Get-ChildItem -Path $path_deps_windows -Exclude '*.h' -Recurse -Force)
Pop-Location
#pragma endregion windows modular headers

Remove-Item $path_temp -Recurse -Force
