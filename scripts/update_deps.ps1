# For now this just grabs gencpp
# Possibly the only thing it will ever grab

clear-host
$path_root = git rev-parse --show-toplevel

$path_project      = join-path $path_root "project"
$path_dependencies = join-path $path_project "dependencies"
$path_temp 	       = join-path $path_dependencies "temp"

# Define the URL of the zip file and the destination directory
$url            = "https://github.com/Ed94/gencpp/releases/download/latest/gencpp_singleheader.zip"
$destinationZip = join-path $path_temp "gencpp_singleheader.zip"

if ( (Test-Path $path_dependencies) -eq $false ) {
	New-Item -ItemType Directory -Path $path_dependencies
}
if ( (Test-Path $path_temp) -eq $false ) {
	New-Item -ItemType Directory -Path $path_temp
}

# Download the zip file
Invoke-WebRequest -Uri $url -OutFile $destinationZip

# Extract the zip file to the specified directory
Expand-Archive -Path $destinationZip -DestinationPath $path_temp

# Move gen.hpp to the project directory
Move-Item -Path (join-path $path_temp "gen.hpp") -Destination $path_dependencies

Remove-Item $path_temp -Recurse
