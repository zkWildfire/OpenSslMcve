cmake_minimum_required(VERSION 3.23)
project(MCVE LANGUAGES CXX ASM_NASM)
find_package(OpenSSL REQUIRED)

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
add_library(GENERATED_ASM STATIC ${GENERATED_ASM_FILES})
target_include_directories(GENERATED_ASM
	PUBLIC
		"${CMAKE_CURRENT_BINARY_DIR}/asm"
)

# Set up the MCVE executable
add_executable(MCVE source.cpp)
target_link_libraries(MCVE
	PRIVATE
		GENERATED_ASM
		OpenSSL::SSL
		OpenSSL::Crypto
)
install(TARGETS MCVE)
