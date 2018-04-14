#include "Mesh.h"
#if BUILD_DIRECTX
#include "VBO_DX.h"
#endif
#if BUILD_OPENGL
#include "VBO_GL.h"
#endif
#include "Vector4.h"
#include <iostream>
#include <fstream>
using namespace std;

/******************************************************************************************************************/

Mesh::Mesh()
	:	_locked(false),
		_vbo(NULL)
	
{
}

/******************************************************************************************************************/

Mesh::~Mesh()
{
	delete _vbo;
	_vbo = NULL;
}

/******************************************************************************************************************/

bool Mesh::AddVertex(Vertex v)
{
	if (!_locked)
	{
		_vertices.push_back(v);
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************************************************/

bool Mesh::DeleteVertex(int i)
{
	if (!_locked)
	{
		_vertices.erase(_vertices.begin() + i);
		return true;
	}
	else
	{
		return false;
	}
}

/******************************************************************************************************************/

bool Mesh::Clear()
{
	if (!_locked)
	{
		_vertices.clear();
		return true;
	}
	return false;
}

/******************************************************************************************************************/

void Mesh::Reset()
{
	delete _vbo;
	_vbo = NULL;
	_locked = false;
}

/******************************************************************************************************************/

VBO* Mesh::CreateVBO(Renderer* renderer)
{
	_locked = true;

#if BUILD_DIRECTX
	_vbo = new VBO_DX();
#else
	_vbo = new VBO_GL();
#endif

	_vbo->Create(renderer, _vertices.data(), NumVertices());

	return _vbo;
}

/******************************************************************************************************************/

float Mesh::CalculateMaxSize()
{
	float max = 0;
	for (int i = 0; i < NumVertices(); i++)
	{
		Vertex& v = _vertices[i];
		Vector4 vec(v.x, v.y, v.z, 1);
		float dist = vec.lengthSquared();
		if (dist > max)
		{
			max = dist;
		}
	}
	return sqrt(max);
}

/******************************************************************************************************************/

bool Mesh::LoadFromFile(std::string filename)
{
	ifstream in(filename);
	if (in)
	{
		int numVertices;
		in >> numVertices;
		for (int i = 0; i < numVertices; i++)
		{
			Vertex v;
			in >> v.x;
			in >> v.y;
			in >> v.z;
			in >> v.r;
			in >> v.g;
			in >> v.b;
			in >> v.a;
			AddVertex(v);
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

bool Mesh::LoadFromStream(std::ifstream& in)
{
	int numVertices;
	in >> numVertices;
	for (int i = 0; i < numVertices; i++)
	{
		Vertex v;
		in >> v.x;
		in >> v.y;
		in >> v.z;
		in >> v.r;
		in >> v.g;
		in >> v.b;
		in >> v.a;
		AddVertex(v);
	}
	return true;
}

/******************************************************************************************************************/
