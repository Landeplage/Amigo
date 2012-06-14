#include "Context.h"
#include "Input.h"
#include "MenuState.h"
#include "GameState.h"
#include "Sprite.h"
#include "Helper.h"
#include "GameEngine.h"
#include "Font.h"
#include <string>

enum MENU
{
	MENU_MAIN,
	MENU_SETTINGS
};

MenuState::MenuState() {}

MenuState::~MenuState()
{
	printf("\n::: MenuState Exit :::\n\n");

	menuSystem->~MenuSystem();
}

void MenuState::Init()
{
	printf("\n::: Menustate init :::\n\n");

	screenW = Context::getWindowWidth();
	screenH = Context::getWindowHeight();
	cloudScroll = 0;
	checkerScroll = 0;
	keyEscPrevious = false;
}

bool MenuState::Load()
{
	// Load sprites
	if (!sprBackground.LoadImage("res\\tx\\back\\back_sky.png")) return false;
	if (!sprChequer.LoadImage("res\\tx\\back\\back_chequer2.png")) return false;
	if (!sprClouds.LoadImage("res\\tx\\back\\back_clouds.png")) return false;
	if (!sprCursor.LoadImage("res\\tx\\ui\\cursor.png")) return false;
	sprCursor.setOrigin(3, 1);
	if (!sprLogo.LoadImage("res\\tx\\ui\\gamelogo.png")) return false;
	sprLogo.setOriginCenter();
	if (!sprUI.LoadImage("res\\tx\\ui\\ui.png")) return false;
	sprUI.setInterpolationMode(GL_NEAREST);

	// Load fonts
	FontRegular.LoadFont("res\\font\\robotob.ttf", 16);
	FontBold.LoadFont("res\\font\\robotob.ttf", 24);

	// Initialize menu system
	menuSystem = new MenuSystem(&sprCursor, &sprUI, &FontBold, &FontRegular);
	
	// Add the main menu
	menuMain = menuSystem->AddMenu(0, 0, 1280, 720);
	menuMain->SetTransition(200, 0, 0.0f, false);

	// Some helper variables
	int centerX, centerY, yy, sep;
	centerX = screenW / 2;
	centerY = screenH / 2;
	yy = 0;
	sep = 36;

	// Main Menu
	yy = 400;
	menuMain->AddButton("Play", centerX - 100, yy, 200, 35, MenuItem::Align::CENTER, MENU_MAIN, [=]()
		{
			GameEngine::ChangeState(new GameState());
		}); yy += sep;
	menuMain->AddButton("Settings", centerX - 100, yy, 200, 35, MenuItem::Align::CENTER, MENU_MAIN, [=]()
		{
			menuMain->GoTo(MENU_SETTINGS);
		}); yy += sep;
	menuMain->AddButton("Map Editor", centerX - 100, yy, 200, 35, MenuItem::Align::CENTER, MENU_MAIN, [=]()
		{
			/**/
		}); yy += sep;
	menuMain->AddButton("Animator", centerX - 100, yy, 200, 35, MenuItem::Align::CENTER, MENU_MAIN, [=]()
		{
			/**/
		}); yy += sep;
	menuMain->AddButton("Quit", centerX - 100, screenH - 45, 200, 35, MenuItem::Align::CENTER, MENU_MAIN, [=]()
		{
			// Show quit-message
			menuSystem->ShowMessage("HA!", "Why would you ever want to quit this game? LOL @ YOU", []()
			{
				//GameEngine::StopGame();
			});
		}); yy += sep;

	// Settings
	menuMain->AddBox("Settings", centerX - 150, 250, 300, 250, MENU_SETTINGS);
	menuMain->AddButton("Back", centerX - 75, 515, 150, 35, MenuItem::Align::CENTER, MENU_SETTINGS, [=]()
		{
			menuMain->GoTo(MENU_MAIN);
		});

	// On draw-event for menuMain
	menuMain->OnDraw([&]()
	{
		// Logo
		GLfloat rot = menuSystem->GetRot();
		if (menuMain->GetMenuCurrent() == MENU_MAIN)
			sprLogo.Draw(Context::getWindowWidth() / 2 + lenDirX(30, rot * 2), 230 + lenDirY(10, rot), lenDirX(4, rot * 5), 1.0f, 1.0f, 1.0f);
	});

	// Add a little menu
	menuTiny = menuSystem->AddMenu(10, screenH - 190, 300, 180);
	menuTiny->SetTransition(0, 0, 0.0f, true);

	menuTiny->AddBox("Info", 10, 40, 280, 130, 0);
	menuTiny->AddButton("<", 20, 50, 50, 35, MenuItem::Align::CENTER, 0, [=](){ menuTiny->GoTo(1); });
	menuTiny->AddButton(">", 230, 50, 50, 35, MenuItem::Align::CENTER, 0, [=](){ menuTiny->GoTo(1); });
	
	menuTiny->AddBox("More info", 10, 40, 280, 130, 1);
	menuTiny->AddButton("<", 20, 50, 50, 35, MenuItem::Align::CENTER, 1, [=](){ menuTiny->GoTo(0); });
	menuTiny->AddButton(">", 230, 50, 50, 35, MenuItem::Align::CENTER, 1, [=](){ menuTiny->GoTo(0); });

	menuTiny->OnDraw([&]()
	{
		if (menuTiny->GetMenuCurrent() == 0)
			FontRegular.DrawLinebreak(20, 90, "Hello, this is just some text to show how this little box looks with a lot of text in it.", 240, 18);
		if (menuTiny->GetMenuCurrent() == 1)
			FontRegular.DrawLinebreak(20, 90, "Oh hey, this is some more text to show how different text looks. Did you see the transition effect?", 240, 18);
	});

	return true;
}

void MenuState::Update(GLdouble time)
{
	// Reset key-previous variable
	keyEscPrevious = false;

	// ESC to exit game
	if (Input::GetKey(GLFW_KEY_ESC) && !keyEscPrevious)
	{
		GameEngine::StopGame();
		keyEscPrevious = true;
	}

	// Cloud background scrolling
	cloudScroll += time;
	if (cloudScroll > 1280)
		cloudScroll -= 1280;
	
	// Checkered background scrolling
	GLfloat slide = menuMain->GetSlide();
	checkerScroll += time * 0.1f + abs(slide * 2.5f);
	if (checkerScroll > 200)
		checkerScroll -= 200;

	// Update menu-system (do last, or deal with checking
	// whether or not variables have been deleted from here
	// on out)
	menuSystem->Update(time);
}

void MenuState::Draw()
{
	GLfloat rot = menuSystem->GetRot();

	// Landscape background
	sprBackground.Draw(0, 0, 0.0f, Context::getWindowWidth(), 1.0f, 1.0f);

	// Clouds
	sprClouds.Draw(cloudScroll, 0);
	sprClouds.Draw(cloudScroll - 1280, 0);

	// Chequered background
	float bW, bH;
	bW = 200;
	bH = 200;
	for(int i = 0; i < (Context::getWindowWidth() / bW) + 2; i ++)
	{
		for(int n = 0; n < (Context::getWindowHeight() / bH) + 2; n ++)
		{
			sprChequer.Draw((int)(i * bW + lenDirX(30, rot * 4)) - (bW * 1) + checkerScroll - 200, (int)(n * bH + lenDirY(100, rot * 3)) - (bH * 1), 0.0f, 1.0f, 1.0f, 0.25f);
		}
	}

	// Draw menu-system
	menuSystem->Draw();
}