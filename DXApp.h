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

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectX/lib/dinput8.lib")
#pragma comment (lib, "DirectX/lib/dwrite.lib")
#pragma comment (lib, "DirectX/lib/dxguid.lib")

using namespace DirectX;

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

	void InitGameInput(HINSTANCE hInstance);
	void KeyBoardInput();
	void UpdateCamera(XMMATRIX & camRotationMatrix, XMVECTOR & camTarget, XMVECTOR & cameraPos, XMMATRIX  &camView, XMVECTOR& UP);

	void MovingBuffersToGPU();

	HWND getWndHandler() const;

	

private:

	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;

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
	valuesFromCpu globalValues{ 0.0,0.0,0.0,0.0 };
	Light light;

	constBuffFrame holdBuffPerFrame;

	ID3D11Buffer* gVertexBuffer = nullptr;

	XMVECTOR cameraPos;
	XMVECTOR lookAt;
	XMVECTOR UP;

	XMMATRIX camView;
	XMMATRIX camProjection;

	XMMATRIX WVP;

private:
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

	float rotx = 0;
	float rotz = 0;
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	DirectX::XMMATRIX Rotationx;
	DirectX::XMMATRIX Rotationz;
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float camYaw = 0.0f;
	float camPitch = 0.0f;

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

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

