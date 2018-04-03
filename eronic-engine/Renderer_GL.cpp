#if BUILD_OPENGL

#include "Renderer_GL.h"
#include "GameObject.h"
#include "Mesh.h"

/******************************************************************************************************************/

Renderer_GL::Renderer_GL()
{
}

/******************************************************************************************************************/

Renderer_GL::~Renderer_GL()
{
}

/******************************************************************************************************************/

void Renderer_GL::ClearScreen()
{
	glClearColor(_clearColour.r(), _clearColour.g(), _clearColour.b(), _clearColour.a());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/******************************************************************************************************************/

void Renderer_GL::Destroy()
{
	delete _shader; // Will delete shader objects automatically through ref counting
}

/******************************************************************************************************************/

void Renderer_GL::Draw(const Mesh* mesh, glm::mat4 MVM, const Colour& colour)
{
	_shader->setUniformMatrix("MVM", 4, 1, false, glm::value_ptr(MVM));
	_shader->setUniform("globalColour", colour.r(), colour.g(), colour.b(), colour.a());

	mesh->GetVBO()->Draw(this);
}

/******************************************************************************************************************/

void Renderer_GL::Initialise(int width, int height)
{
	// Setup projection
	_PM = glm::ortho(-1, +1, -1, +1, -1, +1);

	// Setup viewport and enable depth testing
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	// Set line width
	//glLineWidth(2);


	/////////////////////////////
	// Setup shaders

	// Create shader objects (and compile them)
	_vertexShader	= new ShaderObject_GL("shader.vert", GL_VERTEX_SHADER);
	_fragmentShader = new ShaderObject_GL("shader.frag", GL_FRAGMENT_SHADER);

	// Create shader program and attach shader objects
	_shader = new ShaderProgram_GL();
	_shader->attachShader(_vertexShader);
	_shader->attachShader(_fragmentShader);

	// Link and use the shader
	_shader->link();
	_shader->use();

	_shader->setUniformMatrix("PM", 4, 1, false, glm::value_ptr(_PM));
}

/******************************************************************************************************************/

void Renderer_GL::SwapBuffers()
{
}

/******************************************************************************************************************/

#endif