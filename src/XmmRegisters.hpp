#pragma once
#include <cstdint>
#include <memory>

/// Helper class used to manage XMM registers.
/// This class provides many different static helper methods for manipulating
///   XMM registers. It can also be instantiated to capture a snapshot of the
///   current state of all XMM registers so that the state can be restored later.
class XmmRegisters
{
public:
	/// Total number of XMM registers.
	static constexpr int32_t XMM_REGISTER_COUNT = 16;

	/// Number of floats per XMM register.
	static constexpr int32_t FLOATS_PER_XMM_REGISTER = 4;

	/// Captures a snapshot of the XMM registers.
	/// @param populate If true, the XMM registers will be populated with
	///   non-zero values after being captured. This allows the registers'
	///   values to be inspected later to ensure that they were restored
	///   correctly.
	XmmRegisters();

	/// Captures a snapshot of the XMM registers, then sets them to constants.
	/// This constructor is used to force non-zero values to be present in the
	///   various XMM registers. This allows the registers' values to be
	///   inspected later to ensure that they have been restored correctly by
	///   any called method.
	/// @param populate Dummy parameter used to select this constructor overload.
	XmmRegisters(bool populate);
	XmmRegisters(const XmmRegisters&) = delete;
	XmmRegisters(XmmRegisters&&) = delete;
	XmmRegisters& operator=(const XmmRegisters&) = delete;
	XmmRegisters& operator=(XmmRegisters&&) = delete;

	/// Restores the XMM registers to the state they were in.
	~XmmRegisters();

	/// Reads the specified XMM register.
	/// @param registerIndex The index of the XMM register to read from.
	///   Must be in the range `[0, XMM_REGISTER_COUNT)`.
	/// @param data The data read from the XMM register will be written to this
	///   array. Must be a pointer to an array of at least
	///   `FLOATS_PER_XMM_REGISTER` floats.
	static void ReadRegister(int32_t registerIndex, float* data);

	/// Writes the specified data to the specified XMM register.
	/// @param registerIndex The index of the XMM register to write to. Must
	///   be in the range `[0, XMM_REGISTER_COUNT)`.
	/// @param data The data to write to the XMM register. Must be a pointer to
	///   an array of at least `FLOATS_PER_XMM_REGISTER` floats.
	static void WriteRegister(int32_t registerIndex, const float* data);

	/// Prints the current value of the register.
	/// @param registerIndex The index of the register to print. Must
	///   be in the range `[0, XMM_REGISTER_COUNT)`.
	static void PrintRegister(int32_t registerIndex);

	/// Prints the current value of the register.
	/// @param registerIndex The index of the register to print.
	/// @param values The values to print for the register.
	static void PrintRegister(int32_t registerIndex, const float* values);

	/// Prints the current value of all registers.
	static void PrintAllRegisters();

	/// Prints the saved values of all registers.
	void PrintSavedRegisters() const;

private:
	/// Array containing the state of all XMM registers.
	/// This is stored as a plain C style array of floats rather than an
	///   `std::array` to avoid making any method calls when storing the
	///   register state. This is important because otherwise, MSVC may use
	///   the XMM registers as part of the code that stores the register state,
	///   which partially defeats the purpose of storing the registers in the
	///   first place.
	float m_state[XMM_REGISTER_COUNT][FLOATS_PER_XMM_REGISTER];
};
