#include <iostream>
#include <functional>
#include <openssl/evp.h>
#include <string>
#include "Cipher.hpp"
#include "Validator.hpp"

int main()
{
	auto cipherFuncs = Cipher::GetCipherFuncs();
	for (const auto& cipherFunc : cipherFuncs)
	{
		Validator validator(cipherFunc);
		// TODO: Store the results of each test and print them at the end
		validator.Test();
	}

	return 0;
}
