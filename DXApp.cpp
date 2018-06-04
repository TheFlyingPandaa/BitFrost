#include "DXApp.h"

#include "Matrial.h"

DXApp::DXApp()
{
	//gameInput = KeyboardInput();
	renderObject = new RenderObject(L"r8.obj", L"dick.jpg");
	renderObject->setPosition(0.5f, 2, 0.5f);
	renderObject->setScale(0.1f, 0.1f, 0.f);
	/*secondCube = new RenderObject("r8.obj", L"grass.jpg");
	secondCube->setPosition(-1, -1, -1);
	secondCube->setScale(.1f, .1f, .1f);
	*/
	ORH = new ObjectRenderHandler("Objects");
	ORH->loadObjects();

	//skyMap = new RenderObject(L"r8.obj", L"dick.jpg", true);
	//skyMap->setScale(10, 10, 10);
	heightMap = HeightMap();

	//Matrial m(L"Test.mtl", L"Material");
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
	deferredVertex->Release();
	deferredPixel->Release();

	constPerFrameBuffer->Release();
	gExampleBuffer->Release();
	
	camBuffer->Release();
	computeBuffer->Release();

	//gVertexBuffer->Release();

	//constPerFrameBuffer->Release();


	delete ORH;
	delete renderObject;


	//delete skyMap;
}

