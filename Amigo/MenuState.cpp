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
#include "Color.h"

enum MENU
{
	MENU_SPLASH,
	MENU_MAIN,
	MENU_OPTIONS,
	MENU_DEEP,
	MENU_EVEN_DEEPER
};

const char* menuName[] =
{
	"Splash",
	"Main menu",
	"Options",
	"Deeper!",
	"Even deeper!"
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
	sprBack1.LoadImage("res\\tx\\ui\\menu_back1.png"); // top
	sprBack2.LoadImage("res\\tx\\ui\\menu_back2.png"); // middle
	sprBack3.LoadImage("res\\tx\\ui\\menu_back3.png"); // bottom
	sprChequer.LoadImage("res\\tx\\back\\back_chequer3.png");
	sprCursor.LoadImage("res\\tx\\ui\\cursor.png");
		sprCursor.setOrigin(3, 1);
	sprLogo.LoadImage("res\\tx\\ui\\gamelogo.png");
		sprLogo.setOriginCenter();
	sprLogoSmall.LoadImage("res\\tx\\ui\\gamelogo_small.png");
		sprLogoSmall.setOriginCenter();
	sprUI.LoadImage("res\\tx\\ui\\ui2.png");
		sprUI.setInterpolationMode(GL_NEAREST);

	// Load fonts
	FontRegular.LoadFont("res\\font\\futura_regular.ttf", 14);
	FontBold.LoadFont("res\\font\\futura_heavy.ttf", 14);
	fontBig.LoadFont("res\\font\\futura_heavy.ttf", 24);

	// Initialize menu system
	menuSystem = new MenuSystem(&sprCursor, &sprUI, &FontBold, &FontRegular);
	
	// Add the main menu
	menuMain = menuSystem->AddMenu(0, 0, 1280, 720, MENU_SPLASH);
	menuMain->SetTransition(0, 0, 0.0f, false);

	// Some helper variables
	int centerX, centerY, xx, yy, sep;
	centerX = screenW / 2;
	centerY = screenH / 2;
	xx = 0;
	yy = 0;
	sep = 32;
	MenuItem *box, *button;

	// Splash screen
	menuMain->AddButton("Yup.", centerX - 51, centerY + 200, 102, 28, MenuItem::CENTER, MENU_SPLASH,
		"Yep.",
		[=]()
		{
			menuMain->GoTo(MENU_MAIN);
		});

	// Main Menu
	box = menuMain->AddBox("Menu", centerX - 330, centerY - 150, 130, 305, MENU_MAIN);
	xx = (GLint)(box->GetPosition().x + box->GetSize().x / 2) - 51;
	yy = (GLint)box->GetPosition().y + 15;
	menuMain->AddButton("Create game", xx, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Create an online session.",
		[=]()
		{
			GameEngine::ChangeState(new GameState());
		}); yy += sep;
	button = menuMain->AddButton("Join game", xx, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Join the game you've selected to the right.",
		[=]()
		{
			menuSystem->ShowMessage("Join game", "This is where one would normally join a game...");
		}); yy += sep * 2;
	button->active = false;
	menuMain->AddButton("Options", xx, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Tinker with audio, video and controller settings.",
		[=]()
		{
			menuMain->GoTo(MENU_OPTIONS);
		}); yy += sep;
	menuMain->AddButton("My hamster", xx, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Edit your hamster and write about it's life.",
		[=]()
		{
			menuSystem->ShowMessage("My hamster", "This is where one would customize one's hamster.");
		}); yy += sep;
	menuMain->AddButton("Map editor", xx, yy, 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Create your own map.",
		[=]()
		{
			menuSystem->ShowMessage("Map editor", "Lorem Ipsum er rett og slett dummytekst fra og for trykkeindustrien. Lorem Ipsum har vært bransjens standard for dummytekst helt siden 1500-tallet, da en ukjent boktrykker stokket en mengde bokstaver for å lage et prøveeksemplar av en bok. Lorem Ipsum har tålt tidens tann usedvanlig godt, og har i tillegg til å bestå gjennom fem århundrer også tålt spranget over til elektronisk typografi uten vesentlige endringer. Lorem Ipsum ble gjort allment kjent i 1960-årene ved lanseringen av Letraset-ark med avsnitt fra Lorem Ipsum, og senere med sideombrekkingsprogrammet Aldus PageMaker som tok i bruk nettopp Lorem Ipsum for dummytekst.");
		}); yy += sep;

	menuMain->AddButton("Quit", xx, (GLint)(box->GetPosition().y + box->GetSize().y - 43), 102, 28, MenuItem::CENTER, MENU_MAIN,
		"Close the game and return to windows.",
		[=]()
		{
			// Show quit-message
			menuSystem->ShowQuestion("Quit", "You sure about that?",
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

	menuMain->AddBox("Games in progress", (GLint)box->GetPosition().x + (GLint)box->GetSize().x + 25, (GLint)box->GetPosition().y,
		(GLint)(centerX - box->GetPosition().x - ((box->GetPosition().x + box->GetSize().x + 25) - centerX)), //auto-widen the box so that it all fits nicely in the middle of the screen
		(GLint)box->GetSize().y, MENU_MAIN); // games-in-progress box

	// Options
	GLint space = 10;
	Box *settingsBox;
	settingsBox = (Box*)menuMain->AddBox("Check out these sweet sliders, man", centerX - 150, 250, 300, 250, MENU_OPTIONS);

	Slider *slider;
	slider = (Slider*)menuMain->AddSlider("Slider", Vec2(settingsBox->GetPosition().x + space, settingsBox->GetPosition().y + space), (GLint)(settingsBox->GetSize().x - space * 2), 1.0f, 100.0f, 1.0f, MENU_OPTIONS, [](){});
	//slider->SetValue(0.0f);

	Slider *sliderRange;
	sliderRange = (SliderRange*)menuMain->AddSliderRange("Range slider", Vec2(slider->GetPosition().x, slider->GetPosition().y + slider->GetSize().y + space),
		(GLint)slider->GetSize().x,
		10.0f,
		20.0f, 1, MENU_OPTIONS, [](){});
	sliderRange->SetValue(0.0f);

	menuMain->AddButton("Go deeper...", centerX - 51, 400, 102, 28, MenuItem::CENTER, MENU_OPTIONS, "Menuception!", [=]()
		{
			menuMain->GoTo(MENU_DEEP);
		});

	menuMain->AddButton("Accept", centerX - 51 - 55, 515, 102, 28, MenuItem::CENTER, MENU_OPTIONS, "Accept these changes.", [=]()
		{
			menuMain->GoToPrevious();
		});
	menuMain->AddButton("Cancel", centerX - 51 + 55, 515, 102, 28, MenuItem::CENTER, MENU_OPTIONS, "Cancel these changes and go back.", [=]()
		{
			menuMain->GoToPrevious();
		});

	// DEEP-MENU
	menuMain->AddButton("DEEPER.", centerX - 51, 350, 132, 35, MenuItem::CENTER, MENU_DEEP, "omg", [=]()
		{
			menuMain->GoTo(MENU_EVEN_DEEPER);
		});
	menuMain->AddButton("Back", centerX - 51, 400, 102, 28, MenuItem::CENTER, MENU_DEEP, "Shiiiiet. Go back!", [=]()
		{
			menuMain->GoToPrevious();
		});

	// EVEN DEEPER MENU
	menuMain->AddButton("Back", centerX - 51, 500, 102, 28, MenuItem::CENTER, MENU_EVEN_DEEPER, ":O", [=]()
		{
			menuMain->GoToPrevious();
		});

	// On draw-event for Main Menu
	menuMain->OnDraw([&]()
	{
		// Logo
		GLfloat rot = menuSystem->GetRot();
		if (menuMain->GetMenuCurrent() == MENU_SPLASH)
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
	checkerScroll +=  (0.015f + abs(slide * 1.0f)) * (GLfloat)time;
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

	// Draw the small logo at the top
	if (menuMain->GetMenuCurrent() != MENU_SPLASH)
	{
		sprLogoSmall.Draw(Context::getWindowWidth() / 2, 50);
	}

	// Draw breadcrumbs in top left corner
	GLint x, y, w, h;
	float alpha;
	const char* str;
	str = "n/a";
	alpha = 1.0f;
	x = 60;
	y = 35;
	w = 0;
	h = 100;

	for(int i = 0; i < MENU_HISTORY_LENGTH; i ++)
	{
		if (menuMain->GetHistory(i) >= 0)
		{
			str = menuName[menuMain->GetHistory(i)];
			w += FontRegular.GetWidth(str) + 30;
		}
	}

	// Breadcrumb back
	if (w > 0)
	{
		sprUI.Draw(x, y, 0.0f, 1.0f, 1.0f, alpha, 0, 77, 5, 27); // Left
		sprUI.Draw(x + 5, y, 0.0f, w - 17, 1.0f, alpha, 5, 77, 1, 27); // Mid
		sprUI.Draw(x + w - 12, y, 0.0f, 1.0f, 1.0f, alpha, 6, 77, 12, 27); // Right
	}

	// Breadcrumb text
	w = 0;
	for(int i = MENU_HISTORY_LENGTH - 1; i >= 0; i --)
	{
		if (menuMain->GetHistory(i) >= 0)
		{
			str = menuName[menuMain->GetHistory(i)];
			FontRegular.Draw(x + 12 + w, y + 4, str, 0.0f, 1.0f, 1.0f, Color(255, 255, 255), 0.5f * alpha);
			FontRegular.Draw(x + 12 + w, y + 3, str, 0.0f, 1.0f, 1.0f, Color(77, 77, 77), alpha);
			w += FontRegular.GetWidth(str) + 30;
			if (i > 0) // Draw a separator
			{
				if (menuMain->GetHistory(i - 1) >= 0)
				{
					sprUI.Draw(x + w - 12, y, 0.0f, 1.0f, 1.0f, alpha, 18, 77, 12, 23); // Separator
				}
			}
		}
	}

	alpha = 1 - abs(menuMain->GetSlide());

	int i = 0;
	if (menuMain->GetMenuGoTo() != menuMain->GetHistory(0)) // going to new submenu
	{
		if (menuMain->GetMenuCurrent() != menuMain->GetMenuGoTo())
			i = 1; // out
		else
			i = -1; // in
	}
	else // going back to the previous menu
	{
		if (menuMain->GetMenuCurrent() != menuMain->GetMenuGoTo())
			i = -1; // out
	}
	x += EaseQuadIn(abs(menuMain->GetSlide())) * 50 * i;
	
	fontBig.Draw(x + w + 8, y, menuName[menuMain->GetMenuCurrent()], 0.0f, 1.0f, 1.0f, Color(0, 0, 0), 0.25f * alpha); // Shadow
	fontBig.Draw(x + w + 7, y - 2, menuName[menuMain->GetMenuCurrent()], 0.0f, 1.0f, 1.0f, Color(255, 255, 255), 1.0f * alpha); // Draw current menu

	// Draw menu-system
	menuSystem->Draw();

	// Draw something here
	//sprUI.Draw(105, 100 + 78 / 2 - 5, 0.0f, 8.0f, 1.0f, 1.0f, 8, 31, 1, 10); // Tødler på scroller-knapp

	// Debug
	FontRegular.Draw(30, 150, "Debug", 0.0f, 1.0f, 1.0f, Color(0, 0, 0), 1.0f);
}