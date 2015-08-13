#include "Dropdown.h"
#include "MenuSystem.h"
#include "Input.h"
#include "Context.h"

#include "Helper.h" // used in HandleItems and for clamping

// Dropdown constants
const int
_ITEMHEIGHT = 21; // height of each item (in pixels)

// Scrollerbar constants
const int
SB_WIDTH = 14,
SB_BMINHEIGHT = 30;

Dropdown::Dropdown(MenuSystem* menuSystem, std::string text, Vec2 position, Vec2 size, GLint menuID, std::string tooltip, std::function<void()> onClick)
{
	printf("Dropdown created.");

	this->menuSystem = menuSystem;
	this->text = "<empty>";
	this->tooltip = tooltip;
	this->position = position;
	this->size = size;
	this->menuID = menuID;
	this->function = onClick;

	this->font = menuSystem->GetFontRegular();
	this->sprite = menuSystem->GetSpriteUI();

	// Dropdown-button
	button = new Button(menuSystem, this->text, position, size, Align::LEFT, menuID, tooltip,
		[=]() {
		if (dropdownItems.size() > 0) // Check if there is any point to activating this dropdown
		{
			menuSystem->ResetFocus();
			this->toggleDropdownStatus();
			menuSystem->SetFocus(this);
		}
	});

	// Item handling variables initialize
	itemHover = -1;
	idHandler = 0;
	selectedItem = NULL;

	// Rendertargets initialize
	rt = new RenderTarget(0, 0); // item-list rendertarget
	hrt = new RenderTarget(0, 0); // highlight-box rendertarget

	// Initialize scrolling variables
	scroll = 0;
	scrollPrev = 0;
	maxScroll = 0;
	dropdownOffset = 0; // offsets the entire dropdown list, in case it would go beyond the screen's bottom
	maxDropdownHeight = 0;
	SetMaxDropdownHeight(400); // Set the default value of how tall the dropdown can be before the scroller appears

	// Add button to the list of children
	children.push_back(button);
}

void Dropdown::HandleInput()
{
	// Handle input of button
	button->HandleInput();

	// Remember last update's scroll value
	scrollPrev = scroll;

	// Handle scrolling
	scroll += Input::getMouseWheel() * 10;

	// Clamp scrolling
	scroll = Clamp(scroll, -maxScroll, 0);

	// If the scrolling has changed...
	if (scrollPrev != scroll)
	{
		//Update the rendertarget
		UpdateRendertarget();
	}
}

void Dropdown::Update(GLdouble time)
{
	// Update button
	button->Update(time);

	// If a message/question appears, the dropdown should cancel out
	if (menuSystem->GetOverlayShow() && menuSystem->IsDropdownActive())
	{
		toggleDropdownStatus();
	}

	// Update the dropdownOffset (to prevent dropdown from going off of bottom of screen)
	UpdateDropdownOffset();
}

void Dropdown::Draw()
{
	// Draw button
	button->Draw();

	// Draw details on dropdown button
	Vec2 bPos, bDof;
	bPos = button->GetPosition();
	bDof = button->GetDrawOffset();

	sprite->Draw(Vec2(bPos.x + bDof.x + size.x - 30, bPos.y + bDof.y + 2), 0.0f, Vec2(1.0f, (GLfloat)((size.y - 4) / 24)), 1.0f, 37, 47, 2, 24); // line
	sprite->Draw(Vec2(bPos.x + bDof.x + size.x - 20, bPos.y + bDof.y + (GLint)size.y / 2 - 3), 0.0f, Vec2(1.0f, 1.0f), 1.0f, 31, 89, 10, 6); // triangle
}

void Dropdown::Unload()
{
	// Unload button
	button->Unload();

	// Destroy rendertargets
	rt->~RenderTarget();
	hrt->~RenderTarget();
}

// Show/hide dropdown
void Dropdown::toggleDropdownStatus()
{
	if (menuSystem->IsDropdownActive())
	{
		menuSystem->DeactivateDropdown();
	}
	else
	{
		menuSystem->ActivateDropdown(this);
	}
}

void Dropdown::SetSize(Vec2 size)
{
	MenuItem::SetSize(size);
	button->SetSize(size);

	RefreshButtonText(); // Refresh the button's text
	TruncateDisplayTextOnAllItems(); // Make sure texts are truncated correctly according to the new width.
	UpdateRendertarget(); // Update the rendertarget, since the displaytexts have been truncated
}

void Dropdown::SetPosition(Vec2 position)
{
	button->SetPosition(button->GetPosition() + position - this->position);
	MenuItem::SetPosition(position);
}

void Dropdown::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);
	button->SetOrigin(origin);
}

void Dropdown::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);
	button->SetDrawOffset(drawOffset);
}

