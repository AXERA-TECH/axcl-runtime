@echo off
setlocal enabledelayedexpansion

:: ============================================================================
:: Start timing
:: ============================================================================
set "BUILD_START_TIME=!TIME!"
echo Build started at: !BUILD_START_TIME!
echo.

:: ============================================================================
:: Automated CMake Build Script for Windows x64
:: Location: scripts/build_win64.cmd
:: ============================================================================

set "VERSION_FILE=%SCRIPT_DIR%..\build\version.mak"
set "BUILD_VERSION="
if exist "%VERSION_FILE%" (
    for /f "usebackq tokens=1* delims==" %%a in ("%VERSION_FILE%") do (
        set "KEY=%%a"
        set "KEY=!KEY: =!"
        set "VALUE=%%b"
        set "VALUE=!VALUE: =!"

        if /i "!KEY!"=="SDK_VERSION" (
            set "BUILD_VERSION=!VALUE!"
        )
    )

    if "!BUILD_VERSION!"=="" (
        echo ERROR: SDK_VERSION not found in %VERSION_FILE%
        set "BUILD_VERSION=0.0.0"
    )
) else (
    echo WARNING: %VERSION_FILE% not exist
    set "BUILD_VERSION=0.0.0"
)

:: ============================================================================
:: Configuration Section - Modify as needed
:: ============================================================================
set "BUILD_DIR=build\out\axcl_win_x64"
set "INSTALL_DIR=out\axcl_win_x64"
set "CMAKE_SOURCE_DIR=cmake"
set "ARCH=x64"
set "PARALLEL_JOBS=0"

:: ============================================================================
:: Auto-detect CPU cores if PARALLEL_JOBS is 0
:: ============================================================================
if "%PARALLEL_JOBS%"=="0" (
    for /f %%i in ('wmic cpu get NumberOfCores /value ^| find "="') do (
        for /f "tokens=2 delims==" %%j in ("%%i") do set "PARALLEL_JOBS=%%j"
    )

    if defined PARALLEL_JOBS (
        echo Detected !PARALLEL_JOBS! physical CPU cores
    ) else (
        set "PARALLEL_JOBS=8"
    )
)

:: ============================================================================
:: Parse build type parameter (debug/release, default release)
:: ============================================================================
set "BUILD_TYPE=Release"
if /i "%~1"=="debug" set "BUILD_TYPE=Debug"
if /i "%~1"=="release" set "BUILD_TYPE=Release"

:: ============================================================================
:: Parse build system parameter (ninja/msbuild, default ninja if available)
:: ============================================================================
set "USE_NINJA=true"
if /i "%~2"=="ninja" set "USE_NINJA=true"
if /i "%~2"=="msbuild" set "USE_NINJA=false"

if "%USE_NINJA%"=="true" (
    set "BUILD_SYSTEM=ninja"
) else (
    set "BUILD_SYSTEM=msbuild"
)

:: ============================================================================
:: Get project root directory (script is in scripts subdirectory)
:: ============================================================================
set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."
cd /d "%PROJECT_ROOT%"
set "PROJECT_ROOT=%CD%"

:: Convert to absolute paths
set "BUILD_PATH=%PROJECT_ROOT%\%BUILD_DIR%"
set "INSTALL_PATH=%PROJECT_ROOT%\%INSTALL_DIR%"
set "CMAKE_SOURCE_PATH=%PROJECT_ROOT%\%CMAKE_SOURCE_DIR%"

:: ============================================================================
:: Check if Ninja is available
:: ============================================================================
if "%USE_NINJA%"=="true" (
    echo Checking for Ninja build system...
    ninja --version >nul 2>&1
    if errorlevel 1 (
        echo WARNING: Ninja not found in PATH, falling back to MSBuild
        set "USE_NINJA=false"
    ) else (
        for /f "tokens=*" %%i in ('ninja --version') do (
            echo Found Ninja version: %%i
        )
    )
)

