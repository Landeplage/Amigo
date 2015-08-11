#pragma once
#include "Button.h"
#include "Helper.h"
#include "GLFW.h"
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
	w += 2;
	h += 3;

	// Draw the button
	sprite->DrawRectangleFromTexture(Vec2(x, y), Vec2(w, h), Vec2(xOff, 0), Vec2(5, 7), Vec2(1, 17));

	// Draw the button-text
	GLint bOff = 0;
	if (state == 2) bOff = 2;
	font->Draw((GLint)(x + textOffset.x), (GLint)(y + bOff + h / 2 - 7), text, 0.0f, 1.0f, 1.0f, Color(255, 255, 255), alpha * 1.0f); // highlight
	font->Draw((GLint)(x + textOffset.x), (GLint)(y + bOff + h / 2 - 8), text, 0.0f, 1.0f, 1.0f, Color(139, 98, 38), alpha); // text
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
		case LEFT: textOffset.x = 9; break;
		case CENTER: textOffset.x = (size.x - font->GetWidth(text)) / 2; break;
		case RIGHT: textOffset.x = size.x - font->GetWidth(text) - 9; break;
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