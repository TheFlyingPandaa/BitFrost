//--------------------------------------------------------------------------------------
// BTH - Joakim Trossvik 2018
//--------------------------------------------------------------------------------------
#include <windows.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include "DirectX/include/D3DX11.h"
#include "DirectX/include/dwrite.h"
#include "DXApp.h"
#include <ctime>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "DirectX/lib/d3dx11.lib")
#pragma comment (lib, "DirectX/lib/dwrite.lib")
#pragma comment (lib, "DirectX/lib/dxguid.lib")

const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

using namespace DirectX;

DXApp windowInstance;

//test
#include "Mesh.h"
//test end



int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//Mesh mesh("r8.obj");

	MSG msg = { 0 };

	windowInstance.DxAppInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	float newTime = clock();
	float oldTime = 0;
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			oldTime = newTime;
			newTime = clock();
			float dt = newTime - oldTime;
			windowInstance.Update(dt);
		}
	}

	DestroyWindow(windowInstance.getWndHandler());

	return (int) msg.wParam;
}

