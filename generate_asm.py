#!/usr/bin/env python3
# Helper script used to generate the assembly files used to read and write
#   to the XMM registers.
import argparse
from pathlib import Path

# Template for the assembly files to generate
# To use this template, use `.format()` and pass in the index of the register
#   to read/write to.
ASM_TEMPLATE="""
section .text

global getXmmRegister{0}
global setXmmRegister{0}

; void getXmmRegister{0}(float* values);
; Reads the values from the XMM register and stores them in the array.
; @param values The array to store the values in. Must contain at least 4
;   elements.
getXmmRegister{0}:
	movups [rcx], xmm{0}
	ret

; void setXmmRegister{0}(const float* values);
; Sets the values of the XMM register to the values in the array.
; @param values The array containing the values to set. Must contain at least
;   4 elements.
setXmmRegister{0}:
	movups xmm{0}, [rcx]
	ret
"""

# Set up the argument parser
parser = argparse.ArgumentParser(
	description="Generates .asm files for reading and writing to XMM registers."
)
parser.add_argument(
	"--output",
	"-o",
	type=str,
	help="The output directory to write the .asm files to."
)

# Parse the arguments
args = parser.parse_args()
output_path = Path(args.output)
output_path.mkdir(parents=True, exist_ok=True)

# Generate the assembly files
XMM_REGISTER_COUNT = 16
for i in range(XMM_REGISTER_COUNT):
	# Generate the assembly file contents
	asm = ASM_TEMPLATE.format(i)

	# Generate the assembly file
	output_file_name = f"xmm{i}.asm"
	output_file_path = Path(output_path) / output_file_name
	output_file_path.write_text(asm)

# Create a header file that declares each of the functions
header_file_contents = """
#pragma once
extern "C" {
"""
for i in range(XMM_REGISTER_COUNT):
	header_file_contents += f"\tvoid getXmmRegister{i}(float* values);\n"
	header_file_contents += f"\tvoid setXmmRegister{i}(const float* values);\n\n"

header_file_contents += "}\n"

# Generate the header file
header_file = Path(output_path) / "xmm.hpp"
header_file.write_text(header_file_contents)
