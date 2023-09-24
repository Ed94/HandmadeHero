Clear-Host

$target_arch = Join-Path $PSScriptRoot 'helpers/target_arch.psm1'
$devshell    = Join-Path $PSScriptRoot 'helpers/devshell.ps1'
$path_root   = git rev-parse --show-toplevel

Import-Module $target_arch

Push-Location $path_root

#region Arguments
       $vendor       = $null
       $optimized    = $null
	   $debug 	     = $null
	   $analysis	 = $false
	   $dev          = $false

[array] $vendors = @( "clang", "msvc" )

# This is a really lazy way of parsing the args, could use actual params down the line...

if ( $args ) { $args | ForEach-Object {
	switch ($_){
		{ $_ -in $vendors }   { $vendor    = $_; break }
		"optimized"           { $optimized = $true }
		"debug"               { $debug     = $true }
		"analysis"            { $analysis  = $true }
		"dev"                 { $dev       = $true }
	}
}}
#endregion Argument

#region Configuration
if ($IsWindows) {
	# This HandmadeHero implementation is only designed for 64-bit systems
    & $devshell -arch amd64
}

if ( $vendor -eq $null ) {
	write-host "No vendor specified, assuming clang available"
	$compiler = "clang"
}

write-host "Building HandmadeHero with $vendor"

if ( $dev ) {
	if ( $debug -eq $null ) {
		$debug = $true
	}

	if ( $optimize -eq $null ) {
		$optimize = $false
	}
}

