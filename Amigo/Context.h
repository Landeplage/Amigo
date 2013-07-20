#pragma once
#include <GLTools.h>
#include <GL\glfw.h>

class Context
{
public:
	static GLint getWindowWidth();
	static GLint getWindowHeight();
	
	static int Initialize();
	static void Update(GLdouble time);
	static void Draw();
	static void Cleanup();
private:
	static void GLFWCALL ChangeSize();
	static int GLFWCALL CloseWindow();

	static GLint windowWidth, windowHeight;
	static GLShaderManager shaderManager;
};