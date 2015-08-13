#include "Box.h"
#include "MenuSystem.h"

Box::Box(MenuSystem* menuSystem, std::string title, GLint x, GLint y, GLint w, GLint h, GLint menuID)
{
	this->menuSystem = menuSystem;
	this->title = title;
	this->position = Vec2((GLfloat)x, (GLfloat)y);
	this->size = Vec2((GLfloat)w, (GLfloat)h);
	this->menuID = menuID;

	this->font = menuSystem->GetFontBold();
	this->sprite = menuSystem->GetSpriteUI();
}

void Box::Unload()
{
	// unload any resources
}

void Box::HandleInput()
{
	// handle input
}

void Box::Update(GLdouble time)
{
	// update
}

void Box::Draw()
{
	if (!visible)
		return;

	GLfloat alpha;
	alpha = 1.0f;

	GLint x, y, w, h;
	x = (GLint)position.x + drawOffset.x;
	y = (GLint)position.y + drawOffset.y;
	w = (GLint)size.x;
	h = (GLint)size.y;

	// Draw box
	sprite->DrawRectangleFromTexture(Vec2(x, y), Vec2(w, h), Vec2(0, 45), Vec2(6, 7), Vec2(1, 1));

	// Draw title over box
	if (title != "")
	{
		font->DrawShorten(Vec2(x + 5, y - 17), title, 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), alpha * 0.5f, w - 10);
		font->DrawShorten(Vec2(x + 5, y - 18), title, 0.0f, Vec2(1.0f, 1.0f), Color(116, 85, 38), alpha, w - 10);
	}
}

void Box::SetTitle(std::string title)
{
	this->title = title;
}

std::string Box::GetTitle()
{
	return title;
}