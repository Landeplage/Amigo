#include "Color.h"
#include "Helper.h"

Color Color::operator+(const Color operand)
{
	return Color(Clamp(r + operand.r, 0, 255), Clamp(g + operand.g, 0, 255), Clamp(b + operand.b, 0, 255));
}

Color Color::operator-(const Color operand)
{
	return Color(Clamp(r - operand.r, 0, 255), Clamp(g - operand.g, 0, 255), Clamp(b - operand.b, 0, 255));
}