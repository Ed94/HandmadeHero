Clear-Host

Import-Module ./helpers/target_arch.psm1
$devshell  = Join-Path $PSScriptRoot 'helpers/devshell.ps1'
$path_root = git rev-parse --show-toplevel

Push-Location $path_root

#region Arguments
       $vendor       = $null
       $release      = $null

[array] $vendors = @( "clang", "msvc" )

# This is a really lazy way of parsing the args, could use actual params down the line...

if ( $args ) { $args | ForEach-Object {
	switch ($_){
		{ $_ -in $vendors }   { $vendor      = $_; break }
		"release"             { $release      = $true }
		"debug"               { $release      = $false }
	}
}}
#endregion Arguments

#region Configuration
if ($IsWindows) {
	# This library was really designed to only run on 64-bit systems.
	# (Its a development tool after all)
    & $devshell -arch amd64
}

if ( $vendor -eq $null ) {
	write-host "No vendor specified, assuming clang available"
	$compiler = "clang"
}

if ( $release -eq $null ) {
	write-host "No build type specified, assuming debug"
	$release = $false
}

write-host "Building HandmadeHero with $vendor"
write-host "Build Type: $(if ($release) {"Release"} else {"Debug"} )"

function run-compiler
{
	param( $compiler, $unit, $compiler_args )

	$compiler_args += @(
		($flag_define + 'UNICODE'),
		($flag_define + '_UNICODE')
	)

	write-host "`Compiling $unit"
	write-host "Compiler config:"
	$compiler_args | ForEach-Object {
		write-host $_ -ForegroundColor Cyan
	}

	$time_taken = Measure-Command {
		& $compiler $compiler_args 2>&1 | ForEach-Object {
			$color = 'White'
			switch ($_){
				{ $_ -match "error"   } { $color = 'Red'   ; break }
				{ $_ -match "warning" } { $color = 'Yellow'; break }
			}
			write-host `t $_ -ForegroundColor $color
		}
	}

	if ( Test-Path($unit) ) {
		write-host "$unit compile finished in $($time_taken.TotalMilliseconds) ms"
	}
	else {
		write-host "Compile failed for $unit" -ForegroundColor Red
	}
}

function run-linker
{
	param( $linker, $binary, $linker_args )

	write-host "`Linking $binary"
	write-host "Linker config:"
	$linker_args | ForEach-Object {
		write-host $_ -ForegroundColor Cyan
	}

	$time_taken = Measure-Command {
		& $linker $linker_args 2>&1 | ForEach-Object {
			$color = 'White'
			switch ($_){
				{ $_ -match "error"   } { $color = 'Red'   ; break }
				{ $_ -match "warning" } { $color = 'Yellow'; break }
			}
			write-host `t $_ -ForegroundColor $color
		}
	}

	if ( Test-Path($binary) ) {
		write-host "$binary linking finished in $($time_taken.TotalMilliseconds) ms"
	}
	else {
		write-host "Linking failed for $binary" -ForegroundColor Red
	}
}

if ( $vendor -match "clang" )
{
	# https://clang.llvm.org/docs/ClangCommandLineReference.html
	$flag_compile                    = '-c'
	$flag_color_diagnostics          = '-fcolor-diagnostics'
	$flag_no_color_diagnostics       = '-fno-color-diagnostics'
	$flag_debug                      = '-g'
	$flag_debug_codeview             = '-gcodeview'
	$flag_define                     = '-D'
	$flag_preprocess 			     = '-E'
	$flag_include                    = '-I'
	$flag_library					 = '-l'
	$flag_library_path				 = '-L'
	$flag_link_win                   = '-Wl,'
	$flag_link_win_subsystem_console = '/SUBSYSTEM:CONSOLE'
	$flag_link_win_machine_32        = '/MACHINE:X86'
	$flag_link_win_machine_64        = '/MACHINE:X64'
	$flag_link_win_debug             = '/DEBUG'
	$flag_link_win_pdb 			     = '/PDB:'
	$flag_link_win_path_output       = '/OUT:'
	$flag_no_optimization 		     = '-O0'
	$flag_path_output                = '-o'
	$flag_preprocess_non_intergrated = '-no-integrated-cpp'
	$flag_profiling_debug            = '-fdebug-info-for-profiling'
	$flag_target_arch				 = '-target'
	$flag_wall 					     = '-Wall'
	$flag_warning 					 = '-W'
	$flag_warning_as_error 		     = '-Werror'
	$flag_win_nologo 			     = '/nologo'

	$ignore_warning_ms_include = 'no-microsoft-include'

	$target_arch = Get-TargetArchClang

	$warning_ignores = @(
		$ignore_warning_ms_include
	)

	# https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-170
	$libraries = @(
		'Kernel32' # For Windows API
		# 'msvcrt', # For the C Runtime (Dynamically Linked)
		# 'libucrt',
		'libcmt'    # For the C Runtime (Static Linkage)
	)

	function build-simple
	{
		param( [array]$includes, [array]$compiler_args, [array]$linker_args, [string]$unit, [string]$executable )
		Write-Host "build-simple: clang"

		$object = $executable -replace '\.exe', '.obj'
		$pdb    = $executable -replace '\.exe', '.pdb'

		$compiler_args += @(
			$flag_no_color_diagnostics,
			$flag_target_arch, $target_arch,
			$flag_wall,
			$flag_preprocess_non_intergrated,
			( $flag_path_output + $object )
		)
		if ( $release -eq $false ) {
			$compiler_args += ( $flag_define + 'Build_Debug' )
			$compiler_args += $flag_debug, $flag_debug_codeview, $flag_profiling_debug
			$compiler_args += $flag_no_optimization
		}

		$warning_ignores | ForEach-Object {
			$compiler_args += $flag_warning + $_
		}
		$compiler_args += $includes | ForEach-Object { $flag_include + $_ }

		$compiler_args += $flag_compile, $unit
		run-compiler $compiler $unit $compiler_args

		$linker_args += @(
			$flag_link_win_machine_64,
			$( $flag_link_win_path_output + $executable )
		)
		if ( $release -eq $false ) {
			$linker_args += $flag_link_win_debug
			$linker_args += $flag_link_win_pdb + $pdb
		}
		else {
		}

		$libraries | ForEach-Object {
			$linker_args += $_ + '.lib'
		}

		$linker_args += $object
		run-linker $linker $executable $linker_args
	}

	$compiler = 'clang++'
	$linker   = 'lld-link'
}

if ( $vendor -match "msvc" )
{
	# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-170
	$flag_compile			         = '/c'
	$flag_debug                      = '/Zi'
	$flag_define		             = '/D'
	$flag_include                    = '/I'
	$flag_full_src_path              = '/FC'
	$flag_nologo                     = '/nologo'
	$flag_dll 				         = '/LD'
	$flag_dll_debug 			     = '/LDd'
	$flag_linker 		             = '/link'
	$flag_link_win_debug 	         = '/DEBUG'
	$flag_link_win_pdb 		         = '/PDB:'
	$flag_link_win_machine_32        = '/MACHINE:X86'
	$flag_link_win_machine_64        = '/MACHINE:X64'
	$flag_link_win_path_output       = '/OUT:'
	$flag_link_win_rt_dll            = '/MD'
	$flag_link_win_rt_dll_debug      = '/MDd'
	$flag_link_win_rt_static         = '/MT'
	$flag_link_win_rt_static_debug   = '/MTd'
	$flag_link_win_subsystem_console = '/SUBSYSTEM:CONSOLE'
	$flag_link_win_subsystem_windows = '/SUBSYSTEM:WINDOWS'
	$flag_no_optimization		     = '/Od'
	$flag_out_name                   = '/OUT:'
	$flag_path_interm                = '/Fo'
	$flag_path_debug                 = '/Fd'
	$flag_path_output                = '/Fe'
	$flag_preprocess_conform         = '/Zc:preprocessor'

	# This works because this project uses a single unit to build
	function build-simple
	{
		param( [array]$includes, [array]$compiler_args, [array]$linker_args, [string]$unit, [string]$executable )
		Write-Host "build-simple: msvc"

		$object = $executable -replace '\.exe', '.obj'
		$pdb    = $executable -replace '\.exe', '.pdb'

		$compiler_args += @(
			$flag_nologo,
			$flag_preprocess_conform,
			$flag_full_src_path,
			( $flag_path_interm + $path_build + '\' ),
			( $flag_path_output + $path_build + '\' )
		)
		if ( $release -eq $false ) {
			$compiler_args += $flag_debug
			$compiler_args += ( $flag_define + 'Build_Debug' )
			$compiler_args += ( $flag_path_debug + $path_build + '\' )
			$compiler_args += $flag_link_win_rt_static_debug
			$compiler_args += $flag_no_optimization
		}
		else {
			$compiler_args += $flag_link_win_rt_static
		}
		$compiler_args += $includes | ForEach-Object { $flag_include + $_ }
		$compiler_args += $flag_compile, $unit
		run-compiler $compiler $unit $compiler_args

		$linker_args += @(
			$flag_nologo,
			$flag_link_win_machine_64,
			( $flag_link_win_path_output + $executable )
		)
		if ( $release -eq $false ) {
			$linker_args += $flag_link_win_debug
			$linker_args += $flag_link_win_pdb + $pdb
		}
		else {
		}

		$linker_args += $object
		run-linker $linker $executable $linker_args
	}

	$compiler = 'cl'
	$linker   = 'link'
}
#endregion Configuration

$path_project  = Join-Path $path_root    'project'
$path_build    = Join-Path $path_root    'build'
$path_deps     = Join-Path $path_project 'dependencies'
$path_gen      = Join-Path $path_project 'gen'
$path_platform = Join-Path $path_project 'platform'

$update_deps = Join-Path $PSScriptRoot 'update_deps.ps1'

if ( (Test-Path $path_build) -eq $false ) {
	New-Item $path_build -ItemType Directory
}

if ( (Test-Path $path_deps) -eq $false ) {
	& $update_deps
}

$includes = @(
	$path_project,
	$path_gen,
	$path_deps,
	$path_platform
)
$compiler_args = @()
$compiler_args += ( $flag_define + 'GEN_TIME' )

$linker_args = @(
	$flag_link_win_subsystem_console
)

#region Handmade Generate
if ( $false ) {
	$unit       = Join-Path $path_gen   'handmade_gen.cpp'
	$executable = Join-Path $path_build 'handmade_gen.exe'

	build-simple $includes $compiler_args $linker_args $unit $executable
	write-host

	& $executable
	write-host

	if ( $false ) {
		Remove-Item (Get-ChildItem -Path $path_build -Recurse -Force)
	}
}
#endregion Handmade Generate

#region Handmade Runtime
$includes = @(
	$path_project,
	$path_gen,
	$path_deps,
	$path_platform
)

# Microsoft
$lib_gdi32  = 'Gdi32.lib'
$lib_xinput = 'Xinput.lib'
$lib_user32 = 'User32.lib'

# Github
$lib_jsl = Join-Path $path_deps 'JoyShockLibrary/x64/JoyShockLibrary.lib'

$unit       = Join-Path $path_project 'handmade_win32.cpp'
$executable = Join-Path $path_build   'handmade_win32.exe'

$compiler_args = @()

$linker_args = @(
	$lib_gdi32,
	# $lib_xinput,
	$lib_user32,

	$lib_jsl,

	$flag_link_win_subsystem_windows
)

build-simple $includes $compiler_args $linker_args $unit $executable
#endregion Handmade Runtime

Pop-Location
