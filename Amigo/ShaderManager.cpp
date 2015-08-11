#include "ShaderManager.h"
#include <stdio.h>

ShaderManager *ShaderManager::instance = 0;

ShaderManager::ShaderManager()
{
	printf("Initializing shader...");

	// Create shader object
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Pass shader source to shader object
	const GLchar * src;
	src = GetFragmentShaderSource();
	glShaderSource(fragmentShader, 1, &src, 0);

	// Compile shader object
	glCompileShader(fragmentShader);

	// Create program object
	shaderProgram = glCreateProgram();

	// Attach shader to program
	glAttachShader(shaderProgram, fragmentShader);

	// Link program object
	glLinkProgram(shaderProgram);

	printf("Done.\n");

	// Check for errors
	GLint compileError = 0;

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileError);

	if (compileError == 0)
	{
		printf("WARNING! Shader did not compile.\n");
		GLchar infoLog[2048];
		glGetShaderInfoLog(fragmentShader, 2048, NULL, infoLog);
		printf(infoLog);
	}
}

ShaderManager::~ShaderManager()
{
	// destroy
}

void ShaderManager::Init()
{
	// initialize
}

void ShaderManager::Apply()
{
	// Use program object instead of fixed function OpenGL
	glUseProgram(shaderProgram);
}

void ShaderManager::Reset()
{
	// Use fixed function OpenGL
	glUseProgram(0);
}

GLchar* ShaderManager::GetFragmentShaderSource()
{
	return
		"in vec2 texCoord; uniform sampler2D tex; void main() { vec4 texCol = texture(tex, Vec2(glTexCoord[0])); gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";
}

// Get singleton instance
ShaderManager* ShaderManager::GetInstance()
{
	if (!instance)
	{
		instance = new ShaderManager();
		instance->Init();
	}
	return instance;
}
