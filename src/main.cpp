#include <iostream>
#include "XmmRegisters.hpp"

int main()
{
	std::cout << "Hello, World!\n";
	{
		XmmRegisters registers;
		registers.PrintSavedRegisters();
	}
	return 0;
}
