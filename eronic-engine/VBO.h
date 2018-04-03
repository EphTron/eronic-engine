#pragma once
#include "Vertex.h"

class Renderer;

// Abstract VBO class
class VBO
{
	// Data
protected:
	int _numVertices;

	// Constructors
public:
	VBO();
	virtual ~VBO();

	// Functions
public:
	virtual void Create(Renderer* renderer, Vertex vertices[], int numVertices) = 0;
	virtual void Draw(Renderer* renderer) = 0;
};

