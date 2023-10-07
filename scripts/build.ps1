Clear-Host

$target_arch      = Join-Path $PSScriptRoot 'helpers/target_arch.psm1'
$devshell         = Join-Path $PSScriptRoot 'helpers/devshell.ps1'
$format_cpp	      = Join-Path $PSScriptRoot 'helpers/format_cpp.psm1'
$config_toolchain = Join-Path $PSScriptRoot 'helpers/configure_toolchain.ps1'

$path_root   = git rev-parse --show-toplevel
$path_build  = Join-Path $path_root 'build'

Import-Module $target_arch
Import-Module $format_cpp

#region Arguments
       $vendor       = $null
       $optimize     = $null
	   $debug 	     = $null
	   $analysis	 = $false
	   $dev          = $false
	   $platform     = $null
	   $engine       = $null
	   $game         = $null
	   $verbose      = $null

[array] $vendors = @( "clang", "msvc" )

# This is a really lazy way of parsing the args, could use actual params down the line...

if ( $args ) { $args | ForEach-Object {
	switch ($_){
		{ $_ -in $vendors }   { $vendor    = $_; break }
		"optimize"            { $optimize  = $true }
		"debug"               { $debug     = $true }
		"analysis"            { $analysis  = $true }
		"dev"                 { $dev       = $true }
		"platform"            { $platform  = $true }
		"engine"              { $engine    = $true }
		"game"                { $game      = $true }
		"verbose"             { $verbose   = $true }
	}
}}
#endregion Argument

# Load up toolchain configuraion
. $config_toolchain

#region Building
write-host "Building HandmadeHero with $vendor"

$path_project  = Join-Path $path_root    'project'
$path_scripts  = Join-Path $path_root    'scripts'
$path_data     = Join-Path $path_root	 'data'
$path_binaries = Join-Path $path_data    'binaries'
$path_deps     = Join-Path $path_project 'dependencies'
$path_codegen  = Join-Path $path_project 'codegen'
$path_gen      = Join-Path $path_project 'gen'
$path_platform = Join-Path $path_project 'platform'
$path_engine   = Join-Path $path_project 'engine'

$update_deps = Join-Path $PSScriptRoot 'update_deps.ps1'

$handmade_process_active = Get-Process | Where-Object {$_.Name -like 'handmade_win32*'}

if ( (Test-Path $path_build) -eq $false ) {
	New-Item $path_build -ItemType Directory
}

if ( (Test-Path $path_deps) -eq $false ) {
	& $update_deps
}

if ( (Test-Path $path_binaries) -eq $false ) {
	New-Item $path_binaries -ItemType Directory
}

#region Handmade Runtime
$includes = @(
	$path_project
)

# Microsoft
$lib_gdi32  = 'Gdi32.lib'
$lib_xinput = 'Xinput.lib'
$lib_user32 = 'User32.lib'
$lib_winmm  = 'Winmm.lib'

# Github
$lib_jsl = Join-Path $path_deps 'JoyShockLibrary/x64/JoyShockLibrary.lib'

$stack_size = 1024 * 1024 * 4

$compiler_args = @(
	($flag_define + 'UNICODE'),
	($flag_define + '_UNICODE')
	# ($flag_set_stack_size + $stack_size)
	$flag_wall
	$flag_warnings_as_errors
	$flag_optimize_intrinsics
)

if ( $dev ) {
	$compiler_args += ( $flag_define + 'Build_Development=1' )
}
else {
	$compiler_args += ( $flag_define + 'Build_Development=0' )
}