// Add an item to the dropdown
GLint Dropdown::AddItem(std::string text)
{
	// Create new dropdownItem
	dropdownItem* di;
	di = new dropdownItem(idHandler, text);

	// Truncate the displayText if need be
	TruncateDisplayTextOnItem(di);

	// Add item to list
	dropdownItems.push_back(di);

	// If no item is selected, make the newly added item selected
	if (selectedItem == NULL)
	{
		SetSelectedItem(idHandler);
	}

	// Increment idHandler, so it's ready for the next time an item is added
	idHandler++;

	// Update the rendertarget
	UpdateRendertarget();

	// Update scroller variables
	UpdateScrollVariables();

	// Return ID of the item added
	return idHandler;
}

// Remove an item from the dropdown
void Dropdown::RemoveItem(GLint id)
{
	// First, check if an item with the given ID exists in the list
	GLint index = -1;
	for (int i = 0; i < dropdownItems.size(); i++)
	{
		if (dropdownItems[i]->id == id)
		{
			index = i;
			break;
		}
	}

	// If no matching item was found, return
	if (index == -1)
		return;

	// Check if item we want to remove is the selected item
	bool reselect = false;
	if (GetSelectedItemID() == id)
	{
		// If so, set selection to NULL
		selectedItem = NULL;
		reselect = true; // set reselection-flag
	}

	// Remove item from list
	dropdownItems.erase(dropdownItems.begin() + index);

	// If reselection-flag was set...
	if (reselect)
	{
		// ...and if there are any items to select
		if (dropdownItems.size() > 0)
			SetSelectedItem(dropdownItems[0]->id); // Set selection to first item in list
		else
			SetSelectedItem(-1); // If the list is empty, we give SetSelectedItem a negative ID and it will reset the selection
	}

	// If the dropdown is currently active and empty, we should deactivate it
	if (menuSystem->IsDropdownActive() && dropdownItems.size() == 0)
	{
		toggleDropdownStatus();
		menuSystem->ResetFocus();
	}

	// Update the rendertarget
	UpdateRendertarget();

	// Update scroller variables
	UpdateScrollVariables();
}

// Handle the items
void Dropdown::HandleItems()
{
	// Get mouse input
	Vec2 mouse = Input::getMousePos();
	mouse = mouse - origin;
	mouse.y -= 5; // offset cursor slightly
	mouse.y -= scroll; // offset cursor in accordance with scrolling
	mouse.y -= dropdownOffset; // offset cursor in accordance with the off-screen-safe-guard

	// Set height to check mouse-input in
	GLint h;
	h = dropdownItems.size() * _ITEMHEIGHT;
	if (h > maxDropdownHeight)
		h = maxDropdownHeight;

	// Expand height slightly
	GLint hup;
	hup = 4;

	// Do we need to make room for the scroll-indicator?
	GLint trOff = 0;
	if (maxScroll > 0)
		trOff = 5;

	// Check if mouse is inside the dropdown at all
	bool isInside = false;
	if (IsInside(mouse, (GLint)position.x, (GLint)(position.y + size.y - scroll - hup), (GLint)(position.x + size.x - trOff), (GLint)(position.y + size.y + h - scroll + hup)))
	{
		isInside = true;
	}
	else
	{
		itemHover = -1;
		return;
	}

	if (isInside)
	{
		// Check what item the mouse is hovering on
		for (int i = 0; i < dropdownItems.size(); i++)
		{
			if (IsInside(mouse, position.x, position.y + size.y + i * _ITEMHEIGHT, position.x + size.x, position.y + size.y + i * _ITEMHEIGHT + _ITEMHEIGHT))
			{
				itemHover = i;
				UpdateHighlightRendertarget();
				break;
			}
			else
				itemHover = -1;
		}

		if (Input::getMouseLeftReleased())
		{
			if (itemHover != -1)
				SetSelectedItem(dropdownItems[itemHover]->id);
			function(); // <- Do onClick function
		}
	}
}

