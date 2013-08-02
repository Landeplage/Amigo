#include "ErrorHandler.h"
#include "GameEngine.h"

int main(int argc, char* argv[])
{
	try
	{
		GameEngine::GetInstance()->Start();
	}
	catch(std::string error)
	{
		ErrorHandler::Report(error);
	}

	return 0;
}