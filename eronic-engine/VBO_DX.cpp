#if BUILD_DIRECTX
#include "VBO_DX.h"
#include "Renderer_DX.h"

/******************************************************************************************************************/

VBO_DX::VBO_DX()
	: _vbo(NULL)
{
}

/******************************************************************************************************************/

VBO_DX::~VBO_DX()
{
	if (_vbo)
		_vbo->Release();
}

/******************************************************************************************************************/

void VBO_DX::Create(Renderer* renderer, Vertex vertices[], int numVertices)
{
	Renderer_DX* rendererDX = (Renderer_DX*)renderer;

	_numVertices = numVertices;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * numVertices;   // size is the VERTEX struct * num vertices
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	rendererDX->GetDevice()->CreateBuffer(&bd, NULL, &_vbo);        // create the buffer

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	rendererDX->GetContext()->Map(_vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);		// map the buffer
	memcpy(ms.pData, vertices, sizeof(Vertex) * numVertices);			// copy the data
	rendererDX->GetContext()->Unmap(_vbo, NULL);											// unmap the buffer
}

/******************************************************************************************************************/

void VBO_DX::Draw(Renderer* renderer)
{
	Renderer_DX* rendererDX = (Renderer_DX*)renderer;

	// select which vertex buffer to display
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	rendererDX->GetContext()->IASetVertexBuffers(0, 1, &_vbo, &stride, &offset);

	// select which primtive type we are using
	rendererDX->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// draw the vertex buffer to the back buffer
	rendererDX->GetContext()->Draw(_numVertices, 0);

}

/******************************************************************************************************************/

#endif