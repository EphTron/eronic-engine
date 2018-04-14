#pragma once
#include <map>
#include <string>
#include <vector>
#include "Mesh.h"

typedef std::map<std::string, Mesh* >::iterator		MeshIterator;

class MultiMesh
{
	// Data
private:
	std::map<std::string, Mesh* > _meshes;

	// Constructors etc
public:
	MultiMesh();
	~MultiMesh();

	// Gets/sets
public:
	Mesh* GetMesh(std::string key)		const	;
	int NumMeshes()						const	{ return (int)_meshes.size(); }

	// Functions
public:
	void CreateVBOs(Renderer* r)				;
	void Clear()								;
	
	// Loads the Mesh from a file (returns true if loaded OK)
	bool LoadFromFile(std::string filename)		;

	// Unlocks the mesh but also deletes the VBO
	void Reset()								;
};

