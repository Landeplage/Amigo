#include "Input.h"
#include "GLFW.h"
#include "Context.h"
#include <cstdio>
#include "Helper.h"

Vec2 Input::mouse;
bool Input::mouseLeftCurrent;
bool Input::mouseLeftPrevious;
bool Input::mouseLeftPressed;
bool Input::mouseLeftReleased;
bool Input::mouseRightCurrent;
bool Input::mouseRightPrevious;
bool Input::mouseRightPressed;
bool Input::mouseRightReleased;
GLint Input::mouseWheelDiff;
GLint Input::mouseWheelPrevious;
GLint Input::mouseWheelIncrement;
std::string Input::textInput;

void Input::Initialize()
{
	// Get GLFW window
	GLFWwindow* window;
	window = Context::getWindow();

	// Set callback function for the mouse scrollwheel
	glfwSetScrollCallback(window, ScrollWheelCallback);

	// Set callback for text-input
	glfwSetCharCallback(window, CharacterCallback);

	// Initialize string for text input
	textInput = "";
}

void Input::HandleInput()
{
	// Poll GLFW input events
	glfwPollEvents();

	// Get window from the context-class
	GLFWwindow* window;
	window = Context::getWindow();

	// Get mouse-position
	double mX, mY;
	glfwGetCursorPos(window, &mX, &mY);

	// Limit the cursor to the edges of the window
	/*
	GLint windowWidth, windowHeight;
	windowWidth = Context::getWindowWidth();
	windowHeight = Context::getWindowHeight();
	if (mX < 0)
	{ glfwSetCursorPos(window, 0, mY); mX = 0; }
	if (mY < 0)
	{ glfwSetCursorPos(window, mX, 0); mY = 0; }
	if (mX > windowWidth)
	{ glfwSetCursorPos(window, windowWidth, mY); mX = windowWidth; }
	if (mY > windowHeight)
	{ glfwSetCursorPos(window, mX, windowHeight); mY = windowHeight; }
	*/

	mouse.x = (GLfloat)mX;
	mouse.y = (GLfloat)mY;

	// Get mouse left click
	mouseLeftPressed = false;
	mouseLeftReleased = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		mouseLeftCurrent = true;
	else
		mouseLeftCurrent = false;

	if (mouseLeftCurrent && !mouseLeftPrevious)
	{
		mouseLeftPressed = true;
	}
	if (!mouseLeftCurrent && mouseLeftPrevious)
	{
		mouseLeftReleased = true;
	}

	mouseLeftPrevious = mouseLeftCurrent;

	// Get mouse right click
	mouseRightPressed = false;
	mouseRightReleased = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		mouseRightCurrent = true;
	else
		mouseRightCurrent = false;

	if (mouseRightCurrent && !mouseRightPrevious)
	{
		mouseRightPressed = true;
	}
	if (!mouseRightCurrent && mouseRightPrevious)
	{
		mouseRightReleased = true;
	}

	mouseRightPrevious = mouseRightCurrent;
}

void Input::Update(GLdouble time)
{
	// Reset mouse-wheel variable
	mouseWheelDiff = 0;

	// Reset text-input variable
	textInput = "";
}

Vec2 Input::getMousePos()
{
	return mouse;
}

bool Input::getMouseLeft()
{
	return(mouseLeftCurrent);
}

bool Input::getMouseLeftPressed()
{
	return(mouseLeftPressed);
}

bool Input::getMouseLeftReleased()
{
	return(mouseLeftReleased);
}

bool Input::getMouseRight()
{
	return(mouseRightCurrent);
}

bool Input::getMouseRightPressed()
{
	return(mouseRightPressed);
}

bool Input::getMouseRightReleased()
{
	return(mouseRightReleased);
}

GLint Input::getMouseWheel()
{
	return(mouseWheelDiff);
}

GLint Input::getMouseWheelIncrement()
{
	return(mouseWheelIncrement);
}

void Input::setMousePos(Vec2 position)
{
	glfwSetCursorPos(Context::getWindow(), (GLint)position.x, (GLint)position.y);
}

GLint Input::GetKey(GLint key)
{
	return glfwGetKey(Context::getWindow(), key);
}

// Fetches the current textinput
std::string Input::GetTextInput()
{
	return textInput;
}

void Input::ScrollWheelCallback(GLFWwindow* window, double x, double y)
{
	mouseWheelDiff = y;
}

// GLFW callback for keyboard text input (gets unicode codepoint)
void Input::CharacterCallback(GLFWwindow* window, unsigned int codepoint)
{
	std::string s(1, static_cast<char>(codepoint));
	printf(s.c_str());
	textInput += s;
}