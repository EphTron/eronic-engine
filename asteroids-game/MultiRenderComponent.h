#pragma once
#include "RenderComponent.h"
#include <map>

// Special render component to allow us to choose from multiple meshes
class MultiRenderComponent :
	public RenderComponent
{
	// Data
protected:
	std::map<std::string, Mesh*>					_meshes;		// Map of meshes, keyed by mesh name

	// Structors
public:
	MultiRenderComponent(GameObject* obj)			;
	virtual ~MultiRenderComponent()					;


	// Gets/sets
public:
	int NumMeshes()							const	{ return (int)_meshes.size(); }
	Mesh* GetMesh(std::string name)			const	;
	void AddMesh(std::string name, Mesh* mesh)		{ _meshes[name] = mesh; }

	// Functions
public:
	void SelectCurrentMesh(std::string meshName)	{ _mesh = GetMesh(meshName); }
};


inline Mesh* MultiRenderComponent::GetMesh(std::string name) const
{
	std::map<std::string, Mesh*>::const_iterator i = _meshes.find(name);
	if (i != _meshes.end())
	{
		return i->second;
	}
	return NULL;
}