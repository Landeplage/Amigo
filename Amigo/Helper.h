#pragma once

/*#include <GLTools.h>*/
#include <sstream>
#include <vector>
#include "Vec2.h"

#define _USE_MATH_DEFINES
#include <math.h> // Math-helper

// Calculate the vector of a given length and direction
static GLdouble ldirX(GLfloat length, GLfloat direction)
{
	return (cos(direction * (M_PI / 180.0f)) * length);
}

// Calculate the vector of a given length and direction
static GLdouble ldirY(GLfloat length, GLfloat direction)
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

// Convert an unsigned integer into a string
static std::string toString(unsigned int value)
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
static GLint getNextPower2(int a)
{
    int rval=1;
    // rval<<=1 Is A Prettier Way Of Writing rval*=2;
    while(rval<a) rval<<=1;
    return rval;
}

// Check if a point is inside a rectangle
static bool IsInside(Vec2 Point, int x1, int y1, int x2, int y2)
{
	if (Point.x >= x1 && Point.x < x2 && Point.y >= y1 && Point.y < y2)
		return true;
	return false;
}

// Check if a point is inside a rectangle
static bool IsInside(Vec2 Point, Vec2 Position, Vec2 Size)
{
	if (Point.x >= Position.x && Point.x < (Position.x + Size.x) && Point.y >= Position.y && Point.y < (Position.y + Size.y))
		return true;
	return false;
}

// Get the angle between two vectors (in degrees)
static float Vec2Dir(Vec2 A, Vec2 B)
{
	return (float)(atan2(B.y - A.y, B.x - A.x) * (180 / M_PI));
}

// Get distance between two vectors
static float Vec2Len(Vec2 A, Vec2 B)
{
	return sqrt((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
}

// Clamp a float
static float Clamp(float x, float min, float max)
{
	return x < min ? min : (x > max ? max : x);
}

// Clamp an int
static int Clamp(int x, int min, int max)
{
	return x < min ? min : (x > max ? max : x);
}

// Linear interpolation between two vectors
static Vec2 Lerp(Vec2 A, Vec2 B, float t)
{
	return Vec2(A.x * (1 - t) + B.x * t, A.y * (1 - t) + B.y * t);
}

// Hermite interpolation between two vectors, given two tangents
static Vec2 Herp(Vec2 A, Vec2 B, Vec2 T1, Vec2 T2, float t)
{
	float s = 1.0f - t, x1, x2, x3, x4;
	x1 = (s * s) * (1 + (2 * t));
	x2 = (t * t) * (1 + (2 * s));
	x3 = (s * s) * t;
	x4 = s * (t * t);
	return Vec2(
		(x1 * A.x) + (x2 * B.x) + (x3 * T1.x) - (x4 * T2.x),
		(x1 * A.y) + (x2 * B.y) + (x3 * T1.y) - (x4 * T2.y));
}

// Get tangent for hermite curve based on two points
static Vec2 GetHerpTan(Vec2 A, Vec2 B)
{
	float rot = Vec2Dir(A, B);
	return Vec2((float)ldirX(Vec2Len(A, B), rot) / 2, (float)ldirY(Vec2Len(A, B), rot) / 2);
}

// Get position-vector from a catmull-rom spline, given a list of vectors and a (global) t
static Vec2 GetCatmullrom(std::vector<Vec2> *spline, std::vector<Vec2> *velocities, float tGlobal)
{
	Vec2 T1, T2; // tangents
	float t; // local t
	t = tGlobal;
	while (t >= 1.0f)
		t -= 1.0f;
	if ((unsigned int)tGlobal < spline->size() - 1)
	{
		if ((unsigned int)tGlobal > 0)
			T1 = GetHerpTan(spline->at((unsigned int)tGlobal - 1), spline->at((unsigned int)tGlobal + 1));
		else
			T1 = GetHerpTan(spline->at(spline->size() - 1), spline->at(1));

		if ((unsigned int)tGlobal < spline->size() - 2)
			T2 = GetHerpTan(spline->at((unsigned int)tGlobal), spline->at((unsigned int)tGlobal + 2));
		else
			T2 = GetHerpTan(spline->at((unsigned int)tGlobal), spline->at(0));

		return Herp(spline->at((unsigned int)tGlobal), spline->at((unsigned int)tGlobal + 1), velocities->at((unsigned int)tGlobal), velocities->at((unsigned int)tGlobal + 1), t);
	}
	else
	{
		T1 = GetHerpTan(spline->at(spline->size() - 2), spline->at(0));
		T2 = GetHerpTan(spline->at(spline->size() - 1), spline->at(1));

		return Herp(spline->at(spline->size() - 1), spline->at(0), velocities->at(spline->size() - 1), velocities->at(0), t);
	}
}

// Quadratic ease out - where t is a number between 0 and 1.
static float EaseQuadOut(float t)
{
	return -1 * t * (t - 2);
}

// Quadratic ease in/out - where t is a number between 0 and 1.
static float EaseQuadInOut(float t)
{
	t = t / 0.5f;
	if (t < 1) return 0.5f * t * t;
	t--;
	return -0.5f * (t * (t - 2) - 1);
}

// Quintic ease (?)
static float EaseQuintic(float t)
{
	return pow((t - 1), 5) + 1;
}

// Quadratic ease in (not really quadratic tho)
static float EaseQuadIn(float t)
{
	//t /= d;
	return t*t*t;
}