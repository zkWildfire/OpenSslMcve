#pragma once
#include <array>
#include <cstdint>
#include <openssl/evp.h>

template <size_t InputSize, size_t KeySize, size_t ExpectedSize>
struct TestData
{
	std::array<uint8_t, InputSize> input;
	std::array<uint8_t, KeySize> key;
	std::array<uint8_t, ExpectedSize> expected;
};

template <size_t InputSize, size_t KeySize, size_t ExpectedSize>
void RunPoly1305(TestData<InputSize, KeySize, ExpectedSize> test)
{
}
