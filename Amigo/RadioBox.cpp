#include "RadioBox.h"
#include "Helper.h"

const Vec2 RADIOBUTTON_SIZE = Vec2(18, 18);
const int SPACE_BETWEEN_RADIOBUTTONS = 10, CONTENT_PADDING = 10;

RadioBox::RadioBox(MenuSystem *menuSystem, Vec2 position, GLint width, std::string title, GLint menuID, std::function<void()> onClick)
{
	this->menuSystem = menuSystem;
	this->position = position;
	this->size.x = width;
	this->menuID = menuID;

	this->sprite = menuSystem->GetSpriteUI();

	// Create box
	box = new Box(menuSystem, title, position.x, position.y, this->size.x, this->size.y, menuID);

	// Set onClick function
	this->onClick = onClick;

	// Init idHandler
	idHandler = 0;

	// Init selection
	selected = -1;

	/* Update size of radiobox (even though it is empty at this point)
	This is done so that it doesn't just stay at 0 px height if nothing is added.*/
	SetSize(this->size);
}

void RadioBox::Unload()
{
	// Unload box
	box->Unload();

	// Unload buttons and text-items
	for (int i = 0; i < radioButtons.size(); i++)
	{
		radioButtons[i]->button->Unload();
		radioButtons[i]->textItem->Unload();
	}
}

void RadioBox::HandleInput()
{
	// Handle input of buttons
	for (int i = 0; i < radioButtons.size(); i++)
	{
		radioButtons[i]->button->HandleInput();
	}
}

void RadioBox::Update(GLdouble time)
{
	// Update
}

void RadioBox::Draw()
{
	// Draw box
	box->Draw();

	// Draw button and text-item of the radiobuttons
	for (int i = 0; i < radioButtons.size(); i++)
	{
		// Get position of radioButton's button
		Vec2 pos;
		pos = radioButtons[i]->button->GetPosition() + radioButtons[i]->button->GetDrawOffset();

		// Button
		GLint bState, offset;
		bState = radioButtons[i]->button->GetState();
		offset = bState * 20;

		sprite->Draw(Vec2(pos.x, pos.y), 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 1.0f, 46 + offset, 100, 20, 21); // rounded button
		sprite->Draw(Vec2(pos.x + 4, pos.y + 3), 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 1.0f, 106, 100, 12, 12); // tick-background

		// Text
		radioButtons[i]->textItem->Draw();

		// Draw the little tick on the selected button
		if (radioButtons[i]->id == selected)
		{
			sprite->Draw(Vec2(pos.x + 6, pos.y + 5), 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 1.0f, 106, 112, 8, 8); // tick
		}
	}
}

void RadioBox::SetPosition(Vec2 position)
{
	MenuItem::SetPosition(position);
	box->SetPosition(position);

	// Set position of buttons/textitems
	GLint tmpY;
	for (int i = 0; i < radioButtons.size(); i++)
	{
		tmpY = CONTENT_PADDING + ((RADIOBUTTON_SIZE.y + SPACE_BETWEEN_RADIOBUTTONS) * i);
		radioButtons[i]->button->SetPosition(position + Vec2(CONTENT_PADDING, tmpY));
		radioButtons[i]->textItem->SetPosition(radioButtons[i]->button->GetPosition() + Vec2(RADIOBUTTON_SIZE.x + 10, 3));
	}
}

// NB! Only the x-value (width) will be used.
void RadioBox::SetSize(Vec2 size)
{
	// The y-value will be ignored, as it is calculated based on number of buttons
	Vec2 newSize = Vec2(size.x, CONTENT_PADDING * 2);

	for (int i = 0; i < radioButtons.size(); i++)
	{
		newSize.y += radioButtons[i]->button->GetSize().y + SPACE_BETWEEN_RADIOBUTTONS;

		// Set max width of text-items while we're at it
		radioButtons[i]->textItem->SetMaxWidth(size.x - 25 - (CONTENT_PADDING * 2));
	}

	if (radioButtons.size() > 0)
		newSize.y -= SPACE_BETWEEN_RADIOBUTTONS;

	MenuItem::SetSize(newSize);
	box->SetSize(newSize);
}

void RadioBox::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);
	box->SetOrigin(origin);

	// Set origin of buttons/textitems
	for (int i = 0; i < radioButtons.size(); i++)
	{
		radioButtons[i]->button->SetOrigin(origin);
		radioButtons[i]->textItem->SetOrigin(origin);
	}
}

void RadioBox::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);
	box->SetDrawOffset(drawOffset);

	// Set drawOffset of buttons/textitems
	for (int i = 0; i < radioButtons.size(); i++)
	{
		radioButtons[i]->button->SetDrawOffset(drawOffset);
		radioButtons[i]->textItem->SetDrawOffset(drawOffset);
	}
}

// Add a radio-button to this radio-box
GLint RadioBox::AddItem(std::string text)
{
	GLint bSelID;
	bSelID = idHandler;

	//Create a button
	Button *button;
	button = new Button(menuSystem, "", Vec2(0, 0), Vec2(RADIOBUTTON_SIZE.x, RADIOBUTTON_SIZE.y), Align::CENTER, menuID, "",
		[=]()
	{
		SetSelected(bSelID);
		onClick();
	});

	// Create a text item
	Text *textItem;
	textItem = new Text(menuSystem, text, Vec2(0, 0), menuSystem->GetFontRegular(), menuID);
	textItem->SetTextMode(Text::TextMode::TRUNCATE);

	// Create the radio-button
	RadioButton *radioButton;
	radioButton = new RadioButton(button, textItem, idHandler);

	// Add radio-button to list
	radioButtons.push_back(radioButton);

	// Update position, origin and drawOffset of the items
	SetPosition(position);
	SetOrigin(origin);
	SetDrawOffset(drawOffset);

	// Update size of radiobox
	SetSize(size);

	// Handle ID-distribution (prepare for next time an item is added)
	idHandler ++;

	// If this is the first button added, set it as selected
	if (radioButtons.size() == 1)
	{
		SetSelected(radioButton->id);
	}

	// Return id of the newly created radio-button
	return radioButton->id;
}

// Remove a button from the RadioBox
void RadioBox::RemoveItem(GLint id)
{
	for (int i = 0; i < radioButtons.size(); i++)
	{
		if (radioButtons[i]->id == id)
		{
			// Unload items
			radioButtons[i]->button->Unload();
			radioButtons[i]->textItem->Unload();

			// Remove radiobutton from list
			radioButtons.erase(radioButtons.begin() + i);

			// If the selected item was just removed, reset the selection
			if (selected == id)
				selected = -1;
		}
	}

	// If the selection was reset, find a new button to select instead
	for (int i = 0; i < radioButtons.size(); i++)
	{
		SetSelected(radioButtons[i]->id);
		break;
	}

	// Update positions of items
	SetPosition(position);

	// Update size of the radiobox
	SetSize(size);
}

// Set a radio-button in the radio-box as selected
void RadioBox::SetSelected(GLint id)
{
	// Check if a button with this id exists
	for (int i = 0; i < radioButtons.size(); i++)
	{
		// If a button with this id is found; set selected
		if (radioButtons[i]->id == id)
		{
			selected = id;
			break;
		}
	}
}

// Get id of currently selected button (returns -1 if none is selected)
GLint RadioBox::GetSelected()
{
	return selected;
}

// Set the OnClick callback function called when clicking a radio-button
void RadioBox::SetOnClick(std::function<void()> onClick)
{
	this->onClick = onClick;
}