function build-engine
{
	$path_pdb_lock = Join-Path $path_binaries 'handmade_engine.pdb.lock'
	New-Item $path_pdb_lock -ItemType File -Force

	# Delete old PDBs
	[Array]$pdb_files = Get-ChildItem -Path $path_binaries -Filter "handmade_engine_*.pdb"
	foreach ($file in $pdb_files) {
		Remove-Item -Path $file.FullName -Force
		if ( $verbose ) { Write-Host "Deleted $file" -ForegroundColor Green }
	}

	$local:includes = $script:includes
	$includes      += $path_engine

	$local:compiler_args = $script:compiler_args
	$compiler_args      += ($flag_define + 'Build_DLL=1' )

	if ( $vendor -eq 'msvc' )
	{
		$compiler_args += ($flag_define + 'Engine_API=__declspec(dllexport)')
	}
	if ( $vendor -eq 'clang' )
	{
		$compiler_args += ($flag_define + 'Engine_API=__attribute__((visibility("default")))')
	}

	$local:linker_args = @(
		$flag_link_dll
		# $flag_link_optimize_references
	)

	$unit            = Join-Path $path_project  'handmade_engine.cpp'
	$dynamic_library = Join-Path $path_binaries 'handmade_engine.dll'

	build-simple $path_build $includes $compiler_args $linker_args $unit $dynamic_library

	Remove-Item $path_pdb_lock -Force

	#region CodeGen Post-Build
	if ( -not $handmade_process_active ) {
		# Create the symbol table
		if ( Test-Path $dynamic_library )
		{
			# $data_path = Join-Path $path_data 'handmade_engine.dll'
			# move-item $dynamic_library $data_path -Force
			$path_lib = $dynamic_library -replace '\.dll', '.lib'
			$path_exp = $dynamic_library -replace '\.dll', '.exp'
			Remove-Item $path_lib -Force
			if ( Test-Path $path_exp ) { Remove-Item $path_exp -Force }

			# We need to generate the symbol table so that we can lookup the symbols we need when loading/reloading the library at runtime.
			# This is done by sifting through the emitter.map file from the linker for the base symbol names
			# and mapping them to their found decorated name

			# Initialize the hashtable with the desired order of symbols
			$engine_symbols = [ordered]@{
				'on_module_reload'   = $null
				'startup'            = $null
				'shutdown'           = $null
				'update_and_render'  = $null
				'update_audio'       = $null
			}

			$path_engine_obj = Join-Path $path_build 'handmade_engine.obj'
			$path_engine_map = Join-Path $path_build 'handmade_engine.map'
			$maxNameLength   = ($engine_symbols.Keys | Measure-Object -Property Length -Maximum).Maximum

			Get-Content -Path $path_engine_map | ForEach-Object {
				# If all symbols are found, exit the loop
				if ($engine_symbols.Values -notcontains $null) {
					return
				}
				# Split the line into tokens
				$tokens = $_ -split '\s+', 4
				# Extract only the decorated name using regex for both MSVC and Clang conventions
				$decoratedName = ($tokens[2] -match '(\?[\w@]+|_Z[\w@]+)') ? $matches[1] : $null

				# Check the origin of the symbol
				# If the origin matches 'handmade_engine.obj', then process the symbol
				$originParts = $tokens[3] -split '\s+'
				$origin = if ($originParts.Count -eq 3) { $originParts[2] } else { $originParts[1] }

				# Diagnostic output
				if ( $false -and $decoratedName) {
					write-host "Found decorated name: $decoratedName" -ForegroundColor Yellow
					write-host "Origin              : $origin" -ForegroundColor Yellow
				}

				if ($origin -like 'handmade_engine.obj') {
					# Check each regular name against the current line
					$engine_symbols.Keys | Where-Object { $engine_symbols[$_] -eq $null } | ForEach-Object {
						if ($decoratedName -like "*$_*") {
							$engine_symbols[$_] = $decoratedName
						}
					}
				}
			}

			if ($verbose) { write-host "Engine Symbol Table:" -ForegroundColor Green }
			$engine_symbols.GetEnumerator() | ForEach-Object {
				$paddedName    = $_.Key.PadRight($maxNameLength)
				$decoratedName = $_.Value
				if ($verbose ) { write-host "`t$paddedName, $decoratedName" -ForegroundColor Green }
			}

			# Write the symbol table to a file
			$path_engine_symbols = Join-Path $path_build 'handmade_engine.symbols'
			$engine_symbols.Values | Out-File -Path $path_engine_symbols
		}

		# Delete old PDBs
		$pdb_files = Get-ChildItem -Path $path_build -Filter "engine_postbuild_gen_*.pdb"
		foreach ($file in $pdb_files) {
			Remove-Item -Path $file.FullName -Force
			if ($verbose) { Write-Host "Deleted $file" -ForegroundColor Green }
		}

		$compiler_args = @()
		$compiler_args += ( $flag_define + 'GEN_TIME' )

		$linker_args = @(
			$flag_link_win_subsystem_console
		)

		$unit       = Join-Path $path_codegen 'engine_postbuild_gen.cpp'
		$executable = Join-Path $path_build   'engine_postbuild_gen.exe'

		build-simple $path_build $local:includes $compiler_args $linker_args $unit $executable

		Push-Location $path_build
		$time_taken = Measure-Command {
			& $executable 2>&1 | ForEach-Object {
				write-host `t $_ -ForegroundColor Green
			}
		}
		Pop-Location

		$path_generated_file = Join-Path $path_build 'engine_symbol_table.hpp'
		move-item $path_generated_file (join-path $path_gen (split-path $path_generated_file -leaf)) -Force
	}
}
if ( $engine ) {
	build-engine
}

function build-platform
{
	# CodeGen Pre-Build
	if ( $true )
	{
		# Delete old PDBs
		$pdb_files = Get-ChildItem -Path $path_build -Filter "platform_gen_*.pdb"
		foreach ($file in $pdb_files) {
			Remove-Item -Path $file.FullName -Force
			if ( $verbose ) { Write-Host "Deleted $file" -ForegroundColor Green }
		}

		$path_platform_gen = Join-Path $path_platform 'gen'

		if ( -not (Test-Path $path_platform_gen) ) {
			New-Item $path_platform_gen -ItemType Directory
		}

		$local:includes = $script:includes
		$includes      += $path_platform

		$local:compiler_args  = @()
		$compiler_args       += ( $flag_define + 'GEN_TIME' )

		$local:linker_args = @(
			$flag_link_win_subsystem_console
		)

		$unit       = Join-Path $path_codegen 'platform_gen.cpp'
		$executable = Join-Path $path_build 'platform_gen.exe'

		build-simple $path_build $includes $compiler_args $linker_args $unit $executable $path_build

		Push-Location $path_platform
		$time_taken = Measure-Command {
			& $executable 2>&1 | ForEach-Object {
				write-host `t $_ -ForegroundColor Green
			}
		}
		Pop-Location
	}

	# Delete old PDBs
	$pdb_files = Get-ChildItem -Path $path_binaries -Filter "handmade_win32_*.pdb"
	foreach ($file in $pdb_files) {
		Remove-Item -Path $file.FullName -Force
		if ( $verbose ) { Write-Host "Deleted $file" -ForegroundColor Green }
	}

	$local:compiler_args  = $script:compiler_args
	$compiler_args       += ($flag_define + 'Build_DLL=0' )

	$local:linker_args = @(
		$lib_gdi32,
		# $lib_xinput,
		$lib_user32,
		$lib_winmm,

		$lib_jsl,

		$flag_link_win_subsystem_windows,
		$flag_link_optimize_references
	)

	$unit       = Join-Path $path_project  'handmade_win32.cpp'
	$executable = Join-Path $path_binaries 'handmade_win32.exe'

	build-simple $path_build $includes $compiler_args $linker_args $unit $executable

	# if ( Test-Path $executable )
	# {
	# 	$data_path = Join-Path $path_data 'handmade_win32.exe'
	# 	move-item $executable $data_path -Force
	# }
}
if ( $platform ) {
	build-platform
}

$path_jsl_dll = Join-Path $path_binaries 'JoyShockLibrary.dll'
if ( (Test-Path $path_jsl_dll) -eq $false )
{
	$path_jsl_dep_dll = Join-Path $path_deps 'JoyShockLibrary/x64/JoyShockLibrary.dll'
	Copy-Item $path_jsl_dep_dll $path_jsl_dll
}
#endregion Handmade Runtime

push-location $path_scripts
$include = @(
	'*.cpp'
	'*.hpp'
)
format-cpp $path_gen $include
format-cpp (Join-Path $path_platform 'gen' ) $include
pop-location

Pop-Location
#endregion Building
