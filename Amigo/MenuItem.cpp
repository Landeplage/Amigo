#include "MenuItem.h"

MenuItem::MenuItem()
{
	visible = true;
	active = true;
	position = Vec2(0, 0);
	size = Vec2(0, 0);
	origin = Vec2(0, 0);
	drawOffset = Vec2(0, 0);
	menuID = 0;
}

void MenuItem::SetPosition(Vec2 position)
{
	this->position = position;
}

void MenuItem::SetSize(Vec2 size)
{
	this->size = size;
}

void MenuItem::SetOrigin(Vec2 origin)
{
	this->origin = origin;
}

void MenuItem::SetDrawOffset(Vec2 drawOffset)
{
	this->drawOffset = drawOffset;
}

Vec2 MenuItem::GetPosition()
{
	return position;
}

Vec2 MenuItem::GetSize()
{
	return size;
}

Vec2 MenuItem::GetOrigin()
{
	return origin;
}

Vec2 MenuItem::GetDrawOffset()
{
	return drawOffset;
}

std::vector<MenuItem*> MenuItem::GetChildren()
{
	return children;
}