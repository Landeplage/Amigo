//#include <GLTools.h> // OpenGL Toolkit
#include "Sprite.h"
#include <string>
#include "Helper.h"
#include <IL\ilu.h>

// Constructor
Sprite::Sprite()
{
	texture = 0;
	imageName = 0;
	originX = 0;
	originY = 0;
	width = 0;
	height = 0;
}

// Destructor
Sprite::~Sprite()
{
	if (texture > 0)
	{
		glDeleteTextures(1, &texture);
	}
	if (imageName > 0)
	{
		ilDeleteImages(1, &imageName);
	}
	printf("- Unloaded sprite: %i\n", texture);
}

// Get/Set
ILuint Sprite::getImageName()
{
	return imageName;
}

GLuint Sprite::getTexture()
{
	return texture;
}

void Sprite::setTexture(GLuint texture)
{
	this->texture = texture;
}

GLint Sprite::getWidth()
{
	return width;
}

GLint Sprite::getHeight()
{
	return height;
}

void Sprite::setOrigin(GLint originX, GLint originY)
{
	this->originX = originX;
	this->originY = originY;
}

void Sprite::setOriginCenter()
{
	this->originX = width / 2;
	this->originY = height / 2;
}

void Sprite::setInterpolationMode(GLint mode)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)mode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)mode);
}

ILuint Sprite::CreateImage()
{
	// Generate an image-name
	ilGenImages(1, &imageName);

	// Bind the image-name
	ilBindImage(imageName);

	return imageName;
}

// Load image from file
bool Sprite::LoadTexture(const char *imagePath)
{
	// Init image variables
	ILubyte *imageData;

	// Create an image
	CreateImage();

	// Load the image file into memory
	if (!ilLoadImage((const ILstring) imagePath))
	{
		throw "Could not load image: " + (std::string)imagePath;
	}

	// Get image-data
	imageData = ilGetData();

	// Premultiply alpha
	if (ilGetInteger(IL_IMAGE_FORMAT) == IL_RGBA)
	{
		GLint
			width = ilGetInteger(IL_IMAGE_WIDTH),
			height = ilGetInteger(IL_IMAGE_HEIGHT);

		printf("\tPremultiplying %i pixels...\n", width * height);
		for(int i = 0; i < width * height * 4; i += 4)
		{
			/*// Print five first pixels before multiplication
			if (i < 5 * 4)
				printf("\tPixel %i before = \t%.2f \t%.2f \t%.2f \t%.2f\n", (int)(i / 4), imageData[i] / 255.0f, imageData[i + 1] / 255.0f, imageData[i + 2] / 255.0f, imageData[i + 3] / 255.0f);*/
				
			// Premultiplicate alphas
			for(int n = 0; n < 3; n ++)
			{
				//imageData[i + n] = (unsigned char)(((imageData[i + n] / 255.0f) * (imageData[i + 3] / 255.0f)) * 255.0f);
			}

			/*// Print five first pixels after multiplication
			if (i < 5 * 4)
				printf("\tPixel %i after = \t%.2f \t%.2f \t%.2f\n", (int)(i / 4), imageData[i] / 255.0f, imageData[i + 1] / 255.0f, imageData[i + 2] / 255.0f);*/
		};
	}

	// Generate texture
	glGenTextures(1, &texture);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Map image to texture
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		ilGetInteger(IL_IMAGE_BPP),
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0,
		ilGetInteger(IL_IMAGE_FORMAT),
		GL_UNSIGNED_BYTE,
		imageData);

	// Set texture blend-mode (I think?)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Make the texture repeat on the edges and handle mip-mapping
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Get some info about the image/texture and print to console
	printf("+ Sprite: %s :: %i\n", imagePath, texture);
	ILint depth, bpp, format;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	depth = ilGetInteger(IL_IMAGE_DEPTH);
	bpp = ilGetInteger(IL_IMAGE_BPP);
	format = ilGetInteger(IL_IMAGE_FORMAT);

	/*
	printf("Width: %i \n", width);
	printf("Height: %i \n", height);
	printf("Depth: %i \n", depth);
	printf("BPP: %i \n", bpp);
	printf("Format: %i\n", format);
	*/

	/*
	// Print out the first few pixels
	int n = 0;
	// RGB
	if (format == IL_RGB)
	{
		for(int i = 0; i < 5; i ++)
		{
			printf("Pixel %i = \t%i \t%i \t%i\n", i, (int)imageData[n], (int)imageData[n + 1], (int)imageData[n + 2]);
			n += 3;
		};
	}
	// RGBA
	if (format == IL_RGBA)
	{
		for(int i = 0; i < 5; i ++)
		{
			printf("Pixel %i = \t%i \t%i \t%i \t%i\n", i, (int)imageData[n], (int)imageData[n + 1], (int)imageData[n + 2], (int)imageData[n + 3]);
			n += 4;
		};
	}
	*/

	return true;
}

