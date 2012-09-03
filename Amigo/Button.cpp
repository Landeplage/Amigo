#pragma once
#include "Button.h"
#include "Helper.h"
#include <GL\glfw.h>
#include "Input.h"
#include "MenuSystem.h"

Button::Button(MenuSystem* menuSystem, std::string text, GLint x, GLint y, GLint w, GLint h, MenuItem::Align align, GLint menuID, std::string tooltip, std::function<void()> onClick)
{
	this->menuSystem = menuSystem;
	this->text = text;
	this->tooltip = tooltip;
	this->position = Vec2((GLfloat)x, (GLfloat)y);
	this->size = Vec2((GLfloat)w, (GLfloat)h);
	this->menuID = menuID;
	this->function = onClick;

	this->font = menuSystem->GetFontRegular();
	this->sprite = menuSystem->GetSpriteUI();

	state = 0;

	SetTextAlignment(align);

	hasBeenClicked = false;
}

void Button::Unload()
{
	// unload any resources
}

void Button::Update(GLdouble time)
{
	// Reset hasBeenClicked variable
	hasBeenClicked = false;

	if (!(active && visible))
		return;

	// Get the focused item
	MenuItem* focus = menuSystem->GetFocus();

	if (focus == NULL || focus == this)
	{
		// Handle button-states
		Vec2 mouse = Input::getMousePos();
		mouse.x -= menuOffset.x;
		mouse.y -= menuOffset.y;

		// Check if mouse is over button
		if (IsInside(mouse, (GLint)position.x, (GLint)position.y, (GLint)(position.x + size.x), (GLint)(position.y + size.y)))
		{
			// One-off event when mouse enters button
			if (state == 0 && focus == NULL)
			{
				// Set focus
				menuSystem->SetFocus(this);
				// Set tooltip
				if (tooltip != "")
					menuSystem->SetTooltip(Vec2(position.x + size.x / 2, position.y + 6), tooltip);
			}

			// Set cursor
			menuSystem->SetCursor(1);

			// Set to hover-state
			state = 1;

			// Button pressed
			if (Input::getMouseLeft() && focus == this)
			{
				state = 2;

				// Clicked
				if (Input::getMouseLeftPressed())
				{
					hasBeenClicked = true;
					menuSystem->ResetTooltip();
				}
			}

			// Button released
			if (Input::getMouseLeftReleased())
			{
				state = 0;
				// Reset cursor
				menuSystem->SetCursor(0);
				onClick();
			}
		}
		else
		{
			state = 0;
			if (focus == this)
			{
				if (Input::getMouseLeft())
					state = 2;
				else
				{
					menuSystem->ResetFocus();
					menuSystem->ResetTooltip();
					// Reset cursor
					menuSystem->SetCursor(0);
				}
			}
		}
	}
}

void Button::Draw(GLfloat transition)
{
	if (!visible)
		return;

	GLfloat rot, alpha;
	rot = menuSystem->GetRot();
	alpha = 1 - abs(transition);

	GLint x, y, w, h, hh, xOff;
	x = (GLint)(position.x);
	y = (GLint)position.y;
	w = (GLint)size.x;
	h = (GLint)size.y;
	hh = 17;
	xOff = state * 11;
	if (!active)
		xOff = 33;

	// Compensate x, w and h for edges and shadow of the button texture
	x -= 1;
	w -= 3;
	h -= 4;

	//			x			y			rot			xscale				yscale						red			green		blue		alpha		xx				yy		ww		hh
	sprite->Draw(x + 3,		y + 5,		0.0f,		(GLfloat)(w - 1),	(GLfloat)(h - 5) / 19.0f,	1.0f,		1.0f,		1.0f,		alpha,		5 + xOff,		4,		1,		19);	// Middle
	sprite->Draw(x,			y,			0.0f,		1.0f,				1.0f,						1.0f,		1.0f,		1.0f,		alpha,		0 + xOff,		0,		5,		5);		// Left top corner
	sprite->Draw(x,			y + 5,		0.0f,		1.0f,				(GLfloat)(h - 5) / 19.0f,	1.0f,		1.0f,		1.0f,		alpha,		0 + xOff,		5,		3,		19);		// Left side
	sprite->Draw(x,			y + h,		0.0f,		1.0f,				1.0f,						1.0f,		1.0f,		1.0f,		alpha,		0 + xOff,		24,		5,		7);		// Left bottom corner
	sprite->Draw(x + 5,		y + h,		0.0f,		(GLfloat)(w - 5),	1.0f,						1.0f,		1.0f,		1.0f,		alpha,		5 + xOff,		24,		1,		7);		// Bottom
	sprite->Draw(x + w,		y + h,		0.0f,		1.0f,				1.0f,						1.0f,		1.0f,		1.0f,		alpha,		6 + xOff,		24,		5,		7);		// Right bottom corner
	sprite->Draw(x + w + 2,	y + 5,		0.0f,		1.0f,				(GLfloat)(h - 5) / 19.0f,	1.0f,		1.0f,		1.0f,		alpha,		8 + xOff,		5,		3,		19);		// Right side
	sprite->Draw(x + w,		y,			0.0f,		1.0f,				1.0f,						1.0f,		1.0f,		1.0f,		alpha,		6 + xOff,		0,		5,		5);		// Right top corner
	sprite->Draw(x + 5,		y,			0.0f,		(GLfloat)(w - 5),	1.0f,						1.0f,		1.0f,		1.0f,		alpha,		5 + xOff,		0,		1,		5);		// Top

	// Draw the button-text
	GLint bOff = 0;
	if (state == 2) bOff = 2;
	font->Draw((GLint)(x + textOffset.x), (GLint)(y + bOff + h / 2 - 4), text, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha);
	font->Draw((GLint)(x + textOffset.x), (GLint)(y + bOff + h / 2 - 5), text, 0.0f, 1.0f, 1.0f, (GLfloat)(139.0f / 255.0f), (GLfloat)(98.0f / 255.0f), (GLfloat)(38.0f / 255.0f), alpha);
}

void Button::onClick()
{
	function();
}

void Button::SetText(std::string text, MenuItem::Align align)
{
	this->text = text;
	SetTextAlignment(align);
}

void Button::SetTextAlignment(MenuItem::Align align)
{
	switch(align)
	{
		case LEFT: textOffset.x = 5; break;
		case CENTER: textOffset.x = (size.x - font->GetWidth(text)) / 2; break;
		case RIGHT: textOffset.x = size.x - font->GetWidth(text) - 5; break;
	}
}

void Button::SetOnClick(std::function<void()> onClick)
{
	function = onClick;
}

void Button::SetMenuOffset(GLint xOffset, GLint yOffset)
{
	menuOffset.x = (GLfloat)xOffset;
	menuOffset.y = (GLfloat)yOffset;
}

bool Button::GetHasBeenClicked()
{
	return hasBeenClicked;
}