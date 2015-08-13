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

	// Set default callback-values
	setPositionCallback = [](){};
	setSizeCallback = [](){};
	setOriginCallback = [](){};
	setDrawOffsetCallback = [](){};
}

void MenuItem::SetPosition(Vec2 position)
{
	this->position = position;
	setPositionCallback();
}

void MenuItem::SetSize(Vec2 size)
{
	this->size = size;
	setSizeCallback();
}

void MenuItem::SetOrigin(Vec2 origin)
{
	this->origin = origin;
	setOriginCallback();
}

void MenuItem::SetDrawOffset(Vec2 drawOffset)
{
	this->drawOffset = drawOffset;
	setDrawOffsetCallback();
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

// Set callback-function for when item changes position
void MenuItem::SetPositionCallback(std::function<void()> callback)
{
	setPositionCallback = callback;
}

// Set callback-function for when item changes size
void MenuItem::SetSizeCallback(std::function<void()> callback)
{
	setSizeCallback = callback;
}

// Set callback-function for when item changes origin
void MenuItem::SetOriginCallback(std::function<void()> callback)
{
	setOriginCallback = callback;
}

// Set callback-function for when item changes drawOffset
void MenuItem::SetDrawOffsetCallback(std::function<void()> callback)
{
	setDrawOffsetCallback = callback;
}