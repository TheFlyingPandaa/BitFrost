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

using namespace DirectX;

DXApp windowInstance;



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
		//InitInput(hInstance, windowInstance.getWndHandler());

		windowInstance.InitGameInput(hInstance);

		//SetViewport(); //3. Sätt viewport
		windowInstance.SetViewport();

		//CreateShaders(); //4. Skapa vertex- och pixel-shaders
		windowInstance.CreateShaders();

		windowInstance.CreateTriangleData();
		//CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		windowInstance.CreateConstantBuffer();
		windowInstance.CreateTexture();
		//CreateConstantBufferExample();

		ShowWindow(windowInstance.getWndHandler(), nCmdShow);

		//setActiveShaders();
		windowInstance.setActiveShaders();

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				windowInstance.Render(); //8. Rendera
				IDXGISwapChain* gSwapChain = windowInstance.getGSwapChain();
				gSwapChain->Present(0, 0); //9. Växla front- och back-buffer
				windowInstance.setGSwapChain(gSwapChain);
			}
		}

		

		
		DestroyWindow(windowInstance.getWndHandler());
	}

	return (int) msg.wParam;
}

