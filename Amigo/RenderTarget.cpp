#include "RenderTarget.h"
#include "Context.h"
#include <math.h>
#include <stdio.h>

#include "Helper.h" // remove this

RenderTarget::Viewport RenderTarget::viewport;

RenderTarget::RenderTarget(GLint width, GLint height)
{
	printf("+ Adding rendertarget...");
	// Init variables
	size.x = width;
	size.y = height;
	windowSize.x = Context::getWindowWidth();
	windowSize.y = Context::getWindowHeight();

	// Create and initialize framebuffer
	glGenFramebuffers(1, &frameBufferRT);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferRT);
	
	// Setup texture
	texRT = new GLuint[1];
	glGenTextures(1, texRT);
	glBindTexture(GL_TEXTURE_2D, texRT[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texRT[0], 0);

	// Bind to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	printf("Done\n");
}

RenderTarget::~RenderTarget()
{
	printf("- Deleting rendertarget...");
	delete[] texRT;
	printf("Done.\n");
}

void RenderTarget::Begin()
{
	// Setup framebuffer
	glGetIntegerv(GL_VIEWPORT, viewport.vp);
	glViewport(0, size.y - windowSize.y, windowSize.x, windowSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferRT);

	// Clear the buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Set blend mode
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

void RenderTarget::End()
{
	// Reset blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Reset viewport
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

void RenderTarget::Draw(GLint x, GLint y)
{
	Draw(x, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0, size.x, size.y);
}

void RenderTarget::Draw(GLint x, GLint y, GLfloat alpha)
{
	Draw(x, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 0, 0, size.x, size.y);
}

void RenderTarget::Draw(GLint x, GLint y, GLfloat rotation, GLfloat scaleX, GLfloat scaleY, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, GLint xx, GLint yy, GLint w, GLint h)
{
	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texRT[0]);
	
	// Push the matrix
	glPushMatrix();

	// Rotate and translate the quad
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);

	// Calculate coordinates
	GLdouble coX1, coX2, coY1, coY2;
	coX1 = (GLdouble)xx / size.x;
	coY1 = (GLdouble)yy / size.y;
	coX2 = (GLdouble)(xx + w) / size.x;
	coY2 = (GLdouble)(yy + h) / size.y;

	// Draw the textured quad
	glBegin(GL_QUADS);
	glColor4f(red, green, blue, alpha);
	
	glTexCoord2d(coX1, coY2); glVertex2d(0,				0);
	glTexCoord2d(coX2, coY2); glVertex2d(w * scaleX,	0);
	glTexCoord2d(coX2, coY1); glVertex2d(w * scaleX,	h * scaleY);
	glTexCoord2d(coX1, coY1); glVertex2d(0,				h * scaleY);
	glEnd();

	// Pop the matrix
	glPopMatrix();
}

// Get size of rendertarget
Vec2 RenderTarget::GetSize()
{
	return size;
}