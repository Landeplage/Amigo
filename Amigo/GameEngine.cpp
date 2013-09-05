// Libraries
#include "GameEngine.h"
#include "ErrorHandler.h"
#include "Context.h"
#include "Input.h"
#include "DevIL.h"
#include "FreeType.h"

#include "StateManager.h"
#include "ResourceManager.h"

#include "Sprite.h"
#include "Helper.h"

#include "MenuState.h"
#include "BlendtestState.h"

GameEngine *GameEngine::instance = 0;

GameEngine::GameEngine()
{
	hasLoaded = false;
}

void GameEngine::Start()
{
	// Initialize game engine
	Initialize();

	// Load game engine
	Load();

	// Go to initial state
	StateManager::GetInstance()->QueueState(new MenuState());

	// Run game
	printf("Starting game loop...\n");
	GameLoop();
}

bool GameEngine::Initialize()
{
	printf("Initializing game engine...\n");

	// Initialize context
	Context::Initialize();

	// Initialize DevIL
	DevIL::Init();

	// Initialize FreeType
	FreeType::Init();

	printf("Your OpenGL version is %s\n", (const char*)glGetString(GL_VERSION));

	printf("Game engine initialized.\n");

	return true;
}

void GameEngine::Load()
{
	printf("\nLoading game engine:\n");

	printf("\n");
}

void GameEngine::GameLoop()
{
	// Set framerate cap
	GLdouble framerate = 999.0;
	GLdouble lastFrameTime = 0, currentFrameTime = 0, fpsLastUpdate = 0;
	gameRunning = true;

	// Main-loop
	while(gameRunning)
	{
		currentFrameTime = glfwGetTime();
		if ((currentFrameTime - lastFrameTime) * 1000.0 < 1000.0 / framerate)
		{
			glfwSleep(0.0001);
		}
		else
		{
			// Check if a state has been queued
			if (StateManager::GetInstance()->StateHasBeenQueued())
			{
				StateManager *sM = StateManager::GetInstance();
				ResourceManager * rM = ResourceManager::GetInstance();
				
				// Change state
				rM->UnloadAll();
				sM->ChangeState(sM->GetQueuedState());
				rM->StartLoading();
			}

			// Handle input
			HandleInput();

			// Update
			Update((currentFrameTime - lastFrameTime) * 1000.0);
			
			// Draw
			Draw();

			// update fps-counter
			GLdouble time = currentFrameTime;
			if (time - fpsLastUpdate >= 1.0)
			{
				GLdouble fps = ((currentFrameTime - lastFrameTime) * 1000.0);
				glfwSetWindowTitle(toString((int)((1000.0 / fps) * 100.0) / 100.0).c_str());
				fpsLastUpdate = time;
			}

			lastFrameTime = currentFrameTime;
		}
	}

	// Cleanup game if loop stops
	Cleanup();
}

void GameEngine::HandleInput()
{
	Input::HandleInput();
	
	if (!ResourceManager::GetInstance()->IsLoading())
	{
		StateManager::GetInstance()->HandleInput();
	}
}

// Update the engine
void GameEngine::Update(GLdouble time)
{
	Input::Update(time);
	Context::Update(time);

	if (ResourceManager::GetInstance()->IsLoading())
	{
		ResourceManager::GetInstance()->Update(time);
		if (hasLoaded)
			hasLoaded = false;
	}
	else
	{
		if (!hasLoaded)
		{
			StateManager::GetInstance()->Initialize();
			hasLoaded = true;
		}
		StateManager::GetInstance()->Update(time);
	}
}

// Draw the engine
void GameEngine::Draw()
{
	if (ResourceManager::GetInstance()->IsLoading())
	{
		ResourceManager::GetInstance()->Draw();
	}
	else
	{
		if (hasLoaded)
		{
			StateManager::GetInstance()->Draw();
		}
	}

	Context::Draw();
}

// Queue new state
void GameEngine::ChangeState(State *state)
{
	StateManager::GetInstance()->QueueState(state);
}

// End the game-loop
void GameEngine::StopGame()
{
	gameRunning = false;
}

// Cleanup the game
void GameEngine::Cleanup()
{
	StateManager::GetInstance()->Cleanup();
	ResourceManager::GetInstance()->Cleanup();
	Context::Cleanup();
}

// Get singleton instance
GameEngine* GameEngine::GetInstance()
{
	if (!instance)
	{
		instance = new GameEngine();
	}
	return instance;
}