// Draws the items of the dropdown
void Dropdown::DrawItems()
{
	// Prepare the variables for drawing the list-background
	GLfloat alpha = 1.0f;
	GLint x, y, w, h, xx, yy;
	x = (GLint)position.x + origin.x;
	y = (GLint)position.y + size.y + origin.y + dropdownOffset;
	w = (GLint)size.x;
	h = (GLint)(GetDropdownHeight() + 6);

	xx = 30;
	yy = 77;

	// Draw a box to visualize max-height
	//menuSystem->GetSpriteUI()->Draw(x, y, 0.0f, w, maxDropdownHeight, Color(255, 255, 255), 1.0f, 5, 5, 1, 1);

	// Compensate x, w and h for edges and shadow of the texture
	x -= 1;
	w += 2;
	h += 6;

	// Draw the list-background
	sprite->DrawRectangleFromTexture(Vec2(x, y), Vec2(w, h), Vec2(xx, yy), Vec2(5, 5), Vec2(1, 1));

	/*
	sprite->Draw(x + 5, y + 5, 0.0f, (GLfloat)(w - 10), (GLfloat)(h - 10), alpha, xx + 5, yy + 5, 1, 1);		// Middle
	sprite->Draw(x, y, 0.0f, 1.0f, 1.0f, alpha, xx, yy, 5, 5);		// Left top corner
	sprite->Draw(x, y + 5, 0.0f, 1.0f, (GLfloat)(h - 10), alpha, xx, yy + 5, 5, 1);		// Left side
	sprite->Draw(x, y + h - 5, 0.0f, 1.0f, 1.0f, alpha, xx, yy + 6, 5, 5);		// Left bottom corner
	sprite->Draw(x + 5, y + h - 5, 0.0f, (GLfloat)(w - 10), 1.0f, alpha, xx + 5, yy + 6, 1, 5);		// Bottom
	sprite->Draw(x + w - 5, y + h - 5, 0.0f, 1.0f, 1.0f, alpha, xx + 6, yy + 6, 5, 5);		// Right bottom corner
	sprite->Draw(x + w - 5, y + 5, 0.0f, 1.0f, (GLfloat)(h - 10), alpha, xx + 6, yy + 5, 5, 1);		// Right side
	sprite->Draw(x + w - 5, y, 0.0f, 1.0f, 1.0f, alpha, xx + 6, yy, 5, 5);		// Right top corner
	sprite->Draw(x + 5, y, 0.0f, (GLfloat)(w - 10), 1.0f, alpha, xx + 5, yy, 1, 5);		// Top*/

	// Draw rendertarget (containing the item-texts)
	rt->Draw(position.x + origin.x, y + 1);

	// Draw highlight-box rendertarget
	if (itemHover != -1)
	{
		for (int i = 0; i < dropdownItems.size(); i++)
		{
			if (itemHover == i)
			{
				// Make sure the edges are cut off if needed
				GLint yy, topcut, botcut;
				yy = (i * _ITEMHEIGHT) + scroll;

				topcut = 0;
				botcut = 0;

				if (yy < -2)
					topcut = yy + 2;
				if (yy > rt->GetSize().y - 27)
					botcut = (rt->GetSize().y - 27) - yy;

				// Do we need to make room for the scroll-indicator?
				GLint trOff = 0;
				if (maxScroll > 0)
					trOff = 5;

				// Draw the highlight-box rendertarget
				hrt->Draw(position.x + origin.x + 1, y + 4 + yy - topcut, 0.0f, 1.0f, 1.0f, 1.0f, 0, -botcut, hrt->GetSize().x - trOff, 24 + topcut + botcut);
			}
		}
	}

	// Draw small scroll-indicator
	if (maxScroll > 0)
	{
		GLfloat indH, yOff;
		indH = (GLfloat)maxDropdownHeight / ((GLfloat)(dropdownItems.size() * _ITEMHEIGHT)) * ((GLfloat)(maxDropdownHeight + 7)); // indicator height
		yOff = floor(abs((GLfloat)scroll / (GLfloat)maxScroll) * (((GLfloat)maxDropdownHeight + 7) - indH)); // indicator movement

		sprite->Draw(Vec2(position.x + origin.x + size.x - 6, y + 1), 0.0f, Vec2(1, maxDropdownHeight + 9), Color(0, 0, 0), 0.25f, 5, 5, 1, 1); // line
		sprite->Draw(Vec2(position.x + origin.x + size.x - 4, y + 2 + yOff), 0.0f, Vec2(2, indH), Color(0, 0, 0), 0.25f, 5, 5, 1, 1); // indicator
	}
}

// Set the dropdown to given item
void Dropdown::SetSelectedItem(GLint id)
{
	selectedItem = GetItem(id);
	if (selectedItem != NULL)
		RefreshButtonText();
	else
		button->SetText("<empty>", Align::LEFT);
	itemHover = -1;
}

// Get the ID of the selected item
GLint Dropdown::GetSelectedItemID()
{
	if (selectedItem == NULL)
		return -1;
	
	return selectedItem->id;
}

// Get the selected item's text
std::string Dropdown::GetSelectedItemText()
{
	if (selectedItem == NULL)
		return "";
	
	return selectedItem->text;
}

// Get pointer to an item based on the id of the item (returns NULL if none was found)
Dropdown::dropdownItem* Dropdown::GetItem(GLint id)
{
	for (int i = 0; i < dropdownItems.size(); i++)
	{
		if (dropdownItems[i]->id == id)
			return dropdownItems[i];
	}

	// If none was found, return NULL
	return NULL;
}

// Truncate display-text on all items (used when resizing the dropdown)
void Dropdown::TruncateDisplayTextOnAllItems()
{
	for (int i = 0; i < dropdownItems.size(); i++)
	{
		TruncateDisplayTextOnItem(dropdownItems[i]);
	}
}

