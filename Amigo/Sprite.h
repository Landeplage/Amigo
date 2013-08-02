#pragma once
#include <GL\glew.h>
#include <IL\il.h> // DevIL
#include "Color.h"

class Sprite
{
	ILuint imageName;
	GLuint texture;
	GLint width, height, originX, originY;

public:
	Sprite();
	~Sprite();
	ILuint getImageName();
	GLuint getTexture();
	GLint getWidth();
	GLint getHeight();
	void setTexture(GLuint texture);
	void setOrigin(GLint originX, GLint originY);
	void setOriginCenter();
	void setInterpolationMode(GLint mode);

	ILuint CreateImage();
	bool LoadTexture(const char *imagePath);
	void Draw(GLint x, GLint y);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat alpha);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat alpha, GLint x1, GLint y1, GLint x2, GLint y2);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, Color color, GLfloat alpha);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, Color color, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h);
};