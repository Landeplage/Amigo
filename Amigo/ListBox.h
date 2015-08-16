#pragma once

#include "MenuItem.h"
#include "Vec2.h"
#include "MenuSystem.h"
#include <functional>
#include "ContentBox.h"
#include "RenderTarget.h"
#include "Text.h"
#include "Button.h"
#include "Font.h"
#include "ListItem.h"

class ListBox : public MenuItem
{
public:
	ListBox(MenuSystem *menuSystem, Vec2 position, Vec2 size, std::string title, GLint menuID, std::function<void()> onClick);

	virtual void Unload();
	virtual void HandleInput();
	virtual void Update(GLdouble time);
	virtual void Draw();

	void SetSize(Vec2 size);
	void SetPosition(Vec2 position);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

	ListItem* AddItem(std::string text);
	void RemoveItem(ListItem *item);
	ListItem* GetSelectedItem();
	void SetSelectedItem(ListItem *selectedItem);

private:
	std::function<void()> onClick;
	ContentBox *contentBox;

	// The list of items
	std::vector<ListItem*> listItems;

	// Selected item
	ListItem *selectedItem;
};