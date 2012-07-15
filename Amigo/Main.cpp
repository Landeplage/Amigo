#include "GameEngine.h"
#include "TestState.h"

int main(int argc, char* argv[])
{
	// Initialize game
	if (!GameEngine::Initialize(argc, argv))
	{
		printf("Game failed to initialize!\n");
		return 0;
	}

	// Go to first state
	GameEngine::ChangeState(new TestState(), false);

	// Run game
	GameEngine::GameLoop();

	return 0;
}