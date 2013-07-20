#include "FreeType.h"

FT_Library FreeType::library;

int FreeType::Init()
{
	// Initialize FreeType
	int freeTypeError = FT_Init_FreeType(&library);
	if (freeTypeError)
	{
		throw "Failed to initialize FreeType";
	}

	return 0;
}