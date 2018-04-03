#pragma once
#if BUILD_OPENGL
#include "Renderer.h"

#include "ShaderProgram_GL.h"
#include "ShaderObject_GL.h"

// OpenGL Device & Context etc
class Renderer_GL :
	public Renderer, glex
{
	// Data
protected:
	// Shaders
	ShaderProgram_GL*			_shader;
	ShaderObject_GL*			_vertexShader;
	ShaderObject_GL*			_fragmentShader;

	glm::mat4					_IM;  // Identity matrix
	glm::mat4					_MVM; // ModelView matrix
	glm::mat4					_PM;  // Projection matrix

	// Structors
public:
	Renderer_GL();
	virtual ~Renderer_GL();

	// Functions
public:
	virtual void ClearScreen();
	virtual void Destroy();
	virtual void Draw(const Mesh* mesh, glm::mat4 MVM, const Colour& colour);
	virtual void Initialise(int width, int height);
	virtual void SwapBuffers();
};

#endif