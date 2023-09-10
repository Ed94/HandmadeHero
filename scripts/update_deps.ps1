clear-host
$path_root = & git rev-parse --show-toplevel

$path_data         = Join-Path $path_root         "data"
$path_project      = Join-Path $path_root         "project"
$path_deps         = Join-Path $path_project      "dependencies"
$path_deps_windows = Join-Path $path_deps         "windows"
$path_temp         = Join-Path $path_deps         "temp"
$path_platform     = Join-Path $path_project      "platform"

# Clear out the current content first
if (Test-Path $path_deps) {
	Remove-Item $path_deps -Recurse -Force
	New-Item -ItemType Directory -Path $path_deps
}
New-Item -ItemType Directory -Path $path_temp

$url_gencpp      = "https://github.com/Ed94/gencpp/releases/download/latest/gencpp_singleheader.zip"
$path_gencpp_zip = Join-Path $path_temp "gencpp_singleheader.zip"

#region gencpp
Invoke-WebRequest -Uri  $url_gencpp                      -OutFile         $path_gencpp_zip
Expand-Archive    -Path $path_gencpp_zip                 -DestinationPath $path_temp
Move-Item         -Path (Join-Path $path_temp "gen.hpp") -Destination     $path_deps -Force
#endregion gencpp

#region JoyShockLibrary
$url_jsl_repo       = "https://github.com/JibbSmart/JoyShockLibrary.git"
# $url_jsl_zip        = "https://github.com/JibbSmart/JoyShockLibrary/releases/download/v3.0/JSL_3_0.zip"
$url_jsl_zip        = "https://github.com/Ed94/JoyShockLibrary/releases/download/not_for_public_use/JSL.zip"
$path_jsl_repo      = Join-Path $path_temp    "JoyShockLibraryRepo"
$path_jsl_repo_code = Join-Path $path_jsl_repo "JoyShockLibrary"
$path_jsl_lib_zip   = Join-Path $path_temp    "JSL_3_0.zip"
$path_jsl           = Join-Path $path_deps    "JoyShockLibrary"
$path_jsl_hidapi    = Join-Path $path_jsl     "hidapi"
$path_jsl_lib	    = Join-Path $path_jsl     "x64"

# Grab code from repo
& git clone $url_jsl_repo $path_jsl_repo
Move-Item -Path $path_jsl_repo_code -Destination $path_deps -Force

# Clean up the junk
@( $path_jsl, $path_jsl_hidapi ) | ForEach-Object {
	Get-ChildItem -Path $path_jsl -Recurse -File | Where-Object {
		($_.Extension -ne ".h" -and $_.Extension -ne ".cpp")
	} | Remove-Item -Force
}
Remove-Item (join-path $path_jsl_hidapi 'objs') -Recurse -Force

# Get precompiled binaries
Invoke-WebRequest -Uri  $url_jsl_zip      -OutFile         $path_jsl_lib_zip
Expand-Archive    -Path $path_jsl_lib_zip -DestinationPath $path_temp

if (-not (Test-Path $path_jsl_lib)) {
    New-Item -ItemType Directory -Path $path_jsl_lib
}

$jsl_lib_files = (Get-ChildItem (Join-Path $path_temp "JSL\x64") -Recurse -Include *.dll, *.lib)
Move-Item $jsl_lib_files -Destination $path_jsl_lib -Force

$path_jsl_dll  = Join-Path $path_jsl_lib "JoyShockLibrary.dll"
Move-Item  $path_jsl_dll $path_data -Force
#endregion JoyShockLibrary

Remove-Item $path_temp -Recurse -Force
