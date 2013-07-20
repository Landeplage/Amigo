#pragma once
#include "Slider.h"
#include "MenuSystem.h"
#include "Helper.h"
#include "Input.h"

Slider::Slider()
{
	// default constructor
}

Slider::Slider(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease)
{
	this->menuSystem = menuSystem;
	this->text = text;
	this->position = position;
	this->size.x = (GLfloat)width;
	this->min = min;
	this->max = max;
	this->step = step;
	this->menuID = menuID;
	this->onRelease = onRelease;

	this->font = menuSystem->GetFontRegular();
	this->sprite = menuSystem->GetSpriteUI();

	this->button = new Button(menuSystem, "", (GLint)position.x, (GLint)(position.y + font->GetHeight("Mj")), 1, 1, CENTER, 0, "", onRelease);

	this->value = min;
	mouseOffset = Vec2(0, 0);

	valueString = "?";

	Init();
}

void Slider::Init()
{
	// Make sure min is not over max
	if (this->min > this->max)
		this->min = this->max;

	// Make sure value stays inside the min/max
	if (value < min)
		value = min;
	if (value > max)
		value = max;

	// Set size of button
	/*GLint bWidth = (GLint)(size.x / (this->max - this->min + 1));
	if (bWidth < 30) bWidth = 30;
	button->SetSize(bWidth, 20);*/
	button->SetSize(23, 23);

	// Update height of slider
	this->size.y = button->GetPosition().y - position.y + button->GetSize().y;

	// Move the slider-button to the correct position
	SnapButton(button, &value, (GLint)position.x, (GLint)(position.x + size.x), (GLint)(size.x - button->GetSize().x), 0);

	// Set valuestring
	valueString = toString(value);
}

void Slider::Unload()
{
	// Unload the button
	button->Unload();
}

void Slider::Update(GLdouble time)
{
	if (!(active && visible))
		return;

	// Handle the button
	HandleButton(button, time, (GLint)position.x, (GLint)(position.x + size.x - button->GetSize().x), (GLint)(size.x - button->GetSize().x), 0, &value);

	// Set valuestring
	valueString = toString(value);
}

void Slider::Draw(GLfloat transition)
{
	if (!visible)
		return;

	GLfloat alpha;
	alpha = 1 - abs(transition);

	GLint x, y;
	x = (GLint)(position.x);
	y = (GLint)(button->GetPosition().y + button->GetSize().y / 2 - 2);

	// Draw the elements that all children of 'slider' share
	DrawCommonElements(x, y, alpha);

	// Draw filled part of slider-back
	sprite->Draw(x - 1, y, 0.0f, 1.0f, 1.0f, alpha, 14, 31, 3, 7); // Fill (left)
	sprite->Draw(x + 2, y, 0.0f, (GLfloat)(button->GetPosition().x - position.x - 2), 1.0f, alpha, 17, 31, 1, 7); // Fill (mid)

	// Draw the button
	button->Draw(transition);
	//sprite->Draw((GLint)(button->GetPosition().x - 1), (GLint)(button->GetPosition().y), 0.0f, 1.0f, 1.0f, alpha, 0, 45, 25, 26); // Scroller-button
}

void Slider::DrawCommonElements(GLint x, GLint y, GLfloat alpha)
{
	// Draw slider-back
	sprite->Draw(x, y, 0.0f, 1.0f, 1.0f, alpha, 9, 31, 2, 5); // Slider-back (left)
	sprite->Draw(x + 2, y, 0.0f, (GLfloat)(size.x - 4), 1.0f, alpha, 11, 31, 1, 5); // Slider-back (left)
	sprite->Draw(x + (GLint)size.x - 2, y, 0.0f, 1.0f, 1.0f, alpha, 12, 31, 2, 5); // Slider-back (right)

	// Draw slider-info text
	font->Draw(x, (GLint)position.y, text, 0.0f, 1.0f, 1.0f, Color(139, 98, 38), alpha);
	font->Draw((GLint)(x + size.x - font->GetWidth(valueString)), (GLint)position.y, valueString, 0.0f, 1.0f, 1.0f, Color(139, 98, 38), alpha);
}

