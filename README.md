# OpenSSL XMM Register Bug MCVE
## Overview
This project implements a minimum complete verifiable example used to reproduce
a bug in OpenSSL's assembly code for the x86_64 architecture. The bug is
triggered by running poly1305 methods on a CPU chip that supports AVX-512
instructions and is **not** a Skylake-derived architecture. When this bug
occurs, it results in the XMM registers in the range XMM6-XMM15 getting cleared
and not restored. This can then cause miscellaneous other errors and unexpected
behavior if the compiler has generated other code that expects those registers
to have been restored correctly.

## Prerequisites
* Visual Studio or the Visual Studio build tools
  * This MCVE was tested with Visual Studio 2022. It's likely that any recent
    version of Visual Studio will work, but the exact minimum version is not
    known.
* Python 3
  * This MCVE was tested using Python 3.11. The exact minimum required version
    is not known, but it must be later than v3.4.

## Setup
This bug only occurs on Windows. To run the MCVE, you will need to have Visual
Studio installed and locatable by vswhere.exe, or you will need to have the
Visual Studio build tools installed and run commands in a terminal in which
the `vsdevcmd.bat` script has been run. To handle first time setup, run the
`init.ps1` script (which may require you to configure your powershell execution
policy). Once the script has completed, run `build.bat` to build the project.
