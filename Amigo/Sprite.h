#pragma once

#include <GL\glew.h>

class Sprite
{
	GLuint texture;
	GLint width, height, originX, originY;

public:
	Sprite();
	~Sprite();
	GLuint getTexture();
	GLint getWidth();
	GLint getHeight();
	void setTexture(GLuint texture);
	void setOrigin(GLint originX, GLint originY);
	void setOriginCenter();
	void setInterpolationMode(GLint mode);

	bool LoadImage(char *imagePath);
	void Draw(GLint x, GLint y);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat alpha);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat alpha, GLint x1, GLint y1, GLint x2, GLint y2);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h);
};