#include "XmmRegisters.hpp"
#include "xmm.hpp"
#include <iostream>
#include <ios>

namespace
{
	/// Type of the getter function declared in .asm files.
	typedef void(*GetRegister)(float*);

	/// Type of the setter function declared in .asm files.
	typedef void(*SetRegister)(const float*);

	/// Array of pointers to each getter function for XMM registers.
	/// @note This array is indexed by the register index.
	const GetRegister GET_REGISTER_FUNCTIONS[XmmRegisters::XMM_REGISTER_COUNT] =
	{
		&getXmmRegister0,
		&getXmmRegister1,
		&getXmmRegister2,
		&getXmmRegister3,
		&getXmmRegister4,
		&getXmmRegister5,
		&getXmmRegister6,
		&getXmmRegister7,
		&getXmmRegister8,
		&getXmmRegister9,
		&getXmmRegister10,
		&getXmmRegister11,
		&getXmmRegister12,
		&getXmmRegister13,
		&getXmmRegister14,
		&getXmmRegister15
	};

	/// Array of pointers to each setter function for XMM registers.
	/// @note This array is indexed by the register index.
	const SetRegister SET_REGISTER_FUNCTIONS[XmmRegisters::XMM_REGISTER_COUNT] =
	{
		&setXmmRegister0,
		&setXmmRegister1,
		&setXmmRegister2,
		&setXmmRegister3,
		&setXmmRegister4,
		&setXmmRegister5,
		&setXmmRegister6,
		&setXmmRegister7,
		&setXmmRegister8,
		&setXmmRegister9,
		&setXmmRegister10,
		&setXmmRegister11,
		&setXmmRegister12,
		&setXmmRegister13,
		&setXmmRegister14,
		&setXmmRegister15
	};
}

void XmmRegisters::ReadRegister(int32_t registerIndex, float* data)
{
	GET_REGISTER_FUNCTIONS[registerIndex](data);
}

void XmmRegisters::WriteRegister(int32_t registerIndex, const float* data)
{
	SET_REGISTER_FUNCTIONS[registerIndex](data);
}

void XmmRegisters::PrintRegister(int32_t registerIndex)
{
	float values[FLOATS_PER_XMM_REGISTER];
	ReadRegister(registerIndex, values);
	PrintRegister(registerIndex, values);
}

void XmmRegisters::PrintRegister(int32_t registerIndex, const float* values)
{
	std::cout << "XMM" << std::dec << registerIndex << ": ";
	for (int32_t i = 0; i < FLOATS_PER_XMM_REGISTER; ++i)
	{
		std::cout << values[i] << " ";
	}
	std::cout << "\n";
}

void XmmRegisters::PrintAllRegisters()
{
	for (int32_t i = 0; i < XMM_REGISTER_COUNT; ++i)
	{
		PrintRegister(i);
	}
}

XmmRegisters::XmmRegisters()
{
	// Save the current state of all registers
	for (int32_t i = 0; i < XMM_REGISTER_COUNT; ++i)
	{
		ReadRegister(i, m_state[i]);
	}
}

XmmRegisters::XmmRegisters(bool)
	: XmmRegisters()
{
	// Calculate all values to write to the registers before writing to any
	//   register. This is done to (hopefully) avoid executing any
	//   compiler-generated instructions that use the XMM registers between
	//   calls to the setter method that was declared in the .asm file.
	float values[XMM_REGISTER_COUNT][FLOATS_PER_XMM_REGISTER];
	for (int32_t i = 0; i < XMM_REGISTER_COUNT; ++i)
	{
		for (int32_t j = 0; j < FLOATS_PER_XMM_REGISTER; ++j)
		{
			values[i][j] = static_cast<float>(i * FLOATS_PER_XMM_REGISTER + j);
		}
	}

	// Set each register to the calculated values
	for (int32_t i = 0; i < XMM_REGISTER_COUNT; ++i)
	{
		WriteRegister(i, values[i]);
	}
}

XmmRegisters::~XmmRegisters()
{
	// Restore the state of all registers
	for (int32_t i = 0; i < XMM_REGISTER_COUNT; ++i)
	{
		WriteRegister(i, m_state[i]);
	}
}

void XmmRegisters::PrintSavedRegisters() const
{
	std::cout << "Saved registers:\n";
	for (int32_t i = 0; i < XMM_REGISTER_COUNT; ++i)
	{
		PrintRegister(i, m_state[i]);
	}
	std::cout << "\n";
}
