#include "Texture.h"

Texture::Texture()
{
	texture = nullptr;
	samplerState = nullptr;
}

Texture::~Texture()
{
	if (texture)
		texture->Release();
	if (samplerState)
		samplerState->Release();
}

void Texture::createTexture(ID3D11Device*& gDevice, LPCWSTR textureName)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(gDevice, textureName, NULL, NULL, &texture, NULL);
	if (hr != S_OK)
	{
		D3DX11CreateShaderResourceViewFromFile(gDevice, L"Default.png", NULL, NULL, &texture, NULL);
	}

	gDevice->CreateSamplerState(&sampDesc, &samplerState);
}
