#include "GameState.h"
#include "Input.h"
#include "GameEngine.h"
#include "MenuState.h"
#include <GL\glfw.h>

GameState::GameState() { }

GameState::~GameState()
{
	printf("\n::: GameState Exit :::\n\n");
}

void GameState::Init()
{
	printf("\n::: GameState Init :::\n\n");

	// Create player
	player = Player();
	// Assign sprite to player
	player.SetSprite(&sprPlayer);
}

bool GameState::Load()
{
	printf("GameState Load...\n");

	// Load background-image
	if (!sprBackground.LoadImage("res//tx//back//back_1.png")) return false;
	// Load player-sprite
	if (!sprGround.LoadImage("res//tx//env//ground_graphic.png")) return false;
	// Load player-sprite
	if (!sprPlayer.LoadImage("res//hamster.png")) return false;

	return true;
}

void GameState::Update(GLdouble time)
{
	// Exit to menu with ESC
	if (Input::GetKey(GLFW_KEY_ESC))
	{
		GameEngine::ChangeState(new MenuState());
	}

	// Update player
	player.Update(time);
}

void GameState::Draw()
{
	// Draw background
	sprBackground.Draw(0, 0);

	// Draw ground
	sprGround.Draw(0, 720 - 191);

	// Draw player
	player.Draw();
}