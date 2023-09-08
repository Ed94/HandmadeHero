clear-host
$path_root = & git rev-parse --show-toplevel

$path_project          = Join-Path $path_root         "project"
$path_dependencies     = Join-Path $path_project      "dependencies"
$path_temp             = Join-Path $path_dependencies "temp"
$path_platform_windows = Join-Path $path_project      "windows"

# Define the URL of the zip file and the destination directory
$url            = "https://github.com/Ed94/gencpp/releases/download/latest/gencpp_singleheader.zip"
$destinationZip = Join-Path $path_temp "gencpp_singleheader.zip"

# Create directories if they don't exist
if (-not (Test-Path $path_dependencies)) {
	New-Item -ItemType Directory -Path $path_dependencies
}
if (-not (Test-Path $path_temp)) {
	New-Item -ItemType Directory -Path $path_temp
}

#pragma region gencpp
Invoke-WebRequest -Uri $url -OutFile $destinationZip
Expand-Archive    -Path $destinationZip                  -DestinationPath $path_temp
Move-Item         -Path (Join-Path $path_temp "gen.hpp") -Destination     $path_dependencies -Force
#pragma endregion gencpp

#pragma region windows modular headers
Push-Location $path_temp
& git clone --no-checkout https://github.com/Leandros/WindowsHModular.git

	Push-Location WindowsHModular
	& git sparse-checkout init --cone
	& git sparse-checkout set include/win32
	Pop-Location

# Copy the win32 directory contents to the project/windows directory
Copy-Item -Recurse .\WindowsHModular\include\win32\* $path_platform_windows
Pop-Location
#pragma endregion windows modular headers

Remove-Item $path_temp -Recurse -Force
