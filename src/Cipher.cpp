#include "Cipher.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <stdexcept>
#include "IVs.hpp"
#include "Keys.hpp"

std::vector<std::function<Cipher()>> Cipher::GetCipherFuncs()
{
	return std::vector<std::function<Cipher()>>
	{
		MakeChacha20Poly1305
	};
}

std::vector<Cipher> Cipher::MakeAllCiphers()
{
	std::vector<Cipher> ciphers;
	for (const auto& cipherFunc : GetCipherFuncs())
	{
		ciphers.push_back(cipherFunc());
	}
	return ciphers;
}

Cipher Cipher::MakeChacha20Poly1305()
{
	return Cipher(
		"ChaCha20-Poly1305",
		EVP_chacha20_poly1305,
		EVP_CIPHER_CTX_free,
		std::span(KEY_256),
		std::span(IV_128)
	);
}

Cipher::Cipher(
	const std::string& cipherName,
	CipherFunc cipherFunc,
	CipherFree cipherFree,
	std::span<const uint8_t> key,
	std::span<const uint8_t> iv)
	: m_cipherName(cipherName)
	, m_encryptCtx(EVP_CIPHER_CTX_new(), cipherFree)
	, m_decryptCtx(EVP_CIPHER_CTX_new(), cipherFree)
{
	// Validate context objects
	if (!m_encryptCtx)
	{
		HandleErrors("Failed to create encryption context");
	}
	if (!m_decryptCtx)
	{
		HandleErrors("Failed to create decryption context");
	}

	// Handle remaining initialization
	if (EVP_EncryptInit_ex(
		m_encryptCtx.get(),
		cipherFunc(),
		nullptr,
		key.data(),
		iv.data()) != 1)
	{
		HandleErrors("Failed to initialize encryption");
	}
	if (EVP_DecryptInit_ex(
		m_decryptCtx.get(),
		cipherFunc(),
		nullptr,
		key.data(),
		iv.data()) != 1)
	{
		HandleErrors("Failed to initialize decryption");
	}
}

std::string Cipher::Decrypt(const std::vector<uint8_t>& ciphertext)
{
	// Allocate space for the plaintext
	std::vector<uint8_t> plaintext(
		ciphertext.size() + EVP_CIPHER_CTX_block_size(m_decryptCtx.get())
	);

	// Decrypt the ciphertext
	int len = 0;
	if (EVP_DecryptUpdate(
		m_decryptCtx.get(),
		plaintext.data(),
		&len,
		ciphertext.data(),
		ciphertext.size()) != 1)
	{
		HandleErrors("Failed to decrypt ciphertext");
	}

	// Finalize the decryption
	int plaintextLen = len;
	if (EVP_DecryptFinal_ex(
		m_decryptCtx.get(),
		plaintext.data() + len,
		&len) != 1)
	{
		HandleErrors("Failed to finalize decryption");
	}

	// Update the plaintext length
	plaintextLen += len;
	plaintext.resize(plaintextLen);

	return std::string(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
}

std::vector<uint8_t> Cipher::Encrypt(const std::string& plaintext)
{
	// Allocate space for the ciphertext
	std::vector<uint8_t> ciphertext(
		plaintext.size() + EVP_CIPHER_CTX_block_size(m_encryptCtx.get())
	);

	// Encrypt the plaintext
	int len = 0;
	if (EVP_EncryptUpdate(
		m_encryptCtx.get(),
		ciphertext.data(),
		&len,
		reinterpret_cast<const uint8_t*>(plaintext.data()),
		plaintext.size()) != 1)
	{
		HandleErrors("Failed to encrypt plaintext");
	}

	// Finalize the encryption
	int ciphertextLen = len;
	if (EVP_EncryptFinal_ex(
		m_encryptCtx.get(),
		ciphertext.data() + len,
		&len) != 1)
	{
		HandleErrors("Failed to finalize encryption");
	}

	// Update the ciphertext length
	ciphertextLen += len;
	ciphertext.resize(ciphertextLen);

	return ciphertext;
}

const std::string& Cipher::GetName() const
{
	return m_cipherName;
}

void Cipher::HandleErrors(const std::string& message)
{
	std::cerr << message << "\n";
	ERR_print_errors_fp(stderr);

	throw std::runtime_error(message);
}
