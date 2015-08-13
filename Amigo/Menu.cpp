#include "Menu.h"
#include "Box.h"
#include "ContentBox.h"

#include "Button.h"
#include "Slider.h"
#include "SliderRange.h"
#include "Dropdown.h"

#include "MenuSystem.h"
#include "Helper.h"

Menu::Menu(GLint x, GLint y, GLint width, GLint height, MenuSystem* menuSystem, GLint defaultMenuID)
{
	printf("-> Creating menu...\n");

	this->menuSystem = menuSystem;
	this->position.x = (GLfloat)x;
	this->position.y = (GLfloat)y;
	this->width = width;
	this->height = height;
	sprite = menuSystem->GetSpriteUI();
	fontBold = menuSystem->GetFontBold();

	renderTarget = new RenderTarget(width, height);

	menuCurrent = defaultMenuID;
	menuGoTo = menuCurrent;
	slide = 0;
	slideTarget = 0;
	SetTransition(0, 0, 0.0f);
	onDraw = [](){};

	for(int i = 0; i < MENU_HISTORY_LENGTH; i ++)
	{
		history[i] = -1;
	}
}

Menu::~Menu()
{
	printf("Destroying menu...\n");
	
	//Unload rendertarget
	renderTarget->~RenderTarget();

	// Unload all items
	for(GLuint i = 0; i < items.size(); i ++)
	{
		items[i]->Unload();
	}
}

void Menu::HandleInput()
{
	// Handle input
	if (menuCurrent == menuGoTo)
	{
		// Handle input of all items of the menu
		if (active
			&& slide == 0.0f
			&& (menuSystem->GetOverlaySlide() == -1.0f && !menuSystem->GetOverlayShow()))
		{
			MenuItem* focus = menuSystem->GetFocus();
			for(GLuint i = 0; i < items.size(); i ++)
			{
				if (items[i]->menuID == menuCurrent && items[i] != focus)
					items[i]->HandleInput();
			}

			// Handle the focused element last (as long as it exists within this specific menu)
			for(GLuint i = 0; i < items.size(); i ++)
			{
				if (focus == items[i])
					focus->HandleInput();
			}
		}
	}
}

void Menu::Update(GLdouble time)
{
	// Handle items
	if (menuCurrent == menuGoTo)
	{
		// Update all items of the menu
		if (active && slide == 0.0f)
		{
			MenuItem* focus = menuSystem->GetFocus();
			for(GLuint i = 0; i < items.size(); i ++)
			{
				if (items[i]->menuID == menuCurrent && items[i] != focus)
					items[i]->Update(time);
			}
			// Update the focused element last (as long as it exists within this specific menu)
			for(GLuint i = 0; i < items.size(); i ++)
			{
				if (focus == items[i])
				focus->Update(time);
			}
		}

		// Make the slide settle down
		if (slideTarget == 0 && slide > -0.0001f)
		{
			slide = 0;
		}
	}
	else // handle menu-switching
	{
		// Slide out
		if (slide > 1.0f)
		{
			if (history[0] == menuGoTo)
				GoBackInHistory();
			else
				AddHistory(menuCurrent);
			menuCurrent = menuGoTo;
			slide = -1.0f;
			slideTarget = 0;
		}
	}

	// Slide to target
	//if (slide < slideTarget)
	//	slide += (GLfloat)(abs(slide - slideTarget) * (time / 100.0f));

	if (slide < slideTarget)
		slide += (GLfloat)time / 200.0f;

	// Render items to rendertarget
	Render();
}

void Menu::Draw()
{
	// Scale
	GLfloat tmpScale;
	tmpScale = 1.0f + (slide * scale);
	
	// Draw the rendertarget
	int n = 1;
	if (menuCurrent != menuGoTo)
		n = -1;
	GLfloat rtSlide = EaseQuadIn(abs(slide)) * 1000 * n;

	renderTarget->Draw(
		(GLint)(position.x + (moveX * slide) - ((tmpScale - 1) * width) / 2 - rtSlide),
		(GLint)(position.y + (moveY * slide) - ((tmpScale - 1) * height) / 2),
		0.0f, tmpScale, tmpScale, 1 - abs(slide), 0, 0, width, height);

	// Cheap motion-blur effect
	/*for(int i = 1; i < abs(slide * slide * slide) * 10; i ++)
	{
		renderTarget->Draw(
		(GLint)(position.x + (moveX * slide) - ((tmpScale - 1) * width) / 2 - rtSlide + i * abs(slide) * 30),
		(GLint)(position.y + (moveY * slide) - ((tmpScale - 1) * height) / 2),
		0.0f, tmpScale, tmpScale, (1 - abs(slide) / (10 - i)) * 0.2f, 0, 0, width, height);
		renderTarget->Draw(
		(GLint)(position.x + (moveX * slide) - ((tmpScale - 1) * width) / 2 - rtSlide - i * abs(slide) * 30),
		(GLint)(position.y + (moveY * slide) - ((tmpScale - 1) * height) / 2),
		0.0f, tmpScale, tmpScale, (1 - abs(slide) / (10 - i)) * 0.2f, 0, 0, width, height);
	}*/
}

// Render the items onto the rendertarget
void Menu::Render()
{
	// Begin rendertarget
	renderTarget->Begin();

	// Draw all items of the menu
	MenuItem* focus = menuSystem->GetFocus();
	for(GLuint i = 0; i < items.size(); i ++)
	{
		if (items[i] != focus && items[i]->menuID == menuCurrent)
			items[i]->Draw();
	}

	// Draw the focused element last (as long as it exists within this specific menu)
	for(GLuint i = 0; i < items.size(); i ++)
	{
		if (focus == items[i])
		{
			focus->Draw();
		}
	}

	// Draw any stuff on top
	onDraw();

	// End rendertarget
	renderTarget->End();
}

// Get the current menu ID
GLint Menu::GetMenuCurrent()
{
	return menuCurrent;
}

// Get the menu ID the menu is going to
GLint Menu::GetMenuGoTo()
{
	return menuGoTo;
}

// Get the slide-animation variable
GLfloat Menu::GetSlide()
{
	return slide;
}

// Get the position of the menu
Vec2 Menu::GetPosition()
{
	return position;
}

// Set the attributes of transitions
void Menu::SetTransition(GLint moveX, GLint moveY, GLfloat scale)
{
	this->moveX = moveX;
	this->moveY = moveY;
	this->scale = scale;
}

void Menu::OnDraw(std::function<void()> onDraw)
{
	this->onDraw = onDraw;
}

// Go to a specific submenu
void Menu::GoTo(int menuID)
{
	menuGoTo = menuID;
	menuSystem->ResetFocus();
	slideTarget = 1.0f;
}

// Go to the previous submenu
void Menu::GoToPrevious()
{
	GoTo(history[0]);
}

// Get the menuID history of this menu
GLint Menu::GetHistory(GLint id)
{
	return history[id];
}

// Add to the history array
void Menu::AddHistory(GLint menuID)
{
	for(int i = MENU_HISTORY_LENGTH; i > 0; i --)
	{
		history[i] = history[i - 1];
	}
	history[0] = menuID;
}

// Go back one item in the history
void Menu::GoBackInHistory()
{
	for(int i = 0; i < MENU_HISTORY_LENGTH; i ++)
	{
		history[i] = history[i + 1];
	}
	history[MENU_HISTORY_LENGTH] = -1;
}

// Add a MenuItem to this menu
MenuItem* Menu::AddItem(MenuItem* item)
{
	items.push_back(item);
	return items[items.size() - 1];
}