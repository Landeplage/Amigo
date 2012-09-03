#include "Context.h"
#include "Input.h"
#include "MenuState.h"
#include "GameState.h"
#include "Sprite.h"
#include "Helper.h"
#include "GameEngine.h"
#include "Font.h"
#include <string>

#include "Slider.h"
#include "SliderRange.h"

#include "Helper.h"

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
	checkerScroll = 0;
	keyEscPrevious = false;
}

bool MenuState::Load()
{
	// Load sprites
	if (!sprBack1.LoadImage("res\\tx\\ui\\menu_back1.png")) return false; // top
	if (!sprBack2.LoadImage("res\\tx\\ui\\menu_back2.png")) return false; // middle
	if (!sprBack3.LoadImage("res\\tx\\ui\\menu_back3.png")) return false; // bottom
	if (!sprChequer.LoadImage("res\\tx\\back\\back_chequer3.png")) return false;
	if (!sprCursor.LoadImage("res\\tx\\ui\\cursor.png")) return false;
	sprCursor.setOrigin(3, 1);
	if (!sprLogo.LoadImage("res\\tx\\ui\\gamelogo.png")) return false;
	sprLogo.setOriginCenter();
	if (!sprUI.LoadImage("res\\tx\\ui\\ui2.png")) return false;
	sprUI.setInterpolationMode(GL_NEAREST);
	if (!sprUI2.LoadImage("res\\tx\\ui\\ui2.png")) return false;
	sprUI2.setInterpolationMode(GL_NEAREST);

	// Load fonts
	FontRegular.LoadFont("res\\font\\futura_regular.ttf", 14);
	FontBold.LoadFont("res\\font\\futura_heavy.ttf", 14);

	// Initialize menu system
	menuSystem = new MenuSystem(&sprCursor, &sprUI, &FontBold, &FontRegular);
	
	// Add the main menu
	menuMain = menuSystem->AddMenu(0, 0, 1280, 720);
	menuMain->SetTransition(25, 0, 0.0f, false);

	// Some helper variables
	int centerX, centerY, yy, sep;
	centerX = screenW / 2;
	centerY = screenH / 2;
	yy = 0;
	sep = 36;

	// Main Menu
	yy = 400;
	menuMain->AddButton("Play", centerX - 51, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Start the game. If only this textbox didn't have all this text in it. It would be much, much smaller. Tiny, compared to this monstrosity of a textbox. LOLS",
		[=]()
		{
			GameEngine::ChangeState(new GameState());
		}); yy += sep;
	menuMain->AddButton("Settings", centerX - 51, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Set your preferences.",
		[=]()
		{
			menuMain->GoTo(MENU_SETTINGS);
		}); yy += sep;
	menuMain->AddButton("Map Editor", centerX - 51, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Create maps and share them online.",
		[=]()
		{
			menuSystem->ShowMessage("Map Editor", "Dat ain't done yet.");
		}); yy += sep;
	menuMain->AddButton("Animator", centerX - 51, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Launch the skeletal animation editor.",
		[=]()
		{
			menuSystem->ShowMessage("Animator", "Dat ain't done yet.");
		}); yy += sep;

	menuMain->AddButton("Quit", centerX - 51, screenH - 45, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Close the game and return to windows.",
		[=]()
		{
			// Show quit-message
			menuSystem->ShowQuestion("Quit", "Sucka.",
			[]()
			{
				// Yes-button
				GameEngine::StopGame();
			},
			[]()
			{
				// No-button
			});
		}); yy += sep;

	// Settings
	GLint space = 10;
	Box *settingsBox;
	settingsBox = (Box*)menuMain->AddBox("Settings", centerX - 150, 250, 300, 250, MENU_SETTINGS);

	Slider *slider;
	slider = (Slider*)menuMain->AddSlider("Slider", Vec2(settingsBox->GetPosition().x + space, settingsBox->GetPosition().y + space), (GLint)(settingsBox->GetSize().x - space * 2), 1.0f, 100.0f, 1.0f, MENU_SETTINGS, [](){});
	//slider->SetValue(0.0f);

	Slider *sliderRange;
	sliderRange = (SliderRange*)menuMain->AddSliderRange("Range slider", Vec2(slider->GetPosition().x, slider->GetPosition().y + slider->GetSize().y + space),
		(GLint)slider->GetSize().x,
		10.0f,
		20.0f, 1, MENU_SETTINGS, [](){});
	sliderRange->SetValue(0.0f);

	menuMain->AddButton("Back", centerX - 75, 515, 150, 35, MenuItem::CENTER, MENU_SETTINGS, "Go back to the main menu.", [=]()
		{
			menuMain->GoTo(MENU_MAIN);
		});

	// On draw-event for Main Menu
	menuMain->OnDraw([&]()
	{
		// Logo
		GLfloat rot = menuSystem->GetRot();
		if (menuMain->GetMenuCurrent() == MENU_MAIN)
			sprLogo.Draw((GLint)(Context::getWindowWidth() / 2 + ldirX(30, rot * 2)), (GLint)(230 + ldirY(10, rot)), (GLfloat)(ldirX(4, rot * 5)), 0.8f, 0.8f, 1 - abs(menuMain->GetSlide()));
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
	
	// Checkered background scrolling
	GLfloat slide = menuMain->GetSlide();
	checkerScroll += (GLfloat)time * 0.02f + abs(slide * 1.0f);
	if (checkerScroll > 100)
		checkerScroll -= 100;

	// Update menu-system (do last, or deal with checking
	// whether or not variables have been deleted from here
	// on out)
	menuSystem->Update(time);
}

void MenuState::Draw()
{
	GLfloat rot = menuSystem->GetRot();

	// Draw top and bottom background
	sprBack1.Draw(0, 0);
	sprBack3.Draw(0, 652);

	// Chequered background
	GLfloat bW, bH;
	bW = 100;
	bH = 100;
	for(GLint i = 0; i < (Context::getWindowWidth() / bW) + 2; i ++)
	{
		for(GLint n = 0; n < (Context::getWindowHeight() / bH) + 2; n ++)
		{
			sprChequer.Draw((GLint)(i * bW + ldirX(30, rot * 2) - (bW * 1) + checkerScroll - 100), (GLint)(n * bH + ldirY(100, rot * 3) - (bH * 1)), 0.0f, 1.0f, 1.0f, 0.035f);
		}
	}

	// Draw the middle part of the background
	sprBack2.Draw(0, 85);

	// Draw menu-system
	menuSystem->Draw();

	// Testing drawing new UI
	GLint x, y, w, h;
	x = 200;
	y = 200;
	w = 200;
	h = 100;

	// Draw something here
	sprUI2.Draw(105, 100 + 78 / 2 - 5, 0.0f, 8.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 8, 31, 1, 10); // Tødler på scroller-knapp
	
	// Rectangle
	//sprUI2.Draw(x, y, 0.0f, (GLfloat)w, (GLfloat)h, 1.0f, 1.0f, 1.0f, 0.7f, 5, 5, 1, 1);
}