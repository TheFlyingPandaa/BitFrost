#ifndef DXAPP_H
#define DXAPP_H
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class DXApp
{
	
public:
	DXApp();
	~DXApp();

	HWND InitWindow(HINSTANCE hInstance);
	HRESULT CreateDirect3DContext();
	void SetViewport();
	void CreateShaders();
	//void CreateConstantBufferExample();
	void setActiveShaders();

	void createDepthBuffer();

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

};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



#endif

