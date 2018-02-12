#ifndef STRUCTS_H
#define STRUCTS_H
#include <DirectXMath.h>
#include <minwinbase.h>
#include <winnt.h>


struct computeShader
{
	float val;
	float val1;
	float val2;
	float val3;
};

struct valuesFromCpu
{ 
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX worldSpace;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	DirectX::XMFLOAT3 dir;
	float pad;
	DirectX::XMFLOAT4 ambientLight;
	DirectX::XMFLOAT4 diffues;
	DirectX::XMFLOAT4 Position;
};

struct constBuffFrame
{
	Light light;
};

struct TriangleVertex
{
	float x, y, z;		//Cordinates for triangle
	float r, g;			//u,v cordinates
	//float nx, ny, nz;	//Normals
};

struct TextureRenderTarget
{
	ID3D11Texture2D*			texture = nullptr;
	ID3D11RenderTargetView*		renderTargetView = nullptr;
	ID3D11ShaderResourceView*	shaderResourceView = nullptr;
};

struct cameraBuffer {
	DirectX::XMFLOAT4A cameraPosition;
	DirectX::XMFLOAT3 cam;
};


#endif

