#include "Validator.hpp"
#include "XmmRegisters.hpp"
#include <iostream>
#include <vector>

const std::vector<Validator::TestFunction> Validator::TEST_FUNCS
{
	&Validator::TestShortString,
	&Validator::TestLongString
};

Validator::Validator(const std::function<Cipher()>& makeCipher)
	: m_makeCipher(makeCipher)
{
}

std::string Validator::GetCipherName() const
{
	// To get the name of the cipher, a cipher object must be created
	auto cipher = m_makeCipher();
	return cipher.GetName();
}

bool Validator::Test() const
{
	// Create a cipher object so that its name can be queried
	const auto cipherName = GetCipherName();
	PrintTestSuiteHeader(cipherName);
	auto startTime = std::chrono::steady_clock::now();

	// Run the tests
	bool result = true;
	for (const auto& test : TEST_FUNCS)
	{
		result &= (this->*test)();
	}

	// Wrap up
	PrintTestSuiteFooter(
		cipherName,
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - startTime
		)
	);
	return result;
}

bool Validator::TestShortString() const
{
	return TestString(
		"ShortString",
		"Hello, world!"
	);
}

bool Validator::TestLongString() const
{
	return TestNBytes(1024);
}

bool Validator::TestNBytes(size_t length) const
{
	std::string plaintext;
	plaintext.reserve(length);
	for (size_t i = 0; i < length; ++i)
	{
		plaintext += 'a' + (i % 26);
	}

	return TestString(
		std::to_string(length) + "Bytes",
		plaintext
	);
}

bool Validator::TestString(
	const std::string& testName,
	const std::string& plaintext) const
{
	Cipher cipher = m_makeCipher();
	PrintTestHeader(cipher.GetName(), testName);
	auto startTime = std::chrono::steady_clock::now();
	XmmRegisters registers(true);

	// Encrypt the text and make sure all registers are in the expected state
	auto ciphertext = cipher.Encrypt(plaintext);
	if (!registers.Validate())
	{
		PrintFailText("encryption");
		return false;
	}

	// Decrypt the text and make sure all registers are in the expected state
	auto decryptedPlaintext = cipher.Decrypt(ciphertext);
	if (!registers.Validate())
	{
		PrintFailText("decryption");
		return false;
	}

	// Sanity check - make sure the decrypted text matches the original text
	if (plaintext != decryptedPlaintext)
	{
		PrintFailText("sanity");
		return false;
	}

	PrintSuccessText(
		cipher.GetName(),
		testName,
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - startTime
		)
	);
	return true;
}

void Validator::PrintTestSuiteHeader(const std::string& cipher)
{
	std::cout << "[----------] "
		<< TEST_FUNCS.size()
		<< " tests for "
		<< cipher << "\n";
}

void Validator::PrintTestHeader(
	const std::string& cipher,
	const std::string& testName)
{
	std::cout << "[ RUN      ] "
		<< cipher
		<< "."
		<< testName
		<< "\n";
}

void Validator::PrintFailText(const std::string& phase)
{
	std::cout << "[  FAILED  ] "
		<< "Failed in "
		<< phase
		<< " phase\n";
}

void Validator::PrintSuccessText(
	const std::string& cipher,
	const std::string& testName,
	std::chrono::milliseconds duration)
{
	std::cout << "[       OK ] "
		<< cipher
		<< "."
		<< testName
		<< " ("
		<< duration.count()
		<< " ms)\n";
}

void Validator::PrintTestSuiteFooter(
	const std::string& cipher,
	std::chrono::milliseconds duration)
{
	std::cout << "[----------] "
		<< TEST_FUNCS.size()
		<< " tests for "
		<< cipher
		<< " ("
		<< duration.count()
		<< " ms total)\n";
}
