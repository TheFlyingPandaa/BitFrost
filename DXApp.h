#ifndef DXAPP_H
#define DXAPP_H
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DirectX/include/dinput.h"
#include "DirectX/include/dwrite.h"
#include "DirectX/include/d3dx11tex.h"
#include <DirectXMath.h>
#include "Structs.h"
#include "KeyboardInput.h"
#include <string>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectX/lib/dinput8.lib")
#pragma comment (lib, "DirectX/lib/dwrite.lib")
#pragma comment (lib, "DirectX/lib/dxguid.lib")

using namespace DirectX;


#include "RenderObject.h"

const UINT BUFFER_COUNT = 3;

class DXApp
{
	
public:
	DXApp();
	~DXApp();

	void DxAppInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
	void Update(float dt);

	HWND InitWindow(HINSTANCE hInstance);
	HRESULT CreateDirect3DContext();
	void SetViewport();
	void CreateShaders();
	void CreateConstantBuffer();
	void setActiveShaders();
	void CreateDepthBuffer();

	void CreateTriangleData();
	
	void Render();
	
	void CreateTexture();

	void UpdateCamera(XMMATRIX & camRotationMatrix, XMVECTOR & camTarget, XMVECTOR & cameraPos, XMMATRIX  &camView, XMVECTOR& UP);

	void MovingBuffersToGPU();

	HWND getWndHandler() const;

	void DrawGeometry();

	void InitGBuffer();
	void FirstDrawPass();
	void SecondDrawPass();

private:

	LPCWSTR gameName = L"wop";
	LPCWSTR gameNameP = L"bitFrost Cuz Names are stupid";
	const float WINDOW_WIDTH = 640;
	const float WINDOW_HEIGHT = 480;
	int amountOfValuesInVertex = 5;

	TextureRenderTarget graphicsBuffer[BUFFER_COUNT];
	ID3D11VertexShader* deferredVertex = nullptr;
	ID3D11PixelShader* deferredPixel = nullptr;

	KeyboardInput gameInput;

	HWND wndHandle = NULL;

	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11Device* gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11RenderTargetView* gBackbufferRTV = nullptr;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11InputLayout* gVertexLayout = nullptr;

	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11PixelShader* gPixelShader = nullptr;
	ID3D11GeometryShader* gGeomertyShader = nullptr;

	ID3D11Buffer* gExampleBuffer = nullptr;
	ID3D11Buffer* constPerFrameBuffer = nullptr;

	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;

	XMMATRIX worldMatrix;
	XMVECTOR camTarget;
	XMMATRIX camRotationMatrix;
	valuesFromCpu globalValues{ };
	Light light;

	constBuffFrame holdBuffPerFrame;

	ID3D11Buffer* gVertexBuffer = nullptr;

	XMVECTOR cameraPos;
	XMVECTOR lookAt;
	XMVECTOR UP;

	XMMATRIX camView;
	XMMATRIX camProjection;

	XMMATRIX WVP;

	DIMOUSESTATE mouseLastState;


	//Mesh mesh;
	RenderObject * renderObject;

public:
	IDXGISwapChain* getGSwapChain() const
	{
		return gSwapChain;
	}

	void setGSwapChain(IDXGISwapChain* swap_chain)
	{
		gSwapChain = swap_chain;
	}
	
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



#endif

