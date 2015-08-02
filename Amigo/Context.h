#pragma once
//#include <GLTools.h>
#include <GL\glew.h>
#include "GLFW.h"

#pragma comment(lib, "glew32.lib")

class Context
{
public:
	static GLFWwindow* getWindow();
	static GLint getWindowWidth();
	static GLint getWindowHeight();
	
	static int Initialize();
	static void Update(GLdouble time);
	static void Draw();
	static void Cleanup();
private:
	static void ChangeSize(GLFWwindow* window);
	static void CloseWindow(GLFWwindow* window);

	//static GLShaderManager shaderManager;
	static GLint windowWidth, windowHeight;

	static GLFWwindow* window;
	static GLFWmonitor* monitor;
};