#include "Box.h"
#include "MenuSystem.h"

Box::Box(MenuSystem* menuSystem, std::string title, GLint x, GLint y, GLint w, GLint h, GLint menuID)
{
	this->menuSystem = menuSystem;
	this->title = title;
	this->x = x;
	this->y = y;
	this->size.x = w;
	this->size.y = h;
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

void Box::Draw()
{
	int w, h;
	w = size.x;
	h = size.y;

	sprite->Draw(x - 10, y - 10, 0.0f, 1.0f, 1.0f, 1.0f, 39, 0, 10, 10); // Top left
	sprite->Draw(x, y - 10, 0.0f, w, 1.0f, 1.0f, 49, 0, 1, 10); // Top
	sprite->Draw(x + w, y - 10, 0.0f, 1.0f, 1.0f, 1.0f, 50, 0, 10, 10); // Top right
	sprite->Draw(x + w, y, 0.0f, 1.0f, h, 1.0f, 50, 10, 10, 1); // Right
	sprite->Draw(x + w, y + h, 0.0f, 1.0f, 1.0f, 1.0f, 50, 11, 10, 10); // Bottom right
	sprite->Draw(x, y + h, 0.0f, w, 1.0f, 1.0f, 49, 11, 1, 10); // Bottom
	sprite->Draw(x - 10, y + h, 0.0f, 1.0f, 1.0f, 1.0f, 39, 11, 10, 10); // Bottom left
	sprite->Draw(x - 10, y, 0.0f, 1.0f, h, 1.0f, 39, 10, 10, 1); // Left
	sprite->Draw(x, y, 0.0f, w, h, 0.5f, 49, 10, 1, 1); // Middle

	// Draw title over box
	if (title != "")
	{
		sprite->Draw(x, y - 40, 0.0f, 1.0f, 1.0f, 1.0f, 28, 0, 5, 36); // Left
		sprite->Draw(x + 5, y - 40, 0.0f, w - 10, 1.0f, 1.0f, 33, 0, 1, 36); // Middle
		sprite->Draw(x + w - 5, y - 40, 0.0f, 1.0f, 1.0f, 1.0f, 34, 0, 5, 36); // Right

		font->DrawShorten(x + 10, y - 32, title, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.25f, w - 20);
		font->DrawShorten(x + 10, y - 31, title, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, w - 20);
	}
}

void Box::SetTitle(std::string title)
{
	this->title = title;
}