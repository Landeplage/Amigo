#pragma once
#include <GL\glew.h>

class Vec2
{
public:
	Vec2(GLfloat x = 0, GLfloat y = 0)
	{
		this->x = x;
		this->y = y;
	}

	GLfloat x, y;

	Vec2 operator+(const Vec2 operand);
	Vec2 operator-(const Vec2 operand);
};