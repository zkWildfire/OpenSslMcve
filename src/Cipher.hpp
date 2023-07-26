#pragma once
#include <functional>
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

	/// Function used to free cipher contexts.
	static constexpr auto CipherFree = &EVP_CIPHER_CTX_free;

	/// Gets all functions used to make cipher objects.
	/// @returns A vector of all cipher functions.
	static std::vector<std::function<Cipher()>> GetCipherFuncs();

	/// Initializes the class with the key and IV.
	/// @param cipherName The name of the cipher to use.
	/// @param cipherFunc The function used to get the cipher.
	/// @param key The key to use for encryption and decryption.
	/// @param iv The initialization vector to use for encryption and decryption.
	Cipher(
		const std::string& cipherName,
		CipherFunc cipherFunc,
		std::span<const uint8_t> key,
		std::span<const uint8_t> iv
	);
	Cipher(const Cipher&) = delete;
	Cipher(Cipher&&) = default;
	Cipher& operator=(const Cipher&) = delete;
	Cipher& operator=(Cipher&&) = default;
	~Cipher() = default;

	/// Decrypts the given ciphertext using the test app's key and IV.
	/// @param ciphertext The ciphertext to decrypt.
	/// @return The decrypted plaintext.
	std::string Decrypt(const std::vector<uint8_t>& ciphertext);

	/// Encrypts the given plaintext using the test app's key and IV.
	/// @param plaintext The plaintext to encrypt.
	/// @return The encrypted ciphertext.
	std::vector<uint8_t> Encrypt(const std::string& plaintext);

	/// Gets the name of the cipher used by this object.
	/// @returns The name of the cipher used by this object.
	const std::string& GetName() const;

private:
	/// Prints any errors that occurred on the OpenSSL error stack then throws.
	/// @param message The message to print before exiting.
	static void HandleErrors(const std::string& message);

	/// The name of the cipher to use.
	std::string m_cipherName;

	/// Context object used for encryption.
	std::unique_ptr<EVP_CIPHER_CTX, decltype(CipherFree)> m_encryptCtx;

	/// Context object used for decryption.
	std::unique_ptr<EVP_CIPHER_CTX, decltype(CipherFree)> m_decryptCtx;
};
