#include "Checkbox.h"

const int TEXT_DISTANCE_FROM_CHECKBOX = 12; // distance of text from box, in pixels

Checkbox::Checkbox(MenuSystem *menuSystem, Vec2 position, std::string text, GLint menuID, std::function<void()> onClick)
{
	this->menuSystem = menuSystem;
	this->position = position;
	this->text = text;
	this->menuID = menuID;
	this->onClick = onClick;

	// Get font and sprite
	this->font = menuSystem->GetFontRegular();
	this->sprite = menuSystem->GetSpriteUI();

	// Init button
	button = new Button(this->menuSystem, "", Vec2(0, 0), Vec2(18, 18), Align::CENTER, this->menuID, "", [=]() {
		SetToggled(!toggled);
		onClick();
	});
	children.push_back(button);

	// Init text-item
	textItem = new Text(this->menuSystem, this->text, Vec2(0, 0), font, this->menuID);

	// Init toggled-state
	toggled = false;

	// Do a set-position to place the button and textItem correctly
	SetPosition(this->position);

	// Update size (it's measured from the text)
	UpdateSize();
}

void Checkbox::Unload()
{
	// Unload button
	button->Unload();

	// Unload text-item
	textItem->Unload();
}

void Checkbox::HandleInput()
{
	// Handle input on button
	button->HandleInput();
}

void Checkbox::Update(GLdouble time)
{
	// Update button
	button->Update(time);

	// Update text-item
	textItem->Update(time);
}

void Checkbox::Draw()
{
	// Get properties for drawing
	GLint x, y;
	x = position.x + drawOffset.x;
	y = position.y + drawOffset.y;

	// Draw button
	button->Draw();

	// Draw checkbox-background
	sprite->Draw(Vec2(x + 3, y + 3), 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 1.0f, 21, 100, 12, 12);

	// Draw checkmark
	if (toggled)
	{
		sprite->Draw(Vec2(x + 3, y + 2), 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 1.0f, 33, 100, 13, 13);
	}

	// Draw text-item
	textItem->Draw();
}

void Checkbox::SetSize(Vec2 size)
{
	/*do nothing*/
}

void Checkbox::SetPosition(Vec2 position)
{
	MenuItem::SetPosition(position);
	button->SetPosition(position);
	textItem->SetPosition(position + Vec2(16 + TEXT_DISTANCE_FROM_CHECKBOX, 2));
}

void Checkbox::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);
	button->SetOrigin(origin);
	textItem->SetOrigin(origin);
}

void Checkbox::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);
	button->SetDrawOffset(drawOffset);
	textItem->SetDrawOffset(drawOffset);
}

// Set the toggled-state of this checkbox
void Checkbox::SetToggled(bool toggled)
{
	this->toggled = toggled;
}

// Get the toggled-state of this checkbox
bool Checkbox::GetToggled()
{
	return toggled;
}

// Set the info-text displayed to the right of the checkbox
void Checkbox::SetText(std::string text)
{
	this->text = text;

	// Update the text-item's text
	textItem->SetText(this->text);

	// Update size of this checkbox
	UpdateSize();
}

// Update the size of this checkbox to make sure the text is also being measured
void Checkbox::UpdateSize()
{
	size = Vec2(16 + TEXT_DISTANCE_FROM_CHECKBOX + font->GetWidth(text), 16);
}