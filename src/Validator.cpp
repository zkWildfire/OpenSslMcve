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

bool Validator::Test() const
{
	for (const auto& test : TEST_FUNCS)
	{
		if (!(this->*test)())
		{
			return false;
		}
	}

	return true;
}

bool Validator::TestShortString() const
{
	return TestString("Hello, world!");
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

	return TestString(plaintext);
}

bool Validator::TestString(const std::string& plaintext) const
{
	Cipher cipher = m_makeCipher();
	std::cout << "Running test for cipher " << cipher.GetName()
		<< " using string of length " << plaintext.size() << ".\n";
	XmmRegisters registers(true);

	// Encrypt the text and make sure all registers are in the expected state
	auto ciphertext = cipher.Encrypt(plaintext);
	if (!registers.Validate())
	{
		std::cerr << "Cipher " << cipher.GetName() << " did not restore "
			<< "registers to expected state after encryption.\n";
		return false;
	}

	// Decrypt the text and make sure all registers are in the expected state
	auto decryptedPlaintext = cipher.Decrypt(ciphertext);
	if (!registers.Validate())
	{
		std::cerr << "Cipher " << cipher.GetName() << " did not restore "
			<< "registers to expected state after decryption.\n";
		return false;
	}

	// Sanity check - make sure the decrypted text matches the original text
	if (plaintext != decryptedPlaintext)
	{
		std::cerr << "Cipher " << cipher.GetName() << " did not decrypt "
			<< "correctly.\n";
		return false;
	}

	std::cout << "Test passed for cipher " << cipher.GetName() << ".\n";
	return true;
}
