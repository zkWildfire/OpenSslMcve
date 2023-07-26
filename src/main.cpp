#include <iostream>
#include <string>
#include <vector>
#include "Cipher.hpp"
#include "Validator.hpp"

int main()
{
	auto cipherFuncs = Cipher::GetCipherFuncs();
	int cipherCount = 0;
	int passCount = 0;
	std::vector<std::string> failingCiphers;

	for (const auto& cipherFunc : cipherFuncs)
	{
		Validator validator(cipherFunc);
		cipherCount++;

		auto result =  validator.Test();
		if (result)
		{
			passCount++;
		}
		else
		{
			failingCiphers.push_back(validator.GetCipherName());
		}
	}

	std::cout << "\n";
	std::cout << "Ciphers tested: " << cipherCount << "\n";
	std::cout << "Ciphers passed: " << passCount << "\n";
	std::cout << "Ciphers failed: " << failingCiphers.size() << "\n";

	if (!failingCiphers.empty())
	{
		std::cout << "\nFailed ciphers:\n";
		for (const auto& cipher : failingCiphers)
		{
			std::cout << cipher << "\n";
		}
	}

	return failingCiphers.size();
}
