#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <openssl/evp.h>
#include "Cipher.hpp"
#include "IVs.hpp"
#include "Keys.hpp"
#include "Validator.hpp"

int main()
{
	auto makeCipher = []()
	{
		return Cipher(
			"chacha20-poly1305",
			EVP_chacha20_poly1305,
			std::span(KEY_256),
			std::span(IV_128)
		);
	};
	auto validator = Validator(makeCipher);
	validator.Test();

	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);
	std::condition_variable cv;
	cv.wait_for(lock, std::chrono::milliseconds(100));

	return 0;
}

