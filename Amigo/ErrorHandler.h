#pragma once
#include <string>

const int
	ERROR_UNKNOWN = -1,
	ERROR_CONTEXT_INIT = 0,
	ERROR_GLFW_INIT = 1,
	ERROR_GLFW_OPEN_WINDOW = 2,
	ERROR_GLEW_INIT = 3,
	ERROR_DEVIL_OUTDATED = 4,
	ERROR_FREETYPE_INIT = 5;

class ErrorHandler
{
public:
	static void Report(int error)
	{
		printf("\nERROR: ");
		switch(error)
		{
		case ERROR_UNKNOWN: printf("Unknown error!"); break;
		case ERROR_CONTEXT_INIT: printf("Failed to initialize context."); break;
		case ERROR_GLFW_OPEN_WINDOW: printf("Failed to open a GLFW window."); break;
		case ERROR_GLEW_INIT: printf("Failed to initialize GLEW."); break;
		case ERROR_DEVIL_OUTDATED: printf("One or more of your DevIL library-files seem to be outdated."); break;
		case ERROR_FREETYPE_INIT: printf("Failed to initialize freetype."); break;
		}
		printf("\nCode %i\n\n", error);
	}
	static void Report(std::string error)
	{
		printf("\nERROR: %s\n", error.c_str());
	}
};