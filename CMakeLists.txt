cmake_minimum_required(VERSION 3.23)
project(MCVE LANGUAGES CXX ASM_NASM)
find_package(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a custom command for generating the assembly files
set(GENERATED_ASM_FILES
	"Xmm.hpp"
	"xmm0.asm"
	"xmm1.asm"
	"xmm2.asm"
	"xmm3.asm"
	"xmm4.asm"
	"xmm5.asm"
	"xmm6.asm"
	"xmm7.asm"
	"xmm8.asm"
	"xmm9.asm"
	"xmm10.asm"
	"xmm11.asm"
	"xmm12.asm"
	"xmm13.asm"
	"xmm14.asm"
	"xmm15.asm"
)
list(TRANSFORM GENERATED_ASM_FILES PREPEND "${CMAKE_CURRENT_BINARY_DIR}/asm/")

add_custom_command(
	OUTPUT ${GENERATED_ASM_FILES}
	COMMAND python generate_asm.py --output "${CMAKE_CURRENT_BINARY_DIR}/asm"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	MAIN_DEPENDENCY generate_asm.py
)

# Set up the generated assembly files as a library
add_library(GENERATED_ASM OBJECT ${GENERATED_ASM_FILES})
target_include_directories(GENERATED_ASM
	PUBLIC
		"${CMAKE_CURRENT_BINARY_DIR}/asm"
)

# Create a library that each executable links to
add_library(Common STATIC
	src/Common/IVs.hpp
	src/Common/Keys.hpp
	src/Common/Cipher.hpp
	src/Common/Cipher.cpp
	src/Common/Validator.hpp
	src/Common/Validator.cpp
	src/Common/XmmRegisters.hpp
	src/Common/XmmRegisters.cpp
)
target_include_directories(Common
	INTERFACE
		src/Common
)
target_link_libraries(Common
	PUBLIC
		GENERATED_ASM
		OpenSSL::SSL
		OpenSSL::Crypto
)

# Set up the MCVE executable
# This is the executable that demonstrates the bug by printing XMM registers'
#   state before and after the call to OpenSSL code
add_executable(Mcve
	src/Mcve/Mcve.cpp
)
target_link_libraries(Mcve
	PRIVATE
		Common
)

# Set up the test executable
# This executable uses the XMM register validation code to test multiple
#   different OpenSSL algorithms to see if any other algorithms have the same
#   bug
add_executable(Tests
	src/Tests/Tests.cpp
)
target_link_libraries(Tests
	PRIVATE
		Common
)

# Set up the example executable for demonstrating what sort of unexpected
#   behavior may occur as a result of the OpenSSL bug
add_executable(ExampleWaitFor
	src/Examples/WaitFor.cpp
)
target_link_libraries(ExampleWaitFor
	PRIVATE
		Common
)

install(
	TARGETS
		Mcve
		Tests
		ExampleWaitFor
	DESTINATION
		"${CMAKE_INSTALL_PREFIX}"
)

# Copy the OpenSSL DLLs to the install directory
# This is only needed when using the `x64-windows` triplet. If testing using the
#   `x64-windows-static` triplet, OpenSSL is statically linked and no DLLs are
#   needed.
if (VCPKG_HOST_TRIPLET STREQUAL "x64-windows")
	install(
		FILES
			"${CMAKE_CURRENT_BINARY_DIR}/libcrypto-3-x64.dll"
		DESTINATION
			"${CMAKE_INSTALL_PREFIX}"
	)
endif()
