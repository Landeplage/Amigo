#include "Menu.h"
#include "Box.h"
#include "Button.h"
#include "Slider.h"
#include "SliderRange.h"
#include "MenuSystem.h"
#include "Helper.h"

Menu::Menu(GLint x, GLint y, GLint width, GLint height, MenuSystem* menuSystem)
{
	printf("-> Menu created.\n");

	this->menuSystem = menuSystem;
	this->position.x = (GLfloat)x;
	this->position.y = (GLfloat)y;
	this->width = width;
	this->height = height;
	sprite = menuSystem->GetSpriteUI();
	fontBold = menuSystem->GetFontBold();

	renderTarget = new RenderTarget(width, height);

	menuCurrent = 0;
	menuGoTo = 0;
	slide = 0;
	slideTarget = 0;
	slideSpeed = 0;
	SetTransition(0, 0, 0.0f, false);
	onDraw = [](){};
}

Menu::~Menu()
{
	printf("Deleting menu...\n");
	
	//Unload rendertarget
	renderTarget->~RenderTarget();

	// Unload all items
	for(GLuint i = 0; i < items.size(); i ++)
	{
		items[i]->Unload();
	}
}

void Menu::Update(GLdouble time)
{
	// Handle items
	if (menuCurrent == menuGoTo)
	{
		// Update all items of the menu
		if (active
			&& (GLint)(slide * 10) == 0
			&& (abs(menuSystem->GetOverlaySlide()) > 0.9f && !menuSystem->GetOverlayShow()))
		{
			MenuItem* focus = menuSystem->GetFocus();
			for(GLuint i = 0; i < items.size(); i ++)
			{
				if (items[i]->menuID == menuCurrent && items[i] != focus)
					items[i]->Update(time);
			}
			// Update the focused element last
			if (focus != NULL)
				focus->Update(time);
		}

		// Make the slide settle down
		if (slideTarget == 0 && slide > -0.05f)
		{
			slide = 0;
			slideSpeed = 0;
		}
	}
	else // handle menu-switching
	{
		// Slide out
		if (slide > 1.0f)
		{
			menuCurrent = menuGoTo;
			slide = -1.0f;
			slideTarget = 0;
			slideSpeed = 0;
		}
	}

	// Slide to target
	//printf("Time = %i\n", (GLint)(time));
	/*GLint slideLoop = (GLint)(time);
	do
	{
		slideSpeed = menuSystem->Wobble(slide, slideTarget, slideSpeed, 0.02f, 0.02f, time);
		slide += slideSpeed;
		slideLoop --;
	}
	while (slideLoop > 0);*/

	//if (slide > slideTarget)
	//	slide -= (GLfloat)(abs(slideTarget - slide) * (time / 120.0f));

	// Slide to target
	if (slide < slideTarget)
		slide += (GLfloat)(abs(slide - slideTarget) * (time / 150.0f));

	// Render items to rendertarget
	//Render();
}

// Draw the rendertarget texture
void Menu::Draw()
{
	// Debug
	std::string str = toString(slide) + " -> " + toString(slideTarget);
	fontBold->Draw(10, 10, str);

	// Draw rendertarget
	GLfloat tmpScale, tmpShutterX, tmpShutterW;
	
	// Shutter-effect (a.k.a. fancy effect)
	tmpShutterX = 0;
	tmpShutterW = (GLfloat)width;

	if (fancyEffect)
	{
		tmpShutterX = (abs(slide) * (width / 2));
		tmpShutterW = width * (-abs(slide) + 1);
	}

	// Scale
	tmpScale = 1.0f + (slide * scale);
	/*
	// Draw the rendertarget
	renderTarget->Draw(
		(GLint)(position.x + (moveX * slide) - ((tmpScale - 1) * width) / 2 + tmpShutterX),
		(GLint)(position.y + (moveY * slide) - ((tmpScale - 1) * height) / 2),
		0.0f, tmpScale, tmpScale, 1.0f, 1.0f, 1.0f, alpha, (GLint)tmpShutterX, 0, (GLint)tmpShutterW, height);
		*/

	// Draw all items of the menu
	MenuItem* focus = menuSystem->GetFocus();
	for(GLuint i = 0; i < items.size(); i ++)
	{
		if (items[i] != focus && items[i]->menuID == menuCurrent)
			items[i]->Draw(slide);
	}

	// Draw the focused item last
	for(GLuint i = 0; i < items.size(); i ++)
	{
		if (items[i] == focus)
		{
			focus->Draw(slide);
			break;
		}
	}

	// Draw any stuff on top
	onDraw();
}

// Render the items onto the rendertarget
void Menu::Render()
{
	// Begin rendertarget
	//renderTarget->Begin();

	// End rendertarget
	//renderTarget->End();
}

// Get the current menu ID
GLint Menu::GetMenuCurrent()
{
	return menuCurrent;
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
void Menu::SetTransition(GLint moveX, GLint moveY, GLfloat scale, bool fancyEffect)
{
	this->moveX = moveX;
	this->moveY = moveY;
	this->scale = scale;
	this->fancyEffect = fancyEffect;
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
	slideTarget = 1.5f;
}

// Add a box
MenuItem* Menu::AddBox(std::string title, GLint x, GLint y, GLint width, GLint height, GLint menuID)
{
	items.push_back(new Box(menuSystem, title, x, y, width, height, menuID));
	return items[items.size() - 1];
}

// Add a button
MenuItem* Menu::AddButton(std::string text, GLint x, GLint y, GLint width, GLint height, MenuItem::Align align, GLint menuID, std::string tooltip, std::function<void()> onClick)
{
	items.push_back(new Button(menuSystem, text, x, y, width, height, align, menuID, tooltip, onClick));
	return items[items.size() - 1];
}

// Add a slider
MenuItem* Menu::AddSlider(std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease)
{
	items.push_back(new Slider(menuSystem, text, position, width, min, max, step, menuID, onRelease));
	return items[items.size() - 1];
}

// Add a range-slider
MenuItem* Menu::AddSliderRange(std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease)
{
	items.push_back(new SliderRange(menuSystem, text, position, width, min, max, step, menuID, onRelease));
	return items[items.size() - 1];
}