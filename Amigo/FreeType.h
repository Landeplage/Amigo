#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class FreeType
{
public:
	static int Init();
	static FT_Library library;
};