void DXApp::DxAppInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	wndHandle = InitWindow(hInstance); //Skapa f�nster

	if (wndHandle != NULL)
	{
		
		CreateDirect3DContext();	//2. Skapa och koppla SwapChain, Device och Device Context

		//InitGameInput(hInstance);	//2.5 kontroller
		gameInput.InitKeyboardInput(hInstance, wndHandle);//, DirectInput); //, DIKeyboard, DIMouse);

		SetViewport();	
		//3 viewPort
		CreateShader::CreateComputeShader(gDevice, gComputeShader);
		CreateShader::CreateShaders(gDevice, gVertexShader, gPixelShader, gGeomertyShader, gVertexLayout, deferredVertex, deferredPixel, gHullShader, gDomainShader);
		CreateShader::CreateShaders2(gDevice, shadowMapVertexShader, shadowMapPixelShader);
		//4. Shaders vertex osv

		CreateTriangleData();		//5. Triangeln. kommer bytas

		CreateConstantBuffer();

		setActiveShaders();
		
		InitGBuffer();

		ShowWindow(wndHandle, nCmdShow);

		//cubeTexture.createTexture(gDevice, L"grass.jpg");

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
	wcex.lpszClassName = gameName;
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		gameName,
		gameNameP,
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
	
	ID3D11RasterizerState * state;
	gDeviceContext->RSGetState(&state);

	
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

	exampleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	exampleBufferDesc.ByteWidth = sizeof(cameraBuffer);
	exampleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	exampleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	exampleBufferDesc.MiscFlags = 0;
	exampleBufferDesc.StructureByteStride = 0;
	hr = 0;
	hr = gDevice->CreateBuffer(&exampleBufferDesc, nullptr, &camBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}

	//FUCKING BULLSHIT. CONSTANT BUFFERS NEEDS A BYTEWIDTH OF 16 * ->WHATEVER
	//COMPUTESHADER FROM HERE
	exampleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	exampleBufferDesc.ByteWidth = sizeof(computeShader);
	exampleBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	exampleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	exampleBufferDesc.MiscFlags = 0;
	exampleBufferDesc.StructureByteStride = 0;
	hr = 0;
	hr = gDevice->CreateBuffer(&exampleBufferDesc, nullptr, &computeBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
	//OUTPUTBUFFER
	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_DEFAULT;
	outputDesc.ByteWidth = sizeof(computeShader) * NUM_PARTICLES;
	outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	outputDesc.CPUAccessFlags = 0;
	outputDesc.StructureByteStride = sizeof(computeShader);
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	hr = 0;
	hr  = gDevice->CreateBuffer(&outputDesc, 0, &computeOutputBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
	//SAME SHIT ASS ABOVE BUT FOR OTHER
	outputDesc.Usage = D3D11_USAGE_STAGING;
	outputDesc.BindFlags = 0;
	outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	hr = gDevice->CreateBuffer(&outputDesc, 0, &computeReadWriteBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = NUM_PARTICLES; //Number of "particles"
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	hr = gDevice->CreateUnorderedAccessView(computeOutputBuffer, &uavDesc, &computeBufferUAV);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}
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

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = WINDOW_WIDTH;
	texDesc.Height = WINDOW_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	gDevice->CreateTexture2D(&texDesc, NULL, &texShadowMap);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	gDevice->CreateDepthStencilView(texShadowMap, &dsvDesc, &shadowMapView);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	gDevice->CreateShaderResourceView(texShadowMap, &srvDesc, &shadowMapSRV);

	
}

void DXApp::CreateTriangleData()
{
	

	/*TriangleVertex triangleVertices[6] =
	{
		-0.5f, 0.5f, 0.0f,	//v0 pos
		0.0f, 0.0f,	//v0 color
		//1.0f, -1.0f, -1.0f,

		0.5f, -0.5f, 0.0f,	//v1
		1.0f, 1.0f,	//v1 color
		//1.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, 0.0f, //v2
		0.0f, 1.0f, 	//v2 color
		//1.0f, 1.0f, 1.0f,

		-0.5f, 0.5f, 0.0f,	//v0 pos
		0.0f, 0.0f, 	//v0 color
		//1.0f, -1.0f, -1.0f,

		0.5f, 0.5f, 0.0f,	//v1
		1.0f, 0.0f, 	//v1 color
		//1.0f, 1.0f, 1.0f,

		0.5f, -0.5f, 0.0f, //v2
		1.0f, 1.0f	//v2 color
		//1.0f, 1.0f, 1.0f,
	};*/



	XMFLOAT3 cameraStored = XMFLOAT3(0, 0, -2);
	XMFLOAT3 lookAtStored = XMFLOAT3(0, 0, 0);
	XMFLOAT3 UP_STORED = XMFLOAT3(0, 1, 0);

	cameraPos = XMLoadFloat3(&cameraStored);
	lookAt = XMLoadFloat3(&lookAtStored);
	UP = XMLoadFloat3(&UP_STORED);

	camView = XMMatrixLookAtLH(cameraPos, lookAt, UP);

	camProjection = XMMatrixPerspectiveFovLH(XM_PI * 0.45f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 20.0f);

	light.dir = XMFLOAT3(1.0f, 0.0f, 0.0f);
	light.pad = float(1);
	light.ambientLight = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	light.diffues = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	
	wwo = XMMatrixIdentity();

	wwo = camView * camProjection;

	light.Position = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//light.Position = XMFLOAT3(1, 0, 0);
	/*
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);
	//bufferDesc.ByteWidth = sizeof(mesh.floatArr());

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	//data.pSysMem = mesh.floatArr();

	//Need fix
	
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	*/
	ORH->loadBuffert(gDevice);
	renderObject->loadBuffer(gDevice);
	//skyMap->loadBuffer(gDevice);


}



void DXApp::Render()
{
	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1 };

	//KeyBoardInput();
	gameInput.GameInput(mouseLastState);

	UpdateCamera(camRotationMatrix, camTarget, cameraPos, camView, UP);
	//F� saken att rotera
	float rot = 0;

	worldMatrix = XMMatrixIdentity();

	XMVECTOR rotaxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	worldMatrix = Scale * Rotation * Translation;
	WVP = worldMatrix * camView * camProjection;

	ORH->setCamPosition(cameraPos, lookAt);
	ORH->setMatrix(camView, camProjection);

	//skyMap->setMatrix(camView, camProjection);
	//skyMap->setPosition(XMVectorGetX(cameraPos), XMVectorGetY(cameraPos), XMVectorGetZ(cameraPos));
	
	//gDeviceContext->PSSetShaderResources(0, 1, &cubeTexture.getTexture());
	//gDeviceContext->PSSetSamplers(0, 1, &cubeTexture.getSampleState());
	gDeviceContext->CSSetShader(gComputeShader, NULL, 0);

	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(computeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	//computeValuesStore.val = 1;
	computeValuesStore.output = XMFLOAT2(0,0); //Need Padding
	computeValuesStore.camPos = XMFLOAT2(XMVectorGetX(cameraPos), XMVectorGetZ(cameraPos));
	computeValuesStore.objectPos = XMFLOAT2(renderObject->getPosition().x, renderObject->getPosition().z);
	memcpy(dataPtr.pData, &computeValuesStore, sizeof(computeShader));
	// UnMap constant buffer so that we can use it again in the GPU
	gDeviceContext->Unmap(computeBuffer, 0);
	
	gDeviceContext->CSSetConstantBuffers(0, 1, &computeBuffer);
	gDeviceContext->CSSetUnorderedAccessViews(0,1, &computeBufferUAV, NULL);
	
	gDeviceContext->Dispatch(NUM_PARTICLES, 1, 1);
	
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	gDeviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	gDeviceContext->CSSetShader(NULL, NULL, 0);
	
	gDeviceContext->CopyResource(computeReadWriteBuffer, computeOutputBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = gDeviceContext->Map(computeReadWriteBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	float rotInRad = 0;
	if (SUCCEEDED(hr))
	{
		computeShader* dataView = reinterpret_cast<computeShader*>(mappedResource.pData);
		
		 rotInRad = dataView[0].output.x;

		gDeviceContext->Unmap(computeReadWriteBuffer, 0);
	}
	
	renderObject->setMatrix(camView, camProjection, rotInRad);
	renderObject->setPosition(2, 0, 2);
	renderObject->setScale(1, 1, 1);


	holdBuffPerFrame.light = light;

	//=========DRAW TO SCREEN==========\\

	MovingBuffersToGPU();

	FirstDrawPass();

	SecondDrawPass();
	
}

void DXApp::MovingBuffersToGPU()
{
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	// copy memory from CPU to GPU the entire struct
	globalValues.WVP = XMMatrixTranspose(WVP); // Transponera alltid innna n�got skickas in i matrisen.
	globalValues.worldSpace = XMMatrixTranspose(worldMatrix);
	//camBuff.cameraPosition = DirectX::XMFLOAT4A(XMVectorGetX(camTarget - cameraPos), XMVectorGetY(camTarget - cameraPos), XMVectorGetZ(camTarget - cameraPos), 1.0f);
	
	camBuff.cameraPosition = DirectX::XMFLOAT4A(XMVectorGetX(cameraPos), XMVectorGetY(cameraPos), XMVectorGetZ(cameraPos), 1.0f);
	camBuff.cameraDirection = DirectX::XMFLOAT4A(XMVectorGetX(camTarget - cameraPos), XMVectorGetY(camTarget - cameraPos), XMVectorGetZ(camTarget - cameraPos), 1.0f);
	
	//std::cout << XMVectorGetX(camTarget) << " " << XMVectorGetY(camTarget) << " " << XMVectorGetZ(camTarget) << std::endl;

	gDeviceContext->Map(gExampleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	//Kopierar in det i buffern "constant buffern"
	memcpy(dataPtr.pData, &globalValues, sizeof(valuesFromCpu));
	// UnMap constant buffer so that we can use it again in the GPU
	gDeviceContext->Unmap(gExampleBuffer, 0);
	// set resource to Vertex Shader
	gDeviceContext->GSSetConstantBuffers(0, 1, &gExampleBuffer);
	 
	//D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(constPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	// copy memory from CPU to GPU the entire struct
	//Kopierar in det i buffern "constant buffern"
	memcpy(dataPtr.pData, &holdBuffPerFrame, sizeof(constBuffFrame));
	// UnMap constant buffer so that we can use it again in the GPU
	gDeviceContext->Unmap(constPerFrameBuffer, 0);
	// set resource to Vertex Shader
	gDeviceContext->PSSetConstantBuffers(1, 1, &constPerFrameBuffer);

	gDeviceContext->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	//Kopierar in det i buffern "constant buffern"
	memcpy(dataPtr.pData, &camBuff, sizeof(cameraBuffer));
	// UnMap constant buffer so that we can use it again in the GPU
	gDeviceContext->Unmap(camBuffer, 0);
	// set resource to Vertex Shader
	gDeviceContext->GSSetConstantBuffers(4, 1, &camBuffer);
	gDeviceContext->PSSetConstantBuffers(4, 1, &camBuffer);

	//computeShader
	


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

void DXApp::DrawGeometry()
{
	//gDeviceContext->Draw(6, 0);
	//renderObject->draw(gDeviceContext, new XMFLOAT3(XMVectorGetX(cameraPos), XMVectorGetY(cameraPos), XMVectorGetZ(cameraPos)));
	ORH->render(gDeviceContext);


	//skyMap->draw(gDeviceContext);
}


void DXApp::FirstDrawPass()
{
	gDeviceContext->IASetInputLayout(gVertexLayout);

	gDeviceContext->OMSetRenderTargets(0, 0, shadowMapView);
	gDeviceContext->ClearDepthStencilView(shadowMapView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	gDeviceContext->VSSetShader(this->shadowMapVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(this->shadowMapPixelShader, nullptr, 0);

	DrawGeometry();

	UINT32 vertexSize = sizeof(float) * amountOfValuesInVertex; //The const (5) is the amount of vertexes
	UINT32 offset = 0;

	//gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	//gDeviceContext->IASetInputLayout(gVertexLayout);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.CullMode = D3D11_CULL_NONE;
	if (debugMode)
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	else
		wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.DepthClipEnable = true;
	gDevice->CreateRasterizerState(&wfdesc, &Rasterizer);
	gDeviceContext->RSSetState(Rasterizer);

	gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(this->gHullShader, nullptr, 0);
	gDeviceContext->DSSetShader(this->gDomainShader, nullptr, 0);
	gDeviceContext->GSSetShader(this->gGeomertyShader, nullptr, 0);
	gDeviceContext->PSSetShader(this->gPixelShader, nullptr, 0);

	float Color1[] = { 0.0f,0.0f, 1.0f, 1 };
	float Color2[] = { 0.0f, 0.0f, 0.0f, 1 };

	ID3D11RenderTargetView* renderTargets[]{
		graphicsBuffer[0].renderTargetView,
		graphicsBuffer[1].renderTargetView,
		graphicsBuffer[2].renderTargetView
	};

	gDeviceContext->OMSetRenderTargets(BUFFER_COUNT, renderTargets, depthStencilView);

	gDeviceContext->ClearRenderTargetView(graphicsBuffer[0].renderTargetView, Color1);
	gDeviceContext->ClearRenderTargetView(graphicsBuffer[1].renderTargetView, Color2);
	gDeviceContext->ClearRenderTargetView(graphicsBuffer[2].renderTargetView, Color2);
	gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	//=====THIS IS THE DRAW FUNCTION=====\\

	DrawGeometry();

	wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.CullMode = D3D11_CULL_NONE;
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.DepthClipEnable = true;
	gDevice->CreateRasterizerState(&wfdesc, &Rasterizer);
	gDeviceContext->RSSetState(Rasterizer);
}

void DXApp::SecondDrawPass()
{
	float Color1[] = { 1.0f, 0.1f, 0.1f, 1 };
	gDeviceContext->VSSetShader(this->deferredVertex, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(this->deferredPixel, nullptr, 0);

	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, nullptr);
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, Color1);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	gDeviceContext->PSSetShaderResources(1, 1, &graphicsBuffer[0].shaderResourceView);
	gDeviceContext->PSSetShaderResources(2, 1, &graphicsBuffer[1].shaderResourceView);
	gDeviceContext->PSSetShaderResources(3, 1, &graphicsBuffer[2].shaderResourceView);
	gDeviceContext->PSSetShaderResources(4, 1, &shadowMapSRV);

	
	gDeviceContext->Draw(4, 0);
}

void DXApp::HeghtMaping()
{
	
}

void DXApp::InitGBuffer()
{
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = WINDOW_WIDTH;
	textureDesc.Height = WINDOW_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		gDevice->CreateTexture2D(&textureDesc, NULL, &graphicsBuffer[i].texture);
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;


	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		gDevice->CreateRenderTargetView(graphicsBuffer[i].texture, &renderTargetViewDesc, &graphicsBuffer[i].renderTargetView);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (size_t i = 0; i < BUFFER_COUNT; i++)
	{
		gDevice->CreateShaderResourceView(graphicsBuffer[i].texture, &shaderResourceViewDesc, &graphicsBuffer[i].shaderResourceView);
	}

}