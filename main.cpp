//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014.
//--------------------------------------------------------------------------------------
#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory.h>
#include "DirectX/include/D3DX11.h"
//#include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX11.h>
//#include <dinput.h>
#include "DirectX/include/dinput.h"
#include "DirectX/include/dwrite.h"
#include "DXApp.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectX/lib/d3dx11.lib")
#pragma comment (lib, "DirectX/lib/dinput8.lib")
#pragma comment (lib, "DirectX/lib/dwrite.lib")
#pragma comment (lib, "DirectX/lib/dxguid.lib")

const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

bool superQuit = false;

//#include "bth_image.h"

using namespace DirectX;



DXApp windowInstance;

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDirect3DContext(HWND wndHandle);


ID3D11Buffer* gVertexBuffer = nullptr;


ID3D11ShaderResourceView* gTextureView = NULL;

ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;

ID3D11Texture2D *pTexture = NULL;

ID3D11ShaderResourceView* CubesTexture;
ID3D11SamplerState* CubesTexSamplerState;
bool textureFromFile = true;

ID3D11Buffer* cbPerFrameBuffer; //Light buffern

XMMATRIX lastMatrix;

XMMATRIX worldMatrix;

XMMATRIX camView;
XMMATRIX camProjection;

XMVECTOR cameraPos;
XMVECTOR lookAt;
XMVECTOR UP;


//FIRST PERSON

XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

XMVECTOR camTarget;

XMMATRIX test;
XMMATRIX camRotationMatrix;
XMMATRIX groundWorld;

float moveLeftRight = 0.0f;
float moveBackForward = 0.0f;

float camYaw = 0.0f;
float camPitch = 0.0f;

//STOP FIRST PERSON

//Keyboard input

IDirectInputDevice8* DIKeyboard;
IDirectInputDevice8* DIMouse;

DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

float rotx = 0;
float rotz = 0;
float scaleX = 1.0f;
float scaleY = 1.0f;

XMMATRIX Rotationx;
XMMATRIX Rotationz;

//----------------------------------------------\\


float rot = 0.01f;





// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476898(v=vs.85).aspx
ID3D11Buffer* gExampleBuffer = nullptr; // NEW
struct valuesFromCpu {					// NEW
	float value1;						// NEW
	float value2;						// NEW
	float value3;						// NEW
	float value4;
	
	XMMATRIX WVP;
	XMMATRIX worldSpace;// NEW
};
valuesFromCpu globalValues{ 0.0,0.0,0.0,0.0 }; // NEW

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambientLight;
	XMFLOAT4 diffues;

};

Light light;


ID3D11Buffer* constPerFrameBuffer = nullptr;
struct constBuffFrame
{
	Light light;
};

constBuffFrame holdBuffPerFrame;

bool InitInput(HINSTANCE hInstance, HWND hwnd)
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
	DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}

void CreateConstantBufferExample() // NEW
{

	ID3D11Device* gDevice = windowInstance.getDevice();


	// initializing the constBuffer for the geometry shader
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

	// init for the light
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

	//TEXTURE

	if (textureFromFile == false)
	{
		/*D3D11_TEXTURE2D_DESC bthTexDesc;
		ZeroMemory(&bthTexDesc, sizeof(bthTexDesc));
		bthTexDesc.Width = BTH_IMAGE_WIDTH;
		bthTexDesc.Height = BTH_IMAGE_HEIGHT;
		bthTexDesc.MipLevels = bthTexDesc.ArraySize = 1;
		bthTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bthTexDesc.SampleDesc.Count = 1;
		bthTexDesc.SampleDesc.Quality = 0;
		bthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		bthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bthTexDesc.MiscFlags = 0;
		bthTexDesc.CPUAccessFlags = 0;


		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = (void*)BTH_IMAGE_DATA;
		data.SysMemPitch = BTH_IMAGE_WIDTH * 4 * sizeof(char);
		hr = gDevice->CreateTexture2D(&bthTexDesc, &data, &pTexture);

		D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
		ZeroMemory(&resViewDesc, sizeof(resViewDesc));
		resViewDesc.Format = bthTexDesc.Format;
		resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resViewDesc.Texture2D.MipLevels = bthTexDesc.MipLevels;
		resViewDesc.Texture2D.MostDetailedMip = 0;
		hr = gDevice->CreateShaderResourceView(pTexture, &resViewDesc, &gTextureView);

		pTexture->Release();*/
	}
	else {
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

		hr = gDevice->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
	}

	windowInstance.setDevice(gDevice);
}

