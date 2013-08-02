#include "StateManager.h"

StateManager* StateManager::instance = 0;

StateManager::StateManager()
{
	currentState = 0;
	queuedState = 0;
}

void StateManager::Initialize()
{
	currentState->Init();
}

void StateManager::Update(GLdouble time)
{
	currentState->Update(time);
}

void StateManager::Draw()
{
	currentState->Draw();
}

void StateManager::Cleanup()
{
	currentState->~State();
}

void StateManager::QueueState(State* state)
{
	queuedState = state;
}

bool StateManager::StateHasBeenQueued()
{
	// if a new state is queued, go to queued state
	if (queuedState != currentState)
	{
		return true;
	}

	return false;
}

// Cleanup current state, unload resources, initialize new state, and load new resources.
void StateManager::ChangeState(State* state)
{
	if (currentState)
	{
		delete currentState;
	}
	currentState = state;
	
	currentState->AddResources();
}

StateManager *StateManager::GetInstance()
{
	if (!instance)
	{
		instance = new StateManager();
	}
	return instance;
}

State *StateManager::GetCurrentState()
{
	return currentState;
}

State *StateManager::GetQueuedState()
{
	return queuedState;
}