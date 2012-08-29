#include "SliderRange.h"
#include "MenuSystem.h"
#include "Helper.h"

SliderRange::SliderRange(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease, Vec2 menuOffset)
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
	this->button2 = new Button(menuSystem, "", position.x, position.y + font->GetHeight("Mj") + 2, 1, 1, Align::CENTER, 0, onRelease, this->menuOffset);

	this->value = min;
	this->value2 = max;
	mouseOffset = Vec2(0, 0);

	valueString = "?";

	Init();
}

void SliderRange::Init()
{
	Slider::Init();

	// Set size of button2
	button2->SetSize(button->GetSize().x, button->GetSize().y);

	// Move the slider-button to the correct position
	GLint newX = 0;
	if (max - min != 0)
	{
		newX = ((this->value - min) / (max - min)) * (size.x - button->GetSize().x - button2->GetSize().x);
	}
	button->SetPosition(position.x + newX, button->GetPosition().y);

	// Move button2 to the correct position
	newX = 0;
	if (max - min != 0)
	{
		newX = ((this->value2 - min) / (max - min)) * (size.x - button2->GetSize().x - button2->GetSize().x);
	}
	button2->SetPosition(position.x + newX, button->GetPosition().y);
}

void SliderRange::Update(GLdouble time)
{
	if (!(active && visible))
		return;

	// Handle the buttons
	HandleButton(button, time, 0, button2->GetPosition().x - button->GetSize().x, size.x - button2->GetSize().x - button->GetSize().x, 0, &value);
	HandleButton(button2, time, button->GetPosition().x + button->GetSize().x, position.x + size.x - button2->GetSize().x, size.x - button->GetSize().x, button->GetSize().x, &value2);

	// Set string
	valueString = toString(value) + " - " + toString(value2);
}

void SliderRange::Draw()
{
	if (!visible)
		return;

	Slider::Draw();

	button2->Draw();

	// Draw a popup displaying the difference
	if (menuSystem->GetFocus() == button || menuSystem->GetFocus() == button2)
	{
		std::string diffStr;
		diffStr = toString(value2 - value);
		GLint diffStrX, diffStrW;
		diffStrW = font->GetWidth(diffStr);
		diffStrX = (button->GetPosition().x + button->GetSize().x / 2 + button2->GetPosition().x + button2->GetSize().x / 2) / 2;
		sprite->Draw(diffStrX - diffStrW / 2 - 2, button->GetPosition().y, 0.0f, diffStrW + 4, 20, 1.0f, 1.0f, 1.0f, 1.0f, 49, 10, 1, 1);
		font->Draw(diffStrX - diffStrW / 2, button->GetPosition().y + 3, diffStr);
	}
}