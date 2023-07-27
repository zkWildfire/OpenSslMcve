#pragma once
#include <chrono>
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

	/// Gets the name of the cipher that the validator will test.
	/// @returns The name of the cipher.
	std::string GetCipherName() const;

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
	/// @param testName The name to print for the test.
	/// @param plaintext The plaintext to encrypt and decrypt.
	/// @returns True if the test passed, false otherwise.
	bool TestString(
		const std::string& testName,
		const std::string& plaintext) const;

private:
	/// Signature used by each of the test functions run by `Test()`.
	using TestFunction = bool (Validator::*)() const;

	/// List of pointers to all test functions executed by `Test()`.
	static const std::vector<TestFunction> TEST_FUNCS;

	/// Prints the header for a "test suite".
	/// This is used whenever `Test()` is called.
	/// @param cipher The name of the cipher being tested.
	static void PrintTestSuiteHeader(const std::string& cipher);

	/// Prints the header for a test.
	/// This is used whenever a cipher is tested on a specific string.
	/// @param cipher The name of the cipher being tested.
	/// @param testName The name of the test being run.
	static void PrintTestHeader(
		const std::string& cipher,
		const std::string& testName
	);

	/// Prints a failure line for a test.
	/// @param phase The phase of the test that failed.
	static void PrintFailText(const std::string& phase);

	/// Prints a success line for a test.
	/// @param cipher The name of the cipher being tested.
	/// @param testName The name of the test being run.
	/// @param duration How long the test took to run.
	static void PrintSuccessText(
		const std::string& cipher,
		const std::string& testName,
		std::chrono::milliseconds duration
	);

	/// Prints the footer for a "test suite".
	/// @param cipher The name of the cipher being tested.
	/// @param duration How long the test suite took to run.
	static void PrintTestSuiteFooter(
		const std::string& cipher,
		std::chrono::milliseconds duration
	);

	/// The function used to make cipher objects for testing.
	std::function<Cipher()> m_makeCipher;
};
