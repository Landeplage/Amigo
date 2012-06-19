#pragma once
#include <GL\glew.h>
#include "Point.h"

class MenuItem
{
public:
	MenuItem();

	enum Align
	{
		TOP,
		LEFT,
		CENTER,
		RIGHT,
		BOTTOM
	};

	virtual void Update(GLdouble time) = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;

	Point GetSize();
	Point GetPosition();

	void SetSize(GLint width, GLint height);
	void SetPosition(GLint x, GLint y);

	bool active, visible;
	int x, y, menuID;
	Point size;
};