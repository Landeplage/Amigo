#pragma once
#include "MenuItem.h"
#include "Font.h"

class MenuSystem;

class Box : public MenuItem
{
public:
	Box(MenuSystem* menuSystem, std::string title, GLint x, GLint y, GLint w, GLint h, GLint menuID);

	void Update(GLdouble time);
	void Draw(GLfloat transition);
	void Unload();

	void SetTitle(std::string title);
	std::string GetTitle();

private:
	Font* font;
	std::string title;
	MenuSystem* menuSystem;
};