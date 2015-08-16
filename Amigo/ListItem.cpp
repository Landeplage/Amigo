#include "ListItem.h"

#include "Helper.h" //<- debug

ListItem::ListItem(MenuSystem *menuSystem, std::function<void()> onClick)
{
	this->menuSystem = menuSystem;

	// Set sprite and font
	sprite = menuSystem->GetSpriteUI();
	font = menuSystem->GetFontRegular();

	// Init button
	button = new Button(menuSystem, "", Vec2(0, 0), Vec2(0, 0), Align::CENTER, -1, "", onClick);
	children.push_back(button);

	// Init selection
	isSelected = false;
}

void ListItem::Unload()
{
	//Unload button
	button->Unload();
}

void ListItem::HandleInput()
{
	// Handle input on button
	button->HandleInput();
}

void ListItem::Update(GLdouble time)
{
	// Update button
	button->Update(time);
}

void ListItem::Draw()
{
	// Draw item back (debug)
	if (button->GetState() > 0)
	{
		Color color = Color(255, 255, 255);
		if (button->GetState() == 1)
			color = Color(0, 0, 0);
		if (button->GetState() == 2)
			color = Color(255, 255, 255);
		sprite->Draw(position + drawOffset, 0.0f, size - Vec2(-1, 0), color, 0.05f, 0, 72, 1, 1);
	}

	if (isSelected)
		sprite->Draw(position + drawOffset, 0.0f, Vec2(size.x, 1.0f), 1.0f, 40, 47, 1, 24);

	// Draw text
	Vec2 pos;
	GLint xOff;
	MenuItem::Align align;

	pos = position + drawOffset + Vec2(10, 4);
	for (int u = 0; u < strings.size(); u++)
	{
		align = MenuItem::Align::LEFT;

		// Place the text-items horizontally
		if (strings.size() > 1) // if there is more than one textItem in this item
		{
			if (u == strings.size() - 1) // if this is the last textItem
			{
				pos.x = size.x - 10;
				align = MenuItem::Align::RIGHT;
			}
			else if (u > 0) // if this is not the first textItem
			{
				pos.x = 10 + ((size.x - 20) / (strings.size() - 1) * u);
			}
		}

		// Align text correctly, if needed.
		xOff = 0;
		if (align == MenuItem::Align::RIGHT)
			xOff = font->GetWidth(strings[u]);

		// Draw text
		font->Draw(pos - Vec2(xOff, -1), strings[u], 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 0.6f); // highlight
		font->Draw(pos - Vec2(xOff, 0), strings[u], 0.0f, Vec2(1.0f, 1.0f), Color(139, 98, 38), 1.0f); // text
	}
}

void ListItem::SetPosition(Vec2 position)
{
	MenuItem::SetPosition(position);

	// Update button's position
	button->SetPosition(position);
}

void ListItem::SetSize(Vec2 size)
{
	MenuItem::SetSize(size);

	// Update button's size
	button->SetSize(size);
}

void ListItem::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);

	// Update button's origin
	button->SetOrigin(origin);
}

void ListItem::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);

	// Update button's drawOffset
	button->SetDrawOffset(drawOffset);
}

// Add a string to this list item
void ListItem::AddItem(std::string text)
{
	strings.push_back(text);
}

// Set the onClick function of the list-item's button
void ListItem::SetOnClick(std::function<void()> onClick)
{
	button->SetOnClick(onClick);
}

// Set whether or not this item is selected (used only for visuals)
void ListItem::SetIsSelected(bool isSelected)
{
	this->isSelected = isSelected;
}