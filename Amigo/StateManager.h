#pragma once
#include "State.h"

class StateManager
{
public:
	StateManager();

	static StateManager *instance;
	static StateManager *GetInstance();

	void Initialize();
	void Update(GLdouble time);
	void Draw();
	void Cleanup();

	bool StateHasBeenQueued();
	void QueueState(State* state);
	void ChangeState(State* state);

	State *GetCurrentState();
	State *GetQueuedState();

private:
	State *currentState, *queuedState;
};