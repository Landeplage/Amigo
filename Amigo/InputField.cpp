#pragma once

#include "InputField.h"
#include "MenuSystem.h"
#include "Helper.h"
#include "Input.h"

const int INPUT_INDICATOR_TIME = 1000; // How long it takes for the indicator to blink, in milliseconds

InputField::InputField(MenuSystem* menuSystem, Vec2 position, GLint width, std::string infoText, GLint menuID, std::function<void()> onEnter)
{
	this->menuSystem = menuSystem;
	this->position = position;
	this->size = Vec2(width, 28);
	this->menuID = menuID;
	this->onEnter = onEnter;

	this->font = menuSystem->GetFontRegular();
	this->sprite = menuSystem->GetSpriteUI();

	// Init button
	button = new Button(menuSystem, "", position, this->size, Align::CENTER, menuID, "",
	[=]() {
		// Activate input
		if (!inputActive)
			this->ToggleInputActive();
	});
	children.push_back(button);

	// Init text-variables
	text = "";
	displayText = "";
	this->infoText = infoText;

	// Input handling variables
	inputActive = false;
	backspaceLast = false;
	leftArrowLast = false;
	rightArrowLast = false;
	editPos = 0;

	// Init indicator-timer
	indicatorTimer = 0;
}

void InputField::Unload()
{
	// Unload button
	button->Unload();
}

void InputField::HandleInput()
{
	if (inputActive)
	{
		// Fetch text input
		std::string str;
		str = Input::GetTextInput();
		SetText(text + str);
		editPos += str.length();

		// Backspace functionality
		if (Input::GetKey(GLFW_KEY_BACKSPACE) == GLFW_PRESS && !backspaceLast)
		{
			if (editPos > 0)
			{
				std::string str1, str2;
				str1 = text.substr(0, editPos - 1);
				str2 = text.substr(editPos, text.length() - editPos);

				SetText(str1 + str2);
				editPos--;
			}
			backspaceLast = true;
		}

		// Move edit position left
		if (Input::GetKey(GLFW_KEY_LEFT) && !leftArrowLast)
		{
			if (editPos > 0)
				editPos--;
			leftArrowLast = true;
		}

		// Move edit position right
		if (Input::GetKey(GLFW_KEY_RIGHT) && !rightArrowLast)
		{
			if (editPos < text.length())
				editPos++;
			rightArrowLast = true;
		}

		// Enter-key is used as a confirm-key
		if (Input::GetKey(GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			onEnter();
		}
	}

	// Reset key-variables
	if (Input::GetKey(GLFW_KEY_BACKSPACE) == GLFW_RELEASE)
	{
		backspaceLast = false;
	}

	if (Input::GetKey(GLFW_KEY_LEFT) == GLFW_RELEASE)
	{
		leftArrowLast = false;
	}

	if (Input::GetKey(GLFW_KEY_RIGHT) == GLFW_RELEASE)
	{
		rightArrowLast = false;
	}

	// Deactivate input if user clicks anywhere outside input-field
	/*if (Input::getMouseLeftPressed())
	{
		if (menuSystem->GetFocus() != button && inputActive)
		{
			ToggleInputActive();
		}
	}*/

	// Handle button
	button->HandleInput();
}

void InputField::Update(GLdouble time)
{
	// Update button
	button->Update(time);

	// If a message/question appears, the input-field should deactivate
	if (menuSystem->GetOverlayShow() && inputActive)
	{
		ToggleInputActive();
	}

	// Update indicator-timer
	if (indicatorTimer < 0)
		ResetIndicatorTimer();
	indicatorTimer -= time;
}

void InputField::Draw()
{
	// Draw input-box
	GLfloat alpha = 1.0f;
	GLint x, y, w, h, xx, yy;
	x = (GLint)position.x + drawOffset.x;
	y = (GLint)position.y + drawOffset.y;
	w = (GLint)size.x;
	h = (GLint)size.y;

	// Find the right spot in the sprUI texture
	xx = 30;
	yy = 77;

	// Compensate x, w and h for edges and shadow of the texture
	x -= 1;
	w += 2;
	h += 1;

	// Draw the input-field background
	sprite->DrawRectangleFromTexture(Vec2(x, y), Vec2(w, h), Vec2(xx, yy), Vec2(5, 5), Vec2(1, 1));

	// Draw a box to visualize size
	//sprite->Draw(position.x, position.y, 0.0f, size.x, size.y, Color(0, 0, 0), 0.25f, 0, 72, 1, 1);

	// Draw text
	font->Draw(Vec2(position.x + drawOffset.x + 9, position.y + drawOffset.y + 7), displayText, 0.0f, Vec2(1.0f, 1.0f), Color(116, 85, 38), 1.0f);

	// Draw info-text (when there's nothing in the field)
	if (displayText == "" && !inputActive)
		font->Draw(Vec2(position.x + drawOffset.x + 9, position.y + drawOffset.y + 7), infoText, 0.0f, Vec2(1.0f, 1.0f), Color(193, 181, 166), 1.0f);

	// Draw input-indicator
	if (inputActive && indicatorTimer > (INPUT_INDICATOR_TIME / 2))
	{
		GLint offset;
		offset = font->GetWidth(displayText.substr(0, editPos));
		sprite->Draw(Vec2(position.x + drawOffset.x + 9 + offset, position.y + drawOffset.y + 4), 0.0f, Vec2(1, size.y - 8), Color(255, 255, 255), 1.0f, 35, 77, 1, 1);
	}

	// debug
	//font->Draw(10, 50, toString(drawOffset.y));
}

// Set the text for the input-field to display
void InputField::SetText(std::string text)
{
	this->text = text;

	// Update the displayed text
	UpdateDisplayText();
}

// Get the text currently in the input field
std::string InputField::GetText()
{
	return text;
}

// Update the displayed text
void InputField::UpdateDisplayText()
{
	displayText = font->TruncateString(text, size.x - 12);
}

// Toggle whether or not this inputfield is active
void InputField::ToggleInputActive()
{
	inputActive = !inputActive;

	if (inputActive)
	{
		// Set edit-position to the last character
		editPos = text.length();

		// Reset timer for indicator (so that it will be visible at the very moment input is activated)
		ResetIndicatorTimer();

		/* Let the menuSystem know that an inputfield has been activated
		(so that menuSystem can deactivate it if we click anywhere outside the inputfield */
		menuSystem->ActivateInputField(this);
	}
}

// Resets the timer for the input-indicator
void InputField::ResetIndicatorTimer()
{
	indicatorTimer = INPUT_INDICATOR_TIME;
}

void InputField::SetSize(Vec2 size)
{
	MenuItem::SetSize(size);
	button->SetSize(size);
}

void InputField::SetPosition(Vec2 position)
{
	MenuItem::SetPosition(position);
	button->SetPosition(position);
}

void InputField::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);
	button->SetOrigin(origin);
}

void InputField::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);
	button->SetDrawOffset(drawOffset);
}