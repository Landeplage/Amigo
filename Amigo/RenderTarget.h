#pragma once
#include <GL\glew.h>
#include "Vec2.h"
#include "Color.h"

class RenderTarget
{
public:
	RenderTarget();
	RenderTarget(GLint width, GLint height);
	~RenderTarget();

	void Begin();
	void End();
	void Draw(GLint x, GLint y);
	void Draw(GLint x, GLint y, GLfloat alpha);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h);
	void Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, Color color, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h);

	void SetSize(Vec2 size);
	Vec2 GetSize();

	void SetFinalBlend(bool finalBlend);

private:
	// Rendertarget stuff
	union Viewport { struct { GLint x, y; GLint width, height; }; GLint vp[4]; };
	static Viewport viewport;
	GLuint frameBufferRT;
	GLuint* texRT;
	Vec2 size, windowSize;
	bool finalBlend;
};