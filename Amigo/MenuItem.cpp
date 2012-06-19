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

Point MenuItem::GetSize()
{
	return size;
}

Point MenuItem::GetPosition()
{
	return Point(x, y);
}