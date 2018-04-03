#include "Mesh.h"
#if BUILD_DIRECTX
#include "VBO_DX.h"
#endif
#if BUILD_OPENGL
#include "VBO_GL.h"
#endif

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
#ifdef BUILD_DIRECTX 
	_vbo = new VBO_DX();
#elif BUILD_OPENGL
	_vbo = new VBO_GL();
#endif

	_vbo->Create(renderer, _vertices.data(), NumVertices());

	return _vbo;
}

/******************************************************************************************************************/
