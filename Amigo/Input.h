#pragma once
#include "Point.h"

static class Input
{
public:
	static bool getMouseLeft();
	static bool getMouseLeftPressed();
	static bool getMouseLeftReleased();
	static Point getMousePos();

	static GLint GetKey(GLint key);

	static void Update(GLdouble time);
private:
	static bool mouseLeftCurrent, mouseLeftPrevious, mouseLeftPressed, mouseLeftReleased;
	static Point mouse;
};