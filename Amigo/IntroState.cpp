#include "Context.h"
#include "IntroState.h"
#include "MenuState.h"
#include "Sprite.h"
#include "Helper.h"
#include "GameEngine.h"

IntroState::IntroState() { }

IntroState::~IntroState()
{
	printf("\n::: Introstate Exit :::\n\n");
}

void IntroState::Init()
{
	// Initialize timer until we go to the menu-state
	timerWait = 200;

	finishedLoading = false;
	printf("\n::: Introstate Init :::\n\n");
}

bool IntroState::Load()
{
	if (timerWait == 200)
	{
		sprLogo.LoadImage("res\\tx\\devlogo.png");
		sprLogo.setOriginCenter();
	}

	// We're pretending that loading takes a lot of time
	timerWait--;
	if (timerWait > 240)
	{
		return false;
	}

	return true;
}

void IntroState::Update(GLdouble time)
{
	if (timerWait > 0)
		timerWait --;
	else
		GameEngine::ChangeState(new MenuState());
}

void IntroState::Draw()
{
	sprLogo.Draw(Context::getWindowWidth() / 2, Context::getWindowHeight() / 2 - 50, 0.0f, 1.0f, 1.0f, (float)timerWait * 0.05f);
}