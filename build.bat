@echo off
REM Usage: build.bat [build_type]
REM Configures and builds the project.
REM @param build_type The build type to use. Defaults to debug. May be set to
REM   either "debug" or "release".
setlocal enabledelayedexpansion

REM Get the path to the script's directory
REM pushd and popd are used so that the script path doesn't end in a slash
pushd %~dp0
set REPO_ROOT=%CD%
popd

REM Set the build type
set BUILD_TYPE=%1
if "%BUILD_TYPE%" == "" (
	set BUILD_TYPE=debug
)

REM Add CMake to the path
set PATH=%REPO_ROOT%\_cmake\cmake\bin;%PATH%

REM Run the build
pushd %REPO_ROOT%
set RESULT=0

REM Handle the configure step
cmake --preset %BUILD_TYPE%
if ERRORLEVEL 1 (
	echo CMake configuration failed.
	set RESULT=1
	goto L_END
)

REM Handle the build step
cmake --build --preset %BUILD_TYPE% --target install
if ERRORLEVEL 1 (
	echo Build failed.
	set RESULT=1
	goto L_END
)

REM Handle cleanup
:L_END
popd
if "%RESULT%"=="0" (
	echo Build succeeded.
) else (
	echo Build failed.
)
exit /b %RESULT%
