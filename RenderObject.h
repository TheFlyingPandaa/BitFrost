#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "Mesh.h"

struct MatrixBuffert {
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX worldSpace;
};

class RenderObject
{
private:	
	Mesh * mesh;


	LPCWSTR textureFile;
	Texture * tex;

	ID3D11Buffer * vertexBuffer = nullptr;
	ID3D11Buffer * constantBuffer = nullptr;

	MatrixBuffert matrixBuffer;

	float posX, posY, posZ;
	float scaleX, scaleY, scaleZ;

public:
	RenderObject();
	RenderObject(const wchar_t * meshDirr, LPCWSTR textureFile = NULL);
	~RenderObject();

	void loadBuffer(ID3D11Device *& device);
	void draw(ID3D11DeviceContext *& deviceContext) const;

	void setMatrix(const XMMATRIX& view, const XMMATRIX& proj);

	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
};

#endif // !RENDER_OBJECT_H
