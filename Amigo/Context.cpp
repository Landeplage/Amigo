#include "Context.h"
#include "GameEngine.h"
#include "ErrorHandler.h"

GLShaderManager Context::shaderManager;
GLint Context::windowWidth;
GLint Context::windowHeight;

int Context::Initialize(int argv, char* argc[])
{
	//Init GLFW
	if (!glfwInit())
	{
		throw ERROR_GLFW_INIT;
	}
	
	// Set some GLFW properties
	glfwSwapInterval(1); // V-sync

	// Set window size
	windowWidth = 1280;
	windowHeight = 720;

	// Open a GLFW window
	if (!glfwOpenWindow(windowWidth, windowHeight, 0, 0, 0, 0, 32, 0, GLFW_WINDOW ))
	{
		throw ERROR_GLFW_OPEN_WINDOW;
	}

	// Set some GLFW-window properties
	glfwSetWindowTitle("AmigoUI");
	glfwSetWindowPos(200, 200);
	glfwSetWindowRefreshCallback(ChangeSize);
	glfwSetWindowCloseCallback(CloseWindow);
	//glfwDisable(GLFW_MOUSE_CURSOR);

	// Initiate Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		throw ERROR_GLEW_INIT;
	}

	// Initialize shaders
	shaderManager.InitializeStockShaders();

	// Set some OpenGL properties
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); // Set blend-equation
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blend-function
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
		printf("Whoops! FBOs are not supported on your GPU! #%i\n", status);
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
	glfwSwapBuffers();

	// Clear the buffer for next frame
	glClear(GL_COLOR_BUFFER_BIT);
}

// Window has changed size, or has just been created
void Context::ChangeSize()
{
	// Get window size and do stuff
	int w, h;
	glfwGetWindowSize(&w, &h);

	glViewport(0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, 0, 1);
	windowWidth = w;
	windowHeight = h;
}

// The user has requested the window to close
int Context::CloseWindow()
{
	glfwCloseWindow();
	GameEngine::StopGame();

	return 0;
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