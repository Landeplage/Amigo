#pragma once
#include <string>
#include "FreeType.h"
#include <freetype\ftglyph.h>

struct GlyphInfo
{
	int texX, texY, texW, texH, advanceX, advanceY, offsetX, offsetY;
};

class Font
{
public:
	Font();
	~Font();
	void LoadFont(const char* path, int size);

	void Draw(int x, int y, std::string str);
	void Draw(int x, int y, std::string str, float rotation, float scaleX, float scaleY, float red, float green, float blue, float alpha);
	void DrawFormatted(int x, int y, std::string str, int width, int lineHeight);

	int GetWidth(std::string str);
	int GetHeight(std::string str);
private:
	GLuint texture;
	FT_Face face;
	GlyphInfo glyphs[223];

	int texW, texH, spacing;
};