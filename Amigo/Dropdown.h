#pragma once
#include "MenuItem.h"
#include <functional>
#include "Vec2.h"
#include "Font.h"
#include "Button.h"
#include "RenderTarget.h"

class Dropdown : public MenuItem
{
public:
	Dropdown(MenuSystem* menuSystem, std::string text, Vec2 position, Vec2 size, GLint menuID, std::string tooltip, std::function<void()> onClick);

	virtual void HandleInput();
	virtual void Update(GLdouble time);
	virtual void Draw();
	virtual void Unload();

	void toggleDropdownStatus();

	void SetSize(Vec2 size);
	void SetPosition(Vec2 position);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

	GLint AddItem(std::string text);
	void RemoveItem(GLint id);
	void HandleItems();
	void DrawItems();
	void SetSelectedItem(GLint id);
	GLint GetSelectedItemID();
	std::string GetSelectedItemText();
	void SetMaxDropdownHeight(GLint height);

private:
	std::string text, tooltip;
	Font* font;
	std::function<void()> function;
	Button* button;

	GLint itemHover; // The dropdown-item the cursor is hovering over

	struct dropdownItem
	{
		dropdownItem(GLint id, std::string text)
		{
			this->id = id;
			this->text = text;
			this->displayText = text; // This is what is drawn. It will get truncated if it's wider than the dropdown.
		}

		GLint id;
		std::string text, displayText;
	};

	std::vector<dropdownItem*> dropdownItems; // the list of items in the dropdown
	dropdownItem* selectedItem; // the currently selected item
	GLint idHandler; // Handles the distribution of id-number to new dropdown-items

	dropdownItem* GetItem(GLint id); // Get pointer to an item based on an id
	void TruncateDisplayTextOnAllItems(); // Truncate display-text on all items
	void TruncateDisplayTextOnItem(dropdownItem* item); // Truncate display-text on one item
	void RefreshButtonText();

	// Rendertarget members
	RenderTarget *rt, *hrt;
	void UpdateRendertarget();
	void UpdateHighlightRendertarget();

	// Scroller members
	GLint scroll, scrollPrev, maxScroll, dropdownOffset;
	void UpdateScrollVariables();
	void UpdateDropdownOffset();
	GLint GetDropdownHeight();
	GLint maxDropdownHeight; // max height of dropdown-list before the scroller appears (in pixels)
};