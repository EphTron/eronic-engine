#include "MultiMesh.h"
#include "Vector4.h"
#include <iostream>
#include <fstream>
using namespace std;

/******************************************************************************************************************/

MultiMesh::MultiMesh()
	
{
}

/******************************************************************************************************************/

MultiMesh::~MultiMesh()
{
	//Reset();
}

/******************************************************************************************************************/

void MultiMesh::Clear()
{
	Reset();
}

/******************************************************************************************************************/

void MultiMesh::Reset()
{
	for (MeshIterator i = _meshes.begin(); i != _meshes.end(); ++i)
	{
		delete i->second;
	}
	_meshes.clear();
}

/******************************************************************************************************************/

void MultiMesh::CreateVBOs(Renderer* renderer)
{
	for (MeshIterator i = _meshes.begin(); i != _meshes.end(); ++i)
	{
		i->second->CreateVBO(renderer);
	}
}

/******************************************************************************************************************/

bool MultiMesh::LoadFromFile(std::string filename)
{
	ifstream in(filename);
	if (in)
	{
		int numObjects;
		in >> numObjects;
		
		for (size_t i = 0; i < numObjects; i++)
		{
			string name;
			in >> name;
			
			Mesh* m = new Mesh();
			m->LoadFromStream(in);

			_meshes[name] = m;
		}
		return true;
	}
	else
	{
		// Could not open file
		return false;
	}
}

/******************************************************************************************************************/

Mesh* MultiMesh::GetMesh(std::string key)	const
{
	auto var = _meshes.find(key);
	if (var != _meshes.end())
	{
		return var->second;
	}
	return nullptr;
}

/******************************************************************************************************************/
