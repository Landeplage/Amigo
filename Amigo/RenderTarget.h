#pragma once
#include <GL\glew.h>
#include "Point.h"

class RenderTarget
{
public:
	RenderTarget();
	RenderTarget(GLint width, GLint height, GLint texCount);
	~RenderTarget();

	void Begin();
	void End();
	void Draw(GLint x, GLint y);
	void Draw(GLint x, GLint y, GLfloat alpha);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h);

	Point GetSize();
private:
	// Rendertarget stuff
	union Viewport { struct { GLint x, y; GLint width, height; }; GLint vp[4]; };
	static Viewport viewport;
	GLuint frameBufferRT;
	GLuint* texRT;
	GLint texCount;
	Point size, windowSize;

	GLfloat rot;
};