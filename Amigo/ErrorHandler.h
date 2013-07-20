#pragma once
#include <string>

class ErrorHandler
{
public:
	static void Report(std::string error)
	{
		printf("\nERROR: %s\n", error.c_str());
	}
};