void CreateTriangleData()
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
	ID3D11Device* gDevice = windowInstance.getDevice();
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	windowInstance.setDevice(gDevice);
}


void UpdateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	UP = XMVector3TransformCoord(UP, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	cameraPos += moveLeftRight*camRight;
	cameraPos += moveBackForward*camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = cameraPos + camTarget;

	camView = XMMatrixLookAtLH(cameraPos, camTarget, UP);
}

void KeyboardInput()
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
		superQuit = true;
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

	

	UpdateCamera();
}

void movingConstBuffToGPU(ID3D11DeviceContext* gDeviceContext, XMMATRIX WVP)
{
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(gExampleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	// copy memory from CPU to GPU the entire struct
	globalValues.WVP = XMMatrixTranspose(WVP); // Transponera alltid innna något skickas in i matrisen.
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

void Render()
{
	ID3D11DeviceContext* gDeviceContext = windowInstance.getDeviceContext();

	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1 };

	KeyboardInput();
	//Få saken att rotera
	//rot += .0001f;
	//if (rot > 6.26f)
	//	rot = 0.0f;
	//rot = 1.50f;
	//Tom världsmatris
	worldMatrix = XMMatrixIdentity();

	XMVECTOR rotaxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
 	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	worldMatrix = Scale * Rotation * Translation;
	
	XMMATRIX WVP;

	WVP = worldMatrix * camView * camProjection;


	if (textureFromFile == false)
	{
		gDeviceContext->PSSetShaderResources(0, 1, &gTextureView);
	}
	else {
		gDeviceContext->PSSetShaderResources(0, 1, &CubesTexture);
		gDeviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
	}

	UINT32 vertexSize = sizeof(float) * 8; //The const (5) is the amount of vertexes
	UINT32 offset = 0;

	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetInputLayout(windowInstance.getGVertexLayout());

	holdBuffPerFrame.light = light;

	depthStencilView = windowInstance.getDepthStencilView();

	gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	windowInstance.setDepthStencilView(depthStencilView);

	// Map constant buffer so that we can write to it.
	
	movingConstBuffToGPU(gDeviceContext, WVP);
	
	// ==============================================================

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// clear screen

	ID3D11RenderTargetView* gBackbufferRTV = windowInstance.getGBackbufferRtv();
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	windowInstance.setGBackbufferRtv(gBackbufferRTV);
	// draw geometry
	gDeviceContext->Draw(6, 0);

	windowInstance.setDeviceContext(gDeviceContext);

}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	
	MSG msg = { 0 };
	windowInstance.setWndHandler(windowInstance.InitWindow(hInstance));//1. Skapa fönster
	
	if (windowInstance.getWndHandler())
	{
		//2. Skapa och koppla SwapChain, Device och Device Context
		windowInstance.CreateDirect3DContext();

		//2.5 COOL AS 3D KAMERA WOSH WOSH 
		//NEEDS FIXING GUD KOMMER MÖRDA OSS
		InitInput(hInstance, windowInstance.getWndHandler());

		//SetViewport(); //3. Sätt viewport
		windowInstance.SetViewport();

		//CreateShaders(); //4. Skapa vertex- och pixel-shaders
		windowInstance.CreateShaders();

		CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		CreateConstantBufferExample();

		ShowWindow(windowInstance.getWndHandler(), nCmdShow);

		//setActiveShaders();
		windowInstance.setActiveShaders();

		while (WM_QUIT != msg.message && superQuit == false)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Render(); //8. Rendera
				IDXGISwapChain* gSwapChain = windowInstance.getGSwapChain();
				gSwapChain->Present(0, 0); //9. Växla front- och back-buffer
				windowInstance.setGSwapChain(gSwapChain);
			}
		}

		gVertexBuffer->Release();

		constPerFrameBuffer->Release();
		CubesTexture->Release();
		CubesTexSamplerState->Release();

		DIKeyboard->Unacquire();
		DIMouse->Unacquire();
		DirectInput->Release();
		DestroyWindow(windowInstance.getWndHandler());
	}

	return (int) msg.wParam;
}