// Truncate display-text on one item
void Dropdown::TruncateDisplayTextOnItem(dropdownItem* item)
{
	// Do we need to make room for the scroll-indicator?
	GLint trOff = 0;
	if (maxScroll > 0)
		trOff = 6;

	// Truncate text
	item->displayText = font->TruncateString(item->text, size.x - 12 - trOff);
}

// Refresh the button's text (used for cases where it changed width, or changed selection)
void Dropdown::RefreshButtonText()
{
	if (button == NULL) // make sure button exists
		return;
	if (selectedItem == NULL) // make sure an item is selected
		return;
	button->SetText(font->TruncateString(selectedItem->text, size.x - 39), Align::LEFT); // Set button's text (truncated to fit)
}

// Updates the size of the rendertarget, and renders the dropdown-items to it
void Dropdown::UpdateRendertarget()
{
	//Set the size of the rendertarget
	rt->SetSize(Vec2(size.x, GetDropdownHeight() + 9));

	// Prepare rendertarget
	rt->Begin();

	// Uncomment to make size of rendertarget visible
	//menuSystem->GetSpriteUI()->Draw(0, 0, 0.0f, 1280.0f, 720.0f, Color(255, 255, 255), 0.5f, 5, 5, 1, 1);

	// Draw item texts
	for (int i = 0; i < dropdownItems.size(); i++)
	{
		font->Draw(Vec2(9, (i * _ITEMHEIGHT) + 7 + scroll), dropdownItems[i]->displayText, 0.0f, Vec2(1.0f, 1.0f), Color(139, 98, 38), 1.0f);
	}

	// Perform render
	rt->End();
}

// Set maxScroll, activate scrollButton, etc.
void Dropdown::UpdateScrollVariables()
{
	// Get current total height
	GLint height;
	height = dropdownItems.size() * _ITEMHEIGHT;

	// If the item-list is taller than the set maximum height...
	if (height > maxDropdownHeight)
	{
		// Set how far we can scroll
		maxScroll = height - maxDropdownHeight;
	}
	else
	{
		maxScroll = 0;
	}
}

// Returns the height of the item-list/dropdown
GLint Dropdown::GetDropdownHeight()
{
	GLint height;
	height = dropdownItems.size() * _ITEMHEIGHT;
	if (height > maxDropdownHeight)
		height = maxDropdownHeight;
	return height;
}

// Update the rendertarget for the small highlight-box
void Dropdown::UpdateHighlightRendertarget()
{
	// Set size
	hrt->SetSize(Vec2(size.x - 2, 24));

	// Prepare for drawing into it
	hrt->Begin();

	// Draw highlight-box
	if (itemHover != -1)
	{
		for (int i = 0; i < dropdownItems.size(); i++)
		{
			if (itemHover == i)
			{
				sprite->Draw(Vec2(0, 0), 0.0f, Vec2(hrt->GetSize().x, 1.0f), 1.0f, 40, 47, 1, 24); // highlight-box
				font->Draw(Vec2(8, 4 + 1), dropdownItems[i]->displayText, 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), 0.5f); // text-highlight
				font->Draw(Vec2(8, 4), dropdownItems[i]->displayText, 0.0f, Vec2(1.0f, 1.0f), Color(139, 98, 38), 1.0f); // text
			}
		}
	}

	// Perform render
	hrt->End();
}

// Set the maximum amount of pixels the dropdown can show (before the scroller appears).
void Dropdown::SetMaxDropdownHeight(GLint height)
{
	maxDropdownHeight = height;

	// Limit the max dropdown height to screen's height (with some padding)
	GLint maxH;
	maxH = Context::getWindowHeight() - 20;
	if (maxDropdownHeight > maxH)
		maxDropdownHeight = maxH;

	// Update the rendertarget
	UpdateRendertarget();

	// Update scroller variables
	UpdateScrollVariables();
}

// Update the dropdown offset, in case the dropdown would go beyond the screen's bottom
void Dropdown::UpdateDropdownOffset()
{
	// Get the current height of the dropdown-list
	GLint height;
	height = dropdownItems.size() * _ITEMHEIGHT;
	if (height > maxDropdownHeight)
		height = maxDropdownHeight;

	// Check if it would go beyond screen's bottom
	GLint dropdownBotY, windowBotY; // (dropdown bottom Y, window bottom Y)
	dropdownBotY = position.y + origin.y + size.y + height; // Calculate bottom y-value of dropdown
	windowBotY = Context::getWindowHeight() - 15; // Get bottom of window and add some padding
	
	// Apply offset, and make sure it doesn't go into positive values
	dropdownOffset = windowBotY - dropdownBotY;
	if (dropdownOffset > 0)
		dropdownOffset = 0;
}