#pragma once
#include <Windows.h>
#include <GL\glew.h>
#include "State.h"
#include "Sprite.h"

class GameEngine
{
public:
	GameEngine();

	static GameEngine *instance;
	static GameEngine *GetInstance();

	void Start();
	bool Initialize();
	void Load();
	void GameLoop();
	void StopGame();

	void ChangeState(State *state);

private:
	void HandleInput();
	void Update(GLdouble time);
	void Draw();
	void Cleanup();

	bool gameRunning;
	bool hasLoaded;
};