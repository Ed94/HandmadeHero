Clear-Host

$target_arch = Join-Path $PSScriptRoot 'helpers/target_arch.psm1'
$devshell    = Join-Path $PSScriptRoot 'helpers/devshell.ps1'
$path_root   = git rev-parse --show-toplevel
$path_build  = Join-Path $path_root    'build'

Import-Module $target_arch

Push-Location $path_root

#region Arguments
       $vendor       = $null
       $optimize     = $null
	   $debug 	     = $null
	   $analysis	 = $false
	   $dev          = $false
	   $platform     = $null
	   $engine       = $null
	   $game         = $null

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
	}
}}
#endregion Argument

#region Toolchain Configuration
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
	$flag_all_c 					   = '/TC'
	$flag_all_cpp                      = '/TP'
	$flag_compile                      = '-c'
	$flag_color_diagnostics            = '-fcolor-diagnostics'
	$flag_no_color_diagnostics         = '-fno-color-diagnostics'
	$flag_debug                        = '-g'
	$flag_debug_codeview               = '-gcodeview'
	$flag_define                       = '-D'
	$flag_exceptions_disabled		   = '-fno-exceptions'
	$flag_preprocess 			       = '-E'
	$flag_include                      = '-I'
	$flag_section_data                 = '-fdata-sections'
	$flag_section_functions            = '-ffunction-sections'
	$flag_library					   = '-l'
	$flag_library_path				   = '-L'
	$flag_linker                       = '-Wl,'
	if ( $IsWindows ) {
		$flag_link_dll                 = '/DLL'
		$flag_link_mapfile 		       = '/MAP:'
		$flag_link_optimize_references = '/OPT:REF'
	}
	if ( $IsLinux ) {
		$flag_link_mapfile              = '--Map='
		$flag_link_optimize_references  = '--gc-sections'
	}
	$flag_link_win_subsystem_console    = '/SUBSYSTEM:CONSOLE'
	$flag_link_win_subsystem_windows    = '/SUBSYSTEM:WINDOWS'
	$flag_link_win_machine_32           = '/MACHINE:X86'
	$flag_link_win_machine_64           = '/MACHINE:X64'
	$flag_link_win_debug                = '/DEBUG'
	$flag_link_win_pdb 			        = '/PDB:'
	$flag_link_win_path_output          = '/OUT:'
	$flag_no_optimization 		        = '-O0'
	$flag_optimize_fast 		        = '-O2'
	$flag_optimize_size 		        = '-O1'
	$flag_optimize_intrinsics		    = '-Oi'
	$flag_path_output                   = '-o'
	$flag_preprocess_non_intergrated    = '-no-integrated-cpp'
	$flag_profiling_debug               = '-fdebug-info-for-profiling'
	$flag_set_stack_size			    = '-stack='
	$flag_syntax_only				    = '-fsyntax-only'
	$flag_target_arch				    = '-target'
	$flag_wall 					        = '-Wall'
	$flag_warning 					    = '-W'
	$flag_warnings_as_errors            = '-Werror'
	$flag_win_nologo 			        = '/nologo'

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
		param( [array]$includes, [array]$compiler_args, [array]$linker_args, [string]$unit, [string]$binary )
		Write-Host "build-simple: clang"

		$object = $unit -replace '\.cpp', '.obj'
		$map    = $unit -replace '\.cpp', '.map'
		$object = join-path $path_build (split-path $object -Leaf)
		$map    = join-path $path_build (split-path $map    -Leaf)

		# The PDB file has to also be time-stamped so that we can reload the DLL at runtime
		$timestamp = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
		$pdb       = $binary -replace '\.(exe|dll)$', "_$timestamp.pdb"

		$compiler_args += @(
			$flag_no_color_diagnostics,
			$flag_exceptions_disabled,
			$flag_target_arch, $target_arch,
			$flag_wall,
			$flag_preprocess_non_intergrated,
			$flag_section_data,
			$flag_section_functions,
			( $flag_path_output + $object )
		)
		if ( $optimize ) {
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
			$( $flag_link_win_path_output + $binary )
		)
		if ( $debug ) {
			$linker_args += $flag_link_win_debug
			$linker_args += $flag_link_win_pdb + $pdb
			$linker_args += $flag_link_mapfile + $map
		}

		$libraries | ForEach-Object {
			$linker_args += $_ + '.lib'
		}

		$linker_args += $object
		run-linker $linker $binary $linker_args

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
	$flag_link_dll                   = '/DLL'
	$flag_link_no_incremental 	     = '/INCREMENTAL:NO'
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
		param( [array]$includes, [array]$compiler_args, [array]$linker_args, [string]$unit, [string]$binary )
		Write-Host "build-simple: msvc"

		$object = $unit -replace '\.(cpp)$', '.obj'
		$map    = $unit -replace '\.(cpp)$', '.map'
		$object = join-path $path_build (split-path $object -Leaf)
		$map    = join-path $path_build (split-path $map    -Leaf)

		# The PDB file has to also be time-stamped so that we can reload the DLL at runtime
		$timestamp = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
		$pdb       = $binary -replace '\.(exe|dll)$', "_$timestamp.pdb"

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

			if ( $optimize ) {
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
			$flag_link_no_incremental,
			( $flag_link_win_path_output + $binary )
		)
		if ( $debug ) {
			$linker_args += $flag_link_win_debug
			$linker_args += $flag_link_win_pdb + $pdb
			$linker_args += $flag_link_mapfile + $map
		}
		else {
		}

		$linker_args += $object
		run-linker $linker $binary $linker_args

		# $compiler_args += $unit
		# $compiler_args += $flag_linker
		# $compiler_args += $linker_args
		# run-compiler $compiler $unit $compiler_args
	}

	$compiler = 'cl'
	$linker   = 'link'
}
#endregion Configuration

