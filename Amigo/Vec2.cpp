#include "Vec2.h"

Vec2 Vec2::operator+(const Vec2 operand)
{
	return Vec2(x + operand.x, y + operand.y);
}

Vec2 Vec2::operator-(const Vec2 operand)
{
	return Vec2(x - operand.x, y - operand.y);
}