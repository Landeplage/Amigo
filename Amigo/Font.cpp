#include <GLTools.h>
#include <GL\glew.h>
#include "Font.h"
#include "Helper.h"
#include "DevIL.h"

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
}

void Font::LoadFont(const char* path, int size)
{
	int error = FT_New_Face(FreeType::library, path, 0, &face);

	if (error == FT_Err_Unknown_File_Format)
	{
		printf("FreeType error: Font exists, but the format is unsupported!\n");
	}
	else if (error)
	{
		printf("FreeType error: Font could not be loaded!%s\n", path);
	}

	printf("+ Font: %s\n", path);

	error = FT_Set_Pixel_Sizes(face, 0, size);
	if (error)
	{
		printf("FreeType error: Could not set character size! %s\n", path);
	}

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

	printf("Done.\n");
}

void Font::Draw(int x, int y, std::string str, float rotation, float scaleX, float scaleY, float red, float green, float blue, float alpha)
{
	// Bind the spritefont texture
	glBindTexture(GL_TEXTURE_2D, texture);

	unsigned int charCode;

	for(int i = 0; i < str.length(); i ++)
	{
		// Push the matrix
		glPushMatrix();

		// Get character-code (and subtract by 32, to match the glyph-arrays)
		charCode = (unsigned char)(str[i] - 32);

		// Rotate and translate the quad
		glTranslatef(x + glyphs[charCode].offsetX, y + glyphs[charCode].offsetY, 0);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);

		// Calculate texture coordinates
		long double coX1, coY1, coX2, coY2;
		coX1 = (long double)glyphs[charCode].texX / texW;
		coY1 = (long double)glyphs[charCode].texY / texH;
		coX2 = coX1 + (long double)glyphs[charCode].texW / texW;
		coY2 = coY1 + (long double)glyphs[charCode].texH / texH;

		// Draw the glyph texture
		glBegin(GL_QUADS);
		glColor4f(red, green, blue, alpha);
		glTexCoord2d(coX1, coY1); glVertex2d(0, 0);
		glTexCoord2d(coX2, coY1); glVertex2d(glyphs[charCode].texW, 0);
		glColor4f(red * 0.95f, green * 0.95f, blue * 0.95f, alpha);
		glTexCoord2d(coX2, coY2); glVertex2d(glyphs[charCode].texW, glyphs[charCode].texH);
		glTexCoord2d(coX1, coY2); glVertex2d(0, glyphs[charCode].texH);
		glEnd();

		// Make way for the next character
		x += glyphs[charCode].advanceX + spacing;

		// Pop the matrix
		glPopMatrix();
	}
}

void Font::Draw(int x, int y, std::string str)
{
	Draw(x, y, str, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Font::DrawFormatted(int x, int y, std::string str, int width, int lineHeight)
{
	/*int prevPos = 0;
	while(GetWidth(str) > width)
	{
		int pos = 0;
		while(GetWidth(str.substr(0, pos)) < width)
		{
			pos ++;
		}
		Draw(x, y, str.substr(0, pos - prevPos), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		if (str.length() > 0)
			str = str.substr(pos);
		else
			break;
		y += lineHeight;
		prevPos = pos;
	}*/
	int pos = 0, posPrev = 0;
	while(GetWidth(str.substr(posPrev, str.length() - posPrev)) > width)
	{
		while(GetWidth(str.substr(posPrev, pos)) < width && pos < str.length())
		{
			pos ++;
		}

		Draw(x, y, str.substr(posPrev, pos), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		y += lineHeight;

		posPrev = pos;

		if (GetWidth(str.substr(posPrev, str.length() - posPrev)) <= width)
		{
			Draw(x, y, str.substr(posPrev, str.length() - posPrev), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
	}
}

int Font::GetWidth(std::string str)
{
	int w = 0;
	for(int i = 0; i < str.length(); i++)
	{
		w += glyphs[(int)((unsigned char)(str[i] - 32))].advanceX;
	}

	return w;
}

int Font::GetHeight(std::string str)
{
	int h = 0;
	for(int i = 0; i < str.length(); i++)
	{
		if (glyphs[(int)((unsigned char)(str[i] - 32))].advanceY > h)
			h = glyphs[(int)((unsigned char)(str[i] - 32))].advanceY;
	}

	return h;
}