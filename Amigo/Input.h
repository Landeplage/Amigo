#pragma once
#include "Vec2.h"
#include "GLFW.h"
#include <string>

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
	static std::string GetTextInput();

	static void Initialize();
	static void HandleInput();
	static void Update(GLdouble time);
private:
	static void ScrollWheelCallback(GLFWwindow* window, double x, double y);
	static void CharacterCallback(GLFWwindow* window, unsigned int codepoint);

	static bool mouseLeftCurrent, mouseLeftPrevious, mouseLeftPressed, mouseLeftReleased;
	static bool mouseRightCurrent, mouseRightPrevious, mouseRightPressed, mouseRightReleased;
	static GLint mouseWheelDiff, mouseWheelPrevious, mouseWheelIncrement;
	static Vec2 mouse;

	static std::string textInput;
};