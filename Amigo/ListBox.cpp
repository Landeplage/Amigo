#include "ListBox.h"
#include "Input.h"
#include "Helper.h"

// Listbox constants
const int
_ITEMHEIGHT = 24; // height of each item (in pixels)

// Scrollerbar constants
const int
SB_WIDTH = 14,
SB_BMINHEIGHT = 30;

ListBox::ListBox(MenuSystem *menuSystem, Vec2 position, Vec2 size, std::string title, GLint menuID, std::function<void()> onClick)
{
	this->menuSystem = menuSystem;
	this->menuID = menuID;
	this->position = position;
	this->size = size;
	this->onClick = onClick;

	// Init box
	contentBox = new ContentBox(menuSystem, title, position.x, position.y, size.x, size.y, menuID);
	contentBox->SetHorizontalPadding(0);
	contentBox->SetVerticalPadding(5);
	contentBox->SetResizeMode(ContentBox::ResizeMode::RESIZE_WIDTH);
	children.push_back(contentBox);

	// Init selection
	selectedItem = NULL;
}

void ListBox::Unload()
{
	// Unload contentbox
	contentBox->Unload();
}

void ListBox::HandleInput()
{
	// Handle input on contentbox
	contentBox->HandleInput();
}

void ListBox::Update(GLdouble time)
{
	// Update contentbox
	contentBox->Update(time);
}

void ListBox::Draw()
{
	// Draw box
	contentBox->Draw();
}

void ListBox::SetPosition(Vec2 position)
{
	MenuItem::SetPosition(position);

	// Update box position
	contentBox->SetPosition(position);
}

void ListBox::SetSize(Vec2 size)
{
	MenuItem::SetSize(size);

	// Update box size
	contentBox->SetSize(size);

	// Update position of list-items (now that the box size has changed)
	SetPosition(position);
}

void ListBox::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);

	// Update box origin
	contentBox->SetOrigin(origin);
}

void ListBox::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);

	// Update box drawOffset
	contentBox->SetDrawOffset(drawOffset);
}

// Add an item to the list (returns id)
ListItem* ListBox::AddItem(std::string text)
{
	// Add a ListItem to the contentbox
	ListItem *item;
	item = (ListItem*)contentBox->AddItem(new ListItem(menuSystem, []() {}));

	// Size item to fit box
	item->SetSize(Vec2(size.x, _ITEMHEIGHT));

	// Set onClick function to select this item
	item->SetOnClick([=]()
	{
		this->SetSelectedItem(item);
	});

	// Parse the text, and extract each substring
	GLint pos1, pos2;
	pos1 = 0;
	pos2 = 0;

	while (pos1 < text.length())
	{
		// Find next tag-character
		pos2 = text.find_first_of("#", pos1);
		
		// If none was found...
		if (pos2 == std::string::npos)
		{
			// Set pos2 to the end of the string
			pos2 = text.length();
		}

		// Add substring to list-item
		item->AddItem(text.substr(pos1, pos2 - pos1));

		// Move pos1 to one character after next
		pos1 = pos2 + 1;
	}

	// Add list item to listItems
	listItems.push_back(item);

	// Return id of the new item
	return item;
}

// Remove an item from the list
void ListBox::RemoveItem(ListItem *item)
{
	// If we are removing the selected item, reset selection
	if (selectedItem == item && selectedItem != NULL)
	{
		selectedItem->SetIsSelected(false);
		selectedItem = NULL;
	}

	// Loop through and erase the item with the given id, and remove it from the contentbox (which will handle unloading)
	for (int i = 0; i < listItems.size(); i++)
	{
		if (listItems[i] == item)
		{
			contentBox->RemoveItem(item);
			listItems.erase(listItems.begin() + i);
			break;
		}
	}
}

// Set what item in the list is currently selected (set to NULL to reset selection)
void ListBox::SetSelectedItem(ListItem *selectedItem)
{
	// Make sure the selected item knows its being unselected
	if (this->selectedItem != NULL)
	{
		this->selectedItem->SetIsSelected(false);
	}

	// Set selected item to the new item
	this->selectedItem = selectedItem;

	// Make sure the new item knows it has been selected
	this->selectedItem->SetIsSelected(true);
}

// Get the currently selected list-item (returns NULL if none is selected)
ListItem* ListBox::GetSelectedItem()
{
	return selectedItem;
}