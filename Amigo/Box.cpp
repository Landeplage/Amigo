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

void Box::Update(GLdouble time)
{
	// update
}

void Box::Draw(GLfloat transition)
{
	GLfloat alpha;
	alpha = 1 - abs(transition);

	GLint x, y, w, h;
	x = (GLint)(position.x);
	y = (GLint)position.y;
	w = (GLint)size.x;
	h = (GLint)size.y;
	
	// Compensate for edges
	w -= 8;
	h -= 6;

	// Draw box
	sprite->Draw(x, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 31, 6, 7); // Left top corner
	sprite->Draw(x, y + 7, 0.0f, 1.0f, (GLfloat)(h - 7), 1.0f, 1.0f, 1.0f, alpha, 0, 38, 1, 1); // Left side
	sprite->Draw(x, y + h, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 39, 7, 6); // Left bottom corner
	sprite->Draw(x + 7, y + h + 5, 0.0f, (GLfloat)(w - 6), 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 38, 1, 1); // Bottom
	sprite->Draw(x + w + 8, y + h, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 39, 7, 6); // Right bottom corner
	sprite->Draw(x + w + 7, y + 7, 0.0f, 1.0f, (GLfloat)(h - 7), 1.0f, 1.0f, 1.0f, alpha, 0, 38, 1, 1); // Right side
	sprite->Draw(x + w + 8, y, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 31, 6, 7); // Right top corner
	sprite->Draw(x + 6, y, 0.0f, (GLfloat)(w - 3), 1.0f, 1.0f, 1.0f, 1.0f, alpha, 6, 31, 1, 2); // Top

	// Draw title over box
	if (title != "")
	{
		font->DrawShorten(x + 5, y - 17, title, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha * 0.5f, w - 10);
		font->DrawShorten(x + 5, y - 18, title, 0.0f, 1.0f, 1.0f, (GLfloat)(116.0f / 255.0f), (GLfloat)(85.0f / 255.0f), (GLfloat)(38.0f / 255.0f), alpha, w - 10);
	}
}

void Box::SetTitle(std::string title)
{
	this->title = title;
}