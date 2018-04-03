#pragma once
#if BUILD_DIRECTX

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include "Renderer.h"

typedef struct UniformBuffer
{
	DirectX::XMFLOAT4X4 MVM;
	DirectX::XMFLOAT4 Colour;
} UniformBuffer;

// DirectX Device & Context
class Renderer_DX :
	public Renderer
{
	// Data
protected:
	IDXGISwapChain*			_swapchain;             // the pointer to the swap chain interface
	ID3D11Device*			_device;                // the pointer to our Direct3D device interface
	ID3D11DeviceContext*	_context;				// the pointer to our Direct3D device context
	ID3D11RenderTargetView* _backbuffer;			// the pointer to our back buffer
	ID3D11InputLayout*		_layout;				// the pointer to the input layout
	ID3D11VertexShader*		_vertexShader;			// the pointer to the vertex shader
	ID3D11PixelShader*		_pixelShader;			// the pointer to the pixel shader
	ID3D11Buffer*			_uniformBuffer;			// Stores the MVM and colour

	HWND					_hWnd;					// Window handle

	// Structors
public:
	Renderer_DX(HWND hWnd);
	virtual ~Renderer_DX();

	// Gets/Sets
public:
	ID3D11Device* GetDevice()					const	{ return _device; }
	void SetDevice(ID3D11Device* v)						{ _device = v; }

	ID3D11DeviceContext* GetContext()			const	{ return _context; }
	void SetContext(ID3D11DeviceContext* v)				{ _context = v; }

	// Functions
public:
	virtual void ClearScreen();
	virtual void Destroy();
	virtual void Draw(const Mesh* mesh, glm::mat4 MVM, const Colour& colour);
	virtual void Initialise(int width, int height);
	virtual void SwapBuffers();

	// Initialise the shaders
	void InitialiseShaders();

};

#endif