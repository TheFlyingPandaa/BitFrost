#ifndef STRUCTS_H
#define STRUCTS_H
#include <DirectXMath.h>
#include <minwinbase.h>
#include <winnt.h>

struct valuesFromCpu
{ // NEW
	float value1; // NEW
	float value2; // NEW
	float value3; // NEW
	float value4;

	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX worldSpace;// NEW
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
};

struct constBuffFrame
{
	Light light;
};

struct TriangleVertex
{
	float x, y, z;		//Cordinates for triangle
	float r, g;			//u,v cordinates
	float nx, ny, nz;	//Normals
};

#endif