#region Building
$path_project  = Join-Path $path_root    'project'
$path_data     = Join-Path $path_root	 'data'
$path_binaries = Join-Path $path_data    'binaries'
$path_deps     = Join-Path $path_project 'dependencies'
$path_codegen  = Join-Path $path_project 'codegen'
$path_gen      = Join-Path $path_project 'gen'
$path_platform = Join-Path $path_project 'platform'
$path_engine   = Join-Path $path_project 'engine'

$update_deps = Join-Path $PSScriptRoot 'update_deps.ps1'

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

	($flag_define + 'Build_Unity=1' )
)

if ( $dev ) {
	$compiler_args += ( $flag_define + 'Build_Development=1' )
}
else {
	$compiler_args += ( $flag_define + 'Build_Development=0' )
}

if ( $engine )
{
	$path_pdb_lock = Join-Path $path_binaries 'handmade_engine.pdb.lock'
	New-Item $path_pdb_lock -ItemType File -Force -Verbose

	# Delete old PDBs
	[Array]$pdb_files = Get-ChildItem -Path $path_binaries -Filter "handmade_engine_*.pdb"
	foreach ($file in $pdb_files) {
		Remove-Item -Path $file.FullName -Force
		Write-Host "Deleted $file" -ForegroundColor Green
	}

	$engine_compiler_args = $compiler_args
	$engine_compiler_args += ($flag_define + 'Build_DLL=1' )

	if ( $vendor -eq 'msvc' )
	{
		$engine_compiler_args += ($flag_define + 'Engine_API=__declspec(dllexport)')
	}
	if ( $vendor -eq 'clang' )
	{
		$engine_compiler_args += ($flag_define + 'Engine_API=__attribute__((visibility("default")))')
	}

	$linker_args = @(
		$flag_link_dll,
		$flag_link_optimize_references
	)

	$unit            = Join-Path $path_project  'handmade_engine.cpp'
	$dynamic_library = Join-Path $path_binaries 'handmade_engine.dll'

	build-simple $includes $engine_compiler_args $linker_args $unit $dynamic_library

	if ( Test-Path $dynamic_library )
	{
		# $data_path = Join-Path $path_data 'handmade_engine.dll'
		# move-item $dynamic_library $data_path -Force
		$path_lib = $dynamic_library -replace '\.dll', '.lib'
		$path_exp = $dynamic_library -replace '\.dll', '.exp'
		Remove-Item $path_lib -Force
		Remove-Item $path_exp -Force

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

		write-host "Engine Symbol Table:" -ForegroundColor Green
		$engine_symbols.GetEnumerator() | ForEach-Object {
			$paddedName    = $_.Key.PadRight($maxNameLength)
			$decoratedName = $_.Value
			write-host "`t$paddedName, $decoratedName" -ForegroundColor Green
		}

		# Write the symbol table to a file
		$path_engine_symbols = Join-Path $path_build 'handmade_engine.symbols'
		$engine_symbols.Values | Out-File -Path $path_engine_symbols
	}

	Remove-Item $path_pdb_lock -Force -Verbose
}

if ( $platform )
{

	#region CodeGen
	if ( $true ) {
		$engine_codegen_compiler_args = @()
		$engine_codegen_compiler_args += ( $flag_define + 'GEN_TIME' )

		$engine_codegen_linker_args = @(
			$flag_link_win_subsystem_console
		)

		$unit       = Join-Path $path_codegen 'handmade_platform_gen.cpp'
		$executable = Join-Path $path_build   'handmade_platform_gen.exe'

		build-simple $includes $engine_codegen_compiler_args $engine_codegen_linker_args $unit $executable
		write-host

		Push-Location $path_build
		& $executable
		Pop-Location
		write-host

		$path_generated_file = Join-Path $path_build 'engine_symbol_table.hpp'
		move-item $path_generated_file (join-path $path_gen (split-path $path_generated_file -leaf)) -Force
	}
	#endregion CodeGen

	# Delete old PDBs 
	$pdb_files = Get-ChildItem -Path $path_binaries -Filter "handmade_win32_*.pdb"
	foreach ($file in $pdb_files) {
		Remove-Item -Path $file.FullName -Force
		Write-Host "Deleted $file" -ForegroundColor Green
	}

	$platform_compiler_args  = $compiler_args
	$platform_compiler_args += ($flag_define + 'Build_DLL=0' )

	$linker_args = @(
		$lib_gdi32,
		# $lib_xinput,
		$lib_user32,
		$lib_winmm,

		$lib_jsl,

		$flag_link_win_subsystem_windows
		$flag_link_optimize_references
	)

	$unit       = Join-Path $path_project  'handmade_win32.cpp'
	$executable = Join-Path $path_binaries 'handmade_win32.exe'

	build-simple $includes $platform_compiler_args $linker_args $unit $executable

	# if ( Test-Path $executable )
	# {
	# 	$data_path = Join-Path $path_data 'handmade_win32.exe'
	# 	move-item $executable $data_path -Force
	# }
}

$path_jsl_dll = Join-Path $path_binaries 'JoyShockLibrary.dll'
if ( (Test-Path $path_jsl_dll) -eq $false )
{
	$path_jsl_dep_dll = Join-Path $path_deps 'JoyShockLibrary/x64/JoyShockLibrary.dll'
	Copy-Item $path_jsl_dep_dll $path_jsl_dll
}
#endregion Handmade Runtime

Pop-Location
#endregion Building
