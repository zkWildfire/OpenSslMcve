#include "Poly1305.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <openssl/err.h>
#include <stdexcept>

/// 256-bit key used by the encrypt function
std::array<uint8_t, 32> key =
{
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
	0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31
};

/* A 128 bit IV */
std::array<uint8_t, 16> iv =
{
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
};

Poly1305::Poly1305()
	: m_encryptCtx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free)
	, m_decryptCtx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free)
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
		EVP_chacha20_poly1305(),
		nullptr,
		key.data(),
		iv.data()) != 1)
	{
		HandleErrors("Failed to initialize encryption");
	}
	if (EVP_DecryptInit_ex(
		m_decryptCtx.get(),
		EVP_chacha20_poly1305(),
		nullptr,
		key.data(),
		iv.data()) != 1)
	{
		HandleErrors("Failed to initialize decryption");
	}
}

std::string Poly1305::Decrypt(const std::vector<uint8_t>& ciphertext)
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

std::vector<uint8_t> Poly1305::Encrypt(const std::string& plaintext)
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

void Poly1305::HandleErrors(const std::string& message)
{
	std::cerr << message << "\n";
	ERR_print_errors_fp(stderr);

	throw std::runtime_error(message);
}