// Set the value of the slider
void Slider::SetValue(GLfloat value)
{
	// Clamp value-parameter and assign to variable
	if (value > max) value = max;
	if (value < min) value = min;
	this->value = value;

	Init();
}

// Set the minimum value
void Slider::SetMin(GLint min)
{
	this->min = (GLfloat)min;
	Init();
}

// Set the maximum value
void Slider::SetMax(GLint max)
{
	this->max = (GLfloat)max;
	Init();
}

// Return the current value
GLfloat Slider::GetValue()
{
	return value;
}

// Handle the movement of buttons
void Slider::HandleButton(Button *button, GLdouble time, GLint limitXLeft, GLint limitXRight, GLint maxPos, GLint minPos, GLfloat *value)
{
	// Sync and update the button
	button->active = active;
	button->visible = visible;
	button->Update(time);

	// Get the mouse-offset as the button is clicked
	if (button->GetHasBeenClicked())
	{
		mouseOffset = Vec2((button->GetPosition().x - Input::getMousePos().x), (button->GetPosition().y - Input::getMousePos().y));
	}

	// Get button-input
	if (menuSystem->GetFocus() == button)
	{
		// Button is pressed
		if (Input::getMouseLeft())
		{
			Vec2 mouse = Input::getMousePos();

			// Move the button
			button->SetPosition((GLint)(mouse.x + mouseOffset.x), (GLint)(button->GetPosition().y));

			// Clamp the button's movement
			GLint sliderX, buttonX;
			sliderX = (GLint)position.x;
			buttonX = (GLint)button->GetPosition().x;
			if (buttonX < sliderX + minPos) button->SetPosition(sliderX + minPos, (GLint)button->GetPosition().y);
			if (buttonX > sliderX + maxPos) button->SetPosition(sliderX + maxPos, (GLint)button->GetPosition().y);
			if (buttonX < limitXLeft) button->SetPosition(limitXLeft, (GLint)button->GetPosition().y);
			if (buttonX > limitXRight) button->SetPosition(limitXRight, (GLint)button->GetPosition().y);

			// Set the mouse-position to the button's position
			Input::setMousePos(Vec2(button->GetPosition().x - mouseOffset.x, button->GetPosition().y - mouseOffset.y));

			// Calculate the new value of the slider
			CalculateValue(button, maxPos, minPos, value);

			// Check if step is enabled and move button to correct position
			SnapButton(button, value, limitXLeft, limitXRight, maxPos, minPos);
		}

		// Wheel is scrolled (fine-tuning)
		/*GLint wheelTurn = Input::getMouseWheel();
		if (wheelTurn != 0)
		{
			GLfloat bXPrev = button->GetPosition().x;
			SetValue(*value + wheelTurn);
			Input::setMousePos(Vec2(Input::getMousePos().x + button->GetPosition().x - bXPrev, Input::getMousePos().y));
		}*/
	}
}

// Calculate value based on the position of the button
void Slider::CalculateValue(Button *button, GLint maxPos, GLint minPos, GLfloat *value)
{
	if (max - min != 0)
	{
		GLfloat relativePos;
		relativePos = button->GetPosition().x - (position.x + minPos);
		*value = (GLfloat)(GLint)((relativePos / (maxPos - minPos)) * (max - min) + min + 0.5f);
	}
}

void Slider::SnapButton(Button *button, GLfloat *value, GLint limitXLeft, GLint limitXRight, GLint maxPos, GLint minPos)
{
	//If stepping is enabled, snap button to nearest increment
	if (step > 0 && (max - min != 0))
	{
		*value = (GLint)((*value / step) + 0.5f) * step;
		button->SetPosition((GLint)(position.x + minPos + ((*value) - min) / (max - min) * (maxPos - minPos)), (GLint)(button->GetPosition().y));
	}

	// Make sure button stays inside area
	GLint buttonX = (GLint)(button->GetPosition().x);
	if (buttonX < limitXLeft) button->SetPosition(limitXLeft, (GLint)(button->GetPosition().y));
	if (buttonX > limitXRight) button->SetPosition(limitXRight, (GLint)(button->GetPosition().y));
}