#pragma once
#include <GL\glew.h>

class Vec4
{
public:
	Vec4(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0, GLfloat æ = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->æ = æ;
	}

	GLfloat x, y, z, æ;

	Vec4 operator+(const Vec4 operand);
	Vec4 operator-(const Vec4 operand);
};