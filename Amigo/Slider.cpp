#pragma once
#include "Slider.h"
#include "MenuSystem.h"
#include "Helper.h"
#include "Input.h"

Slider::Slider(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLint menuID, std::function<void()> onRelease, Vec2 menuOffset)
{
	this->menuSystem = menuSystem;
	this->text = text;
	this->position = position;
	this->size.x = width;
	this->min = min;
	this->max = max;
	this->menuID = menuID;
	this->onRelease = onRelease;
	this->menuOffset = menuOffset;

	this->font = menuSystem->GetFontRegular();
	this->sprite = menuSystem->GetSpriteUI();

	this->button = new Button(menuSystem, "", position.x, position.y + font->GetHeight("Mj") + 2, 60, 20, Align::CENTER, 0, onRelease, this->menuOffset);
	this->size.y = button->GetPosition().y - position.y + button->GetSize().y;

	this->value = min;
	mouseOffset = Vec2(0, 0);
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

	// Sync and update the button
	button->active = active;
	button->visible = visible;
	button->Update(time);

	// Get the mouse-offset as the button is clicked
	if (button->GetHasBeenClicked())
	{
		mouseOffset = Vec2((button->GetPosition().x - Input::getMousePos().x), (button->GetPosition().y - Input::getMousePos().y));
	}

	// Button is pressed
	if (Input::getMouseLeft() && menuSystem->GetFocus() == button)
	{
		Vec2 mouse = Input::getMousePos();
		mouse.x -= menuOffset.x;

		// Move the button
		button->SetPosition(mouse.x + mouseOffset.x, button->GetPosition().y);

		// Clamp the button's movement
		GLint sliderX, buttonX;
		sliderX = position.x;
		buttonX = button->GetPosition().x;
		if (buttonX < sliderX) button->SetPosition(sliderX, button->GetPosition().y);
		if (buttonX + button->GetSize().x > sliderX + size.x) button->SetPosition(sliderX + size.x - button->GetSize().x, button->GetPosition().y);

		// Set the mouse-position to the button's position
		Input::setMousePos(Vec2(button->GetPosition().x - mouseOffset.x, button->GetPosition().y - mouseOffset.y));

		// Calculate the new value of the slider
		GLfloat moved, maxMove;
		moved = button->GetPosition().x - position.x;
		maxMove = size.x - button->GetSize().x;
		value = (moved / maxMove) * (max - min) + min;
	}
}

void Slider::Draw()
{
	if (!visible)
		return;

	// Draw slider-background
	sprite->Draw(position.x, button->GetPosition().y, 0.0f, size.x, 20, 1.0f, 1.0f, 1.0f, 1.0f, 49, 10, 1, 1);

	// Draw slider-info text
	font->Draw(position.x, position.y, text);
	std::string valueStr;
	valueStr = toString((GLint)(value + 0.5f));
	font->Draw(position.x + size.x - font->GetWidth(valueStr), position.y, valueStr);

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

	// Move the slider-button to the correct position
	GLint newX;
	newX = ((this->value - min) / (max - min)) * (size.x - button->GetSize().x);
	button->SetPosition(position.x + newX, button->GetPosition().y);
}

// Return the current value
GLfloat Slider::GetValue()
{
	return value;
}