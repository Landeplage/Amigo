#pragma once
#include "Vec2.h"

class Input
{
public:
	static bool getMouseLeft();
	static bool getMouseLeftPressed();
	static bool getMouseLeftReleased();
	static bool getMouseRight();
	static bool getMouseRightPressed();
	static bool getMouseRightReleased();
	static GLint getMouseWheel();
	static GLint getMouseWheelIncrement();
	static Vec2 getMousePos();
	static void setMousePos(Vec2 position);

	static GLint GetKey(GLint key);

	static void Update(GLdouble time);
private:
	static bool mouseLeftCurrent, mouseLeftPrevious, mouseLeftPressed, mouseLeftReleased;
	static bool mouseRightCurrent, mouseRightPrevious, mouseRightPressed, mouseRightReleased;
	static GLint mouseWheelDiff, mouseWheelPrevious, mouseWheelIncrement;
	static Vec2 mouse;
};