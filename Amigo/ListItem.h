#pragma once

#include "MenuItem.h"
#include "MenuSystem.h"
#include "Font.h"
#include "Button.h"
#include <functional>

class ListItem : public MenuItem
{
public:
	ListItem(MenuSystem *menuSystem, std::function<void()> onClick);

	virtual void Unload();
	virtual void HandleInput();
	virtual void Update(GLdouble time);
	virtual void Draw();

	void SetSize(Vec2 size);
	void SetPosition(Vec2 position);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

	void AddItem(std::string);
	void SetOnClick(std::function<void()> onClick);
	void SetIsSelected(bool isSelected);

private:
	Font *font;
	std::vector<std::string> strings;
	Button *button;
	bool isSelected;
};