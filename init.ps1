# Usage: ./init.ps1
# This script handles first time setup for this repository. This script must be
#   run after downloading or cloning the repository in order to compile this
#   MCVE app.
$ErrorActionPreference = "Stop"

# Check if Git is installed
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
	Write-Error "Error: Git is not installed."
	Write-Error "Please download and install Git from: https://gitforwindows.org/"
	exit 1
}

# Get the script's directory
$scriptDirectory = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Define the VCPKG directory
$vcpkgDirectory = Join-Path -Path $scriptDirectory -ChildPath "_vcpkg"

# Clone vcpkg repository
if (-not (Test-Path $vcpkgDirectory))
{
	Write-Host "Cloning vcpkg into '$vcpkgDirectory'..."
	git clone https://github.com/microsoft/vcpkg.git $vcpkgDirectory

	# Run bootstrap-vcpkg.bat
	$bootstrapScript = Join-Path -Path $vcpkgDirectory -ChildPath "bootstrap-vcpkg.bat"
	if (Test-Path $bootstrapScript) {
		Write-Host "Initializing vcpkg..."
		Invoke-Expression $bootstrapScript
	} else {
		Write-Error "Error: Failed to find 'bootstrap-vcpkg.bat' script in the cloned VCPKG directory."
		exit 1
	}
}
else
{
	Write-Host "Skipping vcpkg initialization since the folder already exists."
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

Write-Host "Successfully finished initializing the repository."
