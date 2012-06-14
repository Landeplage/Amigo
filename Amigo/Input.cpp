#include "Input.h"
#include <GL\glfw.h>
#include "Context.h"

Point Input::mouse;
bool Input::mouseLeftCurrent;
bool Input::mouseLeftPrevious;
bool Input::mouseLeftPressed;
bool Input::mouseLeftReleased;

void Input::Update(GLdouble time)
{
	// Get mouse-position
	int mX, mY;
	glfwGetMousePos(&mX, &mY);

	// Limit the cursor to the edges of the window
	GLint windowWidth, windowHeight;
	windowWidth = Context::getWindowWidth();
	windowHeight = Context::getWindowHeight();
	if (mX < 0)
	{ glfwSetMousePos(0, mY); mX = 0; }
	if (mY < 0)
	{ glfwSetMousePos(mX, 0); mY = 0; }
	if (mX > windowWidth)
	{ glfwSetMousePos(windowWidth, mY); mX = windowWidth; }
	if (mY > windowHeight)
	{ glfwSetMousePos(mX, windowHeight); mY = windowHeight; }

	mouse.x = mX;
	mouse.y = mY;

	// Get mouse left click
	mouseLeftPressed = false;
	mouseLeftReleased = false;
	mouseLeftCurrent = (bool)glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);

	if (mouseLeftCurrent && !mouseLeftPrevious)
	{
		mouseLeftPressed = true;
	}
	if (!mouseLeftCurrent && mouseLeftPrevious)
	{
		mouseLeftReleased = true;
	}

	mouseLeftPrevious = mouseLeftCurrent;
}

Point Input::getMousePos()
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

GLint Input::GetKey(GLint key)
{
	return glfwGetKey(key);
}