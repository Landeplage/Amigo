#pragma once

class Color
{
public:
	Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	unsigned char r, g, b;

	Color operator+(const Color operand);
	Color operator-(const Color operand);
};