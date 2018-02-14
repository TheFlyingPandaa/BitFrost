#ifndef MATRIAL_H
#define MATRIAL_H

#include <d3d11.h>
#include <DirectXMath.h>

#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
//#include <wchar.h>


struct mtl {
	std::wstring name;
	float ns, ni, d;
	int ilum;
	DirectX::XMFLOAT4A ka, kd, ks, ke;
	std::wstring textureName;
	mtl() {
		ka = DirectX::XMFLOAT4A();
		kd = DirectX::XMFLOAT4A();
		ks = DirectX::XMFLOAT4A();
	}
};

class Material
{
public:
	Material();
	Material(std::wstring mtlPath, std::wstring materialName = L"");
	
	~Material();

	mtl * getMtl();

private:
	mtl * material;

	std::wstring path;
	void loadMtl(std::wstring materialName = L"");
};

#endif // !MATRIAL_H
