#include "ErrorHandler.h"
#include "GameEngine.h"
#include "MenuState.h"

int main(int argc, char* argv[])
{
	try
	{
		// Initialize game
		GameEngine::Initialize(argc, argv);

		// Go to first state
		GameEngine::ChangeState(new MenuState());

		// Run game
		GameEngine::GameLoop();
	}
	catch(int error)
	{
		ErrorHandler::Report(error);
	}
	catch(std::string error)
	{
		ErrorHandler::Report(error);
	}

	return 0;
}