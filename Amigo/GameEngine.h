#pragma once
#include <GL\glew.h>
#include "State.h"
#include "Sprite.h"

class GameEngine
{
public:
	static void ChangeState(State *state);
	static void ChangeState(State *state, bool showLoading);
	static void Start();
	static void StopGame();
	
	static bool Initialize();
	static void GameLoop();
	static void Update(GLdouble time);
	static void Draw();
	static void Cleanup();
private:
	static void Load();
	static void GoToState(State *state);

	static Sprite sprLoading;
	static bool isLoading, initLoad, gameRunning;
	static GLfloat loadingRot;
	static State *newState;
};