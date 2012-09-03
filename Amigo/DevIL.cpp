#include "DevIL.h"
#include "ErrorHandler.h"

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
		throw ERROR_DEVIL_OUTDATED;
	}

	return 0;
}