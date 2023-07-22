@echo off
REM Usage: build.bat [build_type]
REM Configures and builds the project.
REM @param build_type The build type to use. Defaults to debug. May be set to
REM   either "debug" or "release".
setlocal enabledelayedexpansion

REM Set the build type
set build_type=%1
if "%build_type%" == "" set build_type=debug
