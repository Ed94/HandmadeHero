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
Move-Item -Path (join-path $path_temp "gen.hpp") -Destination $path_dependencies -Force


# if ( Test-Path $path_platform_windows )
# {
# 	Remove-Item (Get-ChildItem -Path $path_platform_windows -Recurse -Force)
# }

Push-Location $path_temp
$path_repo_content = 'include/win32/'

& git clone --no-checkout https://github.com/Leandros/WindowsHModular.git 
& git sparse-checkout init --cone
& git sparse-checkout set $path_repo_content

Copy-Item -Recurse ( './' + $path_repo_content + '*') $path_platform_windows
Pop-Location $path_temp

Remove-Item $path_temp -Recurse
