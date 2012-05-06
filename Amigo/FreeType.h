#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

static class FreeType
{
public:
	static int Init();
	static FT_Library library;
};