void Sprite::Draw(Vec2 position)
{
	Draw(position, 0.0f, Vec2(1.0f, 1.0f), 1.0f, 0, 0, width, height);
}

void Sprite::Draw(Vec2 position, GLfloat rotation, Vec2 scale, GLfloat alpha)
{
	Draw(position, rotation, scale, alpha, 0, 0, width, height);
}

void Sprite::Draw(Vec2 position, GLfloat rotation, Vec2 scale, Color color, GLfloat alpha)
{
	Draw(position, rotation, scale, color, alpha, 0, 0, width, height);
}

void Sprite::Draw(Vec2 position, GLfloat rotation, Vec2 scale, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h)
{
	Draw(position, rotation, scale, Color(255, 255, 255), alpha, xx, yy, w, h);
}

void Sprite::Draw(Vec2 position, GLfloat rotation, Vec2 scale, Color color, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h)
{
	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Push the matrix
	glPushMatrix();

	// Rotate and translate the quad
	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);

	// Calculate coordinates
	GLdouble coX1, coX2, coY1, coY2;
	coX1 = (GLdouble)xx / width;
	coY1 = (GLdouble)yy / height;
	coX2 = (GLdouble)(xx + w) / width;
	coY2 = (GLdouble)(yy + h) / height;

	// Determine alpha
	GLfloat a = Clamp(alpha, 0.0f, 1.0f);
	glColor4ub(color.r, color.g, color.b, (unsigned char)(a * 255));

	// Draw the textured quad
	glBegin(GL_QUADS);
	glTexCoord2d(coX1, coY1); glVertex2d(-originX * scale.x,		-originY * scale.y);
	glTexCoord2d(coX2, coY1); glVertex2d((w - originX) * scale.x,	-originY * scale.y);
	glTexCoord2d(coX2, coY2); glVertex2d((w - originX) * scale.x,	(h - originY) * scale.y);
	glTexCoord2d(coX1, coY2); glVertex2d(-originX * scale.x,		(h - originY) * scale.y);
	glEnd();

	// Pop the matrix
	glPopMatrix();
}

// Draw a rectangle based on a minimized rectangle within a texture
void Sprite::DrawRectangleFromTexture(Vec2 position, Vec2 size, Vec2 imageCoordinates, Vec2 cornerSize, Vec2 sidePixels)
{
	DrawRectangleFromTexture(position, size, imageCoordinates, cornerSize, sidePixels, Color(255, 255, 255));
}

// Draw a rectangle based on a minimized rectangle within a texture
void Sprite::DrawRectangleFromTexture(Vec2 position, Vec2 size, Vec2 imageCoordinates, Vec2 cornerSize, Vec2 sidePixels, Color color)
{
	// Set alpha, rotation, color
	GLfloat a, r;
	a = 1.0f;
	r = 0.0f;

	Color c;
	c = color;

	// Helper variables
	GLfloat x, y, w, h, icx, icy, csx, csy, hpix, vpix;
	x = position.x;
	y = position.y;
	w = size.x;
	h = size.y;
	icx = imageCoordinates.x;
	icy = imageCoordinates.y;
	csx = cornerSize.x;
	csy = cornerSize.y;
	hpix = sidePixels.x;
	vpix = sidePixels.y;

	// Middle
	Draw(Vec2(x + csx, y + csy), r, Vec2((w - (csx * 2)) / hpix, (h - (csy * 2)) / vpix), c, a, icx + csx, icy + csy, hpix, vpix);

	// Top left corner
	Draw(Vec2(x, y), r, Vec2(1.0f, 1.0f), c, a, icx, icy, csx, csy);

	// Top
	Draw(Vec2(x + csx, y), r, Vec2((w - (csx * 2)) / hpix, 1.0f), c, a, icx + csx, icy, hpix, csy);

	// Top right corner
	Draw(Vec2(x + w - csx, y), r, Vec2(1.0f, 1.0f), c, a, icx + csx + hpix, icy, csx, csy);

	// Right side
	Draw(Vec2(x + w - csx, y + csy), r, Vec2(1.0f, (h - (csy * 2)) / vpix), c, a, icx + csx + hpix, icy + csy, csx, vpix);

	// Bottom right corner
	Draw(Vec2(x + w - csx, y + h - csy), r, Vec2(1.0f, 1.0f), c, a, icx + csx + hpix, icy + csy + vpix, csx, csy);

	// Bottom
	Draw(Vec2(x + csx, y + h - csy), r, Vec2((w - (csx * 2)) / hpix, 1.0f), c, a, icx + csx, icy + csy + vpix, hpix, csy);

	// Bottom left corner
	Draw(Vec2(x, y + h - csy), r, Vec2(1.0f, 1.0f), c, a, icx, icy + csy + vpix, csx, csy);

	// Left side
	Draw(Vec2(x, y + csy), r, Vec2(1.0f, (h - (csy * 2)) / vpix), c, a, icx, icy + csy, csx, vpix);
}