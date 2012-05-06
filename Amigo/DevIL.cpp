#include "DevIL.h"

int DevIL::Init()
{
	// Set up DevIL
	int devilErr;
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION
		|| iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION
		|| ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		printf("Wrong version(s) of the DevIL libraries!\n");
		return 1;
	}

	return 0;
}