#pragma once
#include <functional>
#include <string>
#include "Cipher.hpp"

/// Helper class that verifies a cipher's register usage.
class Validator
{
public:
	/// Initializes the object with the given cipher.
	/// @param makeCipher The function used to make cipher objects for testing.
	Validator(const std::function<Cipher()>& makeCipher);
	~Validator() = default;

	/// Runs all tests.
	/// @returns True if all tests passed, false otherwise.
	bool Test() const;

	/// Tests the cipher with a short string.
	/// @returns True if the test passed, false otherwise.
	bool TestShortString() const;

	/// Tests the cipher with a long string.
	/// @returns True if the test passed, false otherwise.
	bool TestLongString() const;

	/// Tests the cipher with a string that is exactly N bytes long.
	/// @param length The length of the string to test.
	/// @returns True if the test passed, false otherwise.
	bool TestNBytes(size_t length) const;

	/// Tests the cipher with the given string.
	/// @param plaintext The plaintext to encrypt and decrypt.
	/// @returns True if the test passed, false otherwise.
	bool TestString(const std::string& plaintext) const;

private:
	/// Signature used by each of the test functions run by `Test()`.
	using TestFunction = bool (Validator::*)() const;

	/// List of pointers to all test functions executed by `Test()`.
	static const std::vector<TestFunction> TEST_FUNCS;

	/// The function used to make cipher objects for testing.
	std::function<Cipher()> m_makeCipher;
};
