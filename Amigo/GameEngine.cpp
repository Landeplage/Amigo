// Libraries
#include "GameEngine.h"
#include "Context.h"
#include "Input.h"
#include "DevIL.h"
#include "FreeType.h"

#include "State.h"
#include "Sprite.h"
#include "Helper.h"

static State *currentState;

bool GameEngine::isLoading;
bool GameEngine::initLoad;
bool GameEngine::gameRunning;
Sprite GameEngine::sprLoading;
GLfloat GameEngine::loadingRot;
State *GameEngine::newState;

bool GameEngine::Initialize(int argc, char* argv[])
{
	printf("Initializing game...\n");

	int error;
	// Initialize context
	error = Context::Initialize(argc, argv);
	if (error)
	{
		printf("Failed to initialize context!\n");
	}

	printf("Your OpenGL version is %s\n", (const char*)glGetString(GL_VERSION));

	// Initialize DevIL
	error = DevIL::Init();
	if (error)
	{
		printf("Failed to initialize DevIL!\n");
	}

	// Initialize FreeType
	error = FreeType::Init();
	if (error)
	{
		printf("Failed to initialize FreeType!\n");
	}

	// Init various variables
	loadingRot = 0;
	isLoading = false;
	initLoad = false;

	// Load stuff
	Load();

	printf("Game initialized!\n");

	return true;
}

void GameEngine::Load()
{
	printf("\nLoading globals:\n");

	// Load sprites
	sprLoading.LoadImage("res\\tx\\ui\\loading.png");
	sprLoading.setOrigin(sprLoading.getWidth() / 2, sprLoading.getHeight() / 2);

	printf("\n");
}

void GameEngine::GameLoop()
{
	// Set framerate cap
	GLint framerate = 999;
	GLdouble lastFrameTime = 0, currentFrameTime = 0, fpsLastUpdate = 0;
	gameRunning = true;

	// Main-loop
	while(gameRunning)
	{
		currentFrameTime = glfwGetTime();
		if ((currentFrameTime - lastFrameTime) * 1000 < 1000 / framerate)
		{
			//glfwSleep(0.001);
		}
		else
		{
			// Check if loading has been initiated and go to new state
			if (initLoad)
			{
				isLoading = true;
				GoToState(newState);
				initLoad = false;
			}

			// Update
			Update((currentFrameTime - lastFrameTime) * 1000);
			
			// Draw
			Draw();

			// update fps-counter
			GLdouble time = currentFrameTime;
			if (time - fpsLastUpdate >= 1)
			{
				GLdouble fps = (currentFrameTime * 1000 - lastFrameTime * 1000);
				glfwSetWindowTitle(toString((GLint)(1000 / fps + 0.5)).c_str());
				fpsLastUpdate = time;
			}

			lastFrameTime = currentFrameTime;
		}
	}

	// Cleanup game if loop stops
	GameEngine::Cleanup();
}

// Update the engine
void GameEngine::Update(GLdouble time)
{
	Input::Update(time);
	Context::Update(time);

	// Handle current game-state
	if (!isLoading)
	{
		currentState->Update(time);
	}
	else
	{
		isLoading = !(currentState->Load());
		
		// Rotation effect-variable
		loadingRot++;
		if (loadingRot >= 360)
			loadingRot -= 360;
	}
}

// Draw the engine
void GameEngine::Draw()
{
	// Draw current game-state
	if (!isLoading)
		currentState->Draw();
	else // or the loading-screen
		sprLoading.Draw(Context::getWindowWidth() / 2, Context::getWindowHeight() / 2, loadingRot * 8, 1.0f, 1.0f, 1.0f);

	Context::Draw();
}

// Queue new state
void GameEngine::ChangeState(State *state)
{
	initLoad = true;
	newState = state;
}

// Queue new state (with an option to hide loading)
void GameEngine::ChangeState(State *state, bool showLoading)
{
	ChangeState(state);
	//isLoading = showLoading;
	//currentState->Load();
}

// Go to new state
void GameEngine::GoToState(State *state)
{
	if (currentState)
	{
		delete currentState;
	}
	currentState = state;
	currentState->Init();
}

// End the game-loop
void GameEngine::StopGame()
{
	gameRunning = false;
}

// Cleanup the game
void GameEngine::Cleanup()
{
	currentState->~State();
	Context::Cleanup();
}

