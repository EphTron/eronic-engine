#if BUILD_DIRECTX

#include "Renderer_DX.h"
#include "Mesh.h"
#include "GameObject.h"

/******************************************************************************************************************/

Renderer_DX::Renderer_DX(HWND hWnd)
	: _hWnd(hWnd)
{
}

/******************************************************************************************************************/

Renderer_DX::~Renderer_DX()
{
}

/******************************************************************************************************************/

void Renderer_DX::ClearScreen()
{
	_context->ClearRenderTargetView(_backbuffer, D3DXCOLOR(_clearColour.r(), _clearColour.g(), _clearColour.b(), _clearColour.a()));
}

/******************************************************************************************************************/

void Renderer_DX::Destroy()
{
	_swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	_layout->Release();
	_vertexShader->Release();
	_pixelShader->Release();
	_swapchain->Release();
	_backbuffer->Release();
	_device->Release();
	_context->Release();

	if (_uniformBuffer)	
	{
		_uniformBuffer->Release();
	}
}

/******************************************************************************************************************/

void Renderer_DX::Draw(const Mesh* mesh, glm::mat4 MVM, const Colour& colour)
{
	MVM = glm::transpose(MVM);

	UniformBuffer uniforms;
	memcpy(&uniforms.MVM, &MVM[0][0], sizeof(DirectX::XMFLOAT4X4));
	colour.copyToArray((float*)&uniforms.Colour);

	// Need to update uniform buffer here
	D3D11_MAPPED_SUBRESOURCE ms;
	_context->Map(_uniformBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);		// map the buffer
	memcpy(ms.pData, &uniforms, sizeof(UniformBuffer));								// copy the data
	_context->Unmap(_uniformBuffer, NULL);											// unmap the buffer
	_context->VSSetConstantBuffers(0, 1, &_uniformBuffer);
	_context->PSSetConstantBuffers(0, 1, &_uniformBuffer);

	mesh->GetVBO()->Draw(this);
}

/******************************************************************************************************************/

void Renderer_DX::Initialise(int width, int height)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = width;                   // set the back buffer width
	scd.BufferDesc.Height = height;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = _hWnd;                              // the window to be used
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&_swapchain,
		&_device,
		NULL,
		&_context);

	// get the address of the back buffer
	ID3D11Texture2D *p_backbuffer;
	_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_backbuffer);

	// use the back buffer address to create the render target
	_device->CreateRenderTargetView(p_backbuffer, NULL, &_backbuffer);
	p_backbuffer->Release();

	// set the render target as the back buffer
	_context->OMSetRenderTargets(1, &_backbuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	_context->RSSetViewports(1, &viewport);


	// Initialise shaders
	InitialiseShaders();
}

/******************************************************************************************************************/

void Renderer_DX::SwapBuffers()
{
	_swapchain->Present(0, 0);
}

/******************************************************************************************************************/

// Load and prepare shaders
void Renderer_DX::InitialiseShaders()
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &_vertexShader);
	_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &_pixelShader);

	// set the shader objects
	_context->VSSetShader(_vertexShader, 0, 0);
	_context->PSSetShader(_pixelShader, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	_device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &_layout);
	_context->IASetInputLayout(_layout);


	// Create uniform buffer
	UniformBuffer uniforms;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(uniforms);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &uniforms;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	_device->CreateBuffer(&cbDesc, &InitData, &_uniformBuffer);
}

/******************************************************************************************************************/

#endif