#pragma once

#include "MenuItem.h"
#include "MenuSystem.h"
#include "Vec2.h"
#include <functional>
#include "Box.h"
#include "Button.h"
#include "Text.h"

class RadioBox : public MenuItem
{
public:
	RadioBox(MenuSystem *menuSystem, Vec2 position, GLint width, std::string title, GLint menuID, std::function<void()> onClick);

	virtual void Unload();
	virtual void HandleInput();
	virtual void Update(GLdouble time);
	virtual void Draw();

	void SetPosition(Vec2 position);
	void SetSize(Vec2 size);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

	GLint AddItem(std::string text);
	void RemoveItem(GLint id);

	void SetSelected(GLint id);
	GLint GetSelected();

	void SetOnClick(std::function<void()> onClick);

private:
	Box *box;
	std::function<void()> onClick;

	// Radio-button members
	struct RadioButton
	{
		RadioButton(Button *button, Text *textItem, GLint id)
		{
			this->button = button;
			this->textItem = textItem;
			this->id = id;
		}

		Button *button;
		Text *textItem;
		GLint id;
	};

	std::vector<RadioButton*> radioButtons;
	GLint idHandler, selected;
};