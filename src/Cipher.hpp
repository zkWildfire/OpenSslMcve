#pragma once
#include <memory>
#include <openssl/evp.h>
#include <span>
#include <string>
#include <vector>

/// Class used to encrypt and decrypt data using any of the OpenSSL ciphers.
class Cipher
{
public:
	/// Typedef for functions used to get ciphers.
	using CipherFunc = const EVP_CIPHER*(*)();

	/// Typedef for functions used to free cipher contexts.
	using CipherFree = void(*)(EVP_CIPHER_CTX*);

	/// Constructs all cipher objects with dedicated "make" functions.
	/// @returns A vector of all cipher objects.
	static std::vector<Cipher> MakeAllCiphers();

	/// Constructs a new cipher object that uses the chacha20_poly1305 cipher.
	/// @returns The new cipher object.
	static Cipher MakeChacha20Poly1305();

	/// Initializes the class with the key and IV.
	/// @param cipherFunc The function used to get the cipher.
	/// @param cipherFree The function used to free the cipher context.
	/// @param key The key to use for encryption and decryption.
	/// @param iv The initialization vector to use for encryption and decryption.
	Cipher(
		CipherFunc cipherFunc,
		CipherFree cipherFree,
		std::span<const uint8_t> key,
		std::span<const uint8_t> iv
	);

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
	std::unique_ptr<EVP_CIPHER_CTX, CipherFree> m_encryptCtx;

	/// Context object used for decryption.
	std::unique_ptr<EVP_CIPHER_CTX, CipherFree> m_decryptCtx;
};
