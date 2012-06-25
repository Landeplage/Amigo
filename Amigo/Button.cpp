#pragma once
#include "Button.h"
#include "Helper.h"
#include <GL\glfw.h>
#include "Input.h"
#include "MenuSystem.h"

Button::Button(MenuSystem* menuSystem, std::string text, GLint x, GLint y, GLint w, GLint h, MenuItem::Align align, GLint menuID, std::function<void()> onClick, Vec2 menuOffset)
{
	this->menuSystem = menuSystem;
	this->text = text;
	this->position = Vec2(x, y);
	this->size = Vec2(w, h);
	this->menuID = menuID;
	this->function = onClick;
	this->menuOffset = menuOffset;

	this->font = menuSystem->GetFontBold();
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
		if (IsInside(mouse, position.x, position.y, position.x + size.x, position.y + size.y))
		{
			// One-off event when mouse enters button
			if (state == 0 && focus == NULL)
			{
				// Set focus
				menuSystem->SetFocus(this);
			}

			// Set to hover-state
			state = 1;

			// Change cursor
			menuSystem->SetCursor(1);

			// Button pressed
			if (Input::getMouseLeft() && focus == this)
			{
				state = 2;

				// Clicked
				if (Input::getMouseLeftPressed())
					hasBeenClicked = true;
			}

			// Button released
			if (Input::getMouseLeftReleased())
			{
				state = 0;
				onClick();
			}
		}
		else
		{
			state = 0;
			if (focus == this)
			{
				if (Input::getMouseLeft())
					state = 1;
				else
				{
					menuSystem->ResetFocus();
				}
			}
		}
	}
}

void Button::Draw()
{
	if (!visible)
		return;

	float rot;
	rot = menuSystem->GetRot();

	int x, y, w, h, hh, xOff;
	x = position.x;
	y = position.y;
	w = size.x;
	h = size.y;
	hh = 17;
	xOff = state * 7;
	if (!active)
		xOff = 21;

	if (h > 35)
	{
		sprite->Draw(x + w - 3, y + 17, 0.0f, 1.0f, h - 35, 1.0f, 4 + xOff, 17, 3, 1); // Right
		sprite->Draw(x, y + 17, 0.0f, 1.0f, h - 35, 1.0f, xOff, 17, 3, 1); // Left
		sprite->Draw(x + 3, y + 17, 0.0f, w - 6, h - 35, 1.0f, 3 + xOff, 17, 1, 1); // Middle
	}
	else
	{
		hh = h / 2;
	}
	sprite->Draw(x, y, 0.0f, 1.0f, 1.0f, 1.0f, xOff, 0, 3, hh); // Top left
	sprite->Draw(x + 3, y, 0.0f, w - 6, 1.0f, 1.0f, 3 + xOff, 0, 1, hh); // Top
	sprite->Draw(x + w - 3, y, 0.0f, 1.0f, 1.0f, 1.0f, 4 + xOff, 0, 3, hh); // Top right
	sprite->Draw(x + w - 3, y + h - 35 + (-hh + 34), 0.0f, 1.0f, 1.0f, 1.0f, 4 + xOff, 18 - hh + 16, 3, hh + 1); // Bottom right
	sprite->Draw(x + 3, y + h - 35 + (-hh + 34), 0.0f, w - 6, 1.0f, 1.0f, 3 + xOff, 18 - hh + 16, 1, hh + 1); // Bottom
	sprite->Draw(x, y + h - 35 + (-hh + 34), 0.0f, 1.0f, 1.0f, 1.0f, xOff, 18 - hh + 16, 3, hh + 1); // Bottom left

	// Draw the button-text
	int bOff = 0;
	if (xOff == 14) bOff = 2;
	font->Draw(x + textOffset.x, y + bOff + h / 2 - 10 - 1, text, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.25f);
	font->Draw(x + textOffset.x, y + bOff + h / 2 - 10, text, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	// Glow around button
	if (state > 0)
	{
		float glowAlpha = 0.25f + ldirX(0.1f, rot * 50);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending
		sprite->Draw(x - 12, y - 12, 0.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Top left
		sprite->Draw(x + 3, y - 12, 0.0f, w - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Top
		sprite->Draw(x + w - 3 + 15, y - 12, 90.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Top right
		sprite->Draw(x + w - 3 + 15, y + 3, 90.0f, h - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Right
		sprite->Draw(x + w - 3 + 15, y + h + 12, 180.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Bottom right
		sprite->Draw(x + w - 3, y + h + 12, 180.0f, w - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Bottom
		sprite->Draw(x - 12, y + h + 12, 270.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Bottom left
		sprite->Draw(x - 12, y + h - 3, 270.0f, h - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Left
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Reset to normal blending
	}
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
		case Align::LEFT: textOffset.x = 5; break;
		case Align::CENTER: textOffset.x = (size.x - font->GetWidth(text)) / 2; break;
		case Align::RIGHT: textOffset.x = size.x - font->GetWidth(text) - 5; break;
	}
}

void Button::SetOnClick(std::function<void()> onClick)
{
	function = onClick;
}

void Button::SetMenuOffset(GLint xOffset, GLint yOffset)
{
	menuOffset.x = xOffset;
	menuOffset.y = yOffset;
}

bool Button::GetHasBeenClicked()
{
	return hasBeenClicked;
}