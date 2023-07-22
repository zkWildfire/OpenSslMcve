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

# Get the script's directory
$scriptDirectory = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Define the VCPKG directory
$vcpkgDirectory = Join-Path -Path $scriptDirectory -ChildPath "_vcpkg"

# Clone vcpkg repository
if (-not (Test-Path $vcpkgDirectory))
{
	Write-Host "Cloning vcpkg into '$vcpkgDirectory'..."
	git clone git@github.com:microsoft/vcpkg.git $vcpkgDirectory
}
else
{
	Write-Host "Skipping vcpkg clone (directory already exists)."
}

# Run bootstrap-vcpkg.bat
$bootstrapScript = Join-Path -Path $vcpkgDirectory -ChildPath "bootstrap-vcpkg.bat"
if (Test-Path $bootstrapScript) {
	Write-Host "Initializing vcpkg..."
	Invoke-Expression $bootstrapScript
} else {
	Write-Host "Error: Failed to find 'bootstrap-vcpkg.bat' script in the cloned VCPKG directory."
	exit 1
}

Write-Host "Successfully finished initializing the repository."