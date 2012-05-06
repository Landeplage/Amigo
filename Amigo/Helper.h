#pragma once

#include <GLTools.h>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h> // Math-helper

// Calculate the vector of a given length and direction
static GLfloat lenDirX(GLfloat length, GLfloat direction)
{
	return (cos(direction * (M_PI / 180.0f)) * length);
}

// Calculate the vector of a given length and direction
static GLfloat lenDirY(GLfloat length, GLfloat direction)
{
	return (sin(direction * (M_PI / 180.0f)) * length);
}

// Convert an integer into a string
static std::string toString(int value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

// Convert a float into a string
static std::string toString(float value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

// Convert a double into a string
static std::string toString(double value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

// Convert a char into a string
static std::string toStringChar(char value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

// Convert an unsigned char into a string
static std::string toStringUChar(unsigned char value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

// Convert a const char* into a string
static std::string toString(const char* value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

// This Function Gets The First Power Of 2
static int getNextPower2(int a)
{
    int rval=1;
    // rval<<=1 Is A Prettier Way Of Writing rval*=2;
    while(rval<a) rval<<=1;
    return rval;
}

// Check if a point is inside a rectangle
static bool isInside(int x, int y, int x1, int y1, int x2, int y2)
{
	if (x >= x1 && x < x2 && y >= y1 && y < y2)
		return true;
	return false;
}