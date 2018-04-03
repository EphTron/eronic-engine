#if BUILD_OPENGL
#include "VBO_GL.h"

/******************************************************************************************************************/

VBO_GL::VBO_GL()
	: _vboID(0), _vaoID(0), _numVertices(0)
{
}

/******************************************************************************************************************/

VBO_GL::~VBO_GL()
{
	glDeleteBuffers(1, &_vboID);
	glDeleteVertexArrays(1, &_vaoID);
}

/******************************************************************************************************************/

void VBO_GL::Create(Renderer* renderer, Vertex vertices[], int numVertices)
{
	_numVertices = numVertices;

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	glGenBuffers(1, &_vboID);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glEnableVertexAttribArray(0); // Vertex positions
	glEnableVertexAttribArray(1); // Colours

	// Set vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);

	// Set colour data
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (byte*)12);

	// Allocate space for the buffers and initialise the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertices, GL_STATIC_DRAW);

	// OK, finish with VAO setup
	glBindVertexArray(0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/******************************************************************************************************************/

void VBO_GL::Draw(Renderer* renderer)
{
	DrawVAO();
}

/******************************************************************************************************************/

void VBO_GL::DrawVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	// Enable that vertex attrib array
	int vertexLocation = 0;
	int colourLocation = 1;
	glEnableVertexAttribArray(vertexLocation);
	glEnableVertexAttribArray(colourLocation);

	// Set vertex data
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, false, sizeof(Vertex), 0);

	// Set colour data
	glVertexAttribPointer(colourLocation, 4, GL_FLOAT, false, sizeof(Vertex), (byte*)16);

	// Draw
	glDrawArrays(GL_LINE_STRIP, 0, _numVertices);

	// Disable vertex attrib array
	glDisableVertexAttribArray(vertexLocation);
	glDisableVertexAttribArray(colourLocation);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/******************************************************************************************************************/

void VBO_GL::DrawVAO()
{
	glBindVertexArray(_vaoID);

	// Draw
	glDrawArrays(GL_LINE_STRIP, 0, _numVertices);

	glBindVertexArray(0);
}

/******************************************************************************************************************/

#endif