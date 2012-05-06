#pragma once

#include <GLTools.h>
#include "GameState.h"
#include "Sprite.h"

static class GameEngine
{
public:
	static void ChangeState(GameState *gameState);
	static void ChangeState(GameState *gameState, bool showLoading);
	static void StopGame();
	
	static bool Initialize(int argc, char* argv[]);
	static void GameLoop();
	static void Update(GLdouble time);
	static void Draw();
	static void Cleanup();
private:
	static void Load();

	static Sprite sprLoading;
	static bool isLoading, gameRunning;
	static GLfloat loadingRot;
};