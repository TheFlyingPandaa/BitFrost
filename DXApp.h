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


	HWND InitWindow(HINSTANCE hInstance);
	HRESULT CreateDirect3DContext();
	void SetViewport();
	void CreateShaders();
	void CreateConstantBuffer();
	void setActiveShaders();
	void CreateDepthBuffer();

	void CreateTexture();

	void InitGameInput(HINSTANCE hInstance);
	void KeyBoardInput();
	void UpdateCamera(XMMATRIX & camRotationMatrix, XMVECTOR & camTarget, XMVECTOR & cameraPos, XMMATRIX  &camView, XMVECTOR& UP);



	void setWndHandler(HWND wndHandle);
	HWND getWndHandler();

	void setDepthStencilView(ID3D11DepthStencilView* depthStencilView);
	ID3D11DepthStencilView * getDepthStencilView();

	void setDeviceContext(ID3D11DeviceContext* getDeviceContext);
	ID3D11DeviceContext* getDeviceContext();
	
	void setDevice(ID3D11Device* gDevice);
	ID3D11Device* getDevice();

private:

	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;

	HWND wndHandle;

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


public:
	ID3D11ShaderResourceView* getCubesTexture() const
	{
		return CubesTexture;
	}

	void setCubesTexture(ID3D11ShaderResourceView* cubes_texture)
	{
		CubesTexture = cubes_texture;
	}

	ID3D11SamplerState* getCubesTexSamplerState() const
	{
		return CubesTexSamplerState;
	}

	void setCubesTexSamplerState(ID3D11SamplerState* cubes_tex_sampler_state)
	{
		CubesTexSamplerState = cubes_tex_sampler_state;
	}

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
	ID3D11RenderTargetView * getGBackbufferRtv() const
	{
		return gBackbufferRTV;
	}

	void setGBackbufferRtv(ID3D11RenderTargetView* backbuffer_rtv)
	{
		gBackbufferRTV = backbuffer_rtv;
	}
	IDXGISwapChain* getGSwapChain() const
	{
		return gSwapChain;
	}

	void setGSwapChain(IDXGISwapChain* swap_chain)
	{
		gSwapChain = swap_chain;
	}
	ID3D11InputLayout * getGVertexLayout() const
	{
		return gVertexLayout;
	}

	void setGVertexLayout(ID3D11InputLayout* vertex_layout)
	{
		gVertexLayout = vertex_layout;
	}

	ID3D11Buffer* getConstPerFrameBuffer() const
	{
		return constPerFrameBuffer;
	}

	void setConstPerFrameBuffer(ID3D11Buffer* const_per_frame_buffer)
	{
		constPerFrameBuffer = const_per_frame_buffer;
	}

	ID3D11Buffer* getGExampleBuffer() const
	{
		return gExampleBuffer;
	}

	void setGExampleBuffer(ID3D11Buffer* example_buffer)
	{
		gExampleBuffer = example_buffer;
	}

private:
	// NEW
	

};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



#endif

