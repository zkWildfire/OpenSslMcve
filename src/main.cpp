#include <iostream>
#include "XmmRegisters.hpp"

int main()
{
	std::cout << "Hello, World!\n";
	{
		XmmRegisters registers(true);
		registers.PrintSavedRegisters();

		std::cout << "Current register state:\n";
		XmmRegisters::PrintAllRegisters();
		std::cout << "\n";
	}
	return 0;
}
