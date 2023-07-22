#include <iostream>
#include <functional>
#include <string>
#include "Poly1305.hpp"
#include "XmmRegisters.hpp"

/// Prints XMM registers before and after the given function is run.
void Run(
	const std::string& funcName,
	const std::function<void()>& func)
{
	XmmRegisters registers(true);
	std::cout << "Pre-" << funcName << " register state:\n";
	XmmRegisters::PrintAllRegisters();
	std::cout << "\n";

	func();

	std::cout << "Post-" << funcName << " register state:\n";
	XmmRegisters::PrintAllRegisters();
	std::cout << "\n";
}

void PrintResults(
	const std::string& plaintext,
	const std::vector<uint8_t>& ciphertext,
	const std::string& decryptedPlaintext)
{
	std::cout << "Plaintext: " << plaintext << "\n";
	std::cout << "Ciphertext: ";
	for (uint8_t byte : ciphertext)
	{
		std::cout << std::hex << static_cast<int>(byte);
	}
	std::cout << "\n";
	std::cout << "Decrypted plaintext: " << decryptedPlaintext << "\n\n";
}

/// Tests the Poly1305 class with a short string.
/// When encrypting data short enough that AVX-512 instructions are not used
///   by the poly1305 implementation, the XMM register bug should not occur.
void TestShortString()
{
	// Create the Poly1305 object
	Poly1305 poly1305;

	// Encrypt and decrypt a message
	const std::string plaintext = "Hello, world!";
	std::vector<uint8_t> ciphertext;
	std::string decryptedPlaintext;

	Run("Encrypt short string", [&]()
	{
		ciphertext = poly1305.Encrypt(plaintext);
	});
	Run("Decrypt short string", [&]()
	{
		decryptedPlaintext = poly1305.Decrypt(ciphertext);
	});

	// Print the results
	PrintResults(plaintext, ciphertext, decryptedPlaintext);
}

/// Tests the Poly1305 class with a long string.
/// When run on an AVX-512 capable CPU that is *not* a Skylake-X derived CPU,
///   the XMM register bug should occur.
void TestLongString()
{
	// Create the Poly1305 object
	Poly1305 poly1305;

	// Generate a string that is long enough to cause AVX-512 instructions to
	//   be used by the poly1305 implementation
	static constexpr size_t TARGET_LENGTH = 1024;
	std::string plaintext;
	plaintext.reserve(TARGET_LENGTH);
	for (size_t i = 0; i < TARGET_LENGTH; ++i)
	{
		plaintext += 'a' + (i % 26);
	}

	// Encrypt and decrypt the message
	std::vector<uint8_t> ciphertext;
	std::string decryptedPlaintext;

	Run("Encrypt long string", [&]()
	{
		ciphertext = poly1305.Encrypt(plaintext);
	});
	Run("Decrypt long string", [&]()
	{
		decryptedPlaintext = poly1305.Decrypt(ciphertext);
	});

	// Print the results
	PrintResults(plaintext, ciphertext, decryptedPlaintext);
}

int main()
{
	TestShortString();
	TestLongString();
	return 0;
}
