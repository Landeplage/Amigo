#pragma once
#include "Vec2.h"

static class Input
{
public:
	static bool getMouseLeft();
	static bool getMouseLeftPressed();
	static bool getMouseLeftReleased();
	static bool getMouseRight();
	static bool getMouseRightPressed();
	static bool getMouseRightReleased();
	static Vec2 getMousePos();

	static GLint GetKey(GLint key);

	static void Update(GLdouble time);
private:
	static bool mouseLeftCurrent, mouseLeftPrevious, mouseLeftPressed, mouseLeftReleased;
	static bool mouseRightCurrent, mouseRightPrevious, mouseRightPressed, mouseRightReleased;
	static Vec2 mouse;
};