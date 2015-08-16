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
#include "Dropdown.h"
#include "InputField.h"
#include "Text.h"
#include "Checkbox.h"
#include "RadioBox.h"
#include "ListBox.h"

#include "Helper.h"
#include "Color.h"

#include "ResourceManager.h"

#include "Vec4.h"

#include "ContentBox.h" // testing

enum MENU
{
	MENU_SPLASH,
	MENU_MAIN,
	MENU_OPTIONS,
};

const char* menuName[] =
{
	"Splash",
	"Main menu",
	"Options",
};

MenuState::MenuState()
{
	// Init variables
	screenW = Context::getWindowWidth();
	screenH = Context::getWindowHeight();
	checkerScroll = 0;
	keyEscPrevious = false;

	// Init fonts
	fontBold = new Font();
	fontRegular = new Font();
	fontBig = new Font();

	// Init sprites
	sprUI = new Sprite();
	sprLogo = new Sprite();
	sprLogoSmall = new Sprite();
	sprCursor = new Sprite();
	sprBack1 = new Sprite();
	sprBack2 = new Sprite();
	sprBack3 = new Sprite();
	sprChequer = new Sprite();

	// Init menu system
	menuSystem = new MenuSystem(sprCursor, sprUI, fontBold, fontRegular);
}

MenuState::~MenuState()
{
	printf("\n::: MenuState Exit :::\n\n");

	menuSystem->~MenuSystem();
}

void MenuState::AddResources()
{
	ResourceManager* rM = ResourceManager::GetInstance();

	// Add sprites to be loaded
	rM->AddSprite(sprUI, "res\\tx\\ui\\ui2.png");
	rM->AddSprite(sprLogo, "res\\tx\\ui\\gamelogo.png");
	rM->AddSprite(sprLogoSmall, "res\\tx\\ui\\gamelogo_small.png");
	rM->AddSprite(sprCursor, "res\\tx\\ui\\cursor.png");
	rM->AddSprite(sprBack1, "res\\tx\\ui\\menu_back1.png");
	rM->AddSprite(sprBack2, "res\\tx\\ui\\menu_back2.png");
	rM->AddSprite(sprBack3, "res\\tx\\ui\\menu_back3.png");
	rM->AddSprite(sprChequer, "res\\tx\\back\\back_chequer3.png");

	// Add fonts to be loaded
	rM->AddFont(fontRegular, "res\\font\\futura_regular.ttf", 14);
	rM->AddFont(fontBold, "res\\font\\futura_heavy.ttf", 14);
	rM->AddFont(fontBig, "res\\font\\futura_heavy.ttf", 24);
}

void MenuState::Init()
{
	printf("\n::: Menustate init :::\n\n");

	// Set sprite properties
	sprUI->setInterpolationMode(GL_NEAREST);
	sprLogo->setOriginCenter();
	sprLogoSmall->setOriginCenter();
	sprCursor->setOrigin(3, 1);

	// Create the game's main menu
	CreateMenu();
}

void MenuState::HandleInput()
{
	// ESC to exit game
	if (Input::GetKey(GLFW_KEY_ESCAPE))
	{
		if (!keyEscPrevious)
		{
			//menuSystem->QueueMessage("Massage", "This is a massage."); // testing various situations where a message appears
			//GameEngine::GetInstance()->StopGame();
		}
		keyEscPrevious = true;
	}
	else
	{
		// Reset key-previous variable
		keyEscPrevious = false;
	}

	// Testing resizing of contentbox
	if (Input::getMouseRight())
	{
		Vec2 mouse = Input::getMousePos();
		if (menuMain->GetMenuCurrent() == MENU_MAIN)
			gameListBox->RemoveItem(gameListBox->GetSelectedItem());
		if (menuMain->GetMenuCurrent() == MENU_SPLASH)
			cb->SetSize(mouse - cb->GetPosition()); // <-- Debug
	}

	// Handle menu input
	menuSystem->HandleInput();
}

void MenuState::Update(GLdouble time)
{	
	// Checkered background scrolling
	checkerScroll +=  0.0f; //(GLfloat)(time / 100);
	if (checkerScroll > 100)
		checkerScroll -= 100;
	else
		if (checkerScroll < 0)
			checkerScroll += 100;

	// Update the join-game button
	if (menuMain->GetMenuCurrent() == MENU_MAIN)
	{
		if (gameListBox->GetSelectedItem() == NULL)
		{
			joinGameButton->active = false;
		}
		else
		{
			joinGameButton->active = true;
		}
	}

	// Update menu-system (do last, or deal with checking
	// whether or not variables have been deleted from here
	// on out)
	menuSystem->Update(time);
}

