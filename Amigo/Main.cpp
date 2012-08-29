#include "GameEngine.h"
#include "MenuState.h"

int main(int argc, char* argv[])
{
	// Initialize game
	if (!GameEngine::Initialize(argc, argv))
	{
		printf("Game failed to initialize!\n");
		return 0;
	}

	// Go to first state
	GameEngine::ChangeState(new MenuState());

	// Run game
	GameEngine::GameLoop();

	return 0;
}