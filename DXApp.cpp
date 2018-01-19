#include "DXApp.h"



DXApp::DXApp()
{
}

DXApp::~DXApp()
{

	gSwapChain->Release();
	gDevice->Release();
	gDeviceContext->Release();
	gBackbufferRTV->Release();

	depthStencilBuffer->Release();
	depthStencilView->Release();

	gVertexLayout->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	gGeomertyShader->Release();

	constPerFrameBuffer->Release();
	gExampleBuffer->Release();
}


HWND DXApp::InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	InitGameInput(hInstance);

	return handle;
}

HRESULT DXApp::CreateDirect3DContext()
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples When using a depth buffer there can't be more the 1 sample
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();


		CreateDepthBuffer();

	}
	return hr;
}

void DXApp::SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)WINDOW_WIDTH;
	vp.Height = (float)WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}

void DXApp::CreateShaders()
{
	//create vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);


	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"Geometry.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see her
	);

	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeomertyShader);

	pGS->Release();

}

void DXApp::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC exampleBufferDesc;
	exampleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	exampleBufferDesc.ByteWidth = sizeof(valuesFromCpu);
	exampleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	exampleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	exampleBufferDesc.MiscFlags = 0;
	exampleBufferDesc.StructureByteStride = 0;

	// check if the creation failed for any reason
	HRESULT hr = 0;
	hr = gDevice->CreateBuffer(&exampleBufferDesc, nullptr, &gExampleBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}

	exampleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	exampleBufferDesc.ByteWidth = sizeof(constBuffFrame);
	exampleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	exampleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	exampleBufferDesc.MiscFlags = 0;
	exampleBufferDesc.StructureByteStride = 0;
	hr = 0;
	hr = gDevice->CreateBuffer(&exampleBufferDesc, nullptr, &constPerFrameBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
//
//	// init for the light
//	exampleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	exampleBufferDesc.ByteWidth = sizeof(constBuffFrame);
//	exampleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	exampleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	exampleBufferDesc.MiscFlags = 0;
//	exampleBufferDesc.StructureByteStride = 0;
//	hr = 0;
//	hr = gDevice->CreateBuffer(&exampleBufferDesc, nullptr, &constPerFrameBuffer);
//	if (FAILED(hr))
//	{
//		// handle the error, could be fatal or a warning...
//		exit(-1);
//	}
}

void DXApp::setActiveShaders()
{

	gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(this->gGeomertyShader, nullptr, 0);
	gDeviceContext->PSSetShader(this->gPixelShader, nullptr, 0);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void DXApp::CreateDepthBuffer() {

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = WINDOW_WIDTH;
	depthStencilDesc.Height = WINDOW_HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	gDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	gDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);


	// set the render target as the back buffer
	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, depthStencilView);
}

void DXApp::CreateTriangleData()
{
	struct TriangleVertex
	{
		float x, y, z;		//Cordinates for triangle
		float r, g;			//u,v cordinates
		float nx, ny, nz;	//Normals
	};

	TriangleVertex triangleVertices[6] =
	{
		-0.5f, 0.5f, 0.0f,	//v0 pos
		0.0f, 0.0f,	//v0 color
		1.0f, -1.0f, -1.0f,

		0.5f, -0.5f, 0.0f,	//v1
		1.0f, 1.0f,	//v1 color
		1.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, 0.0f, //v2
		0.0f, 1.0f, 	//v2 color
		1.0f, 1.0f, 1.0f,

		-0.5f, 0.5f, 0.0f,	//v0 pos
		0.0f, 0.0f, 	//v0 color
		1.0f, -1.0f, -1.0f,

		0.5f, 0.5f, 0.0f,	//v1
		1.0f, 0.0f, 	//v1 color
		1.0f, 1.0f, 1.0f,

		0.5f, -0.5f, 0.0f, //v2
		1.0f, 1.0f,	//v2 color
		1.0f, 1.0f, 1.0f,
	};



	XMFLOAT3 cameraStored = XMFLOAT3(0, 0, -2);
	XMFLOAT3 lookAtStored = XMFLOAT3(0, 0, 0);
	XMFLOAT3 UP_STORED = XMFLOAT3(0, 1, 0);

	cameraPos = XMLoadFloat3(&cameraStored);
	lookAt = XMLoadFloat3(&lookAtStored);
	UP = XMLoadFloat3(&UP_STORED);

	camView = XMMatrixLookAtLH(cameraPos, lookAt, UP);

	camProjection = XMMatrixPerspectiveFovLH(XM_PI * 0.45f, 640.0f / 480.0f, 0.1f, 20.0f);

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	//Need fix
	
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);

}



void DXApp::CreateTexture()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	D3DX11CreateShaderResourceViewFromFile(gDevice, L"grass.jpg", NULL, NULL, &CubesTexture, NULL);

	gDevice->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
}

void DXApp::InitGameInput(HINSTANCE hInstance)
{
	DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);

	DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(this->wndHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(this->wndHandle, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

}

void DXApp::KeyBoardInput()
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	float speed = 0.0001f;


	if (keyboardState[DIK_ESCAPE] & 0x80)
	{
		//superQuit = true;
	}
	if (keyboardState[DIK_A] & 0x80)
	{
		moveLeftRight -= speed;
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		moveLeftRight += speed;
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		moveBackForward += speed;
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		moveBackForward -= speed;
	}
	if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		camYaw += mouseLastState.lX * 0.001f;

		camPitch += mouseCurrState.lY * 0.001f;

		mouseLastState = mouseCurrState;
	}
}

void DXApp::UpdateCamera(XMMATRIX & camRotationMatrix, XMVECTOR  &camTarget, XMVECTOR &cameraPos, XMMATRIX &camView, XMVECTOR &UP)
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	UP = XMVector3TransformCoord(UP, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	cameraPos += moveLeftRight * camRight;
	cameraPos += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = cameraPos + camTarget;

	camView = XMMatrixLookAtLH(cameraPos, camTarget, UP);
}

void DXApp::setWndHandler(HWND wndHandle)
{

	this->wndHandle = wndHandle;

}

HWND DXApp::getWndHandler()
{
	return this->wndHandle;
}

void DXApp::setDepthStencilView(ID3D11DepthStencilView * depthStencilView)
{

	this->depthStencilView = depthStencilView;
}

ID3D11DepthStencilView * DXApp::getDepthStencilView()
{
	return depthStencilView;
}

void DXApp::setDeviceContext(ID3D11DeviceContext * getDeviceContext)
{
	gDeviceContext = getDeviceContext;
}

ID3D11DeviceContext * DXApp::getDeviceContext()
{
	return gDeviceContext;
}

void DXApp::setDevice(ID3D11Device * gDevice)
{
	this->gDevice = gDevice;
}

ID3D11Device * DXApp::getDevice()
{
	return this->gDevice;
}