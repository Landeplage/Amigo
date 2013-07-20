#include "DevIL.h"

int DevIL::Init()
{
	// Set up DevIL
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION
		|| iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION
		|| ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		throw "Failed to initialize DevIL; library files are outdated.";
	}

	return 0;
}