function run-compiler
{
	param( $compiler, $unit, $compiler_args )

	if ( $analysis ) {
		$compiler_args += $flag_syntax_only
	}

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
	$flag_all_c 					 = '/TC'
	$flag_all_cpp                    = '/TP'
	$flag_compile                    = '-c'
	$flag_color_diagnostics          = '-fcolor-diagnostics'
	$flag_no_color_diagnostics       = '-fno-color-diagnostics'
	$flag_debug                      = '-g'
	$flag_debug_codeview             = '-gcodeview'
	$flag_define                     = '-D'
	$flag_exceptions_disabled		 = '-fno-exceptions'
	$flag_preprocess 			     = '-E'
	$flag_include                    = '-I'
	$flag_library					 = '-l'
	$flag_library_path				 = '-L'
	$flag_linker                     = '-Wl,'
	$flag_link_mapfile 				 = '-Map'
	$flag_link_win_subsystem_console = '/SUBSYSTEM:CONSOLE'
	$flag_link_win_subsystem_windows = '/SUBSYSTEM:WINDOWS'
	$flag_link_win_machine_32        = '/MACHINE:X86'
	$flag_link_win_machine_64        = '/MACHINE:X64'
	$flag_link_win_debug             = '/DEBUG'
	$flag_link_win_pdb 			     = '/PDB:'
	$flag_link_win_path_output       = '/OUT:'
	$flag_no_optimization 		     = '-O0'
	$flag_optimize_fast 		     = '-O2'
	$flag_optimize_size 		     = '-O1'
	$flag_optimize_intrinsics		 = '-Oi'
	$flag_path_output                = '-o'
	$flag_preprocess_non_intergrated = '-no-integrated-cpp'
	$flag_profiling_debug            = '-fdebug-info-for-profiling'
	$flag_set_stack_size			 = '-stack='
	$flag_syntax_only				 = '-fsyntax-only'
	$flag_target_arch				 = '-target'
	$flag_wall 					     = '-Wall'
	$flag_warning 					 = '-W'
	$flag_warnings_as_errors         = '-Werror'
	$flag_win_nologo 			     = '/nologo'

	$ignore_warning_ms_include            = 'no-microsoft-include'
	$ignore_warning_return_type_c_linkage = 'no-return-type-c-linkage'

	$target_arch = Get-TargetArchClang

	$warning_ignores = @(
		$ignore_warning_ms_include,
		$ignore_warning_return_type_c_linkage
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
		$map    = $executable -replace '\.exe', '.map'

		$compiler_args += @(
			$flag_no_color_diagnostics,
			$flag_exceptions_disabled,
			$flag_target_arch, $target_arch,
			$flag_wall,
			$flag_preprocess_non_intergrated,
			( $flag_path_output + $object )
		)
		if ( $optimized ) {
			$compiler_args += $flag_optimize_fast
		}
		else {
			$compiler_args += $flag_no_optimization
		}
		if ( $debug ) {
			$compiler_args += ( $flag_define + 'Build_Debug=1' )
			$compiler_args += $flag_debug, $flag_debug_codeview, $flag_profiling_debug
		}
		else {
			$compiler_args += ( $flag_define + 'Build_Debug=0' )
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
		if ( $debug ) {
			$linker_args += $flag_link_win_debug
			$linker_args += $flag_link_win_pdb + $pdb
			# $linker_args += $flag_link_mapfile + $map
		}

		$libraries | ForEach-Object {
			$linker_args += $_ + '.lib'
		}

		$linker_args += $object
		run-linker $linker $executable $linker_args

		# $compiler_args += $unit
		# $linker_args | ForEach-Object {
		# 	$compiler_args += $flag_linker + $_
		# }
		# run-compiler $compiler $unit $compiler_args
	}

	$compiler = 'clang++'
	$linker   = 'lld-link'
}

if ( $vendor -match "msvc" )
{
	# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-170
	$flag_all_c 					 = '/TC'
	$flag_all_cpp                    = '/TP'
	$flag_compile			         = '/c'
	$flag_debug                      = '/Zi'
	$flag_define		             = '/D'
	$flag_exceptions_disabled		 = '/EHsc-'
	$flag_RTTI_disabled				 = '/GR-'
	$flag_include                    = '/I'
	$flag_full_src_path              = '/FC'
	$flag_nologo                     = '/nologo'
	$flag_dll 				         = '/LD'
	$flag_dll_debug 			     = '/LDd'
	$flag_linker 		             = '/link'
	$flag_link_mapfile 				 = '/MAP:'
	$flag_link_optimize_references   = '/OPT:REF'
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
	$flag_optimize_fast 		     = '/O2'
	$flag_optimize_size 		     = '/O1'
	$flag_optimize_intrinsics		 = '/Oi'
	$flag_optimized_debug			 = '/Zo'
	$flag_out_name                   = '/OUT:'
	$flag_path_interm                = '/Fo'
	$flag_path_debug                 = '/Fd'
	$flag_path_output                = '/Fe'
	$flag_preprocess_conform         = '/Zc:preprocessor'
	$flag_set_stack_size			 = '/F'
	$flag_syntax_only				 = '/Zs'
	$flag_wall 					     = '/Wall'
	$flag_warnings_as_errors 		 = '/WX'

	# This works because this project uses a single unit to build
	function build-simple
	{
		param( [array]$includes, [array]$compiler_args, [array]$linker_args, [string]$unit, [string]$executable )
		Write-Host "build-simple: msvc"

		$object = $executable -replace '\.exe', '.obj'
		$pdb    = $executable -replace '\.exe', '.pdb'
		$map    = $executable -replace '\.exe', '.map'

		$compiler_args += @(
			$flag_nologo,
			# $flag_all_cpp,
			$flag_exceptions_disabled,
			( $flag_define + '_HAS_EXCEPTIONS=0' ),
			$flag_RTTI_disabled,
			$flag_preprocess_conform,
			$flag_full_src_path,
			( $flag_path_interm + $path_build + '\' ),
			( $flag_path_output + $path_build + '\' )
		)

		if ( $optimize ) {
			$compiler_args += $flag_optimize_fast
		}
		else {
			$compiler_args += $flag_no_optimization
		}

		if ( $debug )
		{
			$compiler_args += $flag_debug
			$compiler_args += ( $flag_define + 'Build_Debug=1' )
			$compiler_args += ( $flag_path_debug + $path_build + '\' )
			$compiler_args += $flag_link_win_rt_static_debug

			if ( $optimized ) {
				$compiler_args += $flag_optimized_debug
			}
		}
		else {
			$compiler_args += ( $flag_define + 'Build_Debug=0' )
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
		if ( $debug ) {
			$linker_args += $flag_link_win_debug
			$linker_args += $flag_link_win_pdb + $pdb
			# $linker_args += $flag_link_mapfile + $map
		}
		else {
		}

		$linker_args += $object
		run-linker $linker $executable $linker_args

		# $compiler_args += $unit
		# $compiler_args += $flag_linker
		# $compiler_args += $linker_args
		# run-compiler $compiler $unit $compiler_args
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
	# $path_deps,
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
$lib_winmm  = 'Winmm.lib'

# Github
$lib_jsl = Join-Path $path_deps 'JoyShockLibrary/x64/JoyShockLibrary.lib'

$unit       = Join-Path $path_project 'handmade_win32.cpp'
$executable = Join-Path $path_build   'handmade_win32.exe'

$stack_size = 1024 * 1024 * 4

$compiler_args = @(
	($flag_define + 'UNICODE'),
	($flag_define + '_UNICODE')
	# ($flag_set_stack_size + $stack_size)
	$flag_wall
	$flag_warnings_as_errors
	$flag_optimize_intrinsics

	($flag_define + 'Build_DLL=0' )

	# For now this script only supports unity builds... (for the full binary)
	($flag_define + 'Build_Unity=1' )
)

if ( $dev ) {
	$compiler_args += ( $flag_define + 'Build_Development=1' )
}
else {
	$compiler_args += ( $flag_define + 'Build_Development=0' )
}

$linker_args = @(
	$lib_gdi32,
	# $lib_xinput,
	$lib_user32,
	$lib_winmm,

	$lib_jsl,

	$flag_link_win_subsystem_windows
	$flag_link_optimize_references
)

build-simple $includes $compiler_args $linker_args $unit $executable
#endregion Handmade Runtime

Pop-Location
