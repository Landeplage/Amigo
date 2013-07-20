#pragma once
#include <string>
#include "FreeType.h"
#include <freetype\ftglyph.h>
#include "Color.h"

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

	int GetWidth(std::string str);
	int GetWidth(std::string str, GLint maxWidth);
	int GetHeight(std::string str);
	int GetHeight(std::string str, GLint maxWidth, GLint lineHeight);

	void Draw(int x, int y, std::string str);
	void Draw(int x, int y, std::string str, float rotation, float scaleX, float scaleY, Color color, float alpha);
	void DrawShorten(int x, int y, std::string str, float rotation, float scaleX, float scaleY, Color color, float alpha, int width);
	void DrawLinebreak(int x, int y, std::string str, int width, int lineHeight);
	void DrawLinebreak(int x, int y, std::string str, int width, int lineHeight, Color color, float alpha);
private:
	GLuint texture;
	FT_Face face;
	GlyphInfo glyphs[223];

	int texW, texH, spacing;
};