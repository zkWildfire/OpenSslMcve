#include <iostream>
#include "Poly1305.hpp"
#include "XmmRegisters.hpp"

int main()
{
	// Create the Poly1305 object
	Poly1305 poly1305;

	// Encrypt and decrypt a message
	std::string plaintext = "Hello, world!";
	std::vector<uint8_t> ciphertext = poly1305.Encrypt(plaintext);
	std::string decryptedPlaintext = poly1305.Decrypt(ciphertext);

	// Print the results
	std::cout << "Plaintext: " << plaintext << "\n";
	std::cout << "Ciphertext: ";
	for (uint8_t byte : ciphertext)
	{
		std::cout << std::hex << static_cast<int>(byte);
	}
	std::cout << "\n";
	std::cout << "Decrypted plaintext: " << decryptedPlaintext << "\n";

	/*
	{
		XmmRegisters registers(true);
		registers.PrintSavedRegisters();

		std::cout << "Post-SSL register state:\n";
		XmmRegisters::PrintAllRegisters();
		std::cout << "\n";
	}
	*/
	return 0;
}