:: ============================================================================
:: Output configuration information
:: ============================================================================
echo ========================================
echo CMake Windows x64 Build Script
echo ========================================
echo Parallel Jobs :  %PARALLEL_JOBS%
echo Build Type    :  %BUILD_TYPE%
echo Build System  :  %BUILD_SYSTEM%
echo Project Root  :  %PROJECT_ROOT%
echo CMake Source  :  %CMAKE_SOURCE_PATH%
echo Build Dir     :  %BUILD_PATH%
echo Install Dir   :  %INSTALL_PATH%
echo ========================================
echo.

:: ============================================================================
:: Auto-detect Visual Studio / Build Tools (always needed for C++ compilation)
:: ============================================================================
echo Detecting Visual Studio / Build Tools...

set "VCVARS_PATH="
set "CMAKE_GENERATOR="

    :: 1. Use vswhere tool to find VS/BuildTools
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

    if exist "%VSWHERE%" (
        :: Find the latest version of VS or Build Tools
        for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
            set "VS_INSTALL_PATH=%%i"
        )

        if defined VS_INSTALL_PATH (
            if exist "!VS_INSTALL_PATH!\VC\Auxiliary\Build\vcvarsall.bat" (
                set "VCVARS_PATH=!VS_INSTALL_PATH!\VC\Auxiliary\Build\vcvarsall.bat"

                :: Get major version number to determine Generator
                for /f "usebackq tokens=1 delims=." %%i in (`"%VSWHERE%" -latest -products * -property installationVersion`) do (
                    set "VS_MAJOR_VERSION=%%i"
                )

                if "!VS_MAJOR_VERSION!"=="17" set "CMAKE_GENERATOR=Visual Studio 17 2022"
                if "!VS_MAJOR_VERSION!"=="16" set "CMAKE_GENERATOR=Visual Studio 16 2019"
                if "!VS_MAJOR_VERSION!"=="15" set "CMAKE_GENERATOR=Visual Studio 15 2017"

                goto :found_compiler
            )
        )
    )

    :: 2. Manually search common installation paths
    set "SEARCH_PATHS[0]=%ProgramFiles%\Microsoft Visual Studio\2022"
    set "SEARCH_PATHS[1]=%ProgramFiles(x86)%\Microsoft Visual Studio\2022"
    set "SEARCH_PATHS[2]=%ProgramFiles%\Microsoft Visual Studio\2019"
    set "SEARCH_PATHS[3]=%ProgramFiles(x86)%\Microsoft Visual Studio\2019"

    set "EDITIONS[0]=Enterprise"
    set "EDITIONS[1]=Professional"
    set "EDITIONS[2]=Community"
    set "EDITIONS[3]=BuildTools"

    :: First search for VS2022
    for /L %%i in (0,1,1) do (
        for /L %%j in (0,1,3) do (
            call set "CHECK_PATH=%%SEARCH_PATHS[%%i]%%\%%EDITIONS[%%j]%%\VC\Auxiliary\Build\vcvarsall.bat"
            if exist "!CHECK_PATH!" (
                set "VCVARS_PATH=!CHECK_PATH!"
                set "CMAKE_GENERATOR=Visual Studio 17 2022"
                goto :found_compiler
            )
        )
    )

    :: Then search for VS2019
    for /L %%i in (2,1,3) do (
        for /L %%j in (0,1,3) do (
            call set "CHECK_PATH=%%SEARCH_PATHS[%%i]%%\%%EDITIONS[%%j]%%\VC\Auxiliary\Build\vcvarsall.bat"
            if exist "!CHECK_PATH!" (
                set "VCVARS_PATH=!CHECK_PATH!"
                set "CMAKE_GENERATOR=Visual Studio 16 2019"
                goto :found_compiler
            )
        )
    )

    :: 3. Search through registry
    for /f "tokens=2*" %%A in ('reg query "HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VS7" /v "17.0" 2^>nul') do (
        if exist "%%B\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%%B\VC\Auxiliary\Build\vcvarsall.bat"
            set "CMAKE_GENERATOR=Visual Studio 17 2022"
            goto :found_compiler
        )
    )

    for /f "tokens=2*" %%A in ('reg query "HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VS7" /v "16.0" 2^>nul') do (
        if exist "%%B\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%%B\VC\Auxiliary\Build\vcvarsall.bat"
            set "CMAKE_GENERATOR=Visual Studio 16 2019"
            goto :found_compiler
        )
    )

    :: Compiler not found
    echo ERROR: No Visual Studio or Build Tools installation found!
    echo.
    echo Please install one of the following:
    echo   - Visual Studio 2022 (Community/Professional/Enterprise)
    echo   - Visual Studio 2019 (Community/Professional/Enterprise)
    echo   - Visual Studio Build Tools 2022
    echo   - Visual Studio Build Tools 2019
    echo.
    echo Make sure to include the "Desktop development with C++" workload.
    echo.
    echo Or use Ninja build system: build_win64.cmd %BUILD_TYPE% ninja
    exit /b 1
)

