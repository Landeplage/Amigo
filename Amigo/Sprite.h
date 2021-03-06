#pragma once
#include <GL\glew.h>
#include <IL\il.h> // DevIL
#include "Color.h"
#include "Vec2.h"

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
	void Draw(Vec2 position);
	void Draw(Vec2 position, GLfloat rotation, Vec2 scale, GLfloat alpha);
	void Draw(Vec2 position, GLfloat rotation, Vec2 scale, GLfloat alpha, GLint x1, GLint y1, GLint x2, GLint y2);
	void Draw(Vec2 position, GLfloat rotation, Vec2 scale, Color color, GLfloat alpha);
	void Draw(Vec2 position, GLfloat rotation, Vec2 scale, Color color, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h);

	void DrawRectangleFromTexture(Vec2 position, Vec2 size, Vec2 imageCoordinates, Vec2 cornerSize, Vec2 sidePixels);
	void DrawRectangleFromTexture(Vec2 position, Vec2 size, Vec2 imageCoordinates, Vec2 cornerSize, Vec2 sidePixels, Color color);
};