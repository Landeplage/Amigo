#pragma once
#include "Slider.h"
#include "MenuSystem.h"
#include "Helper.h"
#include "Input.h"

Slider::Slider()
{
	// default constructor
}

Slider::Slider(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease, Vec2 menuOffset)
{
	this->menuSystem = menuSystem;
	this->text = text;
	this->position = position;
	this->size.x = width;
	this->min = min;
	this->max = max;
	this->step = step;
	this->menuID = menuID;
	this->onRelease = onRelease;
	this->menuOffset = menuOffset;

	this->font = menuSystem->GetFontRegular();
	this->sprite = menuSystem->GetSpriteUI();

	this->button = new Button(menuSystem, "", position.x, position.y + font->GetHeight("Mj") + 2, 1, 1, Align::CENTER, 0, onRelease, this->menuOffset);

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
	GLint bWidth = (size.x / (this->max - this->min + 1));
	if (bWidth < 30) bWidth = 30;
	button->SetSize(bWidth, 20);

	// Update height of slider
	this->size.y = button->GetPosition().y - position.y + button->GetSize().y;

	// Move the slider-button to the correct position
	GLint newX = 0;
	if (max - min != 0)
	{
		newX = ((this->value - min) / (max - min)) * (size.x - button->GetSize().x);
	}
	button->SetPosition(position.x + newX, button->GetPosition().y);
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
	HandleButton(button, time, position.x, position.x + size.x - button->GetSize().x, size.x - button->GetSize().x, 0, &value);
	valueString = toString(value);
}

void Slider::Draw()
{
	if (!visible)
		return;

	// Draw slider-back
	sprite->Draw(position.x, button->GetPosition().y + 5, 0.0f, size.x, 10, 1.0f, 1.0f, 1.0f, 0.1f, 49, 10, 1, 1);

	// Draw the fjutts
	/*GLfloat fjuttW = size.x / (max - min + 1);
	if (fjuttW < 1) fjuttW = 1;
	for(GLfloat i = 0; i <= size.x - (GLint)fjuttW; i += fjuttW * 2)
	{
		sprite->Draw(position.x + (GLint)i, button->GetPosition().y, 0.0f, fjuttW, 20, 1.0f, 1.0f, 1.0f, 0.1f, 49, 10, 1, 1);
	}*/

	// Draw slider-info text
	font->Draw(position.x, position.y, text);
	font->Draw(position.x + size.x - font->GetWidth(valueString), position.y, valueString);

	// Draw the button
	button->Draw();
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
	this->min = min;
	Init();
}

// Set the maximum value
void Slider::SetMax(GLint max)
{
	this->max = max;
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
			mouse.x -= menuOffset.x;

			// Move the button
			button->SetPosition(mouse.x + mouseOffset.x, button->GetPosition().y);

			// Clamp the button's movement
			GLint sliderX, buttonX;
			sliderX = position.x;
			buttonX = button->GetPosition().x;
			if (buttonX < sliderX + minPos) button->SetPosition(sliderX + minPos, button->GetPosition().y);
			if (buttonX > sliderX + maxPos) button->SetPosition(sliderX + maxPos, button->GetPosition().y);
			if (buttonX < limitXLeft) button->SetPosition(limitXLeft, button->GetPosition().y);
			if (buttonX > limitXRight) button->SetPosition(limitXRight, button->GetPosition().y);

			// Set the mouse-position to the button's position
			Input::setMousePos(Vec2(button->GetPosition().x - mouseOffset.x, button->GetPosition().y - mouseOffset.y));

			// Calculate the new value of the slider
			CalculateValue(button, maxPos, minPos, value);

			// Check if step is enabled and move button to correct position
			SnapButton(button, value, minPos, maxPos);
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
		*value = GLint((relativePos / (maxPos - minPos)) * (max - min) + min + 0.5f);
	}
}

void Slider::SnapButton(Button *button, GLfloat *value, GLint minPos, GLint maxPos)
{
	//If stepping is enabled, snap button to nearest increment
	if (step > 0 && (max - min != 0))
	{
		*value = (GLint)((*value / step) + 0.5f) * step;
		button->SetPosition(position.x + minPos + ((*value) - min) / (max - min) * (maxPos - minPos), button->GetPosition().y);
	}
}