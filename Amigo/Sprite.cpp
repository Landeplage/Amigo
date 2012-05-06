#include <GLTools.h> // OpenGL Toolkit
#include <IL\il.h> // DevIL
#include "Sprite.h"

// Constructor
Sprite::Sprite()
{
	texture = 0;
	originX = 0;
	originY = 0;
	width = 0;
	height = 0;
}

// Destructor
Sprite::~Sprite()
{
	if (texture > 0)
		glDeleteTextures(1, &texture);
	printf("- Unloaded sprite: %i\n", texture);
}

// Get/Set
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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
}

// Member functions
bool Sprite::LoadImage(char *imagePath)
{
	// Init image variables
	ILuint imageName;
	ILubyte *imageData;

	// Generate an image-name
	ilGenImages(1, &imageName);

	// Bind the image-name
	ilBindImage(imageName);

	// Load the .png into memory
	if (!ilLoadImage((const ILstring) imagePath))
	{
		printf("Failed to load image! -> %s\n", imagePath);
		return false;
	}

	// Get image-data
	imageData = ilGetData();

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

	return true;
}

void Sprite::Draw(GLint x, GLint y)
{
	Draw(x, y, 0.0f, 1.0f, 1.0f, 1.0f, 0, 0, width, height);
}

void Sprite::Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat alpha)
{
	Draw(x, y, rotation, scaleX, scaleY, alpha, 0, 0, width, height);
}

void Sprite::Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h)
{
	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Push the matrix
	glPushMatrix();

	// Rotate and translate the quad
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);

	// Calculate coordinates
	GLdouble coX1, coX2, coY1, coY2;
	coX1 = (GLdouble)xx / width;
	coY1 = (GLdouble)yy / height;
	coX2 = (GLdouble)(xx + w) / width;
	coY2 = (GLdouble)(yy + h) / height;

	// Draw the textured quad
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glTexCoord2d(coX1, coY1); glVertex2d(-originX * scaleX,			-originY * scaleY);
	glTexCoord2d(coX2, coY1); glVertex2d((w - originX) * scaleX,	-originY * scaleY);
	glTexCoord2d(coX2, coY2); glVertex2d((w - originX) * scaleX,	(h - originY) * scaleY);
	glTexCoord2d(coX1, coY2); glVertex2d(-originX * scaleX,			(h - originY) * scaleY);
	glEnd();

	// Pop the matrix
	glPopMatrix();
}