:: ============================================================================
:: Compiler found, start building
:: ============================================================================
:found_compiler
echo Found: %VCVARS_PATH%
echo Generator: %CMAKE_GENERATOR%
echo.

:: Configure build environment (always needed for C++ compilation, even with Ninja)
echo Configuring Visual Studio build environment...
call "%VCVARS_PATH%" %ARCH% >nul 2>&1

if errorlevel 1 (
    echo ERROR: Failed to configure build environment!
    exit /b 1
)

echo Visual Studio environment configured successfully.

:: ============================================================================
:: Create and enter build directory
:: ============================================================================
echo Creating build directory...
if not exist "%BUILD_PATH%" mkdir "%BUILD_PATH%"
cd /d "%BUILD_PATH%"

if errorlevel 1 (
    echo ERROR: Failed to create or enter build directory!
    exit /b 1
)

:: ============================================================================
:: Run CMake configuration
:: ============================================================================
echo.
echo Running CMake configuration...

if "%USE_NINJA%"=="true" (
    echo Using Ninja build system
    echo CMAKE command: cmake -G "Ninja" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" "%CMAKE_SOURCE_PATH%" -DCONFIG_BUILD_VERSION="%BUILD_VERSION%"
    echo.

    cmake -G "Ninja" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" "%CMAKE_SOURCE_PATH%" -DCONFIG_BUILD_VERSION="%BUILD_VERSION%"
) else (
    echo Using Visual Studio build system
    echo CMAKE command: cmake -G "%CMAKE_GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" "%CMAKE_SOURCE_PATH%" -DCONFIG_BUILD_VERSION="%BUILD_VERSION%"
    echo.

    cmake -G "%CMAKE_GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX="%INSTALL_PATH%" "%CMAKE_SOURCE_PATH%" -DCONFIG_BUILD_VERSION="%BUILD_VERSION%"
)

if errorlevel 1 (
    echo ERROR: CMake configuration failed!
    echo.
    echo Possible causes:
    echo   1. CMakeLists.txt not found in %CMAKE_SOURCE_PATH%
    echo   2. Missing dependencies
    echo   3. Invalid CMake configuration
    exit /b 1
)

:: ============================================================================
:: Execute build
:: ============================================================================
echo.
echo ========================================
echo Building project (%BUILD_TYPE% configuration)...
echo ========================================

if "%USE_NINJA%"=="true" (
    echo Using Ninja build system with %PARALLEL_JOBS% parallel jobs...
    ninja -j%PARALLEL_JOBS%
) else (
    echo Using MSBuild with %PARALLEL_JOBS% parallel jobs...
    cmake --build . --config %BUILD_TYPE% --parallel %PARALLEL_JOBS%
)

if errorlevel 1 (
    echo ERROR: Build failed!
    exit /b 1
)


:: ============================================================================
:: Execute installation
:: ============================================================================
echo.
echo ========================================
echo Installing to %INSTALL_PATH%...
echo ========================================

if "%USE_NINJA%"=="true" (
    ninja install
) else (
    cmake --build . --config %BUILD_TYPE% --target install
)

if errorlevel 1 (
    echo ERROR: Installation failed!
    exit /b 1
)

set "BUILD_END_TIME=!TIME!"
call :CalculateTimeDiff "!BUILD_START_TIME!" "!BUILD_END_TIME!"


