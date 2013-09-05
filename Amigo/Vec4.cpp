#include "Vec4.h"

Vec4 Vec4::operator+(const Vec4 operand)
{
	return Vec4(x + operand.x, y + operand.y, z + operand.z, æ + operand.æ);
}

Vec4 Vec4::operator-(const Vec4 operand)
{
	return Vec4(x - operand.x, y - operand.y, z - operand.z, æ - operand.æ);
}