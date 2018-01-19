#include "DXApp.h"



DXApp::DXApp()
{
	//gameInput = KeyboardInput();
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
	
	

	gVertexBuffer->Release();

	constPerFrameBuffer->Release();
	CubesTexture->Release();
	CubesTexSamplerState->Release();
}

void DXApp::DxAppInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	wndHandle = InitWindow(hInstance); //Skapa f�nster

	if (wndHandle != NULL)
	{
		
		CreateDirect3DContext();	//2. Skapa och koppla SwapChain, Device och Device Context

		//InitGameInput(hInstance);	//2.5 kontroller
		gameInput.InitKeyboardInput(hInstance, wndHandle);//, DirectInput); //, DIKeyboard, DIMouse);

		SetViewport();				//3 viewPort

		CreateShaders();			//4. Shaders vertex osv

		CreateTriangleData();		//5. Triangeln. kommer bytas

		CreateConstantBuffer();

		CreateTexture();

		setActiveShaders();
		
		ShowWindow(wndHandle, nCmdShow);

	}



}

void DXApp::Update(float dt)
{
	
	Render();

	gSwapChain->Present(0, 0);

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

	//InitGameInput(hInstance);

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

	light.dir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	light.ambientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffues = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

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



void DXApp::Render()
{
	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1 };

	//KeyBoardInput();
	gameInput.GameInput(/*DIKeyboard, DIMouse, moveLeftRight, moveBackForward, camYaw, camPitch,*/ mouseLastState);//,DirectInput);

	UpdateCamera(camRotationMatrix, camTarget, cameraPos, camView, UP);
	//F� saken att rotera
	float rot = 0;
	//if (rot > 6.26f)
	//	rot = 0.0f;
	//rot = 1.50f;
	//Tom v�rldsmatris
	worldMatrix = XMMatrixIdentity();

	XMVECTOR rotaxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f); -

	worldMatrix = Scale * Rotation * Translation;

	

	WVP = worldMatrix * camView * camProjection;

	//ID3D11ShaderResourceView* CubesTexture = getCubesTexture();
	//ID3D11SamplerState* CubesTexSamplerState = getCubesTexSamplerState();

	
	
	gDeviceContext->PSSetShaderResources(0, 1, &CubesTexture);
	gDeviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);

	UINT32 vertexSize = sizeof(float) * 8; //The const (5) is the amount of vertexes
	UINT32 offset = 0;

	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetInputLayout(gVertexLayout);

	holdBuffPerFrame.light = light;

	gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Map constant buffer so that we can write to it.

	MovingBuffersToGPU();
	

	// ==============================================================

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// clear screen

	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

	// draw geometry
	gDeviceContext->Draw(6, 0);
}

void DXApp::MovingBuffersToGPU()
{
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(gExampleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	// copy memory from CPU to GPU the entire struct
	globalValues.WVP = XMMatrixTranspose(WVP); // Transponera alltid innna n�got skickas in i matrisen.
	globalValues.worldSpace = XMMatrixTranspose(worldMatrix);
	//Kopierar in det i buffern "constant buffern"
	memcpy(dataPtr.pData, &globalValues, sizeof(valuesFromCpu));
	// UnMap constant buffer so that we can use it again in the GPU
	gDeviceContext->Unmap(gExampleBuffer, 0);
	// set resource to Vertex Shader
	gDeviceContext->GSSetConstantBuffers(0, 1, &gExampleBuffer);

	D3D11_MAPPED_SUBRESOURCE dataPtr2;
	gDeviceContext->Map(constPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr2);
	// copy memory from CPU to GPU the entire struct
	//Kopierar in det i buffern "constant buffern"
	memcpy(dataPtr2.pData, &holdBuffPerFrame, sizeof(constBuffFrame));
	// UnMap constant buffer so that we can use it again in the GPU
	gDeviceContext->Unmap(constPerFrameBuffer, 0);
	// set resource to Vertex Shader
	gDeviceContext->PSSetConstantBuffers(1, 1, &constPerFrameBuffer);
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

void DXApp::UpdateCamera(XMMATRIX & camRotationMatrix, XMVECTOR  &camTarget, XMVECTOR &cameraPos, XMMATRIX &camView, XMVECTOR &UP)
{

	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	camRotationMatrix = XMMatrixRotationRollPitchYaw(gameInput.getCamPitch(), gameInput.getCamYaw(), 0);
	camTarget = XMVector3TransformCoord(gameInput.DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(gameInput.getCamYaw());

	camRight = XMVector3TransformCoord(gameInput.DefaultRight, RotateYTempMatrix);
	UP = XMVector3TransformCoord(UP, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(gameInput.DefaultForward, RotateYTempMatrix);

	cameraPos += gameInput.getMoveLeftRight() * camRight;
	cameraPos += gameInput.getMoveBackForward() * camForward;

	gameInput.setMoveLeftRight(0);
	gameInput.setMoveBackForward(0);
	camTarget = cameraPos + camTarget;

	camView = XMMatrixLookAtLH(cameraPos, camTarget, UP);
}

HWND DXApp::getWndHandler() const
{
	return this->wndHandle;
}
