# Usage: ./init.ps1
# This script handles first time setup for this repository. This script must be
#   run after downloading or cloning the repository in order to compile this
#   MCVE app.
$ErrorActionPreference = "Stop"

# Check if Git is installed
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
	Write-Host "Error: Git is not installed."
	Write-Host "Please download and install Git from: https://gitforwindows.org/"
	exit 1
}

# Check if Python 3 is installed and available on the PATH
if (-not (Get-Command python -ErrorAction SilentlyContinue)) {
	Write-Host "Error: Python 3 is not installed."
	Write-Host "Please download and install Python 3 from: https://www.python.org/downloads/"
	exit 1
}

# Get the script's directory
$scriptDirectory = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Make sure the vcpkg repository is present
$vcpkgDirectory = Join-Path -Path $scriptDirectory -ChildPath "vcpkg"
git submodule update --init

# Run bootstrap-vcpkg.bat
$bootstrapScript = Join-Path -Path $vcpkgDirectory -ChildPath "bootstrap-vcpkg.bat"
if (Test-Path $bootstrapScript) {
	Write-Host "Initializing vcpkg..."
	Invoke-Expression $bootstrapScript
} else {
	Write-Error "Error: Failed to find 'bootstrap-vcpkg.bat' script in the cloned VCPKG directory."
	exit 1
}

# Handle CMake setup
$cmakeDirectory = Join-Path -Path $scriptDirectory -ChildPath "_cmake"
$cmakeZipFolder = "cmake-3.27.0-windows-x86_64"
$cmakeUrl = "https://github.com/Kitware/CMake/releases/download/v3.27.0/$cmakeZipFolder.zip"
$cmakeZip = Join-Path -Path $cmakeDirectory -ChildPath "cmake.zip"
$cmakeExe = Join-Path -Path $cmakeDirectory -ChildPath "cmake/bin/cmake.exe"

# Download CMake if it doesn't exist
if (-not (Test-Path $cmakeExe))
{
	Write-Host "Downloading CMake..."
	New-Item -ItemType Directory -Path $cmakeDirectory | Out-Null
	Invoke-WebRequest -Uri $cmakeUrl -OutFile $cmakeZip

	Write-Host "Extracting CMake..."
	Expand-Archive -Path $cmakeZip -DestinationPath $cmakeDirectory

	# Rename the folder so that the version number is removed
	Rename-Item -Path "$cmakeDirectory/$cmakeZipFolder" -NewName "cmake"
}
else
{
	Write-Host "Skipping CMake download since the folder already exists."
}

# Set up nasm
$nasmDirectory = Join-Path -Path $scriptDirectory -ChildPath "_nasm"
$nasmZipFolder = "nasm-2.16.01"
$nasmZipFile = Join-Path -Path $nasmDirectory -ChildPath "nasm.zip"
$nasmUrl = "https://www.nasm.us/pub/nasm/releasebuilds/2.16.01/win64/$nasmZipFolder-win64.zip"
$nasmExe = Join-Path -Path $nasmDirectory -ChildPath "nasm/nasm.exe"

# Download nasm if it doesn't exist
if (-not (Test-Path $nasmExe))
{
	Write-Host "Downloading nasm..."
	New-Item -ItemType Directory -Path $nasmDirectory | Out-Null
	Invoke-WebRequest -Uri $nasmUrl -OutFile $nasmZipFile

	Write-Host "Extracting nasm..."
	Expand-Archive -Path $nasmZipFile -DestinationPath $nasmDirectory

	# Rename the folder so that the version number is removed
	Rename-Item -Path "$nasmDirectory/$nasmZipFolder" -NewName "nasm"
}
else
{
	Write-Host "Skipping nasm download since the folder already exists."
}

# Download ninja if it doesn't exist
$ninjaDirectory = Join-Path -Path $scriptDirectory -ChildPath "_ninja"
$ninjaUrl = "https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip"
$ninjaZip = Join-Path -Path $ninjaDirectory -ChildPath "ninja.zip"
$ninjaExe = Join-Path -Path $ninjaDirectory -ChildPath "ninja.exe"

if (-not (Test-Path $ninjaExe))
{
	Write-Host "Downloading ninja..."
	New-Item -ItemType Directory -Path $ninjaDirectory | Out-Null
	Invoke-WebRequest -Uri $ninjaUrl -OutFile $ninjaZip

	Write-Host "Extracting ninja..."
	Expand-Archive -Path $ninjaZip -DestinationPath $ninjaDirectory
}
else
{
	Write-Host "Skipping ninja download since the folder already exists."
}

Write-Host "Successfully finished initializing the repository."
