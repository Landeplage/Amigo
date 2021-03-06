/*#include <GLTools.h>*/
#include <GL\glew.h>
#include "Font.h"
#include "Helper.h"
#include "DevIL.h"
#include <string>

using namespace std;

#define SPRITEFONT_MAX_WIDTH 1000

Font::Font()
{
	texW = 0;
	texH = 0;
	spacing = 0;
	texture = 0;

	// Generate texture
	glGenTextures(1, &texture);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture blend-mode (I think?)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Texture properties
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Font::~Font()
{
	if (texture > 0)
		glDeleteTextures(1, &texture);
	printf("- Unloaded a font: %i\n", texture);
}

void Font::LoadFont(const char* path, int size)
{
	int error = FT_New_Face(FreeType::library, path, 0, &face);

	if (error == FT_Err_Unknown_File_Format)
	{
		throw "Font exists, but the format is unsupported: " + (std::string)path;
	}
	else if (error)
	{
		throw "Font could not be loaded: " + (std::string)path;
	}

	error = FT_Set_Pixel_Sizes(face, 0, size);
	if (error)
	{
		throw "Could not set font's character size!" + (std::string)path;
	}

	printf("+ Font: %s\n", path);

	// Print the numnber of glyphs in the font
	printf("\tGlyphs: %i\n", face->num_glyphs);

	// Print all avaible characters and their glyph index
	/*
	printf("\n:::Printing all available glyphs:::\n\n");
	FT_UInt charCode, gIndex;
	gIndex = 0;
	charCode = FT_Get_First_Char(face, &gIndex);
	printf("Character code = %i -> Glyph index = %i (%i)\n", charCode, gIndex, FT_Get_Char_Index(face, charCode));
	printf("Glyph Index = %i\n", FT_Get_Char_Index(face, charCode));
	while(gIndex != 0)
	{
		charCode = FT_Get_Next_Char(face, charCode, &gIndex);
		printf("Character code: %i -> Glyph index: %i (%i)\n", charCode, gIndex, FT_Get_Char_Index(face, charCode));
	}
	printf("\n:: Finished ::\n");*/

	printf("\tGenerating spritefont...");

	int xOffset = 0, yOffset = 0;

	// Initialize the array of bitmap-glyphs
	FT_BitmapGlyph bitmap_glyph[223];

	// Variables for calculation of spritefont texture size
	int wTemp = 0, hTemp = 0;

	// Find size, offsets and other info about each glyph
	// and calculate the size of the spritefont
	for(int i = 0; i < 223; i++)
	{
		int error;

		// Get glyph index
		FT_UInt glyphIndex = FT_Get_Char_Index(face, i + 32);

		// Load glyph-image into slot
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		if (error)
			continue;

		// Move the face's glyph into a glyph object
		FT_Glyph glyph;
		error = FT_Get_Glyph(face->glyph, &glyph);
		if (error)
			continue;

		// Convert glyph into bitmap
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		bitmap_glyph[i] = (FT_BitmapGlyph)glyph;

		// Create a reference, for ease of use
		FT_Bitmap& bitmap = bitmap_glyph[i]->bitmap;

		// Get bitmap properties
		int width, height;
		width = getNextPower2(bitmap.width);
		height = getNextPower2(bitmap.rows);

		// Calculate the size of the spritefont texture
		wTemp += width;

		if (hTemp < height)
		{
			hTemp = height;
		}

		if (wTemp > SPRITEFONT_MAX_WIDTH)
		{
			if (wTemp > texW)
				texW = wTemp;
			texH += hTemp;
			wTemp = 0;
			hTemp = 0;
		}
		
		// Get texture-size and -coordinates
		glyphs[i].texW = bitmap.width;
		glyphs[i].texH = bitmap.rows;

		// Get advance-variables
		glyphs[i].advanceX = face->glyph->advance.x >> 6;
		glyphs[i].advanceY = face->glyph->metrics.height >> 6;

		// Get offset-values
		glyphs[i].offsetX = bitmap_glyph[i]->left;
		glyphs[i].offsetY = (int)(((double)face->size->metrics.height) / 96) - bitmap_glyph[i]->top;
	}

	// Make sure to make room for the last line of
	// characters which doesn't exceed the max width
	texH += hTemp;

	// Allocate memory for the spritefont texture
	glBindTexture(GL_TEXTURE_2D, texture);
	GLubyte* dummyData = new GLubyte[2 * texW * texH];
	for(int i = 0; i < 2 * texW * texH; i++)
	{
		dummyData[i] = 0;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, texW, texH, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, dummyData);

	// Reset the hTemp variable
	hTemp = 0;

	// Generate the spritefont
	for(int i = 0; i < 223; i++)
	{
		FT_Bitmap& bitmap = bitmap_glyph[i]->bitmap;

		// Get bitmap properties
		int width, height;
		width = getNextPower2(bitmap.width);
		height = getNextPower2(bitmap.rows);
		
		GLubyte* expanded_data = new GLubyte[2 * width * height];

		// Fill in data for expanded bitmap
		for(int j=0; j <height;j++)
		{
			for(int n=0; n < width; n++)
			{
				expanded_data[2 * (n + j * width)] = 255;
				expanded_data[2 * (n + j * width) + 1] = (n >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[n + bitmap.width * j];
			}
		}

		// Map glyph into texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);
		
		glyphs[i].texX = xOffset;
		glyphs[i].texY = yOffset;

		// Delete the array, since we won't use it anymore
		delete [] expanded_data;

		// Calculate offset of next glyph
		xOffset += width;

		if (hTemp < height)
			hTemp = height;
		
		if (xOffset + width > texW)
		{
			xOffset = 0;
			yOffset += hTemp;
			hTemp = 0;
		}
	}

	printf("Done :: %i\n", texture);
}

void Font::Draw(Vec2 position, std::string str, float rotation, Vec2 scale, Color color, float alpha)
{
	// Round position floats
	position = Vec2(round(position.x), round(position.y));

	// Bind the spritefont texture
	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned int charCode;
	long double coX1, coY1, coX2, coY2;

	for(GLuint i = 0; i < str.length(); i ++)
	{
		// Push the matrix
		glPushMatrix();

		// Get character-code (and offset by 32, to match the glyph-arrays)
		charCode = (unsigned char)(str[i] - 32);

		// Rotate and translate the quad
		glTranslatef((GLfloat)(position.x + glyphs[charCode].offsetX), (GLfloat)(position.y + glyphs[charCode].offsetY), 0.0f);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);

		// Calculate texture coordinates
		coX1 = (long double)glyphs[charCode].texX / texW;
		coY1 = (long double)glyphs[charCode].texY / texH;
		coX2 = coX1 + (long double)glyphs[charCode].texW / texW;
		coY2 = coY1 + (long double)glyphs[charCode].texH / texH;

		// Draw the glyph texture
		glBegin(GL_QUADS);
		glColor4ub(color.r, color.g, color.b, (unsigned char)(Clamp(alpha, 0.0f, 1.0f) * 255));
		glTexCoord2d(coX1, coY1); glVertex2d(0, 0);
		glTexCoord2d(coX2, coY1); glVertex2d(glyphs[charCode].texW, 0);
		glTexCoord2d(coX2, coY2); glVertex2d(glyphs[charCode].texW, glyphs[charCode].texH);
		glTexCoord2d(coX1, coY2); glVertex2d(0, glyphs[charCode].texH);
		glEnd();

		// Make way for the next character
		position.x += glyphs[charCode].advanceX + spacing;

		// Pop the matrix
		glPopMatrix();
	}
}

void Font::Draw(Vec2 position, std::string str)
{
	Draw(position, str, 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 1.0f);
}

void Font::DrawShorten(Vec2 position, std::string str, float rotation, Vec2 scale, Color color, float alpha, int width)
{
	int pos = 1;
	if (GetWidth(str) > width)
	{
		while(GetWidth(str.substr(0, str.length() - pos) + "...") > width) // shorten the string if it's too long
		{
			pos ++;
			if (str.length() - pos <= 1)
				break;
			if (str.substr(str.length() - pos - 1, 1) == " ") // we don't want the last character to be a space
			{
				pos ++;
				if (str.length() - pos <= 1)
				break;
			}
		}
		str = str.substr(0, str.length() - pos) + "...";
	}
	Draw(position, str, rotation, scale, color, alpha);
}

void Font::DrawLinebreak(Vec2 position, std::string str, int width, int lineHeight)
{
	DrawLinebreak(position, str, width, lineHeight, Color(255, 255, 255), 1.0f);
}

void Font::DrawLinebreak(Vec2 position, std::string str, int width, int lineHeight, Color color, float alpha)
{
	int pos = 0, posPrev = 0;
	while(GetWidth(str.substr(posPrev, str.length() - posPrev)) > width)
	{
		while(GetWidth(str.substr(posPrev, pos - posPrev)) < width) // find the first line
		{
			pos ++;
		}

		if (pos == 0) break;

		if (str.substr(posPrev, pos - posPrev).find_first_of(" ", 0) != string::npos) // if the line contains a space
		{
			while(str.substr(pos - 1, 1) != " ") // find last occurring space
			{ pos --; }
		}
		else if (str.substr(posPrev, pos - posPrev).find_first_of("-", 0) != string::npos) // if the line contains a dash
		{
			while(str.substr(pos - 1, 1) != "-") // find last occurring dash
			{ pos --; }
		}
		else if (pos - posPrev > 2) // insert a dash at the end of the line (check if there are more than two characters in the line to avoid infinite loop)
		{
			pos--;
			str.insert(pos - 1, "-");
		}

		Draw(position, str.substr(posPrev, pos - posPrev), 0.0f, Vec2(1.0f, 1.0f), color, alpha);
		position.y += lineHeight;

		posPrev = pos;
	}

	// Draw the last line
	Draw(position, str.substr(pos, str.length() - pos), 0.0f, Vec2(1.0f, 1.0f), color, alpha);
}

// Get width of a single-line string
int Font::GetWidth(std::string str)
{
	int w = 0;
	for(GLuint i = 0; i < str.length(); i++)
	{
		w += glyphs[(int)((unsigned char)(str[i] - 32))].advanceX;
	}

	return w;
}

// Get width of a multi-line string
int Font::GetWidth(std::string str, GLint width)
{
	int w = 0, pos = 0, posPrev = 0;
	while(GetWidth(str.substr(posPrev, str.length() - posPrev)) > width)
	{
		while(GetWidth(str.substr(posPrev, pos - posPrev)) < width) // find the first line
		{
			pos ++;
		}

		if (pos == 0) break;

		if (str.substr(posPrev, pos - posPrev).find_first_of(" ", 0) != string::npos) // if the line contains a space
		{
			while(str.substr(pos - 1, 1) != " ") // find last occurring space
			{ pos --; }
		}
		else if (str.substr(posPrev, pos - posPrev).find_first_of("-", 0) != string::npos) // if the line contains a dash
		{
			while(str.substr(pos - 1, 1) != "-") // find last occurring dash
			{ pos --; }
		}
		else if (pos - posPrev > 2) // insert a dash at the end of the line (check if there are more than two characters in the line to avoid infinite loop)
		{
			pos--;
			str.insert(pos - 1, "-");
		}

		if (GetWidth(str.substr(posPrev, pos - posPrev)) > w)
			w = GetWidth(str.substr(posPrev, pos - posPrev));

		posPrev = pos;
	}

	if (GetWidth(str.substr(pos, str.length() - pos)) > w)
			w = GetWidth(str.substr(pos, str.length() - pos));

	return w;
}

int Font::GetHeight(std::string str)
{
	int h = 0;
	for(GLuint i = 0; i < str.length(); i++)
	{
		if (glyphs[(GLint)((unsigned char)(str[i] - 32))].advanceY > h)
			h = glyphs[(GLint)((unsigned char)(str[i] - 32))].advanceY;
	}

	return h;
}

int Font::GetHeight(std::string str, GLint width, GLint lineHeight)
{
	int pos = 0, posPrev = 0, h = 0;
	while(GetWidth(str.substr(posPrev, str.length() - posPrev)) > width)
	{
		while(GetWidth(str.substr(posPrev, pos - posPrev)) < width) // find the first line
		{
			pos ++;
		}

		if (pos == 0) break;

		if (str.substr(posPrev, pos - posPrev).find_first_of(" ", 0) != string::npos) // if the line contains a space
		{
			while(str.substr(pos - 1, 1) != " ") // find last occurring space
			{ pos --; }
		}
		else if (str.substr(posPrev, pos - posPrev).find_first_of("-", 0) != string::npos) // if the line contains a dash
		{
			while(str.substr(pos - 1, 1) != "-") // find last occurring dash
			{ pos --; }
		}
		else if (pos - posPrev > 2) // insert a dash at the end of the line (check if there are more than two characters in the line to avoid infinite loop)
		{
			pos--;
			str.insert(pos - 1, "-");
		}

		h += lineHeight;

		posPrev = pos;
	}

	h += GetHeight(str.substr(pos, str.length() - pos));

	return h;
}

// Truncate string to width with "..." at the end
std::string Font::TruncateString(std::string text, GLint width)
{
	std::string str;
	str = text;

	if (GetWidth(str) > width)
	{
		int pos = 1;
		while (GetWidth(str.substr(0, str.length() - pos) + "...") > width) // shorten the string if it's too long
		{
			pos++;
			if (str.length() - pos <= 1)
				break;
			if (str.substr(str.length() - pos - 1, 1) == " ") // we don't want the last character to be a space
			{
				pos++;
				if (str.length() - pos <= 1)
					break;
			}
		}
		str = str.substr(0, str.length() - pos) + "...";
	}

	return str;
}