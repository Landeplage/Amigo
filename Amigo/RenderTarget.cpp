#include "RenderTarget.h"
#include "Context.h"
#include <math.h>
#include <stdio.h>

#include "Helper.h" // remove this

RenderTarget::Viewport RenderTarget::viewport;

RenderTarget::RenderTarget(GLint width, GLint height, GLint texCount)
{
	size.x = width;
	size.y = height;
	this->texCount = texCount;
	windowSize.x = Context::getWindowWidth();
	windowSize.y = Context::getWindowHeight();

	// Create and initialize framebuffer
	texRT = new GLuint[texCount];

	glGenFramebuffers(1, &frameBufferRT);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferRT);
	
	glGenTextures(texCount, texRT);
	for(int i = 0; i < texCount; i ++)
	{
		glBindTexture(GL_TEXTURE_2D, texRT[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texRT[i], 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	printf("+ RenderTarget added\n");
}

RenderTarget::~RenderTarget()
{
	printf("- RenderTarget deleted\n");
	delete[] texRT;
}

void RenderTarget::Begin()
{
	// Setup framebuffer
	glGetIntegerv(GL_VIEWPORT, viewport.vp);
	glViewport(0, size.y - windowSize.y, windowSize.x, windowSize.y);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferRT);

	GLint max_texture_image_units, active_unit;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
    glGetIntegerv(GL_ACTIVE_TEXTURE, &active_unit); // yes I know the naming is odd
	
	if (texCount > 1)
    {
            for (GLuint i = 0; i < texCount; i++)
            {
                    // make sure RT textures are unbound before attaching
                    for(GLuint u = 0; u < max_texture_image_units; u++)
					{
                            GLint bound_texture;
                            glActiveTexture(GL_TEXTURE0 + u);
                            glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture);
                            if(bound_texture = texRT[i])
							{
                                    // better yet, store (unit,texture) pairs, so that we can restore in RenderTarget::End.
                                    glBindTexture(GL_TEXTURE_2D, 0);
                            }
                    }
            }

            glActiveTexture(active_unit); //okay, this is a kludge. Ideally the code rendering to the FBO takes care of this. But if now, we fail safely.
 
            GLenum* buffers = new GLenum[texCount];
            glBindFramebuffer(GL_FRAMEBUFFER, frameBufferRT);
            for (GLuint i = 0; i < texCount; i++)
            {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texRT[i], 0);
                    buffers[i] = GL_COLOR_ATTACHMENT0 + i;
            }
            glDrawBuffers(texCount, buffers);
            delete[] buffers;
    }

	// Clear the buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderTarget::End()
{
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

	// Testing blend functions
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw the textured quad
	glBegin(GL_QUADS);
	glColor4f(red, green, blue, alpha);
	
	glTexCoord2d(coX1, coY2); glVertex2d(0,				0);
	glTexCoord2d(coX2, coY2); glVertex2d(w * scaleX,	0);
	glTexCoord2d(coX2, coY1); glVertex2d(w * scaleX,	h * scaleY);
	glTexCoord2d(coX1, coY1); glVertex2d(0,				h * scaleY);
	glEnd();

	// Return to default blend-function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Pop the matrix
	glPopMatrix();
}

// Get size of rendertarget
Vec2 RenderTarget::GetSize()
{
	return size;
}