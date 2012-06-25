#pragma once
#include "MenuItem.h"
#include "Font.h"

class MenuSystem;

class Box : public MenuItem
{
public:
	Box(MenuSystem* menuSystem, std::string title, GLint x, GLint y, GLint w, GLint h, GLint menuID);

	void Update(GLdouble time);
	void Draw();
	void Unload();

	void SetTitle(std::string title);

private:
	Font* font;
	std::string title;
	MenuSystem* menuSystem;
};