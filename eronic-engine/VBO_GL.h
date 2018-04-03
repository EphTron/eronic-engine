#pragma once
#if BUILD_OPENGL

#include "GL\GXBase\GXBase.h"
#include "VBO.h"

class VBO_GL
	: public VBO, glex
{
	// Data
private:
	GLuint _vaoID;
	GLuint _vboID;

	int _numVertices;

	// Constructors
public:
	VBO_GL();
	virtual ~VBO_GL();

	// Functions
public:
	virtual void Create(Renderer* renderer, Vertex vertices[], int numVertices);
	virtual void Draw(Renderer* renderer);
private:
	void DrawVBO();
	void DrawVAO();
};

#endif