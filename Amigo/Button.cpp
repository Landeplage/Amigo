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

	this->align = align;
	SetTextAlignment(align);
}

void Button::Unload()
{
	// unload any resources
}

void Button::HandleInput()
{
	if (!(active))
		return;

	MenuItem* focus = menuSystem->GetFocus();

	// Optimization check
	if (focus == NULL || focus == this)
	{
		// Get mouse input
		Vec2 mouse = Input::getMousePos();
		mouse = mouse - origin;

		bool isInside = false;
		if (IsInside(mouse, (GLint)position.x, (GLint)position.y, (GLint)(position.x + size.x), (GLint)(position.y + size.y)))
		{
			isInside = true;
		}

		if (focus == NULL && isInside)
		{
			menuSystem->SetFocus(this);
		}

		if (focus == this)
		{
			if (isInside)
			{
				menuSystem->SetCursor(1);

				if (!Input::getMouseLeft() && state == 0)
				{
					state = 1;
					// play mouseover sound here
					if (tooltip != "")
						menuSystem->SetTooltip(Vec2(
							position.x + origin.x + size.x / 2,
							position.y + origin.y + 6),
							tooltip);
				}

				if (Input::getMouseLeftPressed())
				{
					state = 2;
				}

				if (Input::getMouseLeftReleased() && state == 2)
				{
					onClick();
					state = 0;
					// play button click sound here
					menuSystem->ResetTooltip();
				}
			}
			else
			{
				if (!Input::getMouseLeft())
				{
					menuSystem->ResetFocus();
					state = 0;
					menuSystem->ResetTooltip();
				}
			}
		}
	}
}

void Button::Update(GLdouble time)
{
	if (!(active))
		return;

	if (state != 0)
	{
		if (menuSystem->GetFocus() == NULL)
		{
			state = 0;
		}
	}
}

void Button::Draw()
{
	if (!visible)
		return;

	GLfloat alpha = 1.0f;
	GLint x, y, w, h, hh, xOff;
	x = (GLint)position.x + drawOffset.x;
	y = (GLint)position.y + drawOffset.y;
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

	//			 x			y			rot			xscale				yscale						alpha		xx				yy		ww		hh
	sprite->Draw(x + 3,		y + 5,		0.0f,		(GLfloat)(w - 1),	(GLfloat)(h - 5) / 19.0f,	alpha,		5 + xOff,		4,		1,		19);	// Middle
	sprite->Draw(x,			y,			0.0f,		1.0f,				1.0f,						alpha,		0 + xOff,		0,		5,		5);		// Left top corner
	sprite->Draw(x,			y + 5,		0.0f,		1.0f,				(GLfloat)(h - 5) / 19.0f,	alpha,		0 + xOff,		5,		3,		19);	// Left side
	sprite->Draw(x,			y + h,		0.0f,		1.0f,				1.0f,						alpha,		0 + xOff,		24,		5,		7);		// Left bottom corner
	sprite->Draw(x + 5,		y + h,		0.0f,		(GLfloat)(w - 5),	1.0f,						alpha,		5 + xOff,		24,		1,		7);		// Bottom
	sprite->Draw(x + w,		y + h,		0.0f,		1.0f,				1.0f,						alpha,		6 + xOff,		24,		5,		7);		// Right bottom corner
	sprite->Draw(x + w + 2,	y + 5,		0.0f,		1.0f,				(GLfloat)(h - 5) / 19.0f,	alpha,		8 + xOff,		5,		3,		19);	// Right side
	sprite->Draw(x + w,		y,			0.0f,		1.0f,				1.0f,						alpha,		6 + xOff,		0,		5,		5);		// Right top corner
	sprite->Draw(x + 5,		y,			0.0f,		(GLfloat)(w - 5),	1.0f,						alpha,		5 + xOff,		0,		1,		5);		// Top

	// Draw the button-text
	GLint bOff = 0;
	if (state == 2) bOff = 2;
	font->Draw((GLint)(x + textOffset.x), (GLint)(y + bOff + h / 2 - 4), text, 0.0f, 1.0f, 1.0f, Color(255, 255, 255), alpha * 0.1f);
	font->Draw((GLint)(x + textOffset.x), (GLint)(y + bOff + h / 2 - 5), text, 0.0f, 1.0f, 1.0f, Color(139, 98, 38), alpha);
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

void Button::SetSize(Vec2 size)
{
	MenuItem::SetSize(size);
	SetTextAlignment(align);
}

void Button::SetPosition(Vec2 position)
{
	MenuItem::SetPosition(position);
	SetTextAlignment(align);
}

GLint Button::GetState()
{
	return state;
}