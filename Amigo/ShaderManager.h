#include <GL\glew.h>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	static ShaderManager *instance;
	static ShaderManager* GetInstance();

	void Init();
	void Apply();
	void Reset();
private:
	GLenum shaderProgram, fragmentShader;

	GLchar* GetFragmentShaderSource();
};