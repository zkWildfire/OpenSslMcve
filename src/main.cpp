#include <iostream>
#include "XmmRegisters.hpp"

int main()
{
	std::cout << "Hello, World!\n";
	{
		XmmRegisters registers(true);
		registers.PrintSavedRegisters();
		XmmRegisters::PrintAllRegisters();
	}
	return 0;
}