void MenuState::Draw()
{
	GLfloat rot = menuSystem->GetRot();

	// Draw top and bottom background
	sprBack1->Draw(Vec2(0, 0));
	sprBack3->Draw(Vec2(0, 652));

	// Chequered background
	GLfloat bW, bH;
	bW = 100;
	bH = 100;
	int n = 1;
	if (menuMain->GetMenuCurrent() != menuMain->GetMenuGoTo())
		n = -1;
	GLfloat slide = EaseQuadIn(abs(menuMain->GetSlide())) * n * 200;
	while (slide > 100)
		slide -= 100;
	while (slide < 0)
			slide += 100;
	for(GLint i = 0; i < (Context::getWindowWidth() / bW) + 2; i ++)
	{
		for(GLint n = 0; n < (Context::getWindowHeight() / bH) + 2; n ++)
		{
			//sprChequer.Draw((GLint)(i * bW - (bW * 1) + slide + checkerScroll - 100), (GLint)(n * bH - (bH * 1)), 0.0f, 1.0f, 1.0f, 0.03f);
			sprChequer->Draw(Vec2((GLint)(i * bW + ldirX(30, rot * 2) - (bW * 1) - slide), (GLint)(n * bH + ldirY(20, rot * 3) - (bH * 1))), 0.0f, Vec2(1.0f, 1.0f), 0.03f);
		}
	}

	// Draw the middle part of the background
	sprBack2->Draw(Vec2(0, 85));

	// Draw the small logo at the top
	if (menuMain->GetMenuCurrent() != MENU_SPLASH)
	{
		sprLogoSmall->Draw(Vec2(Context::getWindowWidth() / 2, 50));
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
			w += fontRegular->GetWidth(str) + 30;
		}
	}

	// Breadcrumb back
	if (w > 0)
	{
		sprUI->Draw(Vec2(x, y), 0.0f, Vec2(1.0f, 1.0f), alpha, 0, 77, 5, 27); // Left
		sprUI->Draw(Vec2(x + 5, y), 0.0f, Vec2(w - 17, 1.0f), alpha, 5, 77, 1, 27); // Mid
		sprUI->Draw(Vec2(x + w - 12, y), 0.0f, Vec2(1.0f, 1.0f), alpha, 6, 77, 12, 27); // Right
	}

	// Breadcrumb text
	w = 0;
	for(int i = MENU_HISTORY_LENGTH - 1; i >= 0; i --)
	{
		if (menuMain->GetHistory(i) >= 0)
		{
			str = menuName[menuMain->GetHistory(i)];
			fontRegular->Draw(Vec2(x + 12 + w, y + 4), str, 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 0.5f * alpha);
			fontRegular->Draw(Vec2(x + 12 + w, y + 3), str, 0.0f, Vec2(1.0f, 1.0f), Color(77, 77, 77), alpha);
			w += fontRegular->GetWidth(str) + 30;
			if (i > 0) // Draw a separator
			{
				if (menuMain->GetHistory(i - 1) >= 0)
				{
					sprUI->Draw(Vec2(x + w - 12, y), 0.0f, Vec2(1.0f, 1.0f), alpha, 18, 77, 12, 23); // Separator
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
	
	fontBig->Draw(Vec2(x + w + 8, y), menuName[menuMain->GetMenuCurrent()], 0.0f, Vec2(1.0f, 1.0f), Color(0, 0, 0), 0.25f * alpha); // Shadow
	fontBig->Draw(Vec2(x + w + 7, y - 2), menuName[menuMain->GetMenuCurrent()], 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 1.0f * alpha); // Draw current menu

	// Draw menu-system
	menuSystem->Draw();

	// Debug
	fontRegular->Draw(Vec2(10, 5), "focus = " + toString((GLint)menuSystem->GetFocus()), 0.0f, Vec2(1.0f, 1.0f), Color(0, 0, 0), 0.2f);
}

void MenuState::CreateMenu()
{
	// Add the main menu
	menuMain = menuSystem->AddMenu(0, 0, 1280, 720, MENU_SPLASH);
	menuMain->SetTransition(0, 0, 0.0f);

	// Some helper variables
	int centerX, centerY, xx, yy, sep;
	centerX = screenW / 2;
	centerY = screenH / 2;
	xx = 0;
	yy = 0;
	sep = 32;

	// Splash screen
	menuMain->AddItem(new Button(menuSystem, "Yup.", Vec2(centerX - 51, centerY + 200), Vec2(102, 28), MenuItem::CENTER, MENU_SPLASH,
		"Yep.",
		[=]()
		{
			menuMain->GoTo(MENU_MAIN);
		}));

	// Main Menu
	Box *mainBox;
	mainBox = (Box*)menuMain->AddItem(new Box(menuSystem, "Menu", centerX - 330, centerY - 150, 130, 305, MENU_MAIN));
	xx = (GLint)(mainBox->GetPosition().x + mainBox->GetSize().x / 2) - 51;
	yy = (GLint)mainBox->GetPosition().y + 15;
	menuMain->AddItem(new Button(menuSystem, "Create game", Vec2(xx, yy), Vec2(102, 28), MenuItem::CENTER, MENU_MAIN,
		"Create an online session.",
		[=]()
		{
			GameEngine::GetInstance()->ChangeState(new GameState());
		})); yy += sep;
	joinGameButton = (Button*)menuMain->AddItem(new Button(menuSystem, "Join game", Vec2(xx, yy), Vec2(102, 28), MenuItem::CENTER, MENU_MAIN,
		"Join the game you've selected in the game-list.",
		[=]()
		{
			menuSystem->QueueMessage("Join game", "This is where one would normally join a game...");
		})); yy += sep * 2;
	joinGameButton->active = false;
	menuMain->AddItem(new Button(menuSystem, "Options", Vec2(xx, yy), Vec2(102, 28), MenuItem::CENTER, MENU_MAIN,
		"Tinker with audio, video and controller settings.",
		[=]()
		{
			menuMain->GoTo(MENU_OPTIONS);
		})); yy += sep;
	menuMain->AddItem(new Button(menuSystem, "My hamster", Vec2(xx, yy), Vec2(102, 28), MenuItem::CENTER, MENU_MAIN,
		"Edit your hamster and fantasize about its life.",
		[=]()
		{
			menuSystem->QueueMessage("My hamster", "This is where one would customize one's hamster.");
		})); yy += sep;
	menuMain->AddItem(new Button(menuSystem, "Map editor", Vec2(xx, yy), Vec2(102, 28), MenuItem::CENTER, MENU_MAIN,
		"Create your own map.",
		[=]()
		{
			menuSystem->QueueMessage("Map editor", "Lorem Ipsum er rett og slett dummytekst fra og for trykkeindustrien. Lorem Ipsum har vært bransjens standard for dummytekst helt siden 1500-tallet, da en ukjent boktrykker stokket en mengde bokstaver for å lage et prøveeksemplar av en bok. Lorem Ipsum har tålt tidens tann usedvanlig godt, og har i tillegg til å bestå gjennom fem århundrer også tålt spranget over til elektronisk typografi uten vesentlige endringer. Lorem Ipsum ble gjort allment kjent i 1960-årene ved lanseringen av Letraset-ark med avsnitt fra Lorem Ipsum, og senere med sideombrekkingsprogrammet Aldus PageMaker som tok i bruk nettopp Lorem Ipsum for dummytekst.");
		})); yy += sep;

	menuMain->AddItem(new Button(menuSystem, "Quit", Vec2(xx, (GLint)(mainBox->GetPosition().y + mainBox->GetSize().y - 43)), Vec2(102, 28), MenuItem::CENTER, MENU_MAIN,
		"Close the game and return to windows.",
		[=]()
		{
			// Show quit-message
			menuSystem->QueueQuestion("Quit", "You sure about that?",
			[]()
			{
				// Yes-button
				GameEngine::GetInstance()->StopGame();
			},
			[]()
			{
				// No-button
			});
		})); yy += sep;

	gameListBox = (ListBox*)menuMain->AddItem(new ListBox(menuSystem, Vec2((GLint)mainBox->GetPosition().x + (GLint)mainBox->GetSize().x + 25, (GLint)mainBox->GetPosition().y),
		Vec2((GLint)(centerX - mainBox->GetPosition().x - ((mainBox->GetPosition().x + mainBox->GetSize().x + 25) - centerX)), //auto-widen the box so that it all fits nicely in the middle of the screen
			(GLint)mainBox->GetSize().y), "Games in progress", MENU_MAIN, [](){})); // games-in-progress box
	gameListBox->AddItem("Tamschi's game#In-game#4/8");
	gameListBox->AddItem("jalb's game#Lobby#2/8");
	gameListBox->AddItem("Normano's game#Lobby#8/8");
	gameListBox->AddItem("Bottolf's game#In-game#1/8");

	// Options
	ContentBox *cb;
	cb = (ContentBox*)menuMain->AddItem(new ContentBox(menuSystem, "Audio", centerX - 440, centerY - 150, 280, 300, MENU_OPTIONS));
	cb->AddItem(new Slider(menuSystem, "SFX volume", Vec2(15, 15), cb->GetSize().x - 30, 0.0f, 100.0f, 5, MENU_OPTIONS, [](){}));
	cb->AddItem(new Slider(menuSystem, "Music volume", Vec2(15, 15), cb->GetSize().x - 30, 0.0f, 100.0f, 5, MENU_OPTIONS, [](){}));

	cb = (ContentBox*)menuMain->AddItem(new ContentBox(menuSystem, "Video", centerX - 140, centerY - 150, 280, 300, MENU_OPTIONS));
	cb = (ContentBox*)menuMain->AddItem(new ContentBox(menuSystem, "Controls", centerX + 160, centerY - 150, 280, 300, MENU_OPTIONS));

	menuMain->AddItem(new Button(menuSystem, "Accept", Vec2(centerX - 51 - 55, 600), Vec2(102, 28), MenuItem::CENTER, MENU_OPTIONS, "Accept these changes.", [=]()
		{
			menuMain->GoToPrevious();
		}));
	menuMain->AddItem(new Button(menuSystem, "Cancel", Vec2(centerX - 51 + 55, 600), Vec2(102, 28), MenuItem::CENTER, MENU_OPTIONS, "Cancel these changes and go back.", [=]()
		{
			menuMain->GoToPrevious();
		}));

	// Testing contentbox
	cb = (ContentBox*)menuMain->AddItem(new ContentBox(menuSystem, "Contentbox!", centerX - 200, 200, 400, 330, MENU_SPLASH));
	cb->SetResizeMode(ContentBox::ResizeMode::RESIZE_WIDTH);
	this->cb = cb;
	/*
	ContentBox *boxception;
	boxception = (ContentBox*)cb->AddItem(new ContentBox(menuSystem, "Boxception", 10, 35, (GLint)cb->GetSize().x - 20, 200, 0));

	ContentBox *deeper;
	deeper = (ContentBox*)boxception->AddItem(new ContentBox(menuSystem, "Deeper", 10, 35, (GLint)boxception->GetSize().x - 40, 200, 0));
	*/

	// Testing radioboxes
	RadioBox *radioBox;
	radioBox = (RadioBox*)menuMain->AddItem(new RadioBox(menuSystem, Vec2(100, 200), 200, "What to add?", MENU_SPLASH, [=](){}));
	GLint rb1, rb2, rb3, rb4, rb5, rb6;
	rb1 = radioBox->AddItem("Button");
	rb2 = radioBox->AddItem("Slider");
	rb3 = radioBox->AddItem("Checkbox");
	rb4 = radioBox->AddItem("Input field");
	rb5 = radioBox->AddItem("Dropdown");
	rb6 = radioBox->AddItem("ListBox");

	menuMain->AddItem(new Button(menuSystem, "Add", Vec2(100, 200 + radioBox->GetSize().y + 10), Vec2(200, 28), MenuItem::Align::CENTER, MENU_SPLASH, "Add selected item to the contentbox.",[=]()
	{
		if (radioBox->GetSelected() == rb1)
			cb->AddItem(new Button(menuSystem, "Button", Vec2(10, 10), Vec2(cb->GetSize().x - 20, 28), MenuItem::Align::CENTER, MENU_SPLASH, "This is a button.", []() {}));
		if (radioBox->GetSelected() == rb2)
			cb->AddItem(new Slider(menuSystem, "Slider", Vec2(10, 10), cb->GetSize().x - 20, 0.0f, 100.0f, 1.0f, MENU_SPLASH, []() {}));
		if (radioBox->GetSelected() == rb3)
			cb->AddItem(new Checkbox(menuSystem, Vec2(10, 10), "Checkbox", MENU_SPLASH, []() {}));
		if (radioBox->GetSelected() == rb4)
			cb->AddItem(new InputField(menuSystem, Vec2(10, 10), cb->GetSize().x - 20, "Input field...", MENU_SPLASH, []() {}));
		if (radioBox->GetSelected() == rb5)
		{
			dd = (Dropdown*)cb->AddItem(new Dropdown(menuSystem, "Dropdown", Vec2(10, 10), Vec2(cb->GetSize().x - 20, 28), MENU_SPLASH, "This is a dropdown.", []() {}));
			for (int i = 0; i < 10; i++)
			{ dd->AddItem("Item #" + toString(i)); }
		}
		if (radioBox->GetSelected() == rb6)
		{
			lb = (ListBox*)cb->AddItem(new ListBox(menuSystem, Vec2(10, 25), Vec2(cb->GetSize().x - 20, 200), "List", MENU_SPLASH, [=]() {
			}));
			for (int i = 0; i < 30; i++)
			{
				//lb->AddItem("Item #" + toString(i));
			}
		}
	}));

	// On draw-event for Main Menu
	menuMain->OnDraw([&]()
	{
		// Logo
		GLfloat rot = menuSystem->GetRot();
		/*if (menuMain->GetMenuCurrent() == MENU_SPLASH)
			sprLogo->Draw((GLint)(Context::getWindowWidth() / 2 + ldirX(30, rot * 2)), (GLint)(230 + ldirY(10, rot)), (GLfloat)(ldirX(4, rot * 5)), 0.8f, 0.8f, 1.0f);
			*/
	});
}