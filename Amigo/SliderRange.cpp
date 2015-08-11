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
	children.push_back(button);
	children.push_back(button2);

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
	button2->SetSize(button->GetSize());

	// Move the slider-buttons to the correct position
	SnapButton(button2, &value2, (GLint)(button->GetPosition().x + button->GetSize().x), (GLint)(position.x + size.x - button2->GetSize().x), (GLint)(size.x - button->GetSize().x), (GLint)(button->GetSize().x));
	SnapButton(button, &value, (GLint)position.x, (GLint)(button2->GetPosition().x - button->GetSize().x), (GLint)(size.x - button2->GetSize().x - button->GetSize().x), 0);
}

void SliderRange::HandleInput()
{
	// Handle input of buttons
	button->HandleInput();
	button2->HandleInput();

	// Handle the buttons
	HandleButton(button, (GLint)position.x, (GLint)(button2->GetPosition().x - button->GetSize().x), (GLint)(size.x - button2->GetSize().x - button->GetSize().x), 0, &value);
	HandleButton(button2, (GLint)(button->GetPosition().x + button->GetSize().x), (GLint)(position.x + size.x - button2->GetSize().x), (GLint)(size.x - button->GetSize().x), (GLint)(button->GetSize().x), &value2);
}

void SliderRange::Update(GLdouble time)
{
	if (!(active && visible))
		return;

	// Update buttons
	button->Update(time);
	button2->Update(time);

	// Set valuestring
	valueString = toString(value) + " - " + toString(value2);
}

void SliderRange::Draw()
{
	if (!visible)
		return;

	GLfloat alpha;
	alpha = 1.0f;

	GLint x, y;
	x = (GLint)(position.x) + drawOffset.x;
	y = (GLint)(button->GetPosition().y + button->GetSize().y / 2 - 2) + drawOffset.y;

	// Draw the elements that are common for the sliders
	DrawCommonElements(x, y, alpha);
	
	// Draw filled part
	sprite->Draw(
		(GLint)(button->GetPosition().x + button->GetSize().x) + drawOffset.x,
		y,
		0.0f,
		(GLfloat)(button2->GetPosition().x - (button->GetPosition().x + button->GetSize().x) + 1),
		1.0f, alpha, 17, 31, 1, 7); // Fill

	// Draw buttons
	button->Draw();
	button2->Draw();

	//sprite->Draw((GLint)(button->GetPosition().x + drawOffset.x + 5), (GLint)(button->GetPosition().y + drawOffset.y + 4), 0.0f, 1.0f, 1.0f, alpha, 0, 104, 21, 10); // Metal graphic
	//sprite->Draw((GLint)(button2->GetPosition().x + drawOffset.x + 5), (GLint)(button2->GetPosition().y + drawOffset.y + 4), 0.0f, 1.0f, 1.0f, alpha, 0, 104, 21, 10); // Metal graphic
}

// Set the value of the slider
void SliderRange::SetValue(GLfloat value)
{
	// Clamp value-parameter and assign to variable
	if (value > max) value = max;
	if (value < min) value = min;
	this->value = value;

	// Do some stuff
	Init();
}

void SliderRange::SetSize(Vec2 size)
{
	MenuItem::SetSize(size);

	SnapButton(button, &value, (GLint)position.x, (GLint)(button2->GetPosition().x - button->GetSize().x), (GLint)(size.x - button2->GetSize().x - button->GetSize().x), 0);
	SnapButton(button2, &value2, (GLint)(button->GetPosition().x + button->GetSize().x), (GLint)(position.x + size.x - button2->GetSize().x), (GLint)(size.x - button->GetSize().x), (GLint)(button->GetSize().x));
}

void SliderRange::SetPosition(Vec2 position)
{
	button->SetPosition(button->GetPosition() + position - this->position);
	button2->SetPosition(button2->GetPosition() + position - this->position);
	MenuItem::SetPosition(position);
}

void SliderRange::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);
	button->SetOrigin(origin);
	button2->SetOrigin(origin);
}

void SliderRange::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);
	button->SetDrawOffset(drawOffset);
	button2->SetDrawOffset(drawOffset);
}