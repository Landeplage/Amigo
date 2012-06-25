#include "MenuItem.h"

MenuItem::MenuItem()
{
	visible = true;
	active = true;
	position = Vec2(0, 0);
	size = Vec2(0, 0);
	menuOffset = Vec2(0, 0);
	menuID = 0;
}

void MenuItem::SetSize(GLint width, GLint height)
{
	size.x = width;
	size.y = height;
}

void MenuItem::SetPosition(GLint x, GLint y)
{
	this->position.x = x;
	this->position.y = y;
}

Vec2 MenuItem::GetSize()
{
	return size;
}

Vec2 MenuItem::GetPosition()
{
	return position;
}