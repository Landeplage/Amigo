#include "MenuItem.h"

MenuItem::MenuItem()
{
	visible = true;
	active = true;
}

void MenuItem::SetSize(GLint width, GLint height)
{
	size.x = width;
	size.y = height;
}

void MenuItem::SetPosition(GLint x, GLint y)
{
	this->x = x;
	this->y = y;
}

Vec2 MenuItem::GetSize()
{
	return size;
}

Vec2 MenuItem::GetPosition()
{
	return Vec2(x, y);
}