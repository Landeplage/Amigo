#include "Context.h"
#include "GameEngine.h"

GLShaderManager Context::shaderManager;
GLint Context::windowWidth;
GLint Context::windowHeight;
POINT Context::mouse;

int Context::Initialize(int argv, char* argc[])
{
	//Init GLFW
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW!");
		return 1;
	}
	
	// Set some GLFW properties
	glfwSwapInterval(1); // V-sync

	// Open a GLFW window
	if (!glfwOpenWindow(1280, 720, 0, 0, 0, 0, 32, 0, GLFW_WINDOW ))
	{
		glfwTerminate();
		printf("Failed to create GLFW window!");
		return 2;
	}

	// Set some GLFW-window properties
	glfwSetWindowTitle("Amigo");
	glfwSetWindowPos(200, 200);
	glfwSetWindowRefreshCallback(ChangeSize);
	glfwSetWindowCloseCallback(CloseWindow);

	// Initiate Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 3;
	}

	// Initialize shaders
	shaderManager.InitializeStockShaders();

	// Set some OpenGL properties
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Clear background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return 0;
}

void Context::Update(GLdouble time)
{
	// Get mouse-position relative to window
	GetCursorPos(&mouse);
	mouse.x -= 0;
	mouse.y -= 0;
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

POINT Context::getMousePos()
{
	return mouse;
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