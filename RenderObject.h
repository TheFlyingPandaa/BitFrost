#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "Mesh.h"
#include <string>

struct MatrixBuffert {
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX worldSpace;
};

class RenderObject
{
private:	
	Mesh * mesh;
	
	LPCWSTR textureFile;
	Texture ** tex;
	Texture ** normal;

	ID3D11Buffer ** vertexBuffer = nullptr;
	ID3D11Buffer * constantBuffer = nullptr;

	XMFLOAT3 * position = nullptr;

	MatrixBuffert matrixBuffer;

	float posX, posY, posZ;
	float scaleX, scaleY, scaleZ;

public:
	RenderObject();

	RenderObject(const char * meshDirr, LPCWSTR textureFile = NULL, const bool normalIn = false);
	RenderObject(const wchar_t * meshDirr, LPCWSTR textureFile = NULL, const bool normalIn = false);
	~RenderObject();

	void loadBuffer(ID3D11Device *& device);
	void draw(ID3D11DeviceContext *& deviceContext) const;

	void setMatrix(const XMMATRIX& view, const XMMATRIX& proj, float radsRot = NULL);

	void setPosition(float x, float y, float z);
	XMFLOAT3 getPosition() const;
	void setScale(float x, float y, float z);

	XMFLOAT3 * getPos();
};

#endif // !RENDER_OBJECT_H
