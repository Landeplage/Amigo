#include "SliderRange.h"
#include "MenuSystem.h"
#include "Helper.h"

SliderRange::SliderRange(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease)
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
	this->button2 = new Button(menuSystem, "", (GLint)position.x, (GLint)(position.y + font->GetHeight("Mj")), 1, 1, CENTER, 0, "", onRelease);

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
	button2->SetSize((GLint)button->GetSize().x, (GLint)button->GetSize().y);

	// Move the slider-buttons to the correct position	
	SnapButton(button2, &value2, (GLint)(button->GetPosition().x + button->GetSize().x), (GLint)(position.x + size.x - button2->GetSize().x), (GLint)(size.x - button->GetSize().x), (GLint)(button->GetSize().x));
	SnapButton(button, &value, (GLint)position.x, (GLint)(button2->GetPosition().x - button->GetSize().x), (GLint)(size.x - button2->GetSize().x - button->GetSize().x), 0);

	// Set valuestring
	valueString = toString(value) + " - " + toString(value2);
}

void SliderRange::Update(GLdouble time)
{
	if (!(active && visible))
		return;

	// Handle the buttons
	HandleButton(button, time, (GLint)position.x, (GLint)(button2->GetPosition().x - button->GetSize().x), (GLint)(size.x - button2->GetSize().x - button->GetSize().x), 0, &value);
	HandleButton(button2, time, (GLint)(button->GetPosition().x + button->GetSize().x), (GLint)(position.x + size.x - button2->GetSize().x), (GLint)(size.x - button->GetSize().x), (GLint)(button->GetSize().x), &value2);

	// Set valuestring
	valueString = toString(value) + " - " + toString(value2);
}

void SliderRange::Draw(GLfloat transition)
{
	if (!visible)
		return;

	GLfloat alpha;
	alpha = 1 - abs(transition);

	GLint x, y;
	x = (GLint)(position.x);
	y = (GLint)(button->GetPosition().y + button->GetSize().y / 2 - 2);

	// Draw the elements that are common for the sliders
	DrawCommonElements(x, y, alpha);
	
	// Draw filled part
	sprite->Draw(
		(GLint)(button->GetPosition().x + button->GetSize().x),
		y,
		0.0f,
		(GLfloat)(button2->GetPosition().x - (button->GetPosition().x + button->GetSize().x)),
		1.0f, 1.0f, 1.0f, 1.0f, alpha, 17, 31, 1, 7); // Fill

	// Draw buttons
	//button->Draw(transition);
	//button2->Draw(transition);
	sprite->Draw((GLint)(button->GetPosition().x - 1), (GLint)(button->GetPosition().y), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 45, 25, 26); // Scroller-button
	sprite->Draw((GLint)(button2->GetPosition().x - 1), (GLint)(button2->GetPosition().y), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 45, 25, 26); // Scroller-button
}

// Set the value of the slider
void SliderRange::SetValue(GLfloat value)
{
	// Clamp value-parameter and assign to variable
	if (value > max) value = max;
	if (value < min) value = min;
	this->value = value;

	Init();
}