:: ============================================================================
:: Build successful
:: ============================================================================
echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo Build started : !BUILD_START_TIME!
echo Build ended   : !BUILD_END_TIME!
if not "!TIME_DIFF!"=="00:00:00.00" (
    echo Build elapsed : !TIME_DIFF!
)
echo Build Version : %BUILD_VERSION%
echo Build Type    : %BUILD_TYPE%
echo Build System  : %BUILD_SYSTEM%
echo Generator     : %CMAKE_GENERATOR%
echo Build Dir     : %BUILD_PATH%
echo Install Dir   : %INSTALL_PATH%
echo.
echo Binaries and libraries have been installed to:
echo   %INSTALL_PATH%

exit /b 0

:: ============================================================================
:: Function to calculate time difference
:: ============================================================================
:CalculateTimeDiff
set "START_TIME=%~1"
set "END_TIME=%~2"

:: Remove quotes if present
set "START_TIME=!START_TIME:"=!"
set "END_TIME=!END_TIME:"=!"

:: Parse start time
for /f "tokens=1-4 delims=:." %%a in ("!START_TIME!") do (
    set "START_HOUR=%%a"
    set "START_MIN=%%b"
    set "START_SEC=%%c"
    set "START_USEC=%%d"
)

:: Parse end time
for /f "tokens=1-4 delims=:." %%a in ("!END_TIME!") do (
    set "END_HOUR=%%a"
    set "END_MIN=%%b"
    set "END_SEC=%%c"
    set "END_USEC=%%d"
)

if not defined START_USEC set "START_USEC=00"
if not defined END_USEC   set "END_USEC=00"

if "!START_HOUR:~0,1!" == "0" set "START_HOUR=!START_HOUR:~1!"
if "!START_MIN:~0,1!"  == "0" set "START_MIN=!START_MIN:~1!"
if "!START_SEC:~0,1!"  == "0" set "START_SEC=!START_SEC:~1!"
if "!START_USEC:~0,1!" == "0" set "START_USEC=!START_USEC:~1!"

if "!END_HOUR:~0,1!"  == "0"  set "END_HOUR=!END_HOUR:~1!"
if "!END_MIN:~0,1!"   == "0"  set "END_MIN=!END_MIN:~1!"
if "!END_SEC:~0,1!"   == "0"  set "END_SEC=!END_SEC:~1!"
if "!END_USEC:~0,1!  "== "0"  set "END_USEC=!END_USEC:~1!"

:: Convert to total microseconds
set /a "START_TOTAL_USECS=((!START_HOUR!*3600)+(!START_MIN!*60)+!START_SEC!)*100+!START_USEC!" 2>nul
if errorlevel 1 (
    set "TIME_DIFF=00:00:00.00"
    goto :eof
)

set /a "END_TOTAL_USECS=((!END_HOUR!*3600)+(!END_MIN!*60)+!END_SEC!)*100+!END_USEC!" 2>nul
if errorlevel 1 (
    set "TIME_DIFF=00:00:00.00"
    goto :eof
)

:: Calculate difference
set /a "DIFF_USECS=!END_TOTAL_USECS!-!START_TOTAL_USECS!" 2>nul
if errorlevel 1 (
    set "TIME_DIFF=00:00:00.00"
    goto :eof
)

:: Handle negative time (crossed midnight)
if !DIFF_USECS! LSS 0 (
    set /a "DIFF_USECS=!DIFF_USECS!+8640000" 2>nul
)

:: Convert back to HH:MM:SS.UU format
set /a "DIFF_SECS=!DIFF_USECS!/100" 2>nul
set /a "DIFF_USECS=!DIFF_USECS!%%100" 2>nul
set /a "DIFF_HOURS=!DIFF_SECS!/3600" 2>nul
set /a "DIFF_MINS=(!DIFF_SECS!%%3600)/60" 2>nul
set /a "DIFF_SECS=!DIFF_SECS!%%60" 2>nul

:: Format with leading zeros
if !DIFF_HOURS! LSS 10 set "DIFF_HOURS=0!DIFF_HOURS!"
if !DIFF_MINS! LSS 10 set "DIFF_MINS=0!DIFF_MINS!"
if !DIFF_SECS! LSS 10 set "DIFF_SECS=0!DIFF_SECS!"
if !DIFF_USECS! LSS 10 set "DIFF_USECS=0!DIFF_USECS!"

set "TIME_DIFF=!DIFF_HOURS!:!DIFF_MINS!:!DIFF_SECS!.!DIFF_USECS!"
goto :eof
