#include "FreeType.h"
#include "ErrorHandler.h"

FT_Library FreeType::library;

int FreeType::Init()
{
	// Initialize FreeType
	int freeTypeError = FT_Init_FreeType(&library);
	if (freeTypeError)
	{
		throw ERROR_FREETYPE_INIT;
	}

	return 0;
}