#include "Input.h"
#include <GL\glfw.h>
#include "Context.h"

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

void Input::HandleInput()
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

	mouse.x = (GLfloat)mX;
	mouse.y = (GLfloat)mY;

	// Get mouse left click
	mouseLeftPressed = false;
	mouseLeftReleased = false;
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
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
	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
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

	// Update mouse-wheel variables
	GLint mouseWheelCurrent = glfwGetMouseWheel();
	mouseWheelDiff = mouseWheelCurrent - mouseWheelPrevious;
	mouseWheelIncrement = 0;

	if (mouseWheelCurrent > mouseWheelPrevious)
	{
		mouseWheelIncrement = 1;
	}
	if (mouseWheelCurrent < mouseWheelPrevious)
	{
		mouseWheelIncrement = -1;
	}

	mouseWheelPrevious = mouseWheelCurrent;
}

void Input::Update(GLdouble time)
{
	// update
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
	glfwSetMousePos((GLint)position.x, (GLint)position.y);
}

GLint Input::GetKey(GLint key)
{
	return glfwGetKey(key);
}