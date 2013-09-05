#pragma once
#include "MenuItem.h"
#include "Font.h"

class Box : public MenuItem
{
public:
	Box();
	Box(MenuSystem* menuSystem, std::string title, GLint x, GLint y, GLint w, GLint h, GLint menuID);

	void HandleInput();
	void Update(GLdouble time);
	void Draw();
	void Unload();

	void SetTitle(std::string title);
	std::string GetTitle();

protected:
	Font* font;
	std::string title;
};