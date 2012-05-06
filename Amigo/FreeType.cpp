#include "FreeType.h"

FT_Library FreeType::library;

int FreeType::Init()
{
	// Initialize FreeType
	int freeTypeError = FT_Init_FreeType(&library);
	if (freeTypeError)
	{
		printf("An error occurred during FreeType initialization!\n");
		return 1;
	}

	return 0;
}