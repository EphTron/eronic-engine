#pragma once
#include <vector>
#include "VBO.h"
#include "Vertex.h"

class Renderer;

class Mesh
{
	// Data
private:
	bool _locked;	// True once we've made the VBO; can no longer add vertices etc unless reset
	VBO* _vbo;
	std::vector<Vertex> _vertices;

	// Constructors etc
public:
	Mesh();
	~Mesh();

	// Gets/sets
public:
	VBO* GetVBO()			const	{ return _vbo; }
	int NumVertices()		const	{ return (int)_vertices.size(); }
	Vertex GetVertex(int i)	const	{ return _vertices[i]; }
	Vertex& GetVertexRef(int i)		{ return _vertices[i]; }

	// Functions
public:
	VBO* CreateVBO(Renderer* r)					;
	bool AddVertex(Vertex v)					;
	bool Clear()								;
	bool DeleteVertex(int i)					;
	
	// Loads the Mesh from a file (returns true if loaded OK)
	bool LoadFromFile(std::string filename)		;

	// Loads the Mesh from an existing stream (returns true if OK)
	bool Mesh::LoadFromStream(std::ifstream& in);

	// Unlocks the mesh but also deletes the VBO
	void Reset()								;

	// Calculates max size of mesh in any direction
	float CalculateMaxSize()					;
};

