#include "Cipher.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <stdexcept>
#include "IVs.hpp"
#include "Keys.hpp"

#define MAKE_CIPHER_FUNC(cipherName, cipherFunc, key, iv) \
	[]() \
	{ \
		return Cipher( \
			cipherName, \
			cipherFunc, \
			key.data(), \
			iv.data() \
		); \
	}

std::vector<std::function<Cipher()>> Cipher::GetCipherFuncs()
{
	return std::vector<std::function<Cipher()>>
	{
		MAKE_CIPHER_FUNC("EVP_aes_128_ecb", EVP_aes_128_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_cbc", EVP_aes_128_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_cfb1", EVP_aes_128_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_cfb8", EVP_aes_128_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_cfb128", EVP_aes_128_cfb128, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_ofb", EVP_aes_128_ofb, KEY_256, IV_128),
		/*
		MAKE_CIPHER_FUNC("EVP_aes_128_ctr", EVP_aes_128_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_ccm", EVP_aes_128_ccm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_gcm", EVP_aes_128_gcm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_xts", EVP_aes_128_xts, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_wrap", EVP_aes_128_wrap, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_wrap_pad", EVP_aes_128_wrap_pad, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_ocb", EVP_aes_128_ocb, KEY_256, IV_128),
		*/
		MAKE_CIPHER_FUNC("EVP_aes_192_ecb", EVP_aes_192_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_cbc", EVP_aes_192_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_cfb1", EVP_aes_192_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_cfb8", EVP_aes_192_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_cfb128", EVP_aes_192_cfb128, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_ofb", EVP_aes_192_ofb, KEY_256, IV_128),
		/*
		MAKE_CIPHER_FUNC("EVP_aes_192_ctr", EVP_aes_192_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_ccm", EVP_aes_192_ccm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_gcm", EVP_aes_192_gcm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_wrap", EVP_aes_192_wrap, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_wrap_pad", EVP_aes_192_wrap_pad, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_192_ocb", EVP_aes_192_ocb, KEY_256, IV_128),
		*/
		MAKE_CIPHER_FUNC("EVP_aes_256_ecb", EVP_aes_256_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_cbc", EVP_aes_256_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_cfb1", EVP_aes_256_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_cfb8", EVP_aes_256_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_cfb128", EVP_aes_256_cfb128, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_ofb", EVP_aes_256_ofb, KEY_256, IV_128),
		/*
		MAKE_CIPHER_FUNC("EVP_aes_256_ctr", EVP_aes_256_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_ccm", EVP_aes_256_ccm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_gcm", EVP_aes_256_gcm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_xts", EVP_aes_256_xts, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_wrap", EVP_aes_256_wrap, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_wrap_pad", EVP_aes_256_wrap_pad, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_ocb", EVP_aes_256_ocb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_cbc_hmac_sha1", EVP_aes_128_cbc_hmac_sha1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_cbc_hmac_sha1", EVP_aes_256_cbc_hmac_sha1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_128_cbc_hmac_sha256", EVP_aes_128_cbc_hmac_sha256, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aes_256_cbc_hmac_sha256", EVP_aes_256_cbc_hmac_sha256, KEY_256, IV_128),
		*/
		MAKE_CIPHER_FUNC("EVP_aria_128_ecb", EVP_aria_128_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_128_cbc", EVP_aria_128_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_128_cfb1", EVP_aria_128_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_128_cfb8", EVP_aria_128_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_128_cfb128", EVP_aria_128_cfb128, KEY_256, IV_128),
		/*
		MAKE_CIPHER_FUNC("EVP_aria_128_ctr", EVP_aria_128_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_128_ofb", EVP_aria_128_ofb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_128_gcm", EVP_aria_128_gcm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_128_ccm", EVP_aria_128_ccm, KEY_256, IV_128),
		*/
		MAKE_CIPHER_FUNC("EVP_aria_192_ecb", EVP_aria_192_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_192_cbc", EVP_aria_192_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_192_cfb1", EVP_aria_192_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_192_cfb8", EVP_aria_192_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_192_cfb128", EVP_aria_192_cfb128, KEY_256, IV_128),
		/*
		MAKE_CIPHER_FUNC("EVP_aria_192_ctr", EVP_aria_192_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_192_ofb", EVP_aria_192_ofb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_192_gcm", EVP_aria_192_gcm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_192_ccm", EVP_aria_192_ccm, KEY_256, IV_128),
		*/
		MAKE_CIPHER_FUNC("EVP_aria_256_ecb", EVP_aria_256_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_256_cbc", EVP_aria_256_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_256_cfb1", EVP_aria_256_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_256_cfb8", EVP_aria_256_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_256_cfb128", EVP_aria_256_cfb128, KEY_256, IV_128),
		/*
		MAKE_CIPHER_FUNC("EVP_aria_256_ctr", EVP_aria_256_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_256_ofb", EVP_aria_256_ofb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_256_gcm", EVP_aria_256_gcm, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_aria_256_ccm", EVP_aria_256_ccm, KEY_256, IV_128),
		*/
		MAKE_CIPHER_FUNC("EVP_camellia_128_ecb", EVP_camellia_128_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_128_cbc", EVP_camellia_128_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_128_cfb1", EVP_camellia_128_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_128_cfb8", EVP_camellia_128_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_128_cfb128", EVP_camellia_128_cfb128, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_128_ofb", EVP_camellia_128_ofb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_128_ctr", EVP_camellia_128_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_192_ecb", EVP_camellia_192_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_192_cbc", EVP_camellia_192_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_192_cfb1", EVP_camellia_192_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_192_cfb8", EVP_camellia_192_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_192_cfb128", EVP_camellia_192_cfb128, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_192_ofb", EVP_camellia_192_ofb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_192_ctr", EVP_camellia_192_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_256_ecb", EVP_camellia_256_ecb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_256_cbc", EVP_camellia_256_cbc, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_256_cfb1", EVP_camellia_256_cfb1, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_256_cfb8", EVP_camellia_256_cfb8, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_256_cfb128", EVP_camellia_256_cfb128, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_256_ofb", EVP_camellia_256_ofb, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_camellia_256_ctr", EVP_camellia_256_ctr, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_chacha20", EVP_chacha20, KEY_256, IV_128),
		MAKE_CIPHER_FUNC("EVP_chacha20_poly1305", EVP_chacha20_poly1305, KEY_256, IV_128)
	};
}

Cipher::Cipher(
	const std::string& cipherName,
	CipherFunc cipherFunc,
	const uint8_t* key,
	const uint8_t* iv)
	: m_cipherName(cipherName)
	, m_encryptCtx(EVP_CIPHER_CTX_new(), CipherFree)
	, m_decryptCtx(EVP_CIPHER_CTX_new(), CipherFree)
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
		key,
		iv) != 1)
	{
		HandleErrors("Failed to initialize encryption");
	}
	if (EVP_DecryptInit_ex(
		m_decryptCtx.get(),
		cipherFunc(),
		nullptr,
		key,
		iv) != 1)
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
