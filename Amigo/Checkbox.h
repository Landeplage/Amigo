#pragma once

#include "MenuItem.h"
#include "Button.h"
#include "Vec2.h"
#include <functional>
#include "MenuSystem.h"
#include "Font.h"
#include "Text.h"

class Checkbox : public MenuItem
{
public:
	Checkbox(MenuSystem *menuSystem, Vec2 position, std::string text, GLint menuID, std::function<void()> onClick);

	virtual void Unload();
	virtual void HandleInput();
	virtual void Update(GLdouble time);
	virtual void Draw();

	void SetSize(Vec2 size);
	void SetPosition(Vec2 position);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

	void SetToggled(bool toggled);
	bool GetToggled();

	void SetText(std::string text);

private:
	Font *font;
	std::string text;
	bool toggled;
	Button *button;
	std::function<void()> onClick;
	Text *textItem;

	void UpdateSize();
};