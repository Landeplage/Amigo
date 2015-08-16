#include "Context.h"
#include "GameEngine.h"

//GLShaderManager Context::shaderManager;
GLint Context::windowWidth;
GLint Context::windowHeight;
GLFWwindow* Context::window;
GLFWmonitor* Context::monitor;


int Context::Initialize()
{
	//Init GLFW
	if (!glfwInit())
	{
		throw "Failed to initialize GLFW.";
	}

	printf("- GLFW done.\n");
	
	// Set some GLFW properties
	glfwSwapInterval(1); // V-sync

	// Set window size
	windowWidth = 1280;
	windowHeight = 720;

	// Get primary monitor
	monitor = glfwGetPrimaryMonitor();
	monitor = NULL; // <-- this is to avoid it going fullscreen

	// Create a GLFW window
	window = glfwCreateWindow(windowWidth, windowHeight, "Amigo UI", monitor, NULL);

	// Set current context to window (this enables OpenGL calls)
	glfwMakeContextCurrent(window);

	// Check for error on creating a window
	if (!window)
	{
		throw "Failed to open GLFW window.";
	}

	// Set some GLFW-window properties
	glfwSetWindowPos(window, 200, 200);
	glfwSetWindowRefreshCallback(window, ChangeSize);
	glfwSetWindowCloseCallback(window, CloseWindow);
	//glfwDisable(GLFW_MOUSE_CURSOR);

	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Initialize Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		throw "Failed to initialie GLEW.";
	}

	// Initialize shaders
	//shaderManager.InitializeStockShaders();

	// Set some OpenGL properties
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); // Set blend-equation
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blend-function
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // Tamschi

	glShadeModel(GL_FLAT);
	glAlphaFunc(GL_GREATER, 0.0f); // Skip fully transparent pixels

	// Enables/disables
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_DITHER);
	//glDisable(GL_LIGHTING);

	// Set clear-color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Check if GPU can handle framebuffers
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	default:
		throw "Framebuffers seem to be unsupported on this GPU.";
		break;
	}

	return 0;
}

void Context::Update(GLdouble time)
{
	// Update
}

void Context::Draw()
{
	// Swap front and back rendering buffers
	glfwSwapBuffers(window);

	// Clear the buffer for next frame
	glClear(GL_COLOR_BUFFER_BIT);
}

// Window has changed size, or has just been created
void Context::ChangeSize(GLFWwindow* window)
{
	// Get window size and do stuff
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	glViewport(0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, 0, 1);
	windowWidth = w;
	windowHeight = h;
}

// The user has requested the window to close
void Context::CloseWindow(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	GameEngine::GetInstance()->StopGame();
}

GLFWwindow* Context::getWindow()
{
	return window;
}

int Context::getWindowWidth()
{
	return windowWidth;
}

int Context::getWindowHeight()
{
	return windowHeight;
}

void Context::Cleanup()
{
	// Terminate GLFW
	glfwTerminate();
}