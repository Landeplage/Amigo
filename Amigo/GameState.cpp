#include "GameState.h"
#include "Input.h"
#include "GameEngine.h"
#include "MenuState.h"
#include "Helper.h"
#include <GL\glfw.h>

GameState::GameState() { }

GameState::~GameState()
{
	printf("\n::: GameState Exit :::\n\n");
}

void GameState::Init()
{
	printf("\n::: GameState Init...");

	// Create player
	player = Player();
	// Assign sprite to player
	player.SetSprite(&sprPlayer);
	// Set blend-mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	printf("Done:::\n\n");
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
	// Load test-sprite
	if (!sprTest.LoadImage("res//tile2.png")) return false;
	//sprTest.setOriginCenter();

	// Load debug-font
	font.LoadFont("res\\font\\robotob.ttf", 16);

	printf("Loading done.\n");
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

	// Draw a bunch of tiles
	Vec2 gridSize, gridOffset, tileSize;
	GLint gridAngle;
	gridSize = Vec2(15, 15);
	gridOffset = Vec2(1280 / 2, 0);
	tileSize = Vec2((GLfloat)sprTest.getWidth(), (GLfloat)sprTest.getHeight());
	gridAngle = 45;
	
	Vec2 mouse = Input::getMousePos();
	mouse.x = (GLfloat)(ldirX(mouse.x, (GLfloat)(-(gridAngle))) - ldirY(mouse.y, (GLfloat)(-gridAngle)));
	mouse.y = (GLfloat)(ldirY(mouse.x, (GLfloat)(-gridAngle)) + ldirX(mouse.y, (GLfloat)(-gridAngle)));
	sprTest.Draw((GLint)mouse.x, (GLint)mouse.y);

	GLint selected;
	selected = (GLint)((mouse.x - gridOffset.x) / tileSize.x) + (GLint)((mouse.y - gridOffset.y) / tileSize.y) * (GLint)gridSize.x;
	font.Draw(10, 10, toString(mouse.x) + " x " + toString(mouse.y));

	GLfloat a;
	for(GLint n = 0; n < (GLint)gridSize.x; n ++)
	{
		for(GLint i = 0; i < (GLint)gridSize.y; i ++)
		{
			a = 0.5f;
			if (selected == i + (n * (GLint)gridSize.x))
				a = 1.0f;
			sprTest.Draw(
				(GLint)(ldirX(i * tileSize.y, (GLfloat)gridAngle) + ldirX(n * tileSize.x, (GLfloat)(gridAngle + 90)) + gridOffset.x),
				(GLint)(ldirY(i * tileSize.y, (GLfloat)gridAngle) + ldirY(n * tileSize.x, (GLfloat)gridAngle + 90) + gridOffset.y),
				(GLfloat)gridAngle,
				1.0f,
				1.0f,
				a);
			font.Draw(
				(GLint)(ldirX(i * tileSize.y, (GLfloat)gridAngle) + ldirX(n * tileSize.x, (GLfloat)gridAngle + 90) + gridOffset.x + ldirX(tileSize.x / 2, (GLfloat)(gridAngle + 45)) - font.GetWidth(toString(i + (n * gridSize.x))) / 2),
				(GLint)(ldirY(i * tileSize.y, (GLfloat)gridAngle) + ldirY(n * tileSize.x, (GLfloat)gridAngle + 90) + gridOffset.y + ldirY(tileSize.x / 2, (GLfloat)(gridAngle + 45)) - font.GetHeight(toString(i + (n * gridSize.x))) / 4),
				toString(i + (n * gridSize.x)));
		}
	}
}