#pragma once
#include <memory>
#include <openssl/evp.h>
#include <string>
#include <vector>

/// Class used to encrypt and decrypt data using the Poly1305 algorithm.
/// OpenSSL doesn't directly expose the Poly1305 algorithm, so this class goes
///   through the chacha20-poly1305 algorithm to cause the Poly1305 methods to
///   be invoked.
class Poly1305
{
public:
	/// Initializes the class with the test app's key and IV.
	Poly1305();

	/// Decrypts the given ciphertext using the test app's key and IV.
	/// @param ciphertext The ciphertext to decrypt.
	/// @return The decrypted plaintext.
	std::string Decrypt(const std::vector<uint8_t>& ciphertext);

	/// Encrypts the given plaintext using the test app's key and IV.
	/// @param plaintext The plaintext to encrypt.
	/// @return The encrypted ciphertext.
	std::vector<uint8_t> Encrypt(const std::string& plaintext);

private:
	/// Prints any errors that occurred on the OpenSSL error stack then throws.
	/// @param message The message to print before exiting.
	static void HandleErrors(const std::string& message);

	/// Context object used for encryption.
	std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> m_encryptCtx;

	/// Context object used for decryption.
	std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> m_decryptCtx;
};
