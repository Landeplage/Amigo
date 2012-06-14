#pragma once
#include <GL\glew.h>

class Point
{
public:
	Point(GLfloat x = 0, GLfloat y = 0)
	{
		this->x = x;
		this->y = y;
	}

	GLfloat x, y;
};