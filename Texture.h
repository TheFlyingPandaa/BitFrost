#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <Windows.h>
#include "DirectX/include/d3dx11tex.h"

#pragma comment (lib, "d3d11.lib")

class Texture
{
public:
	Texture();
	~Texture();

	void createTexture(ID3D11Device*& gDevice, LPCWSTR textureName);
private:

	ID3D11ShaderResourceView * texture;
	ID3D11SamplerState* samplerState;
public:
	ID3D11SamplerState* const& getSampleState() const
	{
		return samplerState;
	}
	ID3D11ShaderResourceView * const& getTexture() const
	{
		return texture;
	}
};

#endif

