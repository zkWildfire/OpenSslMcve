#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <openssl/evp.h>
#include "Cipher.hpp"
#include "IVs.hpp"
#include "Keys.hpp"
#include "Validator.hpp"

void Wait()
{
	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);
	std::condition_variable cv;
	cv.wait_for(lock, std::chrono::milliseconds(100));
}

int main()
{
	// Run the tests so that the status of whether the current system is
	//   affected by the OpenSSL bug is printed
	auto makeCipher = []()
	{
		return Cipher(
			"chacha20-poly1305",
			EVP_chacha20_poly1305,
			KEY_256.data(),
			IV_128.data()
		);
	};
	auto validator = Validator(makeCipher);
	validator.Test();

	// Show that the call to `wait_for()` is fine without the register
	//   corruption caused by the OpenSSL bug
	// Note that it's fine for this to occur after the validator runs its
	//   tests because the validator will restore the registers to the correct
	//   state before returning
	std::cout << "\nWaiting for 100ms before call to OpenSSL...\n";
	Wait();
	std::cout << "Finished pre-OpenSSL wait.\n\n";

	// Call an affected OpenSSL method without restoring the registers
	//   afterwards to show what can happen as a result of the bug
	std::cout << "Waiting for 100ms after the call to OpenSSL...\n";
	std::cout << "Note: On a system affected by the bug, this should cause "
		<< "the executable to hang.\n";
	auto cipher = makeCipher();
	cipher.Encrypt("Hello, world!");
	Wait();
	std::cout << "Finished post-OpenSSL wait.\n\n";

